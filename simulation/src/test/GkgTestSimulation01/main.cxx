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

    std::cout << "essai 1" << std::endl;
    gkg::UncommentCounterInputFileStream
      geometryUcis(
         ( gkg::ConfigurationInfo::getInstance().getHomePath() +
           gkg::getDirectorySeparator() +
           "Git/gkg/simulation/src/test/GkgTestSimulation01/" +
           "geometry.json" ).c_str() );
    gkg::BaseObjectReader geometryBaseObjectReader;
    gkg::TypedObject< gkg::Dictionary > geometryDictionary;
    geometryBaseObjectReader.read( geometryUcis, geometryDictionary );
    geometryUcis.close();


//    const gkg::Dictionary& dictionary = geometryDictionary.getTypedValue
//     gkg::Dictionary::const_iterator i = dictionary.find( "populations" );
//     if ( i == dictionary.end() )
//     {
// 
//       throw std::runtime_error( "a 'populations' item is required" );
// 
//     }
// 
// 
//     gkg::Dictionary 
//       populationsDictionary = i->second->getValue< gkg::Dictionary >();
// 
//     gkg::Dictionary::const_iterator
//       pd = populationsDictionary.begin();
// 
//     gkg::Dictionary populationDictionary = 
//                             pd->second->getValue< gkg::Dictionary >();
// 
//     gkg::RCPointer< gkg::Population > population(
//                                 gkg::PopulationFactory::getInstance().create(
//                                   0,
//                                   0,
//                                   populationDictionary ) );
// 
// 
//     std::cout << "population->getCellCount()="
//               << population->getCellCount() << std::endl;
//     std::cout << "population->getTypeName()="
//               << population->getTypeName() << std::endl;

    std::cout << "creating now virtual tissue" << std::endl;

    gkg::RCPointer< gkg::Dictionary > populationTissuePropertyDictionary;
    gkg::RCPointer< gkg::VirtualTissue >
      virtualTissue(
                 new gkg::VirtualTissue( geometryDictionary.getTypedValue(),
                                         populationTissuePropertyDictionary ) );

    std::cout << "end of virtual tissue creation" << std::endl;

    std::string fileNameGeometryOutputMesh = "geometry";
    std::vector< gkg::MeshMap< int32_t, float, 3U > >
      geometryOutputMeshMaps( 2U );
    virtualTissue->getMeshMaps( geometryOutputMeshMaps );

    int32_t meshMapCount = ( int32_t )geometryOutputMeshMaps.size();
    int32_t meshMapIndex = 0;
    for ( meshMapIndex = 0; meshMapIndex < meshMapCount; meshMapIndex++ )
    {

      gkg::Writer::getInstance().write( 
        fileNameGeometryOutputMesh + "_" +
        gkg::StringConverter::toString( meshMapIndex + 1 ),
        geometryOutputMeshMaps[ meshMapIndex ],
        false,
        "aimsmesh" );

    }


  }
  GKG_CATCH_COMMAND( result );

  return result;

}
