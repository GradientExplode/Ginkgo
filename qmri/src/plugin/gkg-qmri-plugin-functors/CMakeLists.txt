################################################################################
# HEADERS
################################################################################

set( HEADERS
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperCommand.h
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperGauge.h
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperLoopContext.h
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMCMCFunction.h
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMCMCParameterSet.h
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryNLPFunction.h
MyelinWaterFractionMapper/IRSEBasedMyelinWaterFractionMCMCFunction.h
MyelinWaterFractionMapper/MyelinWaterFractionMapperCommand.h
MyelinWaterFractionMapper/MyelinWaterFractionMapperGauge.h
MyelinWaterFractionMapper/MyelinWaterFractionMapperLoopContext.h
MyelinWaterFractionMapper/MyelinWaterFractionMCMCFunction.h
MyelinWaterFractionMapper/MyelinWaterFractionMCMCParameterSet.h
MyelinWaterFractionMapper/MyelinWaterFractionNLPFunction.h
MyelinWaterFractionMapper/VFASPGRBasedMyelinWaterFractionMCMCFunction.h
RelaxometryKalmanFilter/RelaxometryKalmanFilterCommand.h
RelaxometryMapper/RelaxometryMapperCommand.h
RelaxometryT1CorticalLaminationMapper/RelaxometryT1CorticalLaminationMapperCommand.h
SingleCompartmentRelaxometryMapper/AcquisitionParameterSet.h
SingleCompartmentRelaxometryMapper/AcquisitionParameterSetFactory.h
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperCommand.h
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperGauge.h
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperLoopContext.h
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunctionFactory.h
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunction.h
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCParameterSet.h
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryNLPFunction.h
SingleCompartmentRelaxometryMapper/T1IRSEAcquisitionParameterSet.h
SingleCompartmentRelaxometryMapper/T1IRSERelaxometryMCMCFunction.h
SingleCompartmentRelaxometryMapper/T1VFASPGRAcquisitionParameterSet.h
SingleCompartmentRelaxometryMapper/T1VFASPGRRelaxometryMCMCFunction.h
SingleCompartmentRelaxometryMapper/T2MSMEAcquisitionParameterSet.h
SingleCompartmentRelaxometryMapper/T2MSMERelaxometryMCMCFunction.h
SingleCompartmentRelaxometryMapper/T2StarMGREAcquisitionParameterSet.h
SingleCompartmentRelaxometryMapper/T2StarMGRERelaxometryMCMCFunction.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperCommand.cxx
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperGauge.cxx
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperLoopContext.cxx
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMCMCFunction.cxx
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMCMCParameterSet.cxx
MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryNLPFunction.cxx
MyelinWaterFractionMapper/IRSEBasedMyelinWaterFractionMCMCFunction.cxx
MyelinWaterFractionMapper/MyelinWaterFractionMapperCommand.cxx
MyelinWaterFractionMapper/MyelinWaterFractionMapperGauge.cxx
MyelinWaterFractionMapper/MyelinWaterFractionMapperLoopContext.cxx
MyelinWaterFractionMapper/MyelinWaterFractionMCMCFunction.cxx
MyelinWaterFractionMapper/MyelinWaterFractionMCMCParameterSet.cxx
MyelinWaterFractionMapper/MyelinWaterFractionNLPFunction.cxx
MyelinWaterFractionMapper/VFASPGRBasedMyelinWaterFractionMCMCFunction.cxx
RelaxometryKalmanFilter/RelaxometryKalmanFilterCommand.cxx
RelaxometryMapper/RelaxometryMapperCommand.cxx
RelaxometryT1CorticalLaminationMapper/RelaxometryT1CorticalLaminationMapperCommand.cxx
SingleCompartmentRelaxometryMapper/AcquisitionParameterSet.cxx
SingleCompartmentRelaxometryMapper/AcquisitionParameterSetFactory.cxx
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperCommand.cxx
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperGauge.cxx
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperLoopContext.cxx
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunction.cxx
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunctionFactory.cxx
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCParameterSet.cxx
SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryNLPFunction.cxx
SingleCompartmentRelaxometryMapper/T1IRSEAcquisitionParameterSet.cxx
SingleCompartmentRelaxometryMapper/T1IRSERelaxometryMCMCFunction.cxx
SingleCompartmentRelaxometryMapper/T1VFASPGRAcquisitionParameterSet.cxx
SingleCompartmentRelaxometryMapper/T1VFASPGRRelaxometryMCMCFunction.cxx
SingleCompartmentRelaxometryMapper/T2MSMEAcquisitionParameterSet.cxx
SingleCompartmentRelaxometryMapper/T2MSMERelaxometryMCMCFunction.cxx
SingleCompartmentRelaxometryMapper/T2StarMGREAcquisitionParameterSet.cxx
SingleCompartmentRelaxometryMapper/T2StarMGRERelaxometryMCMCFunction.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-qmri-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-qmri-plugin-functors PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )

target_link_libraries( gkg-qmri-plugin-functors gkg-qmri )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-qmri-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-qmri-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
