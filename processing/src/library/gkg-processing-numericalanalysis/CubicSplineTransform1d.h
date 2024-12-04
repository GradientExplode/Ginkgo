#ifndef _gkg_processing_numericalanalysis_CubicSplineTransform1d_h_
#define _gkg_processing_numericalanalysis_CubicSplineTransform1d_h_


#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class CubicSplineTransform1d
{

  public:

    CubicSplineTransform1d( const Vector& data );
    virtual ~CubicSplineTransform1d();

    double getCoefficient( int32_t t ) const;
    double getValue( double time ) const;

    static double cubicSplineBasis( double time );

  protected:

    int32_t cubicSplineMirror( int32_t t ) const;


    Vector _coefficients;

    // only for speed
    int32_t _size;
    int32_t _sizeMinusOne;
    int32_t _sizeMinusOneTimes2;

};


}


 #endif
