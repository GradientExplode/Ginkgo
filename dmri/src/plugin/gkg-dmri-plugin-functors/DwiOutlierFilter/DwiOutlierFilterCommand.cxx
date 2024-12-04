#include <gkg-dmri-plugin-functors/DwiOutlierFilter/DwiOutlierFilterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-registration/SimilarityMeasureFactory_i.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-transform/ParameterizedRigidTransform3d.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-processing-algobase/StdDevFilter_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/SphericalQSpaceSampling.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <set>
#include <iomanip>
#include <fstream>


#define EPSILON 1e-6


///////////////////////////////////////////////////////////////////////////////
// various cases to be taken into account
//
// 1)  s,o isolé en s et o
//
//       si spherique
//         interpolation sur la sphere dans l'espace Q
//       sinon
//         interpolation spatiale avec les coupes adjacentes
//
// 2)  s,o non isolé en s et isolé en o
//
//       si sphérique
//         interpolation sur la sphere dans l'espace Q
//       sinon
//        
// 3)  s,o isolé en s et non isolé en o
//
// 4)  s,o non isolé en s et non isolé en o
///////////////////////////////////////////////////////////////////////////////



struct OutlierPosition
{

  OutlierPosition() : slice( 0 ), orientation( 0 ) {}
  OutlierPosition( int32_t theSlice, int32_t theOrientation )
    : slice( theSlice ), orientation( theOrientation ) {}
  OutlierPosition( const OutlierPosition& other )
    : slice( other.slice ), orientation( other.orientation ) {}

  OutlierPosition& operator=( const OutlierPosition& other )
  {

    slice = other.slice;
    orientation = other.orientation;
    return *this;

  }

  int32_t slice;
  int32_t orientation;

};


namespace std
{


ostream& operator<<( ostream& os,
                     const OutlierPosition& thing )
{

  os << "( " << thing.slice << ", " << thing.orientation << " )";
  return os;

}


}


struct OutlierPositionCompare /*: public std::binary_function< OutlierPosition,
                                                             OutlierPosition,
                                                             bool >
                                deprecated in c++17 */
{

  typedef OutlierPosition first_argument_type;
  typedef OutlierPosition second_argument_type;
  typedef bool result_type;

  bool operator()( const OutlierPosition& o1, const OutlierPosition& o2 ) const
  {

    return ( o1.slice < o2.slice ) ||
           ( ( o1.slice == o2.slice ) &&
             ( o1.orientation < o2.orientation ) );
  }

};


#if 0
static bool correctIsolatedSlice( const OutlierPosition& outlierPosition,
                                  gkg::Volume< float >& dw,
                                  const std::string& sliceAxis )
{

  try
  {

    bool ok = false;

    int32_t sizeX = dw.getSizeX();
    int32_t sizeY = dw.getSizeY();
    int32_t sizeZ = dw.getSizeZ();
    int32_t o = outlierPosition.orientation;

    if ( sliceAxis == "x" )
    {

      if ( ( outlierPosition.slice != 0 ) &&
           ( outlierPosition.slice != sizeX - 1 ) )
      {

        int32_t x = outlierPosition.slice;
        int32_t y = 0;
        int32_t z = 0;
        for ( y = 0; y < sizeY; y++ )
        {

          for ( z = 0; z < sizeZ; z++ )
          {

            dw( x, y, z, o ) = ( float )( 0.5 * dw( x - 1, y, z, o ) +
                                          0.5 * dw( x + 1, y, z, o ) );

          }

        }

        ok = true;

      }

    }
    else if ( sliceAxis == "y" )
    {

      if ( ( outlierPosition.slice != 0 ) &&
           ( outlierPosition.slice != sizeY - 1 ) )
      {

        int32_t x = 0;
        int32_t y = outlierPosition.slice;
        int32_t z = 0;
        for ( x = 0; x < sizeX; x++ )
        {

          for ( z = 0; z < sizeZ; z++ )
          {

            dw( x, y, z, o ) = ( float )( 0.5 * dw( x, y - 1, z, o ) +
                                          0.5 * dw( x, y + 1, z, o ) );

          }

        }

        ok = true;

      }

    }
    else if ( sliceAxis == "z" )
    {

      if ( ( outlierPosition.slice != 0 ) &&
           ( outlierPosition.slice != sizeZ - 1 ) )
      {

        int32_t x = 0;
        int32_t y = 0;
        int32_t z = outlierPosition.slice;
        for ( x = 0; x < sizeX; x++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            dw( x, y, z, o ) = ( float )( 0.5 * dw( x, y, z - 1, o ) +
                                          0.5 * dw( x, y, z + 1, o ) );

          }

        }

        ok = true;

      }

    }

    return ok;

  }
  GKG_CATCH( "void correctIsolatedSlice( "
             "const OutlierPosition& outlierPosition, "
             "gkg::Volume< int16_t >& dw )" );

}
#endif


//
// correction algorithm dedicated to single-shell acquisitions
//

