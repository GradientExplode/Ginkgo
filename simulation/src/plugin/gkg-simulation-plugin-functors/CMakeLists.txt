################################################################################
# HEADERS
################################################################################

set( HEADERS
Medusa/MedusaCommand.h
MedusaGeometryDeoverlaper/MedusaGeometryDeoverlaperCommand.h
DwiGradientShapeGenerator/DwiGradientShapeGeneratorCommand.h
MedusaDataToDeepLearningData/MedusaDataToDeepLearningDataCommand.h
RotateAndAddNoiseToMedusaMRISignature/RotateAndAddNoiseToMedusaMRISignatureCommand.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
Medusa/MedusaCommand.cxx
MedusaGeometryDeoverlaper/MedusaGeometryDeoverlaperCommand.cxx
DwiGradientShapeGenerator/DwiGradientShapeGeneratorCommand.cxx
MedusaDataToDeepLearningData/MedusaDataToDeepLearningDataCommand.cxx
RotateAndAddNoiseToMedusaMRISignature/RotateAndAddNoiseToMedusaMRISignatureCommand.cxx
)


################################################################################
# Compilation and link
################################################################################

include_directories( ${Kokkos_INCLUDE_DIR} ${GKG_INCLUDE_DIR}  )

add_library( gkg-simulation-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-simulation-plugin-functors PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )

target_link_libraries( gkg-simulation-plugin-functors gkg-simulation )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-simulation-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-simulation-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
