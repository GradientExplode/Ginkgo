#ifndef _gkg_dmri_microstructure_axon_mapping_CTOGSEAxonMappingOdfCartesianFieldFunctor_h_
#define _gkg_dmri_microstructure_axon_mapping_CTOGSEAxonMappingOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axon-mapping/AxonMappingOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class CTOGSEAxonMappingOdfCartesianFieldFunctor :
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    CTOGSEAxonMappingOdfCartesianFieldFunctor(
             const AxonMappingOdfCartesianField& axonMappingOdfCartesianField );
    
    virtual ~CTOGSEAxonMappingOdfCartesianFieldFunctor();

    OrientationDistributionFunction&
      apply( const Vector3d< int32_t >& site,
             OrientationDistributionFunction& odf );

    const Volume< float >& getAxonMappingCoefficients() const;

    const Volume< float >& getIsotropicFraction() const;
    const Volume< float >& getIsotropicDiffusivity() const;
    const Volume< float >& getExtraPerpendicularDiffusivity() const;
    const Volume< float >& getStationaryFraction() const;
    const Volume< float >& getIntracellularFraction() const;
    const Volume< float >& getKappa() const;
    const Volume< float >& getOrientationDispersion() const;
    const Volume< float >& getParallelDiffusivity() const;
    const Volume< float >& getAxonDiameter() const;

  protected:

    Volume< float > _axonMappingCoefficients;
    Volume< float > _isotropicFraction;
    Volume< float > _isotropicDiffusivity;
    Volume< float > _extraPerpendicularDiffusivity;
    Volume< float > _stationaryFraction;
    Volume< float > _intracellularFraction;
    Volume< float > _kappa;
    Volume< float > _orientationDispersion;
    Volume< float > _parallelDiffusivity;
    Volume< float > _axonDiameter;

};


}


#endif
