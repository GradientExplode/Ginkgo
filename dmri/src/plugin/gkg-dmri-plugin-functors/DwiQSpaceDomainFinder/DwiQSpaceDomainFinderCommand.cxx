#include <gkg-dmri-plugin-functors/DwiQSpaceDomainFinder/DwiQSpaceDomainFinderCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>


#define GAMMA ( 26748.0 * 1e-2 )


/*

  Stejskal-Tanner' equation for trapezoidal gradients:

  targetAmp ----> +--------------+            +--------------+
                 /                \          /                \
                /                  \        /                  \
            ---+--+--------------+--+------+--+--------------+--+--------

  rampWidth ---<->-
                    pulseWidth      separation
               <--------------------><----->

*/

//
// class DwiQSpaceDomainFinderCommand
//

gkg::DwiQSpaceDomainFinderCommand::DwiQSpaceDomainFinderCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                  : gkg::Command( argc, argv, loadPlugin,
                                                  removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiQSpaceDomainFinderCommand::DwiQSpaceDomainFinderCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiQSpaceDomainFinderCommand::DwiQSpaceDomainFinderCommand(
                                        float maximumEchoTime,
                                        float maximumGradientMagnitude,
                                        float timeExcitation,
                                        float timeExcitationToRamp,
                                        float timeRamp,
                                        float timeRampToRefocusing,
                                        float timeRefocusing,
                                        float timeRampToRead,
                                        float timeRead,
                                        int32_t stepCountEchoTime,
                                        int32_t stepCountGradientMagnitude,
                                        int32_t stepCountLittleDelta,
                                        int32_t stepCountBigDelta,
                                        float minimumBValue,
                                        float maximumBValue,
                                        const std::string& qSpaceDomainFileName,
                                        const std::string& bValueFileName,
                                        const std::string& pythonListFileName,
                                        bool verbose )
                                  : gkg::Command()
{

  try
  {

    execute( maximumEchoTime,
             maximumGradientMagnitude,
             timeExcitation,
             timeExcitationToRamp,
             timeRamp,
             timeRampToRefocusing,
             timeRefocusing,
             timeRampToRead,
             timeRead,
             stepCountEchoTime,
             stepCountGradientMagnitude,
             stepCountLittleDelta,
             stepCountBigDelta,
             minimumBValue,
             maximumBValue,
             qSpaceDomainFileName,
             bValueFileName,
             pythonListFileName,
             verbose );

  }
  GKG_CATCH( "gkg::DwiQSpaceDomainFinderCommand::DwiQSpaceDomainFinderCommand( "
             "float maximumEchoTime, "
             "float maximumGradientMagnitude, "
             "float timeExcitation, "
             "float timeExcitationToRamp, "
             "float timeRamp, "
             "float timeRampToRefocusing, "
             "float timeRefocusing, "
             "float timeRampToRead, "
             "float timeRead, "
             "int32_t stepCountEchoTime, "
             "int32_t stepCountGradientMagnitude, "
             "int32_t stepCountLittleDelta, "
             "int32_t stepCountBigDelta, "
             "float minimumBValue, "
             "float maximumBValue, "
             "const std::string& qSpaceDomainFileName, "
             "const std::string& bValueFileName, "
             "const std::string& pythonListFileName, "
             "bool verbose )" );

}


gkg::DwiQSpaceDomainFinderCommand::DwiQSpaceDomainFinderCommand(
                                             const gkg::Dictionary& parameters )
                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_FLOATING_PARAMETER( parameters, float, maximumEchoTime );
    DECLARE_FLOATING_PARAMETER( parameters, float, maximumGradientMagnitude );
    DECLARE_FLOATING_PARAMETER( parameters, float, timeExcitation );
    DECLARE_FLOATING_PARAMETER( parameters, float, timeExcitationToRamp );
    DECLARE_FLOATING_PARAMETER( parameters, float, timeRamp );
    DECLARE_FLOATING_PARAMETER( parameters, float, timeRampToRefocusing );
    DECLARE_FLOATING_PARAMETER( parameters, float, timeRefocusing );
    DECLARE_FLOATING_PARAMETER( parameters, float, timeRampToRead );
    DECLARE_FLOATING_PARAMETER( parameters, float, timeRead );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, stepCountEchoTime );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, 
                              stepCountGradientMagnitude );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, stepCountLittleDelta );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, stepCountBigDelta );
    DECLARE_FLOATING_PARAMETER( parameters, float, minimumBValue );
    DECLARE_FLOATING_PARAMETER( parameters, float, maximumBValue );
    DECLARE_STRING_PARAMETER( parameters, std::string, qSpaceDomainFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, bValueFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, pythonListFileName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( maximumEchoTime,
             maximumGradientMagnitude,
             timeExcitation,
             timeExcitationToRamp,
             timeRamp,
             timeRampToRefocusing,
             timeRefocusing,
             timeRampToRead,
             timeRead,
             stepCountEchoTime,
             stepCountGradientMagnitude,
             stepCountLittleDelta,
             stepCountBigDelta,
             minimumBValue,
             maximumBValue,
             qSpaceDomainFileName,
             bValueFileName,
             pythonListFileName,
             verbose );

  }
  GKG_CATCH( "gkg::DwiQSpaceDomainFinderCommand::DwiQSpaceDomainFinderCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiQSpaceDomainFinderCommand::~DwiQSpaceDomainFinderCommand()
{
}


std::string gkg::DwiQSpaceDomainFinderCommand::getStaticName()
{

  try
  {

    return "DwiQSpaceDomainFinder";

  }
  GKG_CATCH( "std::string gkg::DwiQSpaceDomainFinderCommand::getStaticName()" );

}


void gkg::DwiQSpaceDomainFinderCommand::parse()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    float maximumEchoTime = 0.0;
    float maximumGradientMagnitude = 0.0;
    float timeExcitation = 0.0;
    float timeExcitationToRamp = 0.0;
    float timeRamp = 0.0;
    float timeRampToRefocusing = 0.0;
    float timeRefocusing = 0.0;
    float timeRampToRead = 0.0;
    float timeRead = 0.0;
    int32_t stepCountEchoTime = 0;
    int32_t stepCountGradientMagnitude = 0;
    int32_t stepCountLittleDelta = 0;
    int32_t stepCountBigDelta = 0;
    float minimumBValue = -1.0;
    float maximumBValue = -1.0;
    std::string qSpaceDomainFileName = ""; 
    std::string bValueFileName = ""; 
    std::string pythonListFileName = ""; 
    bool verbose = false;  

    gkg::Application application( _argc, _argv, 
                                  "Tool to define the domain of possible"
                                  " acquisition for a tuple of "
                                  "(delta, Delta, Gmax, TE) parameters",
                                  _loadPlugin );
    application.addSingleOption( "-maximumEchoTime",
                                 "Maximum echo time TE (in ms)",
                                 maximumEchoTime );
    application.addSingleOption( "-maximumGradientMagnitude",
                                 "Maximum grandient magnitude Gmax (in mT/m)",
                                 maximumGradientMagnitude );
    application.addSingleOption( "-timeExcitation",
                                 "Duration of the excitation module (in ms)",
                                 timeExcitation );
    application.addSingleOption( "-timeExcitationToRamp",
                                 "Laps between excitation and first diffusion"
                                 " gradient pulse (in ms)",
                                 timeExcitationToRamp );
    application.addSingleOption( "-timeRamp",
                                 "Duration of the ramp of diffusion"
                                 " gradient pulses (in ms)",
                                 timeRamp );
    application.addSingleOption( "-timeRampToRefocusing",
                                 "Laps between the first diffusion"
                                 " gradient pulse and the refocusing module"
                                 " (in ms)",
                                 timeRampToRefocusing );
    application.addSingleOption( "-timeRefocusing",
                                 "Duration of the refocusing module (in ms)",
                                 timeRefocusing );
    application.addSingleOption( "-timeRampToRead",
                                 "Laps between the second diffusion"
                                 " gradient pulse and the read module"
                                 " (in ms)",
                                 timeRampToRead );
    application.addSingleOption( "-timeRead",
                                 "Duration of the read module part before "
                                 "echo time (in ms)",
                                 timeRead );
    application.addSingleOption( "-stepCountEchoTime",
                                 "Step count between 0 and maximum echo time",
                                 stepCountEchoTime );
    application.addSingleOption( "-stepCountGradientMagnitude",
                                 "Step count between 0 and maximum gradient "
                                 "magnitude",
                                 stepCountGradientMagnitude );
    application.addSingleOption( "-stepCountLittleDelta",
                                 "Step count between 0 and maximum little "
                                 "delta",
                                 stepCountLittleDelta );
    application.addSingleOption( "-stepCountBigDelta",
                                 "Step count between 0 and maximum big "
                                 "delta",
                                 stepCountBigDelta );
    application.addSingleOption( "-minimumBValue",
                                 "Minimum b-value in s/mm^2 "
                                 "(default=not taken into account",
                                 minimumBValue,
                                 true );
    application.addSingleOption( "-maximumBValue",
                                 "Maximum b-value in s/mm^2 "
                                 "(default=not taken into account",
                                 maximumBValue,
                                 true );
    application.addSingleOption( "-qSpaceDomainFileName",
                                 "Filename of the output 4D volume defining"
                                 " the domain of availibility",
                                 qSpaceDomainFileName );
    application.addSingleOption( "-bValueFileName",
                                 "Filename of the output 4D volume defining"
                                 " the b-value in s/mm^2",
                                 bValueFileName );
    application.addSingleOption( "-pythonListFileName",
                                 "Python filename containing the list of "
                                 "[delta,Delta,tau, G, b, TE] parameters "
                                 "corresponding to feasible on the MRI system",
                                 pythonListFileName );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( maximumEchoTime,
             maximumGradientMagnitude,
             timeExcitation,
             timeExcitationToRamp,
             timeRamp,
             timeRampToRefocusing,
             timeRefocusing,
             timeRampToRead,
             timeRead,
             stepCountEchoTime,
             stepCountGradientMagnitude,
             stepCountLittleDelta,
             stepCountBigDelta,
             minimumBValue,
             maximumBValue,
             qSpaceDomainFileName,
             bValueFileName,
             pythonListFileName,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiQSpaceDomainFinderCommand::parse()" );

}


void gkg::DwiQSpaceDomainFinderCommand::execute(
                                        float maximumEchoTime,
                                        float maximumGradientMagnitude,
                                        float timeExcitation,
                                        float timeExcitationToRamp,
                                        float timeRamp,
                                        float timeRampToRefocusing,
                                        float timeRefocusing,
                                        float timeRampToRead,
                                        float timeRead,
                                        int32_t stepCountEchoTime,
                                        int32_t stepCountGradientMagnitude,
                                        int32_t stepCountLittleDelta,
                                        int32_t stepCountBigDelta,
                                        float minimumBValue,
                                        float maximumBValue,
                                        const std::string& qSpaceDomainFileName,
                                        const std::string& bValueFileName,
                                        const std::string& pythonListFileName,
                                        bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( maximumEchoTime <= 0.0 )
    {

      throw std::runtime_error( "maximum echo time must be strictly positive" );

    }


    if ( maximumGradientMagnitude < 0.0 )
    {

      throw std::runtime_error(
                           "maximum gradient magnitude time must be positive" );

    }

    if ( timeExcitation <= 0.0 )
    {

      throw std::runtime_error( "excitation time must be strictly positive" );

    }

    if ( timeExcitationToRamp < 0.0 )
    {

      throw std::runtime_error(
                        "laps between excitation and first diffusion gradient "
                        "must be positive" );

    }

    if ( timeRamp <= 0.0 )
    {

      throw std::runtime_error(
             "ramp duration of diffusion gradients must be strictly positive" );

    }

    if ( timeRampToRefocusing < 0.0 )
    {

      throw std::runtime_error( "laps between first diffusion gradient and "
                                "refocusing module must be positive" );

    }

    if ( timeRefocusing <= 0.0 )
    {

      throw std::runtime_error( "refocusing time must be strictly positive" );

    }

    if ( timeRampToRead < 0.0 )
    {

      throw std::runtime_error( "laps between second diffusion gradient and "
                                "read module must be positive" );

    }

    if ( timeRead <= 0.0 )
    {

      throw std::runtime_error( "read time must be strictly positive" );

    }

    if ( ( timeExcitation +
           timeExcitationToRamp +
           timeRamp +
           timeRampToRefocusing + 
           timeRefocusing +
           timeRampToRefocusing + 
           timeRamp +
           timeRampToRead +
           timeRead ) > maximumEchoTime )
    {

      throw std::runtime_error( 
            "maximum echo time is too short with respect to other parameters" );

    }

    if ( stepCountEchoTime <= 0 )
    {

      throw std::runtime_error(
                             "echo time step count must be strictly positive" );

    }
    if ( stepCountGradientMagnitude <= 0 )
    {

      throw std::runtime_error(
                    "gradient magnitude step count must be strictly positive" );

    }
    if ( stepCountLittleDelta <= 0 )
    {

      throw std::runtime_error(
                          "little delta step count must be strictly positive" );

    }
    if ( stepCountBigDelta <= 0 )
    {

      throw std::runtime_error(
                             "big delta step count must be strictly positive" );

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // displaying input parameter(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "maximum echo time : "
                << maximumEchoTime
                << " ms"
                << std::endl;
      std::cout << "maximum gradient magnitude : "
                << maximumGradientMagnitude
                << " mT/m"
                << std::endl;
      std::cout << "duration of excitation module : "
                << timeExcitation
                << " ms"
                << std::endl;
      std::cout << "laps between excitation module and first diffusion "
                   "gradient : "
                << timeExcitationToRamp
                << " ms"
                << std::endl;
      std::cout << "duration of diffusion gradient ramp : "
                << timeRamp
                << " ms"
                << std::endl;
      std::cout << "laps between first diffusion gradient and refocusing "
                   "module: "
                << timeRampToRefocusing
                << " ms"
                << std::endl;
      std::cout << "duration of refocusing module : "
                << timeRefocusing
                << " ms"
                << std::endl;
      std::cout << "laps between second diffusion gradient and read "
                   "module: "
                << timeRampToRead
                << " ms"
                << std::endl;
      std::cout << "duration of read module before the echo time : "
                << timeRead
                << " ms"
                << std::endl;
      std::cout << "echo time step count : "
                << stepCountEchoTime
                << std::endl;
      std::cout << "gradient magnitude step count : "
                << stepCountGradientMagnitude
                << std::endl;
      std::cout << "little delta step count : "
                << stepCountLittleDelta
                << std::endl;
      std::cout << "big delta step count : "
                << stepCountBigDelta
                << std::endl;

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // computing domain
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing domain : " << std::flush;

    }

    float deltaEchoTime = maximumEchoTime /
                          ( float )stepCountEchoTime;
    float deltaGradientMagnitude = maximumGradientMagnitude /
                                   ( float )stepCountGradientMagnitude;
    float deltaBigDelta = maximumEchoTime / ( float )stepCountBigDelta;
    float deltaLittleDelta = 0.5 * maximumEchoTime /
                             ( float )stepCountLittleDelta;


    gkg::Volume< int16_t > mask( stepCountLittleDelta + 1,
                                 stepCountBigDelta + 1,
                                 stepCountGradientMagnitude + 1,
                                 stepCountEchoTime + 1 );
    gkg::Volume< float > bValues( stepCountLittleDelta + 1,
                                  stepCountBigDelta + 1,
                                  stepCountGradientMagnitude + 1,
                                  stepCountEchoTime + 1 );
    bValues.fill( 0.0 );


    // opening the python filename on demand
    std::ofstream os;
    if ( !pythonListFileName.empty() )
    {

      os.open( pythonListFileName.c_str() );
      os << "diffusionGradientCharacteristics = [ " << std::endl;

    }

    int32_t echoTimeIndex = 0;
    int32_t gradientMagnitudeIndex = 0;
    int32_t bigDeltaIndex = 0;
    int32_t littleDeltaIndex = 0;

    for ( echoTimeIndex = 0;
          echoTimeIndex < stepCountEchoTime + 1; 
          echoTimeIndex++ )
    {

      float echoTime = ( float )echoTimeIndex * deltaEchoTime;

      for ( gradientMagnitudeIndex = 0;
            gradientMagnitudeIndex < stepCountGradientMagnitude + 1;
            gradientMagnitudeIndex++ )
      {

        float gradientMagnitude = ( float )gradientMagnitudeIndex *
                                  deltaGradientMagnitude;

        for ( bigDeltaIndex = 0;
              bigDeltaIndex < stepCountBigDelta + 1;
              bigDeltaIndex++ )
        {

          float bigDelta = ( float )bigDeltaIndex * deltaBigDelta;

          for ( littleDeltaIndex = 0;
                littleDeltaIndex < stepCountLittleDelta + 1;
                littleDeltaIndex++ )
          {

            float littleDelta = ( float )littleDeltaIndex * deltaLittleDelta;

            float time1 = timeExcitation +
                          timeExcitationToRamp + 
                          littleDelta + 
                          timeRamp +
                          timeRampToRefocusing +
                          0.5 * timeRefocusing;

            float time2 = 0.5 * timeRefocusing +
                          timeRampToRefocusing + 
                          littleDelta + 
                          timeRamp +
                          timeRampToRead +
                          timeRead;

            float time3 = littleDelta +
                          timeRamp +
                          timeRampToRefocusing +
                          timeRefocusing +
                          timeRampToRefocusing;

            float time4 = timeExcitation +
                          timeExcitationToRamp + 
                          bigDelta + 
                          littleDelta + 
                          timeRamp +
                          timeRampToRead;

            if ( time4 <= echoTime )
            {

              float bValue = GAMMA * GAMMA * 1e-9 * 
                             gradientMagnitude * gradientMagnitude *
                             ( littleDelta * littleDelta *
                               ( bigDelta - littleDelta / 3.0 ) +
                               timeRamp * timeRamp * timeRamp / 30.0 -
                               littleDelta * timeRamp * timeRamp / 6.0 );

              if ( ( time1 <= 0.5 * echoTime ) &&
                   ( time2 <= 0.5 * echoTime ) &&
                   ( time3 <= bigDelta ) &&
                   ( ( minimumBValue < 0.0 ) ||
                     ( bValue >= minimumBValue ) ) &&
                   ( ( maximumBValue < 0.0 ) ||
                     ( bValue <= maximumBValue ) ) )
              {

                mask( littleDeltaIndex,
                      bigDeltaIndex,
                      gradientMagnitudeIndex,
                      echoTimeIndex ) = 1;

                bValues( littleDeltaIndex,
                         bigDeltaIndex,
                         gradientMagnitudeIndex,
                         echoTimeIndex ) = bValue;

               if ( !pythonListFileName.empty() )
               {

                  os << "                                    "
                     << "[ " << littleDelta
                     << ", " << bigDelta
                     << ", " << bigDelta - littleDelta / 3.0
                     << ", " << gradientMagnitude
                     << ", " << bValue
                     << ", " << echoTime
                     << " ],"
                     << std::endl;

                }

              }
              else
              {

                mask( littleDeltaIndex,
                      bigDeltaIndex,
                      gradientMagnitudeIndex,
                      echoTimeIndex ) = 0;

              }

            }
            else
            {

              mask( littleDeltaIndex,
                    bigDeltaIndex,
                    gradientMagnitudeIndex,
                    echoTimeIndex ) = 0;

            }

          }

        }

      }

    }

    if ( !pythonListFileName.empty() )
    {

      os << "                                   ]" << std::endl;
      os.close();

    }


    mask.setResolution( gkg::Vector3d< double >(
                                             ( double )deltaLittleDelta,
                                             ( double )deltaBigDelta,
                                             ( double )deltaGradientMagnitude ),
                        ( double )deltaEchoTime );

    bValues.setResolution( gkg::Vector3d< double >(
                                             ( double )deltaLittleDelta,
                                             ( double )deltaBigDelta,
                                             ( double )deltaGradientMagnitude ),
                        ( double )deltaEchoTime );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // saving results
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving domain mask '" <<  qSpaceDomainFileName
                << "' : " << std::flush;

    }

    gkg::Writer::getInstance().write( qSpaceDomainFileName, mask );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "saving b-value volume '" <<  bValueFileName
                << "' : " << std::flush;

    }

    gkg::Writer::getInstance().write( bValueFileName, bValues );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiQSpaceDomainFinderCommand::execute( "
             " )" );

}


