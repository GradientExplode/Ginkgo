#include <gkg-processing-plugin-functors/VoiStatistics/VoiStatisticsCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-algobase/UpdatableWeightedMeanStandardDeviationMinMax.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <fstream>


//
// class VoiStatisticsProcess
//


class VoiStatisticsProcess : public gkg::Process
{

  public:

    VoiStatisticsProcess(
           const std::string& theFileNameLabelVolume,
           const std::string& theFileNameDensityMask,
           const std::string& theFileNameLabelOrDensityMaskToMeasureTransform3d,
           const std::string& theFileNameStatistics,
           const int32_t& theRank,
           const double& theBackground,
           const bool& theVerbose );

    const std::string& fileNameLabelVolume;
    const std::string& fileNameDensityMask;
    const std::string& fileNameLabelOrDensityMaskToMeasureTransform3d;
    const std::string& fileNameStatistics;
    const int32_t& rank;
    const double& background;
    const bool& verbose;

  private:

    template < class T >
    static void analyze( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


VoiStatisticsProcess::VoiStatisticsProcess(
           const std::string& theFileNameLabelVolume,
           const std::string& theFileNameDensityMask,
           const std::string& theFileNameLabelOrDensityMaskToMeasureTransform3d,
           const std::string& theFileNameStatistics,
           const int32_t& theRank,
           const double& theBackground,
           const bool& theVerbose )
                : gkg::Process( "Volume" ),
                  fileNameLabelVolume( theFileNameLabelVolume ),
                  fileNameDensityMask( theFileNameDensityMask ),
                  fileNameLabelOrDensityMaskToMeasureTransform3d(
                            theFileNameLabelOrDensityMaskToMeasureTransform3d ),
                  fileNameStatistics( theFileNameStatistics ),
                  rank( theRank ),
                  background( theBackground ),
                  verbose( theVerbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &VoiStatisticsProcess::analyze< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &VoiStatisticsProcess::analyze< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &VoiStatisticsProcess::analyze< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &VoiStatisticsProcess::analyze< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &VoiStatisticsProcess::analyze< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &VoiStatisticsProcess::analyze< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &VoiStatisticsProcess::analyze< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &VoiStatisticsProcess::analyze< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &VoiStatisticsProcess::analyze< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &VoiStatisticsProcess::analyze< double > );

}



template < class T >
void VoiStatisticsProcess::analyze( gkg::Process& process,
                                    const std::string& fileNameMeasureVolume,
                                    const gkg::AnalyzedObject&,
                                    const std::string& )
{

  try
  {

    VoiStatisticsProcess&
      voiStatisticsProcess = static_cast< VoiStatisticsProcess& >( process );

    const std::string& fileNameLabelVolume =
                                       voiStatisticsProcess.fileNameLabelVolume;
    const std::string& fileNameDensityMask =
                                       voiStatisticsProcess.fileNameDensityMask;
    const std::string& fileNameLabelOrDensityMaskToMeasureTransform3d =
            voiStatisticsProcess.fileNameLabelOrDensityMaskToMeasureTransform3d;
    const std::string& fileNameStatistics = 
                                        voiStatisticsProcess.fileNameStatistics;
    const int32_t& rank = voiStatisticsProcess.rank;
    const double& background = voiStatisticsProcess.background;
    const bool& verbose = voiStatisticsProcess.verbose;


    // sanity checks
    if ( !fileNameLabelVolume.empty() && !fileNameDensityMask.empty() )
    {

      throw std::runtime_error(
                 "cannot use label volume and density mask at the same time!" );

    }
         

    // reading input measure volume 
    if ( verbose )
    {

      std::cout << "reading '" << fileNameMeasureVolume << "' : " << std::flush;

    }

    gkg::Volume< T > measureVolume;
    gkg::Reader::getInstance().read( fileNameMeasureVolume, measureVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    // sanity check
    if ( ( rank < 0 ) || ( rank >= measureVolume.getSizeT() ) )
    {

      throw std::runtime_error( "rank out of possible range" );

    }

    // reading label volume 
    if ( verbose )
    {

      if ( !fileNameLabelVolume.empty() )
      {

        std::cout << "reading '" << fileNameLabelVolume << "' : " << std::flush;

      }
      else
      {

        std::cout << "reading '" << fileNameDensityMask << "' : " << std::flush;

      }

    }

    gkg::RCPointer< gkg::Volume< int16_t > > labelVolumeInt16;
    gkg::RCPointer< gkg::Volume< uint8_t > > labelVolumeUint8;
    gkg::RCPointer< gkg::Volume< float > > densityMask;

    if ( !fileNameLabelVolume.empty() )
    {

      try
      {

        labelVolumeInt16.reset( new gkg::Volume< int16_t > );
        gkg::Reader::getInstance().read( fileNameLabelVolume,
                                         *labelVolumeInt16 );

      }
      catch( std::exception& )
      {

        try
        {

          labelVolumeUint8.reset( new gkg::Volume< uint8_t > );
          gkg::Reader::getInstance().read( fileNameLabelVolume,
                                           *labelVolumeUint8 );


        }
        catch( std::exception& )
        {

          throw std::runtime_error( "unable to read label volume" );

        }

      }

    }
    else
    {

      densityMask.reset( new gkg::Volume< float > );
      gkg::Reader::getInstance().read( fileNameDensityMask, *densityMask );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    // reading 3D transform
    gkg::RCPointer< gkg::Transform3d< float > > roiToMeasureTransform3d;

    if ( fileNameLabelOrDensityMaskToMeasureTransform3d.empty() )
    {

      if ( verbose )
      {

        std::cout << "setting 3D transform to identity : " << std::flush;

      }
      roiToMeasureTransform3d.reset( new gkg::IdentityTransform3d< float > );

    }
    else
    {

      if ( verbose )
      {

        std::cout << "reading 3D transform '"
                  << fileNameLabelOrDensityMaskToMeasureTransform3d
                  << "' : " << std::flush;

      }
      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream is(
                       fileNameLabelOrDensityMaskToMeasureTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        roiToMeasureTransform3d.reset( rigidTransform3d );

      }
      catch( std::exception& )
      {

        try
        {

          gkg::AffineWoShearingTransform3d< float >*
            affineWoShearingTransform3d =
              new gkg::AffineWoShearingTransform3d< float >;
          std::ifstream is( 
                       fileNameLabelOrDensityMaskToMeasureTransform3d.c_str() );
          affineWoShearingTransform3d->readTrm( is );
          is.close();
          roiToMeasureTransform3d.reset( affineWoShearingTransform3d );

        }
        catch( std::exception& )
        {

          try
          {

            gkg::AffineTransform3d< float >*
              affineTransform3d =
                new gkg::AffineTransform3d< float >;
            std::ifstream is( 
                       fileNameLabelOrDensityMaskToMeasureTransform3d.c_str() );
            affineTransform3d->readTrm( is );
            is.close();
            roiToMeasureTransform3d.reset( affineTransform3d );

          }
          catch( std::exception& )
          {

            throw std::runtime_error( "unable to read 3D transform" );

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // analyzing volumes of interest
    if ( verbose )
    {

      std::cout << "analyzing : " << std::flush;

    }

    std::vector< gkg::UpdatableWeightedMeanStandardDeviationMinMax > analysis;

    if ( !labelVolumeInt16.isNull() )
    {

      analysis.resize( 32767 );
      gkg::Resampler< int16_t >* resampler =
        gkg::ResamplerFactory< int16_t >::getInstance().getResampler( 0 );

      int32_t sizeX = measureVolume.getSizeX();
      int32_t sizeY = measureVolume.getSizeY();
      int32_t sizeZ = measureVolume.getSizeZ();
      gkg::Vector3d< double > resolution;
      measureVolume.getResolution( resolution );

      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;

      int16_t value = 0;

      gkg::Vector3d< float > site;
 
      for ( z = 0; z < sizeZ; z++ )
      {

        site.z = ( float )z * ( float )resolution.z;

        for ( y = 0; y < sizeY; y++ )
        {

          site.y = ( float )y * ( float )resolution.y;
          for ( x = 0; x < sizeX; x++ )
          {

            site.x = ( float )x * ( float )resolution.x;
            resampler->resample( *labelVolumeInt16,
                                 *roiToMeasureTransform3d,
                                 ( int16_t )background,
                                 site,
                                 value );
            if ( value > 0 )
            {

              analysis[ ( int32_t )value ].add(
                                               measureVolume( x, y, z, rank ),
                                               1.0 );

            }

          }

        }

      }

    }
    else if ( !labelVolumeUint8.isNull() )
    {

      analysis.resize( 255 );
      gkg::Resampler< uint8_t >* resampler =
              gkg::ResamplerFactory< uint8_t >::getInstance().getResampler( 0 );

      int32_t sizeX = measureVolume.getSizeX();
      int32_t sizeY = measureVolume.getSizeY();
      int32_t sizeZ = measureVolume.getSizeZ();
      gkg::Vector3d< double > resolution;
      measureVolume.getResolution( resolution );

      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;

      uint8_t value = 0;

      gkg::Vector3d< float > site;
 
      for ( z = 0; z < sizeZ; z++ )
      {

        site.z = ( float )z * ( float )resolution.z;

        for ( y = 0; y < sizeY; y++ )
        {

          site.y = ( float )y * ( float )resolution.y;
          for ( x = 0; x < sizeX; x++ )
          {

            site.x = ( float )x * ( float )resolution.x;
            resampler->resample( *labelVolumeUint8,
                                 *roiToMeasureTransform3d,
                                 ( uint8_t )background,
                                 site,
                                 value );
            if ( value > 0 )
            {

              analysis[ ( int32_t )value ].add( 
                                               measureVolume( x, y, z, rank ),
                                               1.0 );

            }

          }

        }

      }

    }
    else if ( !densityMask.isNull() )
    {

      int32_t roiCount = densityMask->getSizeT();

      analysis.resize( roiCount );
      gkg::Resampler< float >* resampler =
              gkg::ResamplerFactory< float >::getInstance().getResampler( 1 );

      int32_t sizeX = measureVolume.getSizeX();
      int32_t sizeY = measureVolume.getSizeY();
      int32_t sizeZ = measureVolume.getSizeZ();
      gkg::Vector3d< double > resolution;
      measureVolume.getResolution( resolution );

      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;

      float value = 0;
      gkg::Vector3d< float > site;
      int32_t r = 0;

      for ( r = 0; r < roiCount; r++ )
      {
 
        for ( z = 0; z < sizeZ; z++ )
        {

          site.z = ( float )z * ( float )resolution.z;

          for ( y = 0; y < sizeY; y++ )
          {

            site.y = ( float )y * ( float )resolution.y;
            for ( x = 0; x < sizeX; x++ )
            {

              site.x = ( float )x * ( float )resolution.x;
              resampler->resample( *densityMask,
                                   *roiToMeasureTransform3d,
                                   ( float )background,
                                   site,
                                   value );
              if ( value > 0.0 )
              {

                analysis[ r ].add( measureVolume( x, y, z, rank ), value );

              }

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    // writing output 
    if ( verbose )
    {

      std::cout << "writing '"
                << fileNameStatistics
                << "' : "
                << std::flush;

    }

    std::ofstream os( fileNameStatistics.c_str() );

    os << "statistics={" << std::endl;
    
    std::vector<
             gkg::UpdatableWeightedMeanStandardDeviationMinMax >::const_iterator
      u = analysis.begin(),
      ue = analysis.end();
    int32_t label = 0;
    while ( u != ue )
    {

      if ( u->getCount() )
      {

        const gkg::UpdatableWeightedMeanStandardDeviationMinMax& stats = *u;

        os << "  " << label << " : "
           << "{ " << "'mean' : "
                   << stats.getMean() << ", "
                   << "'standard-deviation' : "
                   << stats.getStandardDeviation() << ", "
                   << "'minimum' : "
                   << stats.getMinimum() << ", "
                   << "'maximum' : "
                   << stats.getMaximum() << " }," << std::endl;

      }
      ++ u;
      ++ label;

    }
    os << "}" << std::endl;

    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "template < class T, class M > "
             "void VoiStatisticsProcess::analyze( "
             "gkg::Process& process, "
             "const std::string& fileNameMeasureVolume, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   VoiStatisticsCommand
//


gkg::VoiStatisticsCommand::VoiStatisticsCommand( int32_t argc,
                                 		 char* argv[],
                                 		 bool loadPlugin,
                                 		 bool removeFirst )
                         : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::VoiStatisticsCommand::VoiStatisticsCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VoiStatisticsCommand::VoiStatisticsCommand(
              const std::string& fileNameMeasureVolume,
    	      const std::string& fileNameLabelVolume,
    	      const std::string& fileNameDensityMask,
              const std::string& fileNameLabelOrDensityMaskToMeasureTransform3d,
    	      const std::string& fileNameStatistics,
    	      int32_t rank,
    	      double background,
    	      bool verbose )
                         : gkg::Command()
{

  try
  {

    execute( fileNameMeasureVolume, fileNameLabelVolume, fileNameDensityMask,
             fileNameLabelOrDensityMaskToMeasureTransform3d,
             fileNameStatistics, rank, background, verbose );

  }
  GKG_CATCH(
           "gkg::VoiStatisticsCommand::VoiStatisticsCommand( "
           "const std::string& fileNameMeasureVolume, "
           "const std::string& fileNameLabelVolume, "
           "const std::string& fileNameDensityMask, "
           "const std::string& fileNameLabelOrDensityMaskToMeasureTransform3d, "
    	   "const std::string& fileNameStatistics, "
    	   "int32_t rank, double background, bool verbose )" );

}


gkg::VoiStatisticsCommand::VoiStatisticsCommand(
                                             const gkg::Dictionary& parameters )
                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMeasureVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameLabelVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDensityMask );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameLabelOrDensityMaskToMeasureTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameStatistics );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, rank );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameMeasureVolume, fileNameLabelVolume, fileNameDensityMask,
             fileNameLabelOrDensityMaskToMeasureTransform3d,
             fileNameStatistics, rank, background, verbose );

  }
  GKG_CATCH( "gkg::VoiStatisticsCommand::VoiStatisticsCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VoiStatisticsCommand::~VoiStatisticsCommand()
{
}


std::string gkg::VoiStatisticsCommand::getStaticName()
{

  try
  {

    return "VoiStatistics";

  }
  GKG_CATCH( "std::string gkg::VoiStatisticsCommand::getStaticName()" );

}


void gkg::VoiStatisticsCommand::parse()
{

  try
  {

    std::string fileNameMeasureVolume;
    std::string fileNameLabelVolume;
    std::string fileNameDensityMask;
    std::string fileNameLabelOrDensityMaskToMeasureTransform3d;
    std::string fileNameStatistics;
    int32_t rank = 0;
    double background = 0.0;
    bool verbose = false;

    gkg::Application application(
               _argc, _argv,
               "Statistical analyzer of intensity within volumes of interest\n",
               _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input measure volume file name",
                                 fileNameMeasureVolume );
    application.addSingleOption( "-l",
                                 "Input label int16_t/uint8_t volume file name",
                                 fileNameLabelVolume,
                                 true );
    application.addSingleOption( "-d",
                                 "Input 4D float density mask file name",
                                 fileNameDensityMask,
                                 true );
    application.addSingleOption( "-t",
                                 "Label/Density mask to measure 3D transform" 
                                 " file name"
                                 " (default=id)",
                                 fileNameLabelOrDensityMaskToMeasureTransform3d,
                                 true );
    application.addSingleOption( "-o",
                                 "Output python dictionary file name",
                                 fileNameStatistics );
    application.addSingleOption( "-r",
                                 "Rank to be considered in the input measure "
                                 "volume (default=0)",
                                 rank,
                                 true );
    application.addSingleOption( "-background",
                                 "Assumed label outside of the label volume "
                                 " (default=0)",
                                 background,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameMeasureVolume, fileNameLabelVolume, fileNameDensityMask,
             fileNameLabelOrDensityMaskToMeasureTransform3d,
             fileNameStatistics, rank, background, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::VoiStatisticsCommand::parse()" );

}


void gkg::VoiStatisticsCommand::execute(
                  	     const std::string& fileNameMeasureVolume,
    	      const std::string& fileNameLabelVolume,
    	      const std::string& fileNameDensityMask,
              const std::string& fileNameLabelOrDensityMaskToMeasureTransform3d,
    	      const std::string& fileNameStatistics,
    	      int32_t rank,
    	      double background,
    	      bool verbose )
{

  try
  {

    //
    // launching process
    //
    VoiStatisticsProcess 
      voiStatisticsProcess( fileNameLabelVolume,
                            fileNameDensityMask,
                            fileNameLabelOrDensityMaskToMeasureTransform3d,
                            fileNameStatistics,
                            rank,
                            background,
                            verbose );
    voiStatisticsProcess.execute( fileNameMeasureVolume );

  }
  GKG_CATCH( 
           "gkg::VoiStatisticsCommand::execute( "
           "const std::string& fileNameMeasureVolume, "
           "const std::string& fileNameLabelVolume, "
           "const std::string& fileNameDensityMask, "
           "const std::string& fileNameLabelOrDensityMaskToMeasureTransform3d, "
    	   "const std::string& fileNameStatistics, "
    	   "int32_t rank, double background, bool verbose )" );

}


RegisterCommandCreator(
  VoiStatisticsCommand,
  DECLARE_STRING_PARAMETER_HELP( fileNameMeasureVolume ) +
  DECLARE_STRING_PARAMETER_HELP( fileNameLabelVolume ) +
  DECLARE_STRING_PARAMETER_HELP( fileNameDensityMask ) +
  DECLARE_STRING_PARAMETER_HELP(
                              fileNameLabelOrDensityMaskToMeasureTransform3d ) +
  DECLARE_STRING_PARAMETER_HELP( fileNameStatistics ) +
  DECLARE_INTEGER_PARAMETER_HELP( rank ) +
  DECLARE_FLOATING_PARAMETER_HELP( background ) +
  DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
