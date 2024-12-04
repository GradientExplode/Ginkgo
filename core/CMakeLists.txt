###############################################################################
# SETTING INCLUDE DIRS
###############################################################################

set( GKG_INCLUDE_DIRS 
     ${Boost_INCLUDE_DIRS} )
include_directories( ${COMPONENT_INCLUDE_DIRS} ${GKG_INCLUDE_DIRS} )


###############################################################################
# ADDING C++ SUB-DIRECTORIES TO BE PROCESSED
###############################################################################

add_subdirectory( src )
