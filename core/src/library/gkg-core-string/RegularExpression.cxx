#include <gkg-core-string/RegularExpression.h>
#include <iostream>
#include <cstring>


// this version is based on the Henry Spencers public domain reimplementation
// of the regular expression matching subroutines.  They are included as
// static subroutines after the externally accessible routines.

//
// forward declarations for regcomp()'s friends.
//

namespace gkg
{

static char* findNewline( char* s );
static char* nextLine( char* s );

static regexp* regcomp( char* exp );
static char* reg( int32_t paren, int32_t* flagp );
static char* regbranch( int32_t* flagp );
static char* regpiece( int32_t* flagp );
static char* regatom( int32_t* flagp );
static char* regnode( char op );
static char* regnext( char* p );
static void regc( char b );
static void reginsert( char op, char* opnd );
static void regtail( char* p, char* val );
static void regoptail( char* p, char* val );
static void regerror( const char* s );
static int32_t regexec( regexp* prog, char* theString );
static int32_t regtry( regexp* prog, char* theString );
static int32_t regmatch( char* prog );
static int32_t regrepeat( char* p );

}

//
// inline functions
//

static char* gkg::findNewline( char* s )
{

  return std::strchr( s, '\n' );

}


static char* gkg::nextLine( char* s )
{

  char* newStart;

  if ( ( newStart = gkg::findNewline( s ) ) != 0 )
  {

    ++ newStart;

  }
  return newStart;

}



//
// class RegularExpression
//

gkg::RegularExpression::RegularExpression ( const std::string& pattern )
                       : _pattern( pattern ),
                         _cPattern( 0 )
{
}


gkg::RegularExpression::~RegularExpression ()
{

  if ( _cPattern )
  {

    delete _cPattern;

  }

}


const std::string& gkg::RegularExpression::getPattern() const
{

  return _pattern;

}


int gkg::RegularExpression::search( const std::string& theText,
                                    int32_t index, int32_t range )
{

  bool forwardSearch;
  bool frontAnchored;
  bool endAnchored;
  char* searchStart;
  char* searchLimit;
  char* endOfLine = 0;
  char* lastMatch = 0;
  char csave = 0;

  const char* text = theText.c_str();
  int32_t length = ( int32_t )theText.length();

  // a small sanity check.  Otherwise length is unused in this function; this is
  // really what the logic embedded in the old version of this routine enforced
  if ( index + range > length )
  {

    range = length - index;
    if ( range < 0 )
    {

      return -1;

    }

  }

  if ( _cPattern )
  {

    delete _cPattern;

  }

  if ( ( _cPattern = gkg::regcomp( ( char* )_pattern.c_str() ) ) == 0 )
  {

    return -1;

  }

  _cPattern->startp[ 0 ] = 0;

  if ( range < 0 )
  {

    forwardSearch = false;
    searchLimit = ( char* )text + index;
    searchStart = ( char* )searchLimit + range; // range is negative

  }
  else
  {

    forwardSearch = true;
    searchStart = ( char* )text + index;
    searchLimit = ( char* )searchStart + range;

  }

  // mark end of text string so search will stop
  char save = *searchLimit;
  *searchLimit = '\0';

  frontAnchored = _pattern[ 0 ] == '^';
  endAnchored = _pattern[ _pattern.length() - 1 ] == '$';
  if ( frontAnchored &&
       ( searchStart != text || searchStart[ -1 ] == '\n' ) )
  {

    searchStart = gkg::nextLine( searchStart );

  }

  while ( searchStart && ( searchStart < searchLimit ) )
  {

    int32_t result;

    if ( endAnchored && 
         ( endOfLine = gkg::findNewline( searchStart ) ) != 0 )
    {

      csave = *endOfLine;
      *endOfLine = '\0';

    }

    result = gkg::regexec( _cPattern, searchStart );

    if ( endOfLine )
    {

      *endOfLine = csave;

    }

    if ( result )
    {

      // found a match
      if ( forwardSearch )
      {

        break;  // done

      }
      else
      {

        lastMatch = _cPattern->startp[ 0 ];
        searchStart = _cPattern->endp[ 0 ];
        if ( frontAnchored )
        {

          searchStart = gkg::nextLine( searchStart );

        }
        continue;

      }

    }
    // did not find a match
    if ( frontAnchored || endAnchored )
    {

      searchStart = gkg::nextLine( searchStart );

    }
    else
    {

      break;

    }

  }

  if ( !forwardSearch && lastMatch )
  {

    if ( endAnchored && ( endOfLine = gkg::findNewline( lastMatch ) ) != 0 )
    {

      csave = *endOfLine;
      *endOfLine = '\0';

    }
    gkg::regexec( _cPattern, lastMatch ); // refill startp and endp
    if ( endOfLine )
    {

      *endOfLine = csave;

    }

  }

  *searchLimit = save;
  _cPattern->textStart = ( char* )text;

  return _cPattern->startp[ 0 ] - _cPattern->textStart;

}


