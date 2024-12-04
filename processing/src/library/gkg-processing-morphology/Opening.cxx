#include <gkg-processing-morphology/Opening.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-morphology/Erosion.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-core-exception/Exception.h>


template < class IN, class OUT >
gkg::Opening< IN, OUT >::Opening( float radius,
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
             "gkg::Opening< IN, OUT >::Opening( "
             "float radius, "
             "const gkg::TestFunction< IN >& testFunction, "
             "const OUT& foreground, "
             "const OUT& background )" );

}


template < class IN, class OUT >
gkg::Opening< IN, OUT >::~Opening()
{
}



template < class IN, class OUT >
void gkg::Opening< IN, OUT >::open( const gkg::Volume< IN >& inputVolume,
                                    gkg::Volume< OUT >& openedVolume,
                                    bool verbose ) const
{


  try
  {

    // apply erosion
    gkg::Erosion< IN, uint8_t > erosion( _radius,
                                         _testFunction,
                                         1U,
                                         0U );
    gkg::Volume< uint8_t > erodedVolume;
    erosion.erode( inputVolume, erodedVolume, verbose );

    // apply dilation
    gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
    gkg::Dilation< uint8_t, OUT > dilation( _radius,
                                            equalToFunction,
                                            _foreground,
                                            _background );
    dilation.dilate( erodedVolume, openedVolume, verbose );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::Opening< IN, OUT >::open( "
             "const gkg::Volume< IN >& inputVolume, "
             "gkg::Volume< OUT >& openedVolume, "
             "bool verbose ) const" );

}


template class gkg::Opening< int8_t, uint8_t >;
template class gkg::Opening< uint8_t, uint8_t >;
template class gkg::Opening< int16_t, uint8_t >;
template class gkg::Opening< uint16_t, uint8_t >;
template class gkg::Opening< int32_t, uint8_t >;
template class gkg::Opening< uint32_t, uint8_t >;
template class gkg::Opening< int64_t, uint8_t >;
template class gkg::Opening< uint64_t, uint8_t >;
template class gkg::Opening< float, uint8_t >;
template class gkg::Opening< double, uint8_t >;

template class gkg::Opening< int8_t, int16_t >;
template class gkg::Opening< uint8_t, int16_t >;
template class gkg::Opening< int16_t, int16_t >;
template class gkg::Opening< uint16_t, int16_t >;
template class gkg::Opening< int32_t, int16_t >;
template class gkg::Opening< uint32_t, int16_t >;
template class gkg::Opening< int64_t, int16_t >;
template class gkg::Opening< uint64_t, int16_t >;
template class gkg::Opening< float, int16_t >;
template class gkg::Opening< double, int16_t >;

template class gkg::Opening< int8_t, int32_t >;
template class gkg::Opening< uint8_t, int32_t >;
template class gkg::Opening< int16_t, int32_t >;
template class gkg::Opening< uint16_t, int32_t >;
template class gkg::Opening< int32_t, int32_t >;
template class gkg::Opening< uint32_t, int32_t >;
template class gkg::Opening< int64_t, int32_t >;
template class gkg::Opening< uint64_t, int32_t >;
template class gkg::Opening< float, int32_t >;
template class gkg::Opening< double, int32_t >;

template class gkg::Opening< int8_t, float >;
template class gkg::Opening< uint8_t, float >;
template class gkg::Opening< int16_t, float >;
template class gkg::Opening< uint16_t, float >;
template class gkg::Opening< int32_t, float >;
template class gkg::Opening< uint32_t, float >;
template class gkg::Opening< int64_t, float >;
template class gkg::Opening< uint64_t, float >;
template class gkg::Opening< float, float >;
template class gkg::Opening< double, float >;

template class gkg::Opening< int8_t, double >;
template class gkg::Opening< uint8_t, double >;
template class gkg::Opening< int16_t, double >;
template class gkg::Opening< uint16_t, double >;
template class gkg::Opening< int32_t, double >;
template class gkg::Opening< uint32_t, double >;
template class gkg::Opening< int64_t, double >;
template class gkg::Opening< uint64_t, double >;
template class gkg::Opening< float, double >;
template class gkg::Opening< double, double >;
