#ifndef _gkg_processing_bias_BiasCorrection_i_h_
#define _gkg_processing_bias_BiasCorrection_i_h_


#include <gkg-processing-bias/BiasCorrection.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::BiasCorrection< T >::BiasCorrection()
{
}


template < class T >
inline
gkg::BiasCorrection< T >::~BiasCorrection()
{
}


template < class T >
inline
void gkg::BiasCorrection< T >::correct( const gkg::Volume< T >& in,
                                        const Volume< int16_t >& mask,
                                        gkg::Volume< T >& out,
                                        gkg::Volume< float >& bias,
                                        bool verbose ) const
{

  try
  {

    // collecting input size and resolution
    gkg::Vector3d< int32_t > size;
    gkg::Vector3d< double > resolution;
    in.getSize( size );
    in.getResolution( resolution );

    // creating weight
    gkg::Volume< float > weight( size.x, size.y, size.z );
    weight.setResolution( resolution );
    weight.fill( 1.0 );
    this->correct( in, mask, weight, out, bias, verbose );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::BiasCorrection< T >::correct( "
             "const gkg::Volume< T >& in, "
             "const Volume< int16_t >& mask, "
             "gkg::Volume< T >& out, "
             "gkg::Volume< float >& bias, "
             "bool verbose ) const" );

}


template < class T >
inline
void gkg::BiasCorrection< T >::correct( const gkg::Volume< T >& in,
                                        gkg::Volume< T >& out,
                                        gkg::Volume< float >& bias,
                                        bool verbose ) const
{

  try
  {

    // collecting input size and resolution
    gkg::Vector3d< int32_t > size;
    gkg::Vector3d< double > resolution;
    in.getSize( size );
    in.getResolution( resolution );

    // creating mask
    gkg::Volume< int16_t > mask( size.x, size.y, size.z );
    mask.setResolution( resolution );
    mask.fill( 1 );
    this->correct( in, mask, out, bias, verbose );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::BiasCorrection< T >::correct( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out, "
             "gkg::Volume< float >& bias, "
             "bool verbose ) const" );

}


#endif


