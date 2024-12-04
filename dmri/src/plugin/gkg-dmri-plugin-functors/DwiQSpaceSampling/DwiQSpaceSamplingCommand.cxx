#include <gkg-dmri-plugin-functors/DwiQSpaceSampling/DwiQSpaceSamplingCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <list>


//
// class DwiQSpaceSamplingProcess
//

class DwiQSpaceSamplingProcess : public gkg::Process
{

  public:

    DwiQSpaceSamplingProcess(
                          const std::vector< double >& theScalarParameters,
                          const std::vector< std::string >& theStringParameters,
                          const std::vector< float >& theCoefficients,
                          const std::string& theFileNameGradientCharacteristics,
                          bool theVerbose );

    const std::vector< double >& scalarParameters;
    const std::vector< std::string >& stringParameters;
    const std::vector< float >& coefficients;
    const std::string& fileNameGradientCharacteristics;
    bool verbose;

  private:

    template < class T >
    static void apply( gkg::Process& process,
                       const std::string& fileNameDW,
                       const gkg::AnalyzedObject&,
                       const std::string& );

};
    

DwiQSpaceSamplingProcess::DwiQSpaceSamplingProcess(
                          const std::vector< double >& theScalarParameters,
                          const std::vector< std::string >& theStringParameters,
                          const std::vector< float >& theCoefficients,
                          const std::string& theFileNameGradientCharacteristics,
                          bool theVerbose )
                         : gkg::Process( "Volume" ),
                           scalarParameters( theScalarParameters ),
                           stringParameters( theStringParameters ),
                           coefficients( theCoefficients ),
                           fileNameGradientCharacteristics(
                                           theFileNameGradientCharacteristics ),
                           verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &DwiQSpaceSamplingProcess::apply< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &DwiQSpaceSamplingProcess::apply< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &DwiQSpaceSamplingProcess::apply< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &DwiQSpaceSamplingProcess::apply< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &DwiQSpaceSamplingProcess::apply< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &DwiQSpaceSamplingProcess::apply< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &DwiQSpaceSamplingProcess::apply< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &DwiQSpaceSamplingProcess::apply< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &DwiQSpaceSamplingProcess::apply< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &DwiQSpaceSamplingProcess::apply< double > );

  }
  GKG_CATCH( "DwiQSpaceSamplingProcess::DwiQSpaceSamplingProcess( "
             "const std::vector< double >& theScalarParameters, "
             "const std::vector< std::string >& theStringParameters, "
             "const std::vector< float >& theCoefficients, "
             "const std::string& theFileNameGradientCharacteristics, "
             "bool theVerbose )" );

}


