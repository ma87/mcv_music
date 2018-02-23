#include "jack_source.h"


int
process (jack_nframes_t nframes, void *arg)
{
	JackSource *source = (JackSource *)arg;

  jack_default_audio_sample_t ** in = (float **)malloc(source->m_number_input_ports * sizeof(float *));
  jack_default_audio_sample_t ** out = (float **)malloc(source->m_number_output_ports * sizeof(float *));

  for (int i=0 ; i < source->m_number_input_ports ; i++)
  {
    in[i] = (jack_default_audio_sample_t*)jack_port_get_buffer (source->m_input_ports[i], nframes);
  }

  for (int i=0 ; i < source->m_number_output_ports ; i++)
  {
    out[i] = (jack_default_audio_sample_t*)jack_port_get_buffer (source->m_output_ports[i], nframes);
  }

  source->m_plugin->process((float **)in, (float **)out, nframes);

	return 0;
}

void
jack_shutdown (void *arg)
{
	// JackSource *source = (JackSource *)arg;
  //
}

JackSource::JackSource(char * plugin_name, AudioProperties * audio_properties)
{
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;

	/* open a client connection to the JACK server */

	m_client = jack_client_open (plugin_name, options, &status, server_name);
	if (m_client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK server started\n");
	}
	if (status & JackNameNotUnique) {
		plugin_name = jack_get_client_name(m_client);
		fprintf (stderr, "unique name `%s' assigned\n", plugin_name);
	}

  m_plugin = createPlugin(plugin_name);
	m_plugin->setProperties(audio_properties);

	jack_set_process_callback (m_client, process, this);


	jack_on_shutdown (m_client, jack_shutdown, this);

	/* display the current sample rate.
	 */

	printf ("engine sample rate: %" PRIu32 "\n",
		jack_get_sample_rate (m_client));

	/* create two ports */
  m_number_input_ports = m_plugin->getNumberChannelsInput();
  m_number_output_ports = m_plugin->getNumberChannelsOutput();
  m_input_ports = (jack_port_t**)malloc(m_number_input_ports * sizeof(jack_port_t *));
  m_output_ports = (jack_port_t**)malloc(m_number_output_ports * sizeof(jack_port_t *));

  char * port_name;
  for (int i = 0 ; i < m_number_input_ports ; i++)
  {
    port_name = (char *)malloc(30 * sizeof(char));
    sprintf(port_name, "input_%d", i);
    m_input_ports[i] = jack_port_register (m_client, port_name,
  					 JACK_DEFAULT_AUDIO_TYPE,
  					 JackPortIsInput, 0);
    free(port_name);
  }

  for (int i = 0 ; i < m_number_output_ports ; i++)
  {
    port_name = (char *)malloc(30 * sizeof(char));
    sprintf(port_name, "output_%d", i);
    m_output_ports[i] = jack_port_register (m_client, port_name,
  					  JACK_DEFAULT_AUDIO_TYPE,
  					  JackPortIsOutput, 0);
    free(port_name);
  }



	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate (m_client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

}

JackSource::~JackSource()
{
	int res;
	if (res = jack_deactivate(m_client))
	{
		fprintf (stderr, "cannot deactivate client, err = %d\n", res);
	}
	else
	{
		printf("client deactivated\n");
	}
	if (res = jack_client_close(m_client))
	{
		fprintf (stderr, "cannot close client, err = %d\n", res);
	}
	else
	{
		printf("client closed\n");
	}
}

AudioEffect * JackSource::createPlugin(char * plugin_name)
{
  if (!strcmp(plugin_name, "Volume"))
  {
    return new AudioEffect(1, 1);
  }
  else if (!strcmp(plugin_name, "Freeverb"))
  {
    return new FreeverbEffect(2, 2, jack_get_sample_rate (m_client));
  }
}

AudioEffect * JackSource::getPlugin()
{
  return m_plugin;
}
