#include <gkg-dmri-plugin-functors/DwiTractographyMask/DwiTractographyMaskCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/BetweenFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/UpdatableMeanAndStandardDeviation.h>
#include <gkg-processing-morphology/Erosion.h>
#include <gkg-processing-morphology/ConnectedComponents.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-algobase/Combiner_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <algorithm>


gkg::DwiTractographyMaskCommand::DwiTractographyMaskCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DwiTractographyMaskCommand::DwiTractographyMaskCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiTractographyMaskCommand::DwiTractographyMaskCommand(
                                    const std::string& fileNameT2,
                                    const std::string& fileNameFA,
                                    const std::string& fileNameWhiteMask,
                                    float faThreshold,
                                    const std::vector< float >& t2StdDevFactors,
                                    float erosionRadius,
                                    float dilationRadius,
                                    const std::string& intermediate,
                                    bool verbose )
                                : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameFA,
             fileNameWhiteMask,
             faThreshold,
             t2StdDevFactors,
             erosionRadius,
             dilationRadius,
             intermediate,
             verbose );

  }
  GKG_CATCH( "gkg::DwiTractographyMaskCommand::DwiTractographyMaskCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameFA, "
             "const std::string& fileNameWhiteMask, "
             "float faThreshold, "
             "const std::vector< float >& t2StdDevFactors, "
             "float erosionRadius, "
             "float dilationRadius, "
             "const std::string& intermediate, "
             "bool verbose  )" );

}


gkg::DwiTractographyMaskCommand::DwiTractographyMaskCommand(
                                             const gkg::Dictionary& parameters )
                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameFA );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameWhiteMask );
    DECLARE_FLOATING_PARAMETER( parameters, float, faThreshold );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           t2StdDevFactors );
    DECLARE_FLOATING_PARAMETER( parameters, float, erosionRadius );
    DECLARE_FLOATING_PARAMETER( parameters, float, dilationRadius );
    DECLARE_STRING_PARAMETER( parameters, std::string, intermediate );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameT2,
             fileNameFA,
             fileNameWhiteMask,
             faThreshold,
             t2StdDevFactors,
             erosionRadius,
             dilationRadius,
             intermediate,
             verbose );

  }
  GKG_CATCH( "gkg::DwiTractographyMaskCommand::DwiTractographyMaskCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiTractographyMaskCommand::~DwiTractographyMaskCommand()
{
}


std::string gkg::DwiTractographyMaskCommand::getStaticName()
{

  try
  {

    return "DwiTractographyMask";

  }
  GKG_CATCH( "std::string gkg::DwiTractographyMaskCommand::getStaticName()" );

}


void gkg::DwiTractographyMaskCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameFA;
    std::string fileNameWhiteMask;

    float faThreshold = 0.25;
    std::vector< float > t2StdDevFactors;
    float erosionRadius = -1.0;
    float dilationRadius = -1.0;

    std::string intermediate = "";
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI tractography mask builder",
                                  _loadPlugin );
    application.addSingleOption( "-t2",
                                 "Input bias corrected T2 at b=0s/mm^2 "
                                 "volume file name",
                                 fileNameT2 );
    application.addSingleOption( "-fa",
                                 "Input FA volume file name",
                                 fileNameFA );
    application.addSingleOption( "-o",
                                 "White matter mask file name",
                                 fileNameWhiteMask );

    application.addSingleOption( "-faThreshold",
                                 "Threshold on FA values (default=0.25)",
                                 faThreshold,
                                 true );
    application.addSeriesOption( "-t2StdDevFactors",
                                 "Lower <lf> and upper <uf> standard deviation "
                                 "factors\n"
                                 "Considers that brain mask corresponds to the "
                                 "range [ mean<T2> - <lf> x stddev(T2); "
                                 "mean<T2> + <uf> x stddev(T2)] "
                                 "(default=1.3 3.5)",
                                 t2StdDevFactors,
                                 0, 2 );
    application.addSingleOption( "-re",
                                 "Radius of erosion (default=minimum resolution)",
                                 erosionRadius, 
                                 true );
    application.addSingleOption( "-rd",
                                 "Radius of dilation (default=erosion radius + "
                                 "0.5)",
                                 dilationRadius, 
                                 true );

    application.addSingleOption( "-intermediate",
                                 "Intermediate root file name",
                                 intermediate, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameT2,
             fileNameFA,
             fileNameWhiteMask,
             faThreshold,
             t2StdDevFactors,
             erosionRadius,
             dilationRadius,
             intermediate,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiTractographyMaskCommand::parse()" );

}


