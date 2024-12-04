################################################################################
# HEADERS
################################################################################

set( HEADERS
CGALComputerGraphicsImplementationFactory.h
CGALPointCloudAlphaShapeMesherImplementation.h
ForceHardLinkWithCGALPlugin.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
CGALComputerGraphicsImplementationFactory.cxx
CGALPointCloudAlphaShapeMesherImplementation.cxx
ForceHardLinkWithCGALPlugin.cxx
)


################################################################################
# Compilation and link
################################################################################

include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )

add_definitions(-DCGAL_EIGEN3_ENABLED)

add_library( gkg-processing-plugin-cgal SHARED ${SOURCES} )
set_property( TARGET gkg-processing-plugin-cgal
              PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-processing-plugin-cgal CGAL::CGAL )


################################################################################
# Installation
################################################################################

install( FILES ${HEADERS}
         DESTINATION include/${PROJECT_NAME}/gkg-processing-plugin-cgal
         COMPONENT devel )
install( TARGETS gkg-processing-plugin-cgal
         DESTINATION lib
         COMPONENT runtime )
