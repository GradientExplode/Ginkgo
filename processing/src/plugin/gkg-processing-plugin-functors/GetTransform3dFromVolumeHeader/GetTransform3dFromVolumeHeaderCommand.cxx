#include <gkg-processing-plugin-functors/GetTransform3dFromVolumeHeader/GetTransform3dFromVolumeHeaderCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <fstream>


//
//   GetTransform3dProcess
//


class GetTransform3dProcess : public gkg::Process
{

  public:

    GetTransform3dProcess( const std::string& fileNameOut,
                           const bool& verbose );

    const std::string& getFileNameOut() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void getTransform3d( gkg::Process& process,
                                const std::string& fileNameIn,
                                const gkg::AnalyzedObject&,
                                const std::string& );

    const std::string& _fileNameOut;
    const bool& _verbose;

};


GetTransform3dProcess::GetTransform3dProcess( const std::string& fileNameOut,
                                             const bool& verbose)
                      : gkg::Process( "Volume" ),
                        _fileNameOut( fileNameOut ),
                        _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &GetTransform3dProcess::getTransform3d< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &GetTransform3dProcess::getTransform3d< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &GetTransform3dProcess::getTransform3d< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &GetTransform3dProcess::getTransform3d< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &GetTransform3dProcess::getTransform3d< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &GetTransform3dProcess::getTransform3d< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &GetTransform3dProcess::getTransform3d< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &GetTransform3dProcess::getTransform3d< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &GetTransform3dProcess::getTransform3d< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &GetTransform3dProcess::getTransform3d< double > );

}


