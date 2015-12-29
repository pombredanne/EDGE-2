#pragma once

#include <string>

namespace eve
{
    class window
    {
        void *pimpl;
        window( const window & ) = delete;
        window &operator=( const window & ) = delete;

        public:

        size_t frames;
        int fbw, fbh;
        float w, h, parent_w, parent_h, fps;
        bool is_fullscreen, is_closing;

        std::string title;

        double t;
        float dt;

        explicit
        window( const std::string &_title = "eve", float proportional_size = 0.75f, bool is_fullscreen = false );

        bool is_blur() const;
        bool is_open() const;
        void toggle_fullscreen();
        void resize( int x, int y );
        void flush( float dt );
        void close();
    };

} // ns eve
