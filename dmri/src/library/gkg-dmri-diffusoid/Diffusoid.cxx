#include <gkg-dmri-diffusoid/Diffusoid_i.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/MinimumIndexFilter_i.h>
#include <gkg-processing-algobase/MaximumIndexFilter_i.h>
#include <gkg-processing-coordinates/SO3Interpolator.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <vector>


gkg::Diffusoid::Diffusoid( const gkg::OrientationSet* orientationSet,
                           const gkg::SO3Interpolator* so3Interpolator )
               : _orientationSet( orientationSet ),
                 _so3Interpolator( so3Interpolator )
{

  try
  {

    if ( orientationSet )
    {

      _magnitudes.resize( orientationSet->getCount() );

    }

  }
  GKG_CATCH( "gkg::Diffusoid::Diffusoid( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator )" );

}


gkg::Diffusoid::Diffusoid( const gkg::Diffusoid& other )
               : _orientationSet( other._orientationSet ),
                 _so3Interpolator( other._so3Interpolator ),
                 _magnitudes( other._magnitudes )
{
}


gkg::Diffusoid::~Diffusoid()
{
}


gkg::Diffusoid& gkg::Diffusoid::operator=( const gkg::Diffusoid& other )
{

  try
  {

    _orientationSet = other._orientationSet;
    _so3Interpolator = other._so3Interpolator;
    _magnitudes = other._magnitudes;

    return *this;

  }
  GKG_CATCH( "gkg::Diffusoid& "
             "gkg::Diffusoid::operator=( const gkg::Diffusoid& other )" );

}


void gkg::Diffusoid::setMagnitudes( const std::vector< float >& magnitudes )
{

  try
  {

    _magnitudes = magnitudes;

  }
  GKG_CATCH( "void gkg::Diffusoid::"
             "setMagnitudes( "
             "const std::vector< float >& magnitudes )" );

}


void gkg::Diffusoid::setOrientationSet(
                                     const gkg::OrientationSet* orientationSet )
{

  try
  {

    if ( orientationSet == 0 )
    {

      throw std::runtime_error( "nul orientation set pointer" );

    }
    _orientationSet = orientationSet;

  }
  GKG_CATCH( "void gkg::Diffusoid::setOrientationSet( "
             "const gkg::OrientationSet* orientationSet )" );

}


const std::vector< float >& gkg::Diffusoid::getMagnitudes() const
{

  return _magnitudes;

}


float gkg::Diffusoid::getMagnitude( int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= ( int32_t )_magnitudes.size() ) )
    {

      throw std::runtime_error( "invalid index" );

    }

#endif

    return _magnitudes[ index ];

  }
  GKG_CATCH( "float gkg::Diffusoid::getMagnitude( int32_t index ) const" );

}


float gkg::Diffusoid::getMagnitude(
                        const gkg::Vector3d< float >& unitaryOrientation ) const
{

  try
  {

    return _magnitudes[ _orientationSet->getNearestOrientationIndex(
                                                    unitaryOrientation ) ];

  }
  GKG_CATCH( "float gkg::Diffusoid::getMagnitude( "
             "const gkg::Vector3d< float >& unitaryOrientation ) const" );

}


float
gkg::Diffusoid::getMinimumMagnitude() const
{

  try
  {

    float minimum = 0.0;
    gkg::MinimumFilter< std::vector< float >, float > minimumFilter;
    minimumFilter.filter( _magnitudes, minimum );

    return minimum;

  }
  GKG_CATCH( "float gkg::Diffusoid::getMinimumMagnitude() const" );

}


float
gkg::Diffusoid::getMaximumMagnitude() const
{

  try
  {

    float maximum = 0.0;
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    maximumFilter.filter( _magnitudes, maximum );

    return maximum;

  }
  GKG_CATCH( "float gkg::Diffusoid::getMaximumMagnitude() const" );

}


const gkg::Vector3d< float >&
gkg::Diffusoid::getMinimumOrientation() const
{

  try
  {

    int32_t index = 0;
    gkg::MinimumIndexFilter< std::vector< float >, int32_t > minimumIndexFilter;
    minimumIndexFilter.filter( _magnitudes, index );

    return _orientationSet->getOrientation( index );

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::Diffusoid::getMinimumOrientation() "
             "const" );

}


