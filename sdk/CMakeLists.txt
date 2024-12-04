cmake_minimum_required( VERSION 3.1 )


###############################################################################
# PROJECT
###############################################################################

project( gkg )

# something has changed somewhere, the ELF format is not recognized by default
# any longer and makes install fail because of a rpath/relink step missing.
# The solution is to force ELF format
# see https://cmake.org/Bug/view.php?id=13934#c37157
set( CMAKE_EXECUTABLE_FORMAT "ELF" )


###############################################################################
# DEPENDENCIES
###############################################################################

set( CMAKE_MODULE_PATH
     /usr/share/gkg/cmake/Modules
     $ENV{GKG_SVN}/gkg-7.0-CentOS-7.4.1708/share/gkg/cmake/Modules
     ${CMAKE_MODULE_PATH} )

find_package( GKG REQUIRED )
find_package( CUDA QUIET )
find_package( Wt QUIET )


###############################################################################
# DEFAULT INSTALL DIRECTORY
###############################################################################

set( CMAKE_INSTALL_PREFIX "$ENV{HOME}" )


###############################################################################
# COMPILATION FLAGS
###############################################################################

set( CMAKE_CXX_FLAGS
     "${CMAKE_CXX_FLAGS} -fPIC -W -Wall -Wpointer-arith -Wformat -pipe -std=c++11" )

add_definitions( -D_XOPEN_SOURCE=500 -D_POSIX_SOURCE
                 -D_POSIX_C_SOURCE=199506 -D_REENTRANT
                 -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS
                 -D__STDC_FORMAT_MACROS
                 -D_LARGEFILE_SOURCE -D_LARGE_FILES
                 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
                 -D__USE_LARGEFILE64 -D__USE_FILE_OFFSET64
                 -DBOOST_SPIRIT_THREADSAFE
                 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG
                 -DWT_DEPRECATED_3_0_0 -DWT_WITH_OLD_INTERNALPATH_API 
                 -D_GLIBCXX_USE_CXX11_ABI=1 )


###############################################################################
# LINKER FLAGS
###############################################################################

set( CMAKE_EXE_LINKER_FLAGS
     "${CMAKE_EXE_LINKER_FLAGS}" )


###############################################################################
# COMPONENTS
###############################################################################

add_subdirectory( src )
