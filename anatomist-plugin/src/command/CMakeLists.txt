if ( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY )

message( STATUS "adding new global tractography viewer" )

else ( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY )

message( STATUS "adding old global tractography viewer" )

add_subdirectory( GkgDwiGlobalTractographyViewer )

endif ( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY )
