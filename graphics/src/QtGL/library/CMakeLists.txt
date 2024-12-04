################################################################################
# HEADERS
################################################################################

set( MOC_HEADERS
gkg-graphics-QtGL-widget/ExtendedSlider.h
gkg-graphics-QtGL-widget/LabelledComboBox.h
gkg-graphics-QtGL-widget/NetworkSetting.h
)

set( HEADERS
gkg-graphics-QtGL-qtext/Qt.h
gkg-graphics-QtGL-qtext/Qwt.h
gkg-graphics-QtGL-trackball/Trackball.h
gkg-graphics-QtGL-slicing/CubeZBuffer.h
gkg-graphics-QtGL-slicing/Slicing.h
gkg-graphics-QtGL-wl/WL.h
${MOC_HEADERS}
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-graphics-QtGL-trackball/Trackball.cxx
gkg-graphics-QtGL-slicing/CubeZBuffer.cxx
gkg-graphics-QtGL-slicing/Slicing.cxx
gkg-graphics-QtGL-widget/ExtendedSlider.cxx
gkg-graphics-QtGL-widget/LabelledComboBox.cxx
gkg-graphics-QtGL-widget/NetworkSetting.cxx
gkg-graphics-QtGL-wl/WL.cxx
)


################################################################################
# Compilation and link
################################################################################

if( DESIRED_QT_VERSION EQUAL 5 )
  qt5_wrap_cpp( MOC_SOURCES ${MOC_HEADERS} )
  qt5_wrap_ui( SAMPLE_UI_HDRS ${SOURCES_UI} )
  qt5_add_resources( SAMPLE_RC_SRCS ${SOURCES_RC} )
  add_definitions( ${Qt5Core_DEFINITIONS} 
                   ${Qt5Widgets_DEFINITIONS}
                   ${Qt5OpenGL_DEFINITIONS} )
  include_directories(  ${Qt5Core_INCLUDE_DIRS} 
                        ${Qt5Widgets_INCLUDE_DIRS}
                        ${Qt5OpenGL_INCLUDE_DIRS} )
  list( APPEND QT_LIBRARIES 
        ${Qt5Core_LIBRARIES} 
        ${Qt5Widgets_LIBRARIES}
        ${Qt5OpenGL_LIBRARIES} )
else( DESIRED_QT_VERSION EQUAL 5 )
  qt4_wrap_cpp( MOC_SOURCES ${MOC_HEADERS} )
  add_definitions( "-DQT3_SUPPORT" )
  include( "${QT_USE_FILE}" )
endif( DESIRED_QT_VERSION EQUAL 5 )

list( APPEND SOURCES ${MOC_SOURCES} )

include_directories( ${QT_INCLUDES} ${QWT_INCLUDE_DIR} )

add_library( gkg-graphics-QtGL SHARED ${SOURCES} )
set_property( TARGET gkg-graphics-QtGL PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-graphics-QtGL gkg-processing
                       ${QWT_LIBRARY} ${QT_QTOPENGL_LIBRARY}
                       ${QT_QT3SUPPORT_LIBRARY} ${QT_FREETYPE_LIBRARY}
                       ${OPENGL_LIBRARIES} ${ZLIB_LIBRARY} ${QT_LIBRARIES} )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-graphics-QtGL DESTINATION lib COMPONENT runtime )