template < class T >
void DwiQSpaceSamplingProcess::apply( gkg::Process& process,
                                      const std::string& fileNameDW,
                                      const gkg::AnalyzedObject&,
                                      const std::string& )
{

  try
  {

    DwiQSpaceSamplingProcess&
      dwiQSpaceSamplingProcess = static_cast< DwiQSpaceSamplingProcess& >(
                                                                      process );

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float >
      theCoefficients = dwiQSpaceSamplingProcess.coefficients;
    if ( theCoefficients.empty() )
    {

      theCoefficients.resize( 16U, 0.0f );
      theCoefficients[ 0 ] = 1.0f;
      theCoefficients[ 5 ] = 1.0f;
      theCoefficients[ 10 ] = 1.0f;
      theCoefficients[ 15 ] = 1.0f;

    }
    else if ( theCoefficients.size() != 16U )
    {

       throw std::runtime_error(
             "the homogeneous 3D transform must have exactly 16 coefficients" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading DW header
    ////////////////////////////////////////////////////////////////////////////

    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << "reading DW '" << fileNameDW << "' volume : " << std::flush;

    }
    gkg::Volume< T > dw;
    std::string 
      format = gkg::Reader::getInstance().readHeader( fileNameDW, dw );
    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating homogeneous 3D transform
    ////////////////////////////////////////////////////////////////////////////

    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << "creating homogeneous 3D transform : " << std::flush;

    }
    gkg::HomogeneousTransform3d< float > transform3d( theCoefficients );
    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << transform3d.getDirectCoefficient( 0, 0 ) << " "
                << transform3d.getDirectCoefficient( 0, 1 ) << " "
                << transform3d.getDirectCoefficient( 0, 2 ) << " "
                << transform3d.getDirectCoefficient( 0, 3 ) << std::endl
                << "                                    "
                << transform3d.getDirectCoefficient( 1, 0 ) << " "
                << transform3d.getDirectCoefficient( 1, 1 ) << " "
                << transform3d.getDirectCoefficient( 1, 2 ) << " "
                << transform3d.getDirectCoefficient( 1, 3 ) << std::endl
                << "                                    "
                << transform3d.getDirectCoefficient( 2, 0 ) << " "
                << transform3d.getDirectCoefficient( 2, 1 ) << " "
                << transform3d.getDirectCoefficient( 2, 2 ) << " "
                << transform3d.getDirectCoefficient( 2, 3 ) << std::endl
                << "                                    "
                << transform3d.getDirectCoefficient( 3, 0 ) << " "
                << transform3d.getDirectCoefficient( 3, 1 ) << " "
                << transform3d.getDirectCoefficient( 3, 2 ) << " "
                << transform3d.getDirectCoefficient( 3, 3 ) << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading gradient characteristics
    ////////////////////////////////////////////////////////////////////////////

    gkg::TypedObject< gkg::Dictionary > gradientCharacteristics;
    if ( !dwiQSpaceSamplingProcess.fileNameGradientCharacteristics.empty() )
    {

      gkg::UncommentCounterInputFileStream
      ucis( dwiQSpaceSamplingProcess.fileNameGradientCharacteristics.c_str() );

      if ( !ucis )
      {

        throw gkg::FileNotFoundException(
                     dwiQSpaceSamplingProcess.fileNameGradientCharacteristics );

      }

      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( ucis, gradientCharacteristics );
      ucis.close();

    }
    else
    {

      gradientCharacteristics.getTypedValue()[ "type" ] =
                                                       std::string( "unknown" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating Q-space sampling
    ////////////////////////////////////////////////////////////////////////////

    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << "creating Q-space sampling : " << std::flush;

    }
    gkg::RCPointer< gkg::QSpaceSampling >
      qSpaceSampling( gkg::QSpaceSamplingFactory::getInstance().create(
                          dwiQSpaceSamplingProcess.stringParameters,
                          dwiQSpaceSamplingProcess.scalarParameters,
                          transform3d,
                          gradientCharacteristics.getTypedValue() ) );
    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // adding Q-space sampling information to DW header
    ////////////////////////////////////////////////////////////////////////////

    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << "adding Q-space sampling information to DW header : "
                << std::flush;

    }
    qSpaceSampling->addInformationToHeader( dw );
    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing DW header
    ////////////////////////////////////////////////////////////////////////////

    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << "writing modified DW header : "
                << std::flush;

    }
    
    gkg::TypedDiskFormat< gkg::Volume< T > >* diskFormat = 
      gkg::DiskFormatFactory< gkg::Volume< T > >::getInstance().
                                                        getDiskFormat( format );
    std::string headerFileName = diskFormat->getNameWithoutExtension(
                                                                  fileNameDW ) +
                                 diskFormat->getHeaderExtension() ;
    gkg::Writer::getInstance().writeHeader( headerFileName, dw );
    if ( dwiQSpaceSamplingProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "template < class T > "
             "void DwiQSpaceSamplingProcess::apply( gkg::Process& process, "
             "const std::string& fileNameDW, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiQSpaceSamplingCommand
//

gkg::DwiQSpaceSamplingCommand::DwiQSpaceSamplingCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DwiQSpaceSamplingCommand::DwiQSpaceSamplingCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiQSpaceSamplingCommand::DwiQSpaceSamplingCommand(
                             const std::string& fileNameDW,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             const std::vector< float >& coefficients,
                             const std::string& fileNameGradientCharacteristics,
                             bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameDW,
             scalarParameters,
             stringParameters,
             coefficients,
             fileNameGradientCharacteristics,
             verbose );

  }
  GKG_CATCH( "gkg::DwiQSpaceSamplingCommand::DwiQSpaceSamplingCommand( "
             "const std::string& fileNameDW, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< float >& coefficients, "
             "const std::string& fileNameGradientCharacteristics, "
             "bool verbose )" );

}


