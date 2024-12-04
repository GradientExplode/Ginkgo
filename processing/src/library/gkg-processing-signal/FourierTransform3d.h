#ifndef _gkg_processing_signal_FourierTransform3d_h_
#define _gkg_processing_signal_FourierTransform3d_h_


#include <gkg-core-pattern/Singleton.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;
template < class T > class Apodization;


template < class T >
class FourierTransform3d : public Singleton< FourierTransform3d< T > >
{

  public:

    enum Axis
    {

      X_AXIS,
      Y_AXIS,
      Z_AXIS

    };


    ~FourierTransform3d();

    void applyRawDirect( Volume< std::complex< T > >& data,
                         Axis axis,
                         const Apodization< T >* apodization = 0,
                         bool verbose = false ) const;
    void applyRawInverse( Volume< std::complex< T > >& data,
                          Axis axis,
                          const Apodization< T >* apodization = 0,
                          bool verbose = false ) const;

    void applyCenteredDirect( Volume< std::complex< T > >& data,
                              Axis axis,
                              const Apodization< T >* apodization = 0,
                              bool verbose = false ) const;
    void applyCenteredInverse( Volume< std::complex< T > >& data,
                               Axis axis,
                               const Apodization< T >* apodization = 0,
                               bool verbose = false ) const;


  private:

    friend class Singleton< FourierTransform3d< T > >;

    FourierTransform3d();

};


}


#endif
