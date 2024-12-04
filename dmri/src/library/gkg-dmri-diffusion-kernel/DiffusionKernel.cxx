#include <gkg-dmri-diffusion-kernel/DiffusionKernel.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <numeric>
#include <algorithm>


gkg::DiffusionKernel::DiffusionKernel(
               gkg::RCPointer< gkg::Volume< float > > t2,
               gkg::RCPointer< gkg::Volume< float > > dw,
               gkg::RCPointer< gkg::Volume< int16_t > > mask,
               gkg::RCPointer< gkg::Volume< float > > tissueClassProbabilityMap,
               gkg::RCPointer< gkg::Transform3d< float > >
                                         transform3dFromTissueClassToDiffusion,
               gkg::RCPointer< gkg::TestFunction< float > >
                                               fractionalAnisotropyTestFunction,
               gkg::DiffusionKernel::FractionalAnisotropySortingType
                                                fractionalAnisotropySortingType,
               gkg::RCPointer< gkg::TestFunction< float > >
                                                        tissueClassTestFunction,
               int32_t wantedVoxelCount,
               gkg::DiffusionKernel::TensorType tensorType,
               bool verbose )
                     : _tensorType( tensorType ),
                       _candidateVoxelCount( 0 ),
                       _selectedVoxelCount( 0 )
{

  try
  {

    if ( _tensorType == gkg::DiffusionKernel::SymmetricTensor )
    {

      estimateAverageSymmetricTensor( t2,
                                      dw,
                                      mask,
                                      tissueClassProbabilityMap,
                                      transform3dFromTissueClassToDiffusion,
                                      fractionalAnisotropyTestFunction,
                                      fractionalAnisotropySortingType,
                                      tissueClassTestFunction,
                                      wantedVoxelCount,
                                      verbose );

    }

  }
  GKG_CATCH( "gkg::DiffusionKernel::DiffusionKernel( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "gkg::RCPointer< gkg::Volume< float > > "
             "tissueClassProbabilityMap, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "transform3dFromTissueClassToDiffusion, "
             "gkg::RCPointer< gkg::TestFunction< float > > "
             "fractionalAnisotropyTestFunction, "
             "gkg::DiffusionKernel::FractionalAnisotropySortingType "
             "fractionalAnisotropySortingType, "
             "gkg::RCPointer< gkg::TestFunction< float > > "
             "tissueClassTestFunction, "
             "int32_t wantedVoxelCount, "
             "gkg::DiffusionKernel::TensorType tensorType, "
             "bool verbose )" );

}


gkg::DiffusionKernel::DiffusionKernel(
               gkg::RCPointer< gkg::Volume< float > > t2,
               gkg::RCPointer< gkg::Volume< float > > dw,
               gkg::RCPointer< gkg::Volume< int16_t > > mask,
               gkg::RCPointer< gkg::TestFunction< float > >
                                               fractionalAnisotropyTestFunction,
               gkg::DiffusionKernel::FractionalAnisotropySortingType
                                                fractionalAnisotropySortingType,
               int32_t wantedVoxelCount,
               gkg::DiffusionKernel::TensorType tensorType,
               bool verbose )
                     : _tensorType( tensorType ),
                       _candidateVoxelCount( 0 ),
                       _selectedVoxelCount( 0 )
{

  try
  {

    if ( _tensorType == gkg::DiffusionKernel::SymmetricTensor )
    {

      // setting empty tissue classes
      gkg::RCPointer< gkg::Volume< float > > tissueClassProbabilityMap;
      gkg::RCPointer< gkg::TestFunction< float > > tissueClassTestFunction;

      gkg::RCPointer< gkg::Transform3d< float > > 
                                          transform3dFromTissueClassToDiffusion;

      estimateAverageSymmetricTensor( t2,
                                      dw,
                                      mask,
                                      tissueClassProbabilityMap,
                                      transform3dFromTissueClassToDiffusion,
                                      fractionalAnisotropyTestFunction,
                                      fractionalAnisotropySortingType,
                                      tissueClassTestFunction,
                                      wantedVoxelCount,
                                      verbose );

    }

  }
  GKG_CATCH( "gkg::DiffusionKernel::DiffusionKernel( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "gkg::RCPointer< gkg::TestFunction< float > > "
             "fractionalAnisotropyTestFunction, "
             "gkg::DiffusionKernel::FractionalAnisotropySortingType "
             "fractionalAnisotropySortingType, "
             "int32_t wantedVoxelCount, "
             "gkg::DiffusionKernel::TensorType tensorType, "
             "bool verbose )" );

}


