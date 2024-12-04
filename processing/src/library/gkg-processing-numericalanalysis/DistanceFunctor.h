#ifndef _gkg_processing_numericalanalysis_DistanceFunctor_h_
#define _gkg_processing_numericalanalysis_DistanceFunctor_h_


#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class DistanceFunctor
{

  public:

    virtual ~DistanceFunctor();

    virtual double getDistanceSquare( const Matrix& samples,
                                      int32_t sampleIndex1,
                                      int32_t sampleIndex2 ) const = 0;

  protected:

    DistanceFunctor();

};


}


#endif

