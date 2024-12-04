#include <gkg-processing-plugin-functors/MorphologicalOperation/MorphologicalOperationCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-morphology/Erosion.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-morphology/Closing.h>
#include <gkg-processing-morphology/Opening.h>
#include <gkg-processing-morphology/Skeletonization.h>
#include <gkg-processing-morphology/WatershededTransformation.h>
#include <gkg-processing-algobase/LowerThanFunction_i.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/DifferentFromFunction_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-algobase/BetweenOrEqualToFunction_i.h>
#include <gkg-processing-algobase/BetweenFunction_i.h>
#include <gkg-processing-algobase/OutsideOrEqualToFunction_i.h>
#include <gkg-processing-algobase/OutsideFunction_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
//   MorphologyProcess
//


class MorphologyProcess : public gkg::Process
{

  public:

    MorphologyProcess( const std::string& operation,
                       const float& radius,
                       const int32_t& neighborhood,
                       const std::string& fileNameOut,
                       const std::string& outType,
                       const std::string& mode,
                       const double& threshold1,
                       const double& threshold2,
                       const double& foreground,
                       const double& background,
                       const bool& ascii,
                       const std::string& format,
                       const bool& verbose );

    const std::string& getOperation() const;
    const float& getRadius() const;
    const int32_t& getNeighborhood() const;
    const std::string& getFileNameOut() const;
    const std::string& getOutType() const;
    const std::string& getMode() const;
    const double& getThreshold1() const;
    const double& getThreshold2() const;
    const double& getForeground() const;
    const double& getBackground() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::string& _operation;
    const float& _radius;
    const int32_t& _neighborhood;
    const std::string& _fileNameOut;
    const std::string& _outType;
    const std::string& _mode;
    const double& _threshold1;
    const double& _threshold2;
    const double& _foreground;
    const double& _background;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


MorphologyProcess::MorphologyProcess( const std::string& operation,
                                      const float& radius,
                                      const int32_t& neighborhood,
                                      const std::string& fileNameOut,
                                      const std::string& outType,
                                      const std::string& mode,
                                      const double& threshold1,
                                      const double& threshold2,
                                      const double& foreground,
                                      const double& background,
                                      const bool& ascii,
                                      const std::string& format,
                                      const bool& verbose)
                  : gkg::Process( "Volume" ),
                    _operation( operation ),
                    _radius( radius ),
                    _neighborhood( neighborhood ),
                    _fileNameOut( fileNameOut ),
                    _outType( outType ),
                    _mode( mode ),
                    _threshold1( threshold1 ),
                    _threshold2( threshold2 ),
                    _foreground( foreground ),
                    _background( background ),
                    _ascii( ascii ),
                    _format( format ),
                    _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &MorphologyProcess::operate< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &MorphologyProcess::operate< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &MorphologyProcess::operate< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &MorphologyProcess::operate< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &MorphologyProcess::operate< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &MorphologyProcess::operate< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &MorphologyProcess::operate< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &MorphologyProcess::operate< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &MorphologyProcess::operate< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &MorphologyProcess::operate< double > );

}


const std::string& MorphologyProcess::getOperation() const
{

  return _operation;

}


const float& MorphologyProcess::getRadius() const
{

  return _radius;

}


const int32_t& MorphologyProcess::getNeighborhood() const
{

  return _neighborhood;

}


const std::string& MorphologyProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& MorphologyProcess::getOutType() const
{

  return _outType;

}


const std::string& MorphologyProcess::getMode() const
{

  return _mode;

}


const double& MorphologyProcess::getThreshold1() const
{

  return _threshold1;

}


const double& MorphologyProcess::getThreshold2() const
{

  return _threshold2;

}


const double& MorphologyProcess::getForeground() const
{

  return _foreground;

}


const double& MorphologyProcess::getBackground() const
{

  return _background;

}


const bool& MorphologyProcess::getAscii() const
{

  return _ascii;

}


const std::string& MorphologyProcess::getFormat() const
{

  return _format;

}


