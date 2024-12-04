#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-core-io/TextFileLineCount.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/StdDevFilter_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <algorithm>


struct IndexedDistance
{

  float distance;
  int32_t index;

};


static int32_t compareDistance( const void *element1, const void *element2 )
{

  return ( int32_t )( ( ( ( IndexedDistance* )element1 )->distance -
                        ( ( IndexedDistance* )element2 )->distance ) * 100000 );
}




gkg::OrientationSet::OrientationSet()
{
}


gkg::OrientationSet::OrientationSet( const std::string& fileName )
{

  try
  {

    int32_t lineCount = gkg::getTextFileLineCount( fileName );
    std::vector< int32_t > itemCounts = gkg::getScalarItemCountPerLine(
                                                                     fileName );

    ////////////////////////////////////////////////////////////////////////////
    // in case we provide a *.bvec file
    ////////////////////////////////////////////////////////////////////////////

    if ( ( lineCount == 3 ) &&
         ( itemCounts[ 0 ] == itemCounts[ 1 ] ) &&
         ( itemCounts[ 0 ] == itemCounts[ 2 ] ) )
    {

      int32_t count = itemCounts[ 0 ];
      _orientations.resize( count );

      // reading orientations from file
      gkg::UncommentCounterInputFileStream is( fileName.c_str() );
      int32_t i = 0;
      for ( i = 0; i < count; i++ )
      {

        is >> _orientations[ i ].x;

      }
      for ( i = 0; i < count; i++ )
      {

        is >> _orientations[ i ].y;

      }
      for ( i = 0; i < count; i++ )
      {

        is >> _orientations[ i ].z;

      }
      is.close();


    }
    ////////////////////////////////////////////////////////////////////////////
    // in case we provide a GKG orientation file
    ////////////////////////////////////////////////////////////////////////////
    else
    {

      // reading orientations from file
      gkg::UncommentCounterInputFileStream is( fileName.c_str() );
      int32_t count = 0;
      is >> count;
      _orientations.resize( count );
      int32_t i = 0;
      for ( i = 0; i < count; i++ )
      {

        is >> _orientations[ i ].x
           >> _orientations[ i ].y
           >> _orientations[ i ].z;

      }
      is.close();

    }

    // building orientation indices of nearest symmetrical orientations
    _nearestSymmetricalOrientationIndices.resize( _orientations.size() );

    std::vector< gkg::Vector3d< float > >::const_iterator
      o = _orientations.begin(),
      oe = _orientations.end();
    std::vector< int32_t >::iterator
      s = _nearestSymmetricalOrientationIndices.begin();
    while ( o != oe )
    {

      *s = getNearestOrientationIndex( -*o );
      ++ o;
      ++ s;

    }


  }
  GKG_CATCH( "gkg::OrientationSet::OrientationSet( "
             "const std::string& fileName )" );

}


gkg::OrientationSet::OrientationSet( int32_t count )
                    : _orientations( count )
{
}


gkg::OrientationSet::OrientationSet(
                     const std::vector< gkg::Vector3d< float > >& orientations )
                    : _orientations( orientations )
{

  try
  {

    // building orientation indices of nearest symmetrical orientations
    _nearestSymmetricalOrientationIndices.resize( _orientations.size() );

    std::vector< gkg::Vector3d< float > >::const_iterator
      o = _orientations.begin(),
      oe = _orientations.end();
    std::vector< int32_t >::iterator
      s = _nearestSymmetricalOrientationIndices.begin();
    while ( o != oe )
    {

      *s = getNearestOrientationIndex( -*o );
      ++ o;
      ++ s;

    }

  }
  GKG_CATCH( "gkg::OrientationSet::OrientationSet( "
             "const std::vector< Vector3d< float > >& orientations )" );

}


gkg::OrientationSet::OrientationSet( const gkg::OrientationSet& other )
                    : _orientations( other._orientations ),
                      _nearestSymmetricalOrientationIndices(
                                   other._nearestSymmetricalOrientationIndices )
{
}


gkg::OrientationSet&
gkg::OrientationSet::operator=( const gkg::OrientationSet& other )
{

  try
  {

    _orientations = other._orientations;
    _nearestSymmetricalOrientationIndices =
      other._nearestSymmetricalOrientationIndices;
    return *this;

  }
  GKG_CATCH( "gkg::OrientationSet& "
             "gkg::OrientationSet::operator=( "
             "const gkg::OrientationSet& other )" );

}