const std::string& GetTransform3dProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const bool& GetTransform3dProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void GetTransform3dProcess::getTransform3d( gkg::Process& process,
                                            const std::string& fileNameIn,
                                            const gkg::AnalyzedObject&,
                                            const std::string& )
{

  try
  {

    GetTransform3dProcess&
      getTransform3dProcess = static_cast< GetTransform3dProcess& >( process );

    // reading data
    if ( getTransform3dProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' proxy : " << std::flush;

    }
    gkg::Volume< T > volumeProxy;
    std::string format =
      gkg::Reader::getInstance().readHeader( fileNameIn, volumeProxy );

    if ( getTransform3dProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( getTransform3dProcess.getVerbose() )
    {

      std::cout << "getting 3D transform : " << std::flush;

    }

    std::vector< gkg::HomogeneousTransform3d< float > > vTransform3d;
    if ( volumeProxy.getHeader().hasAttribute( "transformations" ) )
    {

      gkg::GenericObjectList genericObjectList;
      volumeProxy.getHeader().getAttribute( "transformations", 
                                            genericObjectList );
      
      int32_t transformationCount = genericObjectList.size();
      int32_t t;

      for ( t = 0; t < transformationCount; t++ )
      {

        gkg::GenericObjectList gObjectList = 
                   genericObjectList[ t ]->getValue< gkg::GenericObjectList >();
        gkg::HomogeneousTransform3d< float > transform3d;

        transform3d.setDirectItems( gObjectList[  0 ]->getScalar(),
                                    gObjectList[  1 ]->getScalar(),
                                    gObjectList[  2 ]->getScalar(),
                                    gObjectList[  3 ]->getScalar(),
                                    gObjectList[  4 ]->getScalar(),
                                    gObjectList[  5 ]->getScalar(),
                                    gObjectList[  6 ]->getScalar(),
                                    gObjectList[  7 ]->getScalar(),
                                    gObjectList[  8 ]->getScalar(),
                                    gObjectList[  9 ]->getScalar(),
                                    gObjectList[ 10 ]->getScalar(),
                                    gObjectList[ 11 ]->getScalar(),
                                    gObjectList[ 12 ]->getScalar(),
                                    gObjectList[ 13 ]->getScalar(),
                                    gObjectList[ 14 ]->getScalar(),
                                    gObjectList[ 15 ]->getScalar() );

        vTransform3d.push_back( transform3d );

      }

      if ( getTransform3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( volumeProxy.getHeader().hasAttribute( "PVM_SPackArrGradOrient" ) )
    {

      gkg::GenericObjectList genericObjectList;
      volumeProxy.getHeader().getAttribute( "PVM_SPackArrGradOrient", 
                                            genericObjectList );

      gkg::HomogeneousTransform3d< float > transform3d;

      transform3d.setDirectItems( genericObjectList[  0 ]->getScalar(),
                                  genericObjectList[  1 ]->getScalar(),
                                  genericObjectList[  2 ]->getScalar(),
                                  0.0,
                                  genericObjectList[  3 ]->getScalar(),
                                  genericObjectList[  4 ]->getScalar(),
                                  genericObjectList[  5 ]->getScalar(),
                                  0.0,
                                  genericObjectList[  6 ]->getScalar(),
                                  genericObjectList[  7 ]->getScalar(),
                                  genericObjectList[  8 ]->getScalar(),
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  1.0 );

      vTransform3d.push_back( transform3d );

    }
    else
    {

      if ( getTransform3dProcess.getVerbose() )
      {

        std::cout << "not found" << std::endl;

      }

    }

    // writing transform
    std::vector< gkg::HomogeneousTransform3d< float > >::iterator
      h = vTransform3d.begin(),
      he = vTransform3d.end();

    std::string fileName = getTransform3dProcess.getFileNameOut();
    std::string::size_type pos = fileName.rfind( "." );
    std::string extension;
    std::string baseName;
    int32_t count = 1;

    if ( pos != std::string::npos )
    {

      extension = fileName.substr( pos, fileName.length() - pos );
      baseName = fileName.substr( 0, pos );

    }
    else
    {

      extension = ".trm";
      baseName = fileName;

    }

    while ( h != he )
    {

      std::string fileNameOut = baseName;

      if ( vTransform3d.size() > 1 )
      {

        std::ostringstream oss;
        oss << count;
        fileNameOut += oss.str();

      }

      fileNameOut +=  extension;

      if ( getTransform3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : "
                  << std::flush;

      }
      std::ofstream os( fileNameOut.c_str() );
      h->writeTrm( os );
      os.close();
      if ( getTransform3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      count++;
      ++h;

    }

  }
  GKG_CATCH( "template < class T > "
             "void GetTransform3dProcess::getTransform3d( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   GetTransform3dFromVolumeHeaderCommand
//


gkg::GetTransform3dFromVolumeHeaderCommand::
                                          GetTransform3dFromVolumeHeaderCommand( 
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                          : gkg::Command( argc,
                                                          argv, 
                                                          loadPlugin,
                                                          removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::GetTransform3dFromVolumeHeaderCommand::"
             "GetTransform3dFromVolumeHeaderCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GetTransform3dFromVolumeHeaderCommand::
                                          GetTransform3dFromVolumeHeaderCommand(
                                                 const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 bool verbose )
                                          : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::GetTransform3dFromVolumeHeaderCommand::"
             "GetTransform3dFromVolumeHeaderCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool verbose )" );

}


gkg::GetTransform3dFromVolumeHeaderCommand::
                                          GetTransform3dFromVolumeHeaderCommand(
                                             const gkg::Dictionary& parameters )
                                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::GetTransform3dFromVolumeHeaderCommand::"
             "GetTransform3dFromVolumeHeaderCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GetTransform3dFromVolumeHeaderCommand::
                                        ~GetTransform3dFromVolumeHeaderCommand()
{
}


std::string gkg::GetTransform3dFromVolumeHeaderCommand::getStaticName()
{

  try
  {

    return "GetTransform3dFromVolumeHeader";

  }
  GKG_CATCH( "std::string gkg::GetTransform3dFromVolumeHeaderCommand::"
             "getStaticName()" );

}


void gkg::GetTransform3dFromVolumeHeaderCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Threshold operation on container",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output 3D transform *.trm file name",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( 
                   "void gkg::GetTransform3dFromVolumeHeaderCommand::parse()" );

}


void gkg::GetTransform3dFromVolumeHeaderCommand::execute(
                  	                         const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 bool verbose )
{

  try
  {

    GetTransform3dProcess getTransform3dProcess( fileNameOut,
                                                 verbose );
    getTransform3dProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::GetTransform3dFromVolumeHeaderCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator( GetTransform3dFromVolumeHeaderCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
