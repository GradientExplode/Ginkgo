################################################################################
# TARGET NAME
################################################################################

set( TARGET_NAME GkgTestDetachedThread01 )


################################################################################
# SOURCES
################################################################################

set( SOURCES
DetachedThread.cxx
main.cxx
)


################################################################################
# Compilation and link
################################################################################

add_executable( ${TARGET_NAME} ${SOURCES} )
target_link_libraries( ${TARGET_NAME} gkg-communication )


################################################################################
# Installation
################################################################################

install( TARGETS ${TARGET_NAME} DESTINATION bin )
