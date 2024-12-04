#include <gkg-dmri-plugin-functors/DwiEapField/DwiEapFieldCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-dmri-container/EapCartesianFieldFactory.h>
#include <gkg-dmri-io/EapFunctorWriter.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::DwiEapFieldCommand::DwiEapFieldCommand( int32_t argc,
                                             char* argv[],
                                             bool loadPlugin,
                                             bool removeFirst )
                        : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiEapFieldCommand::DwiEapFieldCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiEapFieldCommand::DwiEapFieldCommand(
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& methodType,
                     const std::list< std::string >& eapFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     int32_t outputOrientationCount,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     bool meshOpacity,
                     const std::vector< float >& eapMeshRadii,
                     const std::vector< float >& odfLowerUpperLimits,
                     bool marginalOdf,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool ascii,
                     bool verbose )
                        : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             methodType,
             eapFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputOrientationCount,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             meshOpacity,
             eapMeshRadii,
             odfLowerUpperLimits,
             marginalOdf,
             specificScalarParameters,
             specificStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiEapFieldCommand::DwiEapFieldCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& methodType, "
             "const std::list< std::string >& eapFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "int32_t outputOrientationCount, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "bool meshOpacity, "
             "const std::vector< float >& eapMeshRadii, "
             "const std::vector< float >& odfLowerUpperLimits, "
             "bool marginalOdf, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiEapFieldCommand::DwiEapFieldCommand(
                                             const gkg::Dictionary& parameters )
                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, methodType );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       eapFunctorNames );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       outputFileNames );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                          iBox );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           fBox );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           superResolution );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, outputOrientationCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, float, thresholdRatio );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, meshMapFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, textureMapFormat );
    DECLARE_FLOATING_PARAMETER( parameters, float, rgbScale );
    DECLARE_FLOATING_PARAMETER( parameters, float, meshScale );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, meshOpacity );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           eapMeshRadii );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           odfLowerUpperLimits );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, marginalOdf );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           specificScalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         specificStringParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             methodType,
             eapFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputOrientationCount,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             meshOpacity,
             eapMeshRadii,
             odfLowerUpperLimits,
             marginalOdf,
             specificScalarParameters,
             specificStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiEapFieldCommand::DwiEapFieldCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiEapFieldCommand::~DwiEapFieldCommand()
{
}


std::string gkg::DwiEapFieldCommand::getStaticName()
{

  try
  {

    return "DwiEapField";

  }
  GKG_CATCH( "std::string gkg::DwiEapFieldCommand::getStaticName()" );

}


