#include <gkg-dmri-plugin-functors/DwiPdfField/DwiPdfFieldCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-dmri-container/PdfCartesianFieldFactory.h>
#include <gkg-dmri-io/PdfFunctorWriter.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <map>
#include <iostream>


gkg::DwiPdfFieldCommand::DwiPdfFieldCommand( int32_t argc,
                                             char* argv[],
                                             bool loadPlugin,
                                             bool removeFirst )
                        : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiPdfFieldCommand::DwiPdfFieldCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiPdfFieldCommand::DwiPdfFieldCommand(
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& modelType,
                     const std::list< std::string >& pdfFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     const std::vector< float >& outputSampling,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     float lowerGFAThreshold,
                     float upperGFAThreshold,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     const std::vector< double >& furtherScalarParameters,
                     const std::vector< std::string >& furtherStringParameters,
                     bool ascii,
                     bool verbose )
                        : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             modelType,
             pdfFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputSampling,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             lowerGFAThreshold,
             upperGFAThreshold,
             specificScalarParameters,
             specificStringParameters,
             furtherScalarParameters,
             furtherStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiPdfFieldCommand::DwiPdfFieldCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& modelType, "
             "const std::list< std::string >& pdfFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const std::vector< float >& outputSampling, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "float lowerGFAThreshold, "
             "float upperGFAThreshold, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "const std::vector< double >& furtherScalarParameters, "
             "const std::vector< std::string >& furtherStringParameters, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiPdfFieldCommand::DwiPdfFieldCommand(
                                             const gkg::Dictionary& parameters )
                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, modelType );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       pdfFunctorNames );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       outputFileNames );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                          iBox );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           fBox );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           superResolution );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           outputSampling );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, float, thresholdRatio );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, meshMapFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, textureMapFormat );
    DECLARE_FLOATING_PARAMETER( parameters, float, rgbScale );
    DECLARE_FLOATING_PARAMETER( parameters, float, meshScale );
    DECLARE_FLOATING_PARAMETER( parameters, float, lowerGFAThreshold );
    DECLARE_FLOATING_PARAMETER( parameters, float, upperGFAThreshold );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           specificScalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         specificStringParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           furtherScalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         furtherStringParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             modelType,
             pdfFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputSampling,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             lowerGFAThreshold,
             upperGFAThreshold,
             specificScalarParameters,
             specificStringParameters,
             furtherScalarParameters,
             furtherStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiPdfFieldCommand::DwiPdfFieldCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiPdfFieldCommand::~DwiPdfFieldCommand()
{
}


std::string gkg::DwiPdfFieldCommand::getStaticName()
{

  try
  {

    return "DwiPdfField";

  }
  GKG_CATCH( "std::string gkg::DwiPdfFieldCommand::getStaticName()" );

}