gkg::OrientationSet::~OrientationSet()
{
}


gkg::OrientationSet::iterator gkg::OrientationSet::begin()
{

  return _orientations.begin();

}


gkg::OrientationSet::iterator gkg::OrientationSet::end()
{

  return _orientations.end();

}


gkg::OrientationSet::const_iterator gkg::OrientationSet::begin() const
{

  return _orientations.begin();

}


gkg::OrientationSet::const_iterator gkg::OrientationSet::end() const
{

  return _orientations.end();

}


gkg::OrientationSet::reverse_iterator gkg::OrientationSet::rbegin()
{

  return _orientations.rbegin();

}


gkg::OrientationSet::reverse_iterator gkg::OrientationSet::rend()
{

  return _orientations.rend();

}


gkg::OrientationSet::const_reverse_iterator gkg::OrientationSet::rbegin() const
{

  return _orientations.rbegin();

}


gkg::OrientationSet::const_reverse_iterator gkg::OrientationSet::rend() const
{

  return _orientations.rend();

}


int32_t gkg::OrientationSet::getCount() const
{

  return ( int32_t )_orientations.size();

}


const gkg::Vector3d< float >& 
gkg::OrientationSet::getOrientation( int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= getCount() ) )
    {

      throw std::runtime_error( "invalid index" );

    }

#endif

    return _orientations[ index ];

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::OrientationSet::getOrientation( int32_t index ) const" );


}


gkg::Vector3d< float >
gkg::OrientationSet::getSymmetricalOrientation( int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= getCount() ) )
    {

      throw std::runtime_error( "invalid index" );

    }

#endif

    return -_orientations[ index ];

  }
  GKG_CATCH( "gkg::Vector3d< float > "
             "gkg::OrientationSet::getSymmetricalOrientation( "
             "int32_t index ) const" );

}

/*
class NearestOrientationIndexContext : public gkg::LoopContext< int32_t >
{


  public:

    NearestOrientationIndexContext( 
      const std::vector< gkg::Vector3d< float > >& orientations,
      const gkg::Vector3d< float >& unitaryOrientation,
      float& bestDot,
      int32_t& bestIndex )
                 : gkg::LoopContext(),
                   _orientations( orientations ),
                   _unitaryOrientation( unitaryOrientation ),
                   _bestDot( bestDot ),
                   _bestIndex( bestIndex ) { }
    ~NearestOrientationIndexContext() { }

    void doIt( int32_t startIndex, int32_t count )
    {

      float bestDot = _orientations[ startIndex ].dot( _unitaryOrientation );
      int32_t bestIndex = startIndex;
      float dot = 0.0;
      for ( int32_t i = startIndex + 1; i < startIndex + count; i++ )
      {

        dot = _orientations[ i ].dot( _unitaryOrientation );
        if ( dot > bestDot )
        {

          bestDot = dot;
          bestIndex = i;

        }

      }
      lock();
      if ( bestDot > _bestDot )
      {

        _bestDot = bestDot;
        _bestIndex = bestIndex;

      }
      unlock();

    }

  private:

    // private parameters of the algorithm
    const std::vector< gkg::Vector3d< float > >& _orientations;
    const gkg::Vector3d< float >& _unitaryOrientation;
    float& _bestDot;
    int32_t& _bestIndex;

};
*/

int32_t gkg::OrientationSet::getNearestOrientationIndex(
                        const gkg::Vector3d< float >& unitaryOrientation ) const
{

  try
  {

    int32_t bestIndex = 0;
    float bestDot = -2.0;
/*
    NearestOrientationIndexContext 
      nearestOrientationIndexContext( _orientations,
                                      unitaryOrientation,
                                      bestDot,
                                      bestIndex );

    gkg::ThreadedLoop< int32_t >
      threadedLoop( &nearestOrientationIndexContext,
                    0,                                // the start index
                    ( int32_t )_orientations.size() ); // the count to be done
    threadedLoop.launch();

*/

    std::vector< gkg::Vector3d< float > >::const_iterator
      o = _orientations.begin(),
      oe = _orientations.end();

    int32_t index = 0;
    float dot = 0.0;
    while ( o != oe )
    {

      dot = o->dot( unitaryOrientation );
      if ( dot > bestDot )
      {

        bestDot = dot;
        bestIndex = index;

      }
      ++ index;
      ++ o;

    }

    return bestIndex;

  }
  GKG_CATCH( "int32_t gkg::OrientationSet::getNearestOrientationIndex( "
             "const gkg::Vector3d< float >& unitaryOrientation ) const" );

}


