#include <gkg-processing-plugin-functors/DataBaseClassifier/DataBaseClassifierCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <set>


gkg::DataBaseClassifierCommand::DataBaseClassifierCommand( int32_t argc,
                                                           char* argv[],
                                                           bool loadPlugin,
                                                           bool removeFirst )
                              : gkg::Command( argc, argv, 
                                              loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DataBaseClassifierCommand::DataBaseClassifierCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DataBaseClassifierCommand::DataBaseClassifierCommand(
                                               const std::string& folderNameIn,
                                               const std::string& folderNameOut,
                                               bool apply,
                                               bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( folderNameIn, folderNameOut, apply, verbose );

  }
  GKG_CATCH( "gkg::DataBaseClassifierCommand::DataBaseClassifierCommand( "
             "const std::string& folderNameIn, "
             "const std::string& folderNameOut, "
             "bool apply, bool verbose )" );

}


gkg::DataBaseClassifierCommand::DataBaseClassifierCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, folderNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, folderNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, apply );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( folderNameIn, folderNameOut, apply, verbose );

  }
  GKG_CATCH( "gkg::DataBaseClassifierCommand::DataBaseClassifierCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DataBaseClassifierCommand::~DataBaseClassifierCommand()
{
}


std::string gkg::DataBaseClassifierCommand::getStaticName()
{

  try
  {

    return "DataBaseClassifier";

  }
  GKG_CATCH( "std::string gkg::DataBaseClassifierCommand::getStaticName()" );

}


void gkg::DataBaseClassifierCommand::parse()
{

  try
  {

    std::string folderNameIn;
    std::string folderNameOut = "DataBase";
    bool apply = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Data base classifer", _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input folder name",
                                 folderNameIn );
    application.addSingleOption( "-o",
                                 "Output folder name",
                                 folderNameOut,
                                 true );
    application.addSingleOption( "-apply",
                                 "Apply really data base construction",
                                 apply, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( folderNameIn, folderNameOut, apply, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DataBaseClassifierCommand::parse()" );

}


void gkg::DataBaseClassifierCommand::execute( const std::string& folderNameIn,
                                              const std::string& folderNameOut,
                                              bool apply,
                                              bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // preparing output directories
    ////////////////////////////////////////////////////////////////////////////

    gkg::Directory outputDirectory( folderNameOut );
    if ( !outputDirectory.isValid() )
    {

      outputDirectory.mkdir();

    }
    gkg::Directory output00Y19YDirectory( outputDirectory.getPath() +
                                          gkg::getDirectorySeparator() + 
                                          "00Y-19Y" );
    if ( !output00Y19YDirectory.isValid() )
    {

      output00Y19YDirectory.mkdir();

    }
    gkg::Directory output20Y34YDirectory( outputDirectory.getPath() +
                                         gkg::getDirectorySeparator() + 
                                         "20Y-34Y" );
    if ( !output20Y34YDirectory.isValid() )
    {

      output20Y34YDirectory.mkdir();

    }
    gkg::Directory output35Y49YDirectory( outputDirectory.getPath() +
                                         gkg::getDirectorySeparator() + 
                                         "35Y-49Y" );
    if ( !output35Y49YDirectory.isValid() )
    {

      output35Y49YDirectory.mkdir();

    }
    gkg::Directory output50Y64YDirectory( outputDirectory.getPath() +
                                         gkg::getDirectorySeparator() + 
                                         "50Y-64" );
    if ( !output50Y64YDirectory.isValid() )
    {

      output50Y64YDirectory.mkdir();

    }
    gkg::Directory output65Y79YDirectory( outputDirectory.getPath() +
                                         gkg::getDirectorySeparator() + 
                                         "65Y-79Y" );
    if ( !output65Y79YDirectory.isValid() )
    {

      output65Y79YDirectory.mkdir();

    }
    gkg::Directory output80Y99YDirectory( outputDirectory.getPath() +
                                         gkg::getDirectorySeparator() + 
                                         "80Y-99Y" );
    if ( !output80Y99YDirectory.isValid() )
    {

      output80Y99YDirectory.mkdir();

    }


    ////////////////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////////////////

    gkg::Directory inputDirectory( folderNameIn );

    if ( !inputDirectory.isValid() )
    {

      throw std::runtime_error( "not a valid input directory" );

    }

    std::set< std::string > fileNames = inputDirectory.getFiles();
    std::set< std::string >::const_iterator
      f = fileNames.begin(),
      fe = fileNames.end();
    while ( f != fe )
    {

      if ( ( f->length() > 9 ) &&
           ( f->substr( f->length() - 9, 9 ) == ".ima.minf" ) )
      {

        std::string inputFileName = inputDirectory.getPath() + 
                                    gkg::getDirectorySeparator() +
                                    *f;

        std::cout << "--------------------------------------------------------"
                     "--------------------------"
                  << std::endl;
        std::cout << inputFileName << std::endl;

        gkg::VolumeProxy< int16_t > volumeProxy;
        gkg::Reader::getInstance().preparePartialBinaryRead<
                                    gkg::Volume< int16_t > >( inputFileName,
                                                              volumeProxy );
        
        gkg::Dictionary dicomDictionary;
        if ( volumeProxy.getHeader().hasAttribute( "dicom" ) )
        {

          volumeProxy.getHeader().getAttribute( "dicom", dicomDictionary );

        }
        else
        {

          throw std::runtime_error( "no DICOM attribute found" );

        }

        std::string patientAge;
        std::string patientSex;
        std::string protocolName;
        std::string examDate;
        if ( dicomDictionary.find( "(0010,1010)" ) != dicomDictionary.end() )
        {

          patientAge = dicomDictionary[ "(0010,1010)" ]->getValue<
                                                                std::string >();

        }
        if ( dicomDictionary.find( "(0010,0040)" ) != dicomDictionary.end() )
        {

          patientSex = dicomDictionary[ "(0010,0040)" ]->getValue<
                                                                std::string >();
          patientSex = ( patientSex == "M" ? "male" : "female" );

        }
        if ( dicomDictionary.find( "(0008,1030)" ) != dicomDictionary.end() )
        {

          protocolName = gkg::StringConverter::toPureAscii(
                           dicomDictionary[ "(0008,1030)" ]->getValue<
                                                              std::string >() );
          protocolName = gkg::StringConverter::toazAZ09( protocolName );

        }
        if ( dicomDictionary.find( "(0008,0012)" ) != dicomDictionary.end() )
        {

          examDate = dicomDictionary[ "(0008,0012)" ]->getValue<
                                                                std::string >();

        }
        std::cout << "Patient's age : " << patientAge << std::endl;
        std::cout << "Patient's sex : " << patientSex << std::endl;
        std::cout << "Protocol      : " << protocolName << std::endl;
        std::cout << "Exam date     : " << examDate << std::endl;

        int32_t integerPatientAge = 0;
        gkg::StringConverter::toScalar(
          patientAge.substr( 0, patientAge.length() - 1 ),
          integerPatientAge );

        gkg::Directory* outputAgeDirectory = 0;
        if ( ( integerPatientAge >= 0 ) && ( integerPatientAge <= 19 ) )
        {

          outputAgeDirectory = &output00Y19YDirectory;

        }
        else if ( ( integerPatientAge >= 20 ) && ( integerPatientAge <= 34 ) )
        {

          outputAgeDirectory = &output20Y34YDirectory;

        }
        else if ( ( integerPatientAge >= 35 ) && ( integerPatientAge <= 49 ) )
        {

          outputAgeDirectory = &output35Y49YDirectory;

        }
        else if ( ( integerPatientAge >= 50 ) && ( integerPatientAge <= 64 ) )
        {

          outputAgeDirectory = &output50Y64YDirectory;

        }
        else if ( ( integerPatientAge >= 65 ) && ( integerPatientAge <= 79 ) )
        {

          outputAgeDirectory = &output65Y79YDirectory;

        }
        else if ( ( integerPatientAge >= 80 ) && ( integerPatientAge <= 99 ) )
        {

          outputAgeDirectory = &output80Y99YDirectory;

        }

        std::string sourceRootName = inputDirectory.getPath() +
                                     gkg::getDirectorySeparator() +
                                     f->substr( 0, f->length() - 9 );

        std::string destinationRootName = outputAgeDirectory->getPath() +
                                          gkg::getDirectorySeparator() +
                                          patientSex + '_' +
                                          protocolName + '_' +
                                          examDate + '_' +
                                          f->substr( 0, f->length() - 9 );
        std::cout << sourceRootName + ".dim" 
                  << " -> "
                  << destinationRootName + ".dim"
                  << std::endl;
        std::cout << sourceRootName + ".ima" 
                  << " -> "
                  << destinationRootName + ".ima"
                  << std::endl;
        std::cout << sourceRootName + ".ima.minf" 
                  << " -> "
                  << destinationRootName + ".ima.minf"
                  << std::endl;

        std::string command1 = std::string( "cp " ) +
                               sourceRootName + ".dim" + " " +
                               destinationRootName + ".dim";

        std::string command2 = std::string( "cp " ) +
                               sourceRootName + ".ima" + " " +
                               destinationRootName + ".ima";

        std::string command3 = std::string( "cp " ) +
                               sourceRootName + ".ima.minf" + " " +
                               destinationRootName + ".ima.minf";

        //std::cout << command1 << std::endl;
        //std::cout << command2 << std::endl;
        //std::cout << command3 << std::endl;

        if ( apply )
        {

          int32_t status = system( command1.c_str() );
	  if ( status < 0 )
	  {
	  
	    throw std::runtime_error( "Error executing command 1" );
	  
	  }
	  
          status = system( command2.c_str() );
	  if ( status < 0 )
	  {
	  
	    throw std::runtime_error( "Error executing command 2" );
	  
	  }

          status = system( command3.c_str() );
	  if ( status < 0 )
	  {
	  
	    throw std::runtime_error( "Error executing command 3" );
	  
	  }

        }

      }
      ++ f;

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DataBaseClassifierCommand::execute( "
             "const std::string& folderNameIn, "
             "const std::string& folderNameOut, "
             "bool apply, bool verbose )" );

}


RegisterCommandCreator( DataBaseClassifierCommand,
                        DECLARE_STRING_PARAMETER_HELP( folderNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( folderNameOut ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( apply ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
