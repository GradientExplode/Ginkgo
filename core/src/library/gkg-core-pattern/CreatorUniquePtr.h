#ifndef _gkg_core_pattern_CreatorUniquePtr_h_
#define _gkg_core_pattern_CreatorUniquePtr_h_


#include <memory>


namespace gkg
{


template < class T, class SubT >
struct CreatorUniquePtrNoArg
{

  static std::unique_ptr< SubT > createInstance()
  { return Wt::cpp14::make_unique< T >(); }

};


template < class T, class SubT, class Arg >
struct CreatorUniquePtr1Arg
{

  static std::unique_ptr< SubT > createInstance( Arg arg )
  { return Wt::cpp14::make_unique< T >( arg ); }

};


template < class T, class SubT, class Arg1, class Arg2 >
struct CreatorUniquePtr2Arg
{

  static std::unique_ptr< SubT > createInstance( Arg1 arg1, Arg2 arg2 )
  { return Wt::cpp14::make_unique< T >( arg1, arg2 ); }

};


template < class T, class SubT, class Arg1, class Arg2, class Arg3 >
struct CreatorUniquePtr3Arg
{

  static std::unique_ptr< SubT > 
                               createInstance( Arg1 arg1, Arg2 arg2, Arg3 arg3 )
  { return Wt::cpp14::make_unique< T >( arg1, arg2, arg3 ); }

};


template < class T, class SubT, 
           class Arg1, class Arg2, class Arg3, class Arg4 >
struct CreatorUniquePtr4Arg
{

  static std::unique_ptr< SubT > 
                    createInstance( Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4 )
  { return Wt::cpp14::make_unique< T >( arg1, arg2, arg3, arg4 ); }

};


template < class T, class SubT, 
  class Arg1, class Arg2, class Arg3, class Arg4, class Arg5 >
struct CreatorUniquePtr5Arg
{

  static std::unique_ptr< SubT > createInstance( Arg1 arg1, Arg2 arg2, 
                               Arg3 arg3, Arg4 arg4, Arg5 arg5 )
  { return Wt::cpp14::make_unique< T >( arg1, arg2, arg3, arg4, arg5 ); }

};


}


#endif
