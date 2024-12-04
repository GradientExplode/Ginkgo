#include <gkg-processing-plugin-functors/AddInformationToVolumeHeader/AddInformationToVolumeHeaderCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-histogram/HistogramAnalysis_i.h>
#include <gkg-processing-transform/AcPcIhCoordinates.h>
#include <gkg-processing-transform/TalairachNormalization3d_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
// class InformationProcess
//

class InformationProcess : public gkg::Process
{

  public:

    InformationProcess( const std::string& hanFileName,
                        const std::string& acPcIhFileName,
                        const std::string& brainMaskFileName,
                        const std::string& fileNameOut,
                        const bool& ascii,
                        const std::string& format,
                        const bool& verbose );

    const std::string& getHanFileName() const;
    const std::string& getAcPcIhFileName() const;
    const std::string& getBrainMaskFileName() const;
    const std::string& getFileNameOut() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void addInformation( gkg::Process& process,
                                const std::string& fileNameIn,
                                const gkg::AnalyzedObject&,
                                const std::string& );

    const std::string& _hanFileName;
    const std::string& _acPcIhFileName;
    const std::string& _brainMaskFileName;
    const std::string& _fileNameOut;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


InformationProcess::InformationProcess( const std::string& hanFileName,
                                        const std::string& acPcIhFileName,
                                        const std::string& brainMaskFileName,
                                        const std::string& fileNameOut,
                                        const bool& ascii,
                                        const std::string& format,
                                        const bool& verbose )
                   : gkg::Process(),
                     _hanFileName( hanFileName ),
                     _acPcIhFileName( acPcIhFileName ),
                     _brainMaskFileName( brainMaskFileName ),
                     _fileNameOut( fileNameOut ),
                     _ascii( ascii ),
                     _format( format ),
                     _verbose( verbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &InformationProcess::addInformation< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &InformationProcess::addInformation< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &InformationProcess::addInformation< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &InformationProcess::addInformation< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &InformationProcess::addInformation< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &InformationProcess::addInformation< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &InformationProcess::addInformation< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &InformationProcess::addInformation< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &InformationProcess::addInformation< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &InformationProcess::addInformation< double > );

  }
  GKG_CATCH( "InformationProcess::InformationProcess( "
             "const std::string& hanFileName, "
             "const std::string& acPcIhFileName, "
             "const std::string& brainMaskFileName, "
             "const std::string& fileNameOut, "
             "const bool& ascii, "
             "const std::string& format, "
             "const bool& verbose )" );

}


const std::string& InformationProcess::getHanFileName() const
{

  return _hanFileName;

}


const std::string& InformationProcess::getAcPcIhFileName() const
{

  return _acPcIhFileName;

}


const std::string& InformationProcess::getBrainMaskFileName() const
{

  return _brainMaskFileName;

}


const std::string& InformationProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const bool& InformationProcess::getAscii() const
{

  return _ascii;

}


const std::string& InformationProcess::getFormat() const
{

  return _format;

}


