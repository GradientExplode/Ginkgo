#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-morphology/MaurerDistanceMapper.h>
#include <gkg-core-exception/Exception.h>


#define EPSILON 0.01


template < class IN, class OUT >
gkg::Dilation< IN, OUT >::Dilation( float radius,
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

       throw std::runtime_error( "radius of dilation should be "
                                 "strictly positive" );

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "gkg::Dilation< IN, OUT >::Dilation( "
             "float radius, "
             "const gkg::TestFunction< IN >& testFunction, "
             "const OUT& foreground, "
             "const OUT& background )" );

}


template < class IN, class OUT >
gkg::Dilation< IN, OUT >::~Dilation()
{
}



template < class IN, class OUT >
void gkg::Dilation< IN, OUT >::dilate( const gkg::Volume< IN >& inputVolume,
                                       gkg::Volume< OUT >& dilatedVolume,
                                       bool verbose ) const
{


  try
  {

    gkg::MaurerDistanceMapper< IN > distanceMapper( _testFunction );
    gkg::Volume< float > distance;

    distanceMapper.getDistance( inputVolume, distance, verbose );

    gkg::LowerOrEqualToFunction< float >
      lowerOrEqualToFunction( _radius + EPSILON );
    gkg::Binarizer< gkg::Volume< float >,
                    gkg::Volume< OUT > > binarizer( lowerOrEqualToFunction,
                                                    _foreground,
                                                    _background );

    binarizer.binarize( distance, dilatedVolume );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::Dilation< IN, OUT >::dilate( "
             "const gkg::Volume< IN >& inputVolume, "
             "gkg::Volume< OUT >& dilatedVolume, "
             "bool verbose ) const" );

}


template class gkg::Dilation< int8_t, uint8_t >;
template class gkg::Dilation< uint8_t, uint8_t >;
template class gkg::Dilation< int16_t, uint8_t >;
template class gkg::Dilation< uint16_t, uint8_t >;
template class gkg::Dilation< int32_t, uint8_t >;
template class gkg::Dilation< uint32_t, uint8_t >;
template class gkg::Dilation< int64_t, uint8_t >;
template class gkg::Dilation< uint64_t, uint8_t >;
template class gkg::Dilation< float, uint8_t >;
template class gkg::Dilation< double, uint8_t >;

template class gkg::Dilation< int8_t, int16_t >;
template class gkg::Dilation< uint8_t, int16_t >;
template class gkg::Dilation< int16_t, int16_t >;
template class gkg::Dilation< uint16_t, int16_t >;
template class gkg::Dilation< int32_t, int16_t >;
template class gkg::Dilation< uint32_t, int16_t >;
template class gkg::Dilation< int64_t, int16_t >;
template class gkg::Dilation< uint64_t, int16_t >;
template class gkg::Dilation< float, int16_t >;
template class gkg::Dilation< double, int16_t >;

template class gkg::Dilation< int8_t, int32_t >;
template class gkg::Dilation< uint8_t, int32_t >;
template class gkg::Dilation< int16_t, int32_t >;
template class gkg::Dilation< uint16_t, int32_t >;
template class gkg::Dilation< int32_t, int32_t >;
template class gkg::Dilation< uint32_t, int32_t >;
template class gkg::Dilation< int64_t, int32_t >;
template class gkg::Dilation< uint64_t, int32_t >;
template class gkg::Dilation< float, int32_t >;
template class gkg::Dilation< double, int32_t >;

template class gkg::Dilation< int8_t, float >;
template class gkg::Dilation< uint8_t, float >;
template class gkg::Dilation< int16_t, float >;
template class gkg::Dilation< uint16_t, float >;
template class gkg::Dilation< int32_t, float >;
template class gkg::Dilation< uint32_t, float >;
template class gkg::Dilation< int64_t, float >;
template class gkg::Dilation< uint64_t, float >;
template class gkg::Dilation< float, float >;
template class gkg::Dilation< double, float >;

template class gkg::Dilation< int8_t, double >;
template class gkg::Dilation< uint8_t, double >;
template class gkg::Dilation< int16_t, double >;
template class gkg::Dilation< uint16_t, double >;
template class gkg::Dilation< int32_t, double >;
template class gkg::Dilation< uint32_t, double >;
template class gkg::Dilation< int64_t, double >;
template class gkg::Dilation< uint64_t, double >;
template class gkg::Dilation< float, double >;
template class gkg::Dilation< double, double >;


#undef EPSILON
