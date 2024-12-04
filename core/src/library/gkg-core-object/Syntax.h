#ifndef _gkg_core_object_Syntax_h_
#define _gkg_core_object_Syntax_h_


#include <string>
#include <map>


namespace gkg
{


struct Semantic
{

  Semantic();
  Semantic( const std::string& theType,
            bool theNeeded = false,
            bool theInternal = false );

  std::string type;    // type of semantic (for instance 'float')
  bool needed;         // is this semantic attribute mandatory ?
  bool internal;       // internal attributes are not written to disk

};


typedef std::map< std::string, Semantic > Syntax;
typedef std::map< std::string, Syntax > SyntaxSet;


}


#endif
