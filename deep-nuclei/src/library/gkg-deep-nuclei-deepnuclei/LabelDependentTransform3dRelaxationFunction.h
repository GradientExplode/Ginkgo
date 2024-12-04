#ifndef _gkg_deep_nuclei_deepnuclei_LabelDependentTransform3dRelaxationFunction_h_
#define _gkg_deep_nuclei_deepnuclei_LabelDependentTransform3dRelaxationFunction_h_


#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-regiongrowing/RegionGrowingEnergy.h>
#include <gkg-processing-transform/ParameterizedTransform3d.h>
#include <string>


namespace gkg
{


template < class L >
class LabelDependentTransform3dRelaxationFunction :
                                           public gkg::NelderMeadSimplexFunction
{

  public:

    LabelDependentTransform3dRelaxationFunction(
        RCPointer< ParameterizedTransform3d< float > > parameterizedTransform3d,
        RCPointer< RegionGrowingEnergy< L > > regionGrowingEnergy,
        const std::string& contrastName,
        const L& label );
    ~LabelDependentTransform3dRelaxationFunction();

    double getValueAt( const Vector& parameters ) const;

  private:

    RCPointer< ParameterizedTransform3d< float > > _parameterizedTransform3d;
    RCPointer< RegionGrowingEnergy< L > > _regionGrowingEnergy;
    std::string _contrastName;
    L _label;

};


}


#endif
