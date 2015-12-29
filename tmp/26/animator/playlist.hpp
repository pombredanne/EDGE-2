// - rlyeh // listening to amplifier - black rainbow

#define USE_ANIMATION_BLENDING 1

#if 0
Input data is very inconsistent
Format for animation list follows:

this is a comment
this is another comment...
frame: 102-123 animation #1
124~149: animation #2 (loop)
frame:130-159:animation #3 (LOOP)
#endif

#if 0

let animation A have frames a0, a1, a2, a3,...
let animation B have frames b0, b1, b2, b3,...
to blend between animation poses we:
@ frame #0: pose0 =                 a0 * 0.66 + b0 * 0.33
@ frame #1: pose1 = pose0 * 0.50 + (a1 * 0.50 + b1 * 0.50) * 0.50
@ frame #2: pose2 = pose1 * 0.50 + (a2 * 0.33 + b2 * 0.66) * 0.50
@ frame #3: pose3 = b3
#endif


#if 0
// api roadmap
mesh {
    transform tf;

    clear_geo() / import_geo() / export_geo(); // save() to save procedural data
    clear_anims() / import_anim() / export_anim();

    findSubmeshByName();
    commit() / purge();
    bind() / unbind();
    render()
    renderInstanced()
    attach( mesh* other, at bone ) / detach();

    std::vector<mesh*> group( subgeo = "*" );

    alpha(a, subgeo = "*");
    show(subgeo = "*");
    hide(subgeo = "*");

    bool collides( mesh* other );
    bool penetrates( mesh* other );
    bool contained( mesh* other );

    get_aabb();
    get_obb();

    // process events, triggers, collides, etc...
    enable(mask);
    disable(mask);

    // hacer que algunas partes de un objeto se muevan para donde uno quiera con IK
    // (por ejmplo, la cabeza mirando hacia un lado)
    IK(transform tf, subgeo = "*");

    bone {
        transform tf;
        unsigned ID;
        std::string name;
    }

    animations = animation[] {
        vector<bone> find("bonename*", frame);

        // position
        float offset(boneA);                   // max distance from origin
        float offset(boneA, frame);            // distance from origin, for a given frame
        float distance(boneA, boneB);          // max distance between bones
        float distance(boneA, boneB, frame);   // distance between bones, for a given frame
        // rotation
        float offset(boneA);                   // max distance from origin
        float offset(boneA, frame);            // distance from origin, for a given frame
        float distance(boneA, boneB);          // max distance between bones
        float distance(boneA, boneB, frame);   // distance between bones, for a given frame

        struct evolution {
            bone begin;
            bone end;
            bone max;
            bone min;
            bone avg;
        };
        evolution get_evolution("bonename*", frame_begin, frame_end); //

        play(anim);
        blend(anim);

        speed(factor=-x1,curve=linear);
        speed(time=s(-3),curve=linear);
        pause()->push_back(speed(0)) / resume();

        events() = {frame, []{ code; } };
    }
}
#endif

#include <string>
#include <fstream>
#include <vector>
#include <wire/wire.hpp>
#include <apathy/apathy.hpp>
#include <bourne/bourne.hpp>
#include <functional>
#include <map>

#include "variant.hpp"
#include "events.hpp"

struct animation {
    std::string name;            // description
    unsigned begin = 0, end = 0; // key frames
    bool loopable = 0;           // able to loop
    unsigned id = 0;             // identifier
    unsigned blend_from = ~0u;   // range [blend_from,eof] from which another animation can be blended into

    ::events events;

    float progress( unsigned f ) const {
        return begin >= end ? 100.f : 100.f * (frame(f) - begin) / ( end - begin );
    }

    unsigned frames() const {
        return 1 + ( end - begin );
    }

    unsigned frame( unsigned f ) const {
        if( begin >= end ) {
            return begin;
        }
        unsigned iterations = f / frames();
        return !loopable && iterations >= 1 ? end : begin + ( f % frames() );
    }
    unsigned operator[]( unsigned f ) const {
        return frame( f );
    }

    bool has_begun( unsigned f ) const {
        return frame(f) == begin;
    }
    bool has_finished( unsigned f ) const {
        return frame(f) == end;
    }
    bool has_looped( unsigned f ) const {
        unsigned iterations = f / frames();
        return iterations >= 1 && has_begun( f );
    }

