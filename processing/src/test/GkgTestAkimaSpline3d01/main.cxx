#include <gkg-processing-container/Curve3d.h>
#include <gkg-processing-numericalanalysis/CubicSpline3d.h>
#include <gkg-processing-numericalanalysis/AkimaSpline3d.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/Writer_i.h>



int main()
{

  gkg::PluginLoader::getInstance().load();

  // in fact, 'gsl' library is the default; so the following line is not
  // mandatory: it is only for didactic purpose
  gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

  std::vector< gkg::Vector3d< float > > points( 9 );

  points[ 0 ].x = -1.0;
  points[ 0 ].y = 0.0;
  points[ 0 ].z = 0.0;
  points[ 1 ].x = -0.75;
  points[ 1 ].y = 0.4;
  points[ 1 ].z = 0.0;
  points[ 2 ].x = -0.5;
  points[ 2 ].y = 0.7;
  points[ 2 ].z = 0.0;
  points[ 3 ].x = -0.25;
  points[ 3 ].y = 0.8;
  points[ 3 ].z = 0.0;
  points[ 4 ].x = 0.0;
  points[ 4 ].y = 0.85;
  points[ 4 ].z = 0.0;
  points[ 5 ].x = 0.25;
  points[ 5 ].y = 0.8;
  points[ 5 ].z = 0.0;
  points[ 6 ].x = 0.5;
  points[ 6 ].y = 0.7;
  points[ 6 ].z = 0.0;
  points[ 7 ].x = 0.75;
  points[ 7 ].y = 0.4;
  points[ 7 ].z = 0.0;
  points[ 8 ].x = 1.0;
  points[ 8 ].y = 0.0;
  points[ 8 ].z = 0.0;

  gkg::Curve3d< float > curve3d( points );

  gkg::Writer::getInstance().write( "model3d", curve3d, true,
                                    "curve3dmesh" );

  points[ 3 ].y = 3.0;
  curve3d.setPoint( 3, points[ 3 ] );

  gkg::Writer::getInstance().write( "curve3d", curve3d, true,
                                    "curve3dmesh" );

  gkg::CubicSpline3d< float > spline3d( curve3d );

  gkg::LightCurve3d< float > lightCurve3d;
  spline3d.getInterpolatedCurve3d( lightCurve3d, 30 );

  gkg::Curve3d< float > interpolatedCurve3d( lightCurve3d );
  gkg::Writer::getInstance().write( "spline3d", interpolatedCurve3d, true,
                                    "curve3dmesh" );

  gkg::AkimaSpline3d< float > akima3d( curve3d );
  akima3d.getInterpolatedCurve3d( lightCurve3d, 30 );

  interpolatedCurve3d = gkg::Curve3d< float >( lightCurve3d );

  gkg::Writer::getInstance().write( "akima3d", interpolatedCurve3d, true,
                                    "curve3dmesh" );

  return EXIT_SUCCESS;

}
