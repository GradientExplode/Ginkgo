#ifndef _gkg_core_pattern_Creator_h_
#define _gkg_core_pattern_Creator_h_



namespace gkg
{


template < class T, class SubT >
struct CreatorNoArg
{

  static SubT* createInstance()
  { return new T; }

};


template < class T, class SubT, class Arg >
struct Creator1Arg
{

  static SubT* createInstance( Arg arg )
  { return new T( arg ); }

};


template < class T, class SubT, class Arg1, class Arg2 >
struct Creator2Arg
{

  static SubT* createInstance( Arg1 arg1, Arg2 arg2 )
  { return new T( arg1, arg2 ); }

};


template < class T, class SubT, class Arg1, class Arg2, class Arg3 >
struct Creator3Arg
{

  static SubT* createInstance( Arg1 arg1, Arg2 arg2, Arg3 arg3 )
  { return new T( arg1, arg2, arg3 ); }

};


template < class T, class SubT, 
           class Arg1, class Arg2, class Arg3, class Arg4 >
struct Creator4Arg
{

  static SubT* createInstance( Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4 )
  { return new T( arg1, arg2, arg3, arg4 ); }

};


template < class T, class SubT, 
  class Arg1, class Arg2, class Arg3, class Arg4, class Arg5 >
struct Creator5Arg
{

  static SubT* createInstance( Arg1 arg1, Arg2 arg2, 
                               Arg3 arg3, Arg4 arg4, Arg5 arg5 )
  { return new T( arg1, arg2, arg3, arg4, arg5 ); }

};


}


#endif
