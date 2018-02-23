#include "fft_perso.h"

handle_fftw * init_fft(int nTemporalSamples)
{
    handle_fftw* struct_fftw = (handle_fftw*)malloc(sizeof(handle_fftw));

	struct_fftw->in = (float*)fftwf_malloc( nTemporalSamples * sizeof(float));
	struct_fftw->out = (fftwf_complex*)fftwf_malloc( (nTemporalSamples/2 + 1 ) * sizeof(fftwf_complex));
    memset(struct_fftw->out, 0, (nTemporalSamples/2 + 1 ) *  sizeof(fftwf_complex));
    memset(struct_fftw->in, 0, nTemporalSamples *  sizeof(float));

	struct_fftw->plan = fftwf_plan_dft_r2c_1d(nTemporalSamples,struct_fftw->in, struct_fftw->out,FFTW_ESTIMATE);
    return struct_fftw;
}

handle_fftw * init_ifft(int nTemporalSamples)
{
    handle_fftw* struct_fftw = (handle_fftw*)malloc(sizeof(handle_fftw));
	struct_fftw->out =  (fftwf_complex*)fftwf_malloc( (nTemporalSamples/2 + 1 ) *  sizeof(fftwf_complex));
    memset(struct_fftw->out, 0, (nTemporalSamples/2 + 1 ) *  sizeof(fftwf_complex));
	struct_fftw->in = (float*)fftwf_malloc( nTemporalSamples*  sizeof(float));
    memset(struct_fftw->in, 0, nTemporalSamples *  sizeof(float));
	struct_fftw->plan = fftwf_plan_dft_c2r_1d(nTemporalSamples, struct_fftw->out, struct_fftw->in, FFTW_ESTIMATE);
	return struct_fftw;
}

void process_fft(handle_fftw * fftw, int nTemporalSamples, float * in, fftwf_complex* out)
{
    memcpy(fftw->in, in, nTemporalSamples * sizeof(float));
    fftwf_execute(fftw->plan);
    memcpy(out, fftw->out, (nTemporalSamples / 2 + 1) * sizeof(fftwf_complex));
}

void process_ifft(handle_fftw * fftw, int nTemporalSamples, fftwf_complex* in, float * out)
{
    int i;
    memcpy(fftw->out, in, (nTemporalSamples / 2 + 1) * sizeof(fftwf_complex));
    fftwf_execute(fftw->plan);
    memcpy(out, fftw->in, nTemporalSamples * sizeof(float));

    float * pout = out;
    for (i=0 ; i<nTemporalSamples ; i++)
    {
        *pout = *pout++ / nTemporalSamples;
    }
}
