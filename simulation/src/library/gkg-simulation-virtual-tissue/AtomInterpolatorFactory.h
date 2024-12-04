#ifndef _gkg_simulation_virtual_tissue_AtomInterpolatorFactory_h_
#define _gkg_simulation_virtual_tissue_AtomInterpolatorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-simulation-virtual-tissue/Atom.h>
#include <vector>


namespace gkg
{


class AtomInterpolatorFactory : public Singleton< AtomInterpolatorFactory >
{

  public:

    typedef void ( *AtomInterpolator )( const Atom&,
                                        const Atom&,
                                        float,
                                        std::list< Atom* >& );

    ~AtomInterpolatorFactory();

    bool registerAtomInterpolator( uint8_t atomType1,
                                   uint8_t atomType2,
                                   AtomInterpolator atomInterpolator );

    AtomInterpolator getAtomInterpolator( uint8_t atomType1,
                                          uint8_t atomType2 ) const;

  protected:

    friend class Singleton< AtomInterpolatorFactory >;

    AtomInterpolatorFactory();

    std::vector< std::vector< AtomInterpolator > > _atomInterpolators; 


};


}


#endif
