################################################################################
# TARGET NAME
################################################################################

set( TARGET_NAME GkgDwiPdfFieldDiffeomorphicRegistration )


################################################################################
# SOURCES
################################################################################

set( SOURCES
main.cxx
ProbabilityDensityFunction.cxx
PolarHarmonicsCache.cxx
SetStandardBasisTypeContext.cxx
PdfFieldInterpolation3DContext.cxx
DissimilarityEnergyComputationContext.cxx
ReferenceHasSiteBoolVectorContext.cxx
ReorientationContext.cxx
Quaternion.cxx
)

################################################################################
# HEADERS
################################################################################

set( HEADERS
DiffeomorphicTransformations.h
ProbabilityDensityFunction.h
PolarHarmonicsCache.h
SetStandardBasisTypeContext.h
PdfFieldInterpolation3DContext.h
DissimilarityEnergyComputationContext.h
ReferenceHasSiteBoolVectorContext.h
ReorientationContext.h
SymmetricDiffeomorphism.h
MinimumSpanningTree.h
DataCost.h
Quaternion.h
)


################################################################################
# Compilation and link
################################################################################

#include_directories( "${CMAKE_BINARY_DIR}/include" "${GKG_INCLUDES}" )
#include_directories( "${CMAKE_CURRENT_BINARY_DIR}" "${GKG_INCLUDES}" )

#set( GKG_USER_LIBRARY "${CMAKE_BINARY_DIR}/src/library/libgkg-user.so" )

add_executable( ${TARGET_NAME} ${SOURCES} )

target_link_libraries( ${TARGET_NAME} gkg-dmri )


################################################################################
# Installation
################################################################################

install( TARGETS ${TARGET_NAME} DESTINATION bin COMPONENT runtime )
