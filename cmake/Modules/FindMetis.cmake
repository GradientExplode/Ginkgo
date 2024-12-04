# - Try to find the METIS library
#
# Once done this will define
#
#  METIS_FOUND - system has Torch
#  METIS_INCLUDE_DIR - the Torch include directory
#  METIS_LIBRARIES - the libraries needed to use Torch

find_path( METIS_INCLUDE_DIR metis.h
           PATHS 
           /usr/include )

find_library( METIS_LIBRARIES metis
              PATHS
              /usr/lib/x86_64-linux-gnu )

# handle the QUIETLY and REQUIRED arguments and set METIS_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Metis REQUIRED_VARS METIS_INCLUDE_DIR METIS_LIBRARIES )

if ( METIS_FOUND )
message( STATUS "metis found" )
endif( METIS_FOUND )

mark_as_advanced( METIS_LIBRARIES METIS_INCLUDE_DIR )
