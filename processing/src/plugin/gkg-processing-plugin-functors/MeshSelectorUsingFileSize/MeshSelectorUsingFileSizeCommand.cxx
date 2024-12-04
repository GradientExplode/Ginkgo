#include <gkg-processing-plugin-functors/MeshSelectorUsingFileSize/MeshSelectorUsingFileSizeCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-cppext/CUniStd.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <set>
#include <map>
#include <fstream>


gkg::MeshSelectorUsingFileSizeCommand::MeshSelectorUsingFileSizeCommand(
                                 			      int32_t argc,
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
  GKG_CATCH( "gkg::MeshSelectorUsingFileSizeCommand::MeshSelectorUsingFileSizeCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MeshSelectorUsingFileSizeCommand::MeshSelectorUsingFileSizeCommand(
                              const std::string& inputDirectoryName,
                              const std::string& inputItkSnapLutName,
                              const std::string& outputDirectoryName,
                              const std::list< std::string >& excludedLabelList,
                              float transparency,
                              bool verbose )
                                     : gkg::Command()
{

  try
  {

    execute( inputDirectoryName, 
             inputItkSnapLutName,
             outputDirectoryName,
             excludedLabelList,
             transparency,
             verbose );

  }
  GKG_CATCH( "gkg::MeshSelectorUsingFileSizeCommand::MeshSelectorUsingFileSizeCommand( "
             "const std::string& inputDirectoryName, "
             "const std::string& inputItkSnapLutName, "
             "const std::string& outputDirectoryName, "
             "const std::list< std::string >& excludedLabelList, "
             "float transparency, "
             "bool verbose )" );

}


gkg::MeshSelectorUsingFileSizeCommand::MeshSelectorUsingFileSizeCommand(
                                             const gkg::Dictionary& parameters )
                                     : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, inputDirectoryName );
    DECLARE_STRING_PARAMETER( parameters, std::string, inputItkSnapLutName );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputDirectoryName );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters,
                                       std::list< std::string >,
                                       excludedLabelList );
    DECLARE_FLOATING_PARAMETER( parameters, float, transparency );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( inputDirectoryName, 
             inputItkSnapLutName,
             outputDirectoryName,
             excludedLabelList,
             transparency,
             verbose );

  }
  GKG_CATCH( "gkg::MeshSelectorUsingFileSizeCommand::"
             "MeshSelectorUsingFileSizeCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MeshSelectorUsingFileSizeCommand::~MeshSelectorUsingFileSizeCommand()
{
}


std::string gkg::MeshSelectorUsingFileSizeCommand::getStaticName()
{

  try
  {

    return "MeshSelectorUsingFileSize";

  }
  GKG_CATCH( "std::string gkg::MeshSelectorUsingFileSizeCommand::getStaticName()" );

}


void gkg::MeshSelectorUsingFileSizeCommand::parse()
{

  try
  {

    std::string inputDirectoryName;
    std::string inputItkSnapLutName = "";
    std::string outputDirectoryName;
    std::list< std::string > excludedLabelList;
    float transparency = 1.0f;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Mesh selector of the biggest components",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input directory name",
                                 inputDirectoryName );
    application.addSingleOption( "-l",
                                 "Input ITKSnap LUT name",
                                 inputItkSnapLutName,
                                 true );
    application.addSeriesOption( "-excludedLabels",
                                 "List of excluded label name(s)",
                                 excludedLabelList );
    application.addSingleOption( "-o",
                                 "Output directory name",
                                 outputDirectoryName );
    application.addSingleOption( "-t",
                                 "Mesh transparency",
                                 transparency,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( inputDirectoryName, 
             inputItkSnapLutName,
             outputDirectoryName,
             excludedLabelList,
             transparency,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MeshSelectorUsingFileSizeCommand::parse()" );

}


void gkg::MeshSelectorUsingFileSizeCommand::execute(
                  	      const std::string& inputDirectoryName,
                  	      const std::string& inputItkSnapLutName,
                              const std::string& outputDirectoryName,
                              const std::list< std::string >& excludedLabelList,
                              float transparency,
                              bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( inputDirectoryName == outputDirectoryName )
    {

      throw std::runtime_error( "input and output directory are the same" );

    }

    std::set< std::string > excludedLabelNames( excludedLabelList.begin(),
                                                excludedLabelList.end() );


    ////////////////////////////////////////////////////////////////////////////
    // analyzing the input directory
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "analyzing '" << inputDirectoryName << "' directory : "
                << std::flush;

    }

    // opening the input directory and parsing files
    gkg::Directory inputDirectory( inputDirectoryName );
    if ( !inputDirectory.isValid() )
    {

      throw std::runtime_error( "input directory is not valid" );

    }
    std::set< std::string > fileNames = inputDirectory.getFiles();
    fileNames.erase( "." );
    fileNames.erase( ".." );

    // building a lookup table with [label root name / index / file size]
    std::map< std::string, std::map< int32_t, uint64_t > > lut;
    std::set< std::string >::const_iterator
      n = fileNames.begin(),
      ne = fileNames.end();
    int32_t index = 0;
    while ( n != ne )
    {

      if ( n->substr( n->rfind( "." ), n->length() - n->rfind( "." ) + 1 ) ==
           ".mesh" )
      {

        std::string labelRootName = n->substr( 0, n->rfind( "_" ) );
        if ( excludedLabelNames.find( labelRootName ) ==
             excludedLabelNames.end() )
        {

          gkg::StringConverter::toScalar(
                              n->substr( n->rfind( "_" ) + 1,
                                         n->rfind( "." ) - n->rfind( "_" ) -1 ),
                              index );
          std::string fileName = gkg::getCanonicalDirectory(
                                                      inputDirectoryName ) + *n;
          gkg::File file( fileName );
          lut[ labelRootName ][ index ] = file.getSize();

        }

      }
      ++ n;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // analyzing the ITK snap LUT
    ////////////////////////////////////////////////////////////////////////////

    std::map< std::string, gkg::RGBComponent > roiToColorMap;
    if ( !inputItkSnapLutName.empty() )
    {

      if ( verbose )
      {

        std::cout << "analyzing '"
                  << inputItkSnapLutName
                  << "' ITK snap LUT file : "
                  << std::flush;

      }

      analyzeITKSnapFile( inputItkSnapLutName, roiToColorMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating the output directory
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "creating '"
                << gkg::getCanonicalDirectory( outputDirectoryName )
                << "' directory : "
                << std::flush;

    }

    // creating output directory if not present
    gkg::Directory outputDirectory( outputDirectoryName );
    if ( !outputDirectory.isValid() )
    {

      outputDirectory.mkdir();

    }

    // processing indices of the biggest file per label
    std::string originFileName;
    std::string destinationFileName;
    std::map< std::string, std::map< int32_t, uint64_t > >::const_iterator
      l = lut.begin(),
      le = lut.end();
    while ( l != le )
    {

      std::string labelRootName = l->first;
      int32_t biggestFileIndex = -1;
      uint64_t biggestFileSize = 0U;
      std::map< int32_t, uint64_t >::const_iterator
        s = l->second.begin(),
        se = l->second.end();
      while ( s != se )
      {

        if ( s->second > biggestFileSize )
        {

          biggestFileIndex = s->first;
          biggestFileSize = s->second;

        }
        ++ s;

      }

      // copying .mesh file(s)
      originFileName = gkg::getCanonicalDirectory( inputDirectoryName ) +
                       labelRootName + '_' +
                       gkg::StringConverter::toString( biggestFileIndex ) +
                       ".mesh";
      destinationFileName =
                       gkg::getCanonicalDirectory( outputDirectoryName ) +
                       labelRootName + '_' +
                       gkg::StringConverter::toString( biggestFileIndex ) +
                       ".mesh";

      int32_t status = link( originFileName.c_str(), 
                             destinationFileName.c_str() );
			     
      if ( status < 0 )
      {
      
        throw std::runtime_error( "Error linking mesh file" );
      
      }

      // creating .mesh.minf file if itk snap LUT is provided
      std::map< std::string, gkg::RGBComponent >::const_iterator
        c = roiToColorMap.find( labelRootName.substr(
                                  labelRootName.rfind( "_" ) + 1 ) );
      if ( c != roiToColorMap.end() )
      {

        destinationFileName =
                       gkg::getCanonicalDirectory( outputDirectoryName ) +
                       labelRootName + '_' +
                       gkg::StringConverter::toString( biggestFileIndex ) +
                       ".mesh.minf";
        std::ofstream os( destinationFileName.c_str() );

        os << "attributes = {"
           << std::endl;
        os << "  \"material\" : { \"diffuse\" : [ "
           << ( float )c->second.r / 255.0 << ", "
           << ( float )c->second.g / 255.0 << ", "
           << ( float )c->second.b / 255.0 << ", "
           << transparency
           << " ] }"
           << std::endl;
        os << "}"
           << std::endl;

        os.close();


      }

      ++ l;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::MeshSelectorUsingFileSizeCommand::execute( "
             "const std::string& inputDirectoryName, "
             "const std::string& inputItkSnapLutName, "
             "const std::string& outputDirectoryName, "
             "const std::list< std::string >& excludedLabelList, "
             "float transparency, "
             "bool verbose )" );

}


void gkg::MeshSelectorUsingFileSizeCommand::analyzeITKSnapFile( 
                                     const std::string& itkSnapFileName,
                             std::map< std::string, gkg::RGBComponent >& 
                               roiToColorMap ) const
{

  try
  {

    std::ifstream is( itkSnapFileName.c_str() );


    std::string line;

    // reading the comments (head of the file)
    int32_t l = 0;
    for ( l = 0; l < 14; l++ )
    {

      std::getline( is, line );

    }

    std::string label;
    int32_t r = 0;
    int32_t g = 0;
    int32_t b = 0;
    while ( !is.eof() )
    {

      std::getline( is, line );

      std::stringstream ss;
      ss << line;

      ss >> label;
      ss >> r;
      ss >> g;
      ss >> b;

      //std::cout << label << " -> " 
      //          << r << " " << g << " " << b << " " << std::endl;

      roiToColorMap[ label ] = gkg::RGBComponent( ( uint8_t )r,
                                                  ( uint8_t )g,
                                                  ( uint8_t )b );

    }

    is.close();


  }
  GKG_CATCH( "void gkg::MeshSelectorUsingFileSizeCommand::analyzeITKSnapFile( " 
             " const std::string& itkSnapFileName, "
             "std::map< std::string, gkg::RGBComponent >& "
             "roiToColorMap ) const" );

}


RegisterCommandCreator(
                   MeshSelectorUsingFileSizeCommand,
                   DECLARE_STRING_PARAMETER_HELP( inputDirectoryName ) +
                   DECLARE_STRING_PARAMETER_HELP( inputItkSnapLutName ) +
                   DECLARE_STRING_PARAMETER_HELP( outputDirectoryName ) +
                   DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( excludedLabelList ) +
                   DECLARE_FLOATING_PARAMETER_HELP( transparency ) +
                   DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
