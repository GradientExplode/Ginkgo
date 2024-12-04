################################################################################
# HEADERS
################################################################################

set( MOC_HEADERS
Abacus/AbacusMainWindow.h
DicomDump/DicomDumpMainWindow.h
Voi/BrushSizeW.h
Voi/CheckedHeader.h
Voi/ComboBox.h
Voi/CoordinateSpinBoxes.h
Voi/CylinderW.h
Voi/Dialog.h                             
Voi/DrawA.h    
Voi/EvolInfo.h                           
Voi/EvolInfoW.h                          
Voi/FusionSliders.h
Voi/GraphicsScene.h
Voi/LabelInfo.h                          
Voi/LabelInfoW.h                         
Voi/MorphologyW.h
Voi/ScrollBars.h
Voi/ShowWidget.h
Voi/Slider.h
Voi/SpinBox.h
Voi/ToolBar.h
Voi/VoiLoadWindow.h
Voi/VoiMainWindow.h                         
)

set( HEADERS
Abacus/AbacusCommand.h
DicomDump/DicomDumpCommand.h
Voi/AxialStrategy.h
Voi/Block.h                      
Voi/Chunk.h                       
Voi/ConnectivityFactory.h                
Voi/ContrastStrategy.h
Voi/CoronalStrategy.h
Voi/CreateState.h
Voi/Cursor.h                             
Voi/CylinderStrategy.h
Voi/Data.h
Voi/DistanceStrategy.h    
Voi/EventState.h
Voi/EvolStrategy.h                       
Voi/FillStrategy.h                       
Voi/HistogramStrategy.h
Voi/Indicator.h                          
Voi/LabelAxialStrategy.h                 
Voi/LabelCoronalStrategy.h               
Voi/LabelFactory.h                       
Voi/LabelSagittalStrategy.h              
Voi/LabelStrategy.h                      
Voi/LutInfo.h                            
Voi/ModeFactory.h
Voi/ModeStrategy.h
Voi/MorphologyStrategy.h
Voi/Node.h
Voi/Observable.h
Voi/Observer.h
Voi/PaintStrategy.h
Voi/PickColorStrategy.h
Voi/ProfileStrategy.h
Voi/ReadProcess.h
Voi/RoiCache.h
Voi/SagittalStrategy.h
Voi/SelectionStrategy.h
Voi/SharedGLWidget.h
Voi/Slicer.h
Voi/SplineCache.h
Voi/SplineItem.h
Voi/SplineStrategy.h
Voi/StartDistanceState.h
Voi/StatisticStrategy.h
Voi/StatisticStrategy.h
Voi/StopDistanceState.h
Voi/Strategy2D.h
Voi/Strategy3D.h
Voi/Strategy.h
Voi/StrategyFactory.h
Voi/ValidateState.h
Voi/VoiCommand.h
Voi/VoiStatistic.h
Voi/VolumeBaseFusion.h
Voi/VoxelEngine.h
Voi/Workspace.h
${MOC_HEADERS}
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
Abacus/AbacusCommand.cxx
Abacus/AbacusMainWindow.cxx
DicomDump/DicomDumpCommand.cxx
DicomDump/DicomDumpMainWindow.cxx
Voi/AxialStrategy.cxx
Voi/Block.cxx                      
Voi/BrushSizeW.cxx
Voi/CheckedHeader.cxx
Voi/Chunk.cxx                       
Voi/ComboBox.cxx     
Voi/ConnectivityFactory.cxx
Voi/ContrastStrategy.cxx
Voi/CoordinateSpinBoxes.cxx
Voi/CoronalStrategy.cxx    
Voi/CreateState.cxx
Voi/Cursor.cxx             
Voi/CylinderStrategy.cxx
Voi/CylinderW.cxx
Voi/Data.cxx               
Voi/Dialog.cxx             
Voi/DistanceStrategy.cxx
Voi/DrawA.cxx
Voi/EventState.cxx
Voi/EvolInfo.cxx           
Voi/EvolInfoW.cxx          
Voi/EvolStrategy.cxx       
Voi/FillStrategy.cxx       
Voi/FusionSliders.cxx
Voi/GraphicsScene.cxx
Voi/HistogramStrategy.cxx
Voi/Indicator.cxx          
Voi/LabelAxialStrategy.cxx 
Voi/LabelCoronalStrategy.cxx
Voi/LabelFactory.cxx        
Voi/LabelInfo.cxx           
Voi/LabelInfoW.cxx          
Voi/LabelSagittalStrategy.cxx
Voi/LabelStrategy.cxx        
Voi/LutInfo.cxx              
Voi/ModeFactory.cxx
Voi/ModeStrategy.cxx
Voi/MorphologyStrategy.cxx
Voi/MorphologyW.cxx
Voi/Node.cxx
Voi/Observable.cxx
Voi/Observer.cxx
Voi/PaintStrategy.cxx
Voi/PickColorStrategy.cxx
Voi/ProfileStrategy.cxx
Voi/ReadProcess.cxx
Voi/RoiCache.cxx
Voi/SagittalStrategy.cxx
Voi/ScrollBars.cxx
Voi/SelectionStrategy.cxx
Voi/SharedGLWidget.cxx
Voi/ShowWidget.cxx
Voi/Slicer.cxx
Voi/Slider.cxx
Voi/SpinBox.cxx
Voi/SplineCache.cxx
Voi/SplineItem.cxx
Voi/SplineStrategy.cxx
Voi/StartDistanceState.cxx
Voi/StatisticStrategy.cxx
Voi/StopDistanceState.cxx
Voi/Strategy2D.cxx
Voi/Strategy3D.cxx
Voi/Strategy.cxx
Voi/StrategyFactory.cxx
Voi/ToolBar.cxx
Voi/ValidateState.cxx
Voi/VoiCommand.cxx
Voi/VoiLoadWindow.cxx
Voi/VoiMainWindow.cxx           
Voi/VoiStatistic.cxx
Voi/VolumeBaseFusion.cxx
Voi/VoxelEngine.cxx
Voi/Workspace.cxx
)


