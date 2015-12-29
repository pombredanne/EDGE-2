// Simple trophy system. MIT licensed.
// - rlyeh ~~ listening to AWOLNATION / Sail

// @todo:
// trophy.uuid
// trophy.hidden (hidden if locked?)
// trophy.depends[] uuids that must be unlocked before this

#pragma once

#include <string>
#include <list>
#include <functional>

struct trophy
{
    unsigned date = 0;                // date of achievement; 0 if not achieved
    short counter = 0, goal = 0;      // score/task progress

    std::string title;          // artistic name
    std::string description;    // description hints (if any)
    std::string image;          // image URL (if any)
    std::string factory;        // plain text data to construct the object from scratch

    // external function that handles logic.
    // returns true if game status meet trophy expectations.
    std::function<bool()> check;

    // sorting operator (if case your app needs it)
    const bool operator<( const trophy &other ) const { return title < other.title; }

    // pretty print
    std::string debug() const;
};

struct trophies
{
    std::list< trophy > todo;   // pending achievements (~quest)
    std::list< trophy > done;   // finished achievements
    std::list< trophy > show;   // recent achievements

    void update();

    // pretty print
    std::string debug() const;
};
