#include <gkg-dmri-ph-basis/PolarHarmonicsCache.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PolarHarmonicsCache::PolarHarmonicsCache()
                         : _order( 0 ),
                           _coefficientCount( 0 )
{

  try
  {

    // getting a pointer to the numerical analysis factory
    _factory =
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

  }
  GKG_CATCH( "gkg::PolarHarmonicsCache::PolarHarmonicsCache()" );

}


gkg::PolarHarmonicsCache::~PolarHarmonicsCache()
{
}


void gkg::PolarHarmonicsCache::update( int32_t order )
{

  try
  {

    _mutex.lock();

    if ( order != _order )
    {

      // updating protected field(s)
      _order = order;

      this->computeBasisIndices();
      this->computeSqrtOfFactorialOfN1N2N3s();
      this->computeBN1N2N3s();

    }

    _mutex.unlock();

  }
  GKG_CATCH( "void gkg::PolarHarmonicsCache::update( "
             "int32_t order )" );

}


int32_t gkg::PolarHarmonicsCache::getOrder() const
{

  try
  {

    return _order;

  }
  GKG_CATCH( "int32_t gkg::PolarHarmonicsCache::getOrder() const" );

}


int32_t gkg::PolarHarmonicsCache::getCoefficientCount() const
{

  try
  {

    return _coefficientCount;

  }
  GKG_CATCH( "int32_t gkg::PolarHarmonicsCache::"
             "getPolarHarmonicsCoefficientCount() const" );

}


const std::vector< gkg::Vector3d< int32_t > >& 
gkg::PolarHarmonicsCache::getBasisIndices() const
{

  try
  {

    return _basisIndices;

  }
  GKG_CATCH( "const std::vector< gkg::Vector3d< int32_t > >& "
             "gkg::PolarHarmonicsCache::getBasisIndices() "
             "const" );

}


const std::vector< float >& 
gkg::PolarHarmonicsCache::getSqrtOfFactorialOfN1N2N3s() const
{

  try
  {

    return _sqrtOfFactorialOfN1N2N3s;

  }
  GKG_CATCH( "const std::vector< float >& "
             "gkg::PolarHarmonicsCache::getSqrtOfFactorialOfN1N2N3s() const" );

}


const std::vector< float >& 
gkg::PolarHarmonicsCache::getBN1N2N3s() const
{

  try
  {

    return _bN1N2N3s;

  }
  GKG_CATCH( "const std::vector< float >& "
             "gkg::PolarHarmonicsCache::getBN1N2N3s() const" );

}


float gkg::PolarHarmonicsCache::getRSpaceBasisFunction3dAt(
              const std::vector< gkg::Vector3d< float > >& rotatedDisplacements,
              const gkg::Vector3d< float >& scaling,
              int32_t displacementIndex,
              int32_t basisIndex ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( basisIndex < 0 ) ||
         ( basisIndex >= ( int32_t )_basisIndices.size() ) )
    {

      throw std::runtime_error( "invalid basis index" );

    }

    if ( ( displacementIndex < 0 ) ||
         ( displacementIndex >= ( int32_t )rotatedDisplacements.size() ) )
    {

      throw std::runtime_error( "invalid displacement index" );

    }

#endif

    const gkg::Vector3d< int32_t >& n1n2n3 = _basisIndices[ basisIndex ];

    return this->getRSpaceBasisFunction1dXAt( n1n2n3.x,
                                                 scaling.x,
                                                 rotatedDisplacements,
                                                 displacementIndex ) *
           this->getRSpaceBasisFunction1dYAt( n1n2n3.y,
                                                 scaling.y,
                                                 rotatedDisplacements,
                                                 displacementIndex ) *
           this->getRSpaceBasisFunction1dZAt( n1n2n3.z,
                                                 scaling.z,
                                                 rotatedDisplacements,
                                                 displacementIndex );

  }
  GKG_CATCH( "float gkg::PolarHarmonicsCache::getRSpaceBasisFunction3dAt( "
             "const std::vector< gkg::Vector3d< float > >& "
             "rotatedDisplacements, "
             "int32_t index ) const" );



}


