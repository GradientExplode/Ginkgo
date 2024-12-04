#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-odf-dsi/DsiOdfCartesianField.h>
#include <gkg-dmri-odf-dot/DotOdfCartesianField.h>
#include <gkg-dmri-io/OdfFunctorWriter.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <vector>
#include <list>

int main( int32_t argc, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;
    
  try
  {
    
    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMask = "";
    std::string methodType;
    std::list< std::string > odfFunctorNames;
    std::list< std::string > outputFileNames;
    std::vector< int32_t > iBox;
    int32_t outputOrientationCount = 500;
    float radius = -1.0;
    float thresholdRatio = 0.04;
    std::string volumeFormat = "gis";
    std::string meshMapFormat = "aimsmesh";
    std::string textureMapFormat = "aimstexture";
    float rgbScale = 1.0;
    float meshScale = 1.0;
    float lowerGFAThreshold = 0.0;
    float upperGFAThreshold = 1.0;
    //bool meshOpacity = false;
    //std::vector< float > meshRadii;
    std::vector< double > specificScalarParameters;
    std::vector< std::string > specificStringParameters;
    std::vector< double > furtherScalarParameters;
    std::vector< std::string > furtherStringParameters;
    bool ascii = false;
    bool verbose = false;
    bool dsi = true;

    ////////////////////////////////////////////////////////////////////////////
    // building ODF functor list
    ////////////////////////////////////////////////////////////////////////////

    std::string odfFunctorText = "ODF functor list among :\n";
    std::list< std::string >
      allOdfFunctorNames = gkg::OdfFunctorWriter::getNames();
    std::list< std::string >::const_iterator
      f = allOdfFunctorNames.begin(),
      fe = allOdfFunctorNames.end();
    while ( f != fe )
    {

      odfFunctorText += "- " + *f + "\n";
      ++ f;

    }

    ////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////
    gkg::Application application( argc, argv, "Test Diffusion Spectrum Imaging" );
    application.addSingleOption( "-t2",
                                 "Input T2 file name",
                                 fileNameT2 );
    application.addSingleOption( "-dw",
                                 "Input DW file name",
                                 fileNameDW );
    application.addSingleOption( "-m",
                                 "Input mask file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-t",
                                 "DSI (true) or DOT(false)",
                                 dsi,
                                 true );
    application.addSeriesOption( "-f",
                                 odfFunctorText,
                                 odfFunctorNames,
                                 1 );
    application.addSeriesOption( "-o",
                                 "Output file name(s)",
                                 outputFileNames,
                                 1 );
    application.addSeriesOption( "-ibox",
                                 "Integer bounding box on which to compute"
                                 " the EAP field",
                                 iBox );
    application.addSingleOption( "-outputOrientationCount",
                                 "Output orientation count "
                                 "(default=symmetrical input orientation set)",
                                 outputOrientationCount,
                                 true );
    application.addSingleOption( "-radius",
                                 "Radius of morphological operation for mask "
                                 "(default=2*min. res.)",
                                 radius,
                                 true );
    application.addSingleOption( "-thresholdRatio",
                                 "Threshold ratio for mask (default=0.04)",
                                 thresholdRatio,
                                 true );
    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
                                 true );
    application.addSingleOption( "-meshMapFormat",
                                 "Ouput MeshMap format (default=aimsmesh)",
                                 meshMapFormat,
                                 true );
    application.addSingleOption( "-textureMapFormat",
                                 "Ouput MeshMap format (default=aimstexture)",
                                 textureMapFormat,
                                 true );
    application.addSeriesOption( 
			    "-scalarParameters",
			    "DSI cartesian field specific parameters as "
			    "a vector of double <P1> <P2> ... <Pn>:"
			    "\n"
			    ".  <P1>: Filtering data or not before FFT\n"
			    ".  <P2>: Minimum R0 for ODF computation (in [0,8])\n"
			    ".  <P3>: Maximum R0 for ODF computation (in [0,8])\n"
			    ".  <P4>: Marginal ODF or not\n"
			    "        (default= 1 / 2 / 6 / 1 )"
			    "\n\n"
			    "DOT cartesian field specific parameters as "
			    "a vector of double <P1> <P2> ... <Pn>:"
			    "\n"
			    ".  <P1>: maximum spherical harmonic order\n"
			    ".  <P2>: effective diffusion time (ms) \n"
			    ".  <P3>: R0 for DOT/ODF computation (um) \n"
			    ".  <P4>: ODF computation or not\n"
			    "        (default= 4 / 25 / 12 / 1 )"
			    "\n\n",
			    specificScalarParameters );
    application.addSeriesOption("-stringParameters",
				"DSI cartesian field specific parameters as "
				"a vector of string <P1> <P2> ... <Pn>:"
				"\n"
				"- in case of dpi_eap_cartesian_field,\n"
				".  No string parameter currently needed..."
				"\n",
				specificStringParameters );
    application.addSeriesOption( "-furtherScalarParameters",
                                 "Further scalar parameters to be used by "
                                 "functors",
                                 furtherScalarParameters );
    application.addSeriesOption( "-furtherStringParameters",
                                 "Further string parameters to be used by "
                                 "functors",
                                 furtherStringParameters );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////
    if ( odfFunctorNames.size() != outputFileNames.size() )
    {

      throw std::runtime_error( "different ODF functor and file name count" );

    }

    if ( outputOrientationCount < 0 )
    {

      throw std::runtime_error( "output orientation count must be positive" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading T2 data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameT2 << "' : " << std::flush;

    }
    gkg::RCPointer< gkg::Volume< float > > t2( new gkg::Volume< float > );
    gkg::TypedVolumeReaderProcess< float > t2VolumeReaderProcess( *t2 );
    t2VolumeReaderProcess.execute( fileNameT2 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading DW data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameDW << "' : " << std::flush;

    }
    gkg::RCPointer< gkg::Volume< float > > dw( new gkg::Volume< float > );
    gkg::TypedVolumeReaderProcess< float > dwVolumeReaderProcess( *dw );
    dwVolumeReaderProcess.execute( fileNameDW );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // making or reading mask
    ////////////////////////////////////////////////////////////////////////////
    gkg::RCPointer< gkg::Volume< short > > mask( new gkg::Volume< int16_t > );
    if ( fileNameMask.empty() )
    {

      if ( verbose )
      {

        std::cout << "building mask over T2 data : " << std::flush;

      }
      if ( radius <= 0 )
      {

        if ( t2->getHeader().hasAttribute( "resolutionX" ) &&
             t2->getHeader().hasAttribute( "resolutionY" ) &&
             t2->getHeader().hasAttribute( "resolutionZ" ) )
        {

          double resolutionX = 0.0;
          double resolutionY = 0.0;
          double resolutionZ = 0.0;
          t2->getHeader().getAttribute( "resolutionX", resolutionX );
          t2->getHeader().getAttribute( "resolutionY", resolutionY );
          t2->getHeader().getAttribute( "resolutionZ", resolutionZ );
          radius = 2.0 * std::min( resolutionX,
                                   std::min( resolutionY, resolutionZ ) );

        }
        else
        {

          radius = 1.0;

        }

      }
      gkg::ClosedMask< float, int16_t > closedMask( radius, thresholdRatio );
      closedMask.getMask( *t2, *mask, verbose );

      if ( verbose )
      {
	
	std::cout << "writing 'mask' : " << std::flush;
	
      }
      gkg::Writer::getInstance().write( "mask",
					*mask,
					ascii,
					volumeFormat );

      if ( verbose )
      {
	
	std::cout << "done" << std::endl;
	
      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading '" << fileNameMask << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read( fileNameMask, *mask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    if( dsi ) {
    ////////////////////////////////////////////////////////////////////////////
    // building input orientation set
    ////////////////////////////////////////////////////////////////////////////
    int32_t inputOrientationCount = dw->getSizeT();
    if ( inputOrientationCount != 515 )
    {
     
      throw std::runtime_error( 
	 "This DSI code was designed for a fixed Cartesian grid with 515 points");

    }
    gkg::GenericObjectList genericObjectListOrientations(
                                                        inputOrientationCount );
    std::vector< gkg::Vector3d< float > > inputOrientations( 
							inputOrientationCount );
    if ( verbose )
    {

      std::cout << "building input orientation count : " << std::flush;
      
    }
    if ( dw->getHeader().hasAttribute( "diffusion_gradient_orientations" ) )
    {

      dw->getHeader().getAttribute( "diffusion_gradient_orientations",
				    genericObjectListOrientations );

    }
    else
    {
	
      throw std::runtime_error( "missing gradient orientations" );
      
    }

    
    // sanity check to make sure the UN-NORMALIZED gradients were given
    int32_t o;
    float min = 10, max = 0;
    for( o = 0; o < inputOrientationCount; o++ )
    {
      
      gkg::GenericObjectList tmp =
	genericObjectListOrientations[ o ]->getValue< 
                                                     gkg::GenericObjectList >();
      
      inputOrientations[ o ].x = ( float )tmp[ 0 ]->getScalar();
      inputOrientations[ o ].y = ( float )tmp[ 1 ]->getScalar();
      inputOrientations[ o ].z = ( float )tmp[ 2 ]->getScalar();

      if( inputOrientations[o].x < min )
	min = inputOrientations[o].x;
      
      if( inputOrientations[o].x > max )
	max = inputOrientations[o].x;

    }

    if ( min != -5 || max != 5 ) 
    {
      
      throw std::runtime_error( 
	 "This DSI code was designed for a fixed latice of radius 5");
      
    }

    if ( verbose )
    {
	
      std::cout << "done" << std::endl;
      
    }
    }
    ////////////////////////////////////////////////////////////////////////////
    // building output orientation set
    ////////////////////////////////////////////////////////////////////////////
    gkg::OrientationSet* outputOrientationSet = 0;
    if ( outputOrientationCount > 0 )
    {

      if ( verbose )
      {

        std::cout << "building output orientation count : " << std::flush;

      }

      outputOrientationSet = new gkg::OrientationSet(
					    gkg::ElectrostaticOrientationSet( 
		     outputOrientationCount / 2 ).getSymmetricalOrientations());

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    gkg::DsiOdfCartesianField *dsiCartesianField = 0;
    gkg::DotOdfCartesianField *dotCartesianField = 0;
  
    if( dsi )
    {
    

      dsiCartesianField->checkOrInitializeDefaultParameters( 
						     specificScalarParameters,
						     specificStringParameters );

      if ( verbose ) 
      {

	std::cout << "building DSI cartesian field : " << std::flush;

      }
      
      if( !iBox.empty() ) 
      {
	
	gkg::BoundingBox< int32_t > boundingBox( iBox[ 0 ], iBox[ 3 ],
						 iBox[ 1 ], iBox[ 4 ],
						 iBox[ 2 ], iBox[ 5 ] );
	
	dsiCartesianField = new gkg::DsiOdfCartesianField(
                                                       t2,
                                                       dw,
                                                       mask,
                                                       boundingBox,
						       outputOrientationSet,
                                                       specificScalarParameters,
                                                       specificStringParameters,
                                                       verbose );
      }
      else
	dsiCartesianField = new gkg::DsiOdfCartesianField(
                                                       t2,
                                                       dw,
                                                       mask,
                                                       outputOrientationSet,
                                                       specificScalarParameters,
                                                       specificStringParameters,
                                                       verbose );
    }
    else
    {

      dotCartesianField->checkOrInitializeDefaultParameters( 
						     specificScalarParameters,
						     specificStringParameters );

      if ( verbose ) 
      {

	std::cout << "building DOT cartesian field : " << std::flush;

      }
      
      if( !iBox.empty() ) 
      {
	
	gkg::BoundingBox< int32_t > boundingBox( iBox[ 0 ], iBox[ 3 ],
						 iBox[ 1 ], iBox[ 4 ],
						 iBox[ 2 ], iBox[ 5 ] );
	
	dotCartesianField = new gkg::DotOdfCartesianField(
                                                       t2,
						       dw,
						       mask,
						       boundingBox,
						       outputOrientationSet,
                                                       specificScalarParameters,
                                                       specificStringParameters,
						       verbose );
      }
      else
	dotCartesianField = new gkg::DotOdfCartesianField(
                                                       t2,
						       dw,
						       mask,
						       outputOrientationSet,
                                                       specificScalarParameters,
                                                       specificStringParameters,
                                                       verbose );


    }
    if ( verbose ) 
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building and writing ODF functor map(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::OdfFunctorWriter odfFunctorWriter( rgbScale,
                                            meshScale,
                                            lowerGFAThreshold,
                                            upperGFAThreshold,
                                            volumeFormat,
                                            meshMapFormat,
                                            textureMapFormat,
                                            ascii,
                                            furtherScalarParameters,
                                            furtherStringParameters,
                                            verbose );
    if( dsi )
      odfFunctorWriter.write( odfFunctorNames,
			      outputFileNames,
			      *dsiCartesianField );
    else
      odfFunctorWriter.write( odfFunctorNames,
			      outputFileNames,
			      *dotCartesianField );
         
    ////////////////////////////////////////////////////////////////////////////
    // deleting output orientationset, EAP field
    ////////////////////////////////////////////////////////////////////////////

    if ( outputOrientationCount > 0 )
    {

      delete outputOrientationSet;

    }

    delete dsiCartesianField;
    delete dotCartesianField;

  }

  GKG_CATCH_COMMAND( result );
  
  return result;
  
}