const gkg::Vector3d< float >&
gkg::Diffusoid::getMaximumOrientation() const
{

  try
  {

    int32_t index = 0;
    gkg::MaximumIndexFilter< std::vector< float >, int32_t > maximumIndexFilter;
    maximumIndexFilter.filter( _magnitudes, index );

    return _orientationSet->getOrientation( index );

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::Diffusoid::getMaximumOrientation() "
             "const" );

}


float gkg::Diffusoid::getStandardDeviation() const
{

  try
  {

    int32_t orientationCount = ( int32_t )_magnitudes.size();

    float mean = 1.0 / ( float )orientationCount;
    float stdDev = 0.0;
    float deviation = 0.0;

    std::vector< float >::const_iterator
      p = _magnitudes.begin(),
      pe = _magnitudes.end();
    while ( p != pe )
    {

      deviation = *p - mean;
      stdDev += deviation * deviation;

      ++ p;

    }

    return std::sqrt( stdDev / ( float )( orientationCount - 1 ) );

  }
  GKG_CATCH( "float gkg::Diffusoid::"
             "getStandardDeviation() const" );

}


void gkg::Diffusoid::getReconstructionMatrix(
                                const gkg::HeaderedObject& headeredObject,
                                const gkg::OrientationSet& outputOrientationSet,
                                gkg::Matrix& reconstructionMatrix )
{

  try
  {

    // getting orientation count
    int32_t orientationCount = 0;

    if ( headeredObject.getHeader().hasAttribute( "sizeT" ) )
    {

      headeredObject.getHeader().getAttribute( "sizeT", orientationCount );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'sizeT'" );

    }
/*
    // building b-value set
    std::vector< double > bValues( orientationCount );
    if ( headeredObject.getHeader().hasAttribute( "bvalues" ) )
    {

      headeredObject.getHeader().getAttribute( "bvalues", bValues );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'bvalues'" );

    }
*/
    // building input orientation set
    gkg::GenericObjectList genericObjectListOrientations( orientationCount );
    if ( headeredObject.getHeader().hasAttribute(
                                           "diffusion_gradient_orientations" ) )
    {

      headeredObject.getHeader().getAttribute(
                                              "diffusion_gradient_orientations",
                                              genericObjectListOrientations );

    }
    else
    {

      throw std::runtime_error(
                        "missing attribute 'diffusion_gradient_orientations'" );

    }

    // the input orientation set needs to be symmetrized before processing
    // diffusoid
    std::vector< gkg::Vector3d< float > > orientations( 2 * orientationCount );
    int32_t o;
    for ( o = 0; o < orientationCount; o++ )
    {

      gkg::GenericObjectList tmp =
      genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList >();
      
      orientations[ o ].x = ( float )tmp[ 0 ]->getScalar();
      orientations[ o ].y = ( float )tmp[ 1 ]->getScalar();
      orientations[ o ].z = ( float )tmp[ 2 ]->getScalar();
      orientations[ o + orientationCount ] = -orientations[ o ];

    }
    gkg::OrientationSet inputOrientationSet( orientations );

    // measure count
    int32_t M = inputOrientationSet.getCount();

    // reconstruction point count
    int32_t N = outputOrientationSet.getCount();

    // allocating final reconstruction matrix
    reconstructionMatrix.reallocate( N, M );
    reconstructionMatrix.fill( 0.0 );

    // building reinterpolation matrix
    int32_t m, n;
    for ( n = 0; n < N; n++ )
    {

      std::vector< std::pair< int32_t, float > >
        indicesAndWeights = _so3Interpolator->getNeighborIndicesAndWeights(
                                     inputOrientationSet,
                                     outputOrientationSet.getOrientation( n ) );
      for ( m = 0; m < ( int32_t )indicesAndWeights.size(); m++ )
      {

        reconstructionMatrix( n, indicesAndWeights[ m ].first ) =
          indicesAndWeights[ m ].second;

      }

    }

  }
  GKG_CATCH( "void gkg::Diffusoid::getReconstructionMatrix( "
             "const gkg::HeaderedObject& headeredObject, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "gkg::Matrix& reconstructionMatrix)" );

}
