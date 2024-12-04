################################################################################
# HEADERS
################################################################################

set( HEADERS
gkg-deep-nuclei-deepnuclei/BackgroundNormalizedSquareCenteredPhiFunction.h
gkg-deep-nuclei-deepnuclei/CsfNormalizedSquareCenteredPhiFunction.h
gkg-deep-nuclei-deepnuclei/LabelDependentTransform3dRelaxationFunction.h
gkg-deep-nuclei-deepnuclei/LabelDependentTransform3dRelaxationFunction_i.h
gkg-deep-nuclei-deepnuclei/LabelIndependentTransform3dRelaxationFunction.h
gkg-deep-nuclei-deepnuclei/LabelIndependentTransform3dRelaxationFunction_i.h
gkg-deep-nuclei-midsagittalplane/MidSagittalPlane.h
gkg-deep-nuclei-midsagittalplane/MidSagittalPlane_i.h
gkg-deep-nuclei-midsagittalplane/MidSagittalPlaneContext.h
gkg-deep-nuclei-midsagittalplane/MidSagittalPlaneContext_i.h
gkg-deep-nuclei-midsagittalplane/MidSagittalPlaneFunction.h
gkg-deep-nuclei-midsagittalsurface/MidSagittalSurface.h
gkg-deep-nuclei-midsagittalsurface/MidSagittalSurface_i.h
gkg-deep-nuclei-midsagittalsurface/MidSagittalSurfaceContext2d.h
gkg-deep-nuclei-midsagittalsurface/MidSagittalSurfaceContext2d_i.h
gkg-deep-nuclei-midsagittalsurface/MidSagittalSurfaceContext3d.h
gkg-deep-nuclei-midsagittalsurface/MidSagittalSurfaceContext3d_i.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-deep-nuclei-deepnuclei/BackgroundNormalizedSquareCenteredPhiFunction.cxx
gkg-deep-nuclei-deepnuclei/CsfNormalizedSquareCenteredPhiFunction.cxx
gkg-deep-nuclei-midsagittalplane/MidSagittalPlaneFunction.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-deep-nuclei SHARED ${SOURCES} )
set_property( TARGET gkg-deep-nuclei PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-deep-nuclei gkg-processing )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-deep-nuclei DESTINATION lib COMPONENT runtime )