int gkg::RegularExpression::match( const std::string& theText, int32_t index )
{

  const char* text = theText.c_str();
  int32_t length = ( int32_t )theText.length();

  if ( _cPattern )
  {

    delete _cPattern;

  }

  if ( ( _cPattern = gkg::regcomp( ( char* )_pattern.c_str() ) ) == 0 )
  {

    return -1;

  }

  _cPattern->startp[ 0 ] = 0;

  char save = *( text + length );
  *( char* )( text + length ) = '\0';

  _cPattern->textStart = ( char* )text;
  gkg::regexec( _cPattern, ( char* )text + index );

  *( char* )( text + length ) = save;

  if ( _cPattern->startp[ 0 ] )
  {

    return _cPattern->endp[ 0 ] - _cPattern->startp[ 0 ];

  }
  else
  {

    return -1;

  }

}


int32_t gkg::RegularExpression::getBeginningOfMatch( int subExp )
{

  if ( subExp < 0 || subExp > NSUBEXP || !_cPattern || !_cPattern->startp[ 0 ] )
  {

    return -1;

  }
  return _cPattern->startp[ subExp ] - _cPattern->textStart;

}


int32_t gkg::RegularExpression::getEndOfMatch( int32_t subExp )
{

  if ( subExp < 0 || subExp > NSUBEXP || !_cPattern || !_cPattern->startp[ 0 ] )
  {

    return -1;

  }
  return _cPattern->endp[ subExp ] - _cPattern->textStart;

}


//
// regcomp and regexec
//
//  Copyright (c) 1986 by University of Toronto.
//  Written by Henry Spencer.  Not derived from licensed software.
//
//  Permission is granted to anyone to use this software for any
//  purpose on any computer system, and to redistribute it freely,
//  subject to the following restrictions:
//
//  1. The author is not responsible for the consequences of use of
//     this software, no matter how awful, even if they arise
//     from defects in it.
//
//  2. The origin of this software must not be misrepresented, either
//     by explicit claim or by omission.
//
//  3. Altered versions must be plainly marked as such, and must not
//     be misrepresented as being the original software.
//
// Beware that some of this code is subtly aware of the way operator
// precedence is structured in regular expressions.  Serious changes in
// regular-expression syntax might require a total rethink.
//

//
// The "internal use only" fields in regexp.h are present to pass info from
// compile to execute that permits the execute phase to run lots faster on
// simple cases.  They are:
//
// regstart     char that must begin a match; '\0' if none obvious
// reganch      is the match anchored (at beginning-of-line only)?
// regmust      string (pointer into program) that match must include, or nil
// regmlen      length of regmust string
//
// Regstart and reganch permit very fast decisions on suitable starting points
// for a match, cutting down the work a lot.  Regmust permits fast rejection
// of lines that cannot possibly match.  The regmust tests are costly enough
// that regcomp() supplies a regmust only if the r.e. contains something
// potentially expensive (at present, the only such thing detected is * or +
// at the start of the r.e., which can involve a lot of backup).  Regmlen is
// supplied because the test in regexec() needs it and regcomp() is computing
// it anyway.
//

//
// Structure for regexp "program".  This is essentially a linear encoding
// of a nondeterministic finite-state machine (aka syntax charts or
// "railroad normal form" in parsing technology).  Each node is an opcode
// plus a "next" pointer, possibly plus an operand.  "Next" pointers of
// all nodes except BRANCH implement concatenation; a "next" pointer with
// a BRANCH on both ends of it is connecting two alternatives.  (Here we
// have one of the subtle syntax dependencies:  an individual BRANCH (as
// opposed to a collection of them) is never concatenated with anything
// because of operator precedence.)  The operand of some types of node is
// a literal string; for others, it is a node leading into a sub-FSM.  In
// particular, the operand of a BRANCH node is the first node of the branch.
// (NB this is *not* a tree structure:  the tail of the branch connects
// to the thing following the set of BRANCHes.)  The opcodes are:
//

