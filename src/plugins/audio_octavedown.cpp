#include "audio_octavedown.h"


OctaveDown::OctaveDown(int nchannels_input, int nchannels_output, int nsamples)
 : AudioEffect(nchannels_input, nchannels_output)
{
  //m_fft = init_fft(nsamples * 2);
  //m_fft = init_fft(nsamples);
  m_fft = init_fft(nsamples / 2);
  m_ifft = init_ifft(nsamples);
}

OctaveDown::~OctaveDown()
{

}

/*
int OctaveDown::process(float * input)
{
  // float * upsample = this->upsample(input, 2);
  //
  // fftwf_complex * inCpx = (fftwf_complex *) fftwf_malloc((m_nsamples+ 1) * sizeof(fftwf_complex));
  // memset(inCpx, 0, (m_nsamples + 1) * sizeof(fftwf_complex));
  //
  // process_fft(m_fft, m_nsamples * 2, upsample, inCpx);
  //

  fftwf_complex * inCpx = (fftwf_complex *) fftwf_malloc((m_nsamples / 2+ 1) * sizeof(fftwf_complex));
  memset(inCpx, 0, (m_nsamples / 2+ 1) * sizeof(fftwf_complex));

  process_fft(m_fft, m_nsamples, input, inCpx);

  fftwf_complex * outCpx = (fftwf_complex *) fftwf_malloc((m_nsamples / 2+ 1) * sizeof(fftwf_complex));
  memset(outCpx, 0, (m_nsamples / 2 + 1) * sizeof(fftwf_complex));

  fftwf_complex * p_inCpx = inCpx;
  fftwf_complex * p_outCpx = outCpx;
  for(int i=0 ; i < m_nsamples / 4 + 1 ; i++)
  {
      //*p_outCpx++ = *p_inCpx;
      memcpy(p_outCpx, p_inCpx, sizeof(fftwf_complex));
      p_outCpx += 1;
      p_inCpx += 2;
  }

  process_ifft(m_ifft, m_nsamples, outCpx, m_samples);


  //free(upsample);
  fftwf_free(inCpx);
  fftwf_free(outCpx);
}
*/


int OctaveDown::process(float ** input, float ** output, int nsamples)
{
  /*
  float * downsample = this->downsample(input, 2);

  fftwf_complex * inCpx = (fftwf_complex *) fftwf_malloc((m_nsamples / 4 + 1) * sizeof(fftwf_complex));
  memset(inCpx, 0, (m_nsamples / 4 + 1) * sizeof(fftwf_complex));

  process_fft(m_fft, m_nsamples / 2, downsample, inCpx);

  fftwf_complex * outCpx = (fftwf_complex *) fftwf_malloc((m_nsamples / 2+ 1) * sizeof(fftwf_complex));
  memset(outCpx, 0, (m_nsamples / 2 + 1) * sizeof(fftwf_complex));

  memcpy(outCpx, inCpx, (m_nsamples / 4 + 1) * sizeof(fftwf_complex));

  process_ifft(m_ifft, m_nsamples, outCpx, m_samples);


  free(downsample);
  fftwf_free(inCpx);
  fftwf_free(outCpx);
  */
}

float * OctaveDown::downsample(float * input, int factor, int nsamples)
{
  float * output = (float *)malloc((nsamples / factor) * sizeof(float));

  float *p_output = output;
  float *p_input = input;
  for (int i=0 ; i < nsamples / factor ; i++)
  {
    *p_output++ = *p_input;
    p_input+=factor;
  }
  return output;
}

float * OctaveDown::upsample(float * input, int factor, int nsamples)
{
  float * output = (float *)malloc(nsamples * factor * sizeof(float));

  float ratio = 1 / (float)factor;

  float *p_output = output;
  float *p_input = input;
  for (int i=0 ; i < nsamples ; i++)
  {
    *p_output++ = *p_input;
    *p_output++ = ratio * *p_input + ratio * *(p_input+1);
    p_input++;
  }
  return output;
}
