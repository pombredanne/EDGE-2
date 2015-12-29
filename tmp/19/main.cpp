//
//  ViewController.m
//  FontstashiOS
//
//  Created by Karim Naaji on 07/10/2014.
//  Copyright (c) 2014 Mapzen. All rights reserved.
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <eve/window.hpp>
#include <eve/hexcolor.hpp>
#include <hertz/hertz.hpp>

#include <stdio.h>
#include <string.h>

#define GLFONTSTASH_IMPLEMENTATION
#define STB_TRUETYPE_STATIC
#include "fontstash/glfontstash.h"

#include <windows.h>

#ifdef  $live
#undef  $live
#endif

#define $live(x) (x)

namespace {
    FONScontext* fs;
    int fontNormal;
    int fontJapanese;
    fsuint text0;
    fsuint text1;
    fsuint text2;
}

void setup(int width, int height) {

    glViewport(0, 0, width, height);
    glClearColor(0/255.f, 40/255.f, 40/255.f, 1.0f);

    fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
    if (fs == NULL) {
        printf("Could not create stash.\n");
    }

    fontNormal = fonsAddFont(fs, "sans", "res/DroidSerif-Regular.ttf");
    if (fontNormal == FONS_INVALID) {
        fontNormal = fonsAddFont(fs, "sans", "data/fonts/DroidSerif-Regular.ttf");
        if(fontNormal == FONS_INVALID) {
            printf("Could not add font normal.\n");
        }
    }
    fontJapanese = fonsAddFont(fs, "sans", "res/DroidSansJapanese.ttf");
    if (fontJapanese == FONS_INVALID) {
        fontJapanese = fonsAddFont(fs, "sans", "data/fonts/DroidSansJapanese.ttf");
        if(fontJapanese == FONS_INVALID) {
            printf("Could not add font japanese.\n");
        }
    }

    fonsSetFont(fs, fontNormal);
    fonsSetSize(fs, 100.0);
    glfonsBufferText(fs, "Fontstash", &text0, FONS_EFFECT_NONE);

    fonsSetFont(fs, fontNormal);
    fonsSetSize(fs, 30.0);
    fonsSetBlur(fs, 5.0);
    fonsSetBlurType(fs, FONS_EFFECT_DISTANCE_FIELD);
    glfonsBufferText(fs, "Fontstash", &text1, FONS_EFFECT_DISTANCE_FIELD);

    fonsSetFont(fs, fontJapanese);
    fonsSetSize(fs, 30.0);
    fonsSetBlur(fs, 5.0);
    fonsSetBlurType(fs, FONS_EFFECT_DISTANCE_FIELD);
    glfonsBufferText(fs, "私はガラスを食べられます。それは私を傷つけません。", &text2, FONS_EFFECT_DISTANCE_FIELD);

}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    static float x, y;
    x += .05f; y += .07f;
    float xnorm = (sin(x) + 1.0) * 0.5;
    float ynorm = (cos(y) + 1.0) * 0.5;

    fonsSetFont(fs, fontNormal);
    glfonsSetColor(fs, 255, 255, 255, 150);

    // showcase scale
    glfonsPushMatrix(fs);
        glfonsTranslate(fs, 100.0, 200.0);
        glfonsDrawText(fs, text0);

        glfonsTranslate(fs, 0.0, 100.0);
        glfonsScale(fs, $live(4.0), xnorm+$live(4.0) );
        glfonsSetColor(fs, 255, 255, 255, 150);
        glfonsSetOutlineColor(fs, 255, 0, 255, 255);
//        glfonsSetSDFProperties(fs, 0.2, 0.3, 0.0, 0.0, 0.1); // gummy, thin border
//        glfonsSetSDFProperties(fs, 0.1, 0.2, 0.8, 1.0, 0.1); // more gummy, thin border
//        glfonsSetSDFProperties(fs, 0.0, 0.1, 0.3, 0.4, 0.25); // soft halo
//        glfonsSetSDFProperties(fs, 0.2, 0.3, 0.0, 0.05, 0.2); // gummy, self-shadow, thin border
        glfonsSetSDFProperties(fs, $live(0), $live(0), $live(0.4), $live(0.5), $live(1)); // solid -> 0.4..0.0 blur
        //glfonsTranslate(fs, 0.0, 30.0 * xnorm);
        glfonsDrawText(fs, text1);
    glfonsPopMatrix(fs);

    // showcase outline
    glfonsPushMatrix(fs);
        eve::hexcolor yellow("#ff0");
        glfonsSetColor(fs, yellow.r*255, yellow.g*255, yellow.b*255, 255);
        glfonsTranslate(fs, 100.0, 800.0);
        glfonsScale(fs, xnorm+$live(3.0), $live(8.0));
        glfonsSetOutlineColor(fs, xnorm*142+ynorm*142, 68, 173, xnorm*127 + $live(128));
        glfonsSetSDFProperties(fs, $live(0.2), $live(0.4), $live(0), $live(0.5), $live(0)); // gummy, shadow, thin border
        glfonsDrawText(fs, text2);
        glfonsSetOutlineColor(fs, 255, 255, 255, 255);
        glfonsSetSDFProperties(fs, $live(0), $live(0), $live(0.4), $live(0.5), $live(1)); // gummy, shadow, thin border
        glfonsDrawText(fs, text2);
    glfonsPopMatrix(fs);
}

void shutdown() {
    glfonsDelete(fs);
}

int main() {
    eve::window app("fontstash");
    setup(app.w, app.h);
    while( app.is_open() ) {
        auto fps = hertz::lock(60,
        []{

        },
        [&]{
            draw();
            app.flush( 1/60.f );
        } );
    }
    hertz::unlock();
    shutdown();
}
