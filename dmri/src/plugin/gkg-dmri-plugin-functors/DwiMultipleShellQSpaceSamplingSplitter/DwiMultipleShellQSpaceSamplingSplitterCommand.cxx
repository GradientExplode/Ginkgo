#include <gkg-dmri-plugin-functors/DwiMultipleShellQSpaceSamplingSplitter/DwiMultipleShellQSpaceSamplingSplitterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/CustomMultipleDifferentShellQSpaceSampling.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-process/SubVolumeProcess.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <list>


//
// class DwiMultipleShellQSpaceSamplingSplitterProcess
//

class DwiMultipleShellQSpaceSamplingSplitterProcess : public gkg::Process
{

  public:

    DwiMultipleShellQSpaceSamplingSplitterProcess(
                                         const std::string& theFileNameOutputDW,
                                         bool theVerbose );

    const std::string& fileNameOutputDW;
    bool verbose;

  private:

    template < class T >
    static void apply( gkg::Process& process,
                       const std::string& fileNameInputDW,
                       const gkg::AnalyzedObject&,
                       const std::string& );

};
    

DwiMultipleShellQSpaceSamplingSplitterProcess::
                                  DwiMultipleShellQSpaceSamplingSplitterProcess(
                                         const std::string& theFileNameOutputDW,
                                         bool theVerbose )
                                              : gkg::Process( "Volume" ),
                                                fileNameOutputDW(
                                                          theFileNameOutputDW ),
                                                verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &DwiMultipleShellQSpaceSamplingSplitterProcess::
                                                         apply< double > );

  }
  GKG_CATCH( "DwiMultipleShellQSpaceSamplingSplitterProcess::DwiMultipleShellQSpaceSamplingSplitterProcess( "
             "const std::string& theFileNameOutputDW, "
             "bool theVerbose )" );

}


