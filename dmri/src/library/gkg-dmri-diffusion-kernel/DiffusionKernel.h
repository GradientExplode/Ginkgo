#ifndef _gkg_dmri_diffusion_kernel_DiffusionKernel_h_
#define _gkg_dmri_diffusion_kernel_DiffusionKernel_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class DiffusionKernel
{

  public:

    enum TensorType
    {

      SymmetricTensor

    };

    enum FractionalAnisotropySortingType
    {

      DecreasingFractionalAnisotropyOrder,
      IncreasingFractionalAnisotropyOrder

    };

    DiffusionKernel(
     RCPointer< Volume< float > > t2,
     RCPointer< Volume< float > > dw,
     RCPointer< Volume< int16_t > > mask,
     RCPointer< Volume< float > > tissueClassProbabilityMap,
     RCPointer< Transform3d< float > > transform3dFromTissueClassToDiffusion,
     RCPointer< TestFunction< float > > fractionalAnisotropyTestFunction,
     FractionalAnisotropySortingType fractionalAnisotropySortingType,
     RCPointer< TestFunction< float > > tissueClassTestFunction,
     int32_t wantedVoxelCount = 300,
     TensorType tensorType = SymmetricTensor,
     bool verbose = false );

    DiffusionKernel(
     RCPointer< Volume< float > > t2,
     RCPointer< Volume< float > > dw,
     RCPointer< Volume< int16_t > > mask,
     RCPointer< TestFunction< float > > fractionalAnisotropyTestFunction,
     FractionalAnisotropySortingType fractionalAnisotropySortingType,
     int32_t wantedVoxelCount = 300,
     TensorType tensorType = SymmetricTensor,
     bool verbose = false );

    DiffusionKernel( float eigenValue = 1.0,
                     float mainToSecondaryEigenValueRatio = 1.0 );

    DiffusionKernel( const DiffusionKernel& other );

    virtual ~DiffusionKernel();

    DiffusionKernel& operator=( const DiffusionKernel& other );

    TensorType getTensorType() const;
    std::string getTensorTypeName() const;

    int32_t getCandidateVoxelCount() const;
    const Volume< uint8_t >& getCandidateVoxelVolume() const;

    int32_t getSelectedVoxelCount() const;
    const Volume< uint8_t >& getSelectedVoxelVolume() const;

    const Tensor& getTensor() const;

    Matrix getSphericalHarmonicsConvolutionMatrix(
                                  const OrientationSet& orientationSet,
                                  float bValueInSecondsPerMillimeterSquare,
                                  int32_t maximumSphericalHarmonicOrder ) const;

    Matrix getSphericalHarmonicsDeconvolutionMatrix(
                                  const OrientationSet& orientationSet,
                                  float bValueInSecondsPerMillimeterSquare,
                                  int32_t maximumSphericalHarmonicOrder,
                                  bool filtered = false ) const;

  protected:

    void estimateAverageSymmetricTensor(
        RCPointer< Volume< float > > t2,
        RCPointer< Volume< float > > dw,
        RCPointer< Volume< int16_t > > mask,
        RCPointer< Volume< float > > tissueClassProbabilityMap,
        RCPointer< Transform3d< float > > transform3dFromTissueClassToDiffusion,
        RCPointer< TestFunction< float > > fractionalAnisotropyTestFunction,
        FractionalAnisotropySortingType fractionalAnisotropySortingType,
        RCPointer< TestFunction< float > > tissueClassTestFunction,
        int32_t wantedVoxelCount,
        bool verbose );

    TensorType _tensorType;
    int32_t _candidateVoxelCount;
    Volume< uint8_t > _candidateVoxelVolume;
    int32_t _selectedVoxelCount;
    Volume< uint8_t > _selectedVoxelVolume;
    Tensor _tensor;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for DiffusionKernel
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::DiffusionKernel, gkg_DiffusionKernel );


#endif