void gkg::DwiPdfFieldCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMask = "";
    std::string modelType;
    std::list< std::string > pdfFunctorNames;
    std::list< std::string > outputFileNames;
    std::vector< int32_t > iBox;
    std::vector< float > fBox;
    std::vector< float > superResolution;
    std::vector< float > outputSampling;
    float radius = -1.0;
    float thresholdRatio = 0.01;
    std::string volumeFormat = "gis";
    std::string meshMapFormat = "aimsmesh";
    std::string textureMapFormat = "aimstexture";
    float rgbScale = 1.0;
    float meshScale = 1.0;
    float lowerGFAThreshold = 0.0;
    float upperGFAThreshold = 1.0;
    std::vector< double > specificScalarParameters;
    std::vector< std::string > specificStringParameters;
    std::vector< double > furtherScalarParameters;
    std::vector< std::string > furtherStringParameters;
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // building PDF model list
    ////////////////////////////////////////////////////////////////////////////
    std::string modelText = "Type of the PDF model :\n";
    std::list< std::string >
      allModelNames = gkg::PdfCartesianFieldFactory::getInstance().getNames();
    std::list< std::string >::const_iterator
      m = allModelNames.begin(),
      me = allModelNames.end();
    while ( m != me )
    {

      modelText += "- " + *m + "\n";
      ++ m;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building PDF functor list
    ////////////////////////////////////////////////////////////////////////////
    std::string pdfFunctorText = "PDF functor list among :\n";
    std::list< std::string >
      allPdfFunctorNames = gkg::PdfFunctorWriter::getNames();
    std::list< std::string >::const_iterator
      f = allPdfFunctorNames.begin(),
      fe = allPdfFunctorNames.end();
    while ( f != fe )
    {

      pdfFunctorText += "- " + *f + "\n";
      ++ f;

    }

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI PDF field model",
                                  _loadPlugin );
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
    application.addSingleOption( "-type",
                                 modelText,
                                 modelType );
    application.addSeriesOption( "-f",
                                 pdfFunctorText,
                                 pdfFunctorNames,
                                 1 );
    application.addSeriesOption( "-o",
                                 "Output file name(s)",
                                 outputFileNames,
                                 1 );
    application.addSeriesOption( "-ibox",
                                 "Integer bounding box on which to compute"
                                 " the PDF field",
                                 iBox );
    application.addSeriesOption( "-fbox",
                                 "Float bounding box on which to compute"
                                 " the PDF field",
                                 fBox );
    application.addSeriesOption( "-superResolution",
                                 "Super resolution of the PDF field"
                                 " (default=input resolution)",
                                 superResolution );
    application.addSeriesOption( "-outputSampling",
                                 "Output sampling information:\n "
                                 "- in case of spherical multiple shell "
                                 "sampling\n "
                                 ". 1 <d1> <o1> ..... <dN> <oN>\n"
                                 ". where the first item is set to 1 to"
                                 "inform of the multiple spherical sampling, \n"
                                 ". and (d1, ..., dN) represent the "
                                 "radii of each displacement sphere in m\n"
                                 ". and (o1, ..., oN) represent the number of "
                                 "directions for each sphere\n\n"
                                 "- in case of Cartesian grid sampling\n "
                                 ". 2 <sx> <sy> <sz> <rx> <ry> <rz>\n"
                                 ". where the first item is set to 2 to "
                                 "inform of the Cartesian grid sampling,"
                                 ". and (sx,sy,sz) represent the grid size\n"
                                 ". and (rx,ry,rz) represent the grid "
                                 "resolution in m\n"
                                 "(default=1 1e-6 60 10e-6 60 100e-6 60)",
                                 outputSampling );
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

    application.addSingleOption( "-rgbScale",
                                 "Scale factor for RGB map(s) (default=1.0)",
                                 rgbScale,
                                 true );
    application.addSingleOption( "-meshScale",
                                 "Scale factor for mesh (default=1.0)",
                                 meshScale,
                                 true );
    application.addSingleOption( "-lowerGFAThreshold",
                                 "Lower GFA threshold (default=0.0)",
                                 lowerGFAThreshold,
                                 true );
    application.addSingleOption( "-upperGFAThreshold",
                                 "Upper GFA threshold (default=1.0)",
                                 upperGFAThreshold,
                                 true );

    application.addSeriesOption( 
			   "-scalarParameters",
			   "PDF cartesian field specific parameters as "
			   "a vector of double <P1> <P2> ... <Pn>:"
			   "\n\n"
			   "- in case of shore_pdf_cartesian_field,\n"
			   ".  <P1>: maximum polar harmonics order\n"
			   ".  <P2>: regularization L-curve factor\n"
			   ".  (default= 6 / 0.006)"
			   "\n\n",
			   specificScalarParameters );
    application.addSeriesOption( "-stringParameters",
                                 "PDF cartesian field specific parameters as "
                                 "a vector of string <P1> <P2> ... <Pn>:"
                                 "\n\n"
                                 "- in case of shore_pdf_cartesian_field,\n"
                                 ".  N/A"
                                 "\n\n",
                                 specificStringParameters );

    application.addSeriesOption(
                           "-furtherScalarParameters",
                           "Further scalar parameters to be used by functors:"
                           "\n\n"
			   "- in case of non_gaussianity,\n"
                           ". N/A\n\n"
			   "- in case of odf_site_map,\n"
                           ". N/A\n\n"
			   "- in case of odf_texture_map,\n"
                           ". with shore-odf type:\n"
			   ".  <P1>: orientation count of ODFs\n"
                           ". with shell-odf type:\n"
			   ".  <P1>: orientation count of ODFs\n"
			   ".  <P2>: displacement in um\n"
			   ".  <P3>: spherical harmonics order\n"
			   ".  <P4>: Laplace-Beltrami regularization factor\n"
                           ". with shore-to-sh-odf type:\n"
			   ".  <P1>: orientation count of ODFs\n"
			   ".  <P2>: spherical harmonics order\n"
			   ".  <P3>: Laplace-Beltrami regularization factor\n\n"
			   "- in case of pdf_decomposition,\n"
                           ". N/A\n\n"
			   "- in case of pdf_site_map,\n"
                           ". N/A\n\n"
			   "- in case of pdf_texture_map,\n"
                           ". N/A\n\n"
			   "- in case of propagator_anisotropy,\n"
                           ". N/A\n\n"
			   "- in case of return_to_axis_probability,\n"
                           ". N/A\n\n"
			   "- in case of return_to_origin_probability,\n"
                           ". N/A\n\n"
			   "- in case of return_to_plane_probability"
                           ". N/A\n\n",
                           furtherScalarParameters );


    application.addSeriesOption(
                           "-furtherStringParameters",
                           "Further string parameters to be used by functors:"
                           "\n\n"
			   "- in case of non_gaussianity,\n"
                           ". N/A\n\n"
			   "- in case of odf_site_map,\n"
                           ". N/A\n\n"
                           "- in case of odf_texture_map,\n"
                           ".  <P1>: ODF reconstruciton type among\n"
                           ".        - shore-odf : standard basis\n"
                           ".        - shell-odf : spherical harmonics basis\n"
                           ".        - shore-to-sh-odf : spherical harmonics "
                           "basis\n\n"
                           ". N/A\n\n"
			   "- in case of pdf_decomposition,\n"
                           ". N/A\n\n"
			   "- in case of pdf_site_map,\n"
                           ". N/A\n\n"
			   "- in case of pdf_texture_map,\n"
                           ". N/A\n\n"
			   "- in case of propagator_anisotropy,\n"
                           ". N/A\n\n"
			   "- in case of return_to_axis_probability,\n"
                           ". N/A\n\n"
			   "- in case of return_to_origin_probability,\n"
                           ". N/A\n\n"
			   "- in case of return_to_plane_probability"
                           ". N/A\n\n",
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

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             modelType,
             pdfFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputSampling,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             lowerGFAThreshold,
             upperGFAThreshold,
             specificScalarParameters,
             specificStringParameters,
             furtherScalarParameters,
             furtherStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiPdfFieldCommand::parse()" );

}


