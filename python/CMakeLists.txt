################################################################################
# SOURCES
################################################################################

set( SOURCES
core/command/__init__.py
core/command/Command.py
core/command/CommandFactory.py
core/command/Commands.py
core/patterns/__init__.py
core/patterns/Singleton.py
processing/bias/__init__.py
processing/bias/AntsBiasFieldCorrection.py
processing/registration/__init__.py
processing/registration/AntsRegistration3d.py
simulation/SWCDataReconstruction.py
)


################################################################################
# Installation
################################################################################

if( NOT NUITKA )
  get_directories( SRC_DIR_LIST SOURCES )
  foreach( d ${SRC_DIR_LIST} )
    install( DIRECTORY ${d}
             DESTINATION share/${PROJECT_NAME}/python
             COMPONENT runtime )
  endforeach()
endif( NOT NUITKA )