// definition   number  opnd?   meaning
#define	END      0      // no	End of program
#define	BOL      1      // no   Match "" at beginning of line
#define	EOL      2      // no   Match "" at end of line
#define	ANY      3      // no   Match any one character
#define	ANYOF    4      // str  Match any character in this string
#define	ANYBUT   5      // str  Match any character not in this string
#define	BRANCH   6      // node Match this alternative, or the next...
#define	BACK     7      // no   Match "", "next" ptr points backward
#define	EXACTLY  8      // str  Match this string
#define	NOTHING  9      // no   Match empty string
#define	STAR    10      // node Match this (simple) thing 0 or more times
#define	PLUS    11      // node Match this (simple) thing 1 or more times
#define	OPEN    20      // no   Mark this point in input as start of #n
                        //      OPEN+1 is number 1, etc
#define	CLOSE	30      // no   Analogous to OPEN

//
// Opcode notes:
//
// BRANCH       The set of branches constituting a single choice are hooked
//              together with their "next" pointers, since precedence prevents
//              anything being concatenated to any individual branch.  The
//              "next" pointer of the last BRANCH in a choice points to the
//              thing following the whole choice.  This is also where the
//              final "next" pointer of each individual branch points; each
//              branch starts with the operand node of a BRANCH node.
//
// BACK         Normal "next" pointers all implicitly point forward; BACK
//              exists to make loop structures possible.
//
// STAR,PLUS    '?', and complex '*' and '+', are implemented as circular
//              BRANCH structures using BACK.  Simple cases (one character
//              per match) are implemented with STAR and PLUS for speed
//              and to minimize recursive plunges.
//
// OPEN,CLOSE   ...are numbered at compile time.
//

//
// A node is one char of opcode followed by two chars of "next" pointer.
// "Next" pointers are stored as two 8-bit pieces, high order first.  The
// value is a positive offset from the opcode of the node containing it.
// An operand, if any, simply follows the node.  (Note that much of the
// code generation knows about this implicit relationship.)
//
// Using two bytes for the "next" pointer is vast overkill for most things,
// but allows patterns to get big without disasters.
//
#define	OP( p )    ( *( p ) )
#define	NEXT( p )  ( ( ( *( (p) + 1 ) &0377 ) << 8 ) + ( *( (p) + 2 ) & 0377 ) )
#define	OPERAND( p )  ( ( p ) + 3 )

//
// Utility definitions
//
#ifndef RE_CHARBITS
#define RE_CHARBITS  0xff
#endif

#define	UCHARAT( p )  ( ( int ) * ( p ) & RE_CHARBITS )

#define	FAIL( m )     { gkg::regerror( m ); return 0; }
#define	ISMULT( c )   ( ( c ) == '*' || ( c ) == '+' || ( c ) == '?' )
#define	META          "^$.[()|?+*\\"

//
// Flags to be passed up and down.
//
#define	HASWIDTH      01   // Known never to match null string
#define	SIMPLE        02   // Simple enough to be STAR/PLUS operand
#define	SPSTART       04   // Starts with * or +
#define	WORST          0   // Worst case

//
// Global work variables for regcomp().
//
static char *regparse;     // Input-scan pointer
static int32_t regnpar;    // () count
static char regdummy;
static char *regcode;      // Code-emit pointer; &regdummy = don't
static int32_t regsize;    // Code size

