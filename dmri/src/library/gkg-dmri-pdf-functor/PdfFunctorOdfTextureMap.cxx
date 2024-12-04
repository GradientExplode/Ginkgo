#include <gkg-dmri-pdf-functor/PdfFunctorOdfTextureMap.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>



gkg::PdfFunctorOdfTextureMap::PdfFunctorOdfTextureMap()
                             : gkg::PdfFunctor< gkg::TextureMap<
                                        gkg::OrientationDistributionFunction >,
                                        gkg::OrientationDistributionFunction >()
{
}


gkg::PdfFunctorOdfTextureMap::~PdfFunctorOdfTextureMap()
{
}


std::string gkg::PdfFunctorOdfTextureMap::getName() const
{

  try
  {

    return "odf_texture_map";

  }
  GKG_CATCH( "std::string gkg::PdfFunctorOdfTextureMap::getName() const" );

}


bool gkg::PdfFunctorOdfTextureMap::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorOdfTextureMap::isStandardBasisRequired() const" );

}


void gkg::PdfFunctorOdfTextureMap::process(
               gkg::TextureMap< gkg::OrientationDistributionFunction >& output,
               gkg::PdfCartesianField& pdfCartesianField,
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

    // sanity checks
    if ( stringParameters.size() != 5U )
    {

      throw std::runtime_error( "size of further string parameters must be 1" );

    }

    std::string odfType = stringParameters[ 4 ];

    if ( ( odfType != "shore-odf" ) &&
         ( odfType != "shell-odf" ) &&
         ( odfType != "shore-to-sh-odf" ) )
    {

      throw std::runtime_error( "unknown ODF type" );

    }


    if ( odfType == "shore-odf" )
    {

      // getting the target orientation count of the ODFs
      int32_t orientationCount = ( int32_t )( scalarParameters[ 5 ] + 0.5 );

      // creating the orientation set
      gkg::OrientationSet orientationSet( gkg::ElectrostaticOrientationSet(
                            orientationCount / 2 ).getSymmetricalOrientations() );

      // creating the map of ODFs
      std::map< int32_t, gkg::OrientationDistributionFunction > odfs;

      // filling the map
      gkg::TextureMap< gkg::ProbabilityDensityFunction >::const_iterator
        p = pdfCartesianField.getItems().begin(),
        pe = pdfCartesianField.getItems().end();
      while ( p != pe )
      {

        odfs[ p->first ] = p->second.getOrientationDistributionFunction(
                                                                 orientationSet );
        ++ p;

      }

      // filling the ODF texture map
      output.setTextures( odfs );


      // collecting the ODF type and adding it to the header of the texture map
      // of ODFs
      gkg::OrientationDistributionFunction::BasisType
        basisType = odfs.begin()->second.getBasisType();
      if ( basisType == gkg::OrientationDistributionFunction::Shore )
      {

        output.getHeader().addAttribute(
                                        "odf_type",
                                        std::string( "gkg_ShoreStdBasisOdf" ) );

      }

      // collecting PDF texture map resolution and adding it to the ODF
      // texture map
      gkg::Vector3d< double > resolution;
      resolution.x = pdfCartesianField.getResolutionX();
      resolution.y = pdfCartesianField.getResolutionY();
      resolution.z = pdfCartesianField.getResolutionZ();
      output.getHeader().addAttribute( "resolutionX", resolution.x );
      output.getHeader().addAttribute( "resolutionY", resolution.y );
      output.getHeader().addAttribute( "resolutionZ", resolution.z );

    }
    else if ( odfType == "shell-odf" )
    {

      // getting the target orientation count of the ODFs
      int32_t orientationCount = ( int32_t )( scalarParameters[ 5 ] + 0.5 );

      // getting shell displacement value 
      float shellDisplacement = ( float )scalarParameters[ 6 ]; 

      // getting maximum spherical harmonic order from scalar parameters
      int32_t maximumSphericalHarmonicOrder = 
                                     ( int32_t )( scalarParameters[ 7 ] + 0.5 );

      // getting regularizationLCurveFactor
      float regularizationLCurveFactor = ( float )scalarParameters[ 8 ];
 
      // creating the orientation set
      gkg::OrientationSet orientationSet( gkg::ElectrostaticOrientationSet(
  			  orientationCount / 2 ).getSymmetricalOrientations() );

      // creating the output single shell sampling
      std::map< float, gkg::OrientationSet >
        outputSingleShellSampling;
      outputSingleShellSampling[ shellDisplacement ] = orientationSet;

      // creating the map of ODFs
      std::map< int32_t, gkg::OrientationDistributionFunction > odfs;

      // getting a pointer to the numerical analysis factory
      gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      // processing maximum j according to maximum order
      int32_t N = ( ( maximumSphericalHarmonicOrder + 1 ) *
   	            ( maximumSphericalHarmonicOrder + 2 ) ) / 2;
		   
      // orientations count
      int32_t M = orientationSet.getCount();

      // building matrix of spherical harmonics on input orientation(s)
      gkg::Matrix B( M, N );
      gkg::Matrix Bplus( N, M );
      int32_t n, m;
      gkg::Vector3d< float > orientation;
      double radial;
      double theta;
      double phi;
      for ( m = 0; m < M; m++ )
      {

   	orientation = orientationSet.getOrientation( m );
   	factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
   		        			     ( double )orientation.y,
   	 					     ( double )orientation.z,
   						     radial,
   						     theta,
   						     phi );
   	for ( n = 0; n < N; n++ )
   	{

   	  B( m, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

   	}

      }

      // building Laplace-Beltrami smoothness matrix for regularization
      gkg::Matrix L( N, N );
      L.setZero();
      double sphericalLaplacian = 0;
      int32_t l;
      for ( n = 0; n < N; n++ )
      {

  	factory->getSymmetricalSphericalHarmonicLM( n, l, m );
  	sphericalLaplacian = - l * ( l + 1 );
  	L( n, n ) = sphericalLaplacian * sphericalLaplacian;

      }

      // building Bplus matrix with size N x M
      // Bplus = (B^T B + l L )^(-1) B^T
      Bplus.reallocate( N, M );
      factory->getThikonovPseudoInverse( B, L,
  					 ( double )regularizationLCurveFactor,
  					 Bplus );

      // filling the map
      gkg::TextureMap< gkg::ProbabilityDensityFunction >::const_iterator
  	p = pdfCartesianField.getItems().begin(),
  	pe = pdfCartesianField.getItems().end();
      while ( p != pe )
      {

        ProbabilityDensityFunction pdf( p->second );
        pdf.checkAndModifyOutputSampling( &outputSingleShellSampling );
        pdf.setOutputSampling( &outputSingleShellSampling );
  	odfs[ p->first ] = 
                 pdf.getOrientationDistributionFunctionFromSingleShell( Bplus );

  	++ p;

      }

      // filling the ODF texture map
      output.setTextures( odfs );


      // collecting the ODF type and adding it to the header of the texture map
      // of ODFs
      gkg::OrientationDistributionFunction::BasisType
  	basisType = odfs.begin()->second.getBasisType();
      if ( basisType ==
                      gkg::OrientationDistributionFunction::SphericalHarmonics )
      {

  	output.getHeader().addAttribute( "odf_type",
  					 std::string( "gkg_ShoreShellOdf" ) );

      }

      // collecting PDF texture map resolution and adding it to the ODF
      // texture map
      gkg::Vector3d< double > resolution;
      resolution.x = pdfCartesianField.getResolutionX();
      resolution.y = pdfCartesianField.getResolutionY();
      resolution.z = pdfCartesianField.getResolutionZ();
      output.getHeader().addAttribute( "resolutionX", resolution.x );
      output.getHeader().addAttribute( "resolutionY", resolution.y );
      output.getHeader().addAttribute( "resolutionZ", resolution.z );

    }
    else
    {

      // getting the target orientation count of the ODFs
      int32_t orientationCount = ( int32_t )( scalarParameters[ 5 ] + 0.5 );

      // creating the orientation set
      gkg::OrientationSet orientationSet( gkg::ElectrostaticOrientationSet(
                          orientationCount / 2 ).getSymmetricalOrientations() );

      // getting maximum spherical harmonic order from scalar parameters
      int32_t maximumSphericalHarmonicOrder =
                                     ( int32_t )( scalarParameters[ 6 ] + 0.5 );

      // getting regularizationLCurveFactor
      float regularizationLCurveFactor = ( float )scalarParameters[ 7 ];
 

      // creating the map of SHORE ODFs
      std::map< int32_t, gkg::OrientationDistributionFunction > odfs;

      // filling the map
      gkg::TextureMap< gkg::ProbabilityDensityFunction >::const_iterator
        p = pdfCartesianField.getItems().begin(),
        pe = pdfCartesianField.getItems().end();
      while ( p != pe )
      {

        odfs[ p->first ] = p->second.getOrientationDistributionFunction(
                                                               orientationSet );
        ++ p;

      }

      // getting a pointer to the numerical analysis factory
      gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      // processing maximum j according to maximum order
      int32_t N = ( ( maximumSphericalHarmonicOrder + 1 ) *
   	            ( maximumSphericalHarmonicOrder + 2 ) ) / 2;
		   
      // orientations count
      int32_t M = orientationSet.getCount();

      // building matrix of spherical harmonics on input orientation(s)
      gkg::Matrix B( M, N );
      gkg::Matrix Bplus( N, M );
      int32_t n, m;
      gkg::Vector3d< float > orientation;
      double radial;
      double theta;
      double phi;
      for ( m = 0; m < M; m++ )
      {

   	orientation = orientationSet.getOrientation( m );
   	factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
   		        			     ( double )orientation.y,
   	 					     ( double )orientation.z,
   						     radial,
   						     theta,
   						     phi );
   	for ( n = 0; n < N; n++ )
   	{

   	  B( m, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

   	}

      }

      // building Laplace-Beltrami smoothness matrix for regularization
      gkg::Matrix L( N, N );
      L.setZero();
      double sphericalLaplacian = 0;
      int32_t l;
      for ( n = 0; n < N; n++ )
      {

  	factory->getSymmetricalSphericalHarmonicLM( n, l, m );
  	sphericalLaplacian = - l * ( l + 1 );
  	L( n, n ) = sphericalLaplacian * sphericalLaplacian;

      }

      // building Bplus matrix with size N x M
      // Bplus = (B^T B + l L )^(-1) B^T
      Bplus.reallocate( N, M );
      factory->getThikonovPseudoInverse( B, L,
  					 ( double )regularizationLCurveFactor,
  					 Bplus );


      // creating the map of SHORE-TO-SH ODFs
      std::map< int32_t, gkg::OrientationDistributionFunction > shOdfs;

      // filling the map
      std::map< int32_t, gkg::OrientationDistributionFunction >::const_iterator
  	o = odfs.begin(),
  	oe = odfs.end();
      while ( o != oe )
      {

        gkg::OrientationDistributionFunction
          shOdf( gkg::OrientationDistributionFunction::SphericalHarmonics );
        shOdf.setValues( Bplus.getComposition( 
                                       gkg::Vector( o->second.getValues() ) ) );
  	shOdfs[ o->first ] = shOdf;

  	++ o;

      }

      // filling the ODF texture map
      output.setTextures( shOdfs );

      // collecting the ODF type and adding it to the header of the texture map
      // of ODFs
      gkg::OrientationDistributionFunction::BasisType
        basisType = odfs.begin()->second.getBasisType();
      if ( basisType == gkg::OrientationDistributionFunction::Shore )
      {

        output.getHeader().addAttribute(
                                        "odf_type",
                                        std::string( "gkg_ShoreToShOdf" ) );

      }

      // collecting PDF texture map resolution and adding it to the ODF
      // texture map
      gkg::Vector3d< double > resolution;
      resolution.x = pdfCartesianField.getResolutionX();
      resolution.y = pdfCartesianField.getResolutionY();
      resolution.z = pdfCartesianField.getResolutionZ();
      output.getHeader().addAttribute( "resolutionX", resolution.x );
      output.getHeader().addAttribute( "resolutionY", resolution.y );
      output.getHeader().addAttribute( "resolutionZ", resolution.z );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::PdfFunctorOdfTextureMap::process( "
             "gkg::TextureMap< gkg::OrientationDistributionFunction >& output, "
             "gkg::PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorOdfTextureMap __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::TextureMap<
                       gkg::OrientationDistributionFunction >,
                       gkg::OrientationDistributionFunction >::getInstance().
                                                           registerPdfFunctor(
                                 gkg::PdfFunctorOdfTextureMap::getInstance() );
