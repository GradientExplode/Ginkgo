ORIENTATION_COUNT=200

GkgDwiQSpaceSampling \
-dw Input/dwRef.ima \
-stringParameters spherical multiple-shell same-orientation-set gkg \
-scalarParameters 4 700 1500 3000 4500 $ORIENTATION_COUNT \
-verbose

############################################################################
# cleaning directories
############################################################################

rm -rf SyntheticSignal
rm -rf Diffusoid
rm -rf TensorOdf
rm -rf NoddiEstimation


############################################################################
# creating directories
############################################################################

mkdir SyntheticSignal
mkdir Diffusoid
mkdir TensorOdf
mkdir NoddiEstimation


############################################################################
# 
############################################################################

GkgTestNoddiModel01 \
-dwRef Input/dwRef.ima \
-outputOrientationCount $ORIENTATION_COUNT \
-isotropicFraction 0.20 \
-intracellularFraction 0.60 \
-kappa 4.0 \
-principalOrientationX 0.707106781 \
-principalOrientationY 0.707106781 \
-principalOrientationZ 0.0 \
-parallelDiffusivity 1.7e-9 \
-isotropicDiffusivity 3.0e-9 \
-stationaryFraction 0.2 \
-noiseStandardDeviation 20 \
-initialParameters  0.0   0.5     4.0   0.0     0.0     0.0   1.7e-9    3.0e-9  0.0  \
-lowerParameters    0.0   0.0     0.0  -1.0    -1.0    -1.0   0.0       0.0     0.0 \
-upperParameters    1.0   1.0    10.0  +1.0    +1.0    +1.0   3.0e-9    6.0e-9  1.0 \
-deltaParameters    0.1   0.1     1.0    0.02   0.02   0.02   0.1e-9   0.50e-9  0.05 \
-fixedParameters     0    0       0      1      1      1      1        1        0 \
-mcmcParameters     500 100 50 10000 \
-ov SyntheticSignal/signal \
-od NoddiEstimation/watson \
-applyQuickSearch true \
-applyNLP true \
-applyMCMC true \
-verbose


############################################################################
# computing diffusoid
############################################################################

GkgDwiDiffusoidField \
-t2 SyntheticSignal/signal_t2.ima \
-dw SyntheticSignal/signal_dw.ima \
-m SyntheticSignal/signal_mask.ima \
-type acquisition_diffusoid_cartesian_field \
-f max_normalized_mesh \
-o Diffusoid/max_normalized_mesh \
-outputOrientationCount 1000 \
-verbose 


############################################################################
# computing odf
############################################################################

GkgDwiOdfField \
-t2 SyntheticSignal/signal_t2.ima \
-dw SyntheticSignal/signal_dw.ima \
-m SyntheticSignal/signal_mask.ima \
-type aqball_odf_cartesian_field \
-f odf_site_map odf_texture_map \
-o TensorOdf/dti_site_map  TensorOdf/dti_texture_map  \
-scalarParameters 2 0.006 0.0 \
-outputOrientationCount 1000 \
-verbose 



# GkgDwiOdfField \
# -t2 SimulatedSignal/signal_t2.ima \
# -dw SimulatedSignal/signal_dw.ima \
# -m SimulatedSignal/signal_mask.ima \
# -type noddi_odf_cartesian_field \
# -f intracellular_fraction \
#    isotropic_diffusivity \
#    isotropic_fraction \
#    kappa \
#    odf_site_map \
#    odf_texture_map \
#    orientation_dispersion \
#    rgb \
# -o NoddiOdf/intracellular_fraction \
#    NoddiOdf/isotropic_diffusivity \
#    NoddiOdf/isotropic_fraction \
#    NoddiOdf/kappa \
#    NoddiOdf/odf_site_map \
#    NoddiOdf/odf_texture_map \
#    NoddiOdf/orientation_dispersion \
#    NoddiOdf/rgb \
# -outputOrientationCount 500 \
# -scalarParameters 0.0	0.5   0.0		  1.7e-9  3.0e-9 \
# 		  0.0	0.0   0.0		  0.0	  0.0	 \
# 		  1.0	1.0   100.0		  3.0e-9  6.0e-9 \
# 		  0.1	0.1   1.0		  0.25e-9 0.50e-9 \
# 		  0	0     0 	 1	  1	  1 \
# 		  6 \
# 		  10 5 5 1000 \
# -stringParameters watson \
# -verbose
