#include "audio_effect.h"

AudioEffect::AudioEffect(int nchannels_input, int nchannels_output)
{
  m_nchannels_input = nchannels_input;
  m_nchannels_output = nchannels_output;

  m_enabled = true;
  m_volume = 1.0f;
}

AudioEffect::~AudioEffect()
{

}

int AudioEffect::getNumberChannelsInput()
{
  return m_nchannels_input;
}

int AudioEffect::getNumberChannelsOutput()
{
  return m_nchannels_output;
}

float AudioEffect::getVolume()
{
  return m_volume / MAX_GAIN;
}

// Input is [0.0 , 1.0]
void AudioEffect::setVolume(float volume)
{
  m_volume = volume * MAX_GAIN;
  std::cout << "set volume to " << m_volume << std::endl;
}

int AudioEffect::process(float ** input, float ** output, int nsamples)
{
  for (int i=0 ; i<m_nchannels_input ; i++)
  {
    for (int j=0 ; j<nsamples ; j++)
    {
      output[i][j] = m_volume * input[i][j];
    }
  }
}

void AudioEffect::setProperties(AudioProperties * audioProperties)
{
  using std::placeholders::_1;
  audioProperties->addFloatProperty(std::string("INPUT_VOLUME"), std::bind( &AudioEffect::setVolume, this, _1 ));
  float value = getVolume();
  audioProperties->setFloatProperty(std::string("INPUT_VOLUME"), value);
}

void AudioEffect::setEnabled(bool enabled)
{
  m_enabled = enabled;
}
