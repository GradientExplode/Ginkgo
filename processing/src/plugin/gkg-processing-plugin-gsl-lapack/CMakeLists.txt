################################################################################
# HEADERS
################################################################################

set( HEADERS
GSLCMatrixImplementation.h
GSLConvexQuadraticProgrammingSolver.h
GSLCVectorImplementation.h
GSLFRCGFunctionImplementation.h
GSLFunctionInterpolatorImplementation.h
GSLLMFunctionImplementation.h
GSLMatrixImplementation.h
GSLNLPFunctionImplementation.h
GSLNMSFunctionImplementation.h
GSLNumAnalysisImplementationFactory.h
GSLRandomGeneratorImplementation.h
GSLSAConfigurationImplementation.h
GSLVectorImplementation.h
Lapack.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
GSLCMatrixImplementation.cxx
GSLConvexQuadraticProgrammingSolver.cxx
GSLCVectorImplementation.cxx
GSLFRCGFunctionImplementation.cxx
GSLFunctionInterpolatorImplementation.cxx
GSLLMFunctionImplementation.cxx
GSLMatrixImplementation.cxx
GSLNLPFunctionImplementation.cxx
GSLNMSFunctionImplementation.cxx
GSLNumAnalysisImplementationFactory.cxx
GSLRandomGeneratorImplementation.cxx
GSLSAConfigurationImplementation.cxx
GSLVectorImplementation.cxx
)


################################################################################
# Compilation and link
################################################################################

include_directories( ${GSL_INCLUDE_DIR}
                     ${BLAS_INCLUDE_DIR}
                     ${LAPACK_INCLUDE_DIR}
                     ${NLOPT_INCLUDE_DIRS} )

add_library( gkg-processing-plugin-gsl-lapack SHARED ${SOURCES} )
set_property( TARGET gkg-processing-plugin-gsl-lapack
              PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-processing-plugin-gsl-lapack gkg-processing
                       ${GSL_LIBRARIES} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES}
                       ${NLOPT_LIBRARIES} )


################################################################################
# Installation
################################################################################

install( FILES ${HEADERS}
         DESTINATION include/${PROJECT_NAME}/gkg-processing-plugin-gsl-lapack
         COMPONENT devel )
install( TARGETS gkg-processing-plugin-gsl-lapack
         DESTINATION lib
         COMPONENT runtime )