const bool& InformationProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void InformationProcess::addInformation( gkg::Process& process,
                                         const std::string& fileNameIn,
                                         const gkg::AnalyzedObject&,
                                         const std::string& )
{

  try
  {

    InformationProcess&
      informationProcess = static_cast< InformationProcess& >( process );

    ////////////////////////////////////////////////////////////////////////////
    // reading input data
    ////////////////////////////////////////////////////////////////////////////
    if ( informationProcess.getVerbose() )
    {

     std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    std::string format = gkg::Reader::getInstance().read( fileNameIn,
                                                          volumeIn );
    bool ascii = false;
    if ( volumeIn.getHeader().hasAttribute( "io_mode" ) )
    {

      std::string io_mode;
      volumeIn.getHeader().getAttribute( "io_mode", io_mode );
      if ( gkg::IOMode::getInstance().getTypeFromName( io_mode ) == 
           gkg::IOMode::Ascii )
      {

        ascii = true;

      }

    }

    if ( informationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    if ( !informationProcess.getHanFileName().empty() )
    {

      //////////////////////////////////////////////////////////////////////////
      // reading HAN data
      //////////////////////////////////////////////////////////////////////////

      if ( informationProcess.getVerbose() )
      {

        std::cout << "reading '" << informationProcess.getHanFileName()
                  << "' : " << std::flush;

      }
      gkg::HistogramAnalysis histogramAnalysis;
      gkg::Reader::getInstance().read( informationProcess.getHanFileName(),
                                       histogramAnalysis );
      if ( informationProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      if ( informationProcess.getVerbose() )
      {

        std::cout << " gray mean = "
                  << histogramAnalysis.getGrayMatterStatistics().mean 
                  << std::endl;
        std::cout << " gray std = "
                << histogramAnalysis.getGrayMatterStatistics().standardDeviation
                  << std::endl;
        std::cout << " white mean = "
                  << histogramAnalysis.getWhiteMatterStatistics().mean 
                  << std::endl;
        std::cout << " white std = "
               << histogramAnalysis.getWhiteMatterStatistics().standardDeviation
                << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // adding histogram analysis information to input volume header
      //////////////////////////////////////////////////////////////////////////

      histogramAnalysis.addToHeaderedObject( volumeIn );

    }

    if ( !informationProcess.getAcPcIhFileName().empty() )
    {


      //////////////////////////////////////////////////////////////////////////
      // reading ACPC data
      //////////////////////////////////////////////////////////////////////////

      if ( informationProcess.getVerbose() )
      {

        std::cout << "reading '" << informationProcess.getAcPcIhFileName()
                  << "' : " << std::flush;

      }
      gkg::AcPcIhCoordinates acPcIhCoordinates;
      gkg::Reader::getInstance().read( informationProcess.getAcPcIhFileName(),
                                       acPcIhCoordinates );
      if ( informationProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      if ( informationProcess.getVerbose() )
      {

        std::cout << " AC = "
                  << acPcIhCoordinates.integerAnteriorCommissure << std::endl;
        std::cout << " PC = "
                  << acPcIhCoordinates.integerPosteriorCommissure << std::endl;
        std::cout << " IH = "
                  << acPcIhCoordinates.integerInterHemispheric << std::endl;
        std::cout << " ACmm = "
                  << acPcIhCoordinates.anteriorCommissure << std::endl;
        std::cout << " PCmm = "
                  << acPcIhCoordinates.posteriorCommissure << std::endl;
        std::cout << " IHmm = "
                  << acPcIhCoordinates.interHemispheric << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // adding AC/PC/IH coordinates information to input volume header
      //////////////////////////////////////////////////////////////////////////

      acPcIhCoordinates.addToHeaderedObject( volumeIn );


      if ( !informationProcess.getBrainMaskFileName().empty() )
      {

        ////////////////////////////////////////////////////////////////////////
        // reading brain mask binary volume
        ////////////////////////////////////////////////////////////////////////
        if ( informationProcess.getVerbose() )
        {

          std::cout << "reading '" << informationProcess.getBrainMaskFileName()
                    << "' : " << std::flush;

        }
        gkg::Volume< int16_t > brainMask;
        gkg::Reader::getInstance().read(
                                     informationProcess.getBrainMaskFileName(),
                                     brainMask );
        if ( informationProcess.getVerbose() )
        {

          std::cout << "done" << std::endl;

        }

        ////////////////////////////////////////////////////////////////////////
        // processing Talairach normalization
        ////////////////////////////////////////////////////////////////////////
        if ( informationProcess.getVerbose() )
        {

          std::cout << "processing Talairach normalization : " << std::flush;

        }

        gkg::GreaterThanFunction< int16_t > greaterThanFunction( 0 );
        gkg::TalairachNormalization3d< float >
          talairachNormalization3d( brainMask,
                                    greaterThanFunction,
                                    acPcIhCoordinates );

        if ( informationProcess.getVerbose() )
        {

          std::cout << "done" << std::endl;

        }

        ////////////////////////////////////////////////////////////////////////
        // adding Talairach normalization to input volume
        ////////////////////////////////////////////////////////////////////////
        talairachNormalization3d.addToHeaderedObject( volumeIn );

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing output data
    ////////////////////////////////////////////////////////////////////////////
    if ( ( informationProcess.getFileNameOut().empty() ||
           ( informationProcess.getFileNameOut() == fileNameIn ) ) &&
         ( informationProcess.getFormat().empty() ||
           ( format == informationProcess.getFormat() ) ) &&
         ( ascii == informationProcess.getAscii() ) )
    {

      gkg::Writer::getInstance().writeHeader( fileNameIn, volumeIn, format );

    }
    else
    {

      gkg::Writer::getInstance().write( informationProcess.getFileNameOut(),
                                        volumeIn,
                                        informationProcess.getAscii(),
                                        informationProcess.getFormat() );

    }

  }
  GKG_CATCH( "template < class T > "
             "void InformationProcess::addInformation( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//  class AddBorder2VolumeCommand
//


gkg::AddInformationToVolumeHeaderCommand::AddInformationToVolumeHeaderCommand(
                                                              int32_t argc,
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
  GKG_CATCH( "gkg::AddInformationToVolumeHeaderCommand::"
             "AddInformationToVolumeHeaderCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AddInformationToVolumeHeaderCommand::AddInformationToVolumeHeaderCommand(
                                           const std::string& fileNameIn,
                                           const std::string& hanFileName,
                                           const std::string& acPcIhFileName,
                                           const std::string& brainMaskFileName,
                                           const std::string& fileNameOut,
                                           bool ascii,
                                           const std::string& format,
                                           bool verbose )
                                        : gkg::Command()
{

  try
  {

    execute( fileNameIn, hanFileName, acPcIhFileName, brainMaskFileName,
             fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::AddInformationToVolumeHeaderCommand::"
             "AddInformationToVolumeHeaderCommand( "
             "const std::string& fileNameIn, "
             "const std::strinf& hanFileName, "
             "const std::string& acPcIhFileName, "
             "const std::string& brainMaskFileName, "
             "const std::string& fileNameOut, "
             "bool ascii, const std::string& format,bool verbose )" );

}


gkg::AddInformationToVolumeHeaderCommand::AddInformationToVolumeHeaderCommand(
                                             const gkg::Dictionary& parameters )
                                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, hanFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, acPcIhFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, brainMaskFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, hanFileName, acPcIhFileName, brainMaskFileName,
             fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::AddInformationToVolumeHeaderCommand::"
             "AddInformationToVolumeHeaderCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AddInformationToVolumeHeaderCommand::~AddInformationToVolumeHeaderCommand()
{
}


std::string gkg::AddInformationToVolumeHeaderCommand::getStaticName()
{

  try
  {

    return "AddInformationToVolumeHeader";

  }
  GKG_CATCH( "std::string " 
             "gkg::AddInformationToVolumeHeaderCommand::getStaticName()" );

}


void gkg::AddInformationToVolumeHeaderCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string hanFileName;
    std::string acPcIhFileName;
    std::string brainMaskFileName;
    std::string fileNameOut = "";
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Adding information to a volume header: \n"
                                  "- histogram analysis information\n"
                                  "- Talairach's transform\n"
                                  "- Talairach's normalization",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameIn );
    application.addSingleOption( "-han",
                                 "Input histogram analysis file name(*.han)",
                                 hanFileName,
                                 true );
    application.addSingleOption( "-talairach",
                                 "Input AIMS AC/PC/IH coordinates file name. "
                                 "Option '-m' is mandatory for adding "
                                 "normalization information",
                                 acPcIhFileName,
                                 true );
    application.addSingleOption( "-m",
                                 "Brain mask file name (int16_t)",
                                 brainMaskFileName,
                                 true );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( fileNameIn, hanFileName, acPcIhFileName, brainMaskFileName,
             fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::AddInformationToVolumeHeaderCommand::parse()" );

}


void gkg::AddInformationToVolumeHeaderCommand::execute( 
                                           const std::string& fileNameIn,
                                           const std::string& hanFileName,
                                           const std::string& acPcIhFileName,
                                           const std::string& brainMaskFileName,
                                           const std::string& fileNameOut,
                                           bool ascii,
                                           const std::string& format,
                                           bool verbose )
{

  try
  {

    InformationProcess informationProcess( hanFileName,
                                           acPcIhFileName,
                                           brainMaskFileName,
                                           fileNameOut,
                                           ascii,
                                           format,
                                           verbose );

    informationProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::AddInformationToVolumeHeaderCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::strinf& hanFileName, "
             "const std::string& acPcIhFileName, "
             "const std::string& brainMaskFileName, "
             "const std::string& fileNameOut, "
             "bool ascii, const std::string& format,bool verbose )" );

}


RegisterCommandCreator( AddInformationToVolumeHeaderCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( hanFileName ) +
                        DECLARE_STRING_PARAMETER_HELP( acPcIhFileName ) +
                        DECLARE_STRING_PARAMETER_HELP( brainMaskFileName ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
