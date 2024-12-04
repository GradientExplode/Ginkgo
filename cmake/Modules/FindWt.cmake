find_path( Wt_INCLUDE_DIR Wt/WObject.h
           PATHS
           /usr/include
           /usr/local/include )

if( Wt_INCLUDE_DIR )

  set( Wt_LIBRARIES "" )

  foreach( lib
    wt
    wtext
    wthttp
    wttest
    wtfcgi
    wtdbo
    wtdbosqlite3
    wtdbopostgres )

    find_library( Wt_${lib}_LIBRARY ${lib}
                  PATHS
                  /usr/lib
                  /usr/local/lib )

    if( Wt_${lib}_LIBRARY )
      list( APPEND Wt_LIBRARIES ${Wt_${lib}_LIBRARY} )
    endif( Wt_${lib}_LIBRARY )

  endforeach()

  # handle the QUIETLY and REQUIRED arguments and set BRAINVISA_FOUND to TRUE if
  # all listed variables are TRUE
  include( FindPackageHandleStandardArgs )
  find_package_handle_standard_args( Wt REQUIRED_VARS
                                     Wt_INCLUDE_DIR Wt_LIBRARIES )

  if ( Wt_FOUND )
    message( STATUS "Wt found" )
  endif( Wt_FOUND )

  mark_as_advanced( Wt_LIBRARIES Wt_INCLUDE_DIR )

endif( Wt_INCLUDE_DIR )
