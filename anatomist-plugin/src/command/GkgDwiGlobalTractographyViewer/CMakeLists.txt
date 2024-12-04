################################################################################
# TARGET NAME
################################################################################

set( TARGET_NAME GkgDwiGlobalTractographyViewer )


################################################################################
# SOURCES
################################################################################

set( MOC_HEADERS
GlobalTractographyViewer.h
)

set( HEADERS
AGlobalTractographyObject.h
AnatomistViewer.h
BoundingBoxCallback.h
GlobalTractographyCallbackIds.h
GlobalTractographyEvent.h
GlobalTractographyService.h
GlobalTractographyServiceThread.h
InitialSpinGlassConfigurationCallback.h
ResetCallback.h
SpinGlassConnectionCreationCallback.h
SpinGlassConnectionDeletionCallback.h
SpinGlassCreationCallback.h
SpinGlassDeletionCallback.h
SpinGlassInformationCallback.h
SpinGlassMoveCallback.h
${MOC_HEADERS}
)

set( SOURCES
AGlobalTractographyObject.cxx
AnatomistViewer.cxx
BoundingBoxCallback.cxx
GlobalTractographyEvent.cxx
GlobalTractographyService.cxx
GlobalTractographyServiceThread.cxx
GlobalTractographyViewer.cxx
InitialSpinGlassConfigurationCallback.cxx
main.cxx
ResetCallback.cxx
SpinGlassConnectionCreationCallback.cxx
SpinGlassConnectionDeletionCallback.cxx
SpinGlassCreationCallback.cxx
SpinGlassDeletionCallback.cxx
SpinGlassInformationCallback.cxx
SpinGlassMoveCallback.cxx
)


################################################################################
# Compilation and link
################################################################################

if( DESIRED_QT_VERSION EQUAL 5 )
  qt5_wrap_cpp( MOC_SOURCES ${MOC_HEADERS} )
  add_definitions( ${Qt5Core_DEFINITIONS} 
                   ${Qt5Widgets_DEFINITIONS}
                   ${Qt5OpenGL_DEFINITIONS} 
                   ${Qt5UiTools_DEFINITIONS} )
  include_directories(  ${Qt5Core_INCLUDE_DIRS} 
                        ${Qt5Widgets_INCLUDE_DIRS}
                        ${Qt5OpenGL_INCLUDE_DIRS}
                        ${Qt5UiTools_INCLUDE_DIRS} )
  list( APPEND QT_LIBRARIES 
        ${Qt5Core_LIBRARIES} 
        ${Qt5Widgets_LIBRARIES}
        ${Qt5OpenGL_LIBRARIES}
        ${Qt5UiTools_LIBRARIES} )
else( DESIRED_QT_VERSION EQUAL 5 )
  qt4_wrap_cpp( MOC_SOURCES ${MOC_HEADERS} )
endif( DESIRED_QT_VERSION EQUAL 5 )

list( APPEND SOURCES ${MOC_SOURCES} )

include_directories( "${CMAKE_CURRENT_BINARY_DIR}" )

add_executable( ${TARGET_NAME} ${SOURCES} )
target_link_libraries( ${TARGET_NAME} 
                       gkg-dmri gkg-anatomist-plugin
                       ${BRAINVISA_LIBRARIES} ${OPENGL_LIBRARIES}
                       ${QT_LIBRARIES} ${QT_QTUITOOLS_LIBRARY}
                       ${QWT_LIBRARY} )


################################################################################
# Installation
################################################################################

install( TARGETS ${TARGET_NAME} DESTINATION bin COMPONENT runtime )
