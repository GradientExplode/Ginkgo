#include <gkg-dmri-pdf-shore/ShorePdfCartesianFieldFunctor.h>
#include <gkg-dmri-pdf-shore/ShorePdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-dmri-ph-basis/PolarHarmonicsCache.h>


gkg::ShorePdfCartesianFieldFunctor::ShorePdfCartesianFieldFunctor(
                     const gkg::ShorePdfCartesianField& shorePdfCartesianField )
     : gkg::DiffusionCartesianFieldFunctor< gkg::ProbabilityDensityFunction >(),
       _size( shorePdfCartesianField.getSizeX(),
              shorePdfCartesianField.getSizeY(),
              shorePdfCartesianField.getSizeZ() ),
       _resolution( shorePdfCartesianField.getResolutionX(),
                    shorePdfCartesianField.getResolutionY(),
                    shorePdfCartesianField.getResolutionZ() )
{

  try
  {

    // getting accessor to polar harmonics cache
    gkg::PolarHarmonicsCache&
      polarHarmonicsCache = gkg::PolarHarmonicsCache::getInstance();

    // ODF spherical harmonics
    _pdfPolarHarmonicsCoefficients.reallocate(
                            _size,
                            polarHarmonicsCache.getCoefficientCount() + 7 );
    _pdfPolarHarmonicsCoefficients.setResolution( _resolution );
 
    _pdfPolarHarmonicsCoefficients.fill( 0.0 );


  }
  GKG_CATCH( "gkg::ShorePdfCartesianFieldFunctor::"
             "ShorePdfCartesianFieldFunctor( "
             "const gkg::ShorePdfCartesianField& shorePdfCartesianField )" );

}

gkg::ShorePdfCartesianFieldFunctor::~ShorePdfCartesianFieldFunctor()
{
}


gkg::ProbabilityDensityFunction&
gkg::ShorePdfCartesianFieldFunctor::apply(
                                          const gkg::Vector3d< int32_t >& site,
                                          gkg::ProbabilityDensityFunction& pdf )
{

  try
  {

    gkg::ShorePdf* shorePdf = dynamic_cast< gkg::ShorePdf* >( &pdf );

    if ( shorePdf )
    {

      int32_t s;
      int32_t valueSize = _pdfPolarHarmonicsCoefficients.getSizeT();


      const std::vector< float >&
        values = shorePdf->getValues();

      for ( s = 0; s < valueSize; s++ )
      {

        _pdfPolarHarmonicsCoefficients( site, s ) = values[ s ];

      }

    }
    return pdf;

  }
  GKG_CATCH( "gkg:ProbabilityDensityFunction& "
             "gkg::ShorePdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:ProbabilityDensityFunction& pdf )" );

}


const gkg::Volume< float >&
gkg::ShorePdfCartesianFieldFunctor::getPdfPolarHarmonicsCoefficients() const
{

  try
  {

    return _pdfPolarHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::ShorePdfCartesianFieldFunctor::"
             "getPdfPolarHarmonicsCoefficients() const" );

}
