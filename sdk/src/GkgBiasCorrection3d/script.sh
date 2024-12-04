GkgBiasCorrection3d \
-i ~/Data/DemoBaseArchi/00-InputData/MORPHOLOGIST/t1-1mm-1/001/t1mri/default_acquisition/001.ima \
-o /tmp/corrected.ima \
-c /tmp/correction.ima \
-correctionStride 16 16 16 \
-histogramBinCount 256 \
-inputCompressionLevelCount 256 \
-lowerThreshold 120 \
-gridCount 4 \
-gibbsSamplerIncrementCount 2 \
-gibbsSamplerIncrementMagnitude 1.03 \
-gibbsSamplerIncrementGeometry 0.66 \
-verbose


