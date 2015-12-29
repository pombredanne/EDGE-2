

#pragma once

#include <string>
#include <vector>
#include <limits.h>
#include <sstream>
#include <iostream>
#include <functional>

struct item;

struct attribute
{
    size_t      mask = 0;  // message mask. what messages does this attrib handle?
    std::string name;
    std::string factory;   // string to build attribute from scratch (serialization)

    bool   on = 1;         // global on/off
    float  add = 0;        // signed!
    float  mul = 1;        // >=0!
    float  min = FLT_MIN;  // clamp range
    float  max = FLT_MAX;  // clamp range

    std::function< void ( ::item &item, int message ) > handle;

    float compute( float value ) const {
        if( on ) {
            value = ( value + add ) * mul;
            /**/ if( value < min ) return min;
            else if( value > max ) return max;
        }
        return value;
    }

    float operator()( float value = 0.f ) const {
        return compute( value );
    }
    bool operator <( const attribute &a ) const {
        return name < a.name;
    }

    template<typename stream>
    inline friend stream &operator<<( stream &os, const attribute &self ) {
        if( self.name.size() ) {
            os << self.name << ":";
        }
        if( self.on ) {
            int mul = int( self.mul * 100 ) - 100;
            os << ( self.add > 0 ? "+" : "") << self.add;
            if( mul ) os << ' ' << ( mul > 0 ? "+" : "") << mul << "%";
            if( self.min > FLT_MIN ) os << " ["   << self.min << "..]";
            if( self.max < FLT_MAX ) os << " [.." << self.max <<   "]";
        }
        else {
            os << "off";
        }
        return os;
    }

/*
private:
    struct naked { bool on; float add, mul, min, max; };
    std::vector< naked > stack;
public:

    size_t size() const {
        return stack.size();
    }

    void push() {
        stack.push_back( naked() );
        naked &n = stack.back();
        n.on  = on;
        n.add = add;
        n.mul = mul;
        n.min = min;
        n.max = max;
    }

    void pop() {
        naked &n = stack.back();
        on  = n.on;
        add = n.add;
        mul = n.mul;
        min = n.min;
        max = n.max;
        stack.pop_back();
    }
*/
};

#include "warn.hpp"

namespace {
    float value( const ::attribute &attribute, float value ) {
        return attribute.compute( value );
    }
    bool verify( const ::attribute &attribute ) {
        if( attribute.name.empty() )
            return warn("attribute invalid name ", attribute.name ), false;

        if( attribute.mul < 0 )
            return warn( "attribute invalid mul ", attribute.name ), false;

        return true;
    }
    std::string debug( const ::attribute &attribute ) {
        std::stringstream ss;
        return (ss << attribute), ss.str();
    }
}

#include <map>

struct item //: std::vector< attribute >
{
    std::string name;           // 'bow'
    std::string plural;         // 'bows'
    std::string description;    // 'use to kill evil crows'
    std::string url2d;          // image url
    std::string url3d;          // mesh url
    std::string factory;        // string to build object from scratch (serialization)

    std::map< std::string, attribute > attributes;
    //std::vector< attribute > attributes;

    item()
    {}

    explicit
    item(   const attribute &a,
            const attribute &b = attribute(),
            const attribute &c = attribute(),
            const attribute &d = attribute(),
            const attribute &e = attribute(),
            const attribute &f = attribute(),
            const attribute &g = attribute(),
            const attribute &h = attribute() )
    {
        if( a.name.size() ) insert( a );
        if( b.name.size() ) insert( b );
        if( c.name.size() ) insert( c );
        if( d.name.size() ) insert( d );
        if( e.name.size() ) insert( e );
        if( f.name.size() ) insert( f );
        if( g.name.size() ) insert( g );
        if( h.name.size() ) insert( h );
    }

    bool insert( const attribute &a )
    {
        if( a.name.empty() )
            return false;

        auto it = attributes.find( a.name );
        bool found = ( it != attributes.end() );

        if( found )
            return false;

        attributes.insert( std::pair<std::string, attribute>( a.name, a ) );
        return true;
    }

    bool erase( const attribute &a )
    {
        if( a.name.empty() )
            return false;

        auto it = attributes.find( a.name );
        bool found = ( it != attributes.end() );

        if( !found )
            return false;

        attributes.erase( it );
        return true;
    }

    void apply( int message ) {
        for( auto &in : attributes ) {
            if( in.second.handle ) {
                in.second.handle( *this, message );
            }
        }
    }

    std::string use()
    {
        std::cout << "dunno how to use!" << std::endl;
    }

    template<typename stream>
    inline friend stream &operator<<( stream &os, const item &self ) {
        return os << '"' << self.name << '"', os;
    }
};

namespace {

    int sum( const std::string &name, const ::item &item )
    {
        int value = 0;
        for( auto &in : item.attributes ) {
            auto &attrib = in.second;
            if( attrib.name == name )
                value = attrib.compute( value );
        }
        return value;
    }
    void equip( ::attribute &attrib, const ::item &item )
    {
        attrib.add += sum( attrib.name, item );
    }
    void unequip( ::attribute &attrib, const ::item &item )
    {
        attrib.add -= sum( attrib.name, item );
    }

}


struct inventory
{
    std::map< std::string, item > items;

    void pickup( const ::item &item )
    {
        if( items.find( item.name ) == items.end() )
            items.insert( std::pair<std::string,::item>( item.name, item) );
    }

    void drop( const ::item &item )
    {
        auto it = items.find( item.name );
        if( it != items.end() )
            items.erase( it );
    }

    void apply( int message ) // ie, wet
    {
        for( auto &in : items )
            in.second.apply( message );
    }
};
