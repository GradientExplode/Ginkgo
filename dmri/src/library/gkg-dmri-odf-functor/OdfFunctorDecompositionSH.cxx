#include <gkg-dmri-odf-functor/OdfFunctorDecompositionSH.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-odf-aqbi/AnalyticalQBallOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sa-aqbi/SolidAngleAnalyticalQBallOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sdt/SDTFiberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sd/SDFiberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-dsi/DsiOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-dot/DotOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>


gkg::OdfFunctorDecompositionSH::OdfFunctorDecompositionSH()
                               : gkg::OdfFunctor< gkg::Volume< float >,
                                                  float >()
{
}


gkg::OdfFunctorDecompositionSH::~OdfFunctorDecompositionSH()
{
}


std::string gkg::OdfFunctorDecompositionSH::getName() const
{

  return "odf_sh";

}


bool gkg::OdfFunctorDecompositionSH::isStandardBasisRequired() const
{

  return false;

}


gkg::Matrix gkg::OdfFunctorDecompositionSH::getReconstructionMatrix( 
                                     const gkg::OrientationSet& orientationSet )
{

  try
  {

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // processing maximum j according to maximum order
    int32_t N = 15;

    // measure count
    int32_t M = orientationSet.getCount();

    gkg::Matrix B( M, N );
    gkg::Matrix Bplus( N, M );

    int32_t n, o;
    gkg::Vector3d< float > orientation;
    double radial;
    double theta;
    double phi;
    for ( o = 0; o< M; o++ )
    {

      orientation = orientationSet.getOrientation( o );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      for ( n = 0; n < N; n++ )
      {

        B( o, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // building Bplus matrix with size N x M
    // Bplus = (B^T B)^(-1) B^T
    factory->getMoorePenrosePseudoInverse( B, Bplus );

    return Bplus;

  }
  GKG_CATCH( "gkg::Matrix gkg::OdfFunctorDecompositionSH "
	     "::getReconstructionMatrix("  
	     "const gkg::OrientationSet& orientationSet )" );

}

void gkg::OdfFunctorDecompositionSH::process(
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

    gkg::DsiOdfCartesianFieldFunctor* dsiOdfCartesianFieldFunctor =
      dynamic_cast< gkg::DsiOdfCartesianFieldFunctor* >( 
                                               odfCartesianField.getFunctor() );

    gkg::DotOdfCartesianFieldFunctor* dotOdfCartesianFieldFunctor =
      dynamic_cast< gkg::DotOdfCartesianFieldFunctor* >( 
                                               odfCartesianField.getFunctor() );

    if ( analyticalQBallOdfCartesianFieldFunctor )
    {

      output = analyticalQBallOdfCartesianFieldFunctor->
	                                 getOdfSphericalHarmonicsCoefficients();

    }
    else if ( solidAngleAnalyticalQBallOdfCartesianFieldFunctor )
    {

      output = solidAngleAnalyticalQBallOdfCartesianFieldFunctor->
	                                 getOdfSphericalHarmonicsCoefficients();

    }
    else if ( sdtFiberOdfCartesianFieldFunctor )
    {

      output = sdtFiberOdfCartesianFieldFunctor->
	                                 getOdfSphericalHarmonicsCoefficients();

    }
    else if ( sdFiberOdfCartesianFieldFunctor )
    {

      output = sdFiberOdfCartesianFieldFunctor->
                                         getOdfSphericalHarmonicsCoefficients();

    }
    else if ( dotOdfCartesianFieldFunctor )
    {

      output = dotOdfCartesianFieldFunctor->
                                         getOdfSphericalHarmonicsCoefficients();

    }
    else if ( dsiOdfCartesianFieldFunctor )
    {	
      
      std::cerr << "Warning!!!: Not possible to get the functor : odf_sh \n"; 

    }
    else
    {
    
      std::cerr << "Warning!!!: Not possible to get the functor : odf_sh \n"; 

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorDecompositionSH::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorDecompositionSH __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
                                gkg::OdfFunctorDecompositionSH::getInstance() );
