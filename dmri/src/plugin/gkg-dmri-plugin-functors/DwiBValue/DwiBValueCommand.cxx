#include <gkg-dmri-plugin-functors/DwiBValue/DwiBValueCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-sequence-tuning/PGSESequenceTuning.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cmath>
#include <iostream>

#define GAM 26748.0/ ( 2 * M_PI )


//
// class DwiBValueCommand
//

gkg::DwiBValueCommand::DwiBValueCommand( int32_t argc,
                                         char* argv[],
                                         bool loadPlugin,
                                         bool removeFirst )
                      : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiBValueCommand::DwiBValueCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBValueCommand::DwiBValueCommand( double maximuGradientAmplitude,
                                         double minimumSlewRate,
                                         double gradientResolution,
                                         double timeSeparation,
                                         double timeBeforeEcho,
                                         double bValue,
                                         double B0,
                                         double ADC,
                                         double averageT2 )
                      : gkg::Command()
{

  try
  {

    execute( maximuGradientAmplitude,
             minimumSlewRate,
             gradientResolution,
             timeSeparation,
             timeBeforeEcho,
             bValue,
             B0,
             ADC,
             averageT2 );

  }
  GKG_CATCH( "gkg::DwiBValueCommand::DwiBValueCommand( "
             "double maximuGradientAmplitude, "
             "double minimumSlewRate, "
             "double gradientResolution, "
             "double timeSeparation, "
             "double timeBeforeEcho, "
             "double bValue, "
             "double B0, "
             "double ADC, "
             "double averageT2  )" );

}


gkg::DwiBValueCommand::DwiBValueCommand( const gkg::Dictionary& parameters )
                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_FLOATING_PARAMETER( parameters, double, maximuGradientAmplitude );
    DECLARE_FLOATING_PARAMETER( parameters, double, minimumSlewRate );
    DECLARE_FLOATING_PARAMETER( parameters, double, gradientResolution );
    DECLARE_FLOATING_PARAMETER( parameters, double, timeSeparation );
    DECLARE_FLOATING_PARAMETER( parameters, double, timeBeforeEcho );
    DECLARE_FLOATING_PARAMETER( parameters, double, bValue );
    DECLARE_FLOATING_PARAMETER( parameters, double, B0 );
    DECLARE_FLOATING_PARAMETER( parameters, double, ADC );
    DECLARE_FLOATING_PARAMETER( parameters, double, averageT2 );
    
    execute( maximuGradientAmplitude,
             minimumSlewRate,
             gradientResolution,
             timeSeparation,
             timeBeforeEcho,
             bValue,
             B0,
             ADC,
             averageT2 );

  }
  GKG_CATCH( "gkg::DwiBValueCommand::DwiBValueCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBValueCommand::~DwiBValueCommand()
{
}


std::string gkg::DwiBValueCommand::getStaticName()
{

  try
  {

    return "DwiBValue";

  }
  GKG_CATCH( "std::string gkg::DwiBValueCommand::getStaticName()" );

}


void gkg::DwiBValueCommand::parse()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    double maximuGradientAmplitude = 0.0;
    double minimumSlewRate = 0.0;
    double gradientResolution = 1.0;  // us
    double timeSeparation = 0.0;
    double timeBeforeEcho = 0;
    double bValue = 0.0;
    double B0 = 0.0;
    double ADC = 0.0;
    double averageT2 = 0.0;

    gkg::Application application( _argc, _argv,
                                  "DWI Stejskal-Tanner' MPGs evaluation",
                                  _loadPlugin );
    application.addSingleOption( "-maximumGradientAmplitude",
                                 "Maximum gradient amplitude in mT/m",
                                 maximuGradientAmplitude );
    application.addSingleOption( "-minimumSlewRate",
                                 "Minimum slew rate in T/m/s",
                                 minimumSlewRate );
    application.addSingleOption( "-gradientResolution",
                                 "Gradient resolution in us (default=1us)",
                                 gradientResolution,
                                 true );
    application.addSingleOption( "-timeSeparation",
                                 "Time separation between the two gradients "
                                 "in ms",
                                 timeSeparation );
    application.addSingleOption( "-timeBeforeEcho",
                                 "Time after end of second diffusion pulse and "
                                 " before echo time in ms",
                                 timeBeforeEcho,
                                 true );
    application.addSingleOption( "-bValue",
                                 "b-balue in s/mm2",
                                 bValue );
    application.addSingleOption( "-B0",
                                 "Static magnetic field in T",
                                 B0,
                                 true );
    application.addSingleOption( "-ADC",
                                 "Apparent diffusion coefficient in mm2/s",
                                 ADC,
                                 true );
    application.addSingleOption( "-averageT2",
                                 "Average T2 value in ms",
                                 averageT2,
                                 true );

    application.initialize();

    execute( maximuGradientAmplitude,
             minimumSlewRate,
             gradientResolution,
             timeSeparation,
             timeBeforeEcho,
             bValue,
             B0,
             ADC,
             averageT2 );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBValueCommand::parse()" );

}


