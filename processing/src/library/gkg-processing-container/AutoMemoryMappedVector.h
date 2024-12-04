#ifndef _gkg_processing_container_AutoMemoryMappedVector_h_
#define _gkg_processing_container_AutoMemoryMappedVector_h_


#include <gkg-core-cppext/StdInt.h>
#include <iterator>


namespace gkg
{


template < class T > class AutoMemoryMappedVector;


//
// class AutoMemoryMappedIterator< I, T >
//

template < class I, class T >
class AutoMemoryMappedIterator
{

  public:

    typedef I iterator_type;
    typedef typename std::iterator_traits< I >::iterator_category
                                                              iterator_category;
    typedef typename std::iterator_traits< I >::value_type value_type;
    typedef typename std::iterator_traits< I >::difference_type difference_type;
    typedef typename std::iterator_traits< I >::reference reference;
    typedef typename std::iterator_traits< I >::pointer pointer;


    AutoMemoryMappedIterator();
    AutoMemoryMappedIterator( const AutoMemoryMappedVector< T >* container,
                              uint64_t index );
    template < class J >
    AutoMemoryMappedIterator( const AutoMemoryMappedIterator< J, T >& other );

    reference operator*() const;
    pointer operator->() const;

    AutoMemoryMappedIterator< I, T >& operator++();
    AutoMemoryMappedIterator< I, T > operator++( int );

    AutoMemoryMappedIterator< I, T >& operator--();
    AutoMemoryMappedIterator< I, T > operator--( int );

    AutoMemoryMappedIterator< I, T >& operator+=( const difference_type& n );
    AutoMemoryMappedIterator< I, T >& operator-=( const difference_type& n );

    AutoMemoryMappedIterator< I, T >
                                    operator+( const difference_type& n ) const;
    AutoMemoryMappedIterator< I, T >
                                    operator-( const difference_type& n ) const;

    const I& getCurrent() const;
    const AutoMemoryMappedVector< T >* getContainer() const;
    const uint64_t& getIndex() const;

  protected:

    I _current;
    const AutoMemoryMappedVector< T >* _container;
    uint64_t _index;

};


template < class I, class T >
bool operator==( const AutoMemoryMappedIterator< I, T >& i1,
                 const AutoMemoryMappedIterator< I, T >& i2 );
                 
template < class I, class T >
bool operator!=( const AutoMemoryMappedIterator< I, T >& i1,
                 const AutoMemoryMappedIterator< I, T >& i2 );
                 



//
// class AutoMemoryMappedVector< T >
//

template < class T >
class AutoMemoryMappedVector
{

  public:

    typedef uint64_t size_type;
    typedef AutoMemoryMappedIterator< T*, T > iterator;
    typedef AutoMemoryMappedIterator< const T*, T > const_iterator;

    AutoMemoryMappedVector();
    AutoMemoryMappedVector( size_type count );
    AutoMemoryMappedVector( const AutoMemoryMappedVector< T >& other );
    virtual ~AutoMemoryMappedVector();

    AutoMemoryMappedVector< T >& operator=(
                                     const AutoMemoryMappedVector< T >& other );

    size_type getItemCount() const;
    size_type size() const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    T& operator[]( size_type index );
    const T& operator[]( size_type index ) const;

    bool isMemoryMapped() const;

  protected:

    friend class AutoMemoryMappedIterator< T*, T >;
    friend class AutoMemoryMappedIterator< const T*, T >;

    void allocate( size_type count );
    void deallocate();
    T* updatePage( size_type i ) const;

    size_type _itemCount;
    mutable T* _items;
    int _fileDescriptor;

    mutable uint64_t _currentOffset;

};


}


#endif
