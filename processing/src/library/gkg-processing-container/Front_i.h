#ifndef _gkg_processing_container_Front_i_h_
#define _gkg_processing_container_Front_i_h_


#include <gkg-processing-container/Front.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>


#define BACKGROUND_LABEL    0


template < class T >
inline
gkg::Front< T >::Front( gkg::RCPointer< gkg::Volume< T > > labelVolume,
                        const T& label,
                        int32_t stride )
                : _labelVolume( labelVolume ),
                  _label( label ),
                  _randomGenerator( gkg::RandomGenerator::Gfsr4 )
{

  try
  {

    _boundingBox = gkg::BoundingBox< int32_t >( * labelVolume );
    _neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets( 
                                         gkg::Neighborhood3d::Neighborhood3d_26,
                                         stride );

    int32_t sizeX = labelVolume->getSizeX();
    int32_t sizeY = labelVolume->getSizeY();
    int32_t sizeZ = labelVolume->getSizeZ();
    gkg::Vector3d< int32_t > site;
    gkg::Vector3d< int32_t > neighbor;
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      n = _neighbors.begin(),
      ne = _neighbors.end();

    for ( site.z = 0; site.z < sizeZ; site.z += stride )
    {

      for ( site.y = 0; site.y < sizeY; site.y += stride )
      {

        for ( site.x = 0; site.x < sizeX; site.x += stride )
        {

          if ( ( * _labelVolume)( site ) == label )
          {

            n = _neighbors.begin();
            while ( n != ne )
            {

              neighbor = site + *n;

              if ( _boundingBox.contains( neighbor ) &&
                   ( ( * labelVolume )( neighbor ) != label ) )
              {

                _sites.push_back( site );
                _uniqueSites.insert( site );
                break;

              }
              ++ n;

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::Front< T >::Front( gkg::RCPointer< gkg::Volume< T > > "
             "labelVolume, "
             "const T& label, "
             "int32_t stride )" );

}


template < class T >
inline
gkg::Front< T >::Front( const gkg::Front< T >& other )
                : _sites( other._sites ),
                  _uniqueSites( other._uniqueSites ),
                  _labelVolume( other._labelVolume ),
                  _label( other._label ),
                  _randomGenerator( gkg::RandomGenerator::Gfsr4 ),
                  _boundingBox( other._boundingBox ),
                  _neighbors( other._neighbors )
{
}

template < class T >
inline
gkg::Front< T >::~Front()
{
}

template < class T >
inline
gkg::Front< T >& gkg::Front< T >::operator=( const gkg::Front< T >& other )
{

  _sites = other._sites;
  _uniqueSites = other._uniqueSites;
  _labelVolume = other._labelVolume;
  _label = other._label;
  _randomGenerator = other._randomGenerator;
  _boundingBox = other._boundingBox;
  _neighbors = other._neighbors;

  return *this;

}

template < class T >
inline
int32_t gkg::Front< T >::getSiteCount() const
{

  try
  {

    return ( int32_t )_sites.size();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "int32_t gkg::Front< T >::getSiteCount() const" );

}


template < class T >
inline
const gkg::Vector3d< int32_t >&  gkg::Front< T >::getSite( int32_t index ) const
{

  try
  {

#if defined( GKG_DEBUG ) || defined ( GKG_DEFAULT )  

    if ( index >= getSiteCount() )
    {

      throw std::runtime_error( "site index overflow" );

    }

#endif

    return _sites[ index ];

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const gkg::Vector3d< int32_t >&"
             "gkg::Front< T >::getSite( int32_t index ) const" );

}


template < class T >
inline
const std::deque< gkg::Vector3d< int32_t > >& gkg::Front< T >::getSites() const
{

  return _sites;

}

template < class T >
inline
const std::set< gkg::Vector3d< int32_t >, gkg::Vector3dCompare< int32_t > >& 
        gkg::Front< T >::getUniqueSites() const
{

  return _uniqueSites;

}

template < class T >
inline
bool gkg::Front< T >::hasSite( const gkg::Vector3d< int32_t >& site ) const
{

  try
  {

    if ( _uniqueSites.find( site ) != _uniqueSites.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "bool gkg::Front< T >::hasSite( "
             "const gkg::Vector3d< int32_t >& site ) const" );

}

template < class T >
inline
void gkg::Front< T >::addSite( const gkg::Vector3d< int32_t >& site )
{

  try
  {

    if ( _uniqueSites.find( site ) != _uniqueSites.end() )
    {

      throw std::runtime_error( "site already present in front" );

    }
    else
    {

      _sites.push_back( site );
      _uniqueSites.insert( site );

      // removing from front the sites that do not belong anymore to the front

      // looping over the neighbors of site
      std::list< gkg::Vector3d< int32_t > >::const_iterator
        n1 = _neighbors.begin(),
        n1e = _neighbors.end();
      std::list< gkg::Vector3d< int32_t > >::const_iterator
        n2 = _neighbors.begin(),
        n2e = _neighbors.end();

      gkg::Vector3d< int32_t > neighbor;
      gkg::Vector3d< int32_t > neighborOfNeighbor;

      while ( n1 != n1e )
      {

        neighbor = site + *n1;

        if ( _boundingBox.contains( neighbor ) )
        {

          if ( ( * _labelVolume )( neighbor ) == _label )
          {

            bool noMoreInFront = true;
            n2 = _neighbors.begin();

            // looping over the neighbors of the neighbor to check if it is a 
            // front site
            while ( n2 != n2e )
            {

              // looping over the neighbors of current site to check if neighbor
              // should be removed or not from the front
              neighborOfNeighbor = neighbor + *n2;
              if( ( ( * _labelVolume )( neighborOfNeighbor ) != _label ) &&
                  ( neighborOfNeighbor != site ) )
              {

                noMoreInFront = false;
                break;

              }
              ++ n2;

            }

            if( noMoreInFront )
            {

              std::deque< gkg::Vector3d< int32_t > >::iterator
                s = _sites.begin(),
                se = _sites.end();
              while ( s != se )
              {

                if ( *s == neighbor )
                {

                  _sites.erase( s );
                  _uniqueSites.erase( neighbor );
                  break;

                }
                ++ s;

              }

            }

          }

        }
        ++ n1;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Front< T >::addSite(" 
             "const gkg::Vector3d< int32_t >& site )" );

}

template < class T >
inline
void gkg::Front< T >::removeSite( const gkg::Vector3d< int32_t >& site )
{

  try
  {

    // removing site from deque
    std::deque< gkg::Vector3d< int32_t > >::iterator
      s = _sites.begin(),
      se = _sites.end();
    while ( s != se )
    {

      if ( *s == site )
      {

        _sites.erase( s );
        _uniqueSites.erase( site );

        // adding to the front the sites that did not belong to the front
        // and that will be part of the front

        //looping over the neighbors of site
        std::list< gkg::Vector3d< int32_t > >::const_iterator
          n = _neighbors.begin(),
          ne = _neighbors.end();
        gkg::Vector3d< int32_t > neighbor;
        while ( n != ne )
        {

          neighbor = site + *n;
          if ( _boundingBox.contains( neighbor ) )
          {

            if ( ( ( * _labelVolume )( neighbor ) == _label ) &&
                 ( !hasSite( neighbor ) ) )
            {

              // adding neighbour to the front
              _sites.push_back( neighbor );
              _uniqueSites.insert( neighbor );

            }

          }
          ++ n;

        }
        break;

      }
      ++ s;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Front< T >::removeSite(" 
             "const gkg::Vector3d< int32_t >& site )" );

}


template < class T >
inline
void gkg::Front< T >::getRandomSite( gkg::Vector3d< int32_t >& site ) const
{

  try
  {

    // selecting randomly a front and setting the label
    int32_t randomSiteIndex = ( int32_t )
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                           getUniformRandomUInt32( _randomGenerator,
                                                   _sites.size() );
    if ( !_sites.empty() )
    {

      site = _sites[ randomSiteIndex ];

    }
    else
    {

      throw std::runtime_error( "the current front has no site" );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Front< T >::getRandomSite( "
             "gkg::Vector3d< int32_t >& site ) const" );

}


#undef BACKGROUND_LABEL


#endif