static void correctSphericalSingleShell( 
            const std::set< OutlierPosition, OutlierPositionCompare >& outliers,
            const gkg::Volume< float >& t2,
            gkg::Volume< float >& dw,
            const gkg::Volume< int16_t >& /*mask*/,
            gkg::RCPointer< gkg::QSpaceSampling > qSpaceSampling,
	    const std::vector< double >& outlierFilterParameters,
            const std::string& sliceAxis,
            bool verbose )
{

  try
  {

    if ( !outlierFilterParameters.empty() &&
         ( outlierFilterParameters.size() != 2U ) )
    {
    
      throw std::runtime_error( "bad outlier filter parameter count" );
    
    }

    int32_t maximumSphericalHarmonicOrder = 4;
    double laplaceBeltramiFactor = 0.006;
    if ( !outlierFilterParameters.empty() )
    {
    
      maximumSphericalHarmonicOrder =
                              ( int32_t )( outlierFilterParameters[ 0 ] + 0.5 );
      laplaceBeltramiFactor = outlierFilterParameters[ 1 ];
    
    }
    
    // processing maximum j according to maximum order
    int32_t N = ( ( maximumSphericalHarmonicOrder + 1 ) *
                  ( maximumSphericalHarmonicOrder + 2 ) ) / 2;
  
    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // creating map of slice->orientation outliers
    std::map< int32_t, std::set< int32_t > > outlierMap;
    
    std::set< OutlierPosition, OutlierPositionCompare >::const_iterator
      o = outliers.begin(),
      oe = outliers.end();
    while ( o != oe )
    {
     
      outlierMap[ o->slice ].insert( o->orientation );
      ++ o;
     
    }

    // building Laplace-Beltrami smoothness matrix for regularization
    gkg::Matrix L( N, N );
    L.setZero();
    double sphericalLaplacian = 0;
    int32_t n, l, m;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      sphericalLaplacian = - l * ( l + 1 );
      L( n, n ) = sphericalLaplacian * sphericalLaplacian;

    }


    // correcting slices
    gkg::Vector dwSphericalHarmonicsCoefficients;
    int32_t sh = 0;
    int32_t count = 0;
    std::map< int32_t, std::set< int32_t > >::const_iterator
      s = outlierMap.begin(),
      se = outlierMap.end();
    while ( s != se )
    {

      if ( verbose )
      {

        if ( count != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << " [ " << std::setw( 3 ) << count + 1 
        	  << "(" << std::setw( 3 ) << s->first << ") "
        	  << " / " << std::setw( 3 ) << outlierMap.size()
        	  << " ]" << std::flush;


      }

      const std::set< int32_t > & outlierOrientationIndices = s->second;
    
      // collecting outlier count for current slice
      int32_t outlierCount = ( int32_t )outlierOrientationIndices.size();

      // measure count
      int32_t M = qSpaceSampling->getCount() - outlierCount;

      // building orientation index look-up table
      std::vector< int32_t > validIndices;
      std::vector< int32_t > outlierIndices;
      validIndices.reserve( M );
      outlierIndices.reserve( outlierCount );
      for ( m = 0; m < qSpaceSampling->getCount(); m++ )
      {
      
        if ( outlierOrientationIndices.find( m ) == 
	     outlierOrientationIndices.end() )
	{
	
	  validIndices.push_back( m );
	  
	}
        else
        {

	  outlierIndices.push_back( m );

        }

      }

      // building matrix of spherical harmonics on input orientation(s)
      gkg::Vector3d< float > orientation;
      double radial = 0.0;
      double theta = 0.0;
      double phi = 0.0;

      gkg::Matrix B( M, N );

      for ( m = 0; m < M; m++ )
      {

        orientation = qSpaceSampling->getOrientation( validIndices[ m ] );
        factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                     ( double )orientation.y,
                                                     ( double )orientation.z,
                                                     radial,
                                                     theta,
                                                     phi );
        for ( n = 0; n < N; n++ )
        {

          B( m, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

        }

      }

      // building Bplus matrix with size N x M
      // Bplus = (B^T B + l L )^(-1) B^T
      gkg::Matrix Bplus( N, M );
      factory->getThikonovPseudoInverse( B, L, laplaceBeltramiFactor, Bplus );

      if ( sliceAxis == "x" )
      {

        int32_t sizeY = t2.getSizeY();
        int32_t sizeZ = t2.getSizeZ();

        gkg::Vector3d< int32_t > site;
        site.x = s->first;

        for ( site.z = 0; site.z < sizeZ; site.z ++ )
        {

          for ( site.y = 0; site.y < sizeY; site.y ++ )
          {

 	    // processing average T2 value
 	    double averageT2 = ( double )t2( site );
 	    if ( averageT2 <= 0 )
 	    {

 	      averageT2 = EPSILON;

 	    }

 	    // building valid normalized signal vector
 	    gkg::Vector normalizedSignal( M );
 	    for ( m = 0; m < M; m++ )
 	    {
 
 	      float dwValue = dw( site, validIndices[ m ] );
 	      normalizedSignal( m ) = ( double )dwValue / averageT2;

 	    }

            // processing DW spherical harmonics coefficient(s)
            dwSphericalHarmonicsCoefficients = Bplus.getComposition(
                                                             normalizedSignal );

            // looping over outliers and correcting them
            for ( m = 0; m < outlierCount; m++ )
            {

              // collecting outlier orientation
              orientation = qSpaceSampling->getOrientation(
                                                          outlierIndices[ m ] );

              // converting cartesian coordinate(s) to spherical coordinate(s)
              factory->getCartesianToSphericalCoordinates(
                                                     ( double )orientation.x,
                                                     ( double )orientation.y,
                                                     ( double )orientation.z,
                                                     radial,
                                                     theta,
                                                     phi );

              // looping over spherical harmonic(s)
              double correctedDwValue = 0;
              for ( sh = 0; sh < N; sh++ )
              {

                correctedDwValue += 
                     dwSphericalHarmonicsCoefficients( sh ) *
                     factory->getSymmetricalSphericalHarmonic( sh, phi, theta );

              }

              // rescaling result with T2 value
              correctedDwValue *= averageT2;
              if ( correctedDwValue < 0 )
              {

                 correctedDwValue = 0;

              }

              // storing back corrected value
              dw( site, outlierIndices[ m ] ) = ( float )correctedDwValue;

            }

          }

        }

      }
      else if ( sliceAxis == "y" )
      {

        int32_t sizeX = t2.getSizeX();
        int32_t sizeZ = t2.getSizeZ();

        gkg::Vector3d< int32_t > site;
        site.y = s->first;

        for ( site.z = 0; site.z < sizeZ; site.z ++ )
        {

          for ( site.x = 0; site.x < sizeX; site.x ++ )
          {

 	    // processing average T2 value
 	    double averageT2 = ( double )t2( site );
 	    if ( averageT2 <= 0 )
 	    {

 	      averageT2 = EPSILON;

 	    }

 	    // building valid normalized signal vector
 	    gkg::Vector normalizedSignal( M );
 	    for ( m = 0; m < M; m++ )
 	    {
 
 	      float dwValue = dw( site, validIndices[ m ] );
 	      normalizedSignal( m ) = ( double )dwValue / averageT2;

 	    }

            // processing DW spherical harmonics coefficient(s)
            dwSphericalHarmonicsCoefficients = Bplus.getComposition(
                                                             normalizedSignal );

            // looping over outliers and correcting them
            for ( m = 0; m < outlierCount; m++ )
            {

              // collecting outlier orientation
              orientation = qSpaceSampling->getOrientation(
                                                          outlierIndices[ m ] );

              // converting cartesian coordinate(s) to spherical coordinate(s)
              factory->getCartesianToSphericalCoordinates(
                                                     ( double )orientation.x,
                                                     ( double )orientation.y,
                                                     ( double )orientation.z,
                                                     radial,
                                                     theta,
                                                     phi );

              // looping over spherical harmonic(s)
              double correctedDwValue = 0;
              for ( sh = 0; sh < N; sh++ )
              {

                correctedDwValue += 
                     dwSphericalHarmonicsCoefficients( sh ) *
                     factory->getSymmetricalSphericalHarmonic( sh, phi, theta );

              }

              // rescaling result with T2 value
              correctedDwValue *= averageT2;
              if ( correctedDwValue < 0 )
              {

                 correctedDwValue = 0;

              }

              // storing back corrected value
              dw( site, outlierIndices[ m ] ) = ( float )correctedDwValue;

            }

          }

        }

      }
      else if ( sliceAxis == "z" )
      {

        int32_t sizeX = t2.getSizeX();
        int32_t sizeY = t2.getSizeY();

        gkg::Vector3d< int32_t > site;
        site.z = s->first;

        for ( site.y = 0; site.y < sizeY; site.y ++ )
        {

          for ( site.x = 0; site.x < sizeX; site.x ++ )
          {

 	    // processing average T2 value
 	    double averageT2 = ( double )t2( site );
 	    if ( averageT2 <= 0 )
 	    {

 	      averageT2 = EPSILON;

 	    }

 	    // building valid normalized signal vector
 	    gkg::Vector normalizedSignal( M );
 	    for ( m = 0; m < M; m++ )
 	    {
 
 	      float dwValue = dw( site, validIndices[ m ] );
 	      normalizedSignal( m ) = ( double )dwValue / averageT2;

 	    }

            // processing DW spherical harmonics coefficient(s)
            dwSphericalHarmonicsCoefficients = Bplus.getComposition(
                                                             normalizedSignal );

            // looping over outliers and correcting them
            for ( m = 0; m < outlierCount; m++ )
            {

              // collecting outlier orientation
              orientation = qSpaceSampling->getOrientation(
                                                          outlierIndices[ m ] );

              // converting cartesian coordinate(s) to spherical coordinate(s)
              factory->getCartesianToSphericalCoordinates(
                                                     ( double )orientation.x,
                                                     ( double )orientation.y,
                                                     ( double )orientation.z,
                                                     radial,
                                                     theta,
                                                     phi );

              // looping over spherical harmonic(s)
              double correctedDwValue = 0;
              for ( sh = 0; sh < N; sh++ )
              {

                correctedDwValue += 
                     dwSphericalHarmonicsCoefficients( sh ) *
                     factory->getSymmetricalSphericalHarmonic( sh, phi, theta );

              }

              // rescaling result with T2 value
              correctedDwValue *= averageT2;
              if ( correctedDwValue < 0 )
              {

                 correctedDwValue = 0;

              }

              // storing back corrected value
              dw( site, outlierIndices[ m ] ) = ( float )correctedDwValue;

            }

          }

        }

      }

      ++ s;
      ++ count;
    
    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 20 );

    }

  }
  GKG_CATCH( "void correctSphericalSingleShell( "
             "const std::set< OutlierPosition, OutlierPositionCompare >& "
	     "outliers, "
             "const gkg::Volume< int16_t >& t2, "
 	     "const gkg::Volume< int16_t >& dw, "
 	     "const gkg::Volume< int16_t >& mask,"
 	     "gkg::RCPointer< gkg::QSpaceSampling > qSpaceSampling, "
	     "const std::vector< double >& outlierFilterParameters, "
	     "gkg::Volume< int16_t >& correctedDw )" );

}
	   

