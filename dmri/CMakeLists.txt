###############################################################################
# SETTING INCLUDE DIRS
###############################################################################

set( GKG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin )

include_directories( ${COMPONENT_INCLUDE_DIRS} ${GKG_INCLUDE_DIRS} )


###############################################################################
# ADDING C++ SUB-DIRECTORIES TO BE PROCESSED
###############################################################################

add_subdirectory( scripts )
add_subdirectory( src )


###############################################################################
# INSTALLATION
###############################################################################

#install( DIRECTORY share/ DESTINATION share/${PROJECT_NAME} COMPONENT runtime )
