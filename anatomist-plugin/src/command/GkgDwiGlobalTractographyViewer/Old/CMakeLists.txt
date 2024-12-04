################################################################################
# TARGET NAME
################################################################################

set( TARGET_NAME GkgDwiGlobalTractographyViewer )


################################################################################
# SOURCES
################################################################################


set( HEADERS
AGlobalTractographyObject.h
AnatomicalPriorsCallback.h
AnatomistViewer.h
DefaultInformationCallback.h
GlobalTractographyCallbackIds.h
GlobalTractographyEvent.h
GlobalTractographyOptimizationInformation.h
GlobalTractographyParameterIds.h
GlobalTractographyService.h
GlobalTractographyServiceThread.h
GlobalTractographyViewer.h
InitialSpinGlassConfigurationCallback.h
NothingToDoInformationCallback.h
SpinGlassConnectionCreationCallback.h
SpinGlassConnectionDeletionCallback.h
SpinGlassConnectionInformationCallback.h
SpinGlassCreationCallback.h
SpinGlassCreationInformationCallback.h
SpinGlassDeletionCallback.h
SpinGlassDeletionInformationCallback.h
SpinGlassMoveCallback.h
SpinGlassOptimalMoveInformationCallback.h
SpinGlassRandomMoveInformationCallback.h
)

set( SOURCES
AGlobalTractographyObject.cxx
AnatomicalPriorsCallback.cxx
AnatomistViewer.cxx
DefaultInformationCallback.cxx
GlobalTractographyEvent.cxx
GlobalTractographyOptimizationInformation.cxx
GlobalTractographyService.cxx
GlobalTractographyServiceThread.cxx
GlobalTractographyViewer.cxx
InitialSpinGlassConfigurationCallback.cxx
NothingToDoInformationCallback.cxx
SpinGlassConnectionCreationCallback.cxx
SpinGlassConnectionDeletionCallback.cxx
SpinGlassConnectionInformationCallback.cxx
SpinGlassCreationCallback.cxx
SpinGlassCreationInformationCallback.cxx
SpinGlassDeletionCallback.cxx
SpinGlassDeletionInformationCallback.cxx
SpinGlassMoveCallback.cxx
SpinGlassOptimalMoveInformationCallback.cxx
SpinGlassRandomMoveInformationCallback.cxx
main.cxx
)


################################################################################
# Compilation and link
################################################################################

BRAINVISA_ADD_MOC_FILES( SOURCES ${HEADERS} )

include( "${ANATOMIST-FREE_USE_FILE}" )
include( "${QT_USE_FILE}" )
include_directories( ${QT_INCLUDES} )
include( "${QWT_USE_FILE}" )
include_directories( ${QWT_INCLUDES} )

INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_BINARY_DIR}" )

add_executable( ${TARGET_NAME} ${SOURCES} )
target_link_libraries( ${TARGET_NAME} -lGL -lGLU gkg-dmri gkg-anatomist-plugin ${ANATOMIST-FREE_LIBRARIES}  ${QT_LIBRARIES} ${QT_QTUITOOLS_LIBRARY} ${QWT_LIBRARY} )


################################################################################
# Installation
################################################################################

BRAINVISA_INSTALL( TARGETS ${TARGET_NAME}
                   DESTINATION bin
                   COMPONENT ${PROJECT_NAME} )
