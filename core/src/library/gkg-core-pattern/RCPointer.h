#ifndef _gkg_core_pattern_RCPointer_h_
#define _gkg_core_pattern_RCPointer_h_


#include <gkg-core-cppext/StdInt.h>
#include <memory>
#include <iostream>
#include <boost/version.hpp>

#if BOOST_VERSION >= 103300 &&\
    ( !defined( linux ) || ( defined( __GNUC__ ) && __GNUC__ >= 4 ) )

  #include <boost/detail/atomic_count.hpp>

  #ifdef BOOST_AC_USE_PTHREADS

    #error Boost is using pthreads which is not supported in this \
           implementation of reference counted pointers

  #endif

#else

  #error Boost release >= 103300 is required to compile this code

#endif


namespace gkg
{


template < class T > class ConstReference;
template < class T > class Reference;


//
// class DefaultReferenceConstruction
//

class DefaultReferenceConstruction
{

  private:

    template < class T > friend class Reference;
    template < class T > friend class ConstReference;

    template < class T >
    static void construct( ConstReference< T >* constReference,
                           const T* t,
                           bool externalOwner );
    template < class T >
    static void destroy( ConstReference< T >* constReference );
    template < class T >
    static const T* release( ConstReference< T >* constReference );

};


//
// class RCObject
//

class RCObject : public virtual DefaultReferenceConstruction
{

  public:

    typedef boost::detail::atomic_count CountType;

    RCObject();
    RCObject( const RCObject& other );
    virtual ~RCObject();

    RCObject& operator= ( const RCObject& other );

  private:

    template < class T > friend class Reference;
    template < class T > friend class ConstReference;

    template < class T >
    static void construct( ConstReference< T >* constReference,
                           const T* t,
                           bool externalOwner );
    template < class T >
    static void destroy( ConstReference< T >* constReference );
    template < class T >
    static const T* release( ConstReference< T >* constReference );

    mutable CountType _count;

};


//
// class ReferenceConstruction< T >
//

template < class T >
class ReferenceConstruction : T, virtual DefaultReferenceConstruction
{

  private:

    template < class U > friend class Reference;
    template < class U > friend class ConstReference;

    // defined to avoid compilation error when not defined for type T
    ReferenceConstruction();    


};


//
// class ReferenceData< T >
//

template < class T >
class ReferenceData
{

  public:

    int32_t getCount() const;

  private:

    template < class U > friend class Reference;
    template < class U > friend class ConstReference;
    friend class DefaultReferenceConstruction;
    friend class RCObject;

    RCObject::CountType* _pointerCount;
    T* _referenced;

};


//
// class ConstReference< T >
//

template < class T >
class ConstReference : public ReferenceData< T >
{

  public:

    typedef T referenced_type;

    ConstReference();
    explicit ConstReference( const T* t );
    explicit ConstReference( const T* t, bool externalOwner );
    ConstReference( const ConstReference< T >& other );
    ConstReference( const Reference< T >& other );
    template < class U > ConstReference( const ConstReference< U >& other );
    template < class U > ConstReference( const Reference< U >& other );
    ~ConstReference();

    ConstReference< T >& operator=( const ConstReference< T >& other );
    ConstReference< T >& operator=( const Reference< T >& other );

    bool isNull() const;

    bool operator==( const ConstReference< T >& other ) const;
    bool operator==( const Reference< T >& other ) const;
    bool operator==( const T* t ) const;

    bool operator!=( const ConstReference< T >& other ) const;
    bool operator!=( const Reference< T >& other ) const;
    bool operator!=( const T* t ) const;

    const T* operator->() const;
    const T& operator*() const;
    const T* getPointer() const;

    //int32_t getCount() const;

  private:

    template < class U > friend class Reference;
    friend class DefaultReferenceConstruction;
    friend class RCObject;

};


//
// class Reference< T >
//

template < class T >
class Reference : public ConstReference< T >
{

  public:

    typedef T referenced_type;  // a supprimer ?

    Reference();
    explicit Reference( const T* t );
    explicit Reference( const T* t, bool externalOwner );
    Reference( const Reference< T >& other );
    template < class U > Reference( const Reference< U >& other );
    ~Reference();

    T* release();

    Reference< T >& operator=( const Reference< T >& other );

