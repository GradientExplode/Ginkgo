#ifndef _gkg_core_io_BaseObjectReader_h_
#define _gkg_core_io_BaseObjectReader_h_


#include <gkg-core-object/BaseObject.h>
#include <gkg-core-object/TypedObject.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/Syntax.h>
#include <iostream>


namespace gkg
{


class BaseObjectReader
{

  public:

    typedef BaseObject* ( *Helper )( std::istream&,
                                     BaseObject*,
                                     const BaseObjectReader&,
                                     const std::string& );
    typedef std::map< std::string, Helper > HelperSet;

    BaseObjectReader( const SyntaxSet& rules = SyntaxSet(),
                      const HelperSet& helpers = HelperSet() );
    virtual ~BaseObjectReader();
    

    virtual void read( std::istream& is,
                       BaseObject& baseObject ) const;

    virtual BaseObject* read( std::istream& is ) const;

    virtual BaseObject* read( std::istream& is,
                              BaseObject* baseObject,
                              const std::string & semantic = "" ) const;

    bool readSyntax( std::istream& is,
                     std::string& syntax ) const;
  
  protected:

    std::string readVariableName( std::istream& is ) const;

    mutable SyntaxSet _rules;    // mutable because read could modify it!
    HelperSet _helpers;
    bool _writeInternals;
    bool _singleLine;

};


}


#endif
