#ifndef __MCV_AUDIO_OCTAVEDOWN_H__
#define __MCV_AUDIO_OCTAVEDOWN_H__

#include "audio_effect.h"
#include "fft_perso.h"

class OctaveDown : public AudioEffect
{
  public:
    OctaveDown(int nchannels_input, int nchannels_output, int nsamples);
    virtual ~OctaveDown();

    virtual int process(float ** input, float ** output, int nsamples);

  private:
    int m_ratio;
    handle_fftw * m_fft;
    handle_fftw * m_ifft;

    float * downsample(float * input, int ratio, int nsamples);
    float * upsample(float * input, int ratio, int nsamples);
};


#endif