//
// class DwiOutlierFilterCommand
//

gkg::DwiOutlierFilterCommand::DwiOutlierFilterCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DwiOutlierFilterCommand::DwiOutlierFilterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiOutlierFilterCommand::DwiOutlierFilterCommand(
                        const std::string& fileNameT2,
                        const std::string& fileNameDW,
                        const std::string& fileNameMask,
                        const std::string& sliceAxis,
                        const std::string& fileNameCorrectedDW,
                        const std::string& fileNameMI,
                        const std::string& fileNameOutliers,
                        double outlierFactor,
                        float radius,
                        float thresholdRatio,
                        const std::vector< double >& outlierFilterParameters,
                        const std::string& volumeFormat,
                        bool ascii,
                        bool verbose )
                             : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             sliceAxis,
             fileNameCorrectedDW,
             fileNameMI,
             fileNameOutliers,
             outlierFactor,
             radius,
             thresholdRatio,
             outlierFilterParameters,
             volumeFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiOutlierFilterCommand::DwiOutlierFilterCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& sliceAxis, "
             "const std::string& fileNameCorrectedDW, "
             "const std::string& fileNameMI, "
             "const std::string& fileNameOutliers, "
             "double outlierFactor, "
             "float radius, "
             "float thresholdRatio, "
             "const std::vector< double >& outlierFilterParameters, "
             "const std::string& volumeFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiOutlierFilterCommand::DwiOutlierFilterCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, sliceAxis );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameCorrectedDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMI );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutliers );
    DECLARE_FLOATING_PARAMETER( parameters, double, outlierFactor );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, float, thresholdRatio );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           outlierFilterParameters );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             sliceAxis,
             fileNameCorrectedDW,
             fileNameMI,
             fileNameOutliers,
             outlierFactor,
             radius,
             thresholdRatio,
             outlierFilterParameters,
             volumeFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiOutlierFilterCommand::DwiOutlierFilterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiOutlierFilterCommand::~DwiOutlierFilterCommand()
{
}


