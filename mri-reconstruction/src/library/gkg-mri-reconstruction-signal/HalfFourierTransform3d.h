#ifndef _gkg_mri_reconstruction_HalfFourierTransform3d_h_
#define _gkg_mri_reconstruction_HalfFourierTransform3d_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-signal/FourierTransform3d.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;


template < class T >
class HalfFourierTransform3d : public Singleton< HalfFourierTransform3d< T > >
{

  public:

    typedef typename FourierTransform3d< T >::Axis Axis;

    ~HalfFourierTransform3d();

    void applyCenteredInverse( Volume< std::complex< T > >& data,
                               Axis axis,
                               int32_t rawSize,
                               int32_t overScanCount,
                               const Apodization< T >* apodization = 0,
                               bool verbose = false ) const;


  private:

    friend class Singleton< HalfFourierTransform3d< T > >;

    HalfFourierTransform3d();

};


}


#endif