void gkg::DwiPdfFieldCommand::execute(
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& modelType,
                     const std::list< std::string >& pdfFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     const std::vector< float >& outputSampling,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     float lowerGFAThreshold,
                     float upperGFAThreshold,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     const std::vector< double >& furtherScalarParameters,
                     const std::vector< std::string >& furtherStringParameters,
                     bool ascii,
                     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( pdfFunctorNames.size() != outputFileNames.size() )
    {

      throw std::runtime_error( "different PDF functor and file name count" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // decoding output sampling information and performing sanity checks
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float > theOutputSampling = outputSampling;

    int32_t outputSamplingType = 1;
    std::map< float, gkg::OrientationSet > outputMultipleShellSampling;
    gkg::Vector3d< int32_t > outputCartesianGridSize;
    gkg::Vector3d< double > outputCartesianGridResolution;

    if ( theOutputSampling.empty() )
    {

      theOutputSampling.resize( 7U );
      theOutputSampling[ 0 ] = 1;
      theOutputSampling[ 1 ] = 1e-6;
      theOutputSampling[ 2 ] = 60;
      theOutputSampling[ 3 ] = 10e-6;
      theOutputSampling[ 4 ] = 60;
      theOutputSampling[ 5 ] = 100e-6;
      theOutputSampling[ 6 ] = 60;

    }
    else
    {

      int32_t outputSamplingType = ( int32_t )( theOutputSampling[ 0 ] + 0.5 );
      if ( outputSamplingType == 1 )
      {

        if ( theOutputSampling.size() % 2 == 0 )
        {

          throw std::runtime_error( "-outputSampling vector must be of shape "
                                    "'1 <d1> <o1> ... <dN> <oN>" );

        }

        int32_t shellCount = ( ( int32_t )theOutputSampling.size() - 1 ) / 2;

        int32_t shell = 0;
        for ( shell = 0; shell < shellCount; shell++ )
        {

          float shellRadius = ( float )( theOutputSampling[ 2 * shell + 1 ] );
          int32_t shellOrientationCount =
                   ( int32_t )( theOutputSampling[ 2 * shell + 2 ] + 0.5 );
          outputMultipleShellSampling[ shellRadius ] = 
            gkg::OrientationSet( gkg::ElectrostaticOrientationSet( 
                     shellOrientationCount / 2 ).getSymmetricalOrientations() );

        }

      }
      else if ( outputSamplingType == 2 )
      {

        if ( theOutputSampling.size() != 7U )
        {

          throw std::runtime_error( "-outputSampling vector must be of shape "
                                    "'2 <sx> <sy> <sz> <rx> <ry> <rz>" );

        }
        outputCartesianGridSize.x = ( int32_t )( theOutputSampling[ 1 ] + 0.5 );
        outputCartesianGridSize.y = ( int32_t )( theOutputSampling[ 2 ] + 0.5 );
        outputCartesianGridSize.z = ( int32_t )( theOutputSampling[ 3 ] + 0.5 );
        outputCartesianGridResolution.x = ( double )theOutputSampling[ 4 ];
        outputCartesianGridResolution.y = ( double )theOutputSampling[ 5 ];
        outputCartesianGridResolution.z = ( double )theOutputSampling[ 6 ];

      }
      else
      {

        throw std::runtime_error( 
                    "-outputSampling vector of scalar must start with 1 or 2" );

      }

    }


    std::vector< double > 
      theSpecificScalarParameters = specificScalarParameters;
    std::vector< std::string >
      theSpecificStringParameters = specificStringParameters;
    gkg::PdfCartesianFieldFactory::getInstance().
      checkOrInitializeDefaultParameters( modelType,
                                          theSpecificScalarParameters,
                                          theSpecificStringParameters );


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

    gkg::RCPointer< gkg::Volume< int16_t > > mask( new gkg::Volume< int16_t > );
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
	
	std::cout << "done" << std::endl;
	
      }

      if ( verbose )
      {
 
        std::cout << "writing 'mask' : " << std::flush;
 
      }

      gkg::File fileMask( outputFileNames.front() );
      std::string fileNameMask = fileMask.getDirectoryName() +
                                 gkg::getDirectorySeparator() +
                                 "mask";
      gkg::Writer::getInstance().write( fileNameMask,
                                        *mask,
                                        ascii,
                                        volumeFormat );

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


    ////////////////////////////////////////////////////////////////////////////
    // building PDF cartesian field
    ////////////////////////////////////////////////////////////////////////////

    gkg::PdfCartesianField* pdfCartesianField = 0;

    if ( outputSamplingType == 1 )
    {

      pdfCartesianField = gkg::PdfCartesianFieldFactory::getInstance().create(
                                                   modelType,
                                                   t2,
                                                   dw,
                                                   mask,
                                                   iBox,
                                                   fBox,
                                                   superResolution,
                                                   &outputMultipleShellSampling,
                                                   theSpecificScalarParameters,
                                                   theSpecificStringParameters,
                                                   verbose );

    }
    else
    {

      pdfCartesianField = gkg::PdfCartesianFieldFactory::getInstance().create(
                                                 modelType,
                                                 t2,
                                                 dw,
                                                 mask,
                                                 iBox,
                                                 fBox,
                                                 superResolution,
                                                 &outputCartesianGridSize,
                                                 &outputCartesianGridResolution,
                                                 theSpecificScalarParameters,
                                                 theSpecificStringParameters,
                                                 verbose );

    }


    ////////////////////////////////////////////////////////////////////////////
    // building and writing PDF functor map(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::PdfFunctorWriter pdfFunctorWriter( rgbScale,
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
    pdfFunctorWriter.write( pdfFunctorNames,
                            outputFileNames,
                            *pdfCartesianField );


    ////////////////////////////////////////////////////////////////////////////
    // deleting PDF field
    ////////////////////////////////////////////////////////////////////////////

    delete pdfCartesianField;

  }
  GKG_CATCH( "void gkg::DwiPdfFieldCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& modelType, "
             "const std::list< std::string >& pdfFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "const std::vector< float >& outputSampling, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "float lowerGFAThreshold, "
             "float upperGFAThreshold, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "const std::vector< double >& furtherScalarParameters, "
             "const std::vector< std::string >& furtherStringParameters, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiPdfFieldCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( modelType ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( pdfFunctorNames ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( outputFileNames ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( iBox ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( fBox ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( superResolution ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( outputSampling ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_STRING_PARAMETER_HELP( meshMapFormat ) +
    DECLARE_STRING_PARAMETER_HELP( textureMapFormat ) +
    DECLARE_FLOATING_PARAMETER_HELP( rgbScale ) +
    DECLARE_FLOATING_PARAMETER_HELP( meshScale ) +
    DECLARE_FLOATING_PARAMETER_HELP( lowerFAThreshold ) +
    DECLARE_FLOATING_PARAMETER_HELP( upperFAThreshold ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( specificScalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( specificStringParameters ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( furtherScalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( furtherStringParameters ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
