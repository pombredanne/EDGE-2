// [ref] http://www.irontowerstudio.com/forum/index.php?topic=414.0

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

#include <cassert>

#include <algorithm>
#include <deque>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <iostream>

#include "bourne/bourne.hpp"

struct line {
    int id;
    std::string text;
    std::vector<int> choices;

    bool operator<( const line &other ) const {
        return id < other.id;
    }
    bool operator==( const line &other ) const {
        return id == other.id && text == other.text && choices == other.choices;
    }
};

BOURNE_DEFINE( line &it, (it.id, it.text, it.choices) );

struct choice {
    int id;
    std::string text;

    bool operator<( const choice &other ) const {
        return id < other.id;
    }
    bool operator==( const choice &other ) const {
        return id == other.id && text == other.text;
    }
};

BOURNE_DEFINE( choice &it, (it.id, it.text) );

struct chat {
    std::vector< line > lines;
    std::vector< choice > choices;
};

BOURNE_DEFINE( chat &it, (it.lines, it.choices) );

struct fact {
    std::string id;
    std::string text;

    bool operator<( const fact &other ) const {
        return id < other.id;
    }
    bool operator==( const fact &other ) const {
        return id == other.id && text == other.text;
    }
};

BOURNE_DEFINE( fact &it, (it.id, it.text) );

struct actor {
    std::string id;
    std::set< line > lines;
    std::set< fact > facts;
    std::set< fact > stages;

    line get_line( int l ) const {
        for( auto &it : lines ) {
            if( it.id == l )
                return it;
        }
        return line();
    };

    // cancel
    // reset
};

BOURNE_DEFINE( actor &it, (it.id, it.lines, it.facts, it.stages) );

namespace grog {
    namespace api {

        // a conversation is a serie of 1 or more turns between two characters
        // a character can say 1 or more sentences in each turn, until finished
        // N(PC), P(LAYER)
        // N [ P [ N [ P [ N ... ]]]]
        // P [ N [ P [ N [ P ... ]]]]
        // samples:
        // N: "Hey foraigner!"
        // P-N: "P: bananas", "N: I hate bananas"
        // N-P-N: "N: Welcome! First visit?", "P: {Yep, Nop}", "N: ok then!"
        // "N: He used to...|*pause*|ahem, he used to live here"

        using actor = int;
        using file = int;
        using response = int;
        using topic = int;
        using json = int;
        using player = int;

        bool load( actor id, file );
        // Note that dialogue state is maintained for player-actor combinations,
        // so if multiple players converse with a NPC, they can each have a different
        // state within the conversation with that NPC.
        json interact( actor, player );
        json interact( actor, player, response id );
        json interact( actor, player, topic text );
    }

    std::string example() {
        actor gnu = {
            "",
            { // lines
                #define NPC "npc: "
                #define PLY "ply: "
                #define SEP "|"
                #define EOC "\x1b"
                #define ESC SEP EOC
                { 0, NPC "Hi! Welcome to OSSland!" SEP "Is this your first visit?", {1,2} },
                { 1, PLY "Yes, I just arrived", {3} },
                { 2, PLY "No, I've been here before", {4} },
                { 3, NPC "Awesome! Have fun!" ESC, {5} },
                { 4, NPC "Hey, welcome back" ESC, {5} },

                { 5, NPC "Do you want to be my friend?", {6,7,8,9} },
                { 6, PLY "Yes", {10} },
                { 7, PLY "No", {11} },
                { 8, PLY "Maybe", {12} },
                { 9, PLY "Not sure", {12} },
                { 10, NPC "Great!" ESC, {14} },
                { 11, NPC "Awwww" ESC, {13} },
                { 12, NPC "Please!" ESC, {5} },

                // {X, PLY "Do you like bananas?", {} },
                // {X, NPC "Meh! I hate #bananas", {} },
                // {X, PLY "Why do you hate bananas?", {} },
                // {X, NPC "I once landed in an island full of bananas..." SEP "I got sick of them" ESC, { ply.remember("bananas", this->npc, this->line, this->place, this->moment); } },

                { 13, NPC "Oh hi" ESC, {13} },
                { 14, NPC "hello friend!" SEP "have you heard about the monster in the forest?" SEP "scary!" ESC, {14} },
            },
            {   // facts, additions after previous knowledge
                { "#banana", NPC "Meh! I hate bananas" ESC },
                { "#banana+", NPC "I once landed in an island full of bananas..." SEP "I got sick of them" ESC }, // tell me more about bananas
            },
            {   // stages
                { "#base", "gnu1.txt" },
                { "#secretkey", "gnu2.txt" }
            }
        };

        return bourne::to_json( gnu );
    }

