#ifndef _gkg_dmri_microstructure_ivim_IvimOdf_h_
#define _gkg_dmri_microstructure_ivim_IvimOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/TypeOf.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;


class IvimOdf : public OrientationDistributionFunction
{

  public:

    //
    // storing the parameters in the _values  field with this order:
    //  _values[ 0 ] -> IVIM fraction
    //  _values[ 1 ] -> IVIM diffusivity
    //  _values[ 2 ] -> parenchyma diffusivity
    //  _values[ 3 ] -> Dxx
    //  _values[ 4 ] -> Dxy
    //  _values[ 5 ] -> Dxz
    //  _values[ 6 ] -> Dyy
    //  _values[ 7 ] -> Dyz
    //  _values[ 8 ] -> Dzz
    //


    // for standard estimation on the acquired grid
    IvimOdf( const OrientationSet* orientationSet,
             const Volume< float >& t2,
             const Volume< float >& dw,
             const Vector3d< int32_t >& site,
             const Tensor& parenchymaTensor,
             int32_t shellCount,
             const std::vector< float >& shellBValues,
             const std::vector< int32_t >& shellIndexOffsets,
             const std::vector< int32_t >& shellOrientationCounts,
             int32_t ivimToParenchymaTransitionShellIndex,
             int32_t parenchymaGaussianToNonGaussianTransitionShellIndex,
             const Vector& initialParameters,
             double noiseVariance,
             const std::vector< RCPointer< OptimizerConstraint > >& 
                                                           optimizerConstraints,
             const std::vector< double >& nlpOptimizerParameters );

    // for partial voluming resampled data
    IvimOdf( const OrientationSet* orientationSet,
             const Volume< float >& t2,
             const Volume< float >& dw,
             const Vector3d< float >& site,
             const Tensor& parenchymaTensor,
             int32_t shellCount,
             const std::vector< float >& shellBValues,
             const std::vector< int32_t >& shellIndexOffsets,
             const std::vector< int32_t >& shellOrientationCounts,
             int32_t ivimToParenchymaTransitionShellIndex,
             int32_t parenchymaGaussianToNonGaussianTransitionShellIndex,
             const Vector& initialParameters,
             double noiseVariance,
             const std::vector< RCPointer< OptimizerConstraint > >& 
                                                           optimizerConstraints,
             const std::vector< double >& nlpOptimizerParameters );

    virtual ~IvimOdf();

    const float& getIvimFraction() const;
    const float& getIvimDiffusivity() const;
    const float& getParenchymaDiffusivity() const;
    Tensor getParenchymaTensor() const;
    Vector getFittedMeasurements() const;
    std::vector< float > getPowderAverage() const;
    
  protected:

    Vector _fittedMeasurements;
    std::vector< float > _powderAverage;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for IvimOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::IvimOdf, gkg_IvimOdf );


#endif