//
// - regcomp - compile a regular expression into internal code
//
// We can't allocate space until we know how big the compiled form will be,
// but we can't compile it (and thus know how big it is) until we've got a
// place to put the code.  So we cheat:  we compile it twice, once with code
// generation turned off and size counting turned on, and once "for real".
// This also means that we don't allocate space until we are sure that the
// thing really will compile successfully, and we never have to move the
// code and thus invalidate pointers into it.  (Note that it has to be in
// one piece because free() must be able to free it all.)
//
// Beware that the optimization-preparation code in here knows about some
// of the structure of the compiled regexp.
//
static gkg::regexp* gkg::regcomp( char* exp )
{

  gkg::regexp* r;
  char* scan;
  char* longest;
  int32_t len;
  int32_t flags;

  if ( !exp )
  {

    FAIL( "nil argument" );

  }

  // First pass: determine size, legality
  regparse = exp;
  regnpar = 1;
  regsize = 0L;
  regcode = &regdummy;
  regc( REGEXP_MAGIC );
  if ( !gkg::reg( 0, &flags ) )
  {

    return 0;

  }

  // Small enough for pointer-storage convention?
  if ( regsize >= 32767L )    // Probably could be 65535L
  {

    FAIL( "regexp too big" );

  }
  // Allocate space
  r = ( regexp* )new char[ sizeof( regexp ) + ( size_t )regsize ];

  // Second pass: emit code
  regparse = exp;
  regnpar = 1;
  regcode = r->program;
  gkg::regc( REGEXP_MAGIC );
  if ( !gkg::reg( 0, &flags ) )
  {

    delete r;
    return 0;

  }

  // Dig out information for optimizations
  r->regstart = '\0';  // Worst-case defaults
  r->reganch = 0;
  r->regmust = 0;
  r->regmlen = 0;
  scan = r->program + 1;              // First BRANCH
  if ( OP( gkg::regnext( scan ) ) == END )
  {

    // Only one top-level choice
    scan = OPERAND( scan );

    // Starting-point info
    if ( OP( scan ) == EXACTLY )
    {

      r->regstart = *OPERAND( scan );

    }
    else if ( OP( scan ) == BOL )
    {

      r->reganch++;

    }

    // If there's something expensive in the r.e., find the
    // longest literal string that must appear and make it the
    // regmust.  Resolve ties in favor of later strings, since
    // the regstart check works with the beginning of the r.e.
    // and avoiding duplication strengthens checking.  Not a
    // strong reason, but sufficient in the absence of others.
    if ( flags & SPSTART )
    {

      longest = 0;
      len = 0;
      for ( ; scan != 0; scan = gkg::regnext( scan ) )
      {

        if ( OP( scan ) == EXACTLY && 
             ( int32_t )strlen( OPERAND( scan ) ) >= len )
        {

          longest = OPERAND( scan );
          len = strlen( OPERAND( scan ) );

        }

      }
      r->regmust = longest;
      r->regmlen = len;

    }

  }
  return r;

}


//
// - reg - regular expression, i.e. main body or parenthesized thing
//
// Caller must absorb opening parenthesis.
//
// Combining parenthesis handling with the base level of regular expression
// is a trifle forced, but the need to tie the tails of the branches to what
// follows makes it hard to avoid.
//
static char* gkg::reg( int32_t paren, int32_t* flagp )
{

  char* ret;
  char* br;
  char* ender;
  int32_t parno = 0;
  int32_t flags;

  *flagp = HASWIDTH;   // Tentatively

  // Make an OPEN node, if parenthesized
  if ( paren )
  {

    if ( regnpar >= NSUBEXP )
    {

      FAIL( "too many ()" );

    }
    parno = regnpar;
    regnpar ++;
    ret = gkg::regnode( OPEN + parno );

  }
  else
  {

    ret = 0;

  }

  // Pick up the branches, linking them together
  br = gkg::regbranch( &flags );
  if ( !br )
  {

    return 0;

  }
  if ( ret )
  {

    gkg::regtail( ret, br );  // OPEN -> first

  }
  else
  {

    ret = br;

  }
  if ( !( flags & HASWIDTH ) )
  {

    *flagp &= ~HASWIDTH;

  }
  *flagp |= flags & SPSTART;
  while ( *regparse == '|' )
  {

    ++ regparse;
    br = gkg::regbranch( &flags );
    if ( !br )
    {

      return 0;

    }
    gkg::regtail( ret, br );  // BRANCH -> BRANCH
    if ( !( flags & HASWIDTH ) )
    {

      *flagp &= ~HASWIDTH;

    }
    *flagp |= flags & SPSTART;

  }

  // Make a closing node, and hook it on the end
  ender = gkg::regnode( paren ? CLOSE + parno : END );	
  gkg::regtail( ret, ender );

  // Hook the tails of the branches to the closing node
  for ( br = ret; br != 0; br = regnext( br ) )
  {

    gkg::regoptail( br, ender );

  }

  // Check for proper termination
  if ( paren && ( *regparse++ != ')' ) )
  {

    FAIL( "unmatched ()" );

  }
  else if ( !paren && ( *regparse != '\0' ) )
  {

    if ( *regparse == ')' )
    {

      FAIL( "unmatched ()" );

    }
    else
    {

      FAIL( "junk on end" );    // can't happen, NOTREACHED

    }

  }
  return ret;

}


