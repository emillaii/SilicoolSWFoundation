#ifndef ESF_MODEL_KERNEL_H
#define ESF_MODEL_KERNEL_H

#include "esf_model.h"

class Esf_model_kernel : public Esf_model {
  public:
    Esf_model_kernel(double in_alpha=13) {

        set_alpha(in_alpha);
    }

    virtual int build_esf(vector< Ordered_point  >& ordered, double* sampled,
        const int fft_size, double max_distance_from_edge, vector<double>& esf,
        Snr& snr, bool allow_peak_shift=false);

    double kernel(double x) const;

    virtual void compute_mtf_corrections(void);
};


#endif
