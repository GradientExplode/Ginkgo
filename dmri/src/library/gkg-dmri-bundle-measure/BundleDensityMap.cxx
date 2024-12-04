#include <gkg-dmri-bundle-measure/BundleDensityMap.h>
#include <gkg-dmri-bundle-measure/BundleMeasure_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-resampling/SpreaderFactory.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


#define DENSITY_FACTOR    10


gkg::BundleDensityMap::BundleDensityMap(
                              const gkg::Vector3d< int32_t >& size,
                              const gkg::Vector3d< double >& resolution )
                      : gkg::BundleMeasure< gkg::Volume< float > >(),
                        _size( size ),
                        _resolution( resolution )
{
}


gkg::BundleDensityMap::~BundleDensityMap()
{
}


gkg::Volume< float > gkg::BundleDensityMap::get(
                            const gkg::BundleDensityMap::Bundle& bundle ) const
{

  try
  {

    // creating and initializing density map
    gkg::Volume< float > densityMap( _size.x, _size.y, _size.z );
    densityMap.getHeader().addAttribute( "resolutionX", _resolution.x );
    densityMap.getHeader().addAttribute( "resolutionY", _resolution.y );
    densityMap.getHeader().addAttribute( "resolutionZ", _resolution.z );
    densityMap.fill( 0.0f );

    // collecting minimum resolution
    double minimumResolution = std::min( std::min( _resolution.x,
                                                   _resolution.y ),
                                         _resolution.z );

    // allocating spreader
    gkg::Spreader< float >*
      spreader = gkg::SpreaderFactory< float >::getInstance().getSpreader(
                                                 "Partial Voluming Spreader" );

    // looping over fiber(s)
    int32_t fiberCount = bundle.getCurve3dCount();
    int32_t f = 0;
    int32_t p = 0;
    int32_t pointCount = 0;
    gkg::LightCurve3d< float > equidistantFiber;
    float contribution = 1.0;
    for ( f = 0; f < fiberCount; f++ )
    {

      const gkg::LightCurve3d< float >& fiber = bundle.getCurve3d( f );
      pointCount = ( int32_t )( DENSITY_FACTOR * fiber.getLength() /
                                minimumResolution );
      equidistantFiber = fiber.getEquidistantCurve( pointCount );

      for ( p = 0; p < pointCount; p++ )
      {

        spreader->spread( contribution,
                          equidistantFiber.getPoint( p ),
                          densityMap,
                          &_resolution );

      }

    }
    return densityMap;

  }
  GKG_CATCH( "gkg::Volume< float > gkg::BundleDensityMap::get( "
             "const gkg::BundleDensityMap::Bundle& bundle ) const" );

}


#undef DENSITY_FACTOR
