#include <gkg-processing-plugin-functors/DwiQBallEquator2Curve/DwiQBallEquator2CurveCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Curve3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-coordinates/GeodesicSO3Interpolator.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


std::string getMeshNameWithoutExtension( const std::string& name )
{

  std::string extension = ".mesh";

  if ( name.length() > extension.length() )
  {

     if ( name.substr( name.length() - extension.length(), extension.length())
          == extension )
     {

       return name.substr( 0, name.length() - extension.length() );

     }

  }
  return name;

}


int32_t getClosestOrientationIndex(
                     const gkg::Vector3d< float >& orientation,
                     const std::vector< gkg::Vector3d< float > >& orientations )
{

  float dotProduct = -1.0f;
  float bestDotProduct = -1.0f;
  std::vector< gkg::Vector3d< float > >::const_iterator
                                                       o = orientations.begin(),
                                                       oe = orientations.end();
  int32_t index = 0;
  int32_t bestIndex = 0;
 
  while ( o != oe )
  {

    dotProduct = o->dot( orientation );
    if ( dotProduct > bestDotProduct )
    {

      bestIndex = index;
      bestDotProduct = dotProduct;

    }
    ++ index;
    ++ o;

  }
  return bestIndex;

}


//
//   DwiQBallEquator2CurveCommand
//


gkg::DwiQBallEquator2CurveCommand::DwiQBallEquator2CurveCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin,
                                 bool removeFirst )
                 : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiQBallEquator2CurveCommand::DwiQBallEquator2CurveCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiQBallEquator2CurveCommand::DwiQBallEquator2CurveCommand(
                             const std::string& fileNameIn,
                             const std::vector< double >& normalizedOrientation,
    			     int32_t equatorPointCount,
    			     const std::vector< int32_t >& roiCorners,
    			     const std::string& fileNameOut,
    			     float scaling,
    			     int16_t minimum,
    			     int16_t maximum,
    			     std::vector< double >& positionOffset,
                             int32_t sO3InterpolatorPointCount,
                             float sO3InterpolatorSigma,
    			     bool ascii,
    			     bool verbose )
                                 : gkg::Command()
{

  try
  {

    execute( fileNameIn, normalizedOrientation, equatorPointCount, roiCorners,
             fileNameOut, scaling, 
             minimum, maximum, positionOffset, 
             sO3InterpolatorPointCount,
             sO3InterpolatorSigma,
             ascii, verbose );

  }
  GKG_CATCH( "gkg::DwiQBallEquator2CurveCommand::DwiQBallEquator2CurveCommand( "
             "const std::string& fileNameIn, "
             "const std::vector< double >& normalizedOrientation, "
    	     "int32_t equatorPointCount,  "
             "const std::vector< int32_t >& roiCorners, "
    	     "const std::string& fileNameOut, float scaling, "
             "int16_t minimum, "
             "int16_t maximum, "
    	     "std::vector< double >& positionOffset, "
             "int32_t sO3InterpolatorPointCount, "
             "float sO3InterpolatorSigma, "
    	     "bool ascii, bool verbose )" );

}


