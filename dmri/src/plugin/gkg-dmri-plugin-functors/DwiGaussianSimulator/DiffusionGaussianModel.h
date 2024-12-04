#ifndef _gkg_dmri_plugin_functors_DwiGaussianSimulator_DiffusionGaussianModel_h_
#define _gkg_dmri_plugin_functors_DwiGaussianSimulator_DiffusionGaussianModel_h_


#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class DiffusionGaussianModel
{

  public:

    virtual ~DiffusionGaussianModel();

    virtual float getSignalContribution( const Vector& u ) const = 0;
    virtual float getOdfContribution( const Vector& u ) const = 0;

  protected:

    DiffusionGaussianModel();

};


}


#endif
