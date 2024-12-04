GkgDwiNoiseFilter \
-t2 t2.ima \
-dw dw-b6000.ima \
-ms mask_brain.ima \
-mn mask_noise.ima \
-ot2 t2-b6000-noisefree.ima \
-odw dw-b6000-noisefree.ima \
-n 3 \
-maximumSHOrder 6 \
-laplaceBeltramiFactor 0.0 \
-spatialWeightFactor 1.0 \
-structuralWeightFactor 1.0 \
-verbose


#GkgDwiTensorField \
#-t2 t2-b6000-noisefree \
#-dw dw-b6000-noisefree \
#-m mask_brain.ima \
#-f rgb \
#-o rgb-b6000-noisefree \
#-verbose