std::vector< int32_t > gkg::OrientationSet::getNearestOrientationIndices(
                               const gkg::Vector3d< float >& unitaryOrientation,
                               int32_t neighborCount ) const
{

  try
  {

    std::vector< int32_t > bestIndices( neighborCount );
    int32_t o = 0;
    int32_t N = ( int32_t )_orientations.size();
    int32_t N2 = N;
    const gkg::Vector3d< float >* optr = &_orientations[ 0 ];
    IndexedDistance distanceNIndex[ N ];
    IndexedDistance* dptr = &distanceNIndex[ 0 ];

    while ( N-- )
    {

      dptr->distance = gkg::safeAcos( optr->dot( unitaryOrientation ) );
      dptr->index = o++;
      ++optr;
      ++dptr;

    }

    qsort( ( void* )&distanceNIndex,
           N2,
           sizeof( IndexedDistance ),
           compareDistance ); 

    int32_t* bptr = &bestIndices[ 0 ];
    dptr = distanceNIndex;
    N = neighborCount;
    while ( N-- )
    {

      *bptr++ = dptr->index;
      ++dptr;

    }

    return bestIndices;

  }
  GKG_CATCH( "std::vector< int32_t > "
             "gkg::OrientationSet::getNearestOrientationIndices( "
             "const gkg::Vector3d< float >& unitaryOrientation, "
             "int32_t neighborCount ) const" );

}


const gkg::Vector3d< float >& 
gkg::OrientationSet::getNearestSymmetricalOrientation(
                        const gkg::Vector3d< float >& unitaryOrientation ) const
{

  try
  {

    return _orientations[ _nearestSymmetricalOrientationIndices[
                           getNearestOrientationIndex( unitaryOrientation ) ] ];

  }
  GKG_CATCH( "const gkg::Vector3d< float >& getNearestSymmetricalOrientation( "
             "const gkg::Vector3d< float >& unitaryOrientation ) const" );

}


int32_t gkg::OrientationSet::getNearestSymmetricalOrientationIndex(
                                                           int32_t index ) const
{

  try
  {

    return _nearestSymmetricalOrientationIndices[ index ];

  }
  GKG_CATCH( "int32_t gkg::OrientationSet::"
             "getNearestSymmetricalOrientationIndex( "
             "int32_t index ) const" );

}


const std::vector< gkg::Vector3d< float > >& 
gkg::OrientationSet::getOrientations() const
{

  return _orientations;

}


std::vector< gkg::Vector3d< float > >
gkg::OrientationSet::getSymmetricalOrientations() const
{

  std::vector< Vector3d< float > >
    symmetricalOrientations( 2 * _orientations.size() );

  int32_t o;
  for ( o = 0; o < ( int32_t )_orientations.size(); o++ )
  {

    symmetricalOrientations[ o ] = _orientations[ o ];
    symmetricalOrientations[ o + ( int32_t )_orientations.size() ] =
                                  -_orientations[ o ];

  }
  return symmetricalOrientations;

}


void gkg::OrientationSet::getStatistics( float* minimumAngle,
                                         float* maximumAngle,
                                         float* averageAngle,
                                         float* angleStdDev ) const
{

  try
  {

    int32_t count = getCount();
    std::vector< float > angles( count, 0.0f );

    float angle = 0.0;
    int32_t o1, o2;
    for ( o1 = 0; o1 < count; o1++ )
    {

      angles[ o1 ] = 2 * M_PI;
      for ( o2 = 0; o2 < count; o2++ )
      {

        if ( o2 != o1 )
        {

          angle = gkg::safeAcos( _orientations[ o1 ].dot( 
                                                        _orientations[ o2 ] ) );

          if ( angle < angles[ o1 ] )
          {

            angles[ o1 ] = angle;

          }

        }

      }

    }

    if ( minimumAngle )
    {

      gkg::MinimumFilter< std::vector< float >, float > minimumFilter;
      minimumFilter.filter( angles, *minimumAngle );

    }
    if ( maximumAngle )
    {

      gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
      maximumFilter.filter( angles, *maximumAngle );

    }
    if ( averageAngle )
    {

      gkg::AverageFilter< std::vector< float >, float > averageFilter;
      averageFilter.filter( angles, *averageAngle );

    }
    if ( angleStdDev )
    {

      gkg::StdDevFilter< std::vector< float >, float > stdDevFilter;
      stdDevFilter.filter( angles, *angleStdDev );

    }

  }
  GKG_CATCH( "void gkg::OrientationSet::getStatistics( float minimumAngle, "
             "float* maximumAngle, "
             "float* averageAngle, "
             "float* angleStdDev ) const" );

}


