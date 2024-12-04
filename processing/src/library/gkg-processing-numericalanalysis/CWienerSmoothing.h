#ifndef _gkg_processing_numericalanalysis_CWienerSmoothing_h_
#define _gkg_processing_numericalanalysis_CWienerSmoothing_h_


#include <gkg-processing-numericalanalysis/CNormWeighting.h>


namespace gkg
{


class CWienerSmoothing : public CNormWeighting
{

  public:

    CWienerSmoothing( const CMatrix& covariance );
    virtual ~CWienerSmoothing();

    const CMatrix& getWeighting() const;

  protected:

    CMatrix* _weighting;

};


}


#endif
