#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>



int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::Application application( argc, argv,
                                  "Test Simulation 01" );
    application.initialize();

    gkg::UncommentCounterInputFileStream
      geometryUcis(
         ( gkg::ConfigurationInfo::getInstance().getHomePath() +
           gkg::getDirectorySeparator() +
           "Git/gkg/simulation/src/test/GkgTestSimulation02/" +
           "geometry.json" ).c_str() );
    gkg::BaseObjectReader geometryBaseObjectReader;
    gkg::TypedObject< gkg::Dictionary > geometryDictionary;
    geometryBaseObjectReader.read( geometryUcis, geometryDictionary );
    geometryUcis.close();


    std::cout << "creating now virtual tissue : " << std::endl;

    gkg::RCPointer< gkg::Dictionary > populationTissuePropertyDictionary;
    gkg::RCPointer< gkg::VirtualTissue >
      virtualTissue(
                 new gkg::VirtualTissue( geometryDictionary.getTypedValue(),
                                         populationTissuePropertyDictionary ) );

    std::cout << "end of virtual tissue creation" << std::endl;

    std::string fileNameGeometryOutputRaw = "geometry";
    std::cout << std::endl
              <<"======================================================="
              << std::endl;
    std::cout << "saving virtual tissue : " << std::endl;

    gkg::Writer::getInstance().write( fileNameGeometryOutputRaw,
                                      *virtualTissue );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
