################################################################################
# SOURCES
################################################################################

set( SOURCES
GkgTrmToAntsAffineTransformation3d.py
)

set( SCRIPTS
GkgTrmToAntsAffineTransformation3d
)


################################################################################
# Installation
################################################################################


if( NUITKA )

  set( EXCLUDES
  PyQt5
  matplotlib
  matplotlib.backends.backend_qt4agg
  matplotlib.figure
  matplotlib.lines
  matplotlib.widgets
  soma_workflow.client
  soma_workflow.constants
  soma_workflow.gui.workflowGui
  PIL
  numpy
  )

  add_nuitka( OUTPUT_EXE ${SOURCES}
              EXCLUDE ${EXCLUDES} )
  install( FILES ${CMAKE_BINARY_DIR}/nuitka/${OUTPUT_EXE}
           DESTINATION bin
           COMPONENT runtime
           PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
                       GROUP_READ GROUP_EXECUTE
                       WORLD_READ WORLD_EXECUTE )
else( NUITKA )
  install( FILES ${SOURCES} 
           DESTINATION share/${PROJECT_NAME}/scripts
           COMPONENT runtime )
  install( FILES ${SCRIPTS}
           DESTINATION bin
           COMPONENT runtime
           PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
                       GROUP_READ GROUP_EXECUTE
                       WORLD_READ WORLD_EXECUTE )
endif( NUITKA )