gkg::DiffusionKernel::DiffusionKernel( float eigenValue,
                                       float mainToSecondaryEigenValueRatio )
                     : _tensorType( gkg::DiffusionKernel::SymmetricTensor ),
                       _candidateVoxelCount( 0 ),
                       _selectedVoxelCount( 0 )
{

  try
  {

    gkg::Vector coefficients( 6 );
    coefficients( 0 ) = eigenValue;  //1.44e-09 is a good value
    coefficients( 1 ) = 0.0;
    coefficients( 2 ) = 0.0;
    coefficients( 3 ) = eigenValue * mainToSecondaryEigenValueRatio;
    coefficients( 4 ) = 0.0;
    coefficients( 5 ) = eigenValue * mainToSecondaryEigenValueRatio;
    _tensor = gkg::Tensor( 0, coefficients );

  }
  GKG_CATCH( "gkg::DiffusionKernel::DiffusionKernel( "
             "float eigenValue, "
             "float mainToSecondaryEigenValueRatio )" );

}


gkg::DiffusionKernel::DiffusionKernel( const gkg::DiffusionKernel& other )
                     : _tensorType( other._tensorType ),
                       _candidateVoxelCount( other._candidateVoxelCount ),
                       _candidateVoxelVolume( other._candidateVoxelVolume ),
                       _selectedVoxelCount( other._selectedVoxelCount ),
                       _selectedVoxelVolume( other._selectedVoxelVolume ),
                       _tensor( other._tensor )
{
}


gkg::DiffusionKernel::~DiffusionKernel()
{
}


gkg::DiffusionKernel& 
gkg::DiffusionKernel::operator=( const gkg::DiffusionKernel& other )
{

  try
  {

    _tensorType = other._tensorType;
    _candidateVoxelCount = other._candidateVoxelCount;
    _candidateVoxelVolume = other._candidateVoxelVolume;
    _selectedVoxelCount = other._selectedVoxelCount;
    _selectedVoxelVolume = other._selectedVoxelVolume;
    _tensor = other._tensor;

    return *this;

  }
  GKG_CATCH( "gkg::DiffusionKernel& "
             "gkg::DiffusionKernel::operator=( "
             "const gkg::DiffusionKernel& other )" );

}


gkg::DiffusionKernel::TensorType
gkg::DiffusionKernel::getTensorType() const
{

  try
  {

    return _tensorType;

  }
  GKG_CATCH( "gkg::DiffusionKernel::TensorType "
             "gkg::DiffusionKernel::getTensorType() const" );

}


std::string gkg::DiffusionKernel::getTensorTypeName() const
{

  try
  {

    std::string tensorTypeName = "";

    if ( _tensorType == gkg::DiffusionKernel::SymmetricTensor )
    {

      tensorTypeName = "symmetric-tensor";

    }

    return tensorTypeName;

  }
  GKG_CATCH( "std::string gkg::DiffusionKernel::getTensorTypeName() const" );

}


