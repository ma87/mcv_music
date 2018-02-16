#ifndef __TOOLS_FFT_PERSO_H
#define __TOOLS_FFT_PERSO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fftw3.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct handle_fftw {
	float* in;
	fftwf_complex* out;
	fftwf_plan plan;
} handle_fftw;

handle_fftw * init_fft(int nTemporalSamples);
handle_fftw * init_ifft(int nTemporalSamples);
void process_fft(handle_fftw * fftw, int nTemporalSamples, float * in, fftwf_complex* out);
void process_ifft(handle_fftw * fftw, int nTemporalSamples, fftwf_complex* in, float * out);

#ifdef __cplusplus
}
#endif

#endif

