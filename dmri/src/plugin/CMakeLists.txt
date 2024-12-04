add_subdirectory( gkg-dmri-plugin-functors )

################################## VTK plugin ##################################
if ( BUILD_GINKGO_VTK_PLUGIN )
  add_subdirectory( gkg-dmri-plugin-vtk )
else ( BUILD_GINKGO_VTK_PLUGIN )
  message( "Warning: gkg-dmri-plugin-vtk not built due to missing VTK dependency" )
endif ( BUILD_GINKGO_VTK_PLUGIN )

