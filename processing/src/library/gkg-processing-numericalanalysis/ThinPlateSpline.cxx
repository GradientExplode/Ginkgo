#include <gkg-processing-numericalanalysis/ThinPlateSpline.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


template < class T >
gkg::ThinPlateSpline< T >::ThinPlateSpline()
{
  try
  {

    _factory =
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

  }
  GKG_CATCH( "template < class T > "
             "gkg::ThinPlateSpline< T >::ThinPlateSpline()" );

}


template < class T >
gkg::ThinPlateSpline< T >::~ThinPlateSpline()
{
}


template < class T >
inline
void gkg::ThinPlateSpline< T >::evaluate(
                         const std::vector< gkg::Vector2d< T > >& controlPoints,
                         const std::vector< double >& values,
                         double regularization )
{

  try
  {

    // Sanity checks
    if ( controlPoints.size() != values.size() )
    {

      throw std::runtime_error( "Bad input vector sizes" );

    }

    if ( controlPoints.size() < 3 )
    {

      throw std::runtime_error( "Not enough control points" );

    }

    _controlPoints = controlPoints;

    int32_t i, n = (int32_t)values.size();
    gkg::Matrix L( n + 3, n + 3 );
    gkg::Vector b( n + 3 );

    L.setZero();
    _v.reallocate( n + 3 );

    for ( i = 0; i < n; i++ )
    {

      b( i ) = values[ i ];

    }

    b( n ) = 0.0;
    b( n + 1 ) = 0.0;
    b( n + 2 ) = 0.0;

    double a = 0.0;
    int32_t j;

    for ( i = 0; i < n; i++ )
    {

      gkg::Vector2d< T > pt = _controlPoints[ i ];

      for ( j = i + 1; j < n; j++ )
      {

        gkg::Vector2d< T > d = pt - _controlPoints[ j ];
        double length = d.getDoubleNorm();
        double value = radialBasis( length );

        L( i, j ) = value;
        L( j, i ) = value;
        a += 2.0 * length;

      }

      L( i, n ) = 1.0;
      L( i, n + 1 ) = (double)pt.x;
      L( i, n + 2 ) = (double)pt.y;
      L( n, i ) = 1.0;
      L( n + 1, i ) = (double)pt.x;
      L( n + 2, i ) = (double)pt.y;

    }

    a /= double( n * n );

    for ( i = 0; i < n; i++ )
    {

      L( i, i ) = regularization * a * a;

    }

    _factory->getLUSolution( L, b, _v );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ThinPlateSpline< T >::evaluate( "
             "const std::vector< double >& values, "
             "double regularization )" );

}


template < class T >
inline
double gkg::ThinPlateSpline< T >::getValueAt( T x, T y ) const
{

  try
  {

    int32_t i, n = (int32_t)_controlPoints.size();
    double h = _v( n ) + _v( n + 1 ) * double( x ) + _v( n + 2 ) * double( y );
    gkg::Vector2d< T > pt( x, y );

    for ( i = 0; i < n; i++ )
    {

      gkg::Vector2d< T > d = _controlPoints[ i ] - pt;
      h += _v( i ) * radialBasis( d.getDoubleNorm() );

    }

    return h;

  }
  GKG_CATCH( "template < class T > "
             "double gkg::ThinPlateSpline< T >::getValue( T x, T y ) const" );

}


template < class T >
inline
double gkg::ThinPlateSpline< T >::radialBasis( double r ) const
{

  try
  {

    if ( r < 1e-6 )
    {

      return 0.0;

    }

    return r * r * std::log( r );

  }
  GKG_CATCH( "template < class T > "
             "double gkg::ThinPlateSpline< T >::radialBasis( double r )" );

}


// 
// forcing instanciations
//


template class gkg::ThinPlateSpline< uint16_t >;
template class gkg::ThinPlateSpline< int16_t >;
template class gkg::ThinPlateSpline< uint32_t >;
template class gkg::ThinPlateSpline< int32_t >;
template class gkg::ThinPlateSpline< uint64_t >;
template class gkg::ThinPlateSpline< int64_t >;
template class gkg::ThinPlateSpline< float >;
template class gkg::ThinPlateSpline< double >;