    void execute( unsigned f ) const {
        events.execute( frame(f) );
    }

};

BOURNE_DEFINE( animation &it, (it.name, it.begin, it.end, it.loopable, it.id, it.blend_from) );

#include <algorithm>
#include <random>

template<typename T>
T shuffle( const T &list ) {
    T copy( list );
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(copy.begin(), copy.end(), generator);
    return copy;
}

template<typename T>
T truncate( int n, const T &list ) {
    T copy( list );
    return copy.resize(n), copy;
}

template<typename T>
T any( int n, const T &list ) {
    return truncate( 1, shuffle( list ) );
}

// reverse a,b,c -> c,b,a
// pingpong a,b,c -> a,b,c,b,a
// pingpongping a,b,c -> pingpong(a) + &ping[1] -> a,b,c,b,a,b,c

struct list {
    std::vector<unsigned> playlist;                        // list of animations
    unsigned selected = 0;                             // selected animation

    std::vector<animation> pAnimations;      // storage for animations; first is invalid by default

    bool operator ==( const list &other ) const {
        return (selected == other.selected && playlist == other.playlist );
    }
    bool operator !=( const list &other ) const {
        return !operator==( other );
    }
};


struct controller : public list
{
    std::map<std::string,unsigned> names;   // string to id table conversion. empty name is invalid
    std::map<unsigned,std::string> ids;     // id to string table conversion. id #0 is invalid
    std::vector<animation> animations;      // storage for animations; first is invalid by default

    unsigned frame = 0;                     // current frame in selected animation

    unsigned rate = 30;                     // framerate in hz
    std::vector<float> factor;              // speed factor

    controller()
    {}

    void hard_reset() {
        ids[ 0 ] = "";
        names[ "" ] = 0;
        animations.push_back( animation() );

        playlist = {0};
        selected = 0; // same than .reset();

        factor = {1};
    }

    bool load( const char *ptr, size_t len ) {
        if( !(ptr && len) ) {
            return false;
        }

        // clear and add first dummy/invalid animation
        *this = controller();
        hard_reset();

        // parse animation format (see top of file)
        wire::strings lines = wire::string( ptr, len ).tokenize("\f\b\v\r\n");
        for( auto &line : lines ) {
            wire::strings words = line.tokenize("()[]:-~;:\t ");
            if( words.size() < 4 ) continue;

            wire::string label = words.front(); if( label != "frame" && label.as<int>() > 0 ) label = "frame"; else words.pop_front();
            wire::string begin = words.front(); words.pop_front(); while( begin.size() && begin.front() == '0' ) begin.pop_front();
            wire::string end = words.front(); words.pop_front(); while( end.size() && end.front() == '0' ) end.pop_front();
            wire::string name = words.str("\1 "); while( name.size() && name.back() == ' ' ) name.pop_back();
            name = name.replace(" ", "-").lowercase();
            bool loopable = (name.find("-loop") != std::string::npos);

            // insert
            animation anim;
            anim.id   = animations.size();
            anim.name = name;
            anim.loopable = loopable;
            anim.begin = begin.as<unsigned>();
            anim.end   = end.as<unsigned>();

            if( label != "frame" ) continue;
            if( anim.end < anim.begin ) continue;

            ids[ anim.id ] = anim.name;
            names[ anim.name ] = anim.id;
            animations.push_back( anim );
        }

        ids[ 0 ] = "";
        names[ "" ] = 0;
        animations[0] = animation();

        return true;
    }

    bool load( const std::string &pathfile ) {
        std::string content = apathy::file(pathfile).read();
        return load( content.c_str(), content.size() );
    }

    template<typename T>
    bool push( const T& anim, unsigned instances = 1 ) {
        auto id = get_id(anim);
        unsigned found = 0;
        for( unsigned i = 0, cursor = selected + 1; cursor < playlist.size() && i < instances; ++i ) {
            if( playlist[ cursor + i ] == id ) {
                found++;
            }
        }
        found = std::max( 0, int(instances - found) );
        for( unsigned i = 0; i < found; ++i ) {
            playlist.push_back( id );
        }
        return true;
    }
    bool pop() {
        if( playlist.size() ) {
            playlist.pop_back();
            return true;
        }
        return false;
    }

