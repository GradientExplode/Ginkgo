#include <gkg-dmri-pdf-dti/TensorFunctorDiffusionWeightedT2.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorDiffusionWeightedT2::TensorFunctorDiffusionWeightedT2()
                     : gkg::TensorFunctor< gkg::Volume< float >, float >()
{
}


gkg::TensorFunctorDiffusionWeightedT2::~TensorFunctorDiffusionWeightedT2()
{
}


std::string gkg::TensorFunctorDiffusionWeightedT2::getName() const
{

  return "diffusion_weighted_t2";

}


void gkg::TensorFunctorDiffusionWeightedT2::process(
                            gkg::Volume< float >& output,
                            gkg::TensorCartesianField& tensorCartesianField,
                            const std::vector< double >& /*scalarParameters*/,
                            const std::vector< std::string >& stringParameters,
                            bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    // getting site count
    int32_t siteCount = tensorCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = tensorCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = tensorCartesianField.getOffset();

    // getting b-value
    std::vector< double > newScalarParameters( 2 );
    newScalarParameters[ 1 ] = tensorCartesianField.getBValues()[ 0 ];

    // getting pointer to T2 image
    const Volume< float >& t2Volume = tensorCartesianField.getT2Volume();

    // preparing ouput volume
    int32_t sizeX = tensorCartesianField.getSizeX();
    int32_t sizeY = tensorCartesianField.getSizeY();
    int32_t sizeZ = tensorCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = tensorCartesianField.getResolutionX();
    resolution.y = tensorCartesianField.getResolutionY();
    resolution.z = tensorCartesianField.getResolutionZ();

    output.reallocate( sizeX, sizeY, sizeZ );
    output.getHeader().addAttribute( "resolutionX", resolution.x );
    output.getHeader().addAttribute( "resolutionY", resolution.y );
    output.getHeader().addAttribute( "resolutionZ", resolution.z );

    output.fill( 0.0f );

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << count + 1 
                    << " / " << std::setw( 7 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      newScalarParameters[ 0 ] = t2Volume( *s );
      this->processItem( output( *s - offset ),
                         *tensorCartesianField.getItem( *s ),
                         newScalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void TensorFunctor< Volume< float >, float >::process( "
             "Volume< float >& output, "
             "TensorCartesianField& tensorCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



void gkg::TensorFunctorDiffusionWeightedT2::processItem(
                 float& item,
                 const gkg::Tensor& tensor,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    float t2 = scalarParameters[ 0 ];
    float bValue = scalarParameters[ 1 ];
    float adc = tensor.getApparentDiffusionCoefficient();

    item = t2 * std::exp( - bValue * adc * 1e6 );

  }
  GKG_CATCH( "void gkg::TensorFunctorDiffusionWeightedT2::processItem( "
             "float& item, "
             "const gkg::Tensor& tensor, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorDiffusionWeightedT2
                                                       __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                          registerTensorFunctor(
                       gkg::TensorFunctorDiffusionWeightedT2::getInstance() );
