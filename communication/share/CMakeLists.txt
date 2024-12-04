###############################################################################
# SETTINGS
###############################################################################

if( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY )
  set( GINKGO_GLOBAL_TRACTOGRAPHY_PLUGIN_FUNCTOR "\'libgkg-global-tractography-plugin-functors.so\'" )
else( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY )
  set( GINKGO_GLOBAL_TRACTOGRAPHY_PLUGIN_FUNCTOR "#" )
endif( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY )

if( GINKGO_BUILD_DARTEL )
  set( GINKGO_DARTEL_PLUGIN_FUNCTOR "\'libgkg-dartel-plugin-functors.so\'" )
else( GINKGO_BUILD_DARTEL )
  set( GINKGO_DARTEL_PLUGIN_FUNCTOR "#" )
endif( GINKGO_BUILD_DARTEL )

if( BUILD_GINKGO_GSL_LAPACK_PLUGIN )
  set( GINKGO_GSL_LAPACK_PLUGIN "\'libgkg-processing-plugin-gsl-lapack.so\'" )
else( BUILD_GINKGO_GSL_LAPACK_PLUGIN )
  set( GINKGO_GSL_LAPACK_PLUGIN "#" )
endif( BUILD_GINKGO_GSL_LAPACK_PLUGIN )

if( BUILD_GINKGO_DICOM_PLUGIN )
  set( GINKGO_DICOM_PLUGIN "\'libgkg-processing-plugin-dicom.so\'" )
else( BUILD_GINKGO_DICOM_PLUGIN )
  set( GINKGO_DICOM_PLUGIN "#" )
endif( BUILD_GINKGO_DICOM_PLUGIN )

if( BUILD_GINKGO_NIFTI_PLUGIN )
  set( GINKGO_NIFTI_PLUGIN "\'libgkg-processing-plugin-nifti.so\'" )
else( BUILD_GINKGO_NIFTI_PLUGIN )
  set( GINKGO_NIFTI_PLUGIN "#" )
endif( BUILD_GINKGO_NIFTI_PLUGIN )

if( BUILD_GINKGO_SVM_PLUGIN )
  set( GINKGO_SVM_PLUGIN "\'libgkg-processing-plugin-svm.so\'" )
else( BUILD_GINKGO_SVM_PLUGIN )
  set( GINKGO_SVM_PLUGIN "#" )
endif( BUILD_GINKGO_SVM_PLUGIN )

if( BUILD_GINKGO_CGAL_PLUGIN )
  set( GINKGO_CGAL_PLUGIN "\'libgkg-processing-plugin-cgal.so\'" )
else( BUILD_GINKGO_CGAL_PLUGIN )
  set( GINKGO_CGAL_PLUGIN "#" )
endif( BUILD_GINKGO_CGAL_PLUGIN )

if( BUILD_GINKGO_VTK_PLUGIN )
  set( GINKGO_VTK_PLUGIN "\'libgkg-dmri-plugin-vtk.so\'" )
else( BUILD_GINKGO_VTK_PLUGIN )
  set( GINKGO_VTK_PLUGIN "#" )
endif( BUILD_GINKGO_VTK_PLUGIN )

if( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY )

  if( BUILD_GINKGO_PLIHDF5_PLUGIN )
    set( GINKGO_PLIHDF5_PLUGIN "\'libgkg-global-tractography-plugin-plihdf5.so\'" )
  else( BUILD_GINKGO_PLIHDF5_PLUGIN )
    set( GINKGO_PLIHDF5_PLUGIN "#" )
  endif( BUILD_GINKGO_PLIHDF5_PLUGIN )

else( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY )

  set( GINKGO_PLIHDF5_PLUGIN "#" )

endif( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY )

if( BUILD_GINKGO_SIMULATION_PLUGIN )
  set( GINKGO_SIMULATION_PLUGIN_FUNCTOR "\'libgkg-simulation-plugin-functors.so\'" )
else( BUILD_GINKGO_SIMULATION_PLUGIN )
  set( GINKGO_SIMULATION_PLUGIN_FUNCTOR "#" )
endif( BUILD_GINKGO_SIMULATION_PLUGIN )



###############################################################################
# CONFIGURATION
###############################################################################

configure_file( functorrc.in functorrc )
configure_file( pluginrc.in pluginrc )


###############################################################################
# INSTALLATION
###############################################################################

install( FILES ${RESOURCES} 
               ${CMAKE_CURRENT_BINARY_DIR}/functorrc
         DESTINATION share/${PROJECT_NAME}
         COMPONENT runtime )

install( FILES ${RESOURCES} 
               ${CMAKE_CURRENT_BINARY_DIR}/pluginrc
         DESTINATION share/${PROJECT_NAME}
         COMPONENT runtime )