    bool stop() {
        playlist = {0};
        selected = 0;
        return true;
    }

    // enhanced api {
    std::vector<unsigned> filter( const std::vector<unsigned> &pl ) {
        std::vector<unsigned> copy;
        for( auto &anim : pl ) {
            if( anim > 0 && anim < animations.size() ) {
                copy.push_back( anim );
            }
        }
        return copy;
    }
    bool play_seq( const std::vector<unsigned> &new_playlist ) {
        auto filtered = filter( new_playlist );
        if( filtered.empty() ) {
            return false;
        }
        if( playlist != filtered || ( !get_animation().loopable && has_playlist_finished() ) ) {
            factor = {1};
            frame = 0;
            selected = 0;
            playlist = filtered;
            return true;
        } else {
            factor = {1};
            playlist = filtered;
            return false;
        }
    }
    template<typename T1>
    bool play( const T1 &a ) {
        return play_seq( { get_id(a) } );
    }
    template<typename T1, typename T2>
    bool play( const T1 &a, const T2 &b ) {
        return play_seq( { get_id(a), get_id(b) } );
    }
    template<typename T1, typename T2, typename T3>
    bool play( const T1 &a, const T2 &b, const T3 &c ) {
        return play_seq( { get_id(a), get_id(b), get_id(c) } );
    }
    template<typename T1, typename T2>
    bool play_any( const T1 &a, const T2 &b ) {
        return play_seq( truncate( 1, shuffle( { get_id(a), get_id(b) } ) ) );
    }
    template<typename T1, typename T2, typename T3>
    bool play_any( const T1 &a, const T2 &b, const T3 &c ) {
        return play_seq( truncate( 1, shuffle( { get_id(a), get_id(b), get_id(c) } ) ) );
    }
    // } enhanced api

    const animation &get_animation() const {
        return animations[ playlist[ selected ] ];
    }
    template<typename T>
    const animation &get_animation( const T &anim ) const {
        return animations[ get_id(anim) ];
    }

    template<typename T>
    bool is_selected( const T &anim ) const {
        return playlist[ selected ] == get_id(anim);
    }
    template<typename T, typename T2>
    bool is_selected( const T &anim, const T2 &anim2 ) const {
        return is_selected(anim) || is_selected(anim2);
    }
    template<typename T, typename T2, typename T3>
    bool is_selected( const T &anim, const T2 &anim2, const T3 &anim3 ) const {
        return is_selected(anim, anim2) || is_selected(anim3);
    }

    template<typename T, typename FN>
    void insert_event( const T &anim, unsigned frame, unsigned type, const FN &fn ) {
        animations[ get_id(anim) ].events[ frame ].push_back( { type, fn } );
    }


    unsigned get_frame( unsigned f = 0 ) const {
        return get_animation().frame( frame + f );
    }
    // get pose at given frame
    unsigned operator[]( unsigned f ) const {
        return get_animation().frame( frame + f );
    }

    float progress() const {
        return get_animation().progress(frame);
    }

    bool has_animation_looped() const {
        return get_animation().has_looped(frame);
    }
    bool has_animation_finished() const {
        return get_animation().has_finished(frame);
    }
    bool has_animation_played() const {
        return has_animation_finished() || has_animation_looped();
    }
    bool is_animation_playing() const {
        return !has_animation_played();
    }

    bool has_playlist_looped() const {
        return false;
    }
    bool has_playlist_finished() const {
        return playlist.size() && selected == playlist.size() - 1 && has_animation_finished();
    }
    bool has_playlist_played() const {
        return playlist.size() && selected == playlist.size() - 1 && has_animation_played();
    }
    bool is_playlist_playing() const {
        return !has_playlist_played();
    }

    template<typename T>
    bool has_animation_looped( const T &anim ) const {
        return has_animation_looped() && is_selected( anim );
    }

