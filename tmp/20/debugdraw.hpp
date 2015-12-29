#pragma once
#include <vector>
#include <string>

void drawstring(float x, float y, const std::string &s);

void debugclear();
void debugstring(const std::string &s);

 int debugmenu( const std::vector<std::string> &lines );