const bool& MorphologyProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void MorphologyProcess::operate( gkg::Process& process,
                                 const std::string& fileNameIn,
                                 const gkg::AnalyzedObject&,
                                 const std::string& )
{

  try
  {

    MorphologyProcess&
      morphologyProcess = static_cast< MorphologyProcess& >( process );

    // reading data
    if ( morphologyProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( morphologyProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( morphologyProcess.getVerbose() )
    {

      std::cout << "applying " << morphologyProcess.getOperation() << " : "
                << std::flush;

    }

    gkg::TestFunction< T >* testFunction = 0;
    if ( morphologyProcess.getMode() == "lt" )
    {

      testFunction = new gkg::LowerThanFunction< T >(
                                       ( T )morphologyProcess.getThreshold1() );

    }
    else if ( morphologyProcess.getMode() == "le" )
    {

      testFunction = new gkg::LowerOrEqualToFunction< T >(
                                       ( T )morphologyProcess.getThreshold1() );

    }
    else if ( morphologyProcess.getMode() == "eq" )
    {

      testFunction = new gkg::EqualToFunction< T >(
                                       ( T )morphologyProcess.getThreshold1() );

    }
    else if ( morphologyProcess.getMode() == "di" )
    {

      testFunction = new gkg::DifferentFromFunction< T >(
                                       ( T )morphologyProcess.getThreshold1() );

    }
    else if ( morphologyProcess.getMode() == "ge" )
    {

      testFunction = new gkg::GreaterOrEqualToFunction< T >(
                                       ( T )morphologyProcess.getThreshold1() );

    }
    else if ( morphologyProcess.getMode() == "gt" )
    {

      testFunction = new gkg::GreaterThanFunction< T >(
                                       ( T )morphologyProcess.getThreshold1() );

    }
    else if ( morphologyProcess.getMode() == "be" )
    {

      testFunction = new gkg::BetweenOrEqualToFunction< T >(
                                       ( T )morphologyProcess.getThreshold1(),
                                       ( T )morphologyProcess.getThreshold2() );

    }
    else if ( morphologyProcess.getMode() == "bt" )
    {

      testFunction = new gkg::BetweenFunction< T >(
                                       ( T )morphologyProcess.getThreshold1(),
                                       ( T )morphologyProcess.getThreshold2() );

    }
    else if ( morphologyProcess.getMode() == "oe" )
    {

      testFunction = new gkg::OutsideOrEqualToFunction< T >(
                                       ( T )morphologyProcess.getThreshold1(),
                                       ( T )morphologyProcess.getThreshold2() );

    }
    else if ( morphologyProcess.getMode() == "ou" )
    {

      testFunction = new gkg::OutsideFunction< T >(
                                       ( T )morphologyProcess.getThreshold1(),
                                       ( T )morphologyProcess.getThreshold2() );

    }

    std::string outType = morphologyProcess.getOutType();
    if ( outType.empty() )
    {

      outType = gkg::TypeOf< int16_t >::getName();

    }

    if ( gkg::TypeOf< uint8_t >::isSameType( outType ) )
    {

      gkg::Volume< uint8_t > volumeOut;

      if ( morphologyProcess.getOperation() == "erosion" )
      {

        gkg::Erosion< T, uint8_t >
          erosion( morphologyProcess.getRadius(),
                   *testFunction,
                   ( uint8_t )morphologyProcess.getForeground(),
                   ( uint8_t )morphologyProcess.getBackground() );
        erosion.erode( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "dilation" )
      {

        gkg::Dilation< T, uint8_t >
          dilation( morphologyProcess.getRadius(),
                    *testFunction,
                    ( uint8_t )morphologyProcess.getForeground(),
                    ( uint8_t )morphologyProcess.getBackground() );
        dilation.dilate( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "closing" )
      {

        gkg::Closing< T, uint8_t >
          closing( morphologyProcess.getRadius(),
                   *testFunction,
                   ( uint8_t )morphologyProcess.getForeground(),
                   ( uint8_t )morphologyProcess.getBackground() );
        closing.close( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "opening" )
      {

        gkg::Opening< T, uint8_t >
          opening( morphologyProcess.getRadius(),
                   *testFunction,
                   ( uint8_t )morphologyProcess.getForeground(),
                   ( uint8_t )morphologyProcess.getBackground() );
        opening.open( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "skeletonization" )
      {

        gkg::Skeletonization< T, uint8_t >
          skeletonization( *testFunction,
                   ( uint8_t )morphologyProcess.getForeground(),
                   ( uint8_t )morphologyProcess.getBackground() );
        skeletonization.skeletonize( volumeIn, 
                                     volumeOut, 
                                     morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "watershed" )
      {

        gkg::Neighborhood3d::Type
          neighborhood = gkg::Neighborhood3d::Neighborhood3d_6;
        if ( morphologyProcess.getNeighborhood() == 18 )
        {

          neighborhood = gkg::Neighborhood3d::Neighborhood3d_18;

        }
        else if ( morphologyProcess.getNeighborhood() == 26 )
        {

          neighborhood = gkg::Neighborhood3d::Neighborhood3d_26;

        }
        gkg::WatershededTransformation< T, uint8_t >
          watershededTransformation(
                   neighborhood,
                   *testFunction,
                   ( uint8_t )morphologyProcess.getForeground(),
                   ( uint8_t )morphologyProcess.getBackground() );
        watershededTransformation.watershed(
                                     volumeIn, 
                                     volumeOut, 
                                     morphologyProcess.getVerbose() );

      }

      if ( morphologyProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( morphologyProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << morphologyProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( morphologyProcess.getFileNameOut(),
                                        volumeOut,
                                        morphologyProcess.getAscii(),
                                        morphologyProcess.getFormat() );
      if ( morphologyProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( outType ) )
    {

      gkg::Volume< int16_t > volumeOut;

      if ( morphologyProcess.getOperation() == "erosion" )
      {

        gkg::Erosion< T, int16_t >
          erosion( morphologyProcess.getRadius(),
                   *testFunction,
                   ( int16_t )morphologyProcess.getForeground(),
                   ( int16_t )morphologyProcess.getBackground() );
        erosion.erode( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "dilation" )
      {

        gkg::Dilation< T, int16_t >
          dilation( morphologyProcess.getRadius(),
                    *testFunction,
                    ( int16_t )morphologyProcess.getForeground(),
                    ( int16_t )morphologyProcess.getBackground() );
        dilation.dilate( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "closing" )
      {

        gkg::Closing< T, int16_t >
          closing( morphologyProcess.getRadius(),
                   *testFunction,
                   ( int16_t )morphologyProcess.getForeground(),
                   ( int16_t )morphologyProcess.getBackground() );
        closing.close( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "opening" )
      {

        gkg::Opening< T, int16_t >
          opening( morphologyProcess.getRadius(),
                   *testFunction,
                   ( int16_t )morphologyProcess.getForeground(),
                   ( int16_t )morphologyProcess.getBackground() );
        opening.open( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "skeletonization" )
      {

        gkg::Skeletonization< T, int16_t >
          skeletonization( *testFunction,
                   ( int16_t )morphologyProcess.getForeground(),
                   ( int16_t )morphologyProcess.getBackground() );
        skeletonization.skeletonize( volumeIn, 
                                     volumeOut, 
                                     morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "watershed" )
      {

        gkg::Neighborhood3d::Type
          neighborhood = gkg::Neighborhood3d::Neighborhood3d_6;
        if ( morphologyProcess.getNeighborhood() == 18 )
        {

          neighborhood = gkg::Neighborhood3d::Neighborhood3d_18;

        }
        else if ( morphologyProcess.getNeighborhood() == 26 )
        {

          neighborhood = gkg::Neighborhood3d::Neighborhood3d_26;

        }
        gkg::WatershededTransformation< T, int16_t >
          watershededTransformation(
                   neighborhood,
                   *testFunction,
                   ( int16_t )morphologyProcess.getForeground(),
                   ( int16_t )morphologyProcess.getBackground() );
        watershededTransformation.watershed(
                                     volumeIn, 
                                     volumeOut, 
                                     morphologyProcess.getVerbose() );

      }

      if ( morphologyProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( morphologyProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << morphologyProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( morphologyProcess.getFileNameOut(),
                                        volumeOut,
                                        morphologyProcess.getAscii(),
                                        morphologyProcess.getFormat() );
      if ( morphologyProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int32_t >::isSameType( outType ) )
    {

      gkg::Volume< int32_t > volumeOut;

      if ( morphologyProcess.getOperation() == "erosion" )
      {

        gkg::Erosion< T, int32_t >
          erosion( morphologyProcess.getRadius(),
                   *testFunction,
                   ( int32_t )morphologyProcess.getForeground(),
                   ( int32_t )morphologyProcess.getBackground() );
        erosion.erode( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "dilation" )
      {

        gkg::Dilation< T, int32_t >
          dilation( morphologyProcess.getRadius(),
                    *testFunction,
                    ( int32_t )morphologyProcess.getForeground(),
                    ( int32_t )morphologyProcess.getBackground() );
        dilation.dilate( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "closing" )
      {

        gkg::Closing< T, int32_t >
          closing( morphologyProcess.getRadius(),
                   *testFunction,
                   ( int32_t )morphologyProcess.getForeground(),
                   ( int32_t )morphologyProcess.getBackground() );
        closing.close( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "opening" )
      {

        gkg::Opening< T, int32_t >
          opening( morphologyProcess.getRadius(),
                   *testFunction,
                   ( int32_t )morphologyProcess.getForeground(),
                   ( int32_t )morphologyProcess.getBackground() );
        opening.open( volumeIn, volumeOut, morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "skeletonization" )
      {

        gkg::Skeletonization< T, int32_t >
          skeletonization( *testFunction,
                   ( int32_t )morphologyProcess.getForeground(),
                   ( int32_t )morphologyProcess.getBackground() );
        skeletonization.skeletonize( volumeIn, 
                                     volumeOut, 
                                     morphologyProcess.getVerbose() );

      }
      else if ( morphologyProcess.getOperation() == "watershed" )
      {

        gkg::Neighborhood3d::Type
          neighborhood = gkg::Neighborhood3d::Neighborhood3d_6;
        if ( morphologyProcess.getNeighborhood() == 18 )
        {

          neighborhood = gkg::Neighborhood3d::Neighborhood3d_18;

        }
        else if ( morphologyProcess.getNeighborhood() == 26 )
        {

          neighborhood = gkg::Neighborhood3d::Neighborhood3d_26;

        }
        gkg::WatershededTransformation< T, int32_t >
          watershededTransformation(
                   neighborhood,
                   *testFunction,
                   ( int32_t )morphologyProcess.getForeground(),
                   ( int32_t )morphologyProcess.getBackground() );
        watershededTransformation.watershed(
                                     volumeIn, 
                                     volumeOut, 
                                     morphologyProcess.getVerbose() );

      }

      if ( morphologyProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( morphologyProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << morphologyProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( morphologyProcess.getFileNameOut(),
                                        volumeOut,
                                        morphologyProcess.getAscii(),
                                        morphologyProcess.getFormat() );
      if ( morphologyProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void MorphologyProcess::operate( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//  MorphologicalOperationCommand
//


gkg::MorphologicalOperationCommand::MorphologicalOperationCommand( 
                                                              int32_t argc,
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
  GKG_CATCH( "gkg::MorphologicalOperationCommand::MorphologicalOperationCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MorphologicalOperationCommand::MorphologicalOperationCommand(
                                   		 const std::string& fileNameIn,
                                   		 const std::string& fileNameOut,
                                   		 const std::string& operation,
                                   		 float radius,
                                   		 int32_t neighborhood,
                                   		 const std::string& outType,
                                   		 const std::string& mode,
                                   		 double threshold1,
                                   		 double threshold2,
                                   		 double foreground,
                                   		 double background,
                                   		 bool ascii,
                                   		 const std::string& format,
                                   		 bool verbose )
                                  : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, operation, radius, neighborhood,
             outType, mode,
             threshold1, threshold2, foreground, background, ascii,
             format, verbose );

  }
  GKG_CATCH( "gkg::MorphologicalOperationCommand::MorphologicalOperationCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& operation, float radius, "
             "int32_t neighborhood, "
             "const std::string& outType, const std::string& mode, "
             "double threshold1, double threshold2, double foreground, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::MorphologicalOperationCommand::MorphologicalOperationCommand(
                                             const gkg::Dictionary& parameters )
                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, operation );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, neighborhood );
    DECLARE_STRING_PARAMETER( parameters, std::string, outType );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, foreground );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, operation, radius, neighborhood,
             outType, mode,
             threshold1, threshold2, foreground, background, ascii,
             format, verbose );

  }
  GKG_CATCH( "gkg::MorphologicalOperationCommand::"
             "MorphologicalOperationCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MorphologicalOperationCommand::~MorphologicalOperationCommand()
{
}


std::string gkg::MorphologicalOperationCommand::getStaticName()
{

  try
  {

    return "MorphologicalOperation";

  }
  GKG_CATCH( "std::string gkg::MorphologicalOperationCommand::getStaticName()" );

}


void gkg::MorphologicalOperationCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string operation = "";
    float radius = 1.0;
    int32_t neighborhood = 18;
    std::string outType = "";
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    double foreground = 1.0;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Morphological operation",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "Output type uint8_t/int16_t/int32_t "
                                 "(default=int16_t)",
                                 outType,
                                 true );
    application.addSingleOption( "-op",
                                 "Morphological operation\n"
                                 " - erosion\n"
                                 " - dilation\n"
                                 " - closing\n"
                                 " - opening\n"
                                 " - skeletonization\n"
                                 " - watershed\n",
                                 operation,
                                 true );
    application.addSingleOption( "-r",
                                 "Radius of structuring element",
                                 radius,
                                 true );
    application.addSingleOption( "-n",
                                 "Neighborhood used in watershed among 6, 18, "
                                 "or 26 (default=18)",
                                 neighborhood,
                                 true );
    application.addSingleOption( "-m",
                                 "Threshold mode\n"
                                 " - lt -> lower than\n"
                                 " - le -> lower or equal to\n"
                                 " - eq -> equal to\n"
                                 " - di -> different from\n"
                                 " - ge -> greater or equal to\n"
                                 " - gt -> greater than\n"
                                 " - be -> between or equal\n"
                                 " - bt -> between\n"
                                 " - oe -> outside or equal\n"
                                 " - ou -> outside\n",
                                 mode );
    application.addSingleOption( "-t1",
                                 "First threshold",
                                 threshold1 );
    application.addSingleOption( "-t2",
                                 "Second threshold (required for be/bt/oe/ou)",
                                 threshold2,
                                 true );
    application.addSingleOption( "-f",
                                 "Foreground value (default=1)",
                                 foreground,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0)",
                                 background,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, operation, radius, neighborhood,
             outType, mode,
             threshold1, threshold2, foreground, background, ascii,
             format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MorphologicalOperationCommand::parse()" );

}


void gkg::MorphologicalOperationCommand::execute(
                  	           		 const std::string& fileNameIn,
                                   		 const std::string& fileNameOut,
                                   		 const std::string& operation,
                                   		 float radius,
                                   		 int32_t neighborhood,
                                   		 const std::string& outType,
                                   		 const std::string& mode,
                                   		 double threshold1,
                                   		 double threshold2,
                                   		 double foreground,
                                   		 double background,
                                   		 bool ascii,
                                   		 const std::string& format,
                                   		 bool verbose )
{

  try
  {

    if ( ( mode != "lt" ) &&
         ( mode != "le" ) &&
         ( mode != "eq" ) &&
         ( mode != "di" ) &&
         ( mode != "ge" ) &&
         ( mode != "gt" ) &&
         ( mode != "be" ) &&
         ( mode != "bt" ) &&
         ( mode != "oe" ) &&
         ( mode != "ou" ) )
    {

      throw std::runtime_error( "invalid threshold mode" );

    }
         
    if ( ( operation != "erosion" ) &&
         ( operation != "dilation" ) &&
         ( operation != "closing" ) &&
         ( operation != "opening" ) &&
         ( operation != "skeletonization" ) &&
         ( operation != "watershed" ) )
    {

      throw std::runtime_error( "invalid morphological operation" );

    }
         
    if ( !outType.empty() &&
         ( ! gkg::TypeOf< uint8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int32_t >::isSameType( outType ) ) )
    {

      throw std::runtime_error( "invalid output type" );

    }

    if ( radius <= 0.0 )
    {

      throw std::runtime_error( "radius of morphological operator should be "
                                "strictly positive" );

    }

    if ( ( neighborhood != 6 ) &&
         ( neighborhood != 18 ) &&
         ( neighborhood != 26 ) )
    {

      throw std::runtime_error( "neighborhood for watershed must be set to "
                                "6, 18 or 26 only" );

    }

    //
    // launching process
    //
    MorphologyProcess morphologyProcess( operation,
                                         radius,
                                         neighborhood,
                                         fileNameOut,
                                         outType,
                                         mode,
                                         threshold1,
                                         threshold2,
                                         foreground,
                                         background,
                                         ascii,
                                         format,
                                         verbose );
    morphologyProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::MorphologicalOperationCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& operation, float radius, "
             "int32_t neighborhood, "
             "const std::string& outType, const std::string& mode, "
             "double threshold1, double threshold2, double foreground, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( MorphologicalOperationCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( operation ) +
    			DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    			DECLARE_INTEGER_PARAMETER_HELP( neighborhood ) +
    			DECLARE_STRING_PARAMETER_HELP( outType ) +
    			DECLARE_STRING_PARAMETER_HELP( mode ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( foreground ) +
    			DECLARE_FLOATING_PARAMETER_HELP( background ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
