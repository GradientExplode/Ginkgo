#add_subdirectory( GkgSDKBinarizer )

#if( CUDA_FOUND )
#  add_subdirectory( GkgSDKCudaThreshold )
#endif( CUDA_FOUND )

if( Wt_FOUND )
  add_subdirectory( webgl )
  add_subdirectory( GkgWMBundleWebGLViewer )
#  add_subdirectory( planner )
endif( Wt_FOUND )


add_subdirectory( GkgBiasCorrection3d )


add_subdirectory( GkgMapR )
add_subdirectory( GkgMapR1 )
# add_subdirectory( GkgMapR2 )
# add_subdirectory( GkgMapR3 )