int32_t gkg::DiffusionKernel::getCandidateVoxelCount() const
{

  try
  {

    return _candidateVoxelCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::DiffusionKernel::getCandidateVoxelCount() const" );

}


const gkg::Volume< uint8_t >& 
gkg::DiffusionKernel::getCandidateVoxelVolume() const
{

  try
  {

    return _candidateVoxelVolume;

  }
  GKG_CATCH( "const gkg::Volume< uint8_t >& "
             "gkg::DiffusionKernel::getCandidateVoxelVolume() const" );

}


int32_t gkg::DiffusionKernel::getSelectedVoxelCount() const
{

  try
  {

    return _selectedVoxelCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::DiffusionKernel::getSelectedVoxelCount() const" );

}


const gkg::Volume< uint8_t >& 
gkg::DiffusionKernel::getSelectedVoxelVolume() const
{

  try
  {

    return _selectedVoxelVolume;

  }
  GKG_CATCH( "const gkg::Volume< uint8_t >& "
             "gkg::DiffusionKernel::getSelectedVoxelVolume() const" );

}


const gkg::Tensor& gkg::DiffusionKernel::getTensor() const
{

  try
  {

    return _tensor;

  }
  GKG_CATCH( "const gkg::Tensor& "
             "gkg::DiffusionKernel::getTensor() const" );

}


gkg::Matrix
gkg::DiffusionKernel::getSphericalHarmonicsConvolutionMatrix(
              const gkg::OrientationSet& orientationSet,
              float bValueInSecondsPerMillimeterSquare,
              int32_t maximumSphericalHarmonicOrder ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // get access to the numerical analysis factory
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();


    ////////////////////////////////////////////////////////////////////////////
    // computing SH coefficient count
    ////////////////////////////////////////////////////////////////////////////

    int32_t shCoefficientCount = ( maximumSphericalHarmonicOrder + 1 ) *
                                 ( maximumSphericalHarmonicOrder + 2 ) / 2;


    ////////////////////////////////////////////////////////////////////////////
    //  computing kernel signal attenuation profile for the corresponding tensor
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix rotationMatrix( 3, 3 );
    rotationMatrix.setIdentity();
    gkg::Vector kernelSignalProfile = _tensor.getRotatedDiffusionWeightedSignal(
                                       orientationSet,
                                       rotationMatrix,
                                       bValueInSecondsPerMillimeterSquare * 1e6,
                                       1.0 ); // S0 = 1.0


    ////////////////////////////////////////////////////////////////////////////
    // computing Cartesian to spherical harmonics basis transformation matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix cartesianToSHBasisMatrix;
    orientationSet.getCartesianToSHBasisTransform(
                                                  maximumSphericalHarmonicOrder,
                                                  cartesianToSHBasisMatrix );


    ////////////////////////////////////////////////////////////////////////////
    // decomposing the tensor signal attenuation onto the SH basis
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector kernelSignalProfileSHDecomposition =
                 cartesianToSHBasisMatrix.getComposition( kernelSignalProfile );


    ////////////////////////////////////////////////////////////////////////////
    // computing convolution matrix in the spherical harmonics basis
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector diracFunction( shCoefficientCount );
    gkg::Vector nonZeroKernelSignalProfileSHDecomposition( 
                                      maximumSphericalHarmonicOrder / 2 + 1 );
    gkg::Vector nonZeroDiracFunction( maximumSphericalHarmonicOrder / 2 + 1 );

    int32_t i = 0;
    int32_t l = 0;
    int32_t m = 0;
    int32_t n = 0;
    for ( n = 0; n < shCoefficientCount; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      diracFunction( n ) = factory->getDiracCoefficient( n, 0, 0 );

      if ( m == 0 )
      {

        nonZeroKernelSignalProfileSHDecomposition( i ) = 
                                        kernelSignalProfileSHDecomposition( n );
        nonZeroDiracFunction( i ) = diracFunction( n );
        i++;

      }

    }

    gkg::Matrix sphericalHarmonicsConvolutionMatrix( shCoefficientCount,
                                                     shCoefficientCount );
    sphericalHarmonicsConvolutionMatrix.setZero();
    for ( n = 0; n < shCoefficientCount; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );

      // R(n n) = syntheticSignal(l, m=0) / Dirac(l,m)
      sphericalHarmonicsConvolutionMatrix( n, n ) =
        nonZeroKernelSignalProfileSHDecomposition( l / 2 ) /
        nonZeroDiracFunction( l / 2 );

    }

    return sphericalHarmonicsConvolutionMatrix;

  }
  GKG_CATCH( "gkg::Matrix "
             "gkg::DiffusionKernel::getSphericalHarmonicsConvolutionMatrix( "
             "const gkg::OrientationSet& orientationSet, "
             "float bValueInSecondsPerMillimeterSquare, "
             "int32_t maximumSphericalHarmonicOrder ) const" );

}


gkg::Matrix
gkg::DiffusionKernel::getSphericalHarmonicsDeconvolutionMatrix(
              const gkg::OrientationSet& orientationSet,
              float bValueInSecondsPerMillimeterSquare,
              int32_t maximumSphericalHarmonicOrder,
              bool filtered ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // get access to the numerical analysis factory
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();


    ////////////////////////////////////////////////////////////////////////////
    // computing SH coefficient count
    ////////////////////////////////////////////////////////////////////////////

    int32_t shCoefficientCount = ( maximumSphericalHarmonicOrder + 1 ) *
                                 ( maximumSphericalHarmonicOrder + 2 ) / 2;


    ////////////////////////////////////////////////////////////////////////////
    // computing the convolution matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix sphericalHarmonicsConvolutionMatrix;
    this->getSphericalHarmonicsConvolutionMatrix(
                                             orientationSet,
                                             bValueInSecondsPerMillimeterSquare,
                                             maximumSphericalHarmonicOrder );

    ////////////////////////////////////////////////////////////////////////////
    // computing the deconvolution matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix sphericalHarmonicsDeconvolutionMatrix( shCoefficientCount,
                                                       shCoefficientCount );
    sphericalHarmonicsDeconvolutionMatrix.setZero();

    std::vector< double > filteringCoefficients;
    if ( filtered )
    {

      // computing filter coefficients up to SH order 15
      filteringCoefficients.resize( 16 );
      filteringCoefficients[  0 ] = 1.0;       // l =  0
      filteringCoefficients[  1 ] = 1.0;       // l =  2
      filteringCoefficients[  2 ] = 1.0;       // l =  4
      filteringCoefficients[  3 ] = 0.5;       // l =  6
      filteringCoefficients[  4 ] = 0.1;       // l =  8
      filteringCoefficients[  5 ] = 0.02;      // l = 10
      filteringCoefficients[  6 ] = 0.002;     // l = 12
      filteringCoefficients[  7 ] = 0.0005;    // l = 14
      filteringCoefficients[  8 ] = 0.0001;    // l = 16
      filteringCoefficients[  9 ] = 0.0001;    // l = 18
      filteringCoefficients[ 10 ] = 0.00001;   // l = 20
      filteringCoefficients[ 11 ] = 0.00001;   // l = 22
      filteringCoefficients[ 12 ] = 0.00001;   // l = 24
      filteringCoefficients[ 13 ] = 0.00001;   // l = 26
      filteringCoefficients[ 14 ] = 0.00001;   // l = 28
      filteringCoefficients[ 15 ] = 0.00001;   // l = 30

      if ( maximumSphericalHarmonicOrder > 15 )
      {

        throw std::runtime_error(
                "filtering available only up to spherical harmonics order 16" );

      }

    }

    int32_t n = 0;
    int32_t l = 0;
    int32_t m = 0;
    for ( n = 0; n < shCoefficientCount; n++ )
    {

      if ( sphericalHarmonicsConvolutionMatrix( n, n ) != 0 )
      {

        if ( !filtered )
        {

          sphericalHarmonicsDeconvolutionMatrix( n, n ) =
                                    1.0 /
                                    sphericalHarmonicsConvolutionMatrix( n, n );

        }
        else
        {

          factory->getSymmetricalSphericalHarmonicLM( n, l, m );
          sphericalHarmonicsDeconvolutionMatrix( n, n ) =
                                    filteringCoefficients[ l / 2 ] /
                                    sphericalHarmonicsConvolutionMatrix( n, n );

        }

      }

    }

    return sphericalHarmonicsDeconvolutionMatrix;

  }
  GKG_CATCH( "gkg::Matrix "
             "gkg::DiffusionKernel::getSphericalHarmonicsDeconvolutionMatrix( "
             "const gkg::OrientationSet& orientationSet, "
             "float bValueInSecondsPerMillimeterSquare, "
             "int32_t maximumSphericalHarmonicOrder, "
             "bool filtered ) const" );


}