//
// - regbranch - one alternative of an | operator
//
// Implements the concatenation operator.
//
static char* gkg::regbranch( int32_t* flagp )
{

  char* ret;
  char* chain;
  char* latest;
  int32_t flags;

  *flagp = WORST;  // Tentatively

  ret = gkg::regnode( BRANCH );
  chain = 0;
  while ( *regparse != '\0' && ( *regparse != '|' ) )
  {

    if ( *regparse == '\\' && ( regparse[ 1 ] == ')' ) )
    {

      regparse ++;
      break;

    }
    latest = gkg::regpiece( &flags );
    if ( !latest )
    {

      return 0;

    }
    *flagp |= flags & HASWIDTH;
    if ( !chain )  // First piece
    {

      *flagp |= flags & SPSTART;

    }
    else
    {

      gkg::regtail( chain, latest );

    }
    chain = latest;

  }
  if ( !chain )    // Loop ran zero times
  {

     gkg::regnode( NOTHING );

  }

  return ret;

}


//
// - regpiece - something followed by possible [*+?]
//
// Note that the branching code sequences used for ? and the general cases
// of * and + are somewhat optimized:  they use the same NOTHING node as
// both the endmarker for their branch list and the body of the last branch.
// It might seem that this node could be dispensed with entirely, but the
// endmarker role is not redundant.
//
static char* gkg::regpiece( int32_t* flagp )
{

  char* ret;
  char op;
  char* next;
  int32_t flags;

  ret = gkg::regatom( &flags );
  if ( !ret )
  {

    return 0;

  }

  op = *regparse;
  if ( !ISMULT( op ) )
  {

    *flagp = flags;
    return ret;

  }

  if ( !( flags & HASWIDTH ) && op != '?' )
  {

    FAIL( "*+ operand could be empty" );

  }
  *flagp = ( op != '+' ) ? ( WORST | SPSTART ) : ( WORST | HASWIDTH );

  if ( op == '*' && ( flags & SIMPLE ) )
  {

    gkg::reginsert( STAR, ret );

  }
  else if ( op == '*' )
  {

    // Emit x* as (x&|), where & means "self"
    gkg::reginsert( BRANCH, ret );                           // Either x
    gkg::regoptail( ret, gkg::regnode( BACK ) );          // and loop
    gkg::regoptail( ret, ret );                              // back
    gkg::regtail( ret, gkg::regnode( BRANCH ) );          // or
    gkg::regtail( ret, gkg::regnode( NOTHING ) );         // null

  }
  else if ( op == '+' && ( flags & SIMPLE ) )
  {

    gkg::reginsert( PLUS, ret );

  }
  else if ( op == '+' )
  {

    // Emit x+ as x(&|), where & means "self"
    next = gkg::regnode( BRANCH );                           // Either
    gkg::regtail( ret, next );
    gkg::regtail( gkg::regnode( BACK ), ret );            // loop back
    gkg::regtail( next, gkg::regnode( BRANCH ) );         // or
    gkg::regtail( ret, gkg::regnode( NOTHING ) );	        // null

  }
  else if ( op == '?' )
  {

    // Emit x? as (x|)
    gkg::reginsert( BRANCH, ret );                           // Either x
    gkg::regtail( ret, gkg::regnode( BRANCH ) );          // or
    next = gkg::regnode( NOTHING );                          // null
    gkg::regtail( ret, next );
    gkg::regoptail( ret, next );

  }
  regparse++ ;
  if ( ISMULT( *regparse ) )
  {

    FAIL( "nested *?+" );

  }
  return ret;

}