gkg::DwiQBallEquator2CurveCommand::DwiQBallEquator2CurveCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           normalizedOrientation );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, equatorPointCount );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          roiCorners );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, float, scaling );
    DECLARE_INTEGER_PARAMETER( parameters, int16_t, minimum );
    DECLARE_INTEGER_PARAMETER( parameters, int16_t, maximum );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           positionOffset );
    DECLARE_FLOATING_PARAMETER( parameters, float, sO3InterpolatorPointCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, sO3InterpolatorSigma );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, normalizedOrientation, equatorPointCount, roiCorners,
             fileNameOut, scaling, 
             minimum, maximum, positionOffset, 
             sO3InterpolatorPointCount,
             sO3InterpolatorSigma,
             ascii, verbose );

  }
  GKG_CATCH( "gkg::DwiQBallEquator2CurveCommand::DwiQBallEquator2CurveCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiQBallEquator2CurveCommand::~DwiQBallEquator2CurveCommand()
{
}


std::string gkg::DwiQBallEquator2CurveCommand::getStaticName()
{

  try
  {

    return "DwiQBallEquator2Curve";

  }
  GKG_CATCH( "std::string gkg::DwiQBallEquator2CurveCommand::getStaticName()" );

}


void gkg::DwiQBallEquator2CurveCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::vector< double > normalizedOrientation;
    int32_t equatorPointCount = -1;
    std::vector< int32_t > roiCorners;
    std::string fileNameOut;
    float scaling = -1.0f;
    int16_t minimum = -1;
    int16_t maximum = -1;
    std::vector< double > positionOffset;
    int32_t sO3InterpolatorPointCount = 0;
    float sO3InterpolatorSigma = 0.0f;
    bool ascii = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Process coordinates of points on the equator"
                                  " of a given orientation",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input DWI volume",
                                 fileNameIn );
    application.addSeriesOption( "-orientation",
                                 "Normalized orientation",
                                 normalizedOrientation,
                                 3, 3 );
    application.addSingleOption( "-c",
                                 "Point count on the equator",
                                 equatorPointCount );
    application.addSeriesOption( "-roi",
                                 "Region of interest defined by two points"
                                 "P1 and P2 (x1 y1 z1 x2 y2 z2)",
                                 roiCorners,
                                 6, 6 );
    application.addSingleOption( "-o",
                                 "Output 3D curve file",
                                 fileNameOut );
    application.addSingleOption( "-s",
                                 "Scaling (default=maximum based)",
                                 scaling,
                                 true );
    application.addSingleOption( "-m",
                                 "Force minimum for scaling",
                                 minimum,
                                 true );
    application.addSingleOption( "-M",
                                 "Force maximum for scaling",
                                 maximum,
                                 true );
    application.addSeriesOption( "-offset",
                                 "Position offset for visualization "
                                 "(default=0,0,0)",
                                 positionOffset,
                                 0, 3 );
    application.addSingleOption( "-sO3InterpolatorPointCount",
                                 "SO3 interpolator neighbor count",
                                 sO3InterpolatorPointCount );
    application.addSingleOption( "-sO3InterpolatorSigma",
                                 "S02 interpolator Gaussina weighting stddev",
                                 sO3InterpolatorSigma );
    application.addSingleOption( "-ascii",
                                 "Save ouput 3D curve in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, normalizedOrientation, equatorPointCount, roiCorners,
             fileNameOut, scaling,
             minimum, maximum, positionOffset,
             sO3InterpolatorPointCount,
             sO3InterpolatorSigma,
             ascii, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiQBallEquator2CurveCommand::parse()" );

}


