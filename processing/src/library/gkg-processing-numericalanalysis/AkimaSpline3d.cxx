#include <gkg-processing-numericalanalysis/AkimaSpline3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::AkimaSpline3d< T >::AkimaSpline3d(
                                    const std::vector< gkg::Vector3d< T > >& controlPoints )
                        : gkg::CubicSpline3d< T >()
{

  try
  {

    this->initialize( controlPoints );
    
  }
  GKG_CATCH( "template < class T > "
             "gkg::AkimaSpline3d< T >::AkimaSpline3d( "
             "const std::vector< gkg::Vector3d< T > >& controlPoints )" );

}


template < class T >
gkg::AkimaSpline3d< T >::AkimaSpline3d(
                                    const gkg::LightCurve3d< T >& lightCurve3d )
                       : gkg::CubicSpline3d< T >()
{

  try
  {

    this->initialize( lightCurve3d.getPoints() );
    
  }
  GKG_CATCH( "template < class T > "
             "gkg::AkimaSpline3d< T >::AkimaSpline3d( "
             "const gkg::LightCurve3d< T >& lightCurve3d )" );

}


template < class T >
void gkg::AkimaSpline3d< T >::initialize(
                       const std::vector< gkg::Vector3d< T > >& controlPoints )
{

  try
  {

    // processing curve tangents
    int32_t i = 0;
    int32_t j = 0;
    int32_t n = ( int32_t )controlPoints.size();
    std::vector< gkg::Vector3d< T > > tangents( n );
    std::vector< gkg::Vector3d< T > > D( n + 4 );

    for ( i = 0, j = 2; i < ( n - 1 ); i++, j++ )
    {

      D[ j ] = controlPoints[ i + 1 ] - controlPoints[ i ];

    }

    D[ 0 ] = D[ 2 ] * T( 3 ) - D[ 3 ] * T( 2 );
    D[ 1 ] = D[ 2 ] * T( 2 ) - D[ 3 ];
    D[ n + 1 ] = D[ n ] * T( 2 ) - D[ n - 1 ];
    D[ n + 2 ] = D[ n ] * T( 3 ) - D[ n - 1 ] * T( 2 );

    for ( i = 0, j = 2; i < ( n - 1 ); i++, j++ )
    {

      gkg::Vector3d< T > ai = ( D[ j - 1 ] - D[ j - 2 ] ).absoluteCoordinates();
      gkg::Vector3d< T > wi = ( D[ j + 1 ] - D[ j ] ).absoluteCoordinates();

      if ( ( wi.x + ai.x ) < 1e-6 )
      {

    	wi.x = ai.x = 1.0;

      }
      if ( ( wi.y + ai.y ) < 1e-6 )
      {

    	wi.y = ai.y = 1.0;

      }
      if ( ( wi.z + ai.z ) < 1e-6 )
      {

    	wi.z = ai.z = 1.0;

      }

      tangents[ i ].x = ( wi.x * D[ j - 1 ].x + ai.x * D[ j ].x ) /
                        ( wi.x + ai.x );
      tangents[ i ].y = ( wi.y * D[ j - 1 ].y + ai.y * D[ j ].y ) /
                        ( wi.y + ai.y );
      tangents[ i ].z = ( wi.z * D[ j - 1 ].z + ai.z * D[ j ].z ) /
                        ( wi.z + ai.z );

    }

    this->processSegments( controlPoints, tangents );

  }
  GKG_CATCH( "template < class T > "
             "gkg::AkimaSpline3d< T >::AkimaSpline3d( "
             "const gkg::LightCurve3d< T >& lightCurve3d )" );

}


// 
// forcing instanciations
//

template class gkg::AkimaSpline3d< float >;
template class gkg::AkimaSpline3d< double >;