//
// - regatom - the lowest level
//
// Optimization:  gobbles an entire sequence of ordinary characters so that
// it can turn them into a single node, which is smaller to store and
// faster to run.  Backslashed characters are exceptions, each becoming a
// separate node; the code is simpler that way and it's not worth fixing.
//
static char* gkg::regatom( int32_t* flagp )
{

  char* ret;
  int32_t flags;

  *flagp = WORST;  // Tentatively

  switch ( *regparse++ )
  {

    case '^':

      ret = gkg::regnode( BOL );
      break;

    case '$':

      ret = gkg::regnode( EOL );
      break;

    case '.':

      ret = gkg::regnode( ANY );
      *flagp |= HASWIDTH | SIMPLE;
      break;

    case '[':

      {
        int32_t classbeg;
        int32_t classend;

        if ( *regparse == '^' )
        {

          // Complement of range
          ret = gkg::regnode( ANYBUT );
          regparse ++;

        }
        else
          ret = gkg::regnode( ANYOF );
        if ( ( *regparse == ']' ) || ( *regparse == '-' ) )
        {

          regc( *regparse++ );

        }
        while ( ( *regparse != '\0' ) && ( *regparse != ']' ) )
        {

          if ( *regparse == '-' )
          {

            ++ regparse;
            if ( ( *regparse == ']' ) || ( *regparse == '\0' ) )
            {

              gkg::regc('-');

            }
            else
            {

              classbeg = UCHARAT( regparse - 2 ) + 1;
              classend = UCHARAT( regparse );
              if ( classbeg > classend + 1 )
              {

                FAIL( "invalid [] range" );

              }
              for ( ; classbeg <= classend; classbeg++ )
              {

                gkg::regc( classbeg );

              }
              ++ regparse;

            }

          }
          else
          {

            gkg::regc( *regparse++ );

          }

        }
        gkg::regc( '\0' );
        if ( *regparse != ']' )
        {

          FAIL( "unmatched []" );

        }
        ++ regparse;
        *flagp |= HASWIDTH | SIMPLE;

      }
      break;

    case '\0':
    case '|':

      {

        FAIL( "internal urp" );  // Supposed to be caught earlier

      }
      break;

    case '?':
    case '+':
    case '*':

      {

        FAIL( "?+* follows nothing" );

      }
      break;

    case '\\':

      if ( *regparse == '\0' )
      {

        FAIL( "trailing \\" );

      }
      if ( *regparse == '(' )
      {

        ++ regparse;
        ret = gkg::reg( 1, &flags );
        if ( !ret )
        {

          return 0;

        }
        *flagp |= flags & ( HASWIDTH | SPSTART );

      }
      else
      {

        ret = gkg::regnode( EXACTLY );
        gkg::regc( *regparse++ );
        gkg::regc( '\0' );
        *flagp |= HASWIDTH | SIMPLE;

      }
      break;

    default:

      {

        int32_t len;
        char ender;

        regparse --;
        len = strcspn( regparse, META );
        if ( len <= 0 )
        {

          FAIL( "internal disaster" );

        }
        ender = *( regparse + len );
        if ( len > 1 && ISMULT( ender ) )
        {

          len--;  // Back off clear of ?+* operand

        }
        *flagp |= HASWIDTH;
        if ( len == 1 )
        {

          *flagp |= SIMPLE;

        }
        ret = gkg::regnode( EXACTLY );
        while ( len > 0 )
        {

          gkg::regc( *regparse++ );
          -- len;

        }
        gkg::regc( '\0' );

      }
      break;

  }

  return ret;

}


//
// - regnode - emit a node
//
static char* gkg::regnode( char op )
{

  char* ret;
  char* ptr;

  ret = regcode;
  if ( ret == &regdummy )
  {

    regsize += 3;
    return ret;

  }

  ptr = ret;
  *ptr++ = op;
  *ptr++ = '\0';  // Null "next" pointer
  *ptr++ = '\0';
  regcode = ptr;

  return ret;

}


//
// - regc - emit (if appropriate) a byte of code
//
static void gkg::regc( char b )
{

  if ( regcode != &regdummy )
  {

    *regcode++ = b;

  }
  else
  {

    ++ regsize;

  }

}


//
// - reginsert - insert an operator in front of already-emitted operand
//
// Means relocating the operand.
//
static void gkg::reginsert( char op, char* opnd )
{

  char* src;
  char* dst;
  char* place;

  if ( regcode == &regdummy )
  {

    regsize += 3;
    return;

  }

  src = regcode;
  regcode += 3;
  dst = regcode;
  while ( src > opnd )
  {

    *--dst = *--src;

  }

  place = opnd;   // Op node, where operand used to be
  *place++ = op;
  *place++ = '\0';
  *place++ = '\0';

}


//
// - regtail - set the next-pointer at the end of a node chain
//
static void gkg::regtail( char* p, char* val )
{

  char* scan;
  char* temp;
  int32_t offset;

  if ( p == &regdummy )
  {

    return;

  }

  // Find last node
  scan = p;
  for ( ; ; )
  {

    temp = gkg::regnext( scan );
    if ( !temp )
    {

      break;

    }
    scan = temp;

  }

  if ( OP( scan ) == BACK )
  {

    offset = scan - val;

  }
  else
  {

    offset = val - scan;

  }
  *( scan + 1 ) = ( offset >> 8 ) & 0377;
  *( scan + 2 ) = offset & 0377;

}


//
// - regoptail - regtail on operand of first argument; nop if operandless
//
static void gkg::regoptail( char* p, char* val )
{

  // "Operandless" and "op != BRANCH" are synonymous in practice
  if ( !p || ( p == &regdummy ) || ( OP( p ) != BRANCH ) )
  {

    return;

  }
  gkg::regtail( OPERAND( p ), val );

}


