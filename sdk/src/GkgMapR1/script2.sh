GkgDiffeomorphicRegistrationScalar \
-reference reference/t2.ima \
-floating floating/t2.ima \
-regularizationWeighting 5.0 \
-halfWidthSearchSpace 8 6 4 \
-controlPointsSpacing 8 6 4 \
-searchSpaceQuantisation 2.0 1.0 0.5 \
-output floating_deformed.ima \
-verbose



GkgDiffeomorphicRegistrationScalar \
-reference reference/reference.ima \
-floating floating/floating.ima \
-regularizationWeighting 5.0 \
-halfWidthSearchSpace 2 1 \
-controlPointsSpacing 2 1 \
-searchSpaceQuantisation 1.0 1.0 \
-output floating_deformed.ima \
-verbose




GkgDiffeomorphicRegistrationScalar \
-reference reference/reference.ima \
-floating floating/floating.ima \
-regularizationWeighting 5.0 \
-halfWidthSearchSpace 8 6 4 \
-controlPointsSpacing 8 6 4 \
-searchSpaceQuantisation 2.0 1.0 0.5 \
-output floating_deformed.ima \
-verbose




GkgDiffeomorphicRegistrationScalar \
-reference reference/reference.ima \
-floating floating/floating.ima \
-regularizationWeighting 5.0 \
-halfWidthSearchSpace 8 6 4 \
-controlPointsSpacing 8 6 4 \
-searchSpaceQuantisation 1.0 1.0 1.0 \
-output floating_deformed.ima \
-verbose