    T* operator->() const;
    T& operator*() const;
    T* getPointer() const;

};


//
// class RCPointer< T >
//

template < class T >
class RCPointer : public Reference< T >
{

  public:

    RCPointer();
    explicit RCPointer( T* t );
    explicit RCPointer( const T* t, bool externalOwner );
    template < class U > RCPointer( const RCPointer< U >& other );

    void reset( T* t = 0 );
    T* get() const;
    bool operator<( const RCPointer< T >& other ) const;

};


}


//
// class DefaultReferenceConstruction
//

template < class T >
inline
void gkg::DefaultReferenceConstruction::construct(
                                      gkg::ConstReference< T >* constReference,
                                      const T* t,
                                      bool externalOwner )
{

  if ( t && !externalOwner )
  {

    constReference->_pointerCount = new gkg::RCObject::CountType( 1 );

  }
  else
  {

    constReference->_pointerCount = 0;

  }
  constReference->_referenced = const_cast< T* >( t );

}


template < class T >
inline
void gkg::DefaultReferenceConstruction::destroy( 
                                     gkg::ConstReference< T >* constReference )
{

  if ( constReference->_pointerCount &&
       ( --( *constReference->_pointerCount ) == 0 ) )
  {

    delete constReference->_pointerCount;
    delete static_cast< T* >( constReference->_referenced );

    constReference->_pointerCount = 0;
    constReference->_referenced = 0;

  }

}


template < class T >
inline
const T* gkg::DefaultReferenceConstruction::release(
                                      gkg::ConstReference< T >* constReference )
{

  const T* const t = constReference->_referenced;
  if ( constReference->_pointerCount &&
       ( --( *constReference->_pointerCount ) == 0 ) )
  {

    delete constReference->_pointerCount;
    constReference->_pointerCount = 0;

  }
  constReference->_referenced = 0;
  return t;

}


//
// class RCObject
//

inline
gkg::RCObject::RCObject()
              : _count( 0 )
{
}


inline
gkg::RCObject::RCObject( const gkg::RCObject& /*other*/ )
              : _count( 0 )
{
}


inline
gkg::RCObject::~RCObject()
{
}


inline
gkg::RCObject& gkg::RCObject::operator=( const gkg::RCObject& /*other*/ )
{

  return *this;

}


template < class T >
inline
void gkg::RCObject::construct( gkg::ConstReference< T >* constReference,
                               const T* t,
                               bool externalOwner )
{

  if ( t )
  {

    int32_t count = t->_count;
    if ( externalOwner && !count )
    {

      -- t->_count;

    }
    else if ( count >= 0 )
    {

      ++ t->_count;

    }
    constReference->_pointerCount = &t->_count;

  }
  else
  {

    constReference->_pointerCount = 0;

  }
  constReference->_referenced = const_cast< T* >( t );

}


template < class T >
inline
void gkg::RCObject::destroy( gkg::ConstReference< T >* constReference )
{

  if ( constReference->_pointerCount &&
       ( --( *constReference->_pointerCount ) == 0 ) )
  {

    delete static_cast< T* >( constReference->_referenced );
    constReference->_referenced = 0;

  }

}


template < class T >
inline
const T* gkg::RCObject::release( gkg::ConstReference< T >* constReference )
{

  const T* const t = constReference->_referenced;
  if ( constReference->_pointerCount &&
       ( *constReference->_pointerCount >= 0 ) )
  {

    -- *constReference->_pointerCount;

  }
  constReference->_pointerCount = 0;
  constReference->_referenced = 0;
  return t;

}


//
// class ReferenceConstruction< T >
//



//
// class ReferenceData< T >
//

template < class T >
inline
int32_t gkg::ReferenceData< T >::getCount() const
{

  return ( _pointerCount ? *_pointerCount : 0 );

}


//
// class ConstReference< T >
//

template < class T >
inline
gkg::ConstReference< T >::ConstReference()
{

  this->_pointerCount = 0;
  this->_referenced = 0;

}


template < class T >
inline
gkg::ConstReference< T >::ConstReference( const T* t )
{

  gkg::ReferenceConstruction< T >::construct( this, t, false );

}


template < class T >
inline
gkg::ConstReference< T >::ConstReference( const T* t, bool externalOwner )
{

  gkg::ReferenceConstruction< T >::construct( this, t, externalOwner );

}