################################################################################
# RESOURCES
################################################################################

set( SOURCES_RC
  Voi/Voi.qrc
)

################################################################################
# Compilation and link
################################################################################

add_definitions( -DGL_GLEXT_PROTOTYPES )

if( DESIRED_QT_VERSION EQUAL 5 )
  qt5_wrap_cpp( MOC_SOURCES ${MOC_HEADERS} )
  qt5_add_resources( SAMPLE_RC_SRCS ${SOURCES_RC} )
  add_definitions( ${Qt5Core_DEFINITIONS} 
                   ${Qt5Widgets_DEFINITIONS}
                   ${Qt5OpenGL_DEFINITIONS} )
  include_directories(  ${Qt5Core_INCLUDE_DIRS} 
                        ${Qt5Widgets_INCLUDE_DIRS}
                        ${Qt5OpenGL_INCLUDE_DIRS} )
else( DESIRED_QT_VERSION EQUAL 5 )
  qt4_wrap_cpp( MOC_SOURCES ${MOC_HEADERS} )
  qt4_add_resources( SAMPLE_RC_SRCS ${SOURCES_RC} )
  include( "${QT_USE_FILE}" )
endif( DESIRED_QT_VERSION EQUAL 5 )

list( APPEND SOURCES ${MOC_SOURCES} ${SAMPLE_RC_SRCS} )

include_directories( ${QT_INCLUDES} ${QWT_INCLUDE_DIR} )

add_library( gkg-graphics-QtGL-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-graphics-QtGL-plugin-functors 
              PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )

target_link_libraries( gkg-graphics-QtGL-plugin-functors gkg-graphics-QtGL )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-graphics-QtGL-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-graphics-QtGL-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