template < class T >
void DwiMultipleShellQSpaceSamplingSplitterProcess::apply(
                                      gkg::Process& process,
                                      const std::string& fileNameInputDW,
                                      const gkg::AnalyzedObject&,
                                      const std::string& )
{

  try
  {

    DwiMultipleShellQSpaceSamplingSplitterProcess&
      dwiMultipleShellQSpaceSamplingSplitterProcess =
        static_cast< DwiMultipleShellQSpaceSamplingSplitterProcess& >(
                                                                      process );

    const std::string& fileNameOutputDW = 
      dwiMultipleShellQSpaceSamplingSplitterProcess.fileNameOutputDW;
    const bool& verbose = dwiMultipleShellQSpaceSamplingSplitterProcess.verbose;

    ////////////////////////////////////////////////////////////////////////////
    // reading DW header
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading input DW '"
                << fileNameInputDW << "' volume header : " << std::flush;

    }
    gkg::Volume< T > inputDw;
    std::string 
      format = gkg::Reader::getInstance().readHeader( fileNameInputDW,
                                                      inputDw );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating Q-space sampling
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "getting Q-space sampling : " << std::flush;

    }
    gkg::RCPointer< gkg::QSpaceSampling >
      qSpaceSampling( gkg::QSpaceSamplingFactory::getInstance().create(
                          inputDw ) );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    gkg::CustomMultipleDifferentShellQSpaceSampling*
      customMultipleDifferentShellQSpaceSampling =
        dynamic_cast< gkg::CustomMultipleDifferentShellQSpaceSampling* >(
                                                  qSpaceSampling.getPointer() );
    if ( customMultipleDifferentShellQSpaceSampling )
    {

      int32_t shellCount = 
                    customMultipleDifferentShellQSpaceSampling->getShellCount();
      if ( verbose )
      {

        std::cout << "shell count : " << shellCount << std::endl;


      }

      std::map< float, std::list< int32_t > > bValueAndIndices;
      std::vector< gkg::RCPointer< gkg::CustomSingleShellQSpaceSampling > >
        customSingleShellQSpaceSamplings =
               customMultipleDifferentShellQSpaceSampling->getShells( 
                                                             bValueAndIndices );

      std::map< float, std::list< int32_t > >::const_iterator
        bi = bValueAndIndices.begin(),
        bie = bValueAndIndices.end();
      std::vector< gkg::RCPointer< 
                        gkg::CustomSingleShellQSpaceSampling > >::const_iterator
        qs = customSingleShellQSpaceSamplings.begin();
      int32_t shellIndex = 1;
      while ( bi != bie )
      {

        if ( verbose )
        {

          std::cout << "extracting shell at b="
                    << bi->first
                    << "s/mm2 : "
                    << std::flush;

        }

        const std::list< int32_t >& indices = bi->second;

        std::string fileNameShellDW =
                                   fileNameOutputDW + "_shell" + 
                                   gkg::StringConverter::toString( shellIndex );
        std::list< int32_t > noneList;
        gkg::SubVolumeProcess 
          subVolumeProcess( fileNameShellDW,
                            -1, -1,
                            -1, -1,
                            -1, -1,
                            -1, -1,
                            noneList,
                            noneList,
                            noneList,
                            indices,
                            false,
                            format,
                            false );
        subVolumeProcess.execute( fileNameInputDW );

        // reading header only 
        gkg::Volume< T > shellDwHeader;
        std::string 
          format = gkg::Reader::getInstance().readHeader( fileNameShellDW,
                                                          shellDwHeader );

        // adding Q-space information
        ( *qs )->addInformationToHeader( shellDwHeader );
                            
        // overwriting header
        gkg::TypedDiskFormat< gkg::Volume< T > >* diskFormat = 
          gkg::DiskFormatFactory< gkg::Volume< T > >::getInstance().
                                                        getDiskFormat( format );
        std::string headerFileName = diskFormat->getNameWithoutExtension(
                                                             fileNameShellDW ) +
                                     diskFormat->getHeaderExtension() ;
        gkg::Writer::getInstance().writeHeader( headerFileName, shellDwHeader );


       if ( verbose )
        {

          std::cout << "done" << std::endl;

        }
        ++ bi;
        ++ qs;
        ++ shellIndex;

      }

    }
    else
    {

      std::cout << "not a custom multiple-different-shell q-space sampling"
                << std::endl; 

    } 

  }
  GKG_CATCH( "template < class T > "
             "void DwiMultipleShellQSpaceSamplingSplitterProcess::apply( "
             "gkg::Process& process, "
             "const std::string& fileNameInputDW, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiMultipleShellQSpaceSamplingSplitterCommand
//

gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::
               DwiMultipleShellQSpaceSamplingSplitterCommand( int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                                   : gkg::Command( argc, argv,
                                                                   loadPlugin,
                                                                   removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::"
             "DwiMultipleShellQSpaceSamplingSplitterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::
                                  DwiMultipleShellQSpaceSamplingSplitterCommand(
                                            const std::string& fileNameInputDW,
                                            const std::string& fileNameOutputDW,
                                            bool verbose )
                                                   : gkg::Command()
{

  try
  {

    execute( fileNameInputDW,
             fileNameOutputDW,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::"
             "DwiMultipleShellQSpaceSamplingSplitterCommand( "
             "const std::string& fileNameInputDW, "
             "const std::string& fileNameOutputDW, "
             "bool verbose )" );

}


gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::
                                 DwiMultipleShellQSpaceSamplingSplitterCommand(
                                             const gkg::Dictionary& parameters )
                                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInputDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputDW );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputDW,
             fileNameOutputDW,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::"
             "DwiMultipleShellQSpaceSamplingSplitterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::
                                ~DwiMultipleShellQSpaceSamplingSplitterCommand()
{
}


std::string gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::getStaticName()
{

  try
  {

    return "DwiMultipleShellQSpaceSamplingSplitter";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::"
             "getStaticName()" );

}


void gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::parse()
{

  try
  {

    std::string fileNameInputDW;
    std::string fileNameOutputDW;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                               "Extracting shells from Q-space multiple shell "
                               " DW volume",
                               _loadPlugin );
    application.addSingleOption( "-dw",
                                 "Input DW file name",
                                 fileNameInputDW );
    application.addSingleOption( "-o",
                                 "Output DW file name",
                                 fileNameOutputDW );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameInputDW,
             fileNameOutputDW,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::"
                     "parse()" );

}


void gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::execute(
                             const std::string& fileNameInputDW,
                             const std::string& fileNameOutputDW,
                             bool verbose )
{

  try
  {

    DwiMultipleShellQSpaceSamplingSplitterProcess
      dwiMultipleShellQSpaceSamplingSplitterProcess( fileNameOutputDW,
                                                     verbose );
    dwiMultipleShellQSpaceSamplingSplitterProcess.execute( fileNameInputDW );

  }
  GKG_CATCH( "void gkg::DwiMultipleShellQSpaceSamplingSplitterCommand::"
             "execute( "
             "const std::string& fileNameInputDW, "
             "const std::string& fileNameOutputDW, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiMultipleShellQSpaceSamplingSplitterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameInputDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutputDW ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