RegisterCommandCreator(
    DwiQSpaceDomainFinderCommand,
    DECLARE_FLOATING_PARAMETER_HELP( maximumEchoTime ) +
    DECLARE_FLOATING_PARAMETER_HELP( maximumGradientMagnitude ) +
    DECLARE_FLOATING_PARAMETER_HELP( timeExcitation ) +
    DECLARE_FLOATING_PARAMETER_HELP( timeExcitationToRamp ) +
    DECLARE_FLOATING_PARAMETER_HELP( timeRamp ) +
    DECLARE_FLOATING_PARAMETER_HELP( timeRampToRefocusing ) +
    DECLARE_FLOATING_PARAMETER_HELP( timeRefocusing ) +
    DECLARE_FLOATING_PARAMETER_HELP( timeRampToRead ) +
    DECLARE_FLOATING_PARAMETER_HELP( timeRead ) +
    DECLARE_INTEGER_PARAMETER_HELP( stepCountEchoTime ) +
    DECLARE_INTEGER_PARAMETER_HELP( stepCountGradientMagnitude ) +
    DECLARE_INTEGER_PARAMETER_HELP( stepCountLittleDelta ) +
    DECLARE_INTEGER_PARAMETER_HELP( stepCountBigDelta ) +
    DECLARE_FLOATING_PARAMETER_HELP( minimumBValue ) +
    DECLARE_FLOATING_PARAMETER_HELP( maximumBValue ) +
    DECLARE_STRING_PARAMETER_HELP( qSpaceDomainFileName ) +
    DECLARE_STRING_PARAMETER_HELP( bValueFileName ) +
    DECLARE_STRING_PARAMETER_HELP( pythonListFileName ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );


#undef GAMMA