void gkg::DwiTractographyMaskCommand::execute(
                                    const std::string& fileNameT2,
                                    const std::string& fileNameFA,
                                    const std::string& fileNameWhiteMask,
                                    float faThreshold,
                                    const std::vector< float >& t2StdDevFactors,
                                    float erosionRadius,
                                    float dilationRadius,
                                    const std::string& intermediate,
                                    bool verbose )
{

  try
  {

    std::vector< float > theT2StdDevFactors = t2StdDevFactors;
    if ( !theT2StdDevFactors.empty() && ( theT2StdDevFactors.size() != 2U ) )
    {

      throw std::runtime_error( "not a valid number of std dev factors" );

    }

    if ( theT2StdDevFactors.empty() )
    {

      theT2StdDevFactors.resize( 2 );
      theT2StdDevFactors[ 0 ] = 1.3;
      theT2StdDevFactors[ 1 ] = 3.5;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading T2 at b=0s/mm^2 volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameT2 << "' : " << std::flush;

    }

    gkg::Volume< float > t2;
    gkg::TypedVolumeReaderProcess< float > t2VolumeReaderProcess( t2 );
    t2VolumeReaderProcess.execute( fileNameT2 );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< double > resolution;
    t2.getResolution( resolution );
    float minimumResolution = ( float )std::min( std::min( resolution.x,
                                                           resolution.y ),
                                                 resolution.z );

    if ( erosionRadius < 0 )
    {

      erosionRadius = minimumResolution;

    }
    if ( dilationRadius < 0 )
    {

      dilationRadius = minimumResolution + 0.5f;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading FA volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameFA << "' : " << std::flush;

    }

    gkg::Volume< float > fa;
    gkg::Reader::getInstance().read( fileNameFA, fa );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////


    if ( !t2.hasSameSize( fa ) || !t2.hasSameResolution( fa ) )
    {

       throw std::runtime_error( "incompatible T2 and FA size or resolution" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // thresholding FA to get a first binary mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "binarizing FA volume for FA >= " << faThreshold << " : "
               << std::flush;

    }

    gkg::GreaterOrEqualToFunction< float >
      faGreaterOrEqualFunction( faThreshold );
    gkg::Binarizer< gkg::Volume< float >, gkg::Volume< uint8_t > >
      faBinarizer( faGreaterOrEqualFunction, 1U, 0U );

    gkg::Volume< uint8_t > faMask;
    faBinarizer.binarize( fa, faMask );

    if ( !intermediate.empty() )
    {

      gkg::Writer::getInstance().write( intermediate + "_fa_mask_1", faMask );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing mean and stddev of T2 signal over the FA mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "processing mean and stddev of T2 signal over the FA mask : "
               << std::flush;

    }

    gkg::UpdatableMeanAndStandardDeviation updatableMeanAndStandardDeviation1;

    gkg::Volume< float >::const_iterator
      v = t2.begin(),
      ve = t2.end();
    gkg::Volume< uint8_t >::const_iterator
      m = faMask.begin();
    while ( v != ve )
    {


      if ( *m )
      {

        updatableMeanAndStandardDeviation1.add( ( double )*v );

      }
      ++ v;
      ++ m;

    }

    double mean = updatableMeanAndStandardDeviation1.getMean();
    double stddev = updatableMeanAndStandardDeviation1.getStandardDeviation();
    if ( verbose )
    {

      std::cout << "mean=" << mean << " stddev=" << stddev << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing brain mask from T2
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "processing brain mask from T2 : "
               << std::flush;

    }

    gkg::BetweenFunction< float >
      brainMaskBetweenFunction(
        std::max( ( float )( mean - theT2StdDevFactors[ 0 ] * stddev ),
                             ( float )0 ),
        std::min( ( float )( mean + theT2StdDevFactors[ 1 ] * stddev ),
                             ( float )1e38 ) );
    gkg::Binarizer< gkg::Volume< float >, gkg::Volume< uint8_t > >
      brainMaskBinarizer( brainMaskBetweenFunction, 1U, 0U );

    gkg::Volume< uint8_t > brainMask;
    brainMaskBinarizer.binarize( t2, brainMask );

    if ( !intermediate.empty() )
    {

      gkg::Writer::getInstance().write( intermediate + "_brain_mask_1",
                                        brainMask );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // eroding brain mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "eroding brain mask (" << erosionRadius << "mm) : "
               << std::flush;

    }

    gkg::EqualToFunction< uint8_t > equalToOneFunction( 1U );
    gkg::Erosion< uint8_t, uint8_t >
      erosion( erosionRadius,
               equalToOneFunction,
               1U,
               0U );
    gkg::Volume< uint8_t > erodedBrainMask;
    erosion.erode( brainMask, erodedBrainMask, verbose );

    if ( !intermediate.empty() )
    {

      gkg::Writer::getInstance().write( intermediate + "_eroded_brain_mask_1",
                                        erodedBrainMask );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // selecting biggest connected component
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "selecting biggest erored connected component : "
               << std::flush;

    }

    gkg::ConnectedComponents< uint8_t, uint8_t >
      connectedComponents( equalToOneFunction,
                           0U,
                           1U,
                           1U,
                           true,
                           gkg::Neighborhood3d::Neighborhood3d_6 );
    gkg::Volume< uint8_t > biggestConnectedComponent;
    connectedComponents.getComponents( erodedBrainMask,
                                       biggestConnectedComponent,
                                       0,
                                       verbose );

    if ( !intermediate.empty() )
    {

      gkg::Writer::getInstance().write(
                                  intermediate + "_biggest_connected_component",
                                  biggestConnectedComponent );

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // dilating biggest connected component
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "dilating biggest connected component "
               << dilationRadius << "mm) : "
               << std::flush;

    }

    gkg::Dilation< uint8_t, uint8_t >
      dilation( dilationRadius,
                equalToOneFunction,
                1U,
                0U );
    gkg::Volume< uint8_t > dilatedBiggestConnectedComponent;
    dilation.dilate( biggestConnectedComponent,
                     dilatedBiggestConnectedComponent,
                     verbose );

    if ( !intermediate.empty() )
    {

      gkg::Writer::getInstance().write( intermediate +
                                        "_dilated_biggest_connected_component",
                                        dilatedBiggestConnectedComponent );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // masking FA mask using brain mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "masking FA mask using brain mask : "
               << std::flush;

    }

    std::vector< std::string > functors1( 1, "id" );
    std::vector< std::string > functors2( 1, "id" );
    std::vector< double > numerator1s( 2 );
    numerator1s[ 0 ] = 1;
    numerator1s[ 1 ] = 0;
    std::vector< double > denominator1s( 2 );
    denominator1s[ 0 ] = 1;
    denominator1s[ 1 ] = 1;
    std::vector< double > numerator2s( 1 );
    numerator2s[ 0 ] = 1;
    std::vector< double > denominator2s( 1 );
    denominator2s[ 0 ] = 1;
    std::vector< std::string > operators( 1, "+" );


    gkg::Combiner< gkg::Volume< uint8_t >,
                   gkg::Volume< uint8_t >,
                   gkg::Volume< uint8_t > >
      faBrainMaskCombiner( functors1,
                           functors2,
                           numerator1s,
                           denominator1s,
                           numerator2s,
                           denominator2s,
                           operators,
                           &equalToOneFunction,
                           0U );
                   
    std::vector< gkg::Volume< uint8_t > > inputVolumes( 1 );
    inputVolumes[ 0 ] = faMask;

    faBrainMaskCombiner.combine( inputVolumes,
                                 dilatedBiggestConnectedComponent,
                                 faMask,
                                 verbose );

    if ( !intermediate.empty() )
    {

      gkg::Writer::getInstance().write( intermediate +
                                        "_fa_mask_2",
                                        faMask );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing mean and stddev of T2 signal over the FA mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "processing mean and stddev of T2 signal over "
                  "the new FA mask : "
               << std::flush;

    }

    gkg::UpdatableMeanAndStandardDeviation updatableMeanAndStandardDeviation2;

    v = t2.begin();
    ve = t2.end();
    m = faMask.begin();
    while ( v != ve )
    {


      if ( *m )
      {

        updatableMeanAndStandardDeviation2.add( ( double )*v );

      }
      ++ v;
      ++ m;

    }

    mean = updatableMeanAndStandardDeviation2.getMean();
    stddev = updatableMeanAndStandardDeviation2.getStandardDeviation();
    if ( verbose )
    {

      std::cout << "mean=" << mean << " stddev=" << stddev << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing white mask from T2
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "processing white mask from T2 : "
               << std::flush;

    }

    gkg::BetweenFunction< float >
      whiteMaskBetweenFunction(
        std::max( ( float )( mean - t2StdDevFactors[ 0 ] * stddev ),
                             ( float )0 ),
        std::min( ( float )( mean + t2StdDevFactors[ 1 ] * stddev ),
                             ( float )1e38 ) );
    gkg::Binarizer< gkg::Volume< float >, gkg::Volume< uint8_t > >
      whiteMaskBinarizer( whiteMaskBetweenFunction, 1U, 0U );

    gkg::Volume< uint8_t > whiteMask;
    whiteMaskBinarizer.binarize( t2, whiteMask );

    if ( !intermediate.empty() )
    {

      gkg::Writer::getInstance().write( intermediate + "_white_mask",
                                        whiteMask );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // masking white mask using brain mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "masking white mask using brain mask : "
               << std::flush;

    }

    gkg::Combiner< gkg::Volume< uint8_t >,
                   gkg::Volume< uint8_t >,
                   gkg::Volume< uint8_t > >
      whiteBrainMaskCombiner( functors1,
                              functors2,
                              numerator1s,
                              denominator1s,
                              numerator2s,
                              denominator2s,
                              operators,
                              &equalToOneFunction,
                              0U );
                   
    inputVolumes[ 0 ] = whiteMask;

    faBrainMaskCombiner.combine( inputVolumes,
                                 dilatedBiggestConnectedComponent,
                                 whiteMask,
                                 verbose );

    if ( !intermediate.empty() )
    {

      gkg::Writer::getInstance().write( intermediate +
                                        "_white_mask_masked_using_brain_mask",
                                        whiteMask );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // selecting biggest connected component of the white mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "selecting biggest connected component of the white mask : "
               << std::flush;

    }


    gkg::Volume< uint8_t > biggestWhiteMaskComponent;
    connectedComponents.getComponents( whiteMask,
                                       biggestWhiteMaskComponent,
                                       0,
                                       verbose );

    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving final white mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "saving final white mask '"
               << fileNameWhiteMask <<  "' : "
               << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameWhiteMask,
                                      biggestWhiteMaskComponent );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiTractographyMaskCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameFA, "
             "const std::string& fileNameWhiteMask, "
             "float faThreshold, "
             "const std::vector< float >& t2StdDevFactors, "
             "float erosionRadius, "
             "float dilationRadius, "
             "const std::string& intermediate, "
             "bool verbose  )" );

}


RegisterCommandCreator(
    DwiTractographyMaskCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameFA ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameWhiteMask ) +
    DECLARE_FLOATING_PARAMETER_HELP( faThreshold ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( t2StdDevFactors ) +
    DECLARE_FLOATING_PARAMETER_HELP( erosionRadius ) +
    DECLARE_FLOATING_PARAMETER_HELP( dilationRadius ) +
    DECLARE_STRING_PARAMETER_HELP( intermediate ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
