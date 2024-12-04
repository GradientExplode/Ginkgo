#ifndef _gkg_core_object_HeaderedObject_h_
#define _gkg_core_object_HeaderedObject_h_


#include <gkg-core-object/GenericObject.h>
#include <gkg-core-object/TypedObject.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/BinaryHeader.h>
#include <gkg-core-pattern/Observer.h>
#include <gkg-core-io/BaseObjectReader.h>
#include <gkg-core-io/BaseObjectWriter.h>


namespace gkg
{


class HeaderedObject : public GenericObject, public Observer
{

  public:

    typedef TypedObject< Dictionary > Header;

    HeaderedObject();
    HeaderedObject( const HeaderedObject& other );
    virtual ~HeaderedObject();

    HeaderedObject& operator = ( const HeaderedObject& other );

    Header& getHeader();
    const Header& getHeader() const;

    BinaryHeader& getBinaryHeader();
    const BinaryHeader& getBinaryHeader() const;

    virtual void addSemantic( const std::string& syntax,
                              const std::string& attribute,
                              const Semantic& semantic );
    virtual const SyntaxSet& getSyntaxSet() const;
    virtual const BaseObjectReader::HelperSet& getReaderHelpers() const;
    virtual const BaseObjectWriter::HelperSet& getWriterHelpers() const;

    virtual void update( const Observable* observable = 0, void* arg = 0 );

  protected:

    Header _header;
    BinaryHeader _binaryHeader;
    SyntaxSet _syntaxSet;
    BaseObjectReader::HelperSet _readerHelpers;
    BaseObjectWriter::HelperSet _writerHelpers;

};


}


#endif
