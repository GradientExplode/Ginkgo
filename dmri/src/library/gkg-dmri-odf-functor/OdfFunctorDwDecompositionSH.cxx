#include <gkg-dmri-odf-functor/OdfFunctorDwDecompositionSH.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-odf-aqbi/AnalyticalQBallOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sa-aqbi/SolidAngleAnalyticalQBallOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sdt/SDTFiberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sd/SDFiberOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorDwDecompositionSH::OdfFunctorDwDecompositionSH()
                                 : gkg::OdfFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::OdfFunctorDwDecompositionSH::~OdfFunctorDwDecompositionSH()
{
}


std::string gkg::OdfFunctorDwDecompositionSH::getName() const
{

  return "dw_sh";

}


bool gkg::OdfFunctorDwDecompositionSH::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorDwDecompositionSH::process(
                         gkg::Volume< float >& output,
                         OdfCartesianField& odfCartesianField,
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

    gkg::AnalyticalQBallOdfCartesianFieldFunctor*
      analyticalQBallOdfCartesianFieldFunctor =
      dynamic_cast< gkg::AnalyticalQBallOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor*
      solidAngleAnalyticalQBallOdfCartesianFieldFunctor =
      dynamic_cast< gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    gkg::SDTFiberOdfCartesianFieldFunctor* sdtFiberOdfCartesianFieldFunctor =
      dynamic_cast< gkg::SDTFiberOdfCartesianFieldFunctor* >(
                                       odfCartesianField.getFunctor() );

    gkg::SDFiberOdfCartesianFieldFunctor* sdFiberOdfCartesianFieldFunctor =
      dynamic_cast< gkg::SDFiberOdfCartesianFieldFunctor* >(
                                       odfCartesianField.getFunctor() );

    if ( analyticalQBallOdfCartesianFieldFunctor )
    {

      output = analyticalQBallOdfCartesianFieldFunctor->
                                          getDwSphericalHarmonicsCoefficients();

    }
    else if ( solidAngleAnalyticalQBallOdfCartesianFieldFunctor )
    {

      output = solidAngleAnalyticalQBallOdfCartesianFieldFunctor->
                                          getDwSphericalHarmonicsCoefficients();

    }
    else if ( sdtFiberOdfCartesianFieldFunctor )
    {

      output = sdtFiberOdfCartesianFieldFunctor->
                                          getDwSphericalHarmonicsCoefficients();

    }
    else if ( sdFiberOdfCartesianFieldFunctor )
    {

      output = sdFiberOdfCartesianFieldFunctor->
                                          getDwSphericalHarmonicsCoefficients();

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorDwDecompositionSH::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorDwDecompositionSH __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
                              gkg::OdfFunctorDwDecompositionSH::getInstance() );
