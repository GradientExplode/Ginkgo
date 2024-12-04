#ifndef _gkg_deep_nuclei_midsagittalsurface_MidSagittalSurfaceContext2d_h_
#define _gkg_deep_nuclei_midsagittalsurface_MidSagittalSurfaceContext2d_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-numericalanalysis/ThinPlateSpline.h>


namespace gkg
{


template < class T > class Volume;


template < class T >
class MidSagittalSurfaceContext2d : public LoopContext< int32_t >
{

  public:

    MidSagittalSurfaceContext2d( const Volume< T >& volumeIn,
                                 Volume< T >& volumeOut,
                                 const ThinPlateSpline< int32_t >& spline );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    int32_t _sizeX;
    int32_t _sizeY;
    const Volume< T >& _volumeIn;
    Volume< T >& _volumeOut;
    const ThinPlateSpline< int32_t >& _spline;

};


}


#endif
