################################################################################
# HEADERS
################################################################################

set( MOC_HEADERS
gkg-anatomist-plugin-rendering-options/ABundleMapRenderingOptionsWidget.h
gkg-anatomist-plugin-rendering-options/AMedusaGeometryObjectRenderingOptionsWidget.h
gkg-anatomist-plugin-rendering-options/AOdfFieldRenderingOptionsWidget.h
gkg-anatomist-plugin-rendering-options/APointCloudFieldObjectRenderingOptionsWidget.h
gkg-anatomist-plugin-rendering-options/APointCloudObjectRenderingOptionsWidget.h
)

#gkg-anatomist-plugin-rendering-options/AFiberRenderingOptionsWidget.h
#gkg-anatomist-plugin-rendering-options/AInteractiveBundleMapRenderingOptionsWidget.h
#gkg-anatomist-plugin-rendering-options/APdfFieldRenderingOptionsWidget.h
#gkg-anatomist-plugin-rendering-options/ATwoPoolModelFieldRenderingOptionsWidget.h
#gkg-anatomist-plugin-rendering-options/OrientationCountSpinBox.h

set( HEADERS
gkg-anatomist-plugin-control/MouseAction.h
gkg-anatomist-plugin-control/MouseControl.h
gkg-anatomist-plugin-control/MouseEvent.h
gkg-anatomist-plugin-module/GkgAnatomistPlugin.h
gkg-anatomist-plugin-objects/ABundleMapObject.h
gkg-anatomist-plugin-objects/AMedusaGeometryObject.h
gkg-anatomist-plugin-objects/AnatomistVolume.h
gkg-anatomist-plugin-objects/AOdfFieldObject.h
gkg-anatomist-plugin-objects/APointCloudFieldObject.h
gkg-anatomist-plugin-objects/APointCloudObject.h
gkg-anatomist-plugin-objects/AQThreadTestObject.h
gkg-anatomist-plugin-server/AnatomistServer.h
${MOC_HEADERS}
)


#gkg-anatomist-plugin-io/AFiberReader.h
#gkg-anatomist-plugin-io/AOdfFieldProcessingContext.h
#gkg-anatomist-plugin-io/APdfFieldProcessingContext.h
#gkg-anatomist-plugin-io/AOdfFieldReader.h
#gkg-anatomist-plugin-io/APdfFieldReader.h
#gkg-anatomist-plugin-io/ATwoPoolModelFieldProcessingContext.h
#gkg-anatomist-plugin-io/ATwoPoolModelFieldReader.h
#gkg-anatomist-plugin-io/NormalsProcessingContext.h
#gkg-anatomist-plugin-io/TransformationsAndTranslationsAndNormalsProcessingContext.h
#gkg-anatomist-plugin-io/TransformationsAndTranslationsProcessingContext.h
#gkg-anatomist-plugin-objects/AFiberObject.h
#gkg-anatomist-plugin-objects/AFiberShader.h
#gkg-anatomist-plugin-objects/AInteractiveBundleMapObject.h
#gkg-anatomist-plugin-objects/APdfFieldObject.h
#gkg-anatomist-plugin-objects/ARealTimeFiberObject.h
#gkg-anatomist-plugin-objects/ARealTimeOdfFieldObject.h
#gkg-anatomist-plugin-objects/ATwoPoolModelFieldObject.h
#gkg-anatomist-plugin-objects/VertexShader.h


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-anatomist-plugin-control/MouseAction.cxx
gkg-anatomist-plugin-control/MouseControl.cxx
gkg-anatomist-plugin-control/MouseEvent.cxx
gkg-anatomist-plugin-module/GkgAnatomistPlugin.cxx
gkg-anatomist-plugin-objects/ABundleMapObject.cxx
gkg-anatomist-plugin-objects/AMedusaGeometryObject.cxx
gkg-anatomist-plugin-objects/AnatomistVolume.cxx
gkg-anatomist-plugin-objects/AOdfFieldObject.cxx
gkg-anatomist-plugin-objects/APointCloudFieldObject.cxx
gkg-anatomist-plugin-objects/APointCloudObject.cxx
gkg-anatomist-plugin-rendering-options/ABundleMapRenderingOptionsWidget.cxx
gkg-anatomist-plugin-rendering-options/AMedusaGeometryObjectRenderingOptionsWidget.cxx
gkg-anatomist-plugin-rendering-options/AOdfFieldRenderingOptionsWidget.cxx
gkg-anatomist-plugin-rendering-options/APointCloudFieldObjectRenderingOptionsWidget.cxx
gkg-anatomist-plugin-rendering-options/APointCloudObjectRenderingOptionsWidget.cxx
gkg-anatomist-plugin-objects/AQThreadTestObject.cxx
gkg-anatomist-plugin-server/AnatomistServer.cxx
)