void gkg::DiffusionKernel::estimateAverageSymmetricTensor(
               gkg::RCPointer< gkg::Volume< float > > t2,
               gkg::RCPointer< gkg::Volume< float > > dw,
               gkg::RCPointer< gkg::Volume< int16_t > > mask,
               gkg::RCPointer< gkg::Volume< float > > tissueClassProbabilityMap,
               gkg::RCPointer< gkg::Transform3d< float > >
                                         transform3dFromTissueClassToDiffusion,
               gkg::RCPointer< gkg::TestFunction< float > >
                                               fractionalAnisotropyTestFunction,
               gkg::DiffusionKernel::FractionalAnisotropySortingType
                                                fractionalAnisotropySortingType,
               gkg::RCPointer< gkg::TestFunction< float > >
                                                        tissueClassTestFunction,
               int32_t wantedVoxelCount,
               bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > volumeSize;
    int32_t volumeSizeT = 0;
    t2->getSize( volumeSize, volumeSizeT );

    gkg::Vector3d< double > volumeResolution( 1.0, 1.0, 1.0 );
    t2->getResolution( volumeResolution );

    if ( volumeSizeT != 1 )
    {

      throw std::runtime_error( "T2 volume should be a 3D volume, not a 4D" );

    }

    if ( !dw->hasSameSize( *t2, true ) ||
         !dw->hasSameResolution( *t2, true ) )
    {

      throw std::runtime_error( "inconsistent DW and T2 volume(s)" );

    }

    if ( !mask->hasSameSize( *t2, true ) ||
         !mask->hasSameResolution( *t2, true ) )
    {

      throw std::runtime_error( "inconsistent mask and T2 volume(s)" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reallocating volume(s)
    ////////////////////////////////////////////////////////////////////////////

    _candidateVoxelVolume.reallocate( volumeSize );
    _candidateVoxelVolume.setResolution( volumeResolution );
    _candidateVoxelVolume.fill( 0U );

    _selectedVoxelVolume.reallocate( volumeSize );
    _selectedVoxelVolume.setResolution( volumeResolution );
    _selectedVoxelVolume.fill( 0U );


    ////////////////////////////////////////////////////////////////////////////
    // computing the DTI Cartesian field from T2 and DW dataset
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {
      std::cout << "computing tensor cartesian field : " << std::flush;

    }

    std::vector< double > scalarParameters;
    std::vector< std::string > stringParameters( 1U );
    stringParameters[ 0 ] = "robust_positive_definite";
    gkg::RCPointer< gkg::TensorCartesianField >
      tensorCartesianField( new gkg::TensorCartesianField( t2,
                                                           dw,
                                                           mask,
                                                           scalarParameters,
                                                           stringParameters,
                                                           verbose ) );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // parsing all voxels
    ////////////////////////////////////////////////////////////////////////////

    // getting pointer to quick partial voluming resampler
    gkg::PartialVolumingQuickResampler< float >& resampler =
      gkg::PartialVolumingQuickResampler< float >::getInstance();

    gkg::Vector3d< double > tissueClassVolumeResolution( 1.0, 1.0, 1.0 );
    if ( !tissueClassProbabilityMap.isNull() )
    {

      tissueClassProbabilityMap->getResolution( tissueClassVolumeResolution );

    }

    if ( verbose )
    {
      std::cout << "looping over voxels : " << std::flush;

    }

    gkg::Vector3d< int32_t > voxel;
    gkg::Vector3d< float > siteInDiffusionFrame;
    gkg::Vector3d< float > siteInTissueClassFrame;
    float tissueClassProbability = 0.0f;
    float currentFractionalAnisotropy = 0.0f;
    float currentAdc = 0.0f;
    std::list< gkg::Vector3d< int32_t > > voxelList;
    std::list< float > fractionalAnisotropyList;
    for ( voxel.z = 0; voxel.z < volumeSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < volumeSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < volumeSize.x; voxel.x++ )
        {

          if ( ( *mask )( voxel ) )
          {

            const gkg::Tensor*
              currentTensor = tensorCartesianField->getItem( voxel );
            currentFractionalAnisotropy =
                                       currentTensor->getFractionalAnisotropy();
            currentAdc = currentTensor->getApparentDiffusionCoefficient();
            if ( fractionalAnisotropyTestFunction->getState(
                                                 currentFractionalAnisotropy ) )
            {

              siteInDiffusionFrame.x = voxel.x * volumeResolution.x;
              siteInDiffusionFrame.y = voxel.y * volumeResolution.y;
              siteInDiffusionFrame.z = voxel.z * volumeResolution.z;

              if ( !tissueClassProbabilityMap.isNull() )
              {

                if ( !transform3dFromTissueClassToDiffusion.isNull() )
                {

                  transform3dFromTissueClassToDiffusion->getInverse( 
                                                       siteInDiffusionFrame,
                                                       siteInTissueClassFrame );


                }
                else
                {

                  siteInTissueClassFrame = siteInDiffusionFrame;

                }
                resampler.resample( *tissueClassProbabilityMap,
                                    0.0f,
                                    siteInTissueClassFrame,
                                    tissueClassProbability,
                                    &tissueClassVolumeResolution,
                                    0 );
                // ( currentFractionalAnisotropy == 0.0f ) &&
                // ( currentAdc == 1e-6f ) correspond to cases where the
                // tensor failed to fit, so we must discard this values
                if ( tissueClassTestFunction->getState(
                                                     tissueClassProbability ) &&
                     !( ( currentFractionalAnisotropy == 0.0f ) &&
                        ( currentAdc == 1e-6f ) )  )
                {

                  ++ _candidateVoxelCount;
                  _candidateVoxelVolume( voxel ) = 1U;
                  voxelList.push_back( voxel );
                  fractionalAnisotropyList.push_back(
                                                  currentFractionalAnisotropy );

                }

              }
              else if ( !( ( currentFractionalAnisotropy == 0.0f ) &&
                        ( currentAdc == 1e-6f ) ) )
              {

                ++ _candidateVoxelCount;
                _candidateVoxelVolume( voxel ) = 1U;
                voxelList.push_back( voxel );
                fractionalAnisotropyList.push_back(
                                                currentFractionalAnisotropy );

              }

            }

          }

        }

      }

    }


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    if ( verbose )
    {
      std::cout << "candidate voxel count : " << voxelList.size() << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reordering voxels in the FA decreasing order
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
      std::cout << "sorting FA values : " << std::flush;

    }

    std::vector< float >
      fractionalAnisotropies( fractionalAnisotropyList.begin(),
                              fractionalAnisotropyList.end() );
    std::vector< gkg::Vector3d< int32_t > >
      voxels( voxelList.begin(), voxelList.end() );

    std::vector< size_t > sortIndices( fractionalAnisotropies.size() );
    std::iota( sortIndices.begin(), sortIndices.end(), 0 );

    if ( fractionalAnisotropySortingType ==
         gkg::DiffusionKernel::DecreasingFractionalAnisotropyOrder )
    {

      std::stable_sort( sortIndices.begin(), sortIndices.end(),
                        [ &fractionalAnisotropies ]( size_t i1, size_t i2 )
                        { return fractionalAnisotropies[ i1 ] >
                                 fractionalAnisotropies[ i2 ]; } );

    }
    else
    {

      std::stable_sort( sortIndices.begin(), sortIndices.end(),
                        [ &fractionalAnisotropies ]( size_t i1, size_t i2 )
                        { return fractionalAnisotropies[ i1 ] <
                                 fractionalAnisotropies[ i2 ]; } );

    }
 
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // selecting the wanted number of voxels
    ////////////////////////////////////////////////////////////////////////////

    _selectedVoxelCount = std::min( wantedVoxelCount,
                                    ( int32_t )voxels.size() );

    if ( verbose )
    {
      std::cout << "selected voxel count : " << _selectedVoxelCount << std::endl;

    }


    if ( _selectedVoxelCount )
    {

      float averageMaximumEigenValue = 0.0f;
      float averageMiddleEigenValue = 0.0f;
      float averageMinimumEigenValue = 0.0f;
      float averageFractionalAnisotropy = 0.0f;

      int32_t s = 0;
      for ( s = 0; s < _selectedVoxelCount; s++ )
      {

        const gkg::Vector3d< int32_t >& voxel = voxels[ sortIndices[ s ] ];
        const float& fractionalAnisotropy = 
                                     fractionalAnisotropies[ sortIndices[ s ] ];
        const gkg::Tensor*
          selectedTensor = tensorCartesianField->getItem( voxel );


        averageMaximumEigenValue += selectedTensor->getEigenValue(
                                                         gkg::Tensor::Maximum );
        averageMiddleEigenValue += selectedTensor->getEigenValue(
                                                         gkg::Tensor::Middle );
        averageMinimumEigenValue += selectedTensor->getEigenValue(
                                                         gkg::Tensor::Minimum );
        averageFractionalAnisotropy += fractionalAnisotropy;

        _selectedVoxelVolume( voxel ) = 1U;

      }


      averageMaximumEigenValue /= ( float )_selectedVoxelCount;
      averageMiddleEigenValue /= ( float )_selectedVoxelCount;
      averageMinimumEigenValue /= ( float )_selectedVoxelCount;
      averageFractionalAnisotropy /= ( float )_selectedVoxelCount;

      gkg::Vector coefficients( 6 );
      coefficients( 0 ) = ( double )averageMaximumEigenValue;
      coefficients( 1 ) = 0.0;
      coefficients( 2 ) = 0.0;
      coefficients( 3 ) = ( double )( averageMiddleEigenValue +
                                      averageMinimumEigenValue ) / 2.0;
      coefficients( 4 ) = 0.0;
      coefficients( 5 ) = ( double )( averageMiddleEigenValue +
                                      averageMinimumEigenValue ) / 2.0;
      _tensor = gkg::Tensor( 0, coefficients );

      if ( verbose )
      {

        std::cout << "average FA for top " << _selectedVoxelCount
                  << " voxel(s) : "
                  << averageFractionalAnisotropy << std::endl;
        std::cout << "diffusion tensor kernel : lambda_max="
                  << averageMaximumEigenValue << "m^2/s, lambda_mid="
                  << averageMiddleEigenValue << "m^2/s, lambda_min="
                  << averageMinimumEigenValue << "m^2/s"
                  << std::endl;

      }

    }
    else
    {

      throw std::runtime_error( "diffusion kernel is null" );

    }

  }
  GKG_CATCH( "void gkg::DiffusionKernel::estimateAverageSymmetricTensor( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "gkg::RCPointer< gkg::Volume< float > > "
             "tissueClassProbabilityMap, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "transform3dFromTissueClassToDiffusion, "
             "gkg::RCPointer< gkg::TestFunction< float > > "
             "fractionalAnisotropyTestFunction, "
             "gkg::DiffusionKernel::FractionalAnisotropySortingType "
             "fractionalAnisotropySortingType, "
             "gkg::RCPointer< gkg::TestFunction< float > > "
             "tissueClassTestFunction, "
             "int32_t wantedVoxelCount, "
             "bool verbose )" );

}


