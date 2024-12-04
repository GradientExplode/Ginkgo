#ifndef _gkg_simulation_virtual_tissue_Cell_h_
#define _gkg_simulation_virtual_tissue_Cell_h_


#include <gkg-simulation-virtual-tissue/Atom.h>
#include <gkg-processing-mesh/MeshAccumulator.h>
#include <vector>
#include <set>


namespace gkg
{


class Population;


class Cell
{

  public:

    typedef std::vector< Atom* >::const_iterator const_iterator;
    typedef std::vector< Atom* >::iterator iterator;

    virtual ~Cell();

    Cell( const Cell& other );

    Cell& operator=( const Cell& other );


    const_iterator begin() const;
    const_iterator end() const;

    iterator begin();
    iterator end();

    void setId( int32_t id );
    int32_t getId() const;


    void setParent( const Population* population );
    const Population* getParent() const;
    int32_t getAtomCount() const;
    uint64_t getAtomParameterCount() const;
    const std::vector< Atom* >& getAtoms() const;
    const Atom* getAtom( int32_t index ) const;
    void setAtom( int32_t index, Atom* other );
    void setAtoms( const std::vector< Atom* >& atoms );

    int32_t getNotBelongingToBranchAtomCount() const;
    const std::set< int32_t >&
      getNotBelongingToBranchAtomIndices() const;
    void fillAtomBranchLabels( uint32_t* atomBranchLabels,
                               uint32_t& lastLabel ) const;

    virtual void fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                       meshAccumulators ) const;

    virtual void addInteractionWithOtherPopulations();
    virtual void regularizeWithFurtherAtoms();
    virtual void addDetails();


  protected:

    friend class MedusaGeometryDiskFormat;

    Cell();
    Cell( int32_t id, const Population* parent );

    int32_t _id;
    const Population* _parent;
    std::vector< Atom* > _atoms;

    std::set< int32_t > _notBelongingToBranchAtomIndices;
    std::vector< std::pair< int32_t, int32_t > > _branches;

};



}



#endif
