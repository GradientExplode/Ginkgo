#include <gkg-processing-plugin-functors/SlrRf/SlrRfCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-rfpulse/ExcitationShinnarLeRouxRFPulse.h>
#include <gkg-processing-rfpulse/InversionShinnarLeRouxRFPulse.h>
#include <gkg-processing-rfpulse/SaturationShinnarLeRouxRFPulse.h>
#include <gkg-processing-rfpulse/SpinEchoShinnarLeRouxRFPulse.h>
#include <gkg-processing-signal/HammingWindowedSincMethod.h>
#include <gkg-processing-signal/ParksMcClellanLinearPhaseMethod.h>
#include <gkg-processing-signal/ParksMcClellanMinimumPhaseMethod.h>
#include <gkg-processing-signal/ParksMcClellanMaximumPhaseMethod.h>
#include <gkg-processing-signal/FourierTransform.h>
#include <gkg-processing-rfpulse/Magnetization.h>
#include <gkg-processing-plot/CurvePlotter.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-communication-sysinfo/KeyboardHit.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <cmath>


std::vector< double > 
getMagnitude( const std::vector< std::complex< double > >& data )
{

  std::vector< double > mag( data.size() );
  for ( size_t i = 0; i < data.size(); i++ )
    mag[ i ] = std::sqrt( std::norm( data[ i ] ) );

  return mag;

}


std::vector< double > 
getLogMagnitude( const std::vector< std::complex< double > >& data )
{

  std::vector< double > logmag( data.size() );
  for ( size_t i = 0; i < data.size(); i++ )
    logmag[ i ] = std::log10( std::sqrt( std::norm( data[ i ] ) ) );

  return logmag;

}


std::vector< double > 
getPhase( const std::vector< std::complex< double > >& data )
{

  std::vector< double > pha( data.size() );
  for ( size_t i = 0; i < data.size(); i++ )
    pha[ i ] = std::arg( data[ i ] );

  return pha;

}


