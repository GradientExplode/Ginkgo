add_subdirectory( gkg-processing-plugin-functors )


############################### DCMTK plugin ###################################
if ( BUILD_GINKGO_DICOM_PLUGIN )
  add_subdirectory( gkg-processing-plugin-dicom )
else ( BUILD_GINKGO_DICOM_PLUGIN )
  message( "Warning: gkg-processing-plugin-dicom not built due to missing PIO or DCMTK dependency" )
endif( BUILD_GINKGO_DICOM_PLUGIN )


################################# GSL/LAPACK plugin ############################
if ( BUILD_GINKGO_GSL_LAPACK_PLUGIN )
  add_subdirectory( gkg-processing-plugin-gsl-lapack )
else ( BUILD_GINKGO_GSL_LAPACK_PLUGIN )
  message( "Warning: gkg-processing-plugin-gsl-lapack not built due to missing GSL or LAPACK or BLAS dependencies" )
endif ( BUILD_GINKGO_GSL_LAPACK_PLUGIN )


################################### NIFTI plugin ###############################
if ( BUILD_GINKGO_NIFTI_PLUGIN )
  add_subdirectory( gkg-processing-plugin-nifti )
else ( BUILD_GINKGO_NIFTI_PLUGIN )
  message( "Warning: gkg-processing-plugin-nifti not built due to missing PIO or NIFTI or ZLIB dependencies" )
endif ( BUILD_GINKGO_NIFTI_PLUGIN )


################################ SVM plugin ####################################
if ( BUILD_GINKGO_SVM_PLUGIN )
  add_subdirectory( gkg-processing-plugin-svm )
else ( BUILD_GINKGO_SVM_PLUGIN )
  message( "Warning: gkg-processing-plugin-svm not built due to missing SVM dependency" )
endif( BUILD_GINKGO_SVM_PLUGIN )


############################### CGAL plugin ####################################
if ( BUILD_GINKGO_CGAL_PLUGIN )
  add_subdirectory( gkg-processing-plugin-cgal )
else ( BUILD_GINKGO_CGAL_PLUGIN )
  message( "Warning: gkg-processing-plugin-cgal not built due to missing CGAL dependency" )
endif( BUILD_GINKGO_CGAL_PLUGIN )


