#ifndef _gkg_fmri_hrf_HemodynamicResponseFunction_h_
#define _gkg_fmri_hrf_HemodynamicResponseFunction_h_


#include <gkg-core-cppext/StdInt.h>


#define GKG_BOLD_STIMULUS_PERSISTENCE   0.2
#define GKG_BOLD_HRF_DURATION          30.0
#define GKG_BOLD_HRF_CONVOLUTION_STEP   0.1


namespace gkg
{


class HemodynamicResponseFunction
{

  public:

    virtual ~HemodynamicResponseFunction();

    virtual double getValue( double t ) const = 0;

    int32_t getRegressorCount() const;

  protected:

    HemodynamicResponseFunction( int32_t regressorCount );

    int32_t _regressorCount;

};


}


#endif
