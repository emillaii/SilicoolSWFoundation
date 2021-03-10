#ifndef ESF_MODEL_H
#define ESF_MODEL_H
#include "include/ordered_point.h"
#include "include/sampling_rate.h"
#include "include/snr.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class Esf_model {
  public:
    Esf_model(double alpha=13.7)
    : alpha(alpha), w(NYQUIST_FREQ*4, 0.0)  {}

    virtual int build_esf(vector< Ordered_point  >& ordered, double* sampled,
        const int fft_size, double max_distance_from_edge, vector<double>& esf,
        Snr& snr, bool allow_peak_shift=false) = 0;

    void moving_average_smoother(vector<double>& smoothed, double* sampled, int fft_size,
        int fft_left, int fft_right, int left_trans, int right_trans, int width=16);

    int estimate_esf_clipping(vector< Ordered_point  >& ordered, double* sampled,
        const int fft_size, bool allow_peak_shift, int effective_maxdot, vector<double>& mean,
        vector<double>& weights, int& fft_left, int& fft_right, int& twidth, Snr& snr);

    virtual void set_alpha(double a) {
        alpha = a;
        compute_mtf_corrections();
    }

    virtual void compute_mtf_corrections(void);

    const vector<double>& get_correction(void) const {
        return w;
    }

    void set_monotonic_filter(bool b) {
        apply_monotonic_filter = b;
    }

    const static std::array<string, 2> esf_model_names;
  protected:
    inline double get_alpha(void) const {
        return alpha;
    }

    inline double sinc(double x) {
        return x == 0 ? 1 : sin(x)/x;
    }

    double alpha = 13.5;
    vector<double> w; // MTF correction weight
    static constexpr double missing = -1e7;
    bool apply_monotonic_filter = false;
};

#endif