void gkg::DwiQBallEquator2CurveCommand::execute(
                  	     const std::string& fileNameIn,
                             const std::vector< double >& normalizedOrientation,
    			     int32_t equatorPointCount,
    			     const std::vector< int32_t >& roiCorners,
    			     const std::string& fileNameOut,
    			     float scaling,
    			     int16_t minimum,
    			     int16_t maximum,
    			     std::vector< double >& positionOffset,
                             int32_t sO3InterpolatorPointCount,
                             float sO3InterpolatorSigma,
    			     bool ascii,
    			     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////
    if ( ( roiCorners[ 0 ] > roiCorners[ 3 ] ) ||
         ( roiCorners[ 1 ] > roiCorners[ 4 ] ) ||
         ( roiCorners[ 2 ] > roiCorners[ 5 ] ) )
    {

      throw std::runtime_error( "bad ROI prescription... should be of the form"
                       "\nx1 y1 z1 x2 y2 z2 with x1 < x2, y1 < y2, z1 < z2" );
    }

    if ( positionOffset.empty() )
    {

      positionOffset = std::vector< double >( 6, 0.0 );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading input data
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading input : " << std::flush;

    }

    std::string format;
    gkg::VolumeProxy< int16_t > inputVolumeProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                                   gkg::Volume< int16_t > >( fileNameIn,
                                                             inputVolumeProxy );
    if ( format != "gis" )
    {

      throw std::runtime_error( "wrong format, should be \'gis\'");

    }

    int32_t orientationCount = inputVolumeProxy.getSizeT();

    gkg::BoundingBox< int32_t >
      inputBoundingBox( roiCorners[ 0 ], roiCorners[ 3 ],
                        roiCorners[ 1 ], roiCorners[ 4 ],
                        roiCorners[ 2 ], roiCorners[ 5 ],
                        0, orientationCount - 1 );

    gkg::Volume< int16_t > inputVolume;
    format =
      gkg::Reader::getInstance().partialBinaryRead<
                                   gkg::Volume< int16_t > >( inputVolumeProxy,
                                                             inputVolume,
                                                             inputBoundingBox,
                                                             "gis" );
    if ( format != "gis" )
    {

      throw std::runtime_error(
                            "wrong format, should be \'gis\'");

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // extracting diffusion gradient orientation information
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "extracting DW orientations : " << std::flush;

    }
    gkg::GenericObjectList genericObjectListOrientations( orientationCount );
    std::vector< gkg::Vector3d< float > > orientations( orientationCount );

    if ( inputVolumeProxy.getHeader().hasAttribute(
                                           "diffusion_gradient_orientations" ) )
    {

      inputVolumeProxy.getHeader().getAttribute(
                                              "diffusion_gradient_orientations",
                                              genericObjectListOrientations );
      int32_t o = 0;
      for ( o = 0; o < orientationCount; o++ )
      {

        gkg::GenericObjectList tmp =
          genericObjectListOrientations[ o ]->getValue< 
                                                     gkg::GenericObjectList >();


	//orientations[ o ].x = -1 * ( float )tmp[ 0 ]->getScalar();
	 orientations[ o ].x = ( float )tmp[ 0 ]->getScalar();
         orientations[ o ].y = ( float )tmp[ 1 ]->getScalar();
         orientations[ o ].z = ( float )tmp[ 2 ]->getScalar();

      }

    }
    else
    {

      throw std::runtime_error(
                        "missing attribute 'diffusion_gradient_orientations'" );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // extracting resolutions
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "extracting resolutions : " << std::flush;

    }
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    if ( inputVolumeProxy.getHeader().hasAttribute( "resolutionX" ) )
    {

      inputVolumeProxy.getHeader().getAttribute( "resolutionX", resolutionX );

    }
    if ( inputVolumeProxy.getHeader().hasAttribute( "resolutionY" ) )
    {

      inputVolumeProxy.getHeader().getAttribute( "resolutionY", resolutionY );

    }
    if ( inputVolumeProxy.getHeader().hasAttribute( "resolutionZ" ) )
    {

      inputVolumeProxy.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }
    if ( verbose )
    {

      std::cout << resolutionX << "mm x "
                << resolutionY << "mm x "
                << resolutionZ << "mm"
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // normalizing orientation
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "normalized orientation : " << std::flush;

    }
    gkg::Vector3d< float > orientation( normalizedOrientation[ 0 ],
                                        normalizedOrientation[ 1 ],
                                        normalizedOrientation[ 2 ] );
    orientation.normalize();
    if ( verbose )
    {

      std::cout << orientation << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing closest points
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing closest points : " << std::flush;

    }
    gkg::GeodesicSO3Interpolator
      geodesicSO3Interpolator( sO3InterpolatorPointCount,
                               sO3InterpolatorSigma );
    gkg::OrientationSet orientationSet( orientations );

    std::vector< std::vector< std::pair< int32_t, float > > >
      neighborIndicesAndWeights( equatorPointCount );

    std::vector< gkg::Vector3d< float > > equatorPoints( equatorPointCount );

    gkg::Vector3d< float > zaxis( 0.0, 0.0, 1.0 );
    gkg::Vector3d< float > point;
    int32_t p;
    for ( p = 0; p < equatorPointCount; p++ )
    {

      // building the pth circle point coordinates
      float theta = 2.0 * M_PI * p / ( float )equatorPointCount;
      float ctheta = std::cos( theta );
      float stheta = std::sin( theta );
      if ( std::operator == ( orientation, zaxis ) == false )
      {

        point.x = ( ( orientation.x * orientation.x ) /
                    ( 1 + orientation.z ) - 1 ) * ctheta +
                    ( orientation.x * orientation.y /
                    ( 1 + orientation.z ) ) * stheta;
        point.y = ( ( orientation.x * orientation.y ) /
                    ( 1 + orientation.z ) ) * ctheta +
                  ( ( orientation.y * orientation.y ) /
                    ( 1 + orientation.z ) - 1 ) * stheta;
        point.z = orientation.x * ctheta + orientation.y * stheta;

      }
      else
      {

        point.x = ctheta;
        point.y = stheta;
        point.z = 0.0;

      }

      equatorPoints[ p ] = point;
      neighborIndicesAndWeights[ p ] =
        geodesicSO3Interpolator.getNeighborIndicesAndWeights(
          orientationSet,
          point );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing equator maximum GKG signal value
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing scaling : " << std::flush;

    }


    if ( minimum < 0 )
    {

      gkg::MinimumFilter< gkg::Volume< int16_t >, int16_t >
        minimumFilter;
      minimumFilter.filter( inputVolume, minimum );

    }

    int32_t sizeX = roiCorners[ 3 ] - roiCorners[ 0 ] + 1;
    int32_t sizeY = roiCorners[ 4 ] - roiCorners[ 1 ] + 1;
    int32_t sizeZ = roiCorners[ 5 ] - roiCorners[ 2 ] + 1;

    int32_t x = 0, y = 0, z = 0, n = 0, neighborCount = 0;
    if ( maximum < 0 )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {


            for ( p = 0; p < equatorPointCount; p++ )
            {

              neighborCount = ( int32_t )neighborIndicesAndWeights[ p ].size();
              float interpolatedValue = 0.0f;
              for ( n = 0; n < neighborCount; n++ )
              {

                interpolatedValue +=
                  ( float )inputVolume(
                                   x, y, z,
                                   neighborIndicesAndWeights[ p ][ n ].first ) *
                  neighborIndicesAndWeights[ p ][ n ].second;

              }
              if ( interpolatedValue > maximum )
              {

                maximum = interpolatedValue;

              }

            }

          }

        }

      }

    }


    if ( scaling < 0 )
    {

      scaling = std::max( resolutionX, std::max( resolutionY, resolutionZ ) ) /
                ( 2.0 * ( ( float )maximum - ( float )minimum ) );

    }
    else
    {

      scaling *= std::max( resolutionX, std::max( resolutionY, resolutionZ ) ) /
                 ( 2.0 * ( ( float )maximum - ( float )minimum ) );

    }


    if ( verbose )
    {

      std::cout << "min = " << minimum
                << "max = " << maximum
                << ", scale = " << scaling << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing equator 3D points
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing equators : " << std::flush;

    }
    int32_t siteCount = sizeX * sizeY * sizeZ;
    int32_t site = 0;

    std::vector< gkg::Curve3d< float > > curves( siteCount );
    std::vector< std::string > names( siteCount );

    double integralEquator = 0.0;
    float interpolatedValue = 0.0f;

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          curves[ site ] = gkg::Curve3d< float >( equatorPointCount );
          curves[ site ].setClosed();
          integralEquator = 0.0;

          for ( p = 0; p < equatorPointCount; p++ )
          {

            neighborCount = ( int32_t )neighborIndicesAndWeights[ p ].size();
            interpolatedValue = 0.0f;
            for ( n = 0; n < neighborCount; n++ )
            {

              interpolatedValue +=
                ( float )inputVolume(
                                   x, y, z,
                                   neighborIndicesAndWeights[ p ][ n ].first ) *
                neighborIndicesAndWeights[ p ][ n ].second;

            }

            integralEquator += interpolatedValue;


            point = gkg::Vector3d< float >(
                                 ( roiCorners[ 0 ] + x ) *
                                 ( float )resolutionX + positionOffset[ 0 ],
                                 ( roiCorners[ 1 ] + y ) *
                                 ( float )resolutionY + positionOffset[ 1 ],
                                 ( roiCorners[ 2 ] + z ) *
                                 ( float )resolutionZ + positionOffset[ 2 ] ) +
                    equatorPoints[ p ] *
                    ( interpolatedValue - minimum ) *
                    scaling;
            curves[ site ].setPoint( p, point );

          }

          curves[ site ].getHeader().addAttribute( "integral_equator",
                                                   integralEquator );
          names[ site ] = getMeshNameWithoutExtension( fileNameOut ) + '_' +
                gkg::StringConverter::toString( roiCorners[ 0 ] + x ) + '_' +
                gkg::StringConverter::toString( roiCorners[ 1 ] + y ) + '_' +
                gkg::StringConverter::toString( roiCorners[ 2 ] + z ) + ".mesh";
          ++ site;

        }

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving 3D curves
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "saving curves : " << std::flush;

    }
    for ( site = 0; site < siteCount; site++ )
    {

      gkg::Writer::getInstance().write( names[ site ], curves[ site ], ascii );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiQBallEquator2CurveCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::vector< double >& normalizedOrientation, "
    	     "int32_t equatorPointCount,  "
             "const std::vector< int32_t >& roiCorners, "
    	     "const std::string& fileNameOut, float scaling, "
             " int16_t minimum, int16_t maximum, "
    	     "std::vector< double >& positionOffset, "
             "int32_t sO3InterpolatorPointCount, "
             "float sO3InterpolatorSigma, "
    	     "bool ascii, bool verbose )" );

}


RegisterCommandCreator(
           DwiQBallEquator2CurveCommand,
           DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
           DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( normalizedOrientation ) +
           DECLARE_INTEGER_PARAMETER_HELP( equatorPointCount ) +
           DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( roiCorners ) +
           DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
           DECLARE_FLOATING_PARAMETER_HELP( scaling ) +
           DECLARE_INTEGER_PARAMETER_HELP( minimum ) +
           DECLARE_INTEGER_PARAMETER_HELP( maximum ) +
           DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( positionOffset ) +
           DECLARE_FLOATING_PARAMETER_HELP( sO3InterpolatorPointCount ) +
           DECLARE_FLOATING_PARAMETER_HELP( sO3InterpolatorSigma ) +
           DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
           DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
