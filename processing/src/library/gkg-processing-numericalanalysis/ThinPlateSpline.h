#ifndef _gkg_processing_numericalanalysis_ThinPlateSpline_h_
#define _gkg_processing_numericalanalysis_ThinPlateSpline_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-coordinates/Vector2d.h>

#include <vector>


namespace gkg
{


class NumericalAnalysisImplementationFactory;


template < class T >
class ThinPlateSpline
{

  public:

    ThinPlateSpline();
    virtual ~ThinPlateSpline();

    void evaluate( const std::vector< Vector2d< T > >& controlPoints,
                   const std::vector< double >& values,
                   double regularization = 0.0 );

    double getValueAt( T x, T y ) const;

  private:

    double radialBasis( double r ) const;

    NumericalAnalysisImplementationFactory* _factory;
    std::vector< Vector2d< T > > _controlPoints;
    Vector _v;

};


}


#endif
