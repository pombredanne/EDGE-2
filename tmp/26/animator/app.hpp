#pragma once

void perspective(float fov, float aspect, float znear, float zfar);
void orthogonal(float fov, float aspect, float znear, float zfar);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void togglefullscreen(void);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void reshape(int w, int h);
void display(void);
void usage(void);

#include <string>
bool load_assets( const std::string &folder );
