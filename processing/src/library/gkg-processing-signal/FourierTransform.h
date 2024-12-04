#ifndef _gkg_processing_signal_FourierTransform_h_
#define _gkg_processing_signal_FourierTransform_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Apodization;


template < class T >
class FourierTransform : public Singleton< FourierTransform< T > >
{

  public:

    ~FourierTransform();

    void applyRawDirect( std::vector< std::complex< T > >& data,
                         const Apodization< T >* apodization = 0 ) const;
    void applyRawInverse( std::vector< std::complex< T > >& data,
                          const Apodization< T >* apodization = 0 ) const;

    void applyCenteredDirect( std::vector< std::complex< T > >& data,
                              const Apodization< T >* apodization = 0 ) const;
    void applyCenteredInverse( std::vector< std::complex< T > >& data,
                               const Apodization< T >* apodization = 0 ) const;


  private:

    friend class Singleton< FourierTransform< T > >;

    FourierTransform();

    void apodize( std::vector< std::complex< T > >& data,
                  const Apodization< T >* apodization ) const;
    void fft( std::vector< std::complex< T > >& data, int32_t sign ) const;
    void dft( std::vector< std::complex< T > >& data, int32_t sign ) const;

};


}


#endif