float gkg::PolarHarmonicsCache::getQSpaceBasisFunction3dAt(
                    const std::vector< Vector3d< float > >& rotatedQSpacePoints,
                    const gkg::Vector3d< float >& scaling,
                    int32_t qSpaceIndex,
                    int32_t basisIndex ) const
{

  try
  {

    const gkg::Vector3d< int32_t >& n1n2n3 = _basisIndices[ basisIndex ];

    return std::real( this->getQSpaceBasisFunction1dXAt( n1n2n3.x,
                                                         scaling.x,
                                                         rotatedQSpacePoints,
                                                         qSpaceIndex ) *
                      this->getQSpaceBasisFunction1dYAt( n1n2n3.y,
                                                         scaling.y,
                                                         rotatedQSpacePoints,
                                                         qSpaceIndex ) *
                      this->getQSpaceBasisFunction1dZAt( n1n2n3.z,
                                                         scaling.z,
                                                         rotatedQSpacePoints,
                                                         qSpaceIndex ) );

  }
  GKG_CATCH( "float gkg::PolarHarmonicsCache::getQSpaceBasisFunction3dAt( "
             "const std::vector< Vector3d< float > >& rotatedQSpacePoints, "
             "const gkg::Vector3d< float >& scaling, "
             "int32_t qSpaceIndex, "
             "int32_t basisIndex ) const" );

}


float gkg::PolarHarmonicsCache::getTmn( int32_t m, int32_t n,
                                        float u, float v ) const
{

  try
  {

    float t = 0.0f;

    if ( ( m + n ) % 2 == 0 )
    {

      float factor = ( float )std::sqrt(
                       _factory->getFactorialFunction( ( double )m ) *
                       _factory->getFactorialFunction( ( double )n ) ) / 
                     ( ( float )std::sqrt( 2 * M_PI ) * u * v );
 
      int32_t r = 0;
      int32_t s = 0;
      for ( r = 0; r <= m; r += 2 )
      {

        for ( s = 0; s <= n; s += 2 )
        {
 
          t += ( factor *
                 ( float )std::pow( ( 1.0 / ( u * u ) + 1.0 / ( v * v ) ),
                                    -0.5 * ( m + n - r - s + 1 ) ) *
                 ( ( ( ( r + s ) / 2 ) % 2 == 1 ) ? -1.0f : +1.0f ) *
                 ( float )std::pow( 2.0, 0.5 * ( m + n - r - s ) ) *
                 _factory->getDoubleFactorialFunction(
                                        ( double )( m + n - r - s - 1 ) ) ) /
               ( ( float )std::pow( ( double )u, ( double )m - ( double )r ) *
                 ( float )std::pow( ( double )v, ( double )( n - s ) ) * 
                 ( float )_factory->getFactorialFunction( ( double )( m - r ) ) *
                 ( float )_factory->getFactorialFunction( ( double )( n - s ) ) *
                 ( float )_factory->getDoubleFactorialFunction( ( double )r ) *
                 ( float )_factory->getDoubleFactorialFunction( ( double )s ) );

        }
 
      }

    }

    return t;

  }
  GKG_CATCH( "float gkg::PolarHarmonicsCache::getTmn( "
             "int32_t m, int32_t n, float u, float v ) const" );

}


float gkg::PolarHarmonicsCache::getRSpaceBasisFunction1dXAt(
              int32_t order,
              float scaling,
              const std::vector< gkg::Vector3d< float > >& rotatedDisplacements,
              int32_t displacementIndex ) const
{

  try
  {

    return ( 1.0 / ( ( float )std::sqrt( std::pow( 2, order + 1 ) *
                                M_PI *
                                _factory->getFactorialFunction(
                                                   ( double )order ) ) *
                     scaling ) ) *
           ( float )std::exp( -std::pow(
                             rotatedDisplacements[ displacementIndex ].x,
                             2 ) /
                     ( 2 * ( float )std::pow( scaling, 2 ) ) ) *
           _factory->getHermitePolynomialFunction(
                            order,
                            rotatedDisplacements[ displacementIndex ].x /
                            scaling );

  }
  GKG_CATCH( "float gkg::PolarHarmonicsCache::getRSpaceBasisFunction1dXAt( "
             "int32_t order, "
             "float scaling, "
             "const std::vector< gkg::Vector3d< float > >& "
             "rotatedDisplacements, "
             "int32_t displacementIndex ) const" );

}


float gkg::PolarHarmonicsCache::getRSpaceBasisFunction1dYAt(
              int32_t order,
              float scaling,
              const std::vector< gkg::Vector3d< float > >& rotatedDisplacements,
              int32_t displacementIndex ) const
{

  try
  {

    return ( 1.0 / ( std::sqrt( std::pow( 2, order + 1 ) *
                                M_PI *
                                _factory->getFactorialFunction(
                                                   ( double )order ) ) *
                     scaling ) ) *
           std::exp( -std::pow( rotatedDisplacements[ displacementIndex ].y,
                                2 ) /
                     ( 2 * std::pow( scaling, 2 ) ) ) *
           _factory->getHermitePolynomialFunction(
                            order,
                            rotatedDisplacements[ displacementIndex ].y /
                            scaling );

  }
  GKG_CATCH( "float gkg::PolarHarmonicsCache::getRSpaceBasisFunction1dYAt( "
             "int32_t order, "
             "float scaling, "
             "const std::vector< gkg::Vector3d< float > >& "
             "rotatedDisplacements, "
             "int32_t displacementIndex ) const" );

}