void gkg::DwiEapFieldCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMask = "";
    std::string methodType;
    std::list< std::string > eapFunctorNames;
    std::list< std::string > outputFileNames;
    std::vector< int32_t > iBox;
    std::vector< float > fBox;
    std::vector< float > superResolution;
    int32_t outputOrientationCount = 0;
    float radius = -1.0;
    float thresholdRatio = 0.04;
    std::string volumeFormat = "gis";
    std::string meshMapFormat = "aimsmesh";
    std::string textureMapFormat = "aimstexture";
    float rgbScale = 1.0;
    float meshScale = 1.0;
    bool meshOpacity = false;
    std::vector< float > eapMeshRadii;
    std::vector< float > odfLowerUpperLimits;
    bool marginalOdf = true;
    std::vector< double > specificScalarParameters;
    std::vector< std::string > specificStringParameters;
    bool ascii = false;
    bool verbose = false;


    /////////////////////////////////////////////////////////////////////////
    // building EAP method list
    /////////////////////////////////////////////////////////////////////////
    std::string methodText = "Type of the EAP reconstruction method :\n";
    std::list< std::string >
      allModelNames = gkg::EapCartesianFieldFactory::getInstance().getNames();
    std::list< std::string >::const_iterator
      m = allModelNames.begin(),
      me = allModelNames.end();
    while ( m != me )
    {

      methodText += "- " + *m + "\n";
      ++ m;

    }

    ////////////////////////////////////////////////////////////////////////
    // building EAP functor list
    ////////////////////////////////////////////////////////////////////////

    std::string eapFunctorText = "EAP functor list among :\n";
    std::list< std::string >
      allEapFunctorNames = gkg::EapFunctorWriter::getNames();
    std::list< std::string >::const_iterator
      f = allEapFunctorNames.begin(),
      fe = allEapFunctorNames.end();
    while ( f != fe )
    {
      
      eapFunctorText += "- " + *f + "\n";
      ++ f;

    }

    ////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI ODF field model",
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
                                 methodText,
                                 methodType );
    application.addSeriesOption( "-f",
                                 eapFunctorText,
                                 eapFunctorNames,
                                 1 );
    application.addSeriesOption( "-o",
                                 "Output file name(s)",
                                 outputFileNames,
                                 1 );
    application.addSeriesOption( "-ibox",
                                 "Integer bounding box on which to compute"
                                 " the EAP field",
                                 iBox );
    application.addSeriesOption( "-fbox",
                                 "Float bounding box on which to compute"
                                 " the EAP field",
                                 fBox );
    application.addSeriesOption( "-superResolution",
                                 "Super resolution of the EAP field"
                                 " (default=input resolution)",
                                 superResolution );
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
    application.addSingleOption( "-rgbScale",
                                 "Scale factor for RGB map(s) (default=1.0)",
                                 rgbScale,
                                 true );
    application.addSingleOption( "-meshScale",
                                 "Scale factor for mesh (default=1.0)",
                                 meshScale,
                                 true );
    application.addSeriesOption( "-eapMeshRadii",
                                 "Radii for meshes visualization of the EAP\n"
                                 "(default= 0 0.5 0.75 1 2 3 5 7 10)",
                                 eapMeshRadii );
    application.addSingleOption( "-meshWithOpacity",
                                 "Automatically set EAP mesh opacity with radii\n"
				 "( works well for single mesh output, not good "
				 "for whole field visualization)\n"
                                 "(default= false)",
                                 meshOpacity,
				 true );
    application.addSingleOption( "-odfMarginal",
                                 "Compute marginal ODF or not. If not, computes Tuch's ODF\n"
                                 "(default= true)",
                                 marginalOdf,
				 true );    
    application.addSeriesOption( "-odfIntegralLowerUpperLimits",
                                 "Lower and upper limits in the ODF computation integral\n"
                                 "(default= 0 6)",
                                 odfLowerUpperLimits );
    application.addSeriesOption( "-scalarParameters",
                                 "EAP cartesian field scalar parameters as "
                                 "a vector of double <P1> <P2> ... <Pn>:"
                                 "\n"
//                                  "- in case of charmed_eap_cartesian_field,\n"
//                                  ". (Assaf et al MRM 2004)"
// 				 " To be implemented..."
// 				 "\n\n"
//                                  "- in case of dot_eap_cartesian_field,\n"
// 				 ". (Ozarslan et al NeuroImage 2006)"
// 				 " To be implemented..."
// 				 "\n\n"
                                 "- in case of dpi_eap_cartesian_field,\n"
				 ". (Descoteaux et al MRM 2009)\n"
                                 ".  <P1>: number of shells (minimum 2)\n"
				 ".  <P2>: number of orientations per shell\n"
				 ".  <P3>: spherical harmonics order\n"
                                 ".  (default= 0 / 0 / 4)"
//                                  "\n\n"
//                                  "- in case of dsi_eap_cartesian_field,\n"
// 				 ". (Wedeen et al MRM 2005)"
// 				 " To be implemented..."
//                                  "\n\n"
//                                  "- in case of tomographic_eap_cartesian_field,\n"
//                                  ". (Pickalov et al ISBI 2006) "
// 				 " To be implemented..."
				 "\n",
                                 specificScalarParameters );
    application.addSeriesOption( 
				"-stringParameters",
				"EAP cartesian field string parameters as "
				"a vector of string <P1> <P2> ... <Pn>:"
				"\n"
// 				"- in case of charmed_eap_cartesian_field,\n"
// 				".  To be implemented..."
// 				"\n\n"
// 				"- in case of dot_eap_cartesian_field,\n"
// 				".  To be implemented..."
// 				"\n\n"
				"- in case of dpi_eap_cartesian_field,\n"
				".  No string parameter currently needed..."
//				"\n\n"
// 				"- in case of dsi_eap_cartesian_field,\n"
// 				".  To be implemented..."
// 				"\n\n"
// 				"- in case of tomographic_eap_cartesian_field,\n"
// 				".  To be implemented..."
				"\n",
				specificStringParameters );
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
             methodType,
             eapFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputOrientationCount,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             meshOpacity,
             eapMeshRadii,
             odfLowerUpperLimits,
             marginalOdf,
             specificScalarParameters,
             specificStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiEapFieldCommand::parse()" );

}


