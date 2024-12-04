#ifndef _gkg_processing_bias_BiasCorrection_h_
#define _gkg_processing_bias_BiasCorrection_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-container/Volume.h>



namespace gkg
{


template < class T >
class BiasCorrection
{

  public:

    virtual ~BiasCorrection();

    virtual void correct( const Volume< T >& in,
                          const Volume< int16_t >& mask,
                          const Volume< float >& weight,
                          Volume< T >& out,
                          Volume< float >& bias,
                          bool verbose ) const = 0;

    virtual void correct( const Volume< T >& in,
                          const Volume< int16_t >& mask,
                          Volume< T >& out,
                          Volume< float >& bias,
                          bool verbose ) const;

    virtual void correct( const Volume< T >& in,
                          Volume< T >& out,
                          Volume< float >& bias,
                          bool verbose ) const;

  protected:

    BiasCorrection();

};


}


#endif
