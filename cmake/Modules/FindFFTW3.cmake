# - Try to find the FFTW3 library
#
# Once done this will define
#
#  FFTW3_FOUND - system has FFTW3
#  FFTW3_INCLUDE_DIR - the FFTW3 include directory
#  FFTW3_LIBRARIES - the libraries needed to use FFTW3

find_path( FFTW3_INCLUDE_DIR fftw3.h
           PATHS 
           /usr/include
           /usr/local/include )

set( FFTW3_LIBRARIES "" )

foreach( lib
  fftw3
)
  find_library( FFTW3_${lib}_LIBRARY ${lib}
                PATHS
                /usr/lib
                /usr/local/lib )
  if( FFTW3_${lib}_LIBRARY )
    list( APPEND FFTW3_LIBRARIES ${FFTW3_${lib}_LIBRARY} )
  endif( FFTW3_${lib}_LIBRARY )
endforeach()

# handle the QUIETLY and REQUIRED arguments and set GKG_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( FFTW3 REQUIRED_VARS FFTW3_INCLUDE_DIR FFTW3_LIBRARIES )

if ( FFTW3_FOUND )
message( STATUS "FFTW3 found" )
endif( FFTW3_FOUND )

mark_as_advanced( FFTW3_LIBRARIES FFTW3_INCLUDE_DIR )
