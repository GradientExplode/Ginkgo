# - Try to find the GKG library
#
# Once done this will define
#
#  GKG_FOUND - system has GKG
#  GKG_INCLUDE_DIR - the GKG include directory
#  GKG_LIBRARIES - the libraries needed to use GKG

find_path( GKG_INCLUDE_DIR gkg-core-object/BaseObject.h
           PATHS 
           /usr/include/gkg
           /usr/local/include/gkg
	   $ENV{GKG_SVN}/gkg-7.0-CentOS-7.4.1708/include/gkg )

set( GKG_LIBRARIES "" )

foreach( lib
  gkg-anatomist-plugin
  gkg-communication
  gkg-communication-plugin-functors
  gkg-dmri
  gkg-dmri-plugin-functors
  gkg-dmri-plugin-vtk
  gkg-core
  gkg-dartel
  gkg-dartel-plugin-functors
  gkg-fmri
  gkg-fmri-plugin-functors
  gkg-graphics-QtGL
  gkg-graphics-QtGL-plugin-functors
  gkg-graphics-Video
  gkg-graphics-X11
  gkg-graphics-X11-plugin-functors
  gkg-global-tractography
  gkg-global-tractography-plugin-functors
  gkg-mri-reconstruction
  gkg-mri-reconstruction-plugin-functors
  gkg-deep-nuclei
  gkg-deep-nuclei-plugin-functors
  gkg-processing
  gkg-processing-plugin-dicom
  gkg-processing-plugin-functors
  gkg-processing-plugin-gsl-lapack
  gkg-processing-plugin-nifti
  gkg-processing-plugin-svm
  gkg-qmri
  gkg-qmri-plugin-functors
  gkg-simulation
  gkg-simulation-plugin-functors
)
  find_library( GKG_${lib}_LIBRARY ${lib}
                PATHS
                /usr/lib
                /usr/local/lib
		$ENV{GKG_SVN}/gkg-7.0-CentOS-7.4.1708/lib )
  if( GKG_${lib}_LIBRARY )
    list( APPEND GKG_LIBRARIES ${GKG_${lib}_LIBRARY} )
  endif( GKG_${lib}_LIBRARY )
endforeach()

# handle the QUIETLY and REQUIRED arguments and set GKG_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( GKG REQUIRED_VARS GKG_INCLUDE_DIR GKG_LIBRARIES )

if ( GKG_FOUND )
message( STATUS "GKG found" )
endif( GKG_FOUND )

mark_as_advanced( GKG_LIBRARIES GKG_INCLUDE_DIR )
