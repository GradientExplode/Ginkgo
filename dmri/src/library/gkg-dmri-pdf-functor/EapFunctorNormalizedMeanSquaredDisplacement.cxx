#include <gkg-dmri-pdf-functor/EapFunctorNormalizedMeanSquaredDisplacement.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


#define CUMULATED_HISTOGRAM_BIN_COUNT 1000
#define CUMULATED_HISTOGRAM_THRESHOLD 0.95


gkg::EapFunctorNormalizedMeanSquaredDisplacement::EapFunctorNormalizedMeanSquaredDisplacement()
                             : gkg::EapFunctor< gkg::Volume< float >, float >()
{
}


gkg::EapFunctorNormalizedMeanSquaredDisplacement::~EapFunctorNormalizedMeanSquaredDisplacement()
{
}


std::string gkg::EapFunctorNormalizedMeanSquaredDisplacement::getName() const
{

  return "normalized_mean_squared_displacement";

}


bool gkg::EapFunctorNormalizedMeanSquaredDisplacement::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorNormalizedMeanSquaredDisplacement::process(
                             gkg::Volume< float >& output,
                             gkg::EapCartesianField& odfCartesianField,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
          getEapFunctor( "mean_squared_displacement" ).process( output,
                                                       odfCartesianField,
                                                       scalarParameters,
                                                       stringParameters,
                                                       false );

    // getting offset
    Vector3d< int32_t > 
      offset = odfCartesianField.getOffset();

    // processing MeanSquaredDisplacement boundaries
    float lowerBoundary = 1.0;
    float upperBoundary = 0.0;
    float value = 0.0;
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = odfCartesianField.getSiteMap().getSites( 0 ).begin(),
      se = odfCartesianField.getSiteMap().getSites( 0 ).end();
    while ( s != se )
    {

      value = output( *s - offset );
      if ( value < lowerBoundary )
      {

        lowerBoundary = value;

      }
      else if ( value > upperBoundary )
      {

        upperBoundary = value;

      }
      ++ s;

    }

    // processing MeanSquaredDisplacement cumulated histogram
    std::vector< float > cumulatedHistogram( CUMULATED_HISTOGRAM_BIN_COUNT );
    s = odfCartesianField.getSiteMap().getSites( 0 ).begin();
    int32_t level = 0;
    float sum = 0;
    while ( s != se )
    {

      value = output( *s - offset );
      level = ( int32_t )( ( value - lowerBoundary ) *
                           ( float )( CUMULATED_HISTOGRAM_BIN_COUNT - 1 ) /
                           ( upperBoundary - lowerBoundary ) );
      cumulatedHistogram[ level ] ++;
      ++ sum;
      ++ s;

    }

    // normalizing histogram
    if ( sum > 0.0 )
    {

      std::vector< float >::iterator h = cumulatedHistogram.begin(),
                                     he = cumulatedHistogram.end();
      while ( h != he )
      {

        *h /= sum;
        ++ h;

      }

    }
    size_t h;
    for ( h = 1; h < cumulatedHistogram.size(); h++ )
    {

      cumulatedHistogram[ h ] += cumulatedHistogram[ h - 1 ];

    }

    // processing MeanSquaredDisplacement rescaler boundaries
    level = CUMULATED_HISTOGRAM_BIN_COUNT - 1;
    while ( ( level >= 0 ) &&
            ( cumulatedHistogram[ level ] > CUMULATED_HISTOGRAM_THRESHOLD ) )
    {

      -- level;

    }
    float meanDisplacementLowerBoundary = lowerBoundary;
    float meanDisplacementUpperBoundary = lowerBoundary + ( float )level * 
                             ( upperBoundary - lowerBoundary ) /
                             ( float )( CUMULATED_HISTOGRAM_BIN_COUNT - 1 );

    gkg::Rescaler< gkg::Volume< float >,
                   gkg::Volume< float > > rescaler( meanDisplacementLowerBoundary,
                                                    meanDisplacementUpperBoundary,
                                                    0.0,
                                                    1.0 );
    rescaler.rescale( output, output );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::EapFunctorNormalizedMeanSquaredDisplacement::process( "
             "gkg::Volume< float >& output, "
             "gkg::EapCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );


}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_EapFunctorNormalizedMeanSquaredDisplacement 
                                                       __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerEapFunctor(
              gkg::EapFunctorNormalizedMeanSquaredDisplacement::getInstance() );


#undef CUMULATED_HISTOGRAM_BIN_COUNT
#undef CUMULATED_HISTOGRAM_THRESHOLD
