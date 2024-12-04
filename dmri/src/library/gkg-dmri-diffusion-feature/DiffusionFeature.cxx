#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-exception/Exception.h>

#define EPSILON  1e-6
#define EPSILON1 1e-3
#define EPSILON2 1e-3


gkg::DiffusionFeature::DiffusionFeature()
{
}


gkg::DiffusionFeature::DiffusionFeature( const gkg::DiffusionFeature& other )
                      : _values( other._values )
{
}


gkg::DiffusionFeature::~DiffusionFeature()
{
}


gkg::DiffusionFeature& gkg::DiffusionFeature::operator=(
                                            const gkg::DiffusionFeature& other )
{

  try
  {

    _values = other._values;

    return *this;

  }
  GKG_CATCH( "gkg::DiffusionFeature& gkg::DiffusionFeature::operator=( "
             "const gkg::DiffusionFeature& other )" );

}


std::string gkg::DiffusionFeature::getBasisTypeName() const
{

  try
  {

    throw std::runtime_error( "should not pass through this function" );

  }
  GKG_CATCH( "std::string gkg::DiffusionFeature::getBasisTypeName() const" );

}


void gkg::DiffusionFeature::setBasisType(
                                        const std::string& /* basisTypeName */ )
{

  try
  {

    throw std::runtime_error( "should not pass through this function" );

  }
  GKG_CATCH( "void gkg::DiffusionFeature::setBasisType( "
             "const std::string& basisTypeName )" );

}


int32_t gkg::DiffusionFeature::getValueCount() const
{

  try
  {

    return ( int32_t )_values.size();

  }
  GKG_CATCH( "int32_t gkg::DiffusionFeature::getValueCount() const" );

}


const std::vector< float >& gkg::DiffusionFeature::getValues() const
{

  try
  {

    return _values;

  }
  GKG_CATCH( "const std::vector< float >& "
             "gkg::DiffusionFeature::getValues() const" );

}


float gkg::DiffusionFeature::getValue( int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= ( int32_t )_values.size() ) )
    {

      throw std::runtime_error( "invalid index" );

    }

#endif

    return _values[ index ];

  }
  GKG_CATCH( "float gkg::DiffusionFeature::"
             "getValue( int32_t index ) const" );

}


void gkg::DiffusionFeature::setValues( const std::vector< float >& values )
{

  try
  {

    _values = values;

  }
  GKG_CATCH( "void gkg::DiffusionFeature::"
             "setValues( "
             "const std::vector< float >& values )" );

}


void gkg::DiffusionFeature::setValues( const gkg::Vector& values )
{

  try
  {


    if ( values.getSize() != ( int32_t )_values.size() )
    {

      _values.resize( values.getSize() );

    }

    int32_t o = 0;
    for( o = 0; o < values.getSize() ; ++o )
    {

       _values[ o ] = values( o );

    }

  }
  GKG_CATCH( "void gkg::DiffusionFeature::setValues( "
             "const gkg::Vector& values )" );

}


void gkg::DiffusionFeature::setValue( int32_t index, float value )
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= ( int32_t )_values.size() ) )
    {

      throw std::runtime_error( "invalid index" );

    }

#endif

    _values[ index ] = value;

  }
  GKG_CATCH( "void gkg::DiffusionFeature::setValue( "
             "int32_t index, float value )" );

}


