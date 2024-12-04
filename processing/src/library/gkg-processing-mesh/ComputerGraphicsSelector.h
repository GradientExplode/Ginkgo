#ifndef _gkg_processing_mesh_ComputerGraphicsSelector_h_
#define _gkg_processing_mesh_ComputerGraphicsSelector_h_


#include <gkg-processing-mesh/ComputerGraphicsImplementationFactory.h>
#include <gkg-core-pattern/Singleton.h>

#include <string>
#include <map>


namespace gkg
{


class ComputerGraphicsSelector : public Singleton< ComputerGraphicsSelector >
{

  public:

    typedef ComputerGraphicsImplementationFactory& ( *Accessor )();

    virtual ~ComputerGraphicsSelector();

    void select( const std::string& name );
    ComputerGraphicsImplementationFactory* getImplementationFactory();

    void registerFactory( const std::string& name, Accessor accessor );

  protected:

    friend class Singleton< ComputerGraphicsSelector >;

    ComputerGraphicsSelector();

    void selectDefaultFactory();

    std::map< std::string, Accessor > _accessors;
    ComputerGraphicsImplementationFactory* _implementationFactory;
    bool _ready;

};


}


#endif
