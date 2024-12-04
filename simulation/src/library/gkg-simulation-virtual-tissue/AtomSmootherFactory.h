#ifndef _gkg_simulation_virtual_tissue_AtomSmootherFactory_h_
#define _gkg_simulation_virtual_tissue_AtomSmootherFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-simulation-virtual-tissue/Atom.h>
#include <vector>


namespace gkg
{


class AtomSmootherFactory : public Singleton< AtomSmootherFactory >
{

  public:

    typedef void ( *AtomSmoother )( const float*,
                                    bool,
                                    float*,
                                    bool,
                                    const float*,
                                    bool );

    ~AtomSmootherFactory();

    bool registerAtomSmoother( uint8_t atomType1,
                               uint8_t atomType2,
                               uint8_t atomType3,
                               AtomSmoother atomSmoother );

    AtomSmoother getAtomSmoother( uint8_t atomType1,
                                  uint8_t atomType2,
                                  uint8_t atomType3 ) const;

  protected:

    friend class Singleton< AtomSmootherFactory >;

    AtomSmootherFactory();

    std::vector< std::vector< std::vector< AtomSmoother > > > _atomSmoothers; 


};


}


#endif
