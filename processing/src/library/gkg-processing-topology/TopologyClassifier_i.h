#ifndef _gkg_processing_topology_TopologyClassifier_i_h_
#define _gkg_processing_topology_TopologyClassifier_i_h_


#include <gkg-processing-topology/TopologyClassifier.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/TypeOf.h>


template < class T >
bool gkg::TopologyClassifier::isSimplePoint( 
                                     const gkg::Volume< T >& volume,
                                     const gkg::TestFunction< T >& testFunction,
                                     const gkg::Vector3d< int32_t >& site,
                                     const int32_t& t )
{

  bool status = false;
  gkg::BoundingBox< int32_t > boundingBox( volume );

  if ( boundingBox.contains( site, t ) )
  {

    int32_t X[ 27 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int32_t* XPtr = X + 1;

    std::list< gkg::Vector3d< int32_t > >::iterator
      n = _neighbors.begin(),
      ne = _neighbors.end();

    while ( n != ne )
    {

      gkg::Vector3d< int32_t > point = site + *n;

      if ( boundingBox.contains( point, t ) )
      {

        *XPtr = testFunction.getState( volume( point, t ) ) ? 1 : 0;

      }

      ++ XPtr;
      ++ n;

    }

    bool inverse;
    int32_t result = binaryDecisionDiagram( X, inverse );

    if ( inverse )
    {

      status = result ? false : true;

    }
    else
    {

      status = result ? true : false;

    }

  }

  return status;

}


template < class T >
bool gkg::TopologyClassifier::isSimplePoint( 
                                     const gkg::Volume< T >& volume,
                                     const gkg::TestFunction< T >& testFunction,
                                     const gkg::Vector3d< int32_t >& site,
                                     int32_t& CStar,
                                     int32_t& CBar,
                                     const int32_t& t )
{

  int32_t cStar, cBar;

  getCStarAndCBar( volume, testFunction, site, cStar, cBar, t );

  CStar = cStar;
  CBar = cBar;

  return ( cStar == 1 && cBar == 1 );

}


template < class T >
bool gkg::TopologyClassifier::isCurvePoint( 
                                     const gkg::Volume< T >& volume,
                                     const gkg::TestFunction< T >& testFunction,
                                     const gkg::Vector3d< int32_t >& site,
                                     int32_t* CStar,
                                     int32_t* CBar,
                                     const int32_t& t )
{

  int32_t cStar, cBar;

  getCStarAndCBar( volume, testFunction, site, cStar, cBar, t );

  if ( CStar )
  {

    *CStar = cStar;

  }

  if ( CBar )
  {

    *CBar = cBar;

  }

  return ( cStar > 1 && cBar == 1 );

}


template < class T >
bool gkg::TopologyClassifier::isSurfacePoint( 
                                     const gkg::Volume< T >& volume,
                                     const gkg::TestFunction< T >& testFunction,
                                     const gkg::Vector3d< int32_t >& site,
                                     int32_t* CStar,
                                     int32_t* CBar,
                                     const int32_t& t )
{

  int32_t cStar, cBar;

  getCStarAndCBar( volume, testFunction, site, cStar, cBar, t );

  if ( CStar )
  {

    *CStar = cStar;

  }

  if ( CBar )
  {

    *CBar = cBar;

  }

  return ( cBar > 1 );

}


template < class T >
void gkg::TopologyClassifier::getCStarAndCBar( 
                                     const gkg::Volume< T >& volume,
                                     const gkg::TestFunction< T >& testFunction,
                                     const gkg::Vector3d< int32_t >& site,
                                     int32_t& CStar,
                                     int32_t& CBar,
                                     const int32_t& t )
{

  gkg::BoundingBox< int32_t > boundingBox( volume );

  CStar = 0;
  CBar = 0;

  if ( boundingBox.contains( site, t ) )
  {

    int32_t X[ 27 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int32_t* XPtr = X + 1;

    std::list< gkg::Vector3d< int32_t > >::iterator
      n = _neighbors.begin(),
      ne = _neighbors.end();

    while ( n != ne )
    {

      gkg::Vector3d< int32_t > point = site + *n;

      if ( boundingBox.contains( point, t ) )
      {

        *XPtr = testFunction.getState( volume( point, t ) ) ? 1 : 0;

      }

      ++ XPtr;
      ++ n;

    }

    CStar = _component26.getNumberOfComponents( X );
    CBar = _component18.getNumberOfComponentsForComplement( X );

  }

}


template < class T >
gkg::Volume< uint8_t > gkg::TopologyClassifier::getClassification( 
                                    const gkg::Volume< T >& volumeIn,
                                    const gkg::TestFunction< T >& testFunction )
{

  int32_t cStar, cBar, x, y, z, t;
  int32_t sizeX = volumeIn.getSizeX();
  int32_t sizeY = volumeIn.getSizeY();
  int32_t sizeZ = volumeIn.getSizeZ();
  int32_t sizeT = volumeIn.getSizeT();

  gkg::Volume< uint8_t > volumeOut( sizeX, sizeY, sizeZ, sizeT );

  volumeOut.getHeader() = volumeIn.getHeader();
  volumeOut.getHeader()[ "item_type" ] = gkg::TypeOf< uint8_t >::getName();


  for ( t = 0; t < sizeT; t++ )
  {

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          gkg::Vector3d< int32_t > point( x, y, z );

          if ( testFunction.getState( volumeIn( point, t ) ) )
          {

            getCStarAndCBar( volumeIn, testFunction, point, cStar, cBar, t );

            if ( cStar > 3 )
            {

              cStar = 3;

            }
            if ( cBar > 3 )
            {

              cBar = 3;

            }

            volumeOut( x, y, z, t ) = _classMatrix[ cStar ][ cBar ];

          }

        }

      }

    }

  }

  return volumeOut;

}


#endif