void 
writeBrukerPulseFile( const std::string& fileNameOut,
                      const gkg::ShinnarLeRouxRFPulse* shinnarLerouxRFPulse,
                      double timeBandWidth )
{


  std::string mode;
  std::string rotation;
  std::string extension;
  std::string fileNameHeader;
  if ( dynamic_cast< const gkg::ExcitationShinnarLeRouxRFPulse* >(
                                                        shinnarLerouxRFPulse ) )
  {

    mode = "Excitation";
    rotation = "90.0";
    extension = ".exc";
    fileNameHeader = fileNameOut;

  }
  else if ( dynamic_cast< const gkg::InversionShinnarLeRouxRFPulse* >(
                                                        shinnarLerouxRFPulse ) )
  {

    mode = "Inversion";
    rotation = "180.0";
    extension = ".inv";
    fileNameHeader = fileNameOut;

  }
  else if ( dynamic_cast< const gkg::SaturationShinnarLeRouxRFPulse* >(
                                                        shinnarLerouxRFPulse ) )
  {

    mode = "Excitation";
    rotation = "90.0";
    extension = "_sat.exc";
    fileNameHeader = fileNameOut + "_sat";

  }
  else if ( dynamic_cast< const gkg::SpinEchoShinnarLeRouxRFPulse* >(
                                                        shinnarLerouxRFPulse ) )
  {

    mode = "Refocussing";
    rotation = "180.0";
    extension = ".rfc";
    fileNameHeader = fileNameOut;

  }

  double integralFactor = 0.0;
  int32_t i = 0;
  int32_t sampleCount = shinnarLerouxRFPulse->getSampleCount();
  double value = 0.0;
  double maximumValue = 0.0;
  for ( i = 0; i < sampleCount; i++ )
  {

    value = std::abs( std::real( shinnarLerouxRFPulse->getSample( i ) ) );
    if ( value > maximumValue )
    {

      maximumValue = value;

    }

  }
  for ( i = 0; i < sampleCount; i++ )
  {

    integralFactor += std::real( shinnarLerouxRFPulse->getSample( i ) ) / 
                      ( maximumValue * ( double )sampleCount );

  }

  std::ofstream os( ( fileNameOut + extension ).c_str() );

  os << "##TITLE= /GKG/" << fileNameHeader << std::endl;
  os << "##JCAMP-DX= 5.00 BRUKER JCAMP library" << std::endl;
  os << "##DATA TYPE= Shape Data" << std::endl;
  os << "##ORIGIN= BRUKER MEDICAL GMBH" << std::endl;
  os << "##OWNER= <dwe>" << std::endl;
  os << "##DATE= 09/07/30" << std::endl;
  os << "##TIME= 12:18:54" << std::endl;
  os << "##MINX= 0.000000e+00" << std::endl;
  os << "##MAXX= 1.000000e+02" << std::endl;
  os << "##MINY= 0.000000e+00" << std::endl;
  os << "##MAXY= 1.800000e+02" << std::endl;
  os << "##$SHAPE_EXMODE= " << mode << std::endl;
  os << "##$SHAPE_TOTROT= " << rotation << std::endl;
  os << "##$SHAPE_BWFAC= " << timeBandWidth << std::endl;
  os << "##$SHAPE_INTEGFAC= " << integralFactor << std::endl;
  os << "##$SHAPE_REPHFAC=50" << std::endl;
  os << "##$SHAPE_TYPE=conventional" << std::endl;
  os << "##$SHAPE_MODE= 0" << std::endl;
  os << "##NPOINTS= " << sampleCount << std::endl;
  os << "##XYPOINTS= (XY..XY)" << std::endl;
  double realValue = 0.0;
  for ( i = 0; i < sampleCount; i++ )
  {

    realValue = std::real( shinnarLerouxRFPulse->getSample( i ) );
    os << ( std::abs( realValue ) / maximumValue ) * 100.0
       << ", "
       << ( realValue >= 0.0 ? 0.0 : 180.0 )
       << std::endl;

  }
  os << "##END=" << std::endl;

  os.close();

}


//
//   SlrRfCommand
//


gkg::SlrRfCommand::SlrRfCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin,
                                 bool removeFirst )
                 : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::SlrRfCommand::SlrRfCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SlrRfCommand::SlrRfCommand( int32_t sampleCount,
                  		 double timeBandWidth,
                  		 double passBandRipple,
                  		 double stopbandRipple,
                  		 const std::string& pulseType,
                  		 const std::string& filterType,
                  		 const std::string& fileNameOut,
                  		 bool gnuplot,
                  		 bool postscript,
                  		 bool brukerFormat,
                  		 bool verbose )
                 : gkg::Command()
{

  try
  {

    execute( sampleCount, timeBandWidth, passBandRipple, stopbandRipple,
             pulseType, filterType, fileNameOut, gnuplot, postscript,
             brukerFormat, verbose );

  }
  GKG_CATCH( "gkg::SlrRfCommand::SlrRfCommand( "
             "int32_t sampleCount, double timeBandWidth, "
             "double passBandRipple, double stopbandRipple, "
             "const std::string& pulseType, const std::string& filterType, "
             "const std::string& fileNameOut, bool gnuplot, bool postscript, "
             "bool brukerFormat, bool verbose )" );

}