gkg::DwiQSpaceSamplingCommand::DwiQSpaceSamplingCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           scalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         stringParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           coefficients );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameGradientCharacteristics );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameDW,
             scalarParameters,
             stringParameters,
             coefficients,
             fileNameGradientCharacteristics,
             verbose );

  }
  GKG_CATCH( "gkg::DwiQSpaceSamplingCommand::DwiQSpaceSamplingCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiQSpaceSamplingCommand::~DwiQSpaceSamplingCommand()
{
}


std::string gkg::DwiQSpaceSamplingCommand::getStaticName()
{

  try
  {

    return "DwiQSpaceSampling";

  }
  GKG_CATCH( "std::string gkg::DwiQSpaceSamplingCommand::getStaticName()" );

}


void gkg::DwiQSpaceSamplingCommand::parse()
{

  try
  {

    std::string fileNameDW;
    std::vector< double > scalarParameters;
    std::vector< std::string > stringParameters;
    std::vector< float > coefficients;
    std::string fileNameGradientCharacteristics;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                           "Adding Q-space sampling information to DW header",
                           _loadPlugin );
    application.addSingleOption( "-dw",
                                 "Input DW file name",
                                 fileNameDW );
    application.addSeriesOption( "-stringParameters",
                                 "String parameters any of:\n"
                                 "1) spherical single-shell gkg\n"
                                 "2) spherical single-shell smsvb15\n"
                                 "3) spherical single-shell gehc\n"
                                 "4) spherical single-shell gehc-new\n"
                                 "5) spherical single-shell custom "
                                 "<filename-bvalues> <filename-orientations>\n"
                                 "6) spherical multiple-shell "
                                 "same-orientation-set gkg\n"
                                 "7) spherical multiple-shell "
                                 "same-orientation-set smsvb15\n"
                                 "8) spherical multiple-shell "
                                 "same-orientation-set gehc\n"
                                 "9) spherical multiple-shell "
                                 "same-orientation-set gehc-new\n"
                                 "10) spherical multiple-shell "
                                 "different-orientation-sets gkg\n"
                                 "11) spherical multiple-shell "
                                 "different-orientation-sets smsvb15\n"
                                 "12) spherical multiple-shell "
                                 "different-orientation-sets gehc\n"
                                 "13) spherical multiple-shell "
                                 "different-orientation-sets gehc-new\n"
                                 "14) spherical multiple-shell "
                                 "different-orientation-sets custom "
                                 "<filename-bvalues> <filename-orientations>\n"
                                 "15) cartesian\n"
                                 "16) arbitrary "
                                 "<filename-bvalues> <filename-orientations>\n"
                                 "17) single-direction "
                                 "<filename-bvalues> <filename-orientations>\n",
                                 stringParameters,
                                 1 );
    application.addSeriesOption( "-scalarParameters",
                                 "Scalar parameters any of:\n"
                                 "1) <b-value> <orientation-count>\n"
                                 "2) <b-value> <orientation-count>\n"
                                 "3) <b-value> <orientation-count>\n"
                                 "4) <b-value> <orientation-count>\n"
                                 "5) N/A\n"
                                 "6) <shell-count> <b-value1> ... <b-valueN> "
                                 "<orientation-count>\n"
                                 "7) <shell-count> <b-value1> ... <b-valueN> "
                                 "<orientation-count>\n"
                                 "8) <shell-count> <b-value1> ... <b-valueN> "
                                 "<orientation-count>\n"
                                 "9) <shell-count> <b-value1> ... <b-valueN> "
                                 "<orientation-count>\n"
                                 "10) <shell-count> <b-value1> ... <b-valueN> "
                                 "<orientation-count1> ... <orientation-countN>"
                                 "\n"
                                 "11) <shell-count> <b-value1> ... <b-valueN> "
                                 "<orientation-count1> ... <orientation-countN>"
                                 "\n"
                                 "12) <shell-count> <b-value1> ... <b-valueN> "
                                 "<orientation-count1> ... <orientation-countN>"
                                 "\n"
                                 "13) <shell-count> <b-value1> ... <b-valueN> "
                                 "<orientation-count1> ... <orientation-countN>"
                                 "\n"
                                 "14) <delta-b-value>\n"
                                 "15) <maximum-b-value> <q-space-point-count>\n"
                                 "16) N/A\n"
                                 "17) N/A",
                                 scalarParameters,
                                 0 );
    application.addSeriesOption( "-homogeneousTransform3d",
                                 "Coefficients of the homogeneous 3D transform"
                                 " (default=identity)",
                                 coefficients );
    application.addSingleOption( "-gradientCharacteristics",
                                 "Dictionary python file providing diffusion "
                                 "gradient characteristics (G, little delta, "
                                 "big delta, ...)",
                                 fileNameGradientCharacteristics,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameDW,
             scalarParameters,
             stringParameters,
             coefficients,
             fileNameGradientCharacteristics,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiQSpaceSamplingCommand::parse()" );

}


void gkg::DwiQSpaceSamplingCommand::execute(
                             const std::string& fileNameDW,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             const std::vector< float >& coefficients,
                             const std::string& fileNameGradientCharacteristics,
                             bool verbose )
{

  try
  {

    DwiQSpaceSamplingProcess
      dwiQSpaceSamplingProcess( scalarParameters,
                                stringParameters,
                                coefficients,
                                fileNameGradientCharacteristics,
                                verbose );
    dwiQSpaceSamplingProcess.execute( fileNameDW );

  }
  GKG_CATCH( "void gkg::DwiQSpaceSamplingCommand::execute( "
             "const std::string& fileNameDW, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< float >& coefficients, "
             "const std::string& fileNameGradientCharacteristics, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiQSpaceSamplingCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( scalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( stringParameters ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( coefficients ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameGradientCharacteristics ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
