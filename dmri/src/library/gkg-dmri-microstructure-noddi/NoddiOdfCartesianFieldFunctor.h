#ifndef _gkg_dmri_microstructure_noddi_NoddiOdfCartesianFieldFunctor_h_
#define _gkg_dmri_microstructure_noddi_NoddiOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-noddi/NoddiOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class NoddiOdfCartesianFieldFunctor : 
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    NoddiOdfCartesianFieldFunctor(
                         const NoddiOdfCartesianField& noddiOdfCartesianField );
    
    virtual ~NoddiOdfCartesianFieldFunctor();

    OrientationDistributionFunction&
      apply( const Vector3d< int32_t >& site,
             OrientationDistributionFunction& odf );

    const Volume< float >& getNoddiCoefficients() const;

    const Volume< float >& getIsotropicFraction() const;
    const Volume< float >& getIntracellularFraction() const;
    const Volume< float >& getKappa() const;
    const Volume< float >& getOrientationDispersion() const;
    const Volume< float >& getParallelDiffusivity() const;
    const Volume< float >& getIsotropicDiffusivity() const;
    const Volume< float >& getStationaryFraction() const;

  protected:

    NoddiOdfCartesianField::SphereDistributionFunction
                                                    _sphereDistributionFunction;

    Volume< float > _noddiCoefficients;
    Volume< float > _isotropicFraction;
    Volume< float > _intracellularFraction;
    Volume< float > _kappa;
    Volume< float > _orientationDispersion;
    Volume< float > _parallelDiffusivity;
    Volume< float > _isotropicDiffusivity;
    Volume< float > _stationaryFraction;

};


}


#endif