std::string gkg::DwiOutlierFilterCommand::getStaticName()
{

  try
  {

    return "DwiOutlierFilter";

  }
  GKG_CATCH( "std::string gkg::DwiOutlierFilterCommand::getStaticName()" );

}


void gkg::DwiOutlierFilterCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMask;
    std::string sliceAxis = "z";
    std::string fileNameCorrectedDW;
    std::string fileNameMI;
    std::string fileNameOutliers;

    double outlierFactor = 5.0;

    float radius = -1.0;
    float thresholdRatio = 0.04;
    
    std::vector< double > outlierFilterParameters;

    std::string volumeFormat = "gis";
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI outlier filter",
                                  _loadPlugin );
    application.addSingleOption( "-t2",
                                 "Input T2-weighted volume file name",
                                 fileNameT2 );
    application.addSingleOption( "-dw",
                                 "Input DW-weighted volume file name",
                                 fileNameDW );
    application.addSingleOption( "-m",
                                 "Input mask file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-axis",
                                 "Slice axis (default=z)",
                                 sliceAxis,
                                 true );
    application.addSingleOption( "-f",
                                 "Outlier factor ie slice is not considered as"
                                 " an outlier if it is contained in the range"
                                 " mean(MI) +- factor x std.dev(MI) "
                                 "(default=5.0)",
                                 outlierFactor,
                                 true );
    application.addSingleOption( "-o",
                                 "Output corrected DW volume file name",
                                 fileNameCorrectedDW );
    application.addSingleOption( "-mi",
                                 "Mutual information spreadsheet filename "
                                 "for any slice / orientation",
                                 fileNameMI,
                                 true );
    application.addSingleOption( "-outliers",
                                 "Outlier python file for matplotlib ",
                                 fileNameOutliers,
                                 true );
				 
    application.addSeriesOption( "-parameters",
                                 "Parameters of the correction algorithm: \n"
				 "- spherical_single-shell:\n"
				 "   <SH_order> <laplace_beltrami_factor>\n"
				 "   (default=4/0.006)",
				 outlierFilterParameters );
    application.addSingleOption( "-radius",
                                 "Radius of morphological operation for mask "
                                 "(default=2*min. res.)",
                                 radius,
                                 true );
    application.addSingleOption( "-thresholdRatio",
                                 "Threshold ratio for mask (default=0.04)",
                                 thresholdRatio,
                                 true );
    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
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

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             sliceAxis,
             fileNameCorrectedDW,
             fileNameMI,
             fileNameOutliers,
             outlierFactor,
             radius,
             thresholdRatio,
             outlierFilterParameters,
             volumeFormat,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiOutlierFilterCommand::parse()" );

}