float gkg::PolarHarmonicsCache::getRSpaceBasisFunction1dZAt(
              int32_t order,
              float scaling,
              const std::vector< gkg::Vector3d< float > >& rotatedDisplacements,
              int32_t displacementIndex ) const
{

  try
  {

    return ( 1.0 / ( std::sqrt( std::pow( 2, order + 1 ) *
                                M_PI *
                                _factory->getFactorialFunction(
                                                   ( double )order ) ) *
                     scaling ) ) *
           std::exp( -std::pow( rotatedDisplacements[ displacementIndex ].z,
                                2 ) /
                     ( 2 * std::pow( scaling, 2 ) ) ) *
           _factory->getHermitePolynomialFunction(
                            order,
                            rotatedDisplacements[ displacementIndex ].z /
                            scaling );

  }
  GKG_CATCH( "float gkg::PolarHarmonicsCache::getRSpaceBasisFunction1dZAt( "
             "int32_t order, "
             "float scaling, "
             "const std::vector< gkg::Vector3d< float > >& "
             "rotatedDisplacements, "
             "int32_t displacementIndex ) const" );

}


std::complex< float > gkg::PolarHarmonicsCache::getQSpaceBasisFunction1dXAt(
              int32_t order,
              float scaling,
              const std::vector< gkg::Vector3d< float > >& rotatedQSpacePoints,
              int32_t qSpaceIndex ) const
{

  try
  {

    float tmp = ( 1.0 / ( float )std::sqrt( 2.0 * M_PI ) * scaling ) * 
                this->getRSpaceBasisFunction1dXAt( order,
                                                   1.0 / ( 2 * M_PI * scaling ),
                                                   rotatedQSpacePoints,
                                                   qSpaceIndex );

    std::complex< float > result;
    // i^N = 1 -> 1/i^N = 1
    if ( order % 4 == 0 )
    {

      result = std::complex< float >( tmp, 0.0 );

    }
    // i^N = i -> 1/i^N = -i
    else if ( order % 4 == 1 )
    {

      result = std::complex< float >( 0.0, -tmp );

    }
    // i^N = -1 -> 1/i^N = -1
    else if ( order % 4 == 2 )
    {

      result = std::complex< float >( -tmp, 0.0 );

    }
    // i^N = -i -> 1/i^N = i
    else if ( order % 4 == 3 )
    {

      result = std::complex< float >( 0.0, tmp );

    }

    return result;

  }
  GKG_CATCH( "float gkg::PolarHarmonicsCache::getQSpaceBasisFunction1dXAt( "
             "int32_t order, "
             "float scaling, "
             "const std::vector< gkg::Vector3d< float > >& "
             "rotatedQSpacePoints, "
             "int32_t qSpaceIndex ) const" );

}


std::complex< float > gkg::PolarHarmonicsCache::getQSpaceBasisFunction1dYAt(
              int32_t order,
              float scaling,
              const std::vector< gkg::Vector3d< float > >& rotatedQSpacePoints,
              int32_t qSpaceIndex ) const
{

  try
  {

    float tmp = ( 1.0 / ( float )std::sqrt( 2.0 * M_PI ) * scaling ) * 
                this->getRSpaceBasisFunction1dYAt( order,
                                                   1.0 / ( 2 * M_PI * scaling ),
                                                   rotatedQSpacePoints,
                                                   qSpaceIndex );

    std::complex< float > result;
    // i^N = 1 -> 1/i^N = 1
    if ( order % 4 == 0 )
    {

      result = std::complex< float >( tmp, 0.0 );

    }
    // i^N = i -> 1/i^N = -i
    else if ( order % 4 == 1 )
    {

      result = std::complex< float >( 0.0, -tmp );

    }
    // i^N = -1 -> 1/i^N = -1
    else if ( order % 4 == 2 )
    {

      result = std::complex< float >( -tmp, 0.0 );

    }
    // i^N = -i -> 1/i^N = i
    else if ( order % 4 == 3 )
    {

      result = std::complex< float >( 0.0, tmp );

    }

    return result;

  }
  GKG_CATCH( "float gkg::PolarHarmonicsCache::getQSpaceBasisFunction1dYAt( "
             "int32_t order, "
             "float scaling, "
             "const std::vector< gkg::Vector3d< float > >& "
             "rotatedQSpacePoints, "
             "int32_t qSpaceIndex ) const" );

}


