add_subdirectory( python )
add_subdirectory( scripts )

###############################################################################
# INSTALLATION
###############################################################################

install( DIRECTORY share/ DESTINATION share/${PROJECT_NAME} COMPONENT runtime )
