################################################################################
# HEADERS
################################################################################

set( HEADERS
BitmapEditor/BitmapEditorCommand.h
Viewer2d/Viewer2dCommand.h
Viewer2d/Viewer2dProcess.h
Viewer2dRpcServer/RpcCommandId.h
Viewer2dRpcServer/RpcExitCallback.h
Viewer2dRpcServer/RpcResetCallback.h
Viewer2dRpcServer/RpcUpdateCallback.h
Viewer2dRpcServer/Viewer2d.h
Viewer2dRpcServer/Viewer2dRpcServerCommand.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
BitmapEditor/BitmapEditorCommand.cxx
Viewer2d/Viewer2dCommand.cxx
Viewer2d/Viewer2dProcess.cxx
Viewer2dRpcServer/RpcExitCallback.cxx
Viewer2dRpcServer/RpcResetCallback.cxx
Viewer2dRpcServer/RpcUpdateCallback.cxx
Viewer2dRpcServer/Viewer2d.cxx
Viewer2dRpcServer/Viewer2dRpcServerCommand.cxx
)


################################################################################
# Compilation and link
################################################################################

include_directories( ${X11_INCLUDE_DIR} )

add_library( gkg-graphics-X11-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-graphics-X11-plugin-functors PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-graphics-X11-plugin-functors gkg-graphics-X11 )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-graphics-X11-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-graphics-X11-plugin-functors 
         DESTINATION lib
         COMPONENT runtime )
