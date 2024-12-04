GkgTestDwiConnectivityMatrix -type 2
GkgDwiConnectivityMatrix \
-b fibers.bundles \
-v1 labels.ima \
-tr1 labels_to_dw.trm \
-m2 sphere.mesh \
-tr2 sphere_to_dw.trm \
-scalarParameters 3 0.1 4.0 4.0 0 0 1.0 100 100 100 \
-f connectivity-matrix \
   average-distance-matrix \
   roi-to-mesh-bundle-map \
   roi-to-roi-connectivity-texture-map \
-o connectivity-matrix \
   average-distance-matrix \
   roi-to-mesh-bundle-map \
   roi-to-roi-connectivity-texture-map \
-verbose
