# - Try to find the Brainvisa library
#
# Once done this will define
#
#  BRAINVISA_FOUND - system has Brainvisa
#  BRAINVISA_INCLUDE_DIRS - the Brainvisa include directories
#  BRAINVISA_LIBRARIES - the libraries needed to use Brainvisa

set( BRAINVISA_BUILD_PATH $ENV{BRAINVISA_BUILD_PATH} )
set( BRAINVISA_INCLUDE_DIRS ${BRAINVISA_BUILD_PATH}/include )


find_path( BRAINVISA_INCLUDE_DIRS anatomist/application/Anatomist.h
           PATHS 
           /usr/include
           /usr/local/include
           ${BRAINVISA_BUILD_PATH}/include )


set( BRAINVISA_LIBRARIES "" )

foreach( lib
  aimsalgo
  aimscifti
  aimsdata
  aimsecat
  aimsfdf
  aimsgui
  aimsjpeg
  aimsply
  aimsqtformats
  aimstiff
  aimsvida
  anatomist
  cartobase
  cartobase_0
  cartobase_1
  cartodata
  graph
  pyaims
  pyanatomistexports
  somadicom
  somagis
  soma-io
  somaminc
  somanifti
  somaopenslide
  somatiff )

  find_library( BRAINVISA_${lib}_LIBRARY ${lib}
                PATHS
                /usr/lib
                /usr/local/lib
                ${BRAINVISA_BUILD_PATH}/lib )

  if( BRAINVISA_${lib}_LIBRARY )
    list( APPEND BRAINVISA_LIBRARIES ${BRAINVISA_${lib}_LIBRARY} )
  endif( BRAINVISA_${lib}_LIBRARY )

endforeach()


# handle the QUIETLY and REQUIRED arguments and set BRAINVISA_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Brainvisa REQUIRED_VARS
                                   BRAINVISA_INCLUDE_DIRS BRAINVISA_LIBRARIES )

if ( BRAINVISA_FOUND )
message( STATUS "Brainvisa found" )
endif( BRAINVISA_FOUND )

mark_as_advanced( BRAINVISA_LIBRARIES BRAINVISA_INCLUDE_DIRS )