void gkg::OrientationSet::getPerpendicularOrientationIndices( 
                                   int32_t index,
                                   float angleError,
                                   std::vector< int32_t >& indices ) const
{

  try
  {

    std::list< int32_t > indexList;

    int32_t o = 0;
    int32_t orientationCount = _orientations.size();
    double angle = 0.0;
    for ( o = 0; o < orientationCount; o++ )
    {

      angle = gkg::safeAcos(
                   ( double )_orientations[ index ].dot( _orientations[ o ] ) );

      if ( gkg::equal( angle, M_PI / 2.0, ( double )angleError ) )
      {

        indexList.push_back( o );

      }

    }

    indices.resize( indexList.size() );
    std::copy( indexList.begin(), indexList.end(), indices.begin() );

  }
  GKG_CATCH( "void gkg::OrientationSet::getPerpendicularOrientationIndices( "
             "int32_t index, "
             "float angleError, "
             "const std::vector< int32_t >& indices ) const" );

}


void gkg::OrientationSet::getSHToCartesianBasisTransform(
                                   int32_t maximumSphericalHarmonicOrder,
                                   gkg::Matrix& shToCartesianBasisMatrix ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // get access to the numerical analysis factory
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    ////////////////////////////////////////////////////////////////////////////
    // computing spherical harmonics to Cartesian basis transformation matrix
    ////////////////////////////////////////////////////////////////////////////

    int32_t shCoefficientCount = ( maximumSphericalHarmonicOrder + 1 ) *
                                 ( maximumSphericalHarmonicOrder + 2 ) / 2;
    int32_t orientationCount = ( int32_t )_orientations.size();

    shToCartesianBasisMatrix.reallocate( orientationCount,
                                         shCoefficientCount );


    int32_t o = 0;
    int32_t n = 0;
    double radial = 0.0;
    double theta = 0.0;
    double phi = 0.0;
    for ( o = 0; o < orientationCount; o++ )
    {

      const gkg::Vector3d< float >& 
        orientation = _orientations[ o ];
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      for ( n = 0; n < shCoefficientCount; n++ )
      {

        shToCartesianBasisMatrix( o, n ) =
          factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

  }
  GKG_CATCH( "void gkg::OrientationSet::getSHToCartesianBasisTransform( "
             "int32_t maximumSphericalHarmonicOrder ) const" );

}


void gkg::OrientationSet::getCartesianToSHBasisTransform(
                                   int32_t maximumSphericalHarmonicOrder,
                                   gkg::Matrix& cartesianToSHBasisMatrix ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // get access to the numerical analysis factory
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    ////////////////////////////////////////////////////////////////////////////
    // computing spherical harmonics to Cartesian basis transformation matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix shToCartesianBasisTransform;
    this->getSHToCartesianBasisTransform( maximumSphericalHarmonicOrder,
                                          shToCartesianBasisTransform );


    ////////////////////////////////////////////////////////////////////////////
    // computing Cartesian to spherical harmonics basis transformation matrix
    ////////////////////////////////////////////////////////////////////////////

    cartesianToSHBasisMatrix.reallocate(
                                       shToCartesianBasisTransform.getSize2(),
                                       shToCartesianBasisTransform.getSize1() );
    factory->getMoorePenrosePseudoInverse( shToCartesianBasisTransform,
                                           cartesianToSHBasisMatrix );

  }
  GKG_CATCH( "void gkg::OrientationSet::getCartesianToSHBasisTransform( "
             "int32_t maximumSphericalHarmonicOrder ) const" );

}

