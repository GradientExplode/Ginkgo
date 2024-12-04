add_subdirectory( gkg-global-tractography-plugin-functors )

################################ PLIHDF5 plugin ################################
if ( BUILD_GINKGO_PLIHDF5_PLUGIN )
  add_subdirectory( gkg-global-tractography-plugin-plihdf5 )
else ( BUILD_GINKGO_PLIHDF5_PLUGIN )
  message( "Warning: gkg-global-tractography-plugin-plihdf5 not built due to missing PLIHDF5 dependency" )
endif ( BUILD_GINKGO_PLIHDF5_PLUGIN )

