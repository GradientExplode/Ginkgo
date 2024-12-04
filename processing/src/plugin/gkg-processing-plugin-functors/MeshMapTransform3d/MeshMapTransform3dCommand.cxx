#include <gkg-processing-plugin-functors/MeshMapTransform3d/MeshMapTransform3dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-processing-mesh/MeshTransformer_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <vector>
#include <iostream>
#include <list>


//
// class MeshMapTransform3dProcess
//


class MeshMapTransform3dProcess : public gkg::Process
{

  public:

    MeshMapTransform3dProcess(
                      const std::string& theFileNameOut,
                      const std::vector< std::string >& theFileNameTransform3ds,
                      const bool& theVerbose );

    const std::string& fileNameOut;
    const std::vector< std::string >& fileNameTransform3ds;
    const bool& verbose;

  private:

    template < class R >
    static void transform( gkg::Process& process,
                           const std::string& fileNameIn,
                           const gkg::AnalyzedObject&,
                           const std::string& );


};


MeshMapTransform3dProcess::MeshMapTransform3dProcess(
                      const std::string& theFileNameOut,
                      const std::vector< std::string >& theFileNameTransform3ds,
                      const bool& theVerbose )
                          : gkg::Process( "MeshMap" ),
                           fileNameOut( theFileNameOut ),
                           fileNameTransform3ds( theFileNameTransform3ds ),
                           verbose( theVerbose )
{

  try
  {

    registerProcess( "MeshMap", gkg::TypeOf< int32_t >::getName(),
                     &MeshMapTransform3dProcess::transform< int32_t > );

  }
  GKG_CATCH( "MeshMapTransform3dProcess::"
             "MeshMapTransform3dProcess( "
	     "const std::string& theFileNameOut, "
	     "const std::vector< std::string >& theFileNameTransform3ds, "
	     "const bool& theVerbose )" );

}


