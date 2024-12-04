#ifndef _gkg_processing_regiongrowing_RegionGrowingPotential_h_
#define _gkg_processing_regiongrowing_RegionGrowingPotential_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-numericalanalysis/PhiFunction.h>
#include <gkg-processing-container/RoiMeasure.h>
#include <map>
#include <list>
#include <utility>


namespace gkg
{


template < class L >
class RegionGrowingPotential
{

  public:

    RegionGrowingPotential();
    virtual ~RegionGrowingPotential();

    void addRoiMeasure( RCPointer< RoiMeasure< L > > roiMeasure,
                        RCPointer< PhiFunction > phiFunction,
                        double weight = 1.0 );

    void initialize();
    void initialize( const L& label );

    std::list< L > getLabels() const;
    bool hasMeasureForLabel( const L& label ) const;
    RCPointer< RoiMeasure< L > > getRoiMeasure( const L& label ) const;
    RCPointer< PhiFunction > getPhiFunction( const L& label ) const;
    double getWeight( const L& label ) const;

    double getValue() const;
    double getDeltaValue() const;

    double getWeightedValueForLabel( const L& label ) const;
    double getUnweightedValueForLabel( const L& label ) const;

    double getWeightedDeltaValueForLabel( const L& label ) const;
    double getUnweightedDeltaValueForLabel( const L& label ) const;

  protected:

    std::map< L, RCPointer< RoiMeasure< L > > > _measures;
    std::map< L, RCPointer< PhiFunction > > _phiFunctions;
    std::map< L, double > _weights;

};


}


#endif
