#include <gkg-processing-morphology/Closing.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-morphology/Erosion.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-core-exception/Exception.h>


template < class IN, class OUT >
gkg::Closing< IN, OUT >::Closing( float radius,
                                  const gkg::TestFunction< IN >& testFunction,
                                  const OUT& foreground,
                                  const OUT& background )
                        : _radius( radius ),
                          _testFunction( testFunction ),
                          _foreground( foreground ),
                          _background( background )

{

  try
  {

    if ( radius <= 0.0 )
    {

       throw std::runtime_error( "radius of closing should be "
                                 "strictly positive" );

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "gkg::Closing< IN, OUT >::Closing( "
             "float radius, "
             "const gkg::TestFunction< IN >& testFunction, "
             "const OUT& foreground, "
             "const OUT& background )" );

}


template < class IN, class OUT >
gkg::Closing< IN, OUT >::~Closing()
{
}



template < class IN, class OUT >
void gkg::Closing< IN, OUT >::close( const gkg::Volume< IN >& inputVolume,
                                      gkg::Volume< OUT >& closedVolume,
                                      bool verbose ) const
{


  try
  {

    // apply dilation
    gkg::Dilation< IN, uint8_t > dilation( _radius,
                                                 _testFunction,
                                                 1U,
                                                 0U );
    gkg::Volume< uint8_t > dilatedVolume;
    dilation.dilate( inputVolume, dilatedVolume, verbose );

    // apply erosion
    gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
    gkg::Erosion< uint8_t, OUT > erosion( _radius,
                                          equalToFunction,
                                          _foreground,
                                          _background );
    erosion.erode( dilatedVolume, closedVolume, verbose );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::Closing< IN, OUT >::close( "
             "const gkg::Volume< IN >& inputVolume, "
             "gkg::Volume< OUT >& closedVolume, "
             "bool verbose ) const" );

}


template class gkg::Closing< int8_t, uint8_t >;
template class gkg::Closing< uint8_t, uint8_t >;
template class gkg::Closing< int16_t, uint8_t >;
template class gkg::Closing< uint16_t, uint8_t >;
template class gkg::Closing< int32_t, uint8_t >;
template class gkg::Closing< uint32_t, uint8_t >;
template class gkg::Closing< int64_t, uint8_t >;
template class gkg::Closing< uint64_t, uint8_t >;
template class gkg::Closing< float, uint8_t >;
template class gkg::Closing< double, uint8_t >;

template class gkg::Closing< int8_t, int16_t >;
template class gkg::Closing< uint8_t, int16_t >;
template class gkg::Closing< int16_t, int16_t >;
template class gkg::Closing< uint16_t, int16_t >;
template class gkg::Closing< int32_t, int16_t >;
template class gkg::Closing< uint32_t, int16_t >;
template class gkg::Closing< int64_t, int16_t >;
template class gkg::Closing< uint64_t, int16_t >;
template class gkg::Closing< float, int16_t >;
template class gkg::Closing< double, int16_t >;

template class gkg::Closing< int8_t, int32_t >;
template class gkg::Closing< uint8_t, int32_t >;
template class gkg::Closing< int16_t, int32_t >;
template class gkg::Closing< uint16_t, int32_t >;
template class gkg::Closing< int32_t, int32_t >;
template class gkg::Closing< uint32_t, int32_t >;
template class gkg::Closing< int64_t, int32_t >;
template class gkg::Closing< uint64_t, int32_t >;
template class gkg::Closing< float, int32_t >;
template class gkg::Closing< double, int32_t >;

template class gkg::Closing< int8_t, float >;
template class gkg::Closing< uint8_t, float >;
template class gkg::Closing< int16_t, float >;
template class gkg::Closing< uint16_t, float >;
template class gkg::Closing< int32_t, float >;
template class gkg::Closing< uint32_t, float >;
template class gkg::Closing< int64_t, float >;
template class gkg::Closing< uint64_t, float >;
template class gkg::Closing< float, float >;
template class gkg::Closing< double, float >;

template class gkg::Closing< int8_t, double >;
template class gkg::Closing< uint8_t, double >;
template class gkg::Closing< int16_t, double >;
template class gkg::Closing< uint16_t, double >;
template class gkg::Closing< int32_t, double >;
template class gkg::Closing< uint32_t, double >;
template class gkg::Closing< int64_t, double >;
template class gkg::Closing< uint64_t, double >;
template class gkg::Closing< float, double >;
template class gkg::Closing< double, double >;


