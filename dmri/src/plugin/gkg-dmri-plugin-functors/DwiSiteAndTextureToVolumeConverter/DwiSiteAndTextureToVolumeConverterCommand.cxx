#include <gkg-dmri-plugin-functors/DwiSiteAndTextureToVolumeConverter/DwiSiteAndTextureToVolumeConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/VolumeProxy_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/OdfCartesianFieldFactory.h>
#include <gkg-processing-container/SiteMapFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-io/OdfFunctorWriter.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <list>


gkg::DwiSiteAndTextureToVolumeConverterCommand::
                                      DwiSiteAndTextureToVolumeConverterCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                               : gkg::Command( argc, argv,
                                                               loadPlugin,
                                                               removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiSiteAndTextureToVolumeConverterCommand::"
             "DwiSiteAndTextureToVolumeConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiSiteAndTextureToVolumeConverterCommand::
                                      DwiSiteAndTextureToVolumeConverterCommand(
                                             const std::string& fileNameSiteMap,
                                             const std::string& fileNameOdfs,
                                             const std::string& outputFileName,
                                             const std::string& volumeFormat,
                                             bool ascii,
                                             bool verbose )
                                               : gkg::Command()
{

  try
  {

    execute( fileNameSiteMap,
             fileNameOdfs,
             outputFileName,
             volumeFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiSiteAndTextureToVolumeConverterCommand::"
             "DwiSiteAndTextureToVolumeConverterCommand( "
             "const std::string& fileNameSiteMap, "
             "const std::string& fileNameOdfs, "
             "const std::string& outputFileName, "
             "const std::string& volumeFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiSiteAndTextureToVolumeConverterCommand::
                                      DwiSiteAndTextureToVolumeConverterCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSiteMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOdfs );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameSiteMap,
             fileNameOdfs,
             outputFileName,
             volumeFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiSiteAndTextureToVolumeConverterCommand::"
             "DwiSiteAndTextureToVolumeConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiSiteAndTextureToVolumeConverterCommand::
                                    ~DwiSiteAndTextureToVolumeConverterCommand()
{
}


std::string gkg::DwiSiteAndTextureToVolumeConverterCommand::getStaticName()
{

  try
  {

    return "DwiSiteAndTextureToVolumeConverter";

  }
  GKG_CATCH( "std::string gkg::DwiSiteAndTextureToVolumeConverterCommand::getStaticName()" );

}


void gkg::DwiSiteAndTextureToVolumeConverterCommand::parse()
{

  try
  {

    std::string fileNameSiteMap;
    std::string fileNameOdfs;
    std::string outputFileName;
    std::string volumeFormat = "gis";
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "DWI Site and Texture to volume converter",
                                  _loadPlugin );
    application.addSingleOption( "-s",
                                 "Input site map",
                                 fileNameSiteMap );
    application.addSingleOption( "-o",
                                 "Input ODF texture map file name",
                                 fileNameOdfs );
    application.addSingleOption( "-v",
                                 "Output file name",
                                 outputFileName );
    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameSiteMap,
             fileNameOdfs,
             outputFileName,
             volumeFormat,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiSiteAndTextureToVolumeConverterCommand::"
                     "parse()" );

}


void gkg::DwiSiteAndTextureToVolumeConverterCommand::execute(
                                             const std::string& fileNameSiteMap,
                                             const std::string& fileNameOdfs,
                                             const std::string& outputFileName,
                                             const std::string& volumeFormat,
                                             bool ascii,
                                             bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading site(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameSiteMap << "' : " << std::flush;

    }
    gkg::SiteMap< int32_t, int32_t > siteMap;
    gkg::Reader::getInstance().read( fileNameSiteMap, siteMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading ODF texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameOdfs << "' : " << std::flush;

    }
    gkg::TextureMap< gkg::OrientationDistributionFunction > odfs;
    gkg::Reader::getInstance().read( fileNameOdfs, odfs );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    
    gkg::TextureMap< gkg::OrientationDistributionFunction >::const_iterator
      o = odfs.begin();
    if ( o == odfs.end() )
    {

      throw std::runtime_error( "ODF field is empty!" );

    }

    gkg::OrientationDistributionFunction::BasisType
      basisType = o->second.getBasisType();

    if ( basisType != gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      throw std::runtime_error( 
                             "texture need to be in Spherical Harmonic basis" );
      
    }

    
    ////////////////////////////////////////////////////////////////////////////
    // building output orientation count
    ////////////////////////////////////////////////////////////////////////////

    gkg::OrientationSet 
      outputOrientationSet( gkg::ElectrostaticOrientationSet(
                                       500 / 2 ).getSymmetricalOrientations() );


    ////////////////////////////////////////////////////////////////////////////
    // building cartesian ODF field from ODF texture map / site map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building ODF cartesian field : " << std::flush;

    }
    gkg::Volume< float > output;
    gkg::OdfCartesianField* 
      odfCartesianField = new gkg::OdfCartesianField( siteMap,
                                                      odfs,
                                                      outputOrientationSet );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // converting texture to volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "converting texture to volume : " << std::flush;

    }
    
    // getting site(s)
    const std::list< gkg::Vector3d< int32_t > >&
      sites = siteMap.getSites( 0 );

    // getting offset
    //gkg::Vector3d< int32_t > offset = odfCartesianField->getOffset();

    // preparing ouput volume
    int32_t sizeX = odfCartesianField->getSizeX();
    int32_t sizeY = odfCartesianField->getSizeY();
    int32_t sizeZ = odfCartesianField->getSizeZ();
     
     int sizeT = (odfs.getTexture(0)).getValues().size();

     if( verbose )
     {
       
       std::cout << sizeX << "x"
                 << sizeY << "x"
                 << sizeZ << "x"
                 << sizeT << " ";
     
     }

     gkg::Vector3d< double > resolution;

     resolution.x = odfCartesianField->getResolutionX();
     resolution.y = odfCartesianField->getResolutionY();
     resolution.z = odfCartesianField->getResolutionZ();

     output.reallocate( sizeX, sizeY, sizeZ,  sizeT );
     output.getHeader().addAttribute( "resolutionX", resolution.x );
     output.getHeader().addAttribute( "resolutionY", resolution.y );
     output.getHeader().addAttribute( "resolutionZ", resolution.z );
     output.fill( 0 ); 

     // looping over site(s) and processing output item(s)
     int32_t i = 0, count = 0;
     std::list< gkg::Vector3d< int32_t > >::const_iterator
       s = sites.begin(),
       se = sites.end();
     while ( s != se )
     {
     
       for( i = 0; i < sizeT; i++ )
       {

	 output( *s, i ) =
                         ( odfCartesianField->getItem( *s )->getValues() )[ i ];

       }
       
       ++ s;
       ++ count;
     
     }
     
     if ( verbose )
     {

       std::cout << "done" << std::endl;
       
     }

     
     if ( verbose )
     {
       
       std::cout << "writing \'" << outputFileName;

     }
       
     gkg::Writer::getInstance().write( outputFileName,
				       output,
				       ascii,
				       volumeFormat );
     
     if ( verbose )
     {
       
       std::cout << "\' done" << std::endl;

     }
     
     delete odfCartesianField;

  }
  GKG_CATCH( "void gkg::DwiSiteAndTextureToVolumeConverterCommand::execute( "
             "const std::string& fileNameSiteMap, "
             "const std::string& fileNameOdfs, "
             "const std::string& outputFileName, "
             "const std::string& volumeFormat, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiSiteAndTextureToVolumeConverterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameSiteMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOdfs ) +
    DECLARE_STRING_PARAMETER_HELP( outputFileName ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
