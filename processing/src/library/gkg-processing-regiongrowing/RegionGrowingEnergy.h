#ifndef _gkg_processing_regiongrowing_RegionGrowingEnergy_h_
#define _gkg_processing_regiongrowing_RegionGrowingEnergy_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-regiongrowing/RegionGrowingPotential.h>
#include <map>
#include <list>
#include <string>
#include <iostream>


namespace gkg
{


template < class L >
class RegionGrowingEnergy
{

  public:

    RegionGrowingEnergy();
    virtual ~RegionGrowingEnergy();

    void addPotential( const std::string& contrastName,
                       const std::string& potentialName,
                       RCPointer< RegionGrowingPotential< L > > potential,
                       double weight );

    std::list< std::string > getContrastNames() const;
    std::list< std::string > getPotentialNames(
                                        const std::string& contrastName ) const;

    RCPointer< RegionGrowingPotential< L > >
      getPotential( const std::string& contrastName,
                    const std::string& potentialName ) const;
    double getWeight( const std::string& contrastName,
                      const std::string& potentialName ) const;

    // looking at the global energy
    double getValue() const;
    double getDeltaValue() const;

    // looking at the global energy for a given label
    double getValueForLabel( const L& label ) const;
    double getDeltaValueForLabel( const L& label ) const;

    // looking at the global energy for a given potential
    double getValueForPotential( const std::string& name ) const; 
    double getDeltaValueForPotential( const std::string& name ) const;


    // looking at a given contrast
    double getWeightedValueForContrast(
                                       const std::string& contrastName ) const;
    double getWeightedDeltaValueForContrast(
                                       const std::string& contrastName ) const;

    // looking at a given potential for a given contrast
    void initializeForContrastAndPotential(
                                       const std::string& contrastName,
                                       const std::string& potentialName );

    double getWeightedValueForContrastAndPotential(
                                       const std::string& contrastName,
                                       const std::string& potentialName ) const;
    double getUnweightedValueForContrastAndPotential(
                                       const std::string& contrastName,
                                       const std::string& potentialName ) const;
    double getWeightedDeltaValueForContrastAndPotential(
                                       const std::string& contrastName,
                                       const std::string& potentialName ) const;
    double getUnweightedDeltaValueForContrastAndPotential(
                                       const std::string& contrastName,
                                       const std::string& potentialName ) const;

    // looking at a given potential for a given label and for a given contrast
    void initializeForContrastAndPotentialAndLabel(
                                       const std::string& contrastName,
                                       const std::string& potentialName,
                                       const L& label );

    double getWeightedValueForContrastAndPotentialAndLabel(
                                       const std::string& contrastName,
                                       const std::string& potentialName,
                                       const L& label ) const;
    double getUnweightedValueForContrastAndPotentialAndLabel(
                                       const std::string& contrastName,
                                       const std::string& potentialName,
                                       const L& label ) const;
    double getWeightedDeltaValueForContrastAndPotentialAndLabel(
                                       const std::string& contrastName,
                                       const std::string& potentialName,
                                       const L& label ) const;
    double getUnweightedDeltaValueForContrastAndPotentialAndLabel(
                                       const std::string& contrastName,
                                       const std::string& potentialName,
                                       const L& label ) const;

    void displayCurrentEnergy( int32_t iteration ) const;
    void writeMatPlotLibProlog( std::ostream& os ) const;
    void writeMatPlotLibEpilog( std::ostream& os ) const;
    void writeMatPlotLibCurrentEnergy( int32_t iteration,
                                       std::ostream& valueOs,
                                       std::ostream& deltaValueOs ) const;


  protected:

    std::map< std::string,
              std::map< std::string,
                        RCPointer< RegionGrowingPotential< L > > > >
       _potentials;
    std::map< std::string, std::map< std::string, double > > _weights;

};


}


#endif
