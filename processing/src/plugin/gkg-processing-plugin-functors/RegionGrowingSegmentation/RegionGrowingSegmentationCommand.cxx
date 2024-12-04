#include <gkg-processing-plugin-functors/RegionGrowingSegmentation/RegionGrowingSegmentationCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::RegionGrowingSegmentationCommand::RegionGrowingSegmentationCommand(
                                         int32_t argc,
                                         char* argv[],
                                         bool loadPlugin,
                                         bool removeFirst )
                                      : gkg::Command( argc,
                                                      argv, 
                                                      loadPlugin,
                                                      removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::RegionGrowingSegmentationCommand::"
             "RegionGrowingSegmentationCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RegionGrowingSegmentationCommand::RegionGrowingSegmentationCommand(
                                const std::string& fileNameVolumeIn,
                                const std::string& fileNameVolumeSeeds,
                                const std::string& fileNameVolumeOut, 
                                int32_t neighborhoodType,
                                float standardDeviation,
                                float isingFactor,
                                int32_t minimumModificationCount,
                                int32_t maximumFrontProgapagationIterationCount,
                                bool isFrontBasedMeanIntensity,
                                bool ascii,
                                const std::string& format,
                                bool verbose )
                                      : gkg::Command()
{

  try
  {

    execute( fileNameVolumeIn,
             fileNameVolumeSeeds,
             fileNameVolumeOut,
             neighborhoodType,
             standardDeviation,
             isingFactor,
             minimumModificationCount,
             maximumFrontProgapagationIterationCount,
             isFrontBasedMeanIntensity,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::RegionGrowingSegmentationCommand::"
             "RegionGrowingSegmentationCommand( "
             "const std::string& fileNameVolumeIn, "
             "const std::string& fileNameVolumeSeeds, "
             "const std::string& fileNameVolumeOut, "
             "int32_t neighborhoodType, "
             "float standardDeviation, "
             "float isingFactor, "
             "int32_t minimumModificationCount, "
             "int32_t maximumFrontProgapagationIterationCount, "
             "bool isFrontBasedMeanIntensity, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::RegionGrowingSegmentationCommand::RegionGrowingSegmentationCommand(
                                     const gkg::Dictionary& parameters )
                                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeSeeds );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, neighborhoodType );
    DECLARE_FLOATING_PARAMETER( parameters, float, standardDeviation );
    DECLARE_FLOATING_PARAMETER( parameters, float, isingFactor );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, 
                               minimumModificationCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, 
                               maximumFrontProgapagationIterationCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, isFrontBasedMeanIntensity );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameVolumeIn,
             fileNameVolumeSeeds,
             fileNameVolumeOut,
             neighborhoodType,
             standardDeviation,
             isingFactor,
             minimumModificationCount,
             maximumFrontProgapagationIterationCount,
             isFrontBasedMeanIntensity,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::RegionGrowingSegmentationCommand::"
             "RegionGrowingSegmentationCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RegionGrowingSegmentationCommand::~RegionGrowingSegmentationCommand()
{
}


std::string gkg::RegionGrowingSegmentationCommand::getStaticName()
{

  try
  {

    return "RegionGrowingSegmentation";

  }
  GKG_CATCH( "std::string "
             "gkg::RegionGrowingSegmentationCommand::getStaticName()" );

}


