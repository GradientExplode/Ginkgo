#include <gkg-dmri-pdf-functor/PdfFunctorPdfTextureMap.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfFunctorPdfTextureMap::PdfFunctorPdfTextureMap()
                             : gkg::PdfFunctor< gkg::TextureMap<
                                             gkg::ProbabilityDensityFunction >,
                                             gkg::ProbabilityDensityFunction >()
{
}


gkg::PdfFunctorPdfTextureMap::~PdfFunctorPdfTextureMap()
{
}


std::string gkg::PdfFunctorPdfTextureMap::getName() const
{

  try
  {

    return "pdf_texture_map";

  }
  GKG_CATCH( "std::string gkg::PdfFunctorPdfTextureMap::getName() const" );

}


bool gkg::PdfFunctorPdfTextureMap::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorPdfTextureMap::isStandardBasisRequired() const" );

}


void gkg::PdfFunctorPdfTextureMap::process(
               gkg::TextureMap< gkg::ProbabilityDensityFunction >& output,
               gkg::PdfCartesianField& pdfCartesianField,
               const std::vector< double >& /*scalarParameters*/,
               const std::vector< std::string >& /*stringParameters*/,
               bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    //pdfCartesianField.setStandardBasis();
    output = pdfCartesianField.getItems();


    std::string basisTypeName = pdfCartesianField.getBasisTypeName();

    output.getHeader().addAttribute( "basis_type", basisTypeName );

    if ( basisTypeName == "std" )
    {

      gkg::Dictionary displacementSpaceSampling;

      if ( pdfCartesianField.hasMultipleShellOutputSampling() )
      {

        displacementSpaceSampling[ "type" ] = std::string( "multiple_shell" );

      }
      else
      {

        displacementSpaceSampling[ "type" ] = std::string( "cartesian" );

      }

      gkg::Dictionary parameters;
      if ( pdfCartesianField.hasMultipleShellOutputSampling() )
      {

        const std::map< float, OrientationSet >& 
          outputMultipleShellSampling = 
                             pdfCartesianField.getOutputMultipleShellSampling();

        int32_t shellCount = ( int32_t )outputMultipleShellSampling.size();
        std::vector< double > displacementMagnitudes( shellCount );
        gkg::GenericObjectList orientationSets( shellCount );

        std::map< float, OrientationSet >::const_iterator
          s = outputMultipleShellSampling.begin(),
          se = outputMultipleShellSampling.end();

        int32_t count = 0;
        while ( s != se )
        {

          displacementMagnitudes[ count ] = ( double )s->first;

          const gkg::OrientationSet& orientationSet = s->second;
          int32_t orientationCount = orientationSet.getCount();

          gkg::GenericObjectList golOrientationSet( orientationCount );
          int32_t o = 0;
          for ( o = 0; o < orientationCount; o++ )
          {

            gkg::GenericObjectList orientation( 3 );
            const gkg::Vector3d< float >& 
              theOrientation = orientationSet.getOrientation( o );
            orientation[ 0 ] = theOrientation.x;
            orientation[ 1 ] = theOrientation.y;
            orientation[ 2 ] = theOrientation.z;
            golOrientationSet[ o ] = orientation;


          }

          orientationSets[ count ] = golOrientationSet;

          ++ s;
          ++ count;

        }
        
        parameters[ "displacement_magnitudes" ] = displacementMagnitudes;
        parameters[ "orientation_sets" ] = orientationSets;


      }
      else
      {

        const gkg::Vector3d< int32_t >& 
          outputCartesianGridSize = 
                                 pdfCartesianField.getOutputCartesianGridSize();
        const gkg::Vector3d< double >&
          outputCartesianGridResolution =
                           pdfCartesianField.getOutputCartesianGridResolution();

        gkg::GenericObjectList cartesianGridSize( 3 );
        cartesianGridSize[ 0 ] = outputCartesianGridSize.x;
        cartesianGridSize[ 1 ] = outputCartesianGridSize.y;
        cartesianGridSize[ 2 ] = outputCartesianGridSize.z;

        gkg::GenericObjectList cartesianGridResolution( 3 );
        cartesianGridResolution[ 0 ] = outputCartesianGridResolution.x;
        cartesianGridResolution[ 1 ] = outputCartesianGridResolution.y;
        cartesianGridResolution[ 2 ] = outputCartesianGridResolution.z;

        parameters[ "grid_size" ] = cartesianGridSize;
        parameters[ "grid_resolution" ] = cartesianGridResolution;

      }
      displacementSpaceSampling[ "parameters" ] = parameters;


      output.getHeader().addAttribute( "displacement_space_sampling",
                                       displacementSpaceSampling );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::PdfFunctorPdfTextureMap::process( "
             "gkg::TextureMap< gkg::ProbabilityDensityFunction >& output, "
             "gkg::PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorPdfTextureMap __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::TextureMap<
                       gkg::ProbabilityDensityFunction >,
                       gkg::ProbabilityDensityFunction >::getInstance().
                                                           registerPdfFunctor(
                                 gkg::PdfFunctorPdfTextureMap::getInstance() );
