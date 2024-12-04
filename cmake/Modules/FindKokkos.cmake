# - Try to find the Kokkos library
#
# Once done this will define
#
#  Kokkos_FOUND - system has Kokkos
#  Kokkos_INCLUDE_DIR - the Kokkos include directory
#  Kokkos_LIBRARIES - the libraries needed to use Kokkos

find_path( Kokkos_INCLUDE_DIR Kokkos_Core.hpp
           PATHS
           /usr/include/kokkos
           /usr/local/include/kokkos )

set( Kokkos_LIBRARIES "" )

foreach( lib
  kokkos
  kokkoscontainers
  kokkoscore )

  find_library( Kokkos_${lib}_LIBRARY ${lib}
                PATHS
                /usr/lib
                /usr/local/lib )

  if( Kokkos_${lib}_LIBRARY )
    list( APPEND Kokkos_LIBRARIES ${Kokkos_${lib}_LIBRARY} )
  endif( Kokkos_${lib}_LIBRARY )
endforeach()

# handle the QUIETLY and REQUIRED arguments and set Kokkos_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Kokkos REQUIRED_VARS Kokkos_INCLUDE_DIR Kokkos_LIBRARIES )

if ( Kokkos_FOUND )
message( STATUS "Kokkos found" )
endif( Kokkos_FOUND )

mark_as_advanced( Kokkos_LIBRARIES Kokkos_INCLUDE_DIR )
