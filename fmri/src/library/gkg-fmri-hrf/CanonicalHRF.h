#ifndef _gkg_fmri_hrf_CanonicalHRF_h_
#define _gkg_fmri_hrf_CanonicalHRF_h_


#include <gkg-fmri-hrf/HemodynamicResponseFunction.h>


#define GKG_BOLD_CANONICAL_HRF_DELAY                   6.0
#define GKG_BOLD_CANONICAL_HRF_DELAY_UNDERSHOOT       16.0
#define GKG_BOLD_CANONICAL_HRF_DISPERSION              1.0
#define GKG_BOLD_CANONICAL_HRF_DISPERSION_UNDERSHOOT   1.0
#define GKG_BOLD_CANONICAL_HRF_RATIO                   6.0

namespace gkg
{


class NumericalAnalysisImplementationFactory;


class CanonicalHRF : public HemodynamicResponseFunction
{

  public:

    CanonicalHRF( double delay =
                           GKG_BOLD_CANONICAL_HRF_DELAY,
                  double delayUndershoot =
                           GKG_BOLD_CANONICAL_HRF_DELAY_UNDERSHOOT,
                  double dispersion =
                           GKG_BOLD_CANONICAL_HRF_DISPERSION,
                  double dispersionUndershoot =
                           GKG_BOLD_CANONICAL_HRF_DISPERSION_UNDERSHOOT,
                  double ratio =
                           GKG_BOLD_CANONICAL_HRF_RATIO );
    ~CanonicalHRF();

    double getValue( double t ) const;

  protected:

    double _delay;
    double _delayUndershoot;
    double _dispersion;
    double _dispersionUndershoot;
    double _ratio;

    NumericalAnalysisImplementationFactory* _factory;

};


}


#endif
