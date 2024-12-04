#include <gkg-graphics-QtGL-plugin-functors/DicomDump/DicomDumpCommand.h>
#include <gkg-graphics-QtGL-plugin-functors/DicomDump/DicomDumpMainWindow.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-graphics-QtGL-qtext/Qt.h>
#include <gkg-core-exception/Exception.h>

#include <QApplication>

#include <iostream>
#include <fstream>


gkg::DicomDumpCommand::DicomDumpCommand( int32_t argc,
                                 	 char* argv[],
                                 	 bool loadPlugin,
                                 	 bool removeFirst )
                     : gkg::Command( argc, argv, loadPlugin, removeFirst ),
                       _qtApplication( 0 )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DicomDumpCommand::DicomDumpCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DicomDumpCommand::DicomDumpCommand( const std::string& fileNameIn,
                                         const std::string& fileNameOut,
                                         const std::string& tagName )
                     : gkg::Command(),
                       _qtApplication( 0 )
{

  try
  {

    execute( fileNameIn, fileNameOut, tagName );

  }
  GKG_CATCH( "gkg::DicomDumpCommand::DicomDumpCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& tagName )" );

}


gkg::DicomDumpCommand::DicomDumpCommand(const gkg::Dictionary& parameters )
                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, tagName );

    execute( fileNameIn, fileNameOut, tagName );

  }
  GKG_CATCH( "gkg::DicomDumpCommand::DicomDumpCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DicomDumpCommand::~DicomDumpCommand()
{

  delete _qtApplication;

}


std::string gkg::DicomDumpCommand::getStaticName()
{

  try
  {

    return "DicomDump";

  }
  GKG_CATCH( "std::string gkg::DicomDumpCommand::getStaticName()" );

}


void gkg::DicomDumpCommand::parse()
{

  try
  {

    std::string fileNameIn = "";
    std::string fileNameOut = "";
    std::string tagName = "";

    _qtApplication = new QApplication( _argc, _argv );

    gkg::Application application( _argc, _argv,
                                  "DICOM dumper",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input DICOM file name",
                                 fileNameIn,
                                 true );
    application.addSingleOption( "-o",
                                 "Output header name",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-tag",
                                 "DICOM tag name to be searched",
                                 tagName,
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, tagName );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DicomDumpCommand::parse()" );

}


void gkg::DicomDumpCommand::execute( const std::string& fileNameIn,
                      		     const std::string& fileNameOut,
                      		     const std::string& tagName )
{

  try
  {

    if ( !fileNameIn.empty() &&
         ( !fileNameOut.empty() || !tagName.empty() ) )
    {

      gkg::Volume< int16_t > volume;
      gkg::Reader::getInstance().readHeader( fileNameIn, volume, "dicom" );

      if ( tagName.empty() )
      {

        // opening output file if provided
        std::ofstream* pOfstream = 0;
        if ( !fileNameOut.empty() )
        {

          pOfstream = new std::ofstream( fileNameOut.c_str() );

          if ( pOfstream->good() )
          {

            // writing header
            gkg::BaseObjectWriter baseObjectWriter;
            baseObjectWriter.write( *pOfstream, volume.getHeader() );

            // closing and removing output file stream
            pOfstream->close();
            delete pOfstream;

          }

        }

      }
      else
      {

        gkg::Dictionary dicomDictionary;
        if ( volume.getHeader().hasAttribute( "dicom" ) )
        {

          volume.getHeader().getAttribute( "dicom", dicomDictionary );

        }
        else
        {

          throw std::runtime_error( "no DICOM attribute found" );

        }

        if ( dicomDictionary.find( tagName ) != dicomDictionary.end() )
        {

          try
          {

            std::string
              value = dicomDictionary[ tagName ]->getValue< std::string >();
            std::cout << value << std::endl;

          }
          catch ( std::exception& )
          {

            try
            {

              std::cout << dicomDictionary[ tagName ]->getString() << std::endl;

            }
            catch ( std::exception& )
            {

              try
              {

                std::cout << dicomDictionary[ tagName ]->getScalar() 
                          << std::endl;

              }
              catch ( std::exception& )
              {

                try
                {

                  gkg::GenericObjectList values = dicomDictionary[ tagName ]->
                                           getValue< gkg::GenericObjectList >();
                  gkg::GenericObjectList::const_iterator i = values.begin(),
                                                         ie = values.end();
                  while ( i != ie )
                  {

                    std::cout << ( *i )->getString() << " ";
                    ++ i;

                  }
                  std::cout << std::endl;

                }
                catch ( std::exception& )
                {

                  try
                  {

                    std::vector< std::string > 
                      values = dicomDictionary[ tagName ]->
                                       getValue< std::vector< std::string > >();
                    std::vector< std::string >::const_iterator 
                      i = values.begin(),
                      ie = values.end();
                    while ( i != ie )
                    {

                      std::cout << *i << " ";
                      ++ i;

                    }
                    std::cout << std::endl;

                  }
                  catch ( std::exception& )
                  {

                    throw std::runtime_error( "not a managed tag type" );

                  }

                }

              }

            }

          }

        }
        else
        {

          throw std::runtime_error( "DICOM tag not found" );

        }

      }

    }
    else
    {

      if ( !_qtApplication )
      {

        _qtApplication = new QApplication( _argc, _argv );

      }

      gkg::DicomDumpMainWindow mainWindow;
      mainWindow.show();

      if ( !fileNameIn.empty() )
      {

        mainWindow.loadFile( fileNameIn );

      }

      _qtApplication->exec();

    }

  }
  GKG_CATCH( "void gkg::DicomDumpCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& tagName )" );

}


RegisterCommandCreator( 
    DicomDumpCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( tagName ) );