template < class R >
void 
MeshMapTransform3dProcess::transform( gkg::Process& process,
 				      const std::string& fileNameIn,
 				      const gkg::AnalyzedObject&,
 				      const std::string& )
{

  try
  {

    MeshMapTransform3dProcess&
      meshMapTransform3dProcess =
        static_cast< MeshMapTransform3dProcess& >( process );

    const std::string& 
      fileNameOut = meshMapTransform3dProcess.fileNameOut;
    const std::vector< std::string >& 
      fileNameTransform3ds = meshMapTransform3dProcess.fileNameTransform3ds;
    const bool&
      verbose = meshMapTransform3dProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( ( fileNameTransform3ds.size() != 1U ) &&
         ( fileNameTransform3ds.size() != 3U ) )
    {

      throw std::runtime_error( "1 or 3 files needed for 3D transformations" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading input mesh map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "reading input '" << fileNameIn << "' mesh map : "
                << std::flush;
     
    }
    gkg::MeshMap< R, float, 3U > meshMap;
    std::string format = gkg::Reader::getInstance().read( fileNameIn,
                                                          meshMap );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }


    ////////////////////////////////////////////////////////////////////////////
    // reading 3D transform
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {
     
      std::cout << "reading 3D transform : "
                << std::flush;
     
    }

    gkg::RCPointer< gkg::Transform3d< float > > transform3d;
    if ( fileNameTransform3ds.size() == 1U )
    {

      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream is( fileNameTransform3ds[ 0 ].c_str() );
        if ( is.fail() )
        {

          throw std::runtime_error( std::string( "error while opening " ) +
                                    fileNameTransform3ds[ 0 ] );

        }
        rigidTransform3d->readTrm( is );
        is.close();
        transform3d.reset( rigidTransform3d );

      }
      catch ( std::exception& )
      {

        try
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          std::ifstream is( fileNameTransform3ds[ 0 ].c_str() );
          if ( is.fail() )
          {

            throw std::runtime_error( std::string( "error while opening " ) +
                                      fileNameTransform3ds[ 0 ] );

          }
          affineTransform3d->readTrm( is );
          is.close();
          transform3d.reset( affineTransform3d );

        }
        catch ( std::exception& )
        {

          throw std::runtime_error(
                                "unable to find an adequate linear transform" );

        }

      }

    }
    else
    {

      gkg::NonLinearTransform3d< float >*
        nonLinearTransform3d = new gkg::NonLinearTransform3d< float >;
      nonLinearTransform3d->readTrm( fileNameTransform3ds[ 0 ],
                                     fileNameTransform3ds[ 1 ],
                                     fileNameTransform3ds[ 2 ] );
      transform3d.reset( nonLinearTransform3d );

    }

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }


    //////////////////////////////////////////////////////////////////////////
    // transforming mesh map
    //////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "transforming mesh : " << std::flush;

    }
    
    gkg::MeshMap< R, float, 3U > outputMeshMap;
    gkg::MeshTransformer< R, float, 3U > meshTransformer;
    meshTransformer.transform( meshMap, *transform3d, outputMeshMap );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }


    ////////////////////////////////////////////////////////////////////////////
    // writing transformed bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "writing output '" << fileNameOut << "' mesh map : "
                << std::flush;
     
    }
    gkg::Writer::getInstance().write( fileNameOut,
                                      outputMeshMap,
                                      false,
                                      format );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }

  }
  GKG_CATCH( "template < class R > "
             "void MeshMapTransform3dProcess::transform( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// MeshMapTransform3dCommand
//


gkg::MeshMapTransform3dCommand::MeshMapTransform3dCommand( int32_t argc,
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
  GKG_CATCH( "gkg::MeshMapTransform3dCommand::MeshMapTransform3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MeshMapTransform3dCommand::MeshMapTransform3dCommand(
                    	 const std::string& fileNameMeshMap,
                    	 const std::string& fileNameOut,
                    	 const std::vector< std::string >& fileNameTransform3ds,
                    	 bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( fileNameMeshMap, fileNameOut, fileNameTransform3ds, verbose );

  }
  GKG_CATCH( "gkg::MeshMapTransform3dCommand::MeshMapTransform3dCommand( "
             "const std::string& fileNameMeshMap, "
             "const std::string& fileNameOut, "
             "const std::vector< std::string >& fileNameTransform3ds, "
             "bool verbose )" );

}


gkg::MeshMapTransform3dCommand::MeshMapTransform3dCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMeshMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameTransform3ds );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameMeshMap, fileNameOut, fileNameTransform3ds, verbose );

  }
  GKG_CATCH( "gkg::MeshMapTransform3dCommand::MeshMapTransform3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MeshMapTransform3dCommand::~MeshMapTransform3dCommand()
{
}


std::string gkg::MeshMapTransform3dCommand::getStaticName()
{

  try
  {

    return "MeshMapTransform3d";

  }
  GKG_CATCH( "std::string gkg::MeshMapTransform3dCommand::getStaticName()" );

}


void gkg::MeshMapTransform3dCommand::parse()
{

  try
  {

    std::string fileNameMeshMap;
    std::string fileNameOut;
    std::vector< std::string > fileNameTransform3ds;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "Bundle map 3D transformer",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input mesh map",
                                 fileNameMeshMap );
    application.addSingleOption( "-o",
                                 "Output mesh map",
                                 fileNameOut );
    application.addSeriesOption( "-t",
                                 "3D transform *.trm[/*.ima] filename",
                                 fileNameTransform3ds,
                                 1, 3 );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 

    application.initialize(); 			 

    execute( fileNameMeshMap, fileNameOut, fileNameTransform3ds, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MeshMapTransform3dCommand::parse()" );

}


void gkg::MeshMapTransform3dCommand::execute(
                       	 const std::string& fileNameMeshMap,
                       	 const std::string& fileNameOut,
                       	 const std::vector< std::string >& fileNameTransform3ds,
                       	 bool verbose )
{

  try
  {

    MeshMapTransform3dProcess meshMapTransform3dProcess( fileNameOut,
                                                         fileNameTransform3ds,
                                                         verbose );
    meshMapTransform3dProcess.execute( fileNameMeshMap );

  }
  GKG_CATCH( "void gkg::MeshMapTransform3dCommand::execute( "
             "const std::string& fileNameMeshMap, "
             "const std::string& fileNameOut, "
             "const std::vector< std::string >& fileNameTransform3ds, "
             "bool verbose )" );

}


RegisterCommandCreator( MeshMapTransform3dCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameMeshMap ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( 
                                                        fileNameTransform3ds ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
