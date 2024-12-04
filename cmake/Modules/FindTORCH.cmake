# - Try to find the Torch library
#
# Once done this will define
#
#  TORCH_FOUND - system has Torch
#  TORCH_INCLUDE_DIR - the Torch include directory
#  TORCH_LIBRARIES - the libraries needed to use Torch

find_path( TORCH_INCLUDE_DIR torch.h
           PATHS 
           /usr/include/torch
           /usr/local/include/torch
           /usr/include/torch/csrc/api/include/torch )

find_library( TORCH_LIBRARIES torch
              PATHS
              /usr/lib
              /usr/local/lib )

# handle the QUIETLY and REQUIRED arguments and set TORCH_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( TORCH REQUIRED_VARS TORCH_INCLUDE_DIR TORCH_LIBRARIES )

if ( TORCH_FOUND )
message( STATUS "Torch found" )
endif( TORCH_FOUND )

mark_as_advanced( TORCH_LIBRARIES TORCH_INCLUDE_DIR )
