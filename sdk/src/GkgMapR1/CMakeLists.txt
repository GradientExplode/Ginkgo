################################################################################
# TARGET NAME
################################################################################

set( TARGET_NAME GkgMapR1 )


################################################################################
# SOURCES
################################################################################

set( SOURCES
main.cxx
)

################################################################################
# HEADERS
################################################################################

set( HEADERS
DiffeomorphicTransformations.h
MinimumSpanningTree.h
DataCostD.h
Regularization.h
SymmetricDiffeomorphism.h
)


################################################################################
# Compilation and link
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
