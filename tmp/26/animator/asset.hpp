#pragma once
#include <map>
#include <apathy/apathy.hpp>
#include <wire/wire.hpp>

struct asset {
    // location folder
    std::string dir;

    // resources in asset
    std::string img, png, tga, jpg, webp, svg;
    std::string geo, fbx, dae, nff;
    std::string txt, json;
    
    // user defined data
    std::string data;

    bool load( const std::string &path ) {
        *this = asset();
        dir = wire::string( path ).lowercase().replace("\\", "/");
        if( apathy::file(dir).is_file() ) {
            dir = dir.substr( 0, dir.find_last_of('/') + 1 );
        }
        if( dir.size() ) {
            apathy::pushd();
            apathy::chdir(dir);
            auto set = apathy::ls();
            for( auto &file : set ) {
                wire::string name = wire::string( file ).lowercase();
                if( name.ends_with(".fbx") ) geo = fbx = name;
                if( name.ends_with(".dae") ) geo = dae = name;
                if( name.ends_with(".nff") ) geo = nff = name;

                if( name.ends_with(".svg") ) img = svg = name;
                if( name.ends_with(".png") ) img = png = name;
                if( name.ends_with(".tga") ) img = tga = name;
                if( name.ends_with(".jpeg") || name.ends_with(".jpg") ) img = jpg = name;
                if( name.ends_with(".webp") || name.ends_with(".wbp") ) img = webp = name;

                if( name.ends_with(".txt") ) txt = name;
            }
            apathy::popd();
        }
        return img.size() || geo.size() || txt.size();
    }
};

char *basedir();
