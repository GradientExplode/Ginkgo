#ifndef _gkg_simulation_virtual_tissue_VirtualTissue_h_
#define _gkg_simulation_virtual_tissue_VirtualTissue_h_



#include <gkg-core-object/HeaderedObject.h>
#include <gkg-simulation-virtual-tissue/PopulationFactory.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <fstream>


namespace gkg
{


class VirtualTissue : public HeaderedObject
{

  public:


    typedef std::vector< RCPointer< Population > >::const_iterator
                                                                 const_iterator;
    typedef std::vector< RCPointer< Population > >::iterator iterator;

    VirtualTissue();
    VirtualTissue( const Dictionary& geometryDictionary,
                   const RCPointer< Dictionary >& 
                                             populationTissuePropertyDictionary,
                   bool verbose = false,
                   std::ofstream* osLog = 0 );
    VirtualTissue( const VirtualTissue& other );
    virtual ~VirtualTissue();

    VirtualTissue& operator=( const VirtualTissue& other );

    const_iterator begin() const;
    const_iterator end() const;

    iterator begin();
    iterator end();

    const BoundingBox< float >& getFieldOfView() const;
    float getGridResolution() const;

    int32_t getPopulationCount() const;
    int32_t getCellCount() const;
    int32_t getAtomCount() const;
    uint64_t getAtomParameterCount() const;

    void addPopulation( const RCPointer< Population >& population );
    const std::vector< RCPointer< Population > >& getPopulations() const;
    const RCPointer< Population >& getPopulation( int32_t populationId ) const;

    const std::string& getPopulationLabelFromId( int32_t populationId ) const;
    int32_t getPopulationIdFromLabel(
                                     const std::string& populationLabel ) const;

    void setPopulationTissueProperties(
            const RCPointer< Dictionary >& populationTissuePropertyDictionary );
    const std::vector< std::vector< std::map< std::string, float > > >&
      getTissueProperties() const;
    float getTissueProperty( int32_t populationId,
                             int32_t cellId,
                             const std::string& propertyLabel,
                             float defaultValue ) const;
    void getPopulationTissuePropertyLabels( 
               std::list< std::string >& populationTissuePropertyLabels ) const;
    void getTissuePropertyOutputPopulationAndCellBasedCSVFile( 
           const std::string&
              fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile ) const;


    RCPointer< Volume< RCPointer< std::list< Vector3d< float > > > > >
      getPopulationAtomCenterLut( 
                         const std::set< std::string >& populationTypes ) const;
    RCPointer< Volume< RCPointer< std::list< int32_t > > > >
      getPopulationAtomIndexLut( int32_t& maximumSphereAtomPerLutVoxel ) const;

    void getMeshMaps( 
                  std::vector< MeshMap< int32_t, float, 3U > >& meshMap ) const;
                  
    const std::list< float >& getTotalRepulsionForceNorms() const;

    void removeOverlaps( float maximumForceNorm,
                         int32_t atomSolverMaximumIterationCount,
                         int32_t repulsionForceMovingAverageWindowSize,
                         float minimumRepulsionForceAttenuationPercentage,
                         float repulsionForceStddevPercentageThreshold,
                         bool verbose,
                         std::ofstream* osLog );

  protected:

    friend class MedusaGeometryDiskFormat;

    void regularizeWithFurtherAtoms();
    void addDetails();

    void addPopulations(
                   const BoundingBox< float >& fieldOfView,
                   const std::vector< RCPointer< Population > >& populations,
                   const std::vector< std::string >& populationTypes,
                   const std::vector< std::string >& populationLabels );

    BoundingBox< float > _fieldOfView;
    float _gridResolution;
 
    std::vector< RCPointer< Population > > _populations;
    std::vector< std::vector< std::map< std::string, float > > >
                                        _populationAndCellBasedTissueProperties;

    std::vector< std::string > _populationTypes;
    std::vector< std::string > _populationLabels;
    std::map< std::string, int32_t > _populationLabelToIds;
    
    std::list< float > _totalRepulsionForceNorms;

};


}


#endif
