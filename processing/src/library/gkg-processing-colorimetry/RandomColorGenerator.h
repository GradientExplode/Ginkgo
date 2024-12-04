#ifndef _gkg_processing_colorimetry_RandomColorGenerator_h_
#define _gkg_processing_colorimetry_RandomColorGenerator_h_


#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>


namespace gkg
{


class RandomColorGenerator
{

  public:

    RandomColorGenerator( float saturation = 0.7, float value = 0.65 );
    virtual ~RandomColorGenerator();

    RGBComponent getRandomColor() const;
    RGBComponent getRandomPastelColor() const;
    gkg::RGBComponent getRandomRainbowColor( int32_t step,
                                             int32_t stepCount ) const;


    RGBComponent getRGBFromHSVComponents( float hue,
                                          float saturation,
                                          float value ) const;
    

  protected:

    float _saturation;
    float _value;
    RandomGenerator _randomGenerator;
    NumericalAnalysisImplementationFactory* _factory;
    float _goldenRatioConjugate;

};



}


#endif