template < class T >
inline
gkg::ConstReference< T >::ConstReference(
                                         const gkg::ConstReference< T >& other )
{

  if ( other._pointerCount && ( *other._pointerCount >= 0 ) )
  {

    ++ ( *other._pointerCount );

  }
  this->_pointerCount = other._pointerCount;
  this->_referenced = static_cast< T* >( other._referenced );

}


template < class T >
inline
gkg::ConstReference< T >::ConstReference( const gkg::Reference< T >& other )
{

  if ( other._pointerCount && ( *other._pointerCount >= 0 ) )
  {

    ++ ( *other._pointerCount );

  }
  this->_pointerCount = other._pointerCount;
  this->_referenced = static_cast< T* >( other._referenced );

}


template < class T > template < class U >
inline
gkg::ConstReference< T >::ConstReference(
                                         const gkg::ConstReference< U >& other )
{

  if ( other._pointerCount && ( *other._pointerCount >= 0 ) )
  {

    ++ ( *other._pointerCount );

  }
  this->_pointerCount = other._pointerCount;
  this->_referenced = static_cast< T* >( other._referenced );

}


template < class T > template < class U >
inline
gkg::ConstReference< T >::ConstReference( const gkg::Reference< U >& other )
{

  const gkg::ReferenceData< U >& o = other;
  if ( o._pointerCount && ( *o._pointerCount >= 0 ) )
  {

    ++ ( *o._pointerCount );

  }
  this->_pointerCount = o._pointerCount;
  this->_referenced = static_cast< T* >( o._referenced );

}


template < class T >
inline
gkg::ConstReference< T >::~ConstReference()
{

  gkg::ReferenceConstruction< T >::destroy( this );

}


template < class T >
inline
gkg::ConstReference< T >& 
gkg::ConstReference< T >::operator=( const gkg::ConstReference< T >& other )
{

  if ( ( this != &other )  && ( this->_referenced != other._referenced ) )
  {

    if ( other._pointerCount && ( *other._pointerCount >= 0 ) )
    {

      ++ ( *other._pointerCount );

    }
    gkg::ReferenceConstruction< T >::destroy( this );
    this->_pointerCount = other._pointerCount;
    this->_referenced = other._referenced;

  }
  return *this;

}


template < class T >
inline
gkg::ConstReference< T >& 
gkg::ConstReference< T >::operator=( const gkg::Reference< T >& other )
{

  if ( this->_referenced != other._referenced )
  {

    if ( other._pointerCount && ( *other._pointerCount >= 0 ) )
    {

      ++ ( *other._pointerCount );

    }
    gkg::ReferenceConstruction< T >::destroy( this );
    this->_pointerCount = other._pointerCount;
    this->_referenced = other._referenced;

  }
  return *this;

}

template < class T >
inline
bool gkg::ConstReference< T >::isNull() const
{

  return ( this->_pointerCount == 0 );

}


template < class T >
inline
bool gkg::ConstReference< T >::operator==(
                                   const gkg::ConstReference< T >& other ) const
{

  return ( this->_referenced == other._referenced );

}


template < class T >
inline
bool gkg::ConstReference< T >::operator==(
                                        const gkg::Reference< T >& other ) const
{

  return ( this->_referenced == other._referenced );

}


template < class T >
inline
bool gkg::ConstReference< T >::operator==( const T* t ) const
{

  return ( this->_referenced == t );

}


template < class T >
inline
bool gkg::ConstReference< T >::operator!=(
                                   const gkg::ConstReference< T >& other ) const
{

  return ( this->_referenced != other._referenced );

}


template < class T >
inline
bool gkg::ConstReference< T >::operator!=(
                                        const gkg::Reference< T >& other ) const
{

  return ( this->_referenced != other._referenced );

}


template < class T >
inline
bool gkg::ConstReference< T >::operator!=( const T* t ) const
{

  return ( this->_referenced != t );

}


template < class T >
inline
const T* gkg::ConstReference< T >::operator->() const
{

  return static_cast< const T* >( this->_referenced );

}


template < class T >
inline
const T& gkg::ConstReference< T >::operator*() const
{

  return *static_cast< const T* >( this->_referenced );

}


template < class T >
inline
const T* gkg::ConstReference< T >::getPointer() const
{

  return static_cast< const T* >( this->_referenced );

}


