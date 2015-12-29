#include "TLFXPortEffectsLibrary.h"

#include <TLFXEffectsLibrary.h>
#include <TLFXParticleManager.h>
#include <TLFXEffect.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sketch2d/sketch2d.h>

#include <sand/sand.hpp>

PImage img1, img2;
PFont font;

TLFX::EffectsLibrary *gEffects = NULL;
TLFXPortParticleManager *gPM = NULL;
std::string effect;

void setup()
{
    auto w = displayWidth/2;
    auto h = displayHeight/2;

    size(w, h);
    noCursor();
    img1 = loadImage("image9.jpg");
    img2 = loadImage("image10.jpg");
    font = loadFont("Roboto-Regular.ttf");

    gEffects = new TLFXPortEffectsLibrary();
    gEffects->Load("particles/data.xml");

    gPM = new TLFXPortParticleManager();
    gPM->SetScreenSize(w, h);
    gPM->SetOrigin(0, 0);

    TLFX::Effect *eff = gEffects->GetEffect(effect.c_str());
	TLFX::Effect *copy = eff ? new TLFX::Effect(*eff, gPM) : 0;

    copy->SetPosition(0.0f, 0.0f);
    gPM->AddEffect(copy);
}

void shutdown()
{
    delete gEffects;
    delete gPM;
}

void draw()
{
    background(gray(0)); //gray(122));

    if (mousePressed)
    {
        image(img1, mouseX, mouseY, img1.width, img1.height);
    }
    else
    {
        image(img2, mouseX, mouseY, img2.width, img2.height);
    }    

    textFont(font);
    textAlign(NVG_ALIGN_CENTER);
    textSize(30);
    textLeading(5);
    text("test everything here", width/2, height/2);

    if (keyPressed)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            quit();
        }
        else if (key == GLFW_KEY_SPACE)
        {
            saveFrame("screenshot.png");
        }
    }

    // Attempt to lock to 60 frames per second
    if( !sand::lock(60) ) {
        return;
    }

    gPM->Update();

    gPM->DrawParticles();
    gPM->Flush();
}

#include <iostream>

int main( int argc, const char **argv ) {
    if( argc > 1 ) {
        std::string sep;
        for( int i = 1; i < argc; ++i ) {
            effect += sep + argv[i];
            sep = " ";
        }        
    } else {
        effect = "Sub Effects/Space Creature"; //Area Effects/Toxic"; //Area Effects/Swirly Balls";
    }
    std::cout << "Loading fx: " << effect << std::endl;
    return sketch2d_main(0,0);
}
