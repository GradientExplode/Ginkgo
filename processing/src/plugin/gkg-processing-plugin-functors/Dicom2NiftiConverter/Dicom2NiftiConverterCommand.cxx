#include <gkg-processing-plugin-functors/Dicom2NiftiConverter/Dicom2NiftiConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>


//
// class Dicom2NiftiProcess
//

class Dicom2NiftiProcess : public gkg::Process
{

  public:

    Dicom2NiftiProcess( const std::string& fileNameOut,
                        bool verbose );

    const std::string& getFileNameOut() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void read( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    bool _verbose;

};


Dicom2NiftiProcess::Dicom2NiftiProcess( const std::string& fileNameOut,
                                        bool verbose )
                  : gkg::Process( "Volume" ),
                    _fileNameOut( fileNameOut ),
                    _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &Dicom2NiftiProcess::read< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &Dicom2NiftiProcess::read< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &Dicom2NiftiProcess::read< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &Dicom2NiftiProcess::read< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &Dicom2NiftiProcess::read< float > );

}


const std::string& Dicom2NiftiProcess::getFileNameOut() const
{

  return _fileNameOut;

}


bool Dicom2NiftiProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void Dicom2NiftiProcess::read( gkg::Process& process,
                               const std::string& fileNameIn,
                               const gkg::AnalyzedObject&,
                               const std::string& )
{

  try
  {

    Dicom2NiftiProcess&
      dicom2NiftiProcess = static_cast< Dicom2NiftiProcess& >( process );

    // checking that data is readable and collecting sizes
    if ( dicom2NiftiProcess.getVerbose() )
    {

      std::cout << "reading DICOM data "
                << fileNameIn << ": "
                << std::flush;

    }
    std::string format;
    gkg::Volume< T > volume;
    format = gkg::Reader::getInstance().template read< gkg::Volume< T > >(
                                                                     fileNameIn,
                                                                     volume,
                                                                     "dicom" );

    if ( format != "dicom" )
    {

      throw std::runtime_error( "wrong format, should be \'dicom\'");

    }
    if ( dicom2NiftiProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

    }

    // writing output
    if ( dicom2NiftiProcess.getVerbose() )
    {

      std::cout << "writing volume in NIFTI format: "
                << std::flush;

    }
    gkg::Writer::getInstance().template write< gkg::Volume< T > >(
                                            dicom2NiftiProcess.getFileNameOut(),
                                            volume,
                                            false,
                                            "nifti" );
    if ( dicom2NiftiProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

    }

    // Writing .bvec and .bval files for diffusion datasets
    if ( volume.getHeader().hasAttribute( "b_values" ) &&
         volume.getHeader().hasAttribute( "diffusion_directions" ) )
    {

      gkg::TypedDiskFormat< gkg::Volume< T > >* diskFormat =
        gkg::DiskFormatFactory< gkg::Volume< T > >::getInstance().getDiskFormat(
                                                                      "nifti" );

      if ( diskFormat )
      {

        std::string nameWithoutExtension = diskFormat->getNameWithoutExtension(
                                          dicom2NiftiProcess.getFileNameOut() );
        std::string bValFileName = nameWithoutExtension + ".bval";
        std::string bVecFileName = nameWithoutExtension + ".bvec";
        std::vector< double > bValues( volume.getSizeT(), 0.0 );
        std::vector< gkg::Vector3d< double > > bVectors;
        gkg::GenericObjectList golDiffDir;

        volume.getHeader().getAttribute( "b_values", bValues );
        volume.getHeader().getAttribute( "diffusion_directions", golDiffDir );

        gkg::GenericObjectList::iterator
          d = golDiffDir.begin(),
          de = golDiffDir.end();

        while ( d != de )
        {

          gkg::GenericObjectList direction = 
                                     (*d)->getValue< gkg::GenericObjectList >();

          gkg::Vector3d< double > dirVec( direction[ 0 ]->getScalar(),
                                          direction[ 1 ]->getScalar(),
                                          direction[ 2 ]->getScalar() );

          bVectors.push_back( dirVec );
          ++d;

        }

        // writing bval
        std::ofstream ofs( bValFileName.c_str() );

        if ( ofs.bad() )
        {
    
          throw std::runtime_error( "Cannot create .bval file" );
    
        }

        if ( dicom2NiftiProcess.getVerbose() )
        {

          std::cout << "Writing " << bValFileName << std::flush;

        }

        std::vector< double >::const_iterator
          v = bValues.begin(),
          ve = bValues.end();

        while ( v != ve )
        {

          ofs << *v << " ";
          ++v;

        }

        ofs << std::endl;
        ofs.close();

        if ( dicom2NiftiProcess.getVerbose() )
        {

          std::cout << " done" << std::endl;

        }

        // writing bvec
        ofs.open( bVecFileName.c_str() );

        if ( ofs.bad() )
        {
    
          throw std::runtime_error( "Cannot create .bvec file" );
    
        }

        if ( dicom2NiftiProcess.getVerbose() )
        {

          std::cout << "Writing " << bVecFileName << std::flush;

        }

        // x
        std::vector< gkg::Vector3d< double > >::const_iterator
          dir = bVectors.begin(),
          dire = bVectors.end();

        while ( dir != dire )
        {

          ofs << dir->x << " ";
          ++dir;

        }

        ofs << std::endl;

        // y
        dir = bVectors.begin();

        while ( dir != dire )
        {

          ofs << dir->y << " ";
          ++dir;

        }

        ofs << std::endl;

        // z
        dir = bVectors.begin();

        while ( dir != dire )
        {

          ofs << dir->z << " ";
          ++dir;

        }

        ofs << std::endl;
        ofs.close();

        if ( dicom2NiftiProcess.getVerbose() )
        {

          std::cout << " done" << std::endl;

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void Dicom2NiftiProcess::read( gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


gkg::Dicom2NiftiConverterCommand::Dicom2NiftiConverterCommand(
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
  GKG_CATCH( "gkg::Dicom2NiftiConverterCommand::Dicom2NiftiConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Dicom2NiftiConverterCommand::Dicom2NiftiConverterCommand(
                                                 const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 bool verbose )
                                : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::Dicom2NiftiConverterCommand::Dicom2NiftiConverterCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, bool verbose )" );

}


gkg::Dicom2NiftiConverterCommand::Dicom2NiftiConverterCommand(
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
  GKG_CATCH( "gkg::Dicom2NiftiConverterCommand::Dicom2NiftiConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Dicom2NiftiConverterCommand::~Dicom2NiftiConverterCommand()
{
}


std::string gkg::Dicom2NiftiConverterCommand::getStaticName()
{

  try
  {

    return "Dicom2NiftiConverter";

  }
  GKG_CATCH( "std::string gkg::Dicom2NiftiConverterCommand::getStaticName()" );

}


void gkg::Dicom2NiftiConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "DICOM to NIFTI disk format converter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input DICOM directory name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output NIFTI image file name",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Dicom2NiftiConverterCommand::parse()" );

}


void gkg::Dicom2NiftiConverterCommand::execute(
                  	     const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 bool verbose )
{

  try
  {

    Dicom2NiftiProcess dicom2NiftiProcess( fileNameOut, verbose );
    dicom2NiftiProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::Dicom2NiftiConverterCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, bool verbose )" );

}


RegisterCommandCreator( Dicom2NiftiConverterCommand ,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
