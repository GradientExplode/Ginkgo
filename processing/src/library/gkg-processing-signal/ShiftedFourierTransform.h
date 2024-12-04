#ifndef _gkg_processing_signal_ShiftedFourierTransform_h_
#define _gkg_processing_signal_ShiftedFourierTransform_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Apodization;


template < class T >
class ShiftedFourierTransform : public Singleton< ShiftedFourierTransform< T > >
{

  public:

    ~ShiftedFourierTransform();

    void applyRawDirect( std::vector< std::complex< T > >& data,
                         const std::vector< T >& shift,
                         const Apodization< T >* apodization = 0 ) const;
    void applyRawInverse( std::vector< std::complex< T > >& data,
                          const std::vector< T >& shift,
                          const Apodization< T >* apodization = 0 ) const;

    void applyCenteredDirect( std::vector< std::complex< T > >& data,
                              const std::vector< T >& shift,
                              const Apodization< T >* apodization = 0 ) const;
    void applyCenteredInverse( std::vector< std::complex< T > >& data,
                               const std::vector< T >& shift,
                               const Apodization< T >* apodization = 0 ) const;


  private:

    friend class Singleton< ShiftedFourierTransform< T > >;

    ShiftedFourierTransform();

    void apodize( std::vector< std::complex< T > >& data,
                  const Apodization< T >* apodization ) const;
    void shiftedDft( std::vector< std::complex< T > >& data,
                     const std::vector< T >& shift,
                     int32_t sign ) const;

};


}


#endif
