if ( QT_FOUND AND QWT_FOUND AND OPENGL_FOUND )
  add_subdirectory( QtGL )
endif( QT_FOUND AND QWT_FOUND AND OPENGL_FOUND  )

add_subdirectory( Video )

if ( X11_FOUND )
  add_subdirectory( X11 )
endif( X11_FOUND )
