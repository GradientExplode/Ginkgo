#ifndef _gkg_anatomist_plugin_objects_AnatomistVolume_h_
#define _gkg_anatomist_plugin_objects_AnatomistVolume_h_


#include <gkg-core-pattern/RCPointer.h>
#include <anatomist/volume/Volume.h>


namespace gkg
{


template < class T > class Volume;


template < class T, class U = T >
class AnatomistVolume : public anatomist::AVolume< U >
{

  public:

    AnatomistVolume( RCPointer< Volume< T > > volume );

    virtual int CanBeDestroyed();

};


}


#endif