/*
template < class T >
inline
int32_t gkg::ConstReference< T >::getCount() const
{

  return ( this->_pointerCount ? *this->_pointerCount : 0 );

}
*/


//
// class Reference< T >
//

template < class T >
inline
gkg::Reference< T >::Reference()
                    : gkg::ConstReference< T >()
{
}


template < class T >
inline
gkg::Reference< T >::Reference( const T* t )
                    : gkg::ConstReference< T >( t )
{
}


template < class T >
inline
gkg::Reference< T >::Reference( const T* t, bool externalOwner )
                    : gkg::ConstReference< T >( t, externalOwner )
{
}


template < class T >
inline
gkg::Reference< T >::Reference( const gkg::Reference< T >& other )
                    : gkg::ConstReference< T >( other )
{
}


template < class T > template < class U >
inline
gkg::Reference< T >::Reference( const gkg::Reference< U >& other )
                    : gkg::ConstReference< T >( other )
{
}


template < class T >
inline
gkg::Reference< T >::~Reference()
{
}


template < class T >
inline
T* gkg::Reference< T >::release()
{

  return const_cast< T* >( gkg::ReferenceConstruction< T >::release( this ) );

}


template < class T >
inline
gkg::Reference< T >& 
gkg::Reference< T >::operator=( const gkg::Reference< T >& other )
{

  this->gkg::ConstReference< T >::operator=( other );
  return *this;

}


template < class T >
inline
T* gkg::Reference< T >::operator->() const
{

  return static_cast< T* >( this->_referenced );

}


template < class T >
inline
T& gkg::Reference< T >::operator*() const
{

  return *static_cast< T* >( this->_referenced );

}


template < class T >
inline
T* gkg::Reference< T >::getPointer() const
{

  return static_cast< T* >( this->_referenced );

}


//
// class RCPointer< T >
//

template < class T >
inline
gkg::RCPointer< T >::RCPointer()
{
}


template < class T >
inline
gkg::RCPointer< T >::RCPointer( T* t )
                    : gkg::Reference< T >( t )
{
}


template < class T >
inline
gkg::RCPointer< T >::RCPointer( const T* t, bool externalOwner )
                    : gkg::Reference< T >( t, externalOwner )
{
}


template < class T > template < class U >
inline
gkg::RCPointer< T >::RCPointer( const gkg::RCPointer< U >& other )
                    : gkg::Reference< T >( other )
{
}


template < class T >
inline
void gkg::RCPointer< T >::reset( T* t )
{

  *static_cast< gkg::Reference< T >* >( this ) = gkg::Reference< T >( t );

}


template < class T >
inline
T* gkg::RCPointer< T >::get() const
{

  return this->getPointer();

}


template < class T >
inline
bool gkg::RCPointer< T >::operator<( const gkg::RCPointer< T >& other ) const
{

  return ( this->getPointer() < other.getPointer() );

}


//
// specialization(s) for scalar types that are non struct or class 
//

#define REGISTER_REFERENCE_CONSTRUCTION( type )                                \
namespace gkg                                                                  \
{                                                                              \
                                                                               \
template <>                                                                    \
class ReferenceConstruction< type > : DefaultReferenceConstruction             \
{                                                                              \
                                                                               \
  private:                                                                     \
                                                                               \
    template < class U > friend class Reference;                               \
    template < class U > friend class ConstReference;                          \
                                                                               \
    ReferenceConstruction();                                                   \
                                                                               \
                                                                               \
};                                                                             \
                                                                               \
}                                                                              \


REGISTER_REFERENCE_CONSTRUCTION( int8_t );
REGISTER_REFERENCE_CONSTRUCTION( uint8_t );
REGISTER_REFERENCE_CONSTRUCTION( int16_t );
REGISTER_REFERENCE_CONSTRUCTION( uint16_t );
REGISTER_REFERENCE_CONSTRUCTION( int32_t );
REGISTER_REFERENCE_CONSTRUCTION( uint32_t );
REGISTER_REFERENCE_CONSTRUCTION( int64_t );
REGISTER_REFERENCE_CONSTRUCTION( uint64_t );
REGISTER_REFERENCE_CONSTRUCTION( float );
REGISTER_REFERENCE_CONSTRUCTION( double );
REGISTER_REFERENCE_CONSTRUCTION( bool );


#endif
