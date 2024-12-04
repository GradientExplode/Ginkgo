#ifndef _gkg_dmri_microstructure_axCaliber_AxCaliberOdfCartesianFieldFunctor_h_
#define _gkg_dmri_microstructure_axCaliber_AxCaliberOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class AxCaliberOdfCartesianFieldFunctor : 
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    AxCaliberOdfCartesianFieldFunctor(
                 const AxCaliberOdfCartesianField& axCaliberOdfCartesianField );
    
    virtual ~AxCaliberOdfCartesianFieldFunctor();

    OrientationDistributionFunction&
      apply( const Vector3d< int32_t >& site,
             OrientationDistributionFunction& odf );

    const Volume< float >& getAxCaliberCoefficients() const;

    const Volume< float >& getHinderedDiffusivity() const;
    const Volume< float >& getIntracellularFraction() const;
    const Volume< float >& getIntracellularPerpendicularDiffusivity() const;
    const Volume< float >& getIntracellularParallelDiffusivity() const;
    const Volume< float >& getDiameterMean() const;
    const Volume< float >& getDiameterStdDev() const;

  protected:

    Volume< float > _axCaliberCoefficients;
    Volume< float > _hinderedDiffusivity;
    Volume< float > _intracellularFraction;
    Volume< float > _intracellularPerpendicularDiffusivity;
    Volume< float > _intracellularParallelDiffusivity;
    Volume< float > _diameterMean;
    Volume< float > _diameterStdDev;

};


}


#endif
