/*
SoLoud audio engine
Copyright (c) 2013-2014 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include <stdlib.h>
#include <stdio.h>

#include <eve/eve.hpp>


// Entry point
int main(int argc, const char *argv[])
{
	eve::app app( argc, argv, "w=640,h=480,title='to be handled'" );
	eve::warn("this is a test");
	eve::fail("this is a fail");

	/*
	eve::route( 8080, "/echo", []( const eve::string &options ) {
		return dispatch( 200, "Content-Type: text/html;charset=UTF-8\r\n", options.str() );
		return dispatch( 200, "Content-Type: application/json;charset=UTF-8\r\n", options.str() );
	} );
	*/

	// Declare some variables
	eve::Soloud soloud; // Engine core
	eve::Speech speech; // A sound source (speech, in this case)
	eve::Wav wav;       // One sample source
	eve::Modplug mod;   // One song source

	// Initialize SoLoud (automatic back-end selection)
	// also, enable visualization for FFT calc
	soloud.init();
	soloud.setVisualizationEnable(1);
	printf("%s\n", "Welcome to Soloud!");

	// Load background sample
	wav.load("windy_ambience.ogg");       // Load a wave file
	int handle1 = soloud.play(wav);             // Play it
	soloud.setVolume(handle1, 0.5f);            // Set volume; 1.0f is "normal"
	soloud.setPan(handle1, -0.2f);              // Set pan; -1 is left, 1 is right
	soloud.setRelativePlaySpeed(handle1, 0.9f); // Play a bit slower; 1.0f is normal

	// Configure sound source
	printf("%s\n", "What is your name?");
	char name[512] = "hello  ";
	scanf("%s",&name[6]);
	speech.setText(name);
	// Play the sound source (we could do this several times if we wanted)
	soloud.play(speech);

	// Wait for voice to finish
	while (soloud.getActiveVoiceCount() > 1)
	{
		// Still going, sleep for a bit
		SoLoud::Thread::sleep(100);
	}

	// Load song
	SoLoud::result loaded = mod.load("BRUCE.S3M");
	if( SoLoud::SO_NO_ERROR == loaded ) {
		soloud.play(mod);
	} else {
		printf("%s\n", "Cannot find BRUCE.S3M (or --with-libmodplug build option may be missing)");
	}

	// Wait
	while (soloud.getActiveVoiceCount() > 0)
	{
		// Still going, sleep for a bit
		SoLoud::Thread::sleep(100);
	}

	// Clean up SoLoud
	soloud.deinit();

	// All done.
	return 0;
}