void gkg::DwiEapFieldCommand::execute(
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& methodType,
                     const std::list< std::string >& eapFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     int32_t outputOrientationCount,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     bool meshOpacity,
                     const std::vector< float >& eapMeshRadii,
                     const std::vector< float >& odfLowerUpperLimits,
                     bool marginalOdf,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool ascii,
                     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( eapFunctorNames.size() != outputFileNames.size() )
    {

      throw std::runtime_error( "different EAP functor and file name count" );

    }
    if ( outputOrientationCount < 0 )
    {

      throw std::runtime_error( "output orientation count must be positive" );

    }

    std::vector< double >
      theSpecificScalarParameters = specificScalarParameters;
    std::vector< std::string >
      theSpecificStringParameters = specificStringParameters;
    gkg::EapCartesianFieldFactory::getInstance().
      checkOrInitializeDefaultParameters( methodType,
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
                    outputOrientationCount / 2 ).getSymmetricalOrientations() );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building EAP cartesian field
    ////////////////////////////////////////////////////////////////////////////

    gkg::EapCartesianField*
      eapCartesianField = gkg::EapCartesianFieldFactory::getInstance().create(
                                                    methodType,
                                                    t2,
                                                    dw,
                                                    mask,
                                                    iBox,
                                                    fBox,
                                                    superResolution,
                                                    outputOrientationSet,
                                                    theSpecificScalarParameters,
                                                    theSpecificStringParameters,
                                                    verbose );


    ////////////////////////////////////////////////////////////////////////////
    // building and writing EAP functor map(s)
    ////////////////////////////////////////////////////////////////////////////


    std::vector< float > theEapMeshRadii = eapMeshRadii;
    if ( theEapMeshRadii.size() == 0 ) 
    {
      
      theEapMeshRadii.push_back( 0.1 );
      theEapMeshRadii.push_back( 0.5 );
      theEapMeshRadii.push_back( 0.75 );
      theEapMeshRadii.push_back( 1.0 );
      theEapMeshRadii.push_back( 2.0 );
      theEapMeshRadii.push_back( 3.0 );
      theEapMeshRadii.push_back( 5.0 );
      theEapMeshRadii.push_back( 7.0 );
      theEapMeshRadii.push_back( 10.0 );

    }

    std::vector< float > theOdfLowerUpperLimits = odfLowerUpperLimits;
    if ( theOdfLowerUpperLimits.size() == 0 ) 
    {
      
      theOdfLowerUpperLimits.push_back( 0.0 );
      theOdfLowerUpperLimits.push_back( 3.0 );

    }
    
    gkg::EapFunctorWriter eapFunctorWriter( rgbScale,
                                            meshScale,
					    theEapMeshRadii,
					    meshOpacity,
					    marginalOdf,
					    theOdfLowerUpperLimits,
                                            volumeFormat,
                                            meshMapFormat,
                                            textureMapFormat,
                                            ascii,
                                            verbose );

    eapFunctorWriter.write( eapFunctorNames,
                            outputFileNames,
                            *eapCartesianField );


    ////////////////////////////////////////////////////////////////////////////
    // deleting output orientationset, EAP field
    ////////////////////////////////////////////////////////////////////////////

    if ( outputOrientationCount > 0 )
    {

      delete outputOrientationSet;

    }
    delete eapCartesianField;

  }
  GKG_CATCH( "void gkg::DwiEapFieldCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& methodType, "
             "const std::list< std::string >& eapFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "int32_t outputOrientationCount, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "bool meshOpacity, "
             "const std::vector< float >& eapMeshRadii, "
             "const std::vector< float >& odfLowerUpperLimits, "
             "bool marginalOdf, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiEapFieldCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( methodType ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( eapFunctorNames ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( outputFileNames ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( iBox ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( fBox ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( superResolution ) +
    DECLARE_INTEGER_PARAMETER_HELP( outputOrientationCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_STRING_PARAMETER_HELP( meshMapFormat ) +
    DECLARE_STRING_PARAMETER_HELP( textureMapFormat ) +
    DECLARE_FLOATING_PARAMETER_HELP( rgbScale ) +
    DECLARE_FLOATING_PARAMETER_HELP( meshScale ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( meshOpacity ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( eapMeshRadii ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( odfLowerUpperLimits ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( marginalOdf ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( specificScalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( specificStringParameters ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