#gkg-anatomist-plugin-io/AFiberReader.cxx
#gkg-anatomist-plugin-io/AOdfFieldProcessingContext.cxx
#gkg-anatomist-plugin-io/APdfFieldProcessingContext.cxx
#gkg-anatomist-plugin-io/AOdfFieldReader.cxx
#gkg-anatomist-plugin-io/APdfFieldReader.cxx
#gkg-anatomist-plugin-io/ATwoPoolModelFieldProcessingContext.cxx
#gkg-anatomist-plugin-io/ATwoPoolModelFieldReader.cxx
#gkg-anatomist-plugin-io/NormalsProcessingContext.cxx
#gkg-anatomist-plugin-io/TransformationsAndTranslationsAndNormalsProcessingContext.cxx
#gkg-anatomist-plugin-io/TransformationsAndTranslationsProcessingContext.cxx
#gkg-anatomist-plugin-objects/AFiberObject.cxx
#gkg-anatomist-plugin-objects/AInteractiveBundleMapObject.cxx
#gkg-anatomist-plugin-objects/AFiberShader.cxx
#gkg-anatomist-plugin-objects/APdfFieldObject.cxx
#gkg-anatomist-plugin-objects/ARealTimeFiberObject.cxx
#gkg-anatomist-plugin-objects/ARealTimeOdfFieldObject.cxx
#gkg-anatomist-plugin-objects/ATwoPoolModelFieldObject.cxx
#gkg-anatomist-plugin-objects/VertexShader.cxx
#gkg-anatomist-plugin-rendering-options/AFiberRenderingOptionsWidget.cxx
#gkg-anatomist-plugin-rendering-options/AInteractiveBundleMapRenderingOptionsWidget.cxx
#gkg-anatomist-plugin-rendering-options/APdfFieldRenderingOptionsWidget.cxx
#gkg-anatomist-plugin-rendering-options/ATwoPoolModelFieldRenderingOptionsWidget.cxx
#gkg-anatomist-plugin-rendering-options/OrientationCountSpinBox.cxx


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


if ( MPI_FOUND )

include_directories( ${MPI_INCLUDE_PATH} )

endif ( MPI_FOUND )


add_library( gkg-anatomist-plugin SHARED ${SOURCES} )
set_property( TARGET gkg-anatomist-plugin PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )

if ( MPI_FOUND )

target_link_libraries( gkg-anatomist-plugin 
                       gkg-simulation
                       ${BRAINVISA_LIBRARIES} ${OPENGL_LIBRARIES}
                       ${QT_LIBRARIES} ${QT_QTUITOOLS_LIBRARY}
                       ${QWT_LIBRARY}
                       ${MPI_LIBRARIES} )

else ( MPI_FOUND )

target_link_libraries( gkg-anatomist-plugin 
                       gkg-simulation
                       ${BRAINVISA_LIBRARIES} ${OPENGL_LIBRARIES}
                       ${QT_LIBRARIES} ${QT_QTUITOOLS_LIBRARY}
                       ${QWT_LIBRARY} )

endif ( MPI_FOUND )


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
install( TARGETS gkg-anatomist-plugin DESTINATION lib COMPONENT runtime )
