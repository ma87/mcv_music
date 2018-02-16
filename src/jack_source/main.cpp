#include "jack_source.h"

int main (int argc, char *argv[])
{
  JackSource sourceVolume((char *)"Volume");
  JackSource sourceFreeverb((char *)"Freeverb");

  sleep (-1);

	/* this is never reached but if the program
	   had some other way to exit besides being killed,
	   they would be important to call.
	*/

	// jack_client_close (client);
	// exit (0);

  return 0;
}