    actor &get_actor( const std::string &name ) {
        static std::map< std::string, actor > actors;
        return actors[ name ];
    }

    bool load( const std::string &name, const std::string &res ) {
        actor &act = get_actor( name );
        act = actor();
        if( !bourne::from_json( act, res ) ) {
            return false;
        }
        act.id = name;
        return true;
    }

    chat interact( const std::string &A, const std::string &B, const std::string &about_ = std::string() ) {

        actor &a = get_actor(A), &b = get_actor(B);
        if( A > B ) std::swap( a, b );

        static std::map< std::string, line > states;

        auto interact_ = [&]( line &l, const std::string &who, const std::string &about ) -> bool {

            auto get_line = [&]( int id ) {
                return a.get_line( id );
            };

            auto reset = [&]( const std::string &who ) {
                states[ who ] = get_line(0);
            };

            auto alive = [&]( const std::string &who ) {
                auto &state = states[ who ];
                bool end_of_chat = ( state.text.size() && state.text.back() == EOC[0] );
                return end_of_chat ? false : true;
            };

            auto choose = [&]( const std::string &who, int choice ) {
                auto &state = states[ who ];
                if( choice >= state.choices.size() ) choice = 0;
                state = get_line( state.choices[ choice ] );
                return alive( who );
            };

            auto next = [&]( const std::string &who ) {
                return choose( who, 0 );
            };

            auto is_answer = [&]( const std::string &who, const std::string &choicestr ) {
                auto &state = states[ who ];
                int it = 0;
                for( auto &choice : state.choices ) {
                    std::stringstream ss;
                    ss << choice;
                    if( ss.str() == choicestr ) {
                        return it;
                    }
                    ++it;
                }
                return -1;
            };

            auto ask = [&]( const std::string &who, const std::string &about ) -> std::string {
                unsigned level = 0;
                for( auto &f : a.facts ) {
                    if( about == f.id ) {
                        return f.text;
                    }
                }
                return std::string();
            };

            if( states.find(who) == states.end() ) {
                auto &state = states[ who ];
                reset( who );
            }

            auto &state = states[ who ];

            if( !about.empty() ) {
                auto text = ask( who, about );
                if( !text.empty() ) {
                    l = { 0, text + SEP + EOC, {state.id} };
                    return true;
                }
                int choice = is_answer( who, about );
                if( choice < 0 ) {
                    l = { 0, EOC, {0} };
                    return false;
                }
                choose( who, choice );
            }
            l = state;
            bool eoc = !alive( who );
            if( l.choices.size() <= 1 ) {
                next( who );
            }
            return eoc;
        };

        chat ls;

        std::string about = about_;
        bool eoc;
        do {
            ls.lines.push_back( line() );
            eoc = interact_( ls.lines.back(), b.id, about );
            about.clear();
            ls.choices.clear();
            if( ls.lines.back().choices.size() > 1 ) {
                for( auto &it : ls.lines.back().choices ) {
                    choice pair { a.get_line( it ).id, a.get_line( it ).text };
                    ls.choices.push_back( pair );
                }
            }
        } while( ls.lines.back().choices.size() <= 1 && !eoc );

        return ls;
    }
}

#include <iostream>
#include <sstream>

int main() {
    auto prompt = []( const std::string &prompt ) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        return input;
    };
    auto split = []( const std::string &line, char delim ) {
        std::vector<std::string> tokens;
        std::istringstream split(line);
        for( std::string each; std::getline(split, each, delim); ) {
            if( !each.empty() ) {
                tokens.push_back(each);
            }
        }
        return tokens;
    };

    grog::load( "me", "" );
    grog::load( "gnu", grog::example() );

    std::cout << bourne::to_json(grog::get_actor("gnu")) << std::endl;

    for( ;; ) {
        std::string action = prompt("wandering>");
        if( action == "talk" ) {
            std::string about;
            for( ;; ) {

                chat cht = grog::interact( "gnu", "me", about );
                for( auto &line : cht.lines ) {
                    for( auto &sentence : split( line.text, '|') ) {
                        if( sentence != EOC ) {
                            std::cout << sentence << std::endl;
                        }
                    }
                }

                if( cht.choices.empty() ) {
                    std::cout << "**end of chat**" << std::endl;
                    break;
                } else {
                    for( auto &alt : cht.choices ) {
                        std::cout << alt.id << ") " << alt.text << std::endl;
                    }
                    about = prompt("talking>");
                }
            }
        }
    }

}