gkg::SlrRfCommand::SlrRfCommand( const gkg::Dictionary& parameters )
                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_INTEGER_PARAMETER( parameters, int32_t, sampleCount );
    DECLARE_FLOATING_PARAMETER( parameters, double, timeBandWidth );
    DECLARE_FLOATING_PARAMETER( parameters, double, passBandRipple );
    DECLARE_FLOATING_PARAMETER( parameters, double, stopbandRipple );
    DECLARE_STRING_PARAMETER( parameters, std::string, pulseType );
    DECLARE_STRING_PARAMETER( parameters, std::string, filterType );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, gnuplot );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, postscript );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, brukerFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( sampleCount, timeBandWidth, passBandRipple, stopbandRipple,
             pulseType, filterType, fileNameOut, gnuplot, postscript,
             brukerFormat, verbose );

  }
  GKG_CATCH( "gkg::SlrRfCommand::SlrRfCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SlrRfCommand::~SlrRfCommand()
{
}


std::string gkg::SlrRfCommand::getStaticName()
{

  try
  {

    return "SlrRf";

  }
  GKG_CATCH( "std::string gkg::SlrRfCommand::getStaticName()" );

}


void gkg::SlrRfCommand::parse()
{

  try
  {

    int32_t sampleCount = 160;
    double timeBandWidth = 8;
    double passBandRipple = 0.01;
    double stopbandRipple = 0.01;
    std::string pulseType = "excitation";
    std::string filterType = "parkslinpha";
    std::string fileNameOut;
    bool gnuplot = false;
    bool postscript = false;
    bool brukerFormat = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Radiofrequency pulse design tool",
                                  _loadPlugin );
    application.addSingleOption( "-samples",
                                 "Sample count (default=160)",
                                 sampleCount,
                                 true );
    application.addSingleOption( "-tbw",
                                 "Time x Bandwidth (default=8.0)",
                                 timeBandWidth,
                                 true );
    application.addSingleOption( "-passband",
                                 "Passband ripple (default=0.01)",
                                 passBandRipple,
                                 true );
    application.addSingleOption( "-stopband",
                                 "Stopband ripple (default=0.01)",
                                 stopbandRipple,
                                 true );
    application.addSingleOption( "-pulse",
                                 "Pulse type : excitation/inversion/saturation/"
                                 "spinecho (default=\"excitation\")",
                                 pulseType,
                                 true );
    application.addSingleOption( "-filter",
                                 "Filter type parkslinpha/parksminpha/"
                                 "parksmaxpha/sinc (default=\"parkslinpha\")",
                                 filterType,
                                 true );
    application.addSingleOption( "-gnuplot",
                                 "Display gnuplot renderings "
                                 "(default=false)",
                                 gnuplot,
                                 true );
    application.addSingleOption( "-postscript",
                                 "Backup files into postscript figures instead"
                                 " of displaying GnuPlot figures "
                                 "(default=false)",
                                 postscript,
                                 true );
    application.addSingleOption( "-brukerFormat",
                                 "Write RF pulse file in BRUKER format"
                                 "(default=false)",
                                 brukerFormat,
                                 true );
    application.addSingleOption( "-o",
                                 "Signa RF pulse image file name",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( sampleCount, timeBandWidth, passBandRipple, stopbandRipple,
             pulseType, filterType, fileNameOut, gnuplot, postscript,
             brukerFormat, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SlrRfCommand::parse()" );

}


void gkg::SlrRfCommand::execute( int32_t sampleCount,
                  		 double timeBandWidth,
                  		 double passBandRipple,
                  		 double stopbandRipple,
                  		 const std::string& pulseType,
                  		 const std::string& filterType,
                  		 const std::string& fileNameOut,
                  		 bool gnuplot,
                  		 bool postscript,
                  		 bool brukerFormat,
                  		 bool verbose )
{

  try
  {

    //
    // sanity checks
    //
    if ( sampleCount <= 0 )
    {

      throw std::runtime_error( "sample count must be strictly positive" );

    }

    if ( timeBandWidth <= 0.0 )
    {

      throw std::runtime_error( "Time x Bandwidth must be strictly positive" );

    }

    if ( ( pulseType != "excitation" ) &&
         ( pulseType != "inversion" ) &&
         ( pulseType != "saturation" ) &&
         ( pulseType != "spinecho" ) )
    {

      throw std::runtime_error( "pulse type should be one of excitation,"
                                " inversion, saturation, spinecho" );

    }

    if ( ( filterType != "parkslinpha" ) &&
         ( filterType != "parksminpha" ) &&
         ( filterType != "parksmaxpha" ) &&
         ( filterType != "sinc" ) )
    {

      throw std::runtime_error( "filter type should be one of parkslinpha,"
                                " parksminpha, parksmaxpha, sinc" );

    }

    // now we select Postscript plotter with Plotter factory if required
    if ( postscript )
    {

      gkg::PlotterFactory::getInstance().select( "PostscriptPlot" );

    }

    //
    // choosing a FIR filter design method
    //
    gkg::FIRFilterDesignMethod* firFilterDesignMethod = NULL;
    if ( filterType == "parkslinpha" )
    {

      firFilterDesignMethod =
        &gkg::ParksMcClellanLinearPhaseMethod::getInstance();

    }
    else if ( filterType == "parksminpha" )
    {

      firFilterDesignMethod =
        &gkg::ParksMcClellanMinimumPhaseMethod::getInstance();

    }
    else if ( filterType == "parksmaxpha" )
    {

      firFilterDesignMethod =
        &gkg::ParksMcClellanMaximumPhaseMethod::getInstance();

    }
    else if ( filterType == "sinc" )
    {

      firFilterDesignMethod = &gkg::HammingWindowedSincMethod::getInstance();

    }


    //
    // choosing the RF pulse type and initializing magnetization
    //
    gkg::ShinnarLeRouxRFPulse* shinnarLerouxRFPulse = 0;
    gkg::Magnetization* magnetization = 0;
    bool crushed = false;

    if ( pulseType == "excitation" )
    {

      gkg::ExcitationShinnarLeRouxRFPulse*
        excitationPulse = new gkg::ExcitationShinnarLeRouxRFPulse(
                                                       sampleCount,
                                                       timeBandWidth,
                                                       passBandRipple,
                                                       stopbandRipple,
                                                       firFilterDesignMethod );
      if ( verbose )
      {

        std::cout << "prescribed (alpha,beta) = "
                  << excitationPulse->getPrescribedCayleyKleinParameters()
                  << std::endl;

      }
      shinnarLerouxRFPulse = excitationPulse;
      magnetization = new gkg::Magnetization( 0.0, 0.0, 1.0 );
      crushed = false;

    }
    else if ( pulseType == "inversion" )
    {

      gkg::InversionShinnarLeRouxRFPulse*
        inversionPulse = new gkg::InversionShinnarLeRouxRFPulse(
                                                       sampleCount,
                                                       timeBandWidth,
                                                       passBandRipple,
                                                       stopbandRipple,
                                                       firFilterDesignMethod );
      if ( verbose )
      {

        std::cout << "prescribed (alpha,beta) = "
                  << inversionPulse->getPrescribedCayleyKleinParameters()
                  << std::endl;

      }
      shinnarLerouxRFPulse = inversionPulse;
      magnetization = new gkg::Magnetization( 0.0, 0.0, 1.0 );
      crushed = false;

    }
    else if ( pulseType == "saturation" )
    {

      gkg::SaturationShinnarLeRouxRFPulse*
        saturationPulse = new gkg::SaturationShinnarLeRouxRFPulse(
                                                       sampleCount,
                                                       timeBandWidth,
                                                       passBandRipple,
                                                       stopbandRipple,
                                                       firFilterDesignMethod );
      if ( verbose )
      {

        std::cout << "prescribed (alpha,beta) = "
                  << saturationPulse->getPrescribedCayleyKleinParameters()
                  << std::endl;

      }
      shinnarLerouxRFPulse = saturationPulse;
      magnetization = new gkg::Magnetization( 0.0, 0.0, 1.0 );
      crushed = false;

    }
    else if ( pulseType == "spinecho" )
    {

      gkg::SpinEchoShinnarLeRouxRFPulse*
        spinEchoPulse = new gkg::SpinEchoShinnarLeRouxRFPulse(
                                                       sampleCount,
                                                       timeBandWidth,
                                                       passBandRipple,
                                                       stopbandRipple,
                                                       firFilterDesignMethod );
      if ( verbose )
      {

        std::cout << "prescribed (alpha,beta) = "
                  << spinEchoPulse->getPrescribedCayleyKleinParameters()
                  << std::endl;

      }
      shinnarLerouxRFPulse = spinEchoPulse;
      magnetization = new gkg::Magnetization( 0.0, 1.0, 0.0 );
      crushed = true;

    }

    int32_t i;
    std::ofstream os;

    //
    // saving complex RF pulse
    //
    if ( !brukerFormat )
    {

      os.open( ( fileNameOut + "_rf.txt" ).c_str() );
      for ( i = 0; i < shinnarLerouxRFPulse->getSampleCount(); i++ )
      {

        os << std::real( shinnarLerouxRFPulse->getSample( i ) ) << "\t"
           << std::imag( shinnarLerouxRFPulse->getSample( i ) ) << std::endl;

      }
      os.close();

    }
    else if ( brukerFormat )
    {

      writeBrukerPulseFile( fileNameOut,
                            shinnarLerouxRFPulse,
                            timeBandWidth );

    }


    //
    // plotting complex RF pulse
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter1;
      plotter1.plot( shinnarLerouxRFPulse->getSamples(),
                     "rank",
                     "real,imaginary",
                     "RF pulse curve" );

    }

    //
    // saving polynom A 
    //
    os.open( ( fileNameOut + "_A.txt" ).c_str() );
    for ( i = 0; i < ( int32_t )shinnarLerouxRFPulse->getPolynomA().size(); 
          i++ )
    {

      os << std::real( shinnarLerouxRFPulse->getPolynomA()[ i ] ) << "\t"
         << std::imag( shinnarLerouxRFPulse->getPolynomA()[ i ] ) << std::endl;

    }
    os.close();

    //
    // plotting complex polynom A
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter2;
      plotter2.plot( shinnarLerouxRFPulse->getPolynomA(),
                     "rank",
                     "real,imaginary",
                     "Polynom A" );

    }

    //
    // saving polynom B
    //
    os.open( ( fileNameOut + "_B.txt" ).c_str() );
    for ( i = 0; i < ( int32_t )shinnarLerouxRFPulse->getPolynomB().size(); 
          i++ )
    {

      os << std::real( shinnarLerouxRFPulse->getPolynomB()[ i ] ) << "\t"
         << std::imag( shinnarLerouxRFPulse->getPolynomB()[ i ] ) << std::endl;

    }
    os.close();

    //
    // plotting complex polynom B
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter3;
      plotter3.plot( shinnarLerouxRFPulse->getPolynomB(),
                     "rank",
                     "real,imaginary",
                     "Polynom B" );

    }

    //
    // processing spectrum of time RF samples
    //
    int32_t nextPowerOfTwo = ( 1 << ( int32_t )ceil(
                       log( ( double )shinnarLerouxRFPulse->getSampleCount() ) /
                       log( 2.0 ) ) ) * 16;
    std::vector< std::complex< double > > 
      spectrum( nextPowerOfTwo, std::complex< double >( 0.0f, 0.0f ) );
    for ( i = 0; i < ( int32_t )shinnarLerouxRFPulse->getSampleCount(); i++ )
    {

      spectrum[ i ] = shinnarLerouxRFPulse->getSample( i );

    }
    gkg::FourierTransform< double >::getInstance().applyCenteredDirect(
                                                               spectrum );


    //
    // plotting magnitude spectrum of RF pulse
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter4;
      plotter4.plot( getMagnitude( spectrum ),
                     "frequency",
                     "abs(cplx)",
                     "Magnitude spectrum of RF pulse" );

    }

    //
    // plotting log-magnitude spectrum of RF pulse
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter5;
      plotter5.plot( getLogMagnitude( spectrum ),
                     "frequency",
                     "log10(abs(cplx))",
                     "Log-magnitude spectrum of RF pulse" );

    }

    //
    // plotting phase spectrum of RF pulse
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter6;
      plotter6.plot( getPhase( spectrum ),
                     "frequency",
                     "arg(cplx)",
                     "Phase spectrum of RF pulse" );

    }

    // simulating selection gradients
    std::vector< double > 
      gradientX( shinnarLerouxRFPulse->getSampleCount(), 
                 2 * M_PI / ( double )shinnarLerouxRFPulse->getSampleCount() );
    std::vector< double > 
      gradientY( shinnarLerouxRFPulse->getSampleCount(), 0.0 );

    if ( verbose )
    {

      std::cout << "in-slice   (alpha,beta) = "
                << shinnarLerouxRFPulse->getEffectiveCayleyKleinParameters(
                                                              gradientX,
                                                              gradientY,
                                                              0.0, 0.0 )
                << std::endl;

    }

    //
    // Bloch equation simulations (neglicting T1 and T2 decay(s))
    //

    //
    // simulating Mxy according to position
    //
    int32_t count = shinnarLerouxRFPulse->getSampleCount();
    std::vector< std::complex< double > > Mxy( count );
    std::vector< double > Mx( count );
    std::vector< double > My( count );
    std::vector< double > Mz( count );
    for ( i = 0; i < count; i++ )
    {

      gkg::Magnetization
        magnetizationPrime = magnetization->applyBlochEquation(
                                                *shinnarLerouxRFPulse,
                                                gradientX,
                                                gradientY,
                                                ( float )( i - count / 2 ), 0.0,
                                                crushed );
      Mxy[ i ] = magnetizationPrime.getMxy();
      Mx[ i ] = magnetizationPrime.getMx();
      My[ i ] = magnetizationPrime.getMy();
      Mz[ i ] = magnetizationPrime.getMz();


    }

    //
    // plotting magnitude of magnetization according to position
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter7;
      plotter7.plot( getMagnitude( Mxy ),
                     "position",
                     "abs(Mxy)",
                     "Magnitude of Mxy magnetization according to position" );

    }

    //
    // plotting magnitude of magnetization according to position
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter8;
      plotter8.plot( getPhase( Mxy ),
                     "position",
                     "arg(Mxy)",
                     "Phase of Mxy magnetization according to position" );

    }

    //
    // plotting Mx according to position
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter9;
      plotter9.plot( Mx,
                     "position",
                     "Mx",
                     "Mx magnetization according to position" );

    }

    //
    // plotting My according to position
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter10;
      plotter10.plot( My,
                      "position",
                      "My",
                      "My magnetization according to position" );

    }

    //
    // plotting Mz according to position
    //
    if ( postscript || gnuplot )
    {

      gkg::CurvePlotter plotter11;
      plotter11.plot( Mz,
                      "position",
                      "Mz",
                      "Mz magnetization according to position" );

    }

    if ( !postscript && gnuplot )
    {

      gkg::keyboardHit();

    }

    delete shinnarLerouxRFPulse;
    delete magnetization;

  }
  GKG_CATCH( "void gkg::SlrRfCommand::execute( "
             "int32_t sampleCount, double timeBandWidth, "
             "double passBandRipple, double stopbandRipple, "
             "const std::string& pulseType, const std::string& filterType, "
             "const std::string& fileNameOut, bool gnuplot, bool postscript, "
             "bool brukerFormat, bool verbose )" );

}


RegisterCommandCreator( SlrRfCommand,
    			DECLARE_INTEGER_PARAMETER_HELP( sampleCount ) +
    			DECLARE_FLOATING_PARAMETER_HELP( timeBandWidth ) +
    			DECLARE_FLOATING_PARAMETER_HELP( passBandRipple ) +
    			DECLARE_FLOATING_PARAMETER_HELP( stopbandRipple ) +
    			DECLARE_STRING_PARAMETER_HELP( pulseType ) +
    			DECLARE_STRING_PARAMETER_HELP( filterType ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( gnuplot ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( postscript ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( brukerFormat ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
