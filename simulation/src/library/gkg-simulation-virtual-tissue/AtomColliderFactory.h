#ifndef _gkg_simulation_virtual_tissue_AtomColliderFactory_h_
#define _gkg_simulation_virtual_tissue_AtomColliderFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-simulation-virtual-tissue/Atom.h>
#include <vector>


namespace gkg
{


class AtomColliderFactory : public Singleton< AtomColliderFactory >
{

  public:

    typedef bool ( *AtomCollider )( const float*,
                                    bool,
                                    const float*,
                                    bool,
                                    float&,
                                    float&,
                                    float& );

    ~AtomColliderFactory();

    bool registerAtomCollider( uint8_t atomType1,
                               uint8_t atomType2,
                               AtomCollider atomCollider );

    AtomCollider getAtomCollider( uint8_t atomType1,
                                  uint8_t atomType2 ) const;

  protected:

    friend class Singleton< AtomColliderFactory >;

    AtomColliderFactory();

    std::vector< std::vector< AtomCollider > > _atomColliders; 


};


}


#endif
