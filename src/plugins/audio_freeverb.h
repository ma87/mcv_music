#ifndef __MCV_AUDIO_FREEVERB_H__
#define __MCV_AUDIO_FREEVERB_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "freeverb/revmodel.hpp"
#include "audio_properties.h"
#include "audio_effect.h"


class FreeverbEffect : public AudioEffect
{
  public:
    FreeverbEffect(int nchannels_input, int nchannels_output, float sampling_rate);
    virtual ~FreeverbEffect();

    virtual int process(float ** input, float ** output, int nsamples);
    virtual void setProperties(AudioProperties * audioProperties);

  private:
    void setreverbtype(int value);
    void setroomsize(float value);
    void setdamp(float value);
    void setwet(float value);
    void setdry(float value);
    void setwidth(float value);

  protected:
    fv3::revmodel_f	m_model;
};


#endif
