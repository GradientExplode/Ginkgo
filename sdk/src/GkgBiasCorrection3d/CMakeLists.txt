################################################################################
# TARGET NAME
################################################################################

set( TARGET_NAME GkgBiasCorrection3d )


################################################################################
# SOURCES
################################################################################

set( SOURCES
main.cxx
BiasCorrection3dProcess.cxx
)


################################################################################
# Compilation and link
# SDK_LIBRARY_PATH: added so that to allow the integration of a further library
################################################################################


set( SDK_LIBRARY_PATH
     ${CMAKE_SOURCE_DIR}/src/library )

include_directories( ${SDK_LIBRARY_PATH} ${GKG_INCLUDE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( ${TARGET_NAME} ${SOURCES} )

target_link_libraries( ${TARGET_NAME} ${GKG_LIBRARIES} )


################################################################################
# Installation
################################################################################

install( TARGETS ${TARGET_NAME} DESTINATION bin )
