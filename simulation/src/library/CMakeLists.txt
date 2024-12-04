################################################################################
# HEADERS
################################################################################

set( HEADERS
#gkg-simulation-dendrite-population/Dendrite.h
#gkg-simulation-dendrite-population/DendritePopulation.h
gkg-simulation-diffusion-process/MonteCarloDiffusionProcess.h
gkg-simulation-distribution/ConstantDistribution.h
gkg-simulation-distribution/DistributionFactory.h
gkg-simulation-distribution/GammaDistribution.h
gkg-simulation-distribution/ParameterDistribution.h
gkg-simulation-distribution/UniformDistribution.h
gkg-simulation-fiber-population/Fiber.h
gkg-simulation-fiber-population/FiberPopulation.h
gkg-simulation-glia-population/Astrocyte.h
gkg-simulation-glia-population/AstrocytePopulation.h
gkg-simulation-glia-population/GlialCell.h
gkg-simulation-glia-population/GliaPopulation.h
gkg-simulation-glia-population/Oligodendrocyte.h
gkg-simulation-glia-population/OligodendrocytePopulation.h
gkg-simulation-io/DefaultAsciiItemReaderOfCell_i.h
gkg-simulation-io/DefaultAsciiItemReaderOfAtom_i.h
gkg-simulation-io/DefaultAsciiItemWriterOfCell_i.h
gkg-simulation-io/DefaultAsciiItemWriterOfAtom_i.h
gkg-simulation-io/DefaultBinaryItemReaderOfCell_i.h
gkg-simulation-io/DefaultBinaryItemReaderOfAtom_i.h
gkg-simulation-io/DefaultBinaryItemWriterOfCell_i.h
gkg-simulation-io/DefaultBinaryItemWriterOfAtom_i.h
gkg-simulation-io/DefaultBSwapItemReaderOfCell_i.h
gkg-simulation-io/DefaultBSwapItemReaderOfAtom_i.h
gkg-simulation-io/DefaultBSwapItemWriterOfCell_i.h
gkg-simulation-io/DefaultBSwapItemWriterOfAtom_i.h
gkg-simulation-io/ForceHardLinkWithSimulationLibrary.h
gkg-simulation-io/MedusaGeometryAnalyzer.h
gkg-simulation-io/MedusaGeometryDiskFormat.h
gkg-simulation-kokkos/Defines.h
gkg-simulation-kokkos/DiffusionProcessKernels.h
gkg-simulation-kokkos/DiffusionProcessKernels_i.h
gkg-simulation-kokkos/AtomLut.h
gkg-simulation-kokkos/AtomLut_i.h
gkg-simulation-kokkos/AtomOverlapSolver.h
gkg-simulation-kokkos/AtomOverlapSolver_i.h
gkg-simulation-mri-sequence/MRISequenceFactory.h
gkg-simulation-mri-sequence/MRISequence.h
gkg-simulation-mri-sequence/MRISequencePool.h
gkg-simulation-mri-sequence/OGSESequence.h
gkg-simulation-mri-sequence/PGSESequence.h
gkg-simulation-mri-sequence/TrapezoidCosineFunction.h
gkg-simulation-mri-sequence/TrapezoidGradientPulse.h
gkg-simulation-mri-sequence/TrapezoidSineFunction.h
gkg-simulation-neuron-population/Neuron.h
gkg-simulation-neuron-population/NeuronPopulation.h
gkg-simulation-soma-population/Soma.h
gkg-simulation-soma-population/SomaPopulation.h
gkg-simulation-virtual-tissue/Atom.h
gkg-simulation-virtual-tissue/AtomColliderFactory.h
gkg-simulation-virtual-tissue/AtomForceApplierFactory.h
gkg-simulation-virtual-tissue/AtomInterpolatorFactory.h
gkg-simulation-virtual-tissue/AtomMethodFactory.h
gkg-simulation-virtual-tissue/AtomSmootherFactory.h
gkg-simulation-virtual-tissue/Cell.h
gkg-simulation-virtual-tissue/EllipsoidAtom.h
gkg-simulation-virtual-tissue/Population.h
gkg-simulation-virtual-tissue/PopulationFactory.h
gkg-simulation-virtual-tissue/PopulationAtomLutContainer.h
gkg-simulation-virtual-tissue/SphereAtom.h
gkg-simulation-virtual-tissue/VirtualTissue.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-simulation-dendrite-population/Dendrite.cxx
gkg-simulation-dendrite-population/DendritePopulation.cxx
gkg-simulation-diffusion-process/MonteCarloDiffusionProcess.cxx
gkg-simulation-distribution/ConstantDistribution.cxx
gkg-simulation-distribution/DistributionFactory.cxx
gkg-simulation-distribution/GammaDistribution.cxx
gkg-simulation-distribution/ParameterDistribution.cxx
gkg-simulation-distribution/UniformDistribution.cxx
gkg-simulation-fiber-population/Fiber.cxx
gkg-simulation-fiber-population/FiberPopulation.cxx
gkg-simulation-glia-population/Astrocyte.cxx
gkg-simulation-glia-population/AstrocytePopulation.cxx
gkg-simulation-glia-population/GlialCell.cxx
gkg-simulation-glia-population/GliaPopulation.cxx
gkg-simulation-glia-population/Oligodendrocyte.cxx
gkg-simulation-glia-population/OligodendrocytePopulation.cxx
gkg-simulation-io/DefaultAsciiItemReaderOfCell.cxx
gkg-simulation-io/DefaultAsciiItemReaderOfAtom.cxx
gkg-simulation-io/DefaultAsciiItemWriterOfCell.cxx
gkg-simulation-io/DefaultAsciiItemWriterOfAtom.cxx
gkg-simulation-io/DefaultBinaryItemReaderOfCell.cxx
gkg-simulation-io/DefaultBinaryItemReaderOfAtom.cxx
gkg-simulation-io/DefaultBinaryItemWriterOfCell.cxx
gkg-simulation-io/DefaultBinaryItemWriterOfAtom.cxx
gkg-simulation-io/DefaultBSwapItemReaderOfCell.cxx
gkg-simulation-io/DefaultBSwapItemReaderOfAtom.cxx
gkg-simulation-io/DefaultBSwapItemWriterOfCell.cxx
gkg-simulation-io/DefaultBSwapItemWriterOfAtom.cxx
gkg-simulation-io/DefaultItemIOFactory.cxx
gkg-simulation-io/ForceHardLinkWithSimulationLibrary.cxx
gkg-simulation-io/MedusaGeometryAnalyzer.cxx
gkg-simulation-io/MedusaGeometryDiskFormat.cxx
gkg-simulation-mri-sequence/MRISequence.cxx
gkg-simulation-mri-sequence/MRISequenceFactory.cxx
gkg-simulation-mri-sequence/MRISequencePool.cxx
gkg-simulation-mri-sequence/OGSESequence.cxx
gkg-simulation-mri-sequence/PGSESequence.cxx
gkg-simulation-mri-sequence/TrapezoidCosineFunction.cxx
gkg-simulation-mri-sequence/TrapezoidGradientPulse.cxx
gkg-simulation-mri-sequence/TrapezoidSineFunction.cxx
gkg-simulation-neuron-population/Neuron.cxx
gkg-simulation-neuron-population/NeuronPopulation.cxx
gkg-simulation-soma-population/Soma.cxx
gkg-simulation-soma-population/SomaPopulation.cxx
gkg-simulation-virtual-tissue/Atom.cxx
gkg-simulation-virtual-tissue/AtomColliderFactory.cxx
gkg-simulation-virtual-tissue/AtomForceApplierFactory.cxx
gkg-simulation-virtual-tissue/AtomInterpolatorFactory.cxx
gkg-simulation-virtual-tissue/AtomMethodFactory.cxx
gkg-simulation-virtual-tissue/AtomSmootherFactory.cxx
gkg-simulation-virtual-tissue/Cell.cxx
gkg-simulation-virtual-tissue/EllipsoidAtom.cxx
gkg-simulation-virtual-tissue/EllipsoidAtomForceApplier.cxx
gkg-simulation-virtual-tissue/EllipsoidAtomToEllipsoidAtomCollider.cxx
gkg-simulation-virtual-tissue/EllipsoidAtomToSphereAtomCollider.cxx
gkg-simulation-virtual-tissue/Population.cxx
gkg-simulation-virtual-tissue/PopulationFactory.cxx
gkg-simulation-virtual-tissue/PopulationAtomLutContainer.cxx
gkg-simulation-virtual-tissue/SphereAtom.cxx
gkg-simulation-virtual-tissue/SphereAtomForceApplier.cxx
gkg-simulation-virtual-tissue/SphereAtomSphereAtomInterpolator.cxx
gkg-simulation-virtual-tissue/SphereAtomSphereAtomSphereAtomSmoother.cxx
gkg-simulation-virtual-tissue/SphereAtomToEllipsoidAtomCollider.cxx
gkg-simulation-virtual-tissue/SphereAtomToSphereAtomCollider.cxx
gkg-simulation-virtual-tissue/VirtualTissue.cxx
)


################################################################################
# Compilation and link
################################################################################

if ( TORCH_FOUND AND GINKGO_USE_TORCH AND GINKGO_TORCH_CUDA_IMPLEMENTATION )

include_directories( ${CUDA_TOOLKIT_INCLUDE} ${Kokkos_INCLUDE_DIR} ${GKG_INCLUDE_DIR} )

else ( TORCH_FOUND AND GINKGO_USE_TORCH AND GINKGO_TORCH_CUDA_IMPLEMENTATION )

include_directories( ${Kokkos_INCLUDE_DIR} ${GKG_INCLUDE_DIR} )

endif ( TORCH_FOUND AND GINKGO_USE_TORCH AND GINKGO_TORCH_CUDA_IMPLEMENTATION )


add_library( gkg-simulation SHARED ${SOURCES} )
set_property( TARGET gkg-simulation PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-simulation gkg-dmri ${Kokkos_LIBRARIES} PRIVATE cuda cudart )



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
install( TARGETS gkg-simulation DESTINATION lib COMPONENT runtime )
