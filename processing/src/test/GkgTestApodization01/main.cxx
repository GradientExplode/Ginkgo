#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-processing-signal/BoxcarApodization.h>
#include <gkg-processing-signal/HammingApodization.h>
#include <gkg-processing-signal/TukeyApodization.h>
#include <gkg-processing-plot/CurvePlotter.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-communication-sysinfo/KeyboardHit.h>
#include <iostream>
#include <cstdlib>


int main()
{

  std::vector< double > input_Boxcar_Standard( 128 );
  std::vector< double > input_Boxcar_Centered( 128 );
  std::vector< double > input_Hamming_Standard( 128 );
  std::vector< double > input_Hamming_Centered( 128 );
  std::vector< double > input_Tukey_Standard( 128 );
  std::vector< double > input_Tukey_Centered( 128 );

  gkg::Apodization< double >*
    apodization_Boxcar_Standard = 
      gkg::ApodizationFactory< double >::getInstance().createApodization(
        gkg::BoxcarApodization< double >::getStaticName(),
        128,
        gkg::Apodization< double >::Standard );

  gkg::Apodization< double >*
    apodization_Boxcar_Centered = 
      gkg::ApodizationFactory< double >::getInstance().createApodization(
        gkg::BoxcarApodization< double >::getStaticName(),
        128,
        gkg::Apodization< double >::Centered );

  gkg::Apodization< double >*
    apodization_Hamming_Standard = 
      gkg::ApodizationFactory< double >::getInstance().createApodization(
        gkg::HammingApodization< double >::getStaticName(),
        128,
        gkg::Apodization< double >::Standard );

  gkg::Apodization< double >*
    apodization_Hamming_Centered = 
      gkg::ApodizationFactory< double >::getInstance().createApodization(
        gkg::HammingApodization< double >::getStaticName(),
        128,
        gkg::Apodization< double >::Centered );

  gkg::Apodization< double >*
    apodization_Tukey_Standard = 
      gkg::ApodizationFactory< double >::getInstance().createApodization(
        gkg::TukeyApodization< double >::getStaticName(),
        128,
        gkg::Apodization< double >::Standard );

  gkg::Apodization< double >*
    apodization_Tukey_Centered = 
      gkg::ApodizationFactory< double >::getInstance().createApodization(
        gkg::TukeyApodization< double >::getStaticName(),
        128,
        gkg::Apodization< double >::Centered );

  for ( size_t i = 0; i < 128; i++ )
  {

    input_Boxcar_Standard[ i ] = 
      apodization_Boxcar_Standard->getCoefficient( i );
    input_Boxcar_Centered[ i ] = 
      apodization_Boxcar_Centered->getCoefficient( i );
    input_Hamming_Standard[ i ] = 
      apodization_Hamming_Standard->getCoefficient( i );
    input_Hamming_Centered[ i ] = 
      apodization_Hamming_Centered->getCoefficient( i );
    input_Tukey_Standard[ i ] = 
      apodization_Tukey_Standard->getCoefficient( i );
    input_Tukey_Centered[ i ] = 
      apodization_Tukey_Centered->getCoefficient( i );

  } 
  delete apodization_Boxcar_Standard;
  delete apodization_Boxcar_Centered;
  delete apodization_Hamming_Standard;
  delete apodization_Hamming_Centered;
  delete apodization_Tukey_Standard;
  delete apodization_Tukey_Centered;

  gkg::CurvePlotter gnuPlotCurvePlotter_Boxcar_Standard;
  gnuPlotCurvePlotter_Boxcar_Standard.plot( input_Boxcar_Standard,
                                            "x", "boxcar[x]",
                                            "Boxcar Apodization - standard" );

  gkg::CurvePlotter gnuPlotCurvePlotter_Hamming_Standard;
  gnuPlotCurvePlotter_Hamming_Standard.plot( input_Hamming_Standard,
                                             "x", "hamming[x]",
                                             "Hamming Apodization - standard" );

  gkg::CurvePlotter gnuPlotCurvePlotter_Tukey_Standard;
  gnuPlotCurvePlotter_Tukey_Standard.plot( input_Tukey_Standard,
                                           "x", "tukey[x]",
                                           "Tukey Apodization - standard" );

  gkg::CurvePlotter gnuPlotCurvePlotter_Boxcar_Centered;
  gnuPlotCurvePlotter_Boxcar_Centered.plot( input_Boxcar_Centered,
                                            "x", "boxcar[x]",
                                            "Boxcar Apodization - centered" );

  gkg::CurvePlotter gnuPlotCurvePlotter_Hamming_Centered;
  gnuPlotCurvePlotter_Hamming_Centered.plot( input_Hamming_Centered,
                                             "x", "hamming[x]",
                                             "Hamming Apodization - centered" );

  gkg::CurvePlotter gnuPlotCurvePlotter_Tukey_Centered;
  gnuPlotCurvePlotter_Tukey_Centered.plot( input_Tukey_Centered,
                                           "x", "tukey[x]",
                                           "Tukey Apodization - centered" );

  gkg::keyboardHit();

  return EXIT_SUCCESS;

}
