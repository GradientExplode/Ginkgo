#ifndef _gkg_dmri_microstructure_ivim_IvimOdfCartesianFieldFunctor_h_
#define _gkg_dmri_microstructure_ivim_IvimOdfCartesianFieldFunctor_h_


#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-ivim/IvimOdfCartesianField.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class IvimOdfCartesianFieldFunctor : 
        public DiffusionCartesianFieldFunctor< OrientationDistributionFunction >
{

  public:


    IvimOdfCartesianFieldFunctor(
                         const IvimOdfCartesianField& ivimOdfCartesianField );
    
    virtual ~IvimOdfCartesianFieldFunctor();

    OrientationDistributionFunction&
      apply( const Vector3d< int32_t >& site,
             OrientationDistributionFunction& odf );

    const Volume< float >& getIvimCoefficients() const;

    const Volume< float >& getIvimFraction() const;
    const Volume< float >& getIvimDiffusivity() const;
    const Volume< float >& getParenchymaDiffusivity() const;
    const Volume< float >& getParenchymaFractionalAnisotropy() const;

    const Volume< float >& getIvimFittedVolume() const;
    const Volume< float >& getIvimPowderAverage() const;

  protected:

    Volume< float > _ivimCoefficients;
    Volume< float > _ivimFraction;
    Volume< float > _ivimDiffusivity;
    Volume< float > _parenchymaDiffusivity;
    Volume< float > _parenchymaFractionalAnisotropy;

    Volume< float > _ivimFittedVolume;
    Volume< float > _ivimPowderAverage;

};


}


#endif
