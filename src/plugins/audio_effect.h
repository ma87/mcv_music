#ifndef __MCV_AUDIO_EFFECT_H__
#define __MCV_AUDIO_EFFECT_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <thread>
#include "audio_properties.h"
#include <condition_variable>
#include <mutex>
#include <atomic>

#define MAX_GAIN 5.656854249492f // 4 * sqrt(2)

class AudioEffect
{
  public:
    AudioEffect(int nchannels_input, int nchannels_output);
    virtual ~AudioEffect();

    float getVolume();
    int   getNumberChannelsInput();
    int   getNumberChannelsOutput();
    void setVolume(float volume);
    void setEnabled(bool enabled);
    virtual int process(float ** input, float ** output, int nsamples);
    virtual void setProperties(AudioProperties * properties);

  protected:
    float m_volume;
    int m_nchannels_input;
    int m_nchannels_output;
    bool m_enabled;
};


#endif