bool gkg::DiffusionFeature::computeNormalizedSignal(
                                           const gkg::Volume< float >& t2,
                                           const gkg::Volume< float >& dw,
                                           const gkg::Vector3d< int32_t >& site,
                                           float& averageT2,
                                           gkg::Vector& normalizedSignal ) const
{

  try
  {

    bool result = true;
    int32_t t2Count = t2.getSizeT();
    int32_t orientationCount = dw.getSizeT();
    int32_t t = 0;

    // processing average T2 value
    averageT2 = ( float )t2( site, 0 );
    for ( t = 1; t < t2Count; t++ )
    {

      averageT2 += ( float )t2( site, t );

    }
    averageT2 /= t2Count;

    if ( averageT2 <= EPSILON )
    {

      result = false;

    }
    else
    {

      // building normalized signal vector
      normalizedSignal.reallocate( orientationCount );

      for ( t = 0; t < orientationCount; t++ )
      {

        float dwValue = dw( site, t );
        double E = ( double )dwValue / ( double )averageT2;

        if ( E < 0.0 )
        {
	
          normalizedSignal( t ) = EPSILON1 / 2;

        }
        else if ( ( E >= 0 ) && ( E < EPSILON1 ) )
        {

          normalizedSignal( t ) = EPSILON1 / 2 + E * E / ( 2 * EPSILON1 );

        }
        else if ( ( E >= EPSILON1 ) && ( E < 1 - EPSILON2 ) )
        {

          normalizedSignal( t ) = E;

        }
        else if ( ( E >= 1 - EPSILON2 ) && ( E < 1.0 ) )
        {

          normalizedSignal( t ) = 1.0 - EPSILON2 / 2 -
                                  ( 1 - E ) * ( 1 - E ) / ( 2 * EPSILON2 );

        }
        else if ( E >= 1.0 )
        {

          normalizedSignal( t ) = 1 - EPSILON2 / 2;

        }

      }

    }

    return result;

  }
  GKG_CATCH( "bool gkg::DiffusionFeature::"
             "computeNormalizedSignal( "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "float& averageT2, "
             "gkg::Vector& normalizedSignal ) const" );

}


bool gkg::DiffusionFeature::computeNormalizedSignal(
                                           const gkg::Volume< float >& t2,
                                           const gkg::Volume< float >& dw,
                                           const gkg::Vector3d< float >& site,
                                           float& averageT2,
                                           gkg::Vector& normalizedSignal ) const
{

  try
  {

    bool result = true;
    int32_t t2Count = t2.getSizeT();
    int32_t orientationCount = dw.getSizeT();
    int32_t t = 0;


    // getting pointer to quick partial voluming resampler
    gkg::PartialVolumingQuickResampler< float >& resampler =
      gkg::PartialVolumingQuickResampler< float >::getInstance();

    // collecting resolution of data
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    if ( t2.getHeader().hasAttribute( "resolutionX" ) )
    {

      t2.getHeader().getAttribute( "resolutionX", resolution.x );

    }
    if ( t2.getHeader().hasAttribute( "resolutionY" ) )
    {

      t2.getHeader().getAttribute( "resolutionY", resolution.y );

    }
    if ( t2.getHeader().hasAttribute( "resolutionZ" ) )
    {

      t2.getHeader().getAttribute( "resolutionZ", resolution.z );

    }

    // processing average T2 value
    float t2Value;

    resampler.resample( t2,
                        ( float )0,
                        gkg::Vector3d< double >( site.x, site.y, site.z ),
                        t2Value,
                        &resolution,
                        0 );

    averageT2 = ( float )t2Value;
    for ( t = 1; t < t2Count; t++ )
    {

      resampler.resample( t2,
                          0.0,
                          gkg::Vector3d< double >( site.x, site.y, site.z ),
                          t2Value,
                          &resolution,
                          t );
      averageT2 += ( float )t2Value;

    }
    averageT2 /= t2Count;

    if ( averageT2 <= EPSILON )
    {

      result = false;

    }
    else
    {

      // building normalized signal vector
      normalizedSignal.reallocate( orientationCount );
      float dwValue = 0;

      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
	double E = ( double )dwValue / ( double )averageT2;

	if ( E < 0.0 )
        {
	
          normalizedSignal( t ) = EPSILON1 / 2;

        }
        else if ( ( E >= 0 ) && ( E < EPSILON1 ) )
        {

          normalizedSignal( t ) = EPSILON1 / 2 + E * E / ( 2 * EPSILON1 );

        }
        else if ( ( E >= EPSILON1 ) && ( E < 1 - EPSILON2 ) )
        {

          normalizedSignal( t ) = E;

        }
        else if ( ( E >= 1 - EPSILON2 ) && ( E < 1.0 ) )
        {

          normalizedSignal( t ) = 1.0 - EPSILON2 / 2 -
                                  ( 1 - E ) * ( 1 - E ) / ( 2 * EPSILON2 );

        }
        else if ( E >= 1.0 )
        {

          normalizedSignal( t ) = 1 - EPSILON2 / 2;

        }

      }

    }

    return result;

  }
  GKG_CATCH( "bool gkg::DiffusionFeature::"
             "computeNormalizedSignal( "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "float& averageT2, "
             "gkg::Vector& normalizedSignal ) const" );

}


#undef EPSILON  
#undef EPSILON1 
#undef EPSILON2 
