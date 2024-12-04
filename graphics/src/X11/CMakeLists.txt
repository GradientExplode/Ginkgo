###############################################################################
# SETTING INCLUDE DIRS
###############################################################################

include_directories( ${COMPONENT_INCLUDE_DIRS} 
                     ${CMAKE_CURRENT_SOURCE_DIR}/library
                     ${CMAKE_CURRENT_SOURCE_DIR}/plugin )


###############################################################################
# ADDING C++ SUB-DIRECTORIES TO BE PROCESSED
###############################################################################

add_subdirectory( library )
add_subdirectory( plugin )
add_subdirectory( test )
