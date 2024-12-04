#ifndef _gkg_core_io_DicomSelector_h_
#define _gkg_core_io_DicomSelector_h_


#include <string>
#include <map>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-io/DicomImplementationFactory.h>


namespace gkg
{


class DicomSelector : public Singleton< DicomSelector >
{

  public:

    typedef DicomImplementationFactory& ( *Accessor )();

    ~DicomSelector();

    void select( const std::string& name );
    DicomImplementationFactory* getImplementationFactory();

    void registerFactory( const std::string& name, Accessor accessor );

  protected:

    friend class Singleton< DicomSelector >;

    DicomSelector();

    void selectDefaultFactory();

    std::map< std::string, Accessor > _accessors;
    DicomImplementationFactory* _implementationFactory;
    bool _ready;

};





}


#endif
