#include <gkg-dmri-pdf-functor/PdfFunctorRGB.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfFunctorRGB::PdfFunctorRGB()
                   : gkg::PdfFunctor< gkg::Volume< gkg::RGBComponent >,
                                      RGBComponent >()
{
}


gkg::PdfFunctorRGB::~PdfFunctorRGB()
{
}


std::string gkg::PdfFunctorRGB::getName() const
{

  try
  {

    return "rgb";

  }
  GKG_CATCH( "std::string "
             "gkg::PdfFunctorRGB::getName() const" );

}


bool gkg::PdfFunctorRGB::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorRGB::"
             "isStandardBasisRequired() "
             "const" );

}



void gkg::PdfFunctorRGB::process(
               gkg::Volume< gkg::RGBComponent >& output,
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

    float rgbScale = scalarParameters[ 0 ];

    // getting site count
    int32_t siteCount = pdfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = pdfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = pdfCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = pdfCartesianField.getSizeX();
    int32_t sizeY = pdfCartesianField.getSizeY();
    int32_t sizeZ = pdfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = pdfCartesianField.getResolutionX();
    resolution.y = pdfCartesianField.getResolutionY();
    resolution.z = pdfCartesianField.getResolutionZ();

    output.reallocate( sizeX, sizeY, sizeZ );
    output.getHeader().addAttribute( "resolutionX", resolution.x );
    output.getHeader().addAttribute( "resolutionY", resolution.y );
    output.getHeader().addAttribute( "resolutionZ", resolution.z );

    output.fill( RGBComponent( 0, 0, 0 ) );


    if ( odfType == "shore-odf" )
    {

      // getting the target orientation count of the ODFs
      int32_t orientationCount = ( int32_t )( scalarParameters[ 5 ] + 0.5 );

      // creating the orientation set
      gkg::OrientationSet orientationSet( gkg::ElectrostaticOrientationSet(
                            orientationCount / 2 ).getSymmetricalOrientations() );

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

        gkg::OrientationDistributionFunction 
             odf( pdfCartesianField.getItem( *s )->
                         getOrientationDistributionFunction( orientationSet ) );

        float scaling = odf.getGeneralizedFractionalAnisotropy() *
                        rgbScale;

        gkg::Vector3d< float > 
          principalOrientation = odf.getPrincipalOrientation();
        float r = std::abs( principalOrientation.x ) * 255.0 * scaling;
        float g = std::abs( principalOrientation.y ) * 255.0 * scaling;
        float b = std::abs( principalOrientation.z ) * 255.0 * scaling;
        r = ( r > 255.0 ? 255.0 : r );
        g = ( g > 255.0 ? 255.0 : g );
        b = ( b > 255.0 ? 255.0 : b );

        output( *s - offset ) = gkg::RGBComponent( ( uint8_t )r,
                                                   ( uint8_t )g,
                                                   ( uint8_t )b );

        ++ s;
        ++ count;

      }
      if ( verbose )
      {

        std::cout << Eraser( 22 ) << "done" << std::endl;

      }

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

        ProbabilityDensityFunction pdf( *pdfCartesianField.getItem( *s ) );
        pdf.checkAndModifyOutputSampling( &outputSingleShellSampling );
        pdf.setOutputSampling( &outputSingleShellSampling );

        gkg::OrientationDistributionFunction 
          odf( pdf.getOrientationDistributionFunctionFromSingleShell( Bplus ) );

        float scaling = odf.getGeneralizedFractionalAnisotropy() *
                        rgbScale;

        gkg::Vector3d< float > 
          principalOrientation = odf.getPrincipalOrientation();
        float r = std::abs( principalOrientation.x ) * 255.0 * scaling;
        float g = std::abs( principalOrientation.y ) * 255.0 * scaling;
        float b = std::abs( principalOrientation.z ) * 255.0 * scaling;
        r = ( r > 255.0 ? 255.0 : r );
        g = ( g > 255.0 ? 255.0 : g );
        b = ( b > 255.0 ? 255.0 : b );

        output( *s - offset ) = gkg::RGBComponent( ( uint8_t )r,
                                                   ( uint8_t )g,
                                                   ( uint8_t )b );

        ++ s;
        ++ count;

      }
      if ( verbose )
      {

        std::cout << Eraser( 22 ) << "done" << std::endl;

      }

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
        gkg::OrientationDistributionFunction 
             odf( pdfCartesianField.getItem( *s )->
                         getOrientationDistributionFunction( orientationSet ) );

        gkg::OrientationDistributionFunction
          shOdf( gkg::OrientationDistributionFunction::SphericalHarmonics,
                 &orientationSet );
        shOdf.setValues( Bplus.getComposition( 
                                       gkg::Vector( odf.getValues() ) ) );

        float scaling = shOdf.getGeneralizedFractionalAnisotropy() *
                        rgbScale;

        gkg::Vector3d< float > 
          principalOrientation = shOdf.getPrincipalOrientation();
        float r = std::abs( principalOrientation.x ) * 255.0 * scaling;
        float g = std::abs( principalOrientation.y ) * 255.0 * scaling;
        float b = std::abs( principalOrientation.z ) * 255.0 * scaling;
        r = ( r > 255.0 ? 255.0 : r );
        g = ( g > 255.0 ? 255.0 : g );
        b = ( b > 255.0 ? 255.0 : b );

        output( *s - offset ) = gkg::RGBComponent( ( uint8_t )r,
                                                   ( uint8_t )g,
                                                   ( uint8_t )b );

        ++ s;
        ++ count;

      }
      if ( verbose )
      {

        std::cout << Eraser( 22 ) << "done" << std::endl;

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::PdfFunctorRGB::process( "
             "gkg::Volume< gkg::RGBComponent >& output, "
             "gkg::PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// PDF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorRGB __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                        gkg::RGBComponent >::getInstance().registerPdfFunctor(
                                            gkg::PdfFunctorRGB::getInstance() );