    template<typename T>
    bool is_playing( const T &anim ) const {
        return is_animation_playing() && is_selected( anim );
    }
    template<typename T, typename T2>
    bool is_playing( const T &anim, const T2 &anim2 ) const {
        return is_animation_playing() && is_selected( anim, anim2 );
    }
    template<typename T, typename T2, typename T3>
    bool is_playing( const T &anim, const T2 &anim2, const T3 &anim3 ) const {
        return is_animation_playing() && is_selected( anim, anim2, anim3 );
    }

    template<typename T>
    bool has_played( const T &anim ) const {
        return has_animation_played() && is_selected( anim );
    }
    template<typename T, typename T2>
    bool has_played( const T &anim, const T2 &anim2 ) const {
        return has_animation_played() && is_selected( anim, anim2 );
    }
    template<typename T, typename T2, typename T3>
    bool has_played( const T &anim, const T2 &anim2, const T3 &anim3 ) const {
        return has_animation_played() && is_selected( anim, anim2, anim3 );
    }

    std::string str() const {
        return wire::string("[\1%] \2 \3/\4", int(progress()), get_animation().name, get_frame(), get_animation().end);
    }

    void push_factor( float f ) {
        factor.push_back( f );
    }
    void pop_factor() {
        if( factor.size() > 1 ) {
            factor.pop_back();
        }
    }

    void pause( bool on = 1 ) {
        if( on ) {
            if( factor.back() != 0 ) {
                push_factor( 0 );
            }
        } else {
            if( factor.back() == 0 ) {
                pop_factor();
            }
        }
    }
    void resume() {
        pause( 0 );
    }


    void update() {
        const auto &anim = get_animation();
        // exec events, if any
        anim.execute(frame);
        // check status
        if( !has_animation_finished() ) {
            frame = floor( frame + factor.back() );
        } else {
            if( anim.loopable ) {
                frame = 0; //-= get_animation().frames();
                return;
            }
            pop_factor();
            // change to next animation
            if( !has_playlist_finished() ) {
                selected++;
                frame = 0;
            } else {
                // keep anim & pose
                selected = selected;
                frame = frame;
            }
        }
    }

    // extras {
    unsigned get_id( const unsigned &anim ) const {
        return anim;
    }
    unsigned get_id( const std::string &anim ) const {
        auto found = names.find( anim );
        if( found != names.end() ) {
            return found->second;
        }
        return 0;
    }
    // }
};

BOURNE_DEFINE( controller &it, (it.animations, it.names, it.frame, it.playlist, it.selected, it.rate) );

struct blend_controller : public controller {
    controller prev;

    enum { FRAMES_TO_BLEND = 3 }; // 3x30hz = ~100ms ; must be odd number! }
    int frame_to_blend = -1;

    std::string str() const {
        const controller &self = *this;
        if( frame_to_blend >= 0 )
        return wire::string("\1 / pass #\2 / blend with: \3", self.str(), frame_to_blend, prev.str());
        else
        return wire::string("\1", self.str());
    }

    template<typename T>
    bool load( const T &anim ) {
        controller &self = *this;
        bool ok = self.load( anim );
        prev = self;
        return ok;
    }

    template<typename T>
    bool play( const T &anim ) {
        controller &self = *this;
        controller next = self;
        bool ok = next.play( anim );
        if( ok ) { prev = self; self = next; frame_to_blend = FRAMES_TO_BLEND; }
        return ok;
    }
    template<typename T, typename T2>
    bool play( const T &anim, const T2 &anim2 ) {
        controller &self = *this;
        controller next = self;
        bool ok = next.play( anim, anim2 );
        if( ok ) { prev = self; self = next; frame_to_blend = FRAMES_TO_BLEND; }
        return ok;
    }
    template<typename T, typename T2, typename T3>
    bool play( const T &anim, const T2 &anim2, const T3 &anim3 ) {
        controller &self = *this;
        controller next = self;
        bool !ok = next.play( anim, anim2, anim3 );
        if( ok ) { prev = self; self = next; frame_to_blend = FRAMES_TO_BLEND; }
        return ok;
    }

    void update() {
        controller &self = *this;

        self.update();

        if( frame_to_blend >= 0 ) {
            prev.update();
            --frame_to_blend;
        }
    }
};

BOURNE_DEFINE( blend_controller &it, ((controller &)it, it.prev, it.frame_to_blend) );

