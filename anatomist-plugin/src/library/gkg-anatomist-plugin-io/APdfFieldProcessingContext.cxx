#include <gkg-anatomist-plugin-io/APdfFieldProcessingContext.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-algobase/MinMaxFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::APdfFieldProcessingContext::APdfFieldProcessingContext(
                       gkg::PdfCartesianField* pdfCartesianField,
                       std::vector< std::vector< float > >& transformations,
                       std::vector< gkg::Translation3d< float > >& translations,
                       float meshScale )
                                : gkg::LoopContext< int32_t >(),
                                  _pdfCartesianField( pdfCartesianField ),
                                  _transformations( transformations ),
                                  _translations( translations ),
                                  _meshScale( meshScale ),
                                  _resolution( 
                                            pdfCartesianField->getResolution() )
{

  try
  {

    std::cout << "resolution=" << _resolution << std::endl;

  }
  GKG_CATCH( "gkg::APdfFieldProcessingContext::APdfFieldProcessingContext( "
             "gkg::PdfCartesianField* pdfCartesianField, "
             "std::vector< std::vector< float > >& transformations, "
             "std::vector< gkg::Translation3d< float > >& translations, "
             "float meshScale )" );

}


gkg::APdfFieldProcessingContext::~APdfFieldProcessingContext()
{
}


void gkg::APdfFieldProcessingContext::doIt( int32_t startIndex,
                                            int32_t indexCount )
{

  try
  {

    // initializing iterator(s)
    int32_t rank = _pdfCartesianField->getSiteMap().getRank( 0 );
    const std::list< gkg::Vector3d< int32_t > >&
      sites = _pdfCartesianField->getSiteMap().getSites( rank );

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

    // preparing min max filter(s)
    gkg::MinMaxFilter< std::vector< float >, float > minMaxFilter;

    // looping over site(s)
    int32_t index = startIndex;
    std::vector< float > displacementProbabilities;
    std::pair< float, float > minMaxProbability( 0.0f, 0.0f );
    while ( s != se )
    {

      lock();
      const gkg::ProbabilityDensityFunction*
        item = _pdfCartesianField->getItem( *s );
      unlock();
            
      displacementProbabilities =  item->getDisplacementProbabilities();

      minMaxFilter.filter( displacementProbabilities, minMaxProbability );
                
      gkg::Rescaler< std::vector< float >, std::vector< float > >
        rescaler( minMaxProbability.first, minMaxProbability.second,
                  0.0, _meshScale );

      rescaler.rescale( displacementProbabilities, _transformations[ index ] );

      _translations[ index ].setDirectTranslation(
                                       ( float )s->x * ( float )_resolution.x,
                                       ( float )s->y * ( float )_resolution.y,
                                       ( float )s->z * ( float )_resolution.z );

      ++ s;
      ++ index;

    }

  }
  GKG_CATCH( "void APdfFieldProcessingContext::doIt("
             "int32_t startIndex,"
             "int32_t indexCount )" );
}
