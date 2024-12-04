#include <gkg-processing-container/Curve3d.h>
#include <gkg-processing-numericalanalysis/CubicSpline3d.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/Writer_i.h>



int main()
{

  gkg::PluginLoader::getInstance().load();

  // in fact, 'gsl' library is the default; so the following line is not
  // mandatory: it is only for didactic purpose
  gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

  std::vector< gkg::Vector3d< float > > points( 36 );
  int32_t i = 0;
  for ( i = 0; i < 36; i++ )
  {

    points[ i ].x = std::sin( ( double )i * 2 * M_PI / 36.0 );
    points[ i ].y = std::cos( ( double )i * 2 * M_PI / 36.0 );
    points[ i ].z = 0.0;

  }

  gkg::Curve3d< float > curve3d( points );

  gkg::Writer::getInstance().write( "curve3d", curve3d, true,
                                    "curve3dmesh" );

  gkg::CubicSpline3d< float > spline3d( curve3d );

  std::cout << "spline3d.getLength()=" << spline3d.getLength() << std::endl;

  std::cout << "points[0]=" << points[ 0 ] << std::endl;
  std::cout << "spline3D(0)=" << spline3d.getValue( 0.0 ) << std::endl;
  
  std::cout << "points[35]=" << points[ 35 ] << std::endl;
  std::cout << "spline3D( " << spline3d.getLength()
            << " )=" << spline3d.getValue( spline3d.getLength() )
            << std::endl;
  
  gkg::LightCurve3d< float > lightCurve3d;
  spline3d.getInterpolatedCurve3d( lightCurve3d );

  gkg::Curve3d< float > interpolatedCurve3d( lightCurve3d );
  gkg::Writer::getInstance().write( "spline3d", interpolatedCurve3d, true,
                                    "curve3dmesh" );

  for ( i = 0; i < spline3d.getSplineSegment3dCount(); i++ )
  {

    std::cout << "point="
              << spline3d.getSplineSegment3d( i ).getValue( 0 )
              << " : curvature="
              << spline3d.getSplineSegment3d( i ).getCurvature( 0 )
              << std::endl;
    if ( i == spline3d.getSplineSegment3dCount() - 1 )
    {

      std::cout << "point=" 
                << spline3d.getSplineSegment3d( i ).getValue( 1 )
                << " : curvature="
                << spline3d.getSplineSegment3d( i ).getCurvature( 1 )
                << std::endl;

    }

  }

  return EXIT_SUCCESS;

}
