###############################################################################
# SETTINGS
###############################################################################

string( TIMESTAMP CURRENT_DATE "%Y_%m_%d" )
string( TIMESTAMP CURRENT_YEAR "%Y" )


###############################################################################
# CONFIGURATION
###############################################################################

configure_file( COPYRIGHT.in COPYRIGHT )
configure_file( VERSION.in VERSION )


###############################################################################
# INSTALLATION
###############################################################################

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/COPYRIGHT
               ${CMAKE_CURRENT_BINARY_DIR}/VERSION
         DESTINATION share/doc/${PROJECT_NAME}
         COMPONENT runtime )
