#include <gkg-deep-nuclei-plugin-functors/OccurrenceAtlas/OccurrenceAtlasCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-roi/LabelledRoiDictionary.h>
#include <gkg-processing-transform/TalairachTransform3d_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>


gkg::OccurrenceAtlasCommand::OccurrenceAtlasCommand( int32_t argc,
                                                     char* argv[], 
                                                     bool loadPlugin,
                                                     bool removeFirst )
                           : gkg::Command( argc, argv,
                                           loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::OccurrenceAtlasCommand::OccurrenceAtlasCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::OccurrenceAtlasCommand::OccurrenceAtlasCommand( 
                             const std::vector< std::string >& labelFileNameIns,
                             const std::string& roiDictionaryFileName,
                             bool talairach,
                             const std::string& fileNameOut,
                             const std::string& fileNameRoiNameToRank,
                             bool ascii,
                             const std::string& format,
                             bool verbose )
                           : gkg::Command()
{

  try
  {

    execute( labelFileNameIns, roiDictionaryFileName, talairach,
             fileNameOut, fileNameRoiNameToRank, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::OccurrenceAtlasCommand::OccurrenceAtlasCommand( "
             "const std::vector< std::string >& labelFileNameIns, "
             "const std::string& roiDictionaryFileName, "
             "bool talairach, const std::string& fileNameOut, "
             "const std::string& fileNameRoiNameToRank, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::OccurrenceAtlasCommand::OccurrenceAtlasCommand(
                                             const gkg::Dictionary& parameters )
                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         labelFileNameIns );
    DECLARE_STRING_PARAMETER( parameters, std::string, roiDictionaryFileName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, talairach );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameRoiNameToRank );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( labelFileNameIns, roiDictionaryFileName, talairach,
             fileNameOut, fileNameRoiNameToRank, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::OccurrenceAtlasCommand::OccurrenceAtlasCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::OccurrenceAtlasCommand::~OccurrenceAtlasCommand()
{
}


std::string gkg::OccurrenceAtlasCommand::getStaticName()
{

  try
  {

    return "OccurrenceAtlas";

  }
  GKG_CATCH( "std::string gkg::OccurrenceAtlasCommand::getStaticName()" );

}


void gkg::OccurrenceAtlasCommand::parse()
{

  try
  {

    std::vector< std::string > labelFileNameIns;
    std::string roiDictionaryFileName;
    bool talairach = true;
    std::string fileNameOut;
    std::string fileNameRoiNameToRank;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Accumulate labels occurrence into a 4D "
                                  "atlas volume",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input label volume file name list",
                                 labelFileNameIns,
                                 1 );
    application.addSingleOption( "-d",
                                 "Labelled ROI dictionary file name",
                                 roiDictionaryFileName );
    application.addSingleOption( "-talairach",
                                 "Build the atlas in the Talairach frame "
                                 "(default=true)",
                                 talairach,
                                 true );
    application.addSingleOption( "-o",
                                 "Output accumulation atlas volume file "
                                 "name (append if file already exists).",
                                 fileNameOut );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode "
                                 "(default=false)",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-fileNameRoiNameToRank",
                                 "Ouput ROI name to rank lookup table",
                                 fileNameRoiNameToRank,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( labelFileNameIns, roiDictionaryFileName, talairach,
             fileNameOut, fileNameRoiNameToRank, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::OccurrenceAtlasCommand::parse()" );

}


void gkg::OccurrenceAtlasCommand::execute(
                             const std::vector< std::string >& labelFileNameIns,
                             const std::string& roiDictionaryFileName,
                             bool talairach,
                             const std::string& fileNameOut,
                             const std::string& fileNameRoiNameToRank,
                             bool ascii,
                             const std::string& format,
                             bool verbose )
{

  try
  {

    int32_t sizeX = 137; // Talairach volume sizes
    int32_t sizeY = 173;
    int32_t sizeZ = 119;


    ////////////////////////////////////////////////////////////////////////////
    // reading labelled ROI dictionary
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '"
                << roiDictionaryFileName
                << "' : " << std::flush;

    }
    gkg::LabelledRoiDictionary 
      labelledRoiDictionary( "deep_nuclei_dictionary" );
    gkg::Reader::getInstance().read( roiDictionaryFileName,
                                     labelledRoiDictionary );
    gkg::Dictionary roiDictionary = labelledRoiDictionary.getDictionary();
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing look-up table(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing label<->rank LUT(s) : " << std::flush;

    }
    gkg::Dictionary labelToRankLutDict;
    std::map< int32_t, int16_t > rankToLabelLut;
    std::map< int16_t, int32_t > labelToRankLut;
    gkg::Dictionary::const_iterator
      r = roiDictionary.begin(),
      re = roiDictionary.end();
    int32_t count = 0;
    while ( r != re )
    {

      std::string roiKey = r->first;
      int16_t label = 0;
      labelledRoiDictionary.getRoiLabel( roiKey, label );
      labelToRankLutDict[ gkg::StringConverter::toString( label ) ] = count;
      labelToRankLut[ label ] = count;
      rankToLabelLut[ count ] = label;

      ++ r;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating a 4D Atlas Volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating atlas : " << std::flush;

    }

    gkg::Volume< int16_t > atlas;
    gkg::Volume< int16_t > labelVolume;
    gkg::File outputFile( fileNameOut );
    std::vector< std::string >::const_iterator
      l = labelFileNameIns.begin(),
      le = labelFileNameIns.end();

    if ( outputFile.isExisting() )
    {

      gkg::Reader::getInstance().read( fileNameOut, atlas );

      if ( atlas.getHeader().hasAttribute( "data_base_count" ) )
      {

        double countDouble = 0.0;
        int32_t data_base_count = 0;
        atlas.getHeader().getAttribute( "data_base_count", countDouble );
        data_base_count = (int32_t)countDouble + 
                          (int32_t)labelFileNameIns.size();
        atlas.getHeader().addAttribute( "data_base_count", data_base_count );

      }

      sizeX = atlas.getSizeX();
      sizeY = atlas.getSizeY();
      sizeZ = atlas.getSizeZ();

    }
    else if ( talairach )
    {

      int32_t originX = 68; // Talairach AC position
      int32_t originY = 69;
      int32_t originZ = 75;

      atlas.reallocate( sizeX, sizeY, sizeZ, count );
      atlas.fill( 0 );
      atlas.getHeader().addAttribute( "resolutionX", 1.0 );
      atlas.getHeader().addAttribute( "resolutionY", 1.0 );
      atlas.getHeader().addAttribute( "resolutionZ", 1.0 );
      atlas.getHeader().addAttribute( "resolutionT", 1.0 );
      atlas.getHeader().addAttribute( "label_to_rank_lut",
                                      labelToRankLutDict );
      atlas.getHeader().addAttribute( "data_base_count",
                                      ( int32_t )labelFileNameIns.size() );
      std::vector< int32_t > anteriorCommissure( 3 );
      anteriorCommissure[ 0 ] = originX;
      anteriorCommissure[ 1 ] = originY;
      anteriorCommissure[ 2 ] = originZ;
      atlas.getHeader().addAttribute( "talairach_AC_coordinates",
                                      anteriorCommissure );
      atlas.getHeader().addAttribute( labelledRoiDictionary.getName(),
                                      labelledRoiDictionary.getDictionary() );

    }
    else
    {

      gkg::Vector3d< double > resolution;

      gkg::Volume< int16_t > labelVolume;
      gkg::TypedVolumeReaderProcess< int16_t > readerProcess( labelVolume );

      readerProcess.execute( *l );

      sizeX = labelVolume.getSizeX();
      sizeY = labelVolume.getSizeY();
      sizeZ = labelVolume.getSizeZ();
      labelVolume.getResolution( resolution );

      atlas.reallocate( sizeX, sizeY, sizeZ, count );
      atlas.fill( 0 );
      atlas.getHeader().addAttribute( "resolutionX", resolution.x );
      atlas.getHeader().addAttribute( "resolutionY", resolution.y );
      atlas.getHeader().addAttribute( "resolutionZ", resolution.z );
      atlas.getHeader().addAttribute( "resolutionT", 1.0 );
      atlas.getHeader().addAttribute( "label_to_rank_lut",
                                      labelToRankLutDict );
      atlas.getHeader().addAttribute( "data_base_count",
                                      ( int32_t )labelFileNameIns.size() );
      atlas.getHeader().addAttribute( labelledRoiDictionary.getName(),
                                      labelledRoiDictionary.getDictionary() );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // filling the atlas volume
    ////////////////////////////////////////////////////////////////////////////

    std::map< int16_t, int32_t >:: iterator
      lr, lre = labelToRankLut.end();

    while ( l != le )
    {

      //////////////////////////////////////////////////////////////////////////
      // reading label volume
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "reading '" << *l << "' : " << std::flush;

      }
      gkg::Volume< int16_t > labelVolume;
      gkg::TypedVolumeReaderProcess< int16_t > readerProcess( labelVolume );

      readerProcess.execute( *l );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      if ( talairach )
      {

        ////////////////////////////////////////////////////////////////////////
        // processing Talairach transform
        ////////////////////////////////////////////////////////////////////////

        gkg::TalairachTransform3d< float > talairachTransform3d( labelVolume,
                                                                 atlas );

        gkg::BoundingBox< int32_t > labelBoundingBox( labelVolume );


        ////////////////////////////////////////////////////////////////////////
        // looping over voxel(s)
        ////////////////////////////////////////////////////////////////////////

        gkg::Vector3d< int32_t > from;
        gkg::Vector3d< int32_t > to;

        for ( from.z = 0; from.z < sizeZ; from.z ++ )
        {

          for ( from.y = 0; from.y < sizeY; from.y ++ )
          {

            for ( from.x = 0; from.x < sizeX; from.x ++ )
            {

              talairachTransform3d.getFullIntegerInverse( from, to );

              if ( labelBoundingBox.contains( to ) )
              {

                lr = labelToRankLut.find( labelVolume( to ) );
                if ( lr != lre )
                {

                  ++atlas( from, lr->second );

                }

              }

            }

          }

        }

      }
      else
      {

        gkg::Vector3d< int32_t > site;

        for ( site.z = 0; site.z < sizeZ; site.z ++ )
        {

          for ( site.y = 0; site.y < sizeY; site.y ++ )
          {

            for ( site.x = 0; site.x < sizeX; site.x ++ )
            {

              lr = labelToRankLut.find( labelVolume( site ) );
              if ( lr != lre )
              {

                ++atlas( site, lr->second );

              }

            }

          }

        }

      }

      ++l;

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing atlas
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, atlas, ascii, format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving ROI name to rank LUT
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameRoiNameToRank.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameRoiNameToRank
                  << "' : " << std::flush;

      }

      int16_t roiLabel = 0;
      std::string roiName;
      int32_t roiRank = 0;
      std::ofstream os( fileNameRoiNameToRank.c_str() );
      os << labelledRoiDictionary.getRoiCount() << std::endl;
      gkg::Dictionary::const_iterator id = roiDictionary.begin(),
                                      ide = roiDictionary.end();
      while ( id != ide )
      {

        roiName = id->first;
        labelledRoiDictionary.getRoiLabel( roiName, roiLabel );
        roiRank = labelToRankLut[ roiLabel ];
        os << roiName << " " << roiLabel << " " << roiRank << std::endl;
        ++ id;

      }

      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::OccurrenceAtlasCommand::execute( "
             "const std::vector< std::string >& labelFileNameIns, "
             "const std::string& roiDictionaryFileName, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameRoiNameToRank, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
                  OccurrenceAtlasCommand,
    		  DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( labelFileNameIns ) +
    		  DECLARE_STRING_PARAMETER_HELP( roiDictionaryFileName ) +
    		  DECLARE_BOOLEAN_PARAMETER_HELP( talairach ) +
    		  DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    		  DECLARE_STRING_PARAMETER_HELP( fileNameRoiNameToRank ) +
    		  DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		  DECLARE_STRING_PARAMETER_HELP( format ) +
    		  DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