//
// regexec and friends
//

//
// Global work variables for regexec().
//
static char* reginput;    // String-input pointer
static char* regbol;      // Beginning of input, for ^ check
static char** regstartp;  // Pointer to startp array
static char** regendp;    // Ditto for endp


//
// - regexec - match a regexp against a string
//
static int32_t gkg::regexec( regexp* prog, char* theString )
{

  char *s;

  // Be paranoid...
  if ( !prog || !theString )
  {

    gkg::regerror( "nil parameter" );
    return 0;

  }

  // Check validity of program
  if ( UCHARAT( prog->program ) != REGEXP_MAGIC )
  {

    gkg::regerror( "corrupted program" );
    return 0;

  }

  // If there is a "must appear" string, look for it
  if ( prog->regmust )
  {

    s = theString;
    while ( ( s = strchr( s, prog->regmust[ 0 ] ) ) != 0 )
    {

      if ( strncmp( s, prog->regmust, prog->regmlen ) == 0 )
      {

        break;     // Found it

      }
      ++ s;

    }
    if ( !s )   // Not present
    {

      return 0;

    }

  }

  // Mark beginning of line for ^
  regbol = theString;

  // Simplest case:  anchored match need be tried only once
  if ( prog->reganch )
  {

    return gkg::regtry( prog, theString );

  }

  // Messy cases:  unanchored match
  s = theString;
  if ( prog->regstart != '\0' ) // We know what char it must start with
  {

    while ( ( s = strchr( s, prog->regstart ) ) != 0 )
    {

      if ( gkg::regtry( prog, s ) )
      {

        return 1;

      }
      ++ s;

    }

  }
  else // We don't -- general case
  {

    do
    {

      if ( gkg::regtry( prog, s ) )
      {

        return 1;

      }

    }
    while ( *s++ != '\0' );

  }

  // Failure
  return 0;

}


//
// - regtry - try match at specific point (0 failure, 1 success)
//
static int32_t gkg::regtry( regexp* prog, char* theString )
{

  int32_t i;
  char** sp;
  char** ep;

  reginput = theString;
  regstartp = prog->startp;
  regendp = prog->endp;

  sp = prog->startp;
  ep = prog->endp;
  for ( i = NSUBEXP; i > 0; i-- )
  {

    *sp++ = 0;
    *ep++ = 0;

  }
  if ( regmatch( prog->program + 1 ) )
  {

    prog->startp[ 0 ] = theString;
    prog->endp[ 0 ] = reginput;
    return 1;

  }
  else
  {

    return 0;

  }

}


