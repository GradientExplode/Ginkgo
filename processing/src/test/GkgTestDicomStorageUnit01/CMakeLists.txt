################################################################################
# TARGET NAME
################################################################################

set( TARGET_NAME GkgTestDicomStorageUnit01 )


################################################################################
# SOURCES
################################################################################

set( SOURCES
main.cxx
)


################################################################################
# Compilation and link
################################################################################

add_executable( ${TARGET_NAME} ${SOURCES} )
target_link_libraries( ${TARGET_NAME} gkg-processing-plugin-functors )


################################################################################
# Installation
################################################################################

install( TARGETS ${TARGET_NAME} DESTINATION bin COMPONENT runtime )
