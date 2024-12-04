#include <gkg-anatomist-plugin-io/ATwoPoolModelFieldProcessingContext.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::ATwoPoolModelFieldProcessingContext::ATwoPoolModelFieldProcessingContext(
               gkg::TwoPoolModelCartesianField* twoPoolModelCartesianField,
               std::vector< std::vector< float > >& fastTransformations,
               std::vector< std::vector< float > >& slowTransformations,
               std::vector< gkg::Translation3d< float > >& translations,
                       float meshScale )
                                : gkg::LoopContext< int32_t >(),
                                  _twoPoolModelCartesianField(
                                                   twoPoolModelCartesianField ),
                                  _fastTransformations( fastTransformations ),
                                  _slowTransformations( slowTransformations ),
                                  _translations( translations ),
                                  _meshScale( meshScale ),
                                  _resolution( 
                                   twoPoolModelCartesianField->getResolution() )
{

  std::cout << "resolution=" << _resolution << std::endl;

}


gkg::ATwoPoolModelFieldProcessingContext::~ATwoPoolModelFieldProcessingContext()
{
}


void gkg::ATwoPoolModelFieldProcessingContext::doIt( int32_t startIndex,
                                                     int32_t indexCount )
{

  try
  {

    // initializing iterator(s)
    int32_t rank = _twoPoolModelCartesianField->getSiteMap().getRank( 0 );
    const std::list< gkg::Vector3d< int32_t > >&
      sites = _twoPoolModelCartesianField->getSiteMap().getSites( rank );

    int32_t siteCount = ( int32_t )sites.size();

    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.begin();
    advance( s, startIndex );
    if ( startIndex + indexCount != siteCount )
    {

      advance( se, ( startIndex + indexCount + 1 ) );

    }
    else
    {

      se = sites.end();

    }

    // preparing max filter
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;

    // looping over site(s)
    int32_t index = startIndex;
    std::vector< float > fastOrientationProbabilities;
    std::vector< float > slowOrientationProbabilities;
    float fastMaximumProbability = 0.0;
    float slowMaximumProbability = 0.0;
    float maximumProbability = 0.0;
    while ( s != se )
    {

      lock();
      const gkg::TwoPoolModel*
        item = _twoPoolModelCartesianField->getItem( *s );
      unlock();
            
      fastOrientationProbabilities =
                   item->getOrientationProbabilities( gkg::TwoPoolModel::Fast );

      maximumFilter.filter( fastOrientationProbabilities,
                            fastMaximumProbability );

      slowOrientationProbabilities =
                   item->getOrientationProbabilities( gkg::TwoPoolModel::Slow );

      maximumFilter.filter( slowOrientationProbabilities,
                            slowMaximumProbability );

      maximumProbability = std::max( fastMaximumProbability,
                                     slowMaximumProbability );

      gkg::Rescaler< std::vector< float >, std::vector< float > >
        rescaler( 0.0, maximumProbability,
                  0.0, _meshScale );

      rescaler.rescale( fastOrientationProbabilities,
                        _fastTransformations[ index ] );

      rescaler.rescale( slowOrientationProbabilities,
                        _slowTransformations[ index ] );

      _translations[ index ].setDirectTranslation(
                                       ( float )s->x * ( float )_resolution.x,
                                       ( float )s->y * ( float )_resolution.y,
                                       ( float )s->z * ( float )_resolution.z );


      ++ s;
      ++ index;

    }

  }
  GKG_CATCH( "void ProcessOdfContext::doIt("
             "int32_t startIndex,"
             "int32_t indexCount )" );
}
