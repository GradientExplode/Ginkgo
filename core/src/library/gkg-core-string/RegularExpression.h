#ifndef _gkg_core_string_RegularExpression_h_
#define _gkg_core_string_RegularExpression_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


// These definitions are from Henry Spencers public-domain regular expression
// matching routines.
// Definitions etc. for regexp(3) routines.
// Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
// not the System V one.
#define NSUBEXP  10

struct regexp
{

  char *startp[ NSUBEXP ];
  char *endp[ NSUBEXP ];
  char *textStart;
  char regstart;        // internal use only
  char reganch;	        // internal use only
  char *regmust;        // internal use only
  int32_t regmlen;      // internal use only
  char program[ 1 ];    // unwarranted chumminess with compiler

};


// the first byte of the regexp internal "program" is actually this magic
// number; the start node begins in the second byte.
#define	REGEXP_MAGIC    0234


// regular expression searching
class RegularExpression
{

  public:

    RegularExpression( const std::string& pattern );
    ~RegularExpression();

    const std::string& getPattern() const;
    int32_t search( const std::string& text, int32_t index, int32_t range );
    int32_t match( const std::string& text, int32_t index );
    int32_t getBeginningOfMatch( int32_t subExp = 0 );
    int32_t getEndOfMatch( int32_t subExp = 0 );

  private:

    std::string _pattern;
    regexp* _cPattern;

};


}


#endif
