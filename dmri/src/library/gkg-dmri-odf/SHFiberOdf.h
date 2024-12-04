#ifndef _gkg_dmri_odf_SHFiberOdf_h_
#define _gkg_dmri_odf_SHFiberOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-dmri-diffusion-kernel/DiffusionKernel.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


class SHFiberOdf : public OrientationDistributionFunction
{

  public:

    virtual ~SHFiberOdf();

 protected:

    // Tournier et al NeuroImage 2007
    // Strongly advised if maximumSphericalHarmonicOrder >= 6
    // Take ~ 1/2 second per voxel -> 3-4 days of computation for a full brain
    SHFiberOdf( const OrientationSet* orientationSet = 0 );

    void applyConstrainedSuperResolutionRegularization(
                                               const Vector& normalizedSignal,
                                               const Matrix& B,
                                               const Matrix& regularizationMatrix,
                                               const Matrix& convolutionMatrix,
                                               bool superResolution );

    void getDeconvolutionMatrices( const DiffusionKernel& diffusionKernel,
                                   int32_t maximumSphericalHarmonicOrder,
                                   double bValue,
                                   Matrix& regularizationMatrix,
                                   Matrix& convolutionMatrix );

    void getSuperResolutionMatrices( const Matrix& A,
                                     const Matrix& L2,
                                     const Vector& signal,
                                     Matrix& M,
                                     Vector& bPrime );
};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for SHFiberOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::SHFiberOdf, gkg_SHFiberOdf );


#endif
