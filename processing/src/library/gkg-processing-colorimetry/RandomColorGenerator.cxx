#include <gkg-processing-colorimetry/RandomColorGenerator.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>



gkg::RandomColorGenerator::RandomColorGenerator( float saturation,
                                                 float value )
                          : _saturation( saturation ),
                            _value( value ),
                            _randomGenerator( gkg::RandomGenerator::Taus )
{

  try
  {

    if ( ( saturation < 0.0f ) || ( saturation > 1.0f ) )
    {

      throw std::runtime_error( "saturation must belong to [0.0;1.0]" );


    }

    if ( ( value < 0.0f ) || ( value > 1.0f ) )
    {

      throw std::runtime_error( "value must belong to [0.0;1.0]" );


    }

    _factory = 
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    _goldenRatioConjugate = ( ( float )std::sqrt( 5.0 ) - 1.0f ) / 2.0f;

  }
  GKG_CATCH( "gkg::RandomColorGenerator::RandomColorGenerator()" );

}



gkg::RandomColorGenerator::~RandomColorGenerator()
{
}



gkg::RGBComponent gkg::RandomColorGenerator::getRandomColor() const
{

  try
  {

    float randomHue = ( float )_factory->getUniformRandomNumber(
                                                               _randomGenerator,
                                                               0.0,
                                                               1.0 );
    randomHue = fmodf( randomHue +_goldenRatioConjugate, 1.0f );

    return this->getRGBFromHSVComponents( randomHue, _saturation, _value );

  }
  GKG_CATCH( "gkg::RGBComponent "
             "gkg::RandomColorGenerator::getRandomColor() const" );


}


gkg::RGBComponent gkg::RandomColorGenerator::getRandomPastelColor() const
{

  try
  {

    return gkg::RGBComponent( ( uint8_t )( ( _factory->getUniformRandomNumber(
                                                      _randomGenerator,
                                                      0.0,
                                                      255.0 ) + 255.0 ) / 2 ),
                              ( uint8_t )( ( _factory->getUniformRandomNumber(
                                                      _randomGenerator,
                                                      0.0,
                                                      255.0 ) + 255.0 ) / 2 ),
                              ( uint8_t )( ( _factory->getUniformRandomNumber(
                                                      _randomGenerator,
                                                      0.0,
                                                      255.0 ) + 255.0 ) / 2 ) );
                              
  }
  GKG_CATCH( "gkg::RGBComponent "
             "gkg::RandomColorGenerator::getRandomPastelColor() const" );


}


gkg::RGBComponent gkg::RandomColorGenerator::getRandomRainbowColor(
                                         int32_t step, int32_t stepCount ) const
{

  try
  {

    double h = ( double )step / ( double )stepCount;

    int32_t i = ( int32_t )std::floor( h * 6.0 );
    double f = h * 6.0 - i;
    double q = 1.0 - f;

    switch ( i % 6 )
    {

      case 0 : 

        return gkg::RGBComponent( 255U,
                                  ( uint32_t )( 255.0 * f ),
                                  0U );

      case 1 : 

        return gkg::RGBComponent( ( uint32_t )( 255.0 * q ),
                                  255U,
                                  0U );

      case 2 : 

        return gkg::RGBComponent( 0U,
                                  255U,
                                  ( uint32_t )( 255.0 * f ) );

      case 3 : 

        return gkg::RGBComponent( 0U,
                                  ( uint32_t )( 255.0 * q ),
                                  255U );

      case 4 : 

        return gkg::RGBComponent( ( uint32_t )( 255.0 * f ),
                                  0U,
                                  255U );

      case 5 : 

        return gkg::RGBComponent( 255U,
                                  0U,
                                  ( uint32_t )( 255.0 * q ) );

    }
    return gkg::RGBComponent( 0U, 0U, 0U );

  }
  GKG_CATCH( "gkg::RGBComponent "
             "gkg::RandomColorGenerator::getRandomPastelColor() const" );


}



gkg::RGBComponent gkg::RandomColorGenerator::getRGBFromHSVComponents(
                                                             float hue,
                                                             float saturation,
                                                             float value ) const
{

  try
  {

    int32_t hueInt32 = ( int32_t )( hue * 6.0f );
    float f = hue * 6.0f - ( float )hueInt32;
    float p = value * ( 1.0f - saturation );
    float q = value * ( 1.0f - f * saturation );
    float t = value * ( 1.0f - ( 1.0f - f ) * saturation );

    gkg::RGBComponent rgbComponent;
    switch ( hueInt32 )
    {

       case 0:

         rgbComponent.r = ( uint8_t )( 255 * value );
         rgbComponent.g = ( uint8_t )( 255 * t );
         rgbComponent.b = ( uint8_t )( 255 * p );
         break;

       case 1:

         rgbComponent.r = ( uint8_t )( 255 * q );
         rgbComponent.g = ( uint8_t )( 255 * value );
         rgbComponent.b = ( uint8_t )( 255 * p );
         break;

       case 2:

         rgbComponent.r = ( uint8_t )( 255 * p );
         rgbComponent.g = ( uint8_t )( 255 * value );
         rgbComponent.b = ( uint8_t )( 255 * t );
         break;

       case 3:

         rgbComponent.r = ( uint8_t )( 255 * p );
         rgbComponent.g = ( uint8_t )( 255 * q);
         rgbComponent.b = ( uint8_t )( 255 * value );
         break;

       case 4:

         rgbComponent.r = ( uint8_t )( 255 * t );
         rgbComponent.g = ( uint8_t )( 255 * p );
         rgbComponent.b = ( uint8_t )( 255 * value );
         break;

       case 5:

         rgbComponent.r = ( uint8_t )( 255 * value );
         rgbComponent.g = ( uint8_t )( 255 * p );
         rgbComponent.b = ( uint8_t )( 255 * q );
         break;

    }

    return rgbComponent;

  }
  GKG_CATCH( "gkg::RGBComponent gkg::RandomColorGenerator::"
             "getRGBFromHSVComponents( "
             "float hue, "
             "float saturation, "
             "float value ) const" );

}
