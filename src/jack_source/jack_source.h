#ifndef __MCV_JACK_SOURCE_H__
#define __MCV_JACK_SOURCE_H__

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <jack/jack.h>

#include "audio_effect.h"
#include "audio_freeverb.h"

class JackSource
{
  public:
    JackSource(char * plugin_name);
    AudioEffect   * getPlugin();

    jack_port_t **m_input_ports;
    int           m_number_input_ports;
    jack_port_t **m_output_ports;
    int           m_number_output_ports;
    jack_client_t *m_client;
    AudioEffect   *m_plugin;

  private:
    AudioEffect   * createPlugin(char * plugin_name);
};




#endif