void gkg::DwiOutlierFilterCommand::execute(
                        const std::string& fileNameT2,
                        const std::string& fileNameDW,
                        const std::string& fileNameMask,
                        const std::string& sliceAxis,
                        const std::string& fileNameCorrectedDW,
                        const std::string& fileNameMI,
                        const std::string& fileNameOutliers,
                        double outlierFactor,
                        float radius,
                        float thresholdRatio,
                        const std::vector< double >& outlierFilterParameters,
                        const std::string& volumeFormat,
                        bool ascii,
                        bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( ( sliceAxis != "x" ) &&
         ( sliceAxis != "y" ) &&
         ( sliceAxis != "z" ) )
    {

      throw std::runtime_error( "bad signal axis" );

    }
         

    ////////////////////////////////////////////////////////////////////////////
    // map of correction algorithm
    ////////////////////////////////////////////////////////////////////////////

    typedef void ( *CorrectionAlgorithm )( 
                     const std::set< OutlierPosition, OutlierPositionCompare >&,
                     const gkg::Volume< float >&,
                     gkg::Volume< float >&,
                     const gkg::Volume< int16_t >&,
                     gkg::RCPointer< gkg::QSpaceSampling >,
                     const std::vector< double >&,
                     const std::string&,
                     bool );
					   
    std::map< std::string, CorrectionAlgorithm >
      correctionAlgorithmMap;
      
    correctionAlgorithmMap[ "spherical_single-shell" ] = 
      correctSphericalSingleShell;


    ////////////////////////////////////////////////////////////////////////////
    // reading T2 data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameT2 << "' : " << std::flush;

    }

    gkg::Volume< float > t2;
    gkg::TypedVolumeReaderProcess< float > t2VolumeReaderProcess( t2 );
    t2VolumeReaderProcess.execute( fileNameT2 );
    int32_t sizeX = t2.getSizeX();
    int32_t sizeY = t2.getSizeY();
    int32_t sizeZ = t2.getSizeZ();
    gkg::Vector3d< double > resolution;
    t2.getResolution( resolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading DW volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameDW << "' : " << std::flush;

    }

    gkg::Volume< float > dw;
    gkg::TypedVolumeReaderProcess< float > dwVolumeReaderProcess( dw );
    dwVolumeReaderProcess.execute( fileNameDW );
    int32_t inputOrientationCount = dw.getSizeT();

    if ( ( dw.getSizeX() != sizeX ) ||
         ( dw.getSizeY() != sizeY ) ||
         ( dw.getSizeZ() != sizeZ ) )
    {

      throw std::runtime_error( "T2 and DW volumes are not of the same size!" );

    }

    gkg::Vector3d< double > tmpResolution;
    dw.getResolution( tmpResolution );
    if ( tmpResolution != resolution )
    {

      throw std::runtime_error( "T2 and DW voxel resolutions are not of the "
                                "same resolution!" );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // making or reading mask of signal
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< int16_t > mask;
    if ( fileNameMask.empty() )
    {

      if ( verbose )
      {

        std::cout << "building mask of signal from T2 data : " << std::flush;

      }
      if ( radius <= 0 )
      {

        radius = 2.0 * std::min( resolution.x,
                                 std::min( resolution.y, resolution.z ) );

      }
      gkg::ClosedMask< float, int16_t > closedMask( radius, thresholdRatio );
      closedMask.getMask( t2, mask, verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading '" << fileNameMask << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read( fileNameMask, mask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // obtaining Q-space sampling from DW data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "obtaining Q-space sampling : " << std::flush;

    }

    gkg::RCPointer< gkg::QSpaceSampling >
      qSpaceSampling(  gkg::QSpaceSamplingFactory::getInstance().create( dw ) );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    std::string qSpaceSamplingName = "";
    bool isCartesian = qSpaceSampling->isCartesian();
    bool isSpherical = qSpaceSampling->isSpherical();


    if ( verbose )
    {

      if ( isCartesian )
      {
      
        std::cout << "sampling type : cartesian" << std::endl; 
        qSpaceSamplingName = "cartesian";

      }
      else if ( isSpherical )
      {
      
        std::cout << "sampling type : spherical" << std::flush; 
        qSpaceSamplingName = "spherical";

      }

    }
    
    bool isSingleShell = false;
    bool isMultipleShell = false;
    if ( isSpherical )
    {
    
      gkg::SphericalQSpaceSampling*
        sphericalQSpaceSampling = static_cast< gkg::SphericalQSpaceSampling* >(
	  qSpaceSampling.get() );
	  
      isSingleShell = sphericalQSpaceSampling->isSingleShell();
      isMultipleShell = sphericalQSpaceSampling->isMultipleShell();

      if ( isSingleShell )
      {
      
        std::cout << ", single-shell" << std::endl;
        qSpaceSamplingName += "_single-shell";

      }
      else if ( isMultipleShell )
      {
      
        std::cout << ", multiple-shell" << std::endl;
        qSpaceSamplingName += "_multiple-shell";

      }
    
    }
    

    ////////////////////////////////////////////////////////////////////////////
    // evaluating mutual information for each pair of DW/T2 slices
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "estimating mutual information between T2/DW pairs : "
                << std::flush;

    }

    gkg::RCPointer< gkg::ParameterizedRigidTransform3d< float > >
      identityTransform3d( new gkg::ParameterizedRigidTransform3d< float > );

    gkg::RCPointer< gkg::Matrix > MI( new gkg::Matrix );

    // std::ofstream os( "id.trm" );
    // identityTransform3d->writeTrm( os );
    // os.close();

    std::ofstream osMI;
    if ( !fileNameMI.empty() )
    {

      osMI.open( fileNameMI.c_str() );

    }

    gkg::MinimumFilter< gkg::Volume< float >, float > minimumFilter;
    gkg::MaximumFilter< gkg::Volume< float >, float > maximumFilter;
    float minimum = 0;
    float maximum = 0;

    if ( sliceAxis == "x" )
    {

      gkg::RCPointer< gkg::Volume< float > >
        t2Slice( new gkg::Volume< float >( sizeY, sizeZ ) );
      t2Slice->getHeader().addAttribute( "resolutionX", resolution.y );
      t2Slice->getHeader().addAttribute( "resolutionY", resolution.z );
      t2Slice->getHeader().addAttribute( "resolutionZ", ( double )1.0 );

      gkg::RCPointer< gkg::Volume< float > >
        dwSlice( new gkg::Volume< float >( sizeY, sizeZ ) );
      dwSlice->getHeader().addAttribute( "resolutionX", resolution.y );
      dwSlice->getHeader().addAttribute( "resolutionY", resolution.z );
      dwSlice->getHeader().addAttribute( "resolutionZ", ( double )1.0 );

      // reducing dynamic and smoothing joint pdf
      std::vector< double > scalarParameters( 2 );
      scalarParameters[ 0 ] = 64;
      scalarParameters[ 1 ] = 1;
      gkg::Vector parameters( 6 );
      parameters.fill( 0 );

      MI->reallocate( sizeX, inputOrientationCount );


      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;
      int32_t o = 0;
      for ( x = 0; x < sizeX; x++ )
      {

        if ( verbose )
        {

          if ( x )
          {

            std::cout << gkg::Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << x + 1 
                    << " / " << std::setw( 7 ) << sizeX
                    << " ]" << std::flush;


        }

        for ( y = 0; y < sizeY; y++ )
        {

          for ( z = 0; z < sizeZ; z++ )
          {

            ( *t2Slice )( y, z ) = t2( x, y, z );

          }

        }

        minimumFilter.filter( *t2Slice, minimum );
        maximumFilter.filter( *t2Slice, maximum );

        if ( minimum == maximum )
        {

          for ( o = 0; o < inputOrientationCount; o++ )
          {

            ( *MI )( x, o ) = -1.0; // not to be processed
            osMI << x << "   "
                 << o << "   "
                 << "N/A"
                 << std::endl;

          }

        }
        else
        {

          for ( o = 0; o < inputOrientationCount; o++ )
          {

            for ( y = 0; y < sizeY; y++ )
            {

              for ( z = 0; z < sizeZ; z++ )
              {

                ( *dwSlice )( y, z ) = dw( x, y, z, o );

              }

            }
            try
            {

              gkg::MutualInformationSimilarityMeasure< float, float >
              similarityMeasure(
                         t2Slice,
                         dwSlice,
                         0,                  // reference lower threshold
                         0,                  // floating lower threshold
                         1,                       // resampling order
                         std::max( sizeY, sizeZ ),// sub-sampling size
                         scalarParameters,
                         false );
              ( *MI )( x, o ) = -similarityMeasure.getValueAt(
                                                         *identityTransform3d );
              osMI << x << "   "
                   << o << "   "
                   << ( *MI )( x, o )
                   << std::endl;

            }
            catch( std::exception& )
            {

              ( *MI )( x, o ) = -1.0; // not to be processed
              osMI << x << "   "
                   << o << "   "
                   << "N/A"
                   << std::endl;

            }

          }

        }

      }

    }
    else if ( sliceAxis == "y" )
    {

      gkg::RCPointer< gkg::Volume< float > >
        t2Slice( new gkg::Volume< float >( sizeX, sizeZ ) );
      t2Slice->getHeader().addAttribute( "resolutionX", resolution.x );
      t2Slice->getHeader().addAttribute( "resolutionY", resolution.z );
      t2Slice->getHeader().addAttribute( "resolutionZ", ( double )1.0 );

      gkg::RCPointer< gkg::Volume< float > >
        dwSlice( new gkg::Volume< float >( sizeX, sizeZ ) );
      dwSlice->getHeader().addAttribute( "resolutionX", resolution.x );
      dwSlice->getHeader().addAttribute( "resolutionY", resolution.z );
      dwSlice->getHeader().addAttribute( "resolutionZ", ( double )1.0 );

      // reducing dynamic and smoothing joint pdf
      std::vector< double > scalarParameters( 2 );
      scalarParameters[ 0 ] = 64;
      scalarParameters[ 1 ] = 1;
      gkg::Vector parameters( 6 );
      parameters.fill( 0 );

      MI->reallocate( sizeY, inputOrientationCount );

      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;
      int32_t o = 0;
      for ( y = 0; y < sizeY; y++ )
      {

        if ( verbose )
        {

          if ( y )
          {

            std::cout << gkg::Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << y + 1 
                    << " / " << std::setw( 7 ) << sizeY
                    << " ]" << std::flush;


        }

        for ( x = 0; x < sizeX; x++ )
        {

          for ( z = 0; z < sizeZ; z++ )
          {

            ( *t2Slice )( x, z ) = t2( x, y, z );

          }

        }

        minimumFilter.filter( *t2Slice, minimum );
        maximumFilter.filter( *t2Slice, maximum );

        if ( minimum == maximum )
        {

          for ( o = 0; o < inputOrientationCount; o++ )
          {

            ( *MI )( y, o ) = -1.0; // not to be processed
            osMI << y << "   "
                 << o << "   "
                 << "N/A"
                 << std::endl;

          }

        }
        else
        {

          for ( o = 0; o < inputOrientationCount; o++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              for ( z = 0; z < sizeZ; z++ )
              {

                ( *dwSlice )( x, z ) = dw( x, y, z, o );

              }

            }
            try
            {

              gkg::MutualInformationSimilarityMeasure< float, float >
              similarityMeasure(
                         t2Slice,
                         dwSlice,
                         0,                   // reference lower threshold
                         0,                   // floating lower threshold
                         1,                     // resampling order
                         std::max( sizeX, sizeZ ),// sub-sampling size
                         scalarParameters,
                         false );
              ( *MI )( y, o ) = -similarityMeasure.getValueAt(
                                                         *identityTransform3d );
              osMI << y << "   "
                   << o << "   "
                   << ( *MI )( y, o )
                   << std::endl;

            }
            catch( std::exception& )
            {

              ( *MI )( y, o ) = -1.0; // not to be processed
              osMI << y << "   "
                   << o << "   "
                   << "N/A"
                   << std::endl;

            }

          }

        }

      }

    }
    else if ( sliceAxis == "z" )
    {

      gkg::RCPointer< gkg::Volume< float > >
        t2Slice( new gkg::Volume< float >( sizeX, sizeY ) );
      t2Slice->getHeader().addAttribute( "resolutionX", resolution.x );
      t2Slice->getHeader().addAttribute( "resolutionY", resolution.y );
      t2Slice->getHeader().addAttribute( "resolutionZ", ( double )1.0 );

      gkg::RCPointer< gkg::Volume< float > >
        dwSlice( new gkg::Volume< float >( sizeX, sizeY ) );
      dwSlice->getHeader().addAttribute( "resolutionX", resolution.x );
      dwSlice->getHeader().addAttribute( "resolutionY", resolution.y );
      dwSlice->getHeader().addAttribute( "resolutionZ", ( double )1.0 );

      // reducing dynamic and smoothing joint pdf
      std::vector< double > scalarParameters( 2 );
      scalarParameters[ 0 ] = 64;
      scalarParameters[ 1 ] = 1;
      gkg::Vector parameters( 6 );
      parameters.fill( 0 );

      MI->reallocate( sizeZ, inputOrientationCount );

      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;
      int32_t o = 0;
      for ( z = 0; z < sizeZ; z++ )
      {

        if ( verbose )
        {

          if ( z )
          {

            std::cout << gkg::Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << z + 1 
                    << " / " << std::setw( 7 ) << sizeZ
                    << " ]" << std::flush;


        }

        for ( x = 0; x < sizeX; x++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            ( *t2Slice )( x, y ) = t2( x, y, z );

          }

        }

        minimumFilter.filter( *t2Slice, minimum );
        maximumFilter.filter( *t2Slice, maximum );

        if ( minimum == maximum )
        {

          for ( o = 0; o < inputOrientationCount; o++ )
          {

            ( *MI )( z, o ) = -1.0; // not to be processed
            osMI << z << "   "
                 << o << "   "
                 << "N/A"
                 << std::endl;

          }

        }
        else
        {

          for ( o = 0; o < inputOrientationCount; o++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              for ( y = 0; y < sizeY; y++ )
              {

                ( *dwSlice )( x, y ) = dw( x, y, z, o );

              }

            }
            try
            {

              gkg::MutualInformationSimilarityMeasure< float, float >
              similarityMeasure(
                         t2Slice,
                         dwSlice,
                         0,                   // reference lower threshold
                         0,                   // floating lower threshold
                         1,                     // resampling order
                         std::max( sizeX, sizeY ),// sub-sampling size
                         scalarParameters,
                         false );
              ( *MI )( z, o ) = -similarityMeasure.getValueAt( 
                                                         *identityTransform3d );
              osMI << z << "   "
                   << o << "   "
                   << ( *MI )( z, o )
                   << std::endl;

            }
            catch( std::exception& )
            {

              ( *MI )( z, o ) = -1.0; // not to be processed
              osMI << z << "   "
                   << o << "   "
                   << "N/A"
                   << std::endl;

            }

          }

        }

      }

    }
    if ( !fileNameMI.empty() )
    {

      osMI.close();

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing average / median / std dev mutual information through slices
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "processing average/median/std.dev of MI : " << std::flush;

    }

    int32_t sliceCount = MI->getSize1();
    gkg::AverageFilter< std::vector< double >, double > averageFilter;
    gkg::MedianFilter< std::vector< double >, double > medianFilter;
    gkg::StdDevFilter< std::vector< double >, double > stdDevFilter;
    std::vector< double > values( inputOrientationCount );
    gkg::Vector averageMI( sliceCount );
    gkg::Vector medianMI( sliceCount );
    gkg::Vector stdDevMI( sliceCount );

    int32_t s = 0;
    int32_t o = 0;
    for ( s = 0; s < sliceCount; s++ )
    {

      for ( o = 0; o < inputOrientationCount; o++ )
      {

        values[ o ] = ( *MI )( s, o );

      }
      averageFilter.filter( values, averageMI( s ) );
      medianFilter.filter( values, medianMI( s ) );
      stdDevFilter.filter( values, stdDevMI( s ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

      for ( s = 0; s < sliceCount; s++ )
      {

        if ( averageMI( s ) >= 0.0 )
        {

          std::cout << "slice " << std::setw( 3 ) << s 
                    << " (mean/median/stddev) : "
                    << averageMI( s ) << " "
                    << medianMI( s ) << " "
                    << stdDevMI( s ) << std::endl;

        }
        else
        {

          std::cout << "slice " << std::setw( 3 ) << s 
                    << " (mean/median/stddev) : N/A"
                    << std::endl;

        }
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // determining (s,o) coordinates of outliers
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "processing outlier positions : " << std::flush;

    }

    std::set< OutlierPosition, OutlierPositionCompare > outlierPositions;

    for ( s = 0; s < sliceCount; s++ )
    {

      for ( o = 0; o < inputOrientationCount; o++ )
      {

        if ( ( *MI )( s, o ) >= 0.0 )
        {

          if ( std::abs( ( *MI )( s, o ) - averageMI( s ) ) >
               outlierFactor * stdDevMI( s ) )
          {

            outlierPositions.insert( OutlierPosition( s, o ) );

          }

        }

      }

    }


    if ( verbose )
    {

      std::cout << "done" << std::endl;

      if ( !outlierPositions.empty() )
      {

        std::cout << "outliers : " << std::flush;
        std::set< OutlierPosition, OutlierPositionCompare >::const_iterator
          op = outlierPositions.begin(),
          ope = outlierPositions.end();
        while ( op != ope )
        {

          std::cout << *op << " "; 
          ++ op;

        }
        std::cout << std::endl;

      }
      else
      {
      
        std::cout << "no outlier in this DW dataset" << std::endl;
      
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // buidling outlier python file
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameOutliers.empty() )
    {

      if ( verbose )
      {

        std::cout << "buidling outliers : " << std::flush;

      }
      if ( !outlierPositions.empty() )
      {
        int32_t outlierCount = ( int32_t )outlierPositions.size();

        std::vector< int32_t > zPositions( outlierCount );
        std::vector< int32_t > oPositions( outlierCount );

        std::set< OutlierPosition, OutlierPositionCompare >::const_iterator
          op = outlierPositions.begin(),
          ope = outlierPositions.end();
        int32_t count = 0;
        while ( op != ope )
        {

          zPositions[ count ] = op->slice;
          oPositions[ count ] = op->orientation;
          ++ op;
          ++ count;

        }

        std::ofstream os( fileNameOutliers.c_str() );

        os << "orientations = [ " << std::flush;
        int32_t outlier = 0;
        for ( outlier = 0; outlier < count - 1; outlier++ )
        {

          os << oPositions[ outlier ] << ", ";

        }
        os << oPositions[ outlier ] << " ] " << std::endl;
        os << "slices = [ " << std::flush;
        for ( outlier = 0; outlier < count - 1; outlier++ )
        {

          os << zPositions[ outlier ] << ", ";

        }
        os << zPositions[ outlier ] << " ] " << std::endl;
        os.close();

      }
      else
      {

        std::ofstream os( fileNameOutliers.c_str() );
        os << "orientations = []" << std::endl;
        os << "slices = []" << std::endl;
        os.close();

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // correcting outliers
    ////////////////////////////////////////////////////////////////////////////

    if ( !outlierPositions.empty() )
    {

      if ( verbose )
      {

        std::cout << "correcting outliers : " << std::flush;

      }

      correctionAlgorithmMap[ qSpaceSamplingName ]( outlierPositions,
                                                    t2,
                                                    dw,
		  	                            mask,
			                            qSpaceSampling,
			                            outlierFilterParameters,
		                                    sliceAxis,
                                                    verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving result
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving ' " << fileNameCorrectedDW << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameCorrectedDW,
                                      dw,
                                      ascii,
                                      volumeFormat );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


/*
    ////////////////////////////////////////////////////////////////////////////
    // correcting isolated outliers considering only slice position and
    // performing spatial reinterpolation
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "correcting spatially isolated outliers : " << std::flush;

    }

    // detecting isolated outliers
    std::set< OutlierPosition, OutlierPositionCompare >
      isolatedOutlierPositions;
    std::set< OutlierPosition, OutlierPositionCompare >::const_iterator
      op = outlierPositions.begin(),
      ope = outlierPositions.end();
    while ( op != ope )
    {

      if ( ( outlierPositions.find( OutlierPosition( op->slice - 1,
                                                     op->orientation ) ) ==
             outlierPositions.end() ) &&
           ( outlierPositions.find( OutlierPosition( op->slice + 1,
                                                     op->orientation ) ) ==
             outlierPositions.end() ) )
      {

        if ( correctIsolatedSlice( *op, dw, sliceAxis ) )
        {

          isolatedOutlierPositions.insert( *op );

        }

      }
      ++ op;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // removing isolated outliers from global outlier set
    if ( verbose )
    {

      std::cout << "corrected isolated outliers : " << std::flush;

    }
    std::set< OutlierPosition, OutlierPositionCompare >::const_iterator
      iop = isolatedOutlierPositions.begin(),
      iope = isolatedOutlierPositions.end();
    while ( iop != iope )
    {

      if ( verbose )
      {

        std::cout << *iop << " " ;

      }
      outlierPositions.erase( *iop );
      ++ iop;

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }

*/

  }
  GKG_CATCH( "void gkg::DwiOutlierFilterCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& sliceAxis, "
             "const std::string& fileNameCorrectedDW, "
             "const std::string& fileNameMI, "
             "const std::string& fileNameOutliers, "
             "double outlierFactor, "
             "float radius, "
             "float thresholdRatio, "
             "const std::vector< double >& outlierFilterParameters, "
             "const std::string& volumeFormat, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiOutlierFilterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( sliceAxis ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameCorrectedDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMI ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutliers ) +
    DECLARE_FLOATING_PARAMETER_HELP( outlierFactor ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( outlierFilterParameters ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );


#undef EPSILON
