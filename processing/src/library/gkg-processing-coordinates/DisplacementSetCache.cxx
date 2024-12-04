#include <gkg-processing-coordinates/DisplacementSetCache.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>
#include <cstdio>
#include <cmath>
#include <list>


gkg::DisplacementSetCache::DisplacementSetCache()
                          : _multipleShellSampling( 0 ),
                            _cartesianGridSize( 0 ),
                            _cartesianGridResolution( 0 )
{
}


gkg::DisplacementSetCache::~DisplacementSetCache()
{
}


void 
gkg::DisplacementSetCache::update( 
           const std::map< float, gkg::OrientationSet >& multipleShellSampling )
{

  try
  {

    _mutex.lock();
    if ( &multipleShellSampling != _multipleShellSampling )
    {

      // updating protected field(s)
      _multipleShellSampling = &multipleShellSampling;

      int32_t displacementCount = 0;
      std::map< float, gkg::OrientationSet >::const_iterator
        s = _multipleShellSampling->begin(),
        se = _multipleShellSampling->end();
      while ( s != se )
      {

        displacementCount += s->second.getCount();
        ++ s;

      }
      _displacements.resize( displacementCount );
      s = _multipleShellSampling->begin();
      std::vector< gkg::Vector3d< float > >::iterator
        d = _displacements.begin();
      while ( s != se )
      {

        const float& radius = s->first;
        const gkg::OrientationSet& orientationSet = s->second;

        gkg::OrientationSet::const_iterator
          o = orientationSet.begin(),
          oe = orientationSet.end();
        while ( o != oe )
        {

          *d = *o * radius;
          ++ o;
          ++ d;

        }

        ++ s;

      }

    }
    _mutex.unlock();

  }
  GKG_CATCH( "void "
             "gkg::DisplacementSetCache::update( "
             "const std::map< float, gkg::OrientationSet >& "
             "multipleShellSampling )" );

}


void 
gkg::DisplacementSetCache::update( 
                        const gkg::Vector3d< int32_t >& cartesianGridSize,
                        const gkg::Vector3d< double >& cartesianGridResolution )
{

  try
  {

    _mutex.lock();
    if ( ( &cartesianGridSize != _cartesianGridSize ) ||
         ( &cartesianGridResolution != _cartesianGridResolution ) )
    {

      // updating protected field(s)
      _cartesianGridSize = &cartesianGridSize;
      _cartesianGridResolution = &cartesianGridResolution;

      int32_t displacementCount = ( cartesianGridSize.x * 2 + 1 ) *
                                  ( cartesianGridSize.y * 2 + 1 ) *
                                  ( cartesianGridSize.z * 2 + 1 );
                                  
      _displacements.resize( displacementCount );
      std::vector< gkg::Vector3d< float > >::iterator
        d = _displacements.begin();

      int32_t i = 0;
      int32_t j = 0;
      int32_t k = 0;
      for ( k = -cartesianGridSize.z; k <= cartesianGridSize.z; k++ )
      {

        for ( j = -cartesianGridSize.y; j <= cartesianGridSize.y; j++ )
        {

          for ( i = -cartesianGridSize.x; i <= cartesianGridSize.x; i++ )
          {

            d->x = ( float )i * ( float )cartesianGridResolution.x;
            d->y = ( float )j * ( float )cartesianGridResolution.y;
            d->z = ( float )k * ( float )cartesianGridResolution.z;


          }

        }

      }

    }
    _mutex.unlock();

  }
  GKG_CATCH( "void "
             "gkg::DisplacementSetCache::update( "
             "const gkg::Vector3d< int32_t >& cartesianGridSize, "
             "const gkg::Vector3d< double >& cartesianGridResolution )" );

}


const std::map< float, gkg::OrientationSet >* 
gkg::DisplacementSetCache::getMultiShellSampling() const
{

  try
  {

    return _multipleShellSampling;

  }
  GKG_CATCH( "const std::map< float, gkg::OrientationSet >* "
             "gkg::DisplacementSetCache::getMultiShellSampling() const" );

}


const gkg::Vector3d< int32_t >* 
gkg::DisplacementSetCache::getCartesianGridSize() const
{

  try
  {

    return _cartesianGridSize;

  }
  GKG_CATCH( "const gkg::Vector3d< int32_t >* "
             "gkg::DisplacementSetCache::getCartesianGridSize() const" );

}


const gkg::Vector3d< double >* 
gkg::DisplacementSetCache::getCartesianGridResolution() const
{

  try
  {

    return _cartesianGridResolution;

  }
  GKG_CATCH( "const gkg::Vector3d< double >* "
             "gkg::DisplacementSetCache::getCartesianGridResolution() const" );

}




int32_t gkg::DisplacementSetCache::getCount() const
{

  try
  {

    return ( int32_t )_displacements.size();

  }
  GKG_CATCH( "int32_t "
             "gkg::DisplacementSetCache::getCount() const" );

}


const gkg::Vector3d< float >& 
gkg::DisplacementSetCache::getDisplacement( int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( index >= ( int32_t )_displacements.size() )
    {

      char message[ 256 ];
      sprintf( message, "invalid index ( %d )", index );
      throw std::runtime_error( message );

    }

#endif

    return _displacements[ index ];

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::DisplacementSetCache::getDisplacement( "
             "int32_t index ) const" );

}


const std::vector< gkg::Vector3d< float > >& 
gkg::DisplacementSetCache::getDisplacements() const
{

  try
  {

    return _displacements;

  }
  GKG_CATCH( "const std::vector< gkg::Vector3d< float > >& "
             "gkg::DisplacementSetCache::getDisplacements() const" );

}


int32_t gkg::DisplacementSetCache::getNearestDisplacementIndex(
                              const gkg::Vector3d< float >& displacement ) const
{

  try
  {

    int32_t index = 0;
    int32_t optimalIndex = 0;
    float optimalDistance = 1e38;
    float distance = 1e38;

    std::vector< gkg::Vector3d< float > >::const_iterator
      d = _displacements.begin(),
      de = _displacements.end();
    while ( d != de )
    {

      distance = ( *d - displacement ).getNorm2();
      if ( distance < optimalDistance )
      {

        optimalIndex = index;
        optimalDistance = distance;

      }
      ++ d;
      ++ index;

    }
    return optimalIndex;

  }
  GKG_CATCH( "int32_t gkg::DisplacementSetCache::getNearestDisplacementIndex( "
             "const gkg::Vector3d< float >& displacement ) const" );

}

