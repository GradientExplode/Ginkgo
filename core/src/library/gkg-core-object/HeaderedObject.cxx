#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/TypedObject_i.h>


gkg::HeaderedObject::HeaderedObject()
                    : gkg::GenericObject(),
                      gkg::Observer()
{

  _header.addObserver( this );

}


gkg::HeaderedObject::HeaderedObject( const gkg::HeaderedObject& other )
                    : gkg::GenericObject( /*other*/ ),
                      gkg::Observer(),
                      _header( other._header ),
                      _binaryHeader( other._binaryHeader ),
                      _syntaxSet( other._syntaxSet ),
                      _readerHelpers( other._readerHelpers ),
                      _writerHelpers( other._writerHelpers )

{


  _header.addObserver( this );

}


gkg::HeaderedObject::~HeaderedObject()
{

  _header.removeObserver( this );

}


gkg::HeaderedObject& 
gkg::HeaderedObject::operator = ( const gkg::HeaderedObject& other )
{

  this->gkg::GenericObject::operator = ( this );
  _header = other._header;
  _binaryHeader = other._binaryHeader;
  return *this;

}


gkg::HeaderedObject::Header& gkg::HeaderedObject::getHeader()
{

  return _header;

}


const gkg::HeaderedObject::Header& gkg::HeaderedObject::getHeader() const
{

  return _header;

}


gkg::BinaryHeader& gkg::HeaderedObject::getBinaryHeader()
{

  return _binaryHeader;

}


const gkg::BinaryHeader& gkg::HeaderedObject::getBinaryHeader() const
{

  return _binaryHeader;

}


void gkg::HeaderedObject::addSemantic( const std::string& syntax,
                                       const std::string& attribute,
                                       const gkg::Semantic& semantic )
{

  _syntaxSet[ syntax ][ attribute ] = semantic;

}


const gkg::SyntaxSet& gkg::HeaderedObject::getSyntaxSet() const
{

  return _syntaxSet;

}


const gkg::BaseObjectReader::HelperSet&
gkg::HeaderedObject::getReaderHelpers() const
{

  return _readerHelpers;

}


const gkg::BaseObjectWriter::HelperSet& 
gkg::HeaderedObject::getWriterHelpers() const
{

  return _writerHelpers;

}


void gkg::HeaderedObject::update( const gkg::Observable*, void* )
{
}

