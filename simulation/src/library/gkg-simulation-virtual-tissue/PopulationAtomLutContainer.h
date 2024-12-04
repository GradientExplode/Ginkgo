#ifndef _gkg_simulation_virtual_tissue_PopulationAtomLutContainer_h_
#define _gkg_simulation_virtual_tissue_PopulationAtomLutContainer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>


namespace gkg
{


class PopulationAtomLutContainer : 
                                  public Singleton< PopulationAtomLutContainer >
{

  public:

    ~PopulationAtomLutContainer();

    RCPointer< Volume< RCPointer< std::list< Vector3d< float > > > > >
      getPopulationAtomCenterLut( 
                               const VirtualTissue& virtualTissue,
                               const std::set< std::string >& populationTypes );

  protected:

    friend class Singleton< PopulationAtomLutContainer >;

    PopulationAtomLutContainer();


    std::map< std::string,
              RCPointer<
                Volume<
                  RCPointer<
                    std::list< Vector3d< float > > > > > >
                                                      _populationAtomCenterLuts;

};


}


#endif

