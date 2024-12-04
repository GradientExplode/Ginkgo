#ifndef _gkg_processing_numericalanalysis_AkimaSpline3d_h_
#define _gkg_processing_numericalanalysis_AkimaSpline3d_h_


#include <gkg-processing-numericalanalysis/CubicSpline3d.h>


namespace gkg
{


template < class T >
class AkimaSpline3d : public CubicSpline3d< T >
{

  public:

    AkimaSpline3d( const std::vector< Vector3d< T > >& controlPoints );
    AkimaSpline3d( const LightCurve3d< T >& lightCurve3d );
    
  protected:
  
    void initialize( const std::vector< Vector3d< T > >& controlPoints );

};


}


#endif