void gkg::DwiBValueCommand::execute( double maximuGradientAmplitude,
                                     double minimumSlewRate,
                                     double gradientResolution,
                                     double timeSeparation,
                                     double timeBeforeEcho,
                                     double bValue,
                                     double B0,
                                     double ADC,
                                     double averageT2 )
{

  try
  {

    double littleDelta = 0.0;
    double bigDelta = 0.0;
    double rampWidth = 0.0;
    double diffusionTime = 0.0;
    double echoTime = 0.0;
    gkg::getPGSEParameters( maximuGradientAmplitude,
                            minimumSlewRate,
                            gradientResolution,
                            timeSeparation,
                            timeBeforeEcho,
                            bValue,
                            littleDelta,
                            bigDelta,
                            rampWidth,
                            diffusionTime,
                            echoTime );

    // ex: maxAmp = 4 G/cm
    //     minSlewRate = 0.015 G/cm/us
    //     separation = 7000 us
    //     bValue = 700 s/mm2
    // gamma in MHz / T = 42.57 
    double gammaInHertzPerTesla = GAM * 1e-2 * 1e6;
    
    double littleDeltaInSecond = littleDelta * 1e-3;
    double maximumGradientAmplitudeInTeslaPerMeter = maximuGradientAmplitude *
                                                     1e-3;

    double q = gammaInHertzPerTesla * littleDeltaInSecond *
               maximumGradientAmplitudeInTeslaPerMeter;      
    

    //double q2 = std::sqrt( bValue / (4*M_PI*M_PI*tau*1e-3) );
    
    std::cout << std::endl;
    std::cout << "Maximum gradient amplitude in mT/m              : "
              << maximuGradientAmplitude
              << std::endl;
    std::cout << "Minimum slew rate in T/m/s                      : "
              << minimumSlewRate
              << std::endl;
    std::cout << "Time separation between the two gradients in ms : "
              << timeSeparation
              << std::endl;
    std::cout << "B-value in s/mm2                                : "
              << bValue
              << std::endl;
    std::cout << "Ramp width in ms                                : "
              << rampWidth
              << std::endl;
    std::cout << "little delta in ms                              : "
              << littleDelta 
              << std::endl;
    std::cout << "big delta in ms                                 : "
              << bigDelta 
              << std::endl;
    std::cout << "diffusion time in ms                            : "
              << diffusionTime 
              << std::endl;
    std::cout << "q in 1/m                                        : "
              << q       
              << std::endl;
    std::cout << "radius in um                                    : "
              << 1 / q * 1e6      
              << std::endl;
    std::cout << "optimum TE in ms                                : "
              << echoTime
              << std::endl;

    double t2Decay = std::exp( -echoTime / averageT2 );
    std::cout << "T2 decay                                        : "
              << t2Decay
              << std::endl;

    double dwAttenuation = std::exp( -bValue * ADC );
    std::cout << "DW attenuation                                  : "
              << dwAttenuation
              << std::endl;

    double t2DecayTimesDwAttenuation = t2Decay * dwAttenuation;
    std::cout << "T2 decay x DW attenuation                       : "
              << t2DecayTimesDwAttenuation
              << std::endl;

    double t2DecayTimesDwAttenuationTimesBO = t2Decay * dwAttenuation * B0;
    std::cout << "T2 decay x DW attenuation x B0                  : "
              << t2DecayTimesDwAttenuationTimesBO
              << std::endl;

  }
  GKG_CATCH( "void gkg::DwiBValueCommand::execute( "
             " )" );

}


RegisterCommandCreator( 
    DwiBValueCommand,
    DECLARE_FLOATING_PARAMETER_HELP( maximuGradientAmplitude ) +
    DECLARE_FLOATING_PARAMETER_HELP( minimumSlewRate ) +
    DECLARE_FLOATING_PARAMETER_HELP( gradientResolution ) +
    DECLARE_FLOATING_PARAMETER_HELP( timeSeparation ) +
    DECLARE_FLOATING_PARAMETER_HELP( timeBeforeEcho ) +
    DECLARE_FLOATING_PARAMETER_HELP( bValue ) +
    DECLARE_FLOATING_PARAMETER_HELP( B0 ) +
    DECLARE_FLOATING_PARAMETER_HELP( ADC ) +
    DECLARE_FLOATING_PARAMETER_HELP( averageT2 ) );

#undef GAM