void gkg::RegionGrowingSegmentationCommand::parse()
{

  try
  {

    std::string fileNameVolumeIn;
    std::string fileNameVolumeSeeds;
    std::string fileNameVolumeOut;
    int32_t neighborhoodType = 26;
    float standardDeviation = 0.0;
    float isingFactor = 1.0;
    int32_t minimumModificationCount = 2;
    int32_t maximumFrontProgapagationIterationCount = 1000;
    bool isFrontBasedMeanIntensity = false;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Region-growing segmentation tool",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameVolumeIn );
    application.addSingleOption( "-s",
                                 "Input seed(s) volume file name",
                                 fileNameVolumeSeeds );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameVolumeOut );
    application.addSingleOption( "-n",
                                 "Neighborhood type 6 / 18 / 26 (default=26)",
                                 neighborhoodType,
                                 true );
    application.addSingleOption( "-sd",
                                 "Standard deviation of intensity",
                                 standardDeviation );
    application.addSingleOption( "-ising",
                                 "Ising regularisation factor",
                                 isingFactor );
    application.addSingleOption( "-M",
                                 "Minimum modification count during front "
                                 "propagation (default=2)",
                                 minimumModificationCount,
                                 true );
    application.addSingleOption( "-I",
                                 "Maximum front propagation iteration count "
                                 "(default=1000)",
                                 maximumFrontProgapagationIterationCount,
                                 true );
    application.addSingleOption( "-isFrontBasedMeanIntensity",
                                 "updating mean intensity with front values "
                                 "(default=false)",
                                 isFrontBasedMeanIntensity,
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

    execute( fileNameVolumeIn,
             fileNameVolumeSeeds,
             fileNameVolumeOut,
             neighborhoodType,
             standardDeviation,
             isingFactor,
             minimumModificationCount,
             maximumFrontProgapagationIterationCount,
             isFrontBasedMeanIntensity,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::RegionGrowingSegmentationCommand::parse()" );

}


void gkg::RegionGrowingSegmentationCommand::execute( 
                                const std::string& fileNameVolumeIn,
                                const std::string& fileNameVolumeSeeds,
                                const std::string& fileNameVolumeOut, 
                                int32_t neighborhoodType,
                                float standardDeviation,
                                float isingFactor,
                                int32_t minimumModificationCount,
                                int32_t maximumFrontProgapagationIterationCount,
                                bool isFrontBasedMeanIntensity,
                                bool ascii,
                                const std::string& format,
                                bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( minimumModificationCount <= 0 )
    {

      throw std::runtime_error(
                          "minimum modification count must be greater than 0" );

    }

    if ( maximumFrontProgapagationIterationCount <= 0 )
    {

      throw std::runtime_error(
           "maximum front propagation iteration count must be greater than 0" );

    }

    if ( ( neighborhoodType != 6 ) &&
         ( neighborhoodType != 18 ) &&
         ( neighborhoodType != 26 ) )
    {

      throw std::runtime_error( "bad neighborhood type" );

    }

    if ( standardDeviation <= 0 )
    {

      throw std::runtime_error(
                        "intensity standard deviation must be greater than 0" );

    }
         
    if ( isingFactor < 0 )
    {

      throw std::runtime_error(
                  "Ising regularization factor must be greater or equal to 0" );

    }

 
    ////////////////////////////////////////////////////////////////////////////
    // reading the input volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading input volume '" << fileNameVolumeIn << "' : "
                << std::flush;

    }

    gkg::Volume< float > volumeIn;
    gkg::TypedVolumeReaderProcess< float > 
     typedVolumeReaderProcessIn( volumeIn );
    typedVolumeReaderProcessIn.execute( fileNameVolumeIn );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > inputSize( 1, 1, 1);
    int32_t inputSizeT = 1;
    volumeIn.getSize( inputSize, inputSizeT );
    if ( inputSizeT != 1 )
    {

      throw std::runtime_error( "input volume must be 3D only" );

    }

    gkg::Vector3d< double > inputResolution( 1, 1, 1);
    double inputResolutionT = 1.0;
    volumeIn.getResolution( inputResolution, inputResolutionT );

    if ( verbose )
    {

      std::cout << "input volume size / resolution : "
                << inputSize << " / "
                << inputResolution << " mm3"
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading the seeds volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading seeds volume '" << fileNameVolumeSeeds << "' : "
                << std::flush;

    }

    gkg::Volume< int16_t > volumeSeeds;
    gkg::TypedVolumeReaderProcess< int16_t > 
     typedVolumeReaderProcessSeeds( volumeSeeds );
    typedVolumeReaderProcessSeeds.execute( fileNameVolumeSeeds );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( !volumeSeeds.hasSameSize( volumeIn, false ) ||
         !volumeSeeds.hasSameResolution( volumeIn, false ) )
    {

      throw std::runtime_error( 
                  "input and seeds volume have incoherent size or resolution" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating the output volume : "
                << std::flush;

    }

    gkg::Volume< int16_t > volumeOut( inputSize );
    volumeOut.setResolution( inputResolution, inputResolutionT );
    volumeOut.fill( 0 );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // extracting all seeds from seed volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "extracting all seeds from input seed volume : "
                << std::flush;

    }
    gkg::Vector3d< int32_t > seed;
    std::map< int16_t, std::list< gkg::Vector3d< int32_t > > > labelToSeeds;
    int16_t label = 0;
    for ( seed.z = 0; seed.z < inputSize.z; seed.z++ )
    {

      for ( seed.y = 0; seed.y < inputSize.y; seed.y++ )
      {

        for ( seed.x = 0; seed.x < inputSize.x; seed.x++ )
        {

          label = volumeSeeds( seed );
          if ( label != 0 )
          {

            labelToSeeds[ label ].push_back( seed );
            volumeOut( seed ) = label;

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing neighbor offsets and bounding box and computing Ising 
    // parameters from Ising regularization factor and neighbor count
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing neighbor offsets and bounding box : "
                << std::flush;

    }
    
    std::list< Vector3d< int32_t > > neighborOffsets;
    float isingInFactor = ( ( float )neighborhoodType - 2.0 ) * isingFactor +
                          0.1;
    float isingOutFactor = ( float )neighborhoodType * isingFactor;
    if ( neighborhoodType == 6 )
    {

      neighborOffsets = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                        gkg::Neighborhood3d::Neighborhood3d_6 );

    }
    else if ( neighborhoodType == 18 )
    {

      neighborOffsets = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                       gkg::Neighborhood3d::Neighborhood3d_18 );

    }
    else if ( neighborhoodType == 26 )
    {

      neighborOffsets = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                       gkg::Neighborhood3d::Neighborhood3d_26 );

    }
    gkg::BoundingBox< int32_t > boundingBox( volumeIn );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    

    ////////////////////////////////////////////////////////////////////////////
    // looping over labels
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "looping over labels : "
                << std::flush;

    }

    std::map< int16_t, std::list< gkg::Vector3d< int32_t > > >::const_iterator
      l = labelToSeeds.begin(),
      le = labelToSeeds.end();
    while ( l != le )
    {

      label = l->first;
      if ( verbose )
      {

        std::cout << label << std::flush;

      }

      this->doRegionGrowingSegmentation(
                                        volumeIn,
                                        l->second,
                                        label,
                                        neighborOffsets,
                                        boundingBox,
                                        standardDeviation,
                                        isingFactor,
                                        isingInFactor,
                                        isingOutFactor,
                                        minimumModificationCount,
                                        maximumFrontProgapagationIterationCount,
                                        isFrontBasedMeanIntensity,
                                        volumeOut,
                                        verbose );

      if ( verbose )
      {

        std::cout << std::endl;

      }

      ++ l;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing the output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing segmentation output volume '"
                << fileNameVolumeOut << "' : "
                << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameVolumeOut,
                                      volumeOut,
                                      ascii,
                                      format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::RegionGrowingSegmentationCommand::execute( "
             "const std::string& fileNameVolumeIn, "
             "const std::string& fileNameVolumeSeeds, "
             "const std::string& fileNameVolumeOut, "
             "int32_t neighborhoodType, "
             "float standardDeviation, "
             "float isingFactor, "
             "int32_t minimumModificationCount, "
             "int32_t maximumFrontProgapagationIterationCount, "
             "bool isFrontBasedMeanIntensity, "
             "bool ascii, const std::string& format, bool verbose )" );

}


void gkg::RegionGrowingSegmentationCommand::doRegionGrowingSegmentation(
                   const gkg::Volume< float >& volumeIn,
                   const std::list< gkg::Vector3d< int32_t > >& seeds,
                   const int16_t& label,
                   const std::list< gkg::Vector3d< int32_t > >& neighborOffsets,
                   const gkg::BoundingBox< int32_t >& boundingBox,
                   float standardDeviation,
                   float isingFactor,
                   float isingInFactor,
                   float isingOutFactor,
                   int32_t minimumModificationCount,
                   int32_t maximumFrontProgapagationIterationCount,
                   bool isFrontBasedMeanIntensity,
                   gkg::Volume< int16_t >& volumeOut,
                   bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // allocating the current and next front(s)
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::Vector3d< int32_t > > currentFront;
    std::list< gkg::Vector3d< int32_t > > nextFront;


    ////////////////////////////////////////////////////////////////////////////
    // initializing the current front from the starting seed
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = seeds.begin(),
      se = seeds.end();
    while ( s != se )
    {

      currentFront.push_back( *s );
      ++ s;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the variance of the signal intensity
    ////////////////////////////////////////////////////////////////////////////

    float intensityVariance = standardDeviation * standardDeviation;


    ////////////////////////////////////////////////////////////////////////////
    // computing the mean and stddeve of intensity within the starting front
    ////////////////////////////////////////////////////////////////////////////

    float meanIntensity = 0.0f;

    gkg::Vector3d< int32_t > neighbor;
    int32_t measurementCount = 0;

    s = seeds.begin();
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      o = neighborOffsets.begin(),
      oe = neighborOffsets.end();
    while ( s != se )
    {

      meanIntensity += volumeIn( *s );
      ++ measurementCount;

      o = neighborOffsets.begin();
      while ( o != oe )
      {

        neighbor = *s + *o;
        if ( boundingBox.contains( neighbor ) )
        {

          meanIntensity += volumeIn( neighbor );
          ++ measurementCount;

        }
        ++ o;

      }

      ++ s;

    }
    meanIntensity /= ( float )measurementCount;

    float differenceIntensity = 0.0f;
    float varianceIntensity = 0.0f;
    s = seeds.begin();
    while ( s != se )
    {

      differenceIntensity = ( volumeIn( *s ) - meanIntensity );
      varianceIntensity += differenceIntensity * differenceIntensity;
      ++ measurementCount;

      o = neighborOffsets.begin();
      while ( o != oe )
      {

        neighbor = *s + *o;
        if ( boundingBox.contains( neighbor ) )
        {

          differenceIntensity = ( volumeIn( neighbor ) - meanIntensity );
          varianceIntensity += differenceIntensity * differenceIntensity;
          ++ measurementCount;

        }
        ++ o;

      }

      ++ s;

    }

    varianceIntensity /= ( float )measurementCount;
    float stdDevIntensity = std::sqrt( varianceIntensity );

    if ( verbose )
    {

      std::cout << "(" << meanIntensity << "+-" << stdDevIntensity << ") "
                << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // propagating the front to perform the region growing
    ////////////////////////////////////////////////////////////////////////////

    int32_t iterationCount = 0;
    int32_t modificationCount = 0;
    bool isInObject = true;
    float deltaEnergy = 0.0f;
    float neighborIntensity = 0.0f;
    gkg::Vector3d< int32_t > neighborPrime;
    while ( ( iterationCount < maximumFrontProgapagationIterationCount ) && 
            ( ( iterationCount == 0 ) ||
              ( modificationCount > minimumModificationCount ) ) )
    {

      // std::cout << iterationCount << std::endl;

      modificationCount = 0;

      //////////////////////////////////////////////////////////////////////////
      // looping over points in the current front
      //////////////////////////////////////////////////////////////////////////

      std::list< gkg::Vector3d< int32_t > >::const_iterator
        p = currentFront.begin(),
        pe = currentFront.end();
      while ( p != pe )
      {

        isInObject = true;

        ////
        if ( isFrontBasedMeanIntensity )
        {


          meanIntensity = volumeIn( *p );
          measurementCount = 1;
          
          o = neighborOffsets.begin();
          while ( o != oe )
          {

            neighbor = *p + *o;
            if ( boundingBox.contains( neighbor ) )
            {

              meanIntensity += volumeIn( neighbor );
              ++ measurementCount;

            }
            ++ o;

          }
          meanIntensity /= ( float )measurementCount;

        }

        o = neighborOffsets.begin();
        while ( o != oe )
        {

          neighbor = *p + *o;
          deltaEnergy = 0.0f;

          if ( boundingBox.contains( neighbor ) )
          {

            neighborIntensity = volumeIn( neighbor );

            if ( volumeOut( neighbor ) == 0 )
            {

              deltaEnergy = isingInFactor * 
                            std::min( ( neighborIntensity - meanIntensity ) *
                                      ( neighborIntensity - meanIntensity ) /
                                      intensityVariance, 4.0f ) -
                            isingOutFactor;

              std::list< gkg::Vector3d< int32_t > >::const_iterator
                oPrime = neighborOffsets.begin();
              while ( oPrime != oe )
              {

                neighborPrime = neighbor + *oPrime;

                if ( boundingBox.contains( neighborPrime ) )
                {

                  if ( volumeOut( neighborPrime ) != label )
                  {

                    deltaEnergy += isingFactor;

                  }
                  else
                  {

                    deltaEnergy -= isingFactor;

                  }

                }

                ++ oPrime;

              }

              if ( deltaEnergy < 0.0f )
              {

                nextFront.push_back( neighbor );
                volumeOut( neighbor ) = label;
                ++ modificationCount;

              }
              else
              {

                isInObject = false;

              }


            }

          }

          ++ o;

        }

        if ( !isInObject )
        {

          nextFront.push_back( *p );

        }

        ++ p;

      }

      currentFront = nextFront;
      nextFront.clear();

      ++ iterationCount;

    }

  }
  GKG_CATCH( "void gkg::RegionGrowingSegmentationCommand::"
             "doRegionGrowingSegmentation( "
             "const gkg::Volume< float >& volumeIn, "
             "const std::list< gkg::Vector3d< int32_t > >& seeds, "
             "const std::list< gkg::Vector3d< int32_t > >& neighbors, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "float standardDeviation, "
             "float regularizationFactor, "
             "int32_t maximumFrontProgapagationIterationCount, "
             "gkg::Volume< int16_t >& volumeOut ) const" );

}



RegisterCommandCreator( RegionGrowingSegmentationCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolumeIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolumeSeeds ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolumeOut ) +
                        DECLARE_INTEGER_PARAMETER_HELP( neighborhoodType ) +
                        DECLARE_FLOATING_PARAMETER_HELP( standardDeviation ) +
                        DECLARE_FLOATING_PARAMETER_HELP(isingFactor ) +
                        DECLARE_INTEGER_PARAMETER_HELP(
                                                    minimumModificationCount ) +
                        DECLARE_INTEGER_PARAMETER_HELP(
                                     maximumFrontProgapagationIterationCount ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( 
                                                   isFrontBasedMeanIntensity ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
