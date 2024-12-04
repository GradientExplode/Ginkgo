###############################################################################
# SETTING INCLUDE DIRS
###############################################################################

set( GKG_INCLUDE_DIRS 
     ${BRAINVISA_INCLUDE_DIRS}
     ${Sigc++2_INCLUDE_DIRS}
     ${QT_INCLUDES}
     ${QWT_INCLUDE_DIR} )
set( QT_USE_QTOPENGL TRUE )

if( DESIRED_QT_VERSION LESS 5 )
  include( "${QT_USE_FILE}" )
endif( DESIRED_QT_VERSION LESS 5 )

include_directories( ${COMPONENT_INCLUDE_DIRS} ${GKG_INCLUDE_DIRS} )


###############################################################################
# ADDING C++ SUB-DIRECTORIES TO BE PROCESSED
###############################################################################

add_subdirectory( src )


################################################################################
# Installation
################################################################################

install( DIRECTORY share/ DESTINATION share/${PROJECT_NAME} COMPONENT runtime )
