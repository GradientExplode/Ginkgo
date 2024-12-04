#ifndef _gkg_simulation_virtual_tissue_Population_h_
#define _gkg_simulation_virtual_tissue_Population_h_


#include <gkg-simulation-virtual-tissue/Cell.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/DictionaryHelpers.h>


namespace gkg
{


class VirtualTissue;


class Population
{

  public:

    typedef std::vector< RCPointer< Cell > >::const_iterator const_iterator;
    typedef std::vector< RCPointer< Cell > >::iterator iterator;

    virtual ~Population();

    Population( const Population& other );

    Population& operator=( const Population& other );


    const_iterator begin() const;
    const_iterator end() const;

    iterator begin();
    iterator end();

    int32_t getId() const;
    virtual std::string getTypeName() const; 
    void setParent( const VirtualTissue* parent );
    const VirtualTissue* getParent() const;
    float getBranchAtomOversamplingRatio() const;
    void setBranchAtomOversamplingRatio( float branchAtomOversamplingRatio );
    int32_t getCellCount() const;
    const std::vector< RCPointer< Cell > >& getCells() const;
    const RCPointer< Cell >& getCell( int32_t index ) const;
    void setCell( int32_t index, const RCPointer< Cell >& other );

    int32_t getAtomCount() const;
    uint64_t getAtomParameterCount() const;

    void fillMeshAccumulators( 
               std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                       meshAccumulators ) const;

    virtual void addInteractionWithOtherPopulations();
    virtual void regularizeWithFurtherAtoms();
    void addDetails();

  protected:

    friend class MedusaGeometryDiskFormat;

    Population( int32_t id, const VirtualTissue* parent );
    Population( int32_t id,
                const std::string& typeName, 
                const VirtualTissue* parent,
                const std::vector< RCPointer< Cell > >& cells );

    int32_t _id;
    std::string _typeName;
    const VirtualTissue* _parent;
    float _branchAtomOversamplingRatio;
    std::vector< RCPointer< Cell > > _cells;


};


}


#endif
