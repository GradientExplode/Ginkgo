#ifndef _gkg_core_io_BaseObjectWriter_h_
#define _gkg_core_io_BaseObjectWriter_h_


#include <gkg-core-object/BaseObject.h>
#include <gkg-core-object/Syntax.h>
#include <iostream>


namespace gkg
{


class BaseObjectWriter
{

  public:

    typedef void ( *Helper )( std::ostream&,
                              const BaseObject&,
                              const BaseObjectWriter&,
                              int32_t indentationLevel );
    typedef std::map< std::string, Helper > HelperSet;

    BaseObjectWriter( const SyntaxSet& rules = SyntaxSet(),
                      const HelperSet& helpers = HelperSet(),
                      bool writeInternals = false,
                      bool singleLine = false );
    virtual ~BaseObjectWriter();
    
    bool getWriteInternals() const;
    bool getSingleLine() const;
    bool isInternal( const std::string& syntax,
                     const std::string& semantic ) const;

    virtual void write( std::ostream& os,
                        const BaseObject& baseObject ) const;

    virtual void write( std::ostream& os,
                        const BaseObject& baseObject,
                        int32_t indentationLevel,
                        const std::string & syntax = "", 
                        const std::string & semantic = "" ) const;
  
  protected:

    SyntaxSet _rules;
    HelperSet _helpers;
    bool _writeInternals;
    bool _singleLine;

};


}


#endif
