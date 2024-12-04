#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-anatomist-plugin-objects/CartoVolume.h>
#include <gkg-anatomist-plugin-objects/AimsVolume.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>
#include <anatomist/volume/Volume.h>
#include <string>
#include <iostream>


class SomaPluginProcess : public gkg::Process
{

  public:

    SomaPluginProcess();

  private:

  template < class T >
  static void conversion( gkg::Process& process, 
                          const std::string& fileName,
                          const gkg::AnalyzedObject&,
                          const std::string& );

};


SomaPluginProcess::SomaPluginProcess()
                  : gkg::Process()
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &SomaPluginProcess::conversion< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &SomaPluginProcess::conversion< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &SomaPluginProcess::conversion< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &SomaPluginProcess::conversion< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &SomaPluginProcess::conversion< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &SomaPluginProcess::conversion< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &SomaPluginProcess::conversion< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &SomaPluginProcess::conversion< double > );

}


template < class T >
void SomaPluginProcess::conversion( gkg::Process&,
                                    const std::string& fileName,
                                    const gkg::AnalyzedObject&,
                                    const std::string& )
{

  try
  {

    // Gkg level
    gkg::RCPointer< gkg::Volume< T > > volume( new gkg::Volume< T > );
    gkg::Reader::getInstance().read( fileName, *volume );

    std::cout << "Gkg volume : ";
    std::cout << volume->getSizeX() << "x";
    std::cout << volume->getSizeY() << "x";
    std::cout << volume->getSizeZ() << "x";
    std::cout << volume->getSizeT() << std::endl;

    // Cartodata level
    gkg::CartoVolume< T > cartoVolume( volume );
    int32_t sizeX = cartoVolume->getSizeX();
    int32_t sizeY = cartoVolume->getSizeY();
    int32_t sizeZ = cartoVolume->getSizeZ();
    int32_t sizeT = cartoVolume->getSizeT();
    std::cout << "Gkg cartoVolume : ";
    std::cout << sizeX << "x";
    std::cout << sizeY << "x";
    std::cout << sizeZ << "x";
    std::cout << sizeT;

    carto::Object vs = cartoVolume->header().getProperty( "voxel_size" );

    if ( !vs.isNull() && vs->size() >= 4 )
    {

      std::cout << " (";
      std::cout << (float)vs->getArrayItem( 0 )->getScalar() << "x";
      std::cout << (float)vs->getArrayItem( 1 )->getScalar() << "x";
      std::cout << (float)vs->getArrayItem( 2 )->getScalar() << "x";
      std::cout << (float)vs->getArrayItem( 3 )->getScalar() << ")";

    }

    std::cout << std::endl;

    // Aims level
    gkg::AimsVolume< T > aimsVolume( volume );
    std::cout << "Gkg aimsVolume : ";
    std::cout << aimsVolume->dimX() << "x";
    std::cout << aimsVolume->dimY() << "x";
    std::cout << aimsVolume->dimZ() << "x";
    std::cout << aimsVolume->dimT() << " (";
    std::cout << aimsVolume->sizeX() << "x";
    std::cout << aimsVolume->sizeY() << "x";
    std::cout << aimsVolume->sizeZ() << "x";
    std::cout << aimsVolume->sizeT() << ")" << std::endl;

    std::cout << "===> Test CartoVolume" << std::endl;
    typename gkg::CartoVolume< T >::iterator v = cartoVolume.begin();
    int32_t x, y, z, t;
    for ( t = 0; t < sizeT; t++ )
    {

      std::cout << "Frame = " << t << ":" << std::endl;

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            std::cout << *v++ << " ";

          }

          std::cout << std::endl;

        }

        std::cout << std::endl;

      }

      std::cout << std::endl;

    }

    std::cout << "===> Test AimsVolume" << std::endl;
    typename AimsData< T >::iterator a = aimsVolume->begin();
    for ( t = 0; t < sizeT; t++ )
    {

      std::cout << "Frame = " << t << ":" << std::endl;

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            std::cout << *a++ << " ";

          }

          std::cout << std::endl;

        }

        std::cout << std::endl;

      }

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void SomaPluginProcess::conversion( "
             "gkg::Process& process, "
             "const std::string& fileName, "
             "const gkg::AnalyzedObject&, "                       
             "const std::string& )" );                            

}


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileName;

    gkg::Application application( argc, argv,
                                  "Soma plugin test" );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileName );
    application.initialize();

    SomaPluginProcess somaPluginProcess;

    somaPluginProcess.execute( fileName );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