std::complex< float > gkg::PolarHarmonicsCache::getQSpaceBasisFunction1dZAt(
              int32_t order,
              float scaling,
              const std::vector< gkg::Vector3d< float > >& rotatedQSpacePoints,
              int32_t qSpaceIndex ) const
{

  try
  {

    float tmp = ( 1.0 / ( float )std::sqrt( 2.0 * M_PI ) * scaling ) * 
                this->getRSpaceBasisFunction1dZAt( order,
                                                   1.0 / ( 2 * M_PI * scaling ),
                                                   rotatedQSpacePoints,
                                                   qSpaceIndex );

    std::complex< float > result;
    // i^N = 1 -> 1/i^N = 1
    if ( order % 4 == 0 )
    {

      result = std::complex< float >( tmp, 0.0 );

    }
    // i^N = i -> 1/i^N = -i
    else if ( order % 4 == 1 )
    {

      result = std::complex< float >( 0.0, -tmp );

    }
    // i^N = -1 -> 1/i^N = -1
    else if ( order % 4 == 2 )
    {

      result = std::complex< float >( -tmp, 0.0 );

    }
    // i^N = -i -> 1/i^N = i
    else if ( order % 4 == 3 )
    {

      result = std::complex< float >( 0.0, tmp );

    }

    return result;

  }
  GKG_CATCH( "float gkg::PolarHarmonicsCache::getQSpaceBasisFunction1dZAt( "
             "int32_t order, "
             "float scaling, "
             "const std::vector< gkg::Vector3d< float > >& "
             "rotatedQSpacePoints, "
             "int32_t qSpaceIndex ) const" );

}


void gkg::PolarHarmonicsCache::computeBasisIndices()
{

  try
  {

    _coefficientCount = ( ( _order / 2 + 1 ) *
                          ( _order / 2 + 2 ) *
                          ( 2 * _order + 3 ) ) / 6;

    _basisIndices.resize( _coefficientCount );

    int32_t n1 = 0;
    int32_t n2 = 0;
    int32_t n3 = 0;
    int32_t c = 0;
    for ( n1 = 0; n1 <= _order; n1 += 2 )
    {

      for ( n2 = 0; n2 <= n1; n2++ )
      {

        for ( n3 = 0; n3 <= ( n1 - n2 ); n3++ )
        {

          gkg::Vector3d< int32_t >& n1n2n3 = _basisIndices[ c ];
          n1n2n3.x = n1 - n2 - n3;
          n1n2n3.y = n3;
          n1n2n3.z = n2;

          ++ c;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::PolarHarmonicsCache::computeBasisIndices()" );

}


void gkg::PolarHarmonicsCache::computeSqrtOfFactorialOfN1N2N3s()
{

  try
  {

    _sqrtOfFactorialOfN1N2N3s.resize( _coefficientCount );

    int32_t c = 0;
    for ( c = 0; c < _coefficientCount; c++ )
    {

      const gkg::Vector3d< int32_t >& n1n2n3 = _basisIndices[ c ];
      _sqrtOfFactorialOfN1N2N3s[ c ] = ( float )std::sqrt(
                       _factory->getFactorialFunction( ( double )n1n2n3.x ) *
                       _factory->getFactorialFunction( ( double )n1n2n3.y ) *
                       _factory->getFactorialFunction( ( double )n1n2n3.z ) );

    }

  }
  GKG_CATCH( "void gkg::PolarHarmonicsCache::computeSqrtOfFactorialN1N2N3s()" );

}


void gkg::PolarHarmonicsCache::computeBN1N2N3s()
{

  try
  {

    _bN1N2N3s.resize( _coefficientCount );

    int32_t c = 0;
    for ( c = 0; c < _coefficientCount; c++ )
    {

      const gkg::Vector3d< int32_t >& n1n2n3 = _basisIndices[ c ];

      if ( ( n1n2n3.x % 2 == 0 ) &&
           ( n1n2n3.y % 2 == 0 ) &&
           ( n1n2n3.z % 2 == 0 ) )
      {

        _bN1N2N3s[ c ] =
          ( float )std::sqrt(
                      _factory->getFactorialFunction( ( double )n1n2n3.x ) *
                      _factory->getFactorialFunction( ( double )n1n2n3.y ) *
                      _factory->getFactorialFunction( ( double )n1n2n3.z ) ) /
          ( float )(
            _factory->getFactorialFunction( _factory->getFactorialFunction(
                                          ( double )n1n2n3.x ) ) *
            _factory->getFactorialFunction( _factory->getFactorialFunction(
                                          ( double )n1n2n3.y ) ) *
            _factory->getFactorialFunction( _factory->getFactorialFunction(
                                          ( double )n1n2n3.z ) ) );



      }
      else           
      {

        _bN1N2N3s[ c ] = 0.0f;

      }

    }

  }
  GKG_CATCH( "void gkg::PolarHarmonicsCache::computeBN1N2N3s()" );

}
