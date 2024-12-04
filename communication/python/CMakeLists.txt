###############################################################################
# SOURCES
###############################################################################

set( SOURCES
gkg_wrapper.cxx
)

set( INCLUDE_DIRS
${COMPONENT_INCLUDE_DIRS}
${GKG_INCLUDE_DIRS}
)


###############################################################################
# SETTINGS
###############################################################################

set( tmpStr "" )
foreach( i ${INCLUDE_DIRS} )
  string( APPEND tmpStr "\"-I${i}\", " )
endforeach()
string( REGEX REPLACE ",\ $" "" INCLUDE_STRING "${tmpStr}" )

set( LIBRARY_DIR
  "${CMAKE_BINARY_DIR}/communication/src/library"
)
set( LIBRARY_STRING 
  "\"${LIBRARY_DIR}/libgkg-communication.so\", \"-Wl,-rpath,${LIBRARY_DIR}\""
)


###############################################################################
# CONFIGURATION
###############################################################################

configure_file( setup.py.in setup.py )

file( COPY ${SOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )


###############################################################################
# COMPILATION
###############################################################################

set( TARGET_MODULE ${PROJECT_NAME}.so )
add_custom_command( OUTPUT ${TARGET_MODULE}
                    COMMAND ${PYTHON_EXECUTABLE} 
                            ${CMAKE_CURRENT_BINARY_DIR}/setup.py build
                    COMMAND ${CMAKE_COMMAND} -E copy
                            build/*/${PROJECT_NAME}*.so
                            ${TARGET_MODULE}
                    DEPENDS ${SOURCES} )
add_custom_target( GkgPythonModule ALL
                   DEPENDS ${TARGET_MODULE} gkg-communication
                   VERBATIM )


###############################################################################
# INSTALLATION
###############################################################################

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_MODULE}
         DESTINATION share/${PROJECT_NAME}/python
         COMPONENT runtime )
