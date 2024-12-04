################################################################################
# HEADERS
################################################################################

set( HEADERS
gkg-fmri-contrast/ContrastEffects.h
gkg-fmri-contrast/ContrastEffectsContext.h
gkg-fmri-glm/BoldGeneralLinearModelContext.h
gkg-fmri-glm/BoldGeneralLinearModelContext_i.h
gkg-fmri-glm/BoldGeneralLinearModelSolveContext.h
gkg-fmri-glm/BoldGeneralLinearModelSolveContext_i.h
gkg-fmri-glm/BoldGeneralLinearModelSolver.h
gkg-fmri-glm/BoldGeneralLinearModelSolver_i.h
gkg-fmri-hrf/CanonicalHRF.h
gkg-fmri-hrf/HemodynamicResponseFunction.h
gkg-fmri-io/ContrastEffectsAnalyzer.h
gkg-fmri-io/ContrastEffectsDiskFormat.h
gkg-fmri-io/ParadigmAnalyzer.h
gkg-fmri-io/ParadigmDiskFormat.h
gkg-fmri-paradigm/Paradigm.h
gkg-fmri-statistics/GLMStatisticalFunction.h
gkg-fmri-statistics/PValueFunction.h
gkg-fmri-statistics/StatisticalFunctionFactory.h
gkg-fmri-statistics/StatisticalFunctionScoreContext.h
gkg-fmri-statistics/StatisticalFunctionScoreMaskContext.h
gkg-fmri-statistics/TScoreFunction.h
gkg-fmri-statistics/ZScoreFunction.h
gkg-fmri-stimulus/ContinuousStimulus.h
gkg-fmri-stimulus/DiscreteStimulus.h
gkg-fmri-stimulus/Stimulus.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-fmri-contrast/ContrastEffects.cxx
gkg-fmri-contrast/ContrastEffectsContext.cxx
gkg-fmri-glm/BoldGeneralLinearModelSolver.cxx
gkg-fmri-hrf/CanonicalHRF.cxx
gkg-fmri-hrf/HemodynamicResponseFunction.cxx
gkg-fmri-io/ContrastEffectsAnalyzer.cxx
gkg-fmri-io/ContrastEffectsDiskFormat.cxx
gkg-fmri-io/ContrastEffectsDiskFormatFactory.cxx
gkg-fmri-io/ParadigmAnalyzer.cxx
gkg-fmri-io/ParadigmDiskFormat.cxx
gkg-fmri-io/ParadigmDiskFormatFactory.cxx
gkg-fmri-paradigm/Paradigm.cxx
gkg-fmri-statistics/GLMStatisticalFunction.cxx
gkg-fmri-statistics/PValueFunction.cxx
gkg-fmri-statistics/StatisticalFunctionFactory.cxx
gkg-fmri-statistics/StatisticalFunctionScore.cxx
gkg-fmri-statistics/StatisticalFunctionScoreContext.cxx
gkg-fmri-statistics/StatisticalFunctionScoreMaskContext.cxx
gkg-fmri-statistics/StatisticalFunctionScoreWithMask.cxx
gkg-fmri-statistics/TScoreFunction.cxx
gkg-fmri-statistics/ZScoreFunction.cxx
gkg-fmri-stimulus/ContinuousStimulus.cxx
gkg-fmri-stimulus/DiscreteStimulus.cxx
gkg-fmri-stimulus/Stimulus.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-fmri SHARED ${SOURCES} )
set_property( TARGET gkg-fmri PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-fmri gkg-processing )


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
install( TARGETS gkg-fmri DESTINATION lib COMPONENT runtime )
