#ifndef _gkg_simulation_virtual_tissue_AtomForceApplierFactory_h_
#define _gkg_simulation_virtual_tissue_AtomForceApplierFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-simulation-virtual-tissue/Atom.h>
#include <vector>


namespace gkg
{


class AtomForceApplierFactory : public Singleton< AtomForceApplierFactory >
{

  public:

    typedef void ( *AtomForceApplier )( const float&,
                                        const float&,
                                        const float&,
                                        float*,
                                        bool );

    ~AtomForceApplierFactory();

    bool registerAtomForceApplier( uint8_t atomType,
                                   AtomForceApplier atomForceApplier );

    AtomForceApplier getAtomForceApplier( uint8_t atomType ) const;

  protected:

    friend class Singleton< AtomForceApplierFactory >;

    AtomForceApplierFactory();

    std::vector< AtomForceApplier > _atomForceAppliers; 


};


}


#endif
