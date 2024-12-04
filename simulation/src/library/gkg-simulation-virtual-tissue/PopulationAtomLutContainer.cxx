#include <gkg-simulation-virtual-tissue/PopulationAtomLutContainer.h>
#include <gkg-core-exception/Exception.h>



gkg::PopulationAtomLutContainer::PopulationAtomLutContainer()
{
}


gkg::PopulationAtomLutContainer::~PopulationAtomLutContainer()
{
}



gkg::RCPointer< gkg::Volume< gkg::RCPointer<
                                      std::list< gkg::Vector3d< float > > > > >
gkg::PopulationAtomLutContainer::getPopulationAtomCenterLut( 
                           const gkg::VirtualTissue& virtualTissue,
                           const std::set< std::string >& populationTypes )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // building key from population type set
    ////////////////////////////////////////////////////////////////////////////

    std::string key = "|";
    std::set< std::string >::const_iterator
      pt = populationTypes.begin(),
      pte = populationTypes.end();
    while ( pt != pte )
    {

      key += *pt + '|';
      ++ pt;

    }


    ////////////////////////////////////////////////////////////////////////////
    // if the lut is already stored, return it
    ////////////////////////////////////////////////////////////////////////////

    std::map< std::string, 
              gkg::RCPointer< 
                gkg::Volume< 
                  gkg::RCPointer<
                    std::list< gkg::Vector3d< float > > > > > >::const_iterator
      l = _populationAtomCenterLuts.find( key );
    if ( l != _populationAtomCenterLuts.end() )
    {

      return l->second;

    }


    ////////////////////////////////////////////////////////////////////////////
    // else creating it
    ////////////////////////////////////////////////////////////////////////////

    _populationAtomCenterLuts[ key ] =
                                 virtualTissue.getPopulationAtomCenterLut(
                                                              populationTypes );

    return _populationAtomCenterLuts[ key ];


  }
  GKG_CATCH( "gkg::RCPointer< gkg::Volume< std::list< "
             "gkg::Vector3d< float > > > > "
             "gkg::PopulationAtomLutContainer::"
             "getPopulationAtomCenterLut( "
             "const gkg::VirtualTissue& virtualTissue, "
             "const std::set< std::string >& populationTypes )" );

}