//
// - regmatch - main matching routine
//
// Conceptually the strategy is simple:  check to see whether the current
// node matches, call self recursively to see whether the rest matches,
// and then act accordingly.  In practice we make some effort to avoid
// recursion, in particular by going through "ordinary" nodes (that don't
// need to know whether the rest of the match failed) by a loop instead of
// by recursion.
// 0 failure, 1 success
//
static int32_t gkg::regmatch( char* prog )
{

  char* scan;   // Current node
  char* next;            // Next node

  scan = prog;
  while ( scan )
  {

    next = gkg::regnext( scan );

    switch ( OP( scan ) )
    {

      case BOL:

        if ( reginput != regbol )
        {

          return 0;

        }
        break;

      case EOL:

        if ( *reginput != '\0' )
        {

          return 0;

        }
        break;

      case ANY:

        if ( *reginput == '\0' )
        {

          return 0;

        }
        ++ reginput;
        break;

      case EXACTLY:

        {

          int32_t len;
          char* opnd;

          opnd = OPERAND( scan );
          // Inline the first character, for speed
          if ( *opnd != *reginput )
          {

            return 0;

          }
          len = strlen( opnd );
          if ( ( len > 1 ) && ( strncmp( opnd, reginput, len ) != 0 ) )
          {

            return 0;

          }
          reginput += len;

        }
        break;

      case ANYOF:

        if ( *reginput == '\0' )
        {

          return 0;

        }
        if ( !strchr( OPERAND( scan ), *reginput ) )
        {

          return 0;

        }
        ++ reginput;
        break;

      case ANYBUT:

        if ( *reginput == '\0' )
        {

          return 0;

        }
        if ( strchr( OPERAND( scan ), *reginput ) )
        {

          return 0;

        }
        ++ reginput;
        break;

      case NOTHING:

        break;

      case BACK:

        break;

      case OPEN+1:
      case OPEN+2:
      case OPEN+3:
      case OPEN+4:
      case OPEN+5:
      case OPEN+6:
      case OPEN+7:
      case OPEN+8:
      case OPEN+9:

        {

          int32_t no;
          char* save;

          no = OP( scan ) - OPEN;
          save = reginput;

          if ( gkg::regmatch( next ) )
          {

            // Don't set startp if some later invocation of the same parentheses
            // already has
            if ( !regstartp[ no ] )
            {

              regstartp[ no ] = save;

            }
            return 1;

          }
          else
          {

            return 0;

          }

        }
        break;

      case CLOSE+1:
      case CLOSE+2:
      case CLOSE+3:
      case CLOSE+4:
      case CLOSE+5:
      case CLOSE+6:
      case CLOSE+7:
      case CLOSE+8:
      case CLOSE+9:

        {

          int32_t no;
          char* save;

          no = OP( scan ) - CLOSE;
          save = reginput;

          if ( gkg::regmatch( next ) )
          {

            // Don't set endp if some later  invocation of the same parentheses
            // already has.
            if ( !regendp[ no ] )
            {

              regendp[ no ] = save;

            }
            return 1;

          }
          else
          {

            return 0;

          }

        }
        break;

      case BRANCH:

        {

          char* save;

          if ( OP( next ) != BRANCH )    // No choice
          {

            next = OPERAND( scan );      // Avoid recursion

          }
          else
          {

            do
            {

              save = reginput;
              if ( gkg::regmatch( OPERAND( scan ) ) )
              {

                return 1;

              }
              reginput = save;
              scan = gkg::regnext( scan );

            }
            while ( scan && OP( scan ) == BRANCH );
              return 0;
            // NOTREACHED

          }

        }
        break;

      case STAR:
      case PLUS:

        {

          char nextch;
          int32_t no;
          char* save;
          int32_t min;

          // Lookahead to avoid useless match attempts
          // when we know what character comes next.
          nextch = '\0';
          if ( OP( next ) == EXACTLY )
          {

            nextch = *OPERAND( next );

          }
          min = ( OP( scan ) == STAR ) ? 0 : 1;
          save = reginput;
          no = gkg::regrepeat( OPERAND( scan ) );
          while ( no >= min )
          {

            // If it could work, try it
            if ( nextch == '\0' || *reginput == nextch )
            {

              if ( gkg::regmatch( next ) )
              {

                return 1;

              }

            }
            // Couldn't or didn't -- back up
            -- no;
            reginput = save + no;

          }
          return 0;

        }
        break;

      case END:

        return 1;  // Success!

      default:

        gkg::regerror( "memory corruption" );
        return 0;

    }
    scan = next;

  }

  // we get here only if there's trouble -- normally "case END" is
  // the terminating point.
  gkg::regerror( "corrupted pointers" );
  return 0;

}


//
// - regrepeat - repeatedly match something simple, report how many
//
static int32_t gkg::regrepeat( char* p )
{

  int32_t count = 0;
  char* scan;
  char* opnd;

  scan = reginput;
  opnd = OPERAND( p );
  switch ( OP( p ) )
  {

    case ANY:

      count = strlen( scan );
      scan += count;
      break;

    case EXACTLY:

      while ( *opnd == *scan )
      {

        ++ count;
        ++ scan;

      }
      break;

    case ANYOF:

      while ( ( *scan != '\0' ) && ( strchr( opnd, *scan ) != 0 ) )
      {

        ++ count;
        ++ scan;

      }
      break;

    case ANYBUT:

      while ( ( *scan != '\0' ) && ( strchr( opnd, *scan ) == 0 ) )
      {

        ++ count;
        ++ scan;

      }
      break;

    default:    // Oh dear.  Called inappropriately

      gkg::regerror( "internal foulup" );
      count = 0;  // Best compromise
      break;

  }
  reginput = scan;

  return count;

}


//
// - regnext - dig the "next" pointer out of a node
//
static char* gkg::regnext( char* p )
{

  int32_t offset;

  if ( p == &regdummy )
  {

    return 0;

  }

  offset = NEXT( p );
  if ( offset == 0 )
  {

    return 0;

  }

  if ( OP( p ) == BACK )
  {

    return p-offset;

  }
  else
  {

    return p + offset;

  }

}


static void gkg::regerror( const char* s )
{

  std::cerr << "regexp: " << s << "\n";

}


