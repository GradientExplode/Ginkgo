#ifndef _gkg_deep_nuclei_midsagittalsurface_MidSagittalSurface_h_
#define _gkg_deep_nuclei_midsagittalsurface_MidSagittalSurface_h_


#include <gkg-processing-numericalanalysis/ThinPlateSpline.h>
#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class Volume;


template < class T >
class MidSagittalSurface
{

  public:

    MidSagittalSurface( int32_t slabWidth = 30, 
                        double sigma = 4.0, 
                        double minCorrelation = 0.9,
                        double regularization = 0.3 );
    virtual ~MidSagittalSurface();

    void compute( const Volume< T >& volumeIn,
                  const Volume< uint8_t >& mask,
                  Volume< T >& volumeOut,
                  bool output3D = false,
                  bool verbose = false );

  private:

    void fill2d( const Volume< T >& volumeIn, Volume< T >& volumeOut ) const;
    void fill3d( const Volume< T >& volumeIn, Volume< T >& volumeOut ) const;

    ThinPlateSpline< int32_t > _spline;

    int32_t _slabWidth;
    double _sigma;
    double _minCorrelation;
    double _regularization;

};


}


#endif
