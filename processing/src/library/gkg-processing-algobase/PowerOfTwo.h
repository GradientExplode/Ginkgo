#ifndef _gkg_processing_algobase_PowerOfTwo_h_
#define _gkg_processing_algobase_PowerOfTwo_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class PowerOfTwo : public Singleton< PowerOfTwo >
{

  public:

    ~PowerOfTwo();

    template < class T >
    int32_t getLowerOrEqual( const T& value ) const;
    template < class T >
    int32_t getGreaterOrEqual( const T& value ) const;

    template < class T >
    int32_t getStrictlyLower( const T& value ) const;
    template < class T >
    int32_t getStrictlyGreater( const T& value ) const;

    template < class T >
    int32_t getClosest( const T& value ) const;

    template < class T >
    int32_t getLowerOrEqualExponent( const T& value ) const;
    template < class T >
    int32_t getGreaterOrEqualExponent( const T& value ) const;

    template < class T >
    int32_t getStrictlyLowerExponent( const T& value ) const;
    template < class T >
    int32_t getStrictlyGreaterExponent( const T& value ) const;

    template < class T >
    int32_t getClosestExponent( const T& value ) const;

  protected:

    friend class Singleton< PowerOfTwo >;

    PowerOfTwo();

};



}


#endif
