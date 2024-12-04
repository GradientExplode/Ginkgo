#include <gkg-processing-morphology/Erosion.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-algobase/InverseFunction_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-morphology/MaurerDistanceMapper.h>
#include <gkg-core-exception/Exception.h>


#define EPSILON 0.01f


template < class IN, class OUT >
gkg::Erosion< IN, OUT >::Erosion( float radius,
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

       throw std::runtime_error( "radius of erosion should be "
                                 "strictly positive" );

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "gkg::Erosion< IN, OUT >::Erosion( "
             "float radius, "
             "const gkg::TestFunction< IN >& testFunction, "
             "const OUT& foreground, "
             "const OUT& background )" );

}


template < class IN, class OUT >
gkg::Erosion< IN, OUT >::~Erosion()
{
}


template < class IN, class OUT >
void gkg::Erosion< IN, OUT >::erode( const gkg::Volume< IN >& inputVolume,
                                     gkg::Volume< OUT >& erodedVolume,
                                     bool verbose ) const
{

  try
  {

    gkg::InverseFunction< IN >*
      inverseFunction = new InverseFunction< IN >( _testFunction );

    gkg::MaurerDistanceMapper< IN > distanceMapper( *inverseFunction );
    gkg::Volume< float > distance;

    distanceMapper.getDistance( inputVolume, distance, verbose );


    gkg::GreaterThanFunction< float > greaterThanFunction( _radius + EPSILON );
    gkg::Binarizer< gkg::Volume< float >,
                    gkg::Volume< OUT > > binarizer( greaterThanFunction,
                                                    _foreground,
                                                    _background );

    binarizer.binarize( distance, erodedVolume );

    delete inverseFunction;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::Erosion< IN, OUT >::erode( "
             "const gkg::Volume< IN >& inputVolume, "
             "gkg::Volume< OUT >& erodedVolume, "
             "bool verbose ) const" );

}


template class gkg::Erosion< int8_t, uint8_t >;
template class gkg::Erosion< uint8_t, uint8_t >;
template class gkg::Erosion< int16_t, uint8_t >;
template class gkg::Erosion< uint16_t, uint8_t >;
template class gkg::Erosion< int32_t, uint8_t >;
template class gkg::Erosion< uint32_t, uint8_t >;
template class gkg::Erosion< int64_t, uint8_t >;
template class gkg::Erosion< uint64_t, uint8_t >;
template class gkg::Erosion< float, uint8_t >;
template class gkg::Erosion< double, uint8_t >;

template class gkg::Erosion< int8_t, int16_t >;
template class gkg::Erosion< uint8_t, int16_t >;
template class gkg::Erosion< int16_t, int16_t >;
template class gkg::Erosion< uint16_t, int16_t >;
template class gkg::Erosion< int32_t, int16_t >;
template class gkg::Erosion< uint32_t, int16_t >;
template class gkg::Erosion< int64_t, int16_t >;
template class gkg::Erosion< uint64_t, int16_t >;
template class gkg::Erosion< float, int16_t >;
template class gkg::Erosion< double, int16_t >;

template class gkg::Erosion< int8_t, int32_t >;
template class gkg::Erosion< uint8_t, int32_t >;
template class gkg::Erosion< int16_t, int32_t >;
template class gkg::Erosion< uint16_t, int32_t >;
template class gkg::Erosion< int32_t, int32_t >;
template class gkg::Erosion< uint32_t, int32_t >;
template class gkg::Erosion< int64_t, int32_t >;
template class gkg::Erosion< uint64_t, int32_t >;
template class gkg::Erosion< float, int32_t >;
template class gkg::Erosion< double, int32_t >;

template class gkg::Erosion< int8_t, float >;
template class gkg::Erosion< uint8_t, float >;
template class gkg::Erosion< int16_t, float >;
template class gkg::Erosion< uint16_t, float >;
template class gkg::Erosion< int32_t, float >;
template class gkg::Erosion< uint32_t, float >;
template class gkg::Erosion< int64_t, float >;
template class gkg::Erosion< uint64_t, float >;
template class gkg::Erosion< float, float >;
template class gkg::Erosion< double, float >;

template class gkg::Erosion< int8_t, double >;
template class gkg::Erosion< uint8_t, double >;
template class gkg::Erosion< int16_t, double >;
template class gkg::Erosion< uint16_t, double >;
template class gkg::Erosion< int32_t, double >;
template class gkg::Erosion< uint32_t, double >;
template class gkg::Erosion< int64_t, double >;
template class gkg::Erosion< uint64_t, double >;
template class gkg::Erosion< float, double >;
template class gkg::Erosion< double, double >;


#undef EPSILON
