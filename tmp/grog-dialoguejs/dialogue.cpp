/* gnu.txt
0 Hi! Welcome to OSSland! Is this your first visit? [1,2]
1 Yes, I just arrived -> 3
2 No, I've been here before -> 4
3 Awesome! Have fun! -> 5
4 Hey, welcome back -> 5
5 Do you want to be my friend? [6,7,8,9]
6 Yes -> 10
7 No -> 11
8 Maybe -> 12
9 Not sure -> 12
10 Great! -> 13
11 Awwww -> 13
12 Please! -> 5
13 Oh hi -> 13
banana: I hate bananas
*/

#include <string>
#include <map>
#include <functional>
#include <sstream>
#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include <fstream>

#include "wire/wire.hpp"

struct dialogue {

    struct guide { // union would fit better
        std::string text;
        std::string script;
        std::map<std::string, std::string> choices;
        bool eof = 0;

        template<typename T>
        inline friend T &operator <<( T &os, const guide &l ) {
            if( l.eof ) {
                os << "eof|";
            }
            if( !l.script.empty() ) {
                os << "script:" << l.script;
                os << "|";
            }
            if( !l.choices.empty() ) {
                os << "choices:";
                for( auto &kv : l.choices ) {
                    os << kv.first << "->" << kv.second << ",";
                }
                os << "|";
            }
            if( !l.text.empty() ) {
                os << "text: '" << l.text << "'";
            }
            return os;
        }
    };

    using guides = std::deque< guide >;
    using guideline = std::map< std::string, guides >;

    guideline plot;

    int state = 0, substate = 0, initialized = 0;

    std::function< void() > on_begin;
    std::function< void() > on_end;
    std::function< void(const std::map<std::string,std::string> &choices) > on_choice;
    std::function< void(const char *) > on_text;
    std::function< void(const char *) > on_script;

    bool load( const std::string &fname ) {
        return load( std::ifstream( fname.c_str(), std::ios::binary ) );
    }

    bool load( std::istream &is ) {
        plot.clear();

        std::stringstream ss;
        ss << is.rdbuf();
        if( ss.good() && is.good() ) {
            auto lines = wire::string( ss.str() ).tokenize("\r\n");
            for( auto &line : lines ) {
                // comment?
                if( line.starts_with("--") ) {
                    continue;
                }
                // text | texts... -> action
                bool has_action = line.left_of("->") != line;
                std::string action;
                if( has_action ) {
                   action = line.right_of("->").trim();
                   action = &action[2];
                   action = wire::string( action ).trim();
                }
                auto text = line.left_of("->").tokenize("|");
                for( auto &t : text ) {
                    t = t.trim();
                }
                // key
                auto key = text[0].tokenize(":")[0];
                text[0] = wire::string( text[0].substr( key.size() + 1 ) ).trim();
                // inscribe texts
                auto &guides = plot[ key.trim() ];
                for( auto &t : text ) {
                    if( !t.empty() ) {
                        guides.push_back( guide() );
                        switch( t[0] ) {
                            break; default : guides.back().text = t;
                            break; case '<': guides.back().script = t;
                        }
                    }
                }
                // inscribe action
                if( action.empty() ) action = "!";
                auto parse = []( const std::string &text ) {
                    std::map<std::string, std::string> out;
                    auto wr = wire::string( text );
                    if( wr.back() == '?' || wr.back() == '!' ) wr.pop_back();
                    for( auto &it : wr.tokenize( " " ) ) {
                        out[ it ];
                    }
                    return out;
                };
                if( !action.empty() ) {
                    guides.push_back( guide() );
                    guides.back().choices = parse( action );
                    guides.back().eof = ( action.back() == '!' && !guides.back().eof );
                }
            }

            // resolve choices
            for( auto &it : plot ) {
                for( auto &jt : it.second ) {
                    for( auto &choice : jt.choices ) {
                        std::string number = choice.first;
                        choice.second = plot[ number ].begin()->text;
                    }
                }
            }

            return true;
        }

        return false;
    }

    void debug( std::ostream &out = std::cout ) const {
        for( auto &kv : plot ) {
            out << kv.first << std::endl;
            for( auto &v : kv.second ) {
                out << "L" << __LINE__ << "\t" << v << std::endl;
            }
        }
    }

    guideline::const_iterator get_state() const {
        auto it = plot.begin();
        return std::advance( it, state ), it;
    }

    guides::const_iterator get_substate() const {
        auto it = get_state();
        auto jt = it->second.begin();
        return std::advance( jt, substate ), jt;
    }

    bool jump( const std::string &text ) {
        if( !text.empty() ) {
            auto find = plot.find( text );
            auto found = find != plot.end();
            if( found ) {
                // topic
                state = std::distance( plot.begin(), find );
                substate = 0;
                // std::cout << "jump to " << state << std::endl;
                return true;
            }
        }
        return false;
    }

    void interact( const std::string &text ) {
        // is it registered topic (word) or jump state (number) ?
        jump(text);

        if( 1 ) {

            if( !initialized ) {
                initialized = 1;
                if( on_begin ) {
                    on_begin();
                }
            }

            auto it = get_state();
            auto jt = get_substate();

            if( !jt->script.empty() ) {
                if( on_script ) {
                    on_script( jt->script.c_str() );
                }
            }

            if( !jt->text.empty() ) {
                if( on_text ) {
                    on_text( jt->text.c_str() );
                }
            }

            if( jt->eof ) {
                if( on_end ) {
                    on_end();
                }
                initialized = 0;
            }

            // advance if possible, else restart state
            if( (++substate) >= it->second.size() ) {
                --substate;
            }

            if( !jt->choices.empty() ) {
                if( jt->choices.size() == 1 ) {
                    jump( jt->choices.begin()->first );
                }
                if( on_choice && jt->choices.size() > 1 ) {
                    on_choice( jt->choices );
                }
            }

            // std::cout << state << ',' << substate << ',' << *jt << std::endl;
        }
    }
};

#include <fstream>

int main() {

    auto prompt = []( const std::string &prompt ) {
        std::cout << "L" << __LINE__ << "\t" << prompt;
        std::string input;
        std::getline(std::cin, input);
        return input;
    };

    dialogue gnu;

    gnu.load( "gnu.txt" );
    gnu.debug();

    std::string status = "wandering";

    gnu.on_begin = [&] {
        status = "talking";
        std::cout << "L" << __LINE__ << "\t" << "-- begin of conversation" << std::endl;
    };
    gnu.on_end = [&] {
        status = "wandering";
        std::cout << "L" << __LINE__ << "\t" << "-- end of conversation" << std::endl;
    };
    gnu.on_script = [&]( const char *text ) {
        std::cout << "L" << __LINE__ << "\t" << "adding topics: " << text << std::endl;
    };
    gnu.on_text = [&]( const char *text ) {
        std::cout << "L" << __LINE__ << "\t" << text << std::endl;
    };
    gnu.on_choice = [&]( const std::map<std::string, std::string> &items ) {
        for( auto &it : items ) {
            std::cout << "L" << __LINE__ << "\t" << it.first << "\t" << it.second << std::endl;
        }
    };

    for(;;) {
        auto text = prompt(status + ">");
        if( text == "debug" ) {
            gnu.debug();
            continue;
        }
        if( status == "talking" || (status == "wandering" && text == "talk") ) {
            gnu.interact( text );
        }
    }

}

