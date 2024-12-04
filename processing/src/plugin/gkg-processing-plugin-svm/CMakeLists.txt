################################################################################
# HEADERS
################################################################################

set( HEADERS
LibsvmMachineLearningDataImplementation.h
LibsvmMachineLearningImplementationFactory.h
LibsvmMachineLearningProblemImplementation.h
LibsvmSupportVectorMachineImplementation.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
LibsvmMachineLearningDataImplementation.cxx
LibsvmMachineLearningImplementationFactory.cxx
LibsvmMachineLearningProblemImplementation.cxx
LibsvmSupportVectorMachineImplementation.cxx
)


################################################################################
# Compilation and link
################################################################################

include_directories( ${SVM_INCLUDE_DIR} )

add_library( gkg-processing-plugin-svm SHARED ${SOURCES} )
set_property( TARGET gkg-processing-plugin-svm
              PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-processing-plugin-svm gkg-processing
                       ${SVM_LIBRARIES} )


################################################################################
# Installation
################################################################################

install( FILES ${HEADERS}
         DESTINATION include/${PROJECT_NAME}/gkg-processing-plugin-svm
         COMPONENT devel )
install( TARGETS gkg-processing-plugin-svm DESTINATION lib COMPONENT runtime )
