#ifndef _gkg_processing_regiongrowing_RegionGrowingAlgorithm_h_
#define _gkg_processing_regiongrowing_RegionGrowingAlgorithm_h_


#include <gkg-processing-regiongrowing/RegionGrowingEnergy.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/BoundingBox.h>


namespace gkg
{


template < class L >
class RegionGrowingAlgorithm
{

  public:

    virtual ~RegionGrowingAlgorithm();

    gkg::RCPointer< Volume< L > > getLabelVolume() const;
    gkg::RCPointer< RegionGrowingEnergy< L > > getEnergy();

    virtual void getRandomSite( Vector3d< int32_t >& site,
                                L& label ) const = 0;
    virtual std::set< L > getNewPutativeLabels( const Vector3d< int32_t >& site,
                                                const L& label ) const = 0;

    virtual double getDeltaEnergy( const Vector3d< int32_t >& site,
                                   const L& oldLabel,
                                   const L& newLabel ) = 0;
    virtual void applyModification( const Vector3d< int32_t >& site,
                                    const L& oldLabel,
                                    const L& newLabel ) = 0;

  protected:

    RegionGrowingAlgorithm();
    RegionGrowingAlgorithm( RCPointer< Volume< L > > labelVolume );
    virtual void initializeLabelVolume() = 0;
    virtual void initializeEnergy() = 0;

    RCPointer< Volume< L > > _labelVolume;
    BoundingBox< int32_t > _labelVolumeBoundingBox;
    RCPointer< RegionGrowingEnergy< L > > _energy;

};


}


#endif
