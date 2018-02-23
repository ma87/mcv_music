#include "jack_source.h"
#include <iostream>
#include <string>

int main (int argc, char *argv[])
{
  AudioProperties audio_properties;
  JackSource sourceVolume((char *)"Volume", &audio_properties);
  JackSource sourceFreeverb((char *)"Freeverb", &audio_properties);

  std::string waitToQuit;
  std::cin >> waitToQuit;

  

	/* this is never reached but if the program
	   had some other way to exit besides being killed,
	   they would be important to call.
	*/

	// jack_client_close (client);
	// exit (0);

  return 0;
}
