#ifndef _NewAutoMemoryMappedVector_h_
#define _NewAutoMemoryMappedVector_h_


#include <gkg-core-cppext/StdInt.h>
#include <iterator>


namespace gkg
{


template < class T > class NewAutoMemoryMappedVector;


//
// class NewAutoMemoryMappedIterator< I, T >
//

template < class I, class T >
class NewAutoMemoryMappedIterator
{

  public:

    typedef I iterator_type;
    typedef typename std::iterator_traits< I >::iterator_category
                                                              iterator_category;
    typedef typename std::iterator_traits< I >::value_type value_type;
    typedef typename std::iterator_traits< I >::difference_type difference_type;
    typedef typename std::iterator_traits< I >::reference reference;
    typedef typename std::iterator_traits< I >::pointer pointer;


    NewAutoMemoryMappedIterator();
    NewAutoMemoryMappedIterator( const NewAutoMemoryMappedVector< T >* container,
                              uint64_t index );
    template < class J >
    NewAutoMemoryMappedIterator( const NewAutoMemoryMappedIterator< J, T >& other );

    reference operator*() const;
    pointer operator->() const;

    NewAutoMemoryMappedIterator< I, T >& operator++();
    NewAutoMemoryMappedIterator< I, T > operator++( int );

    NewAutoMemoryMappedIterator< I, T >& operator--();
    NewAutoMemoryMappedIterator< I, T > operator--( int );

    NewAutoMemoryMappedIterator< I, T >& operator+=( const difference_type& n );
    NewAutoMemoryMappedIterator< I, T >& operator-=( const difference_type& n );

    NewAutoMemoryMappedIterator< I, T >
                                    operator+( const difference_type& n ) const;
    NewAutoMemoryMappedIterator< I, T >
                                    operator-( const difference_type& n ) const;

    const I& getCurrent() const;
    const NewAutoMemoryMappedVector< T >* getContainer() const;
    const uint64_t& getIndex() const;

  protected:

    I _current;
    const NewAutoMemoryMappedVector< T >* _container;
    uint64_t _index;

};


template < class I, class T >
bool operator==( const NewAutoMemoryMappedIterator< I, T >& i1,
                 const NewAutoMemoryMappedIterator< I, T >& i2 );
                 
template < class I, class T >
bool operator!=( const NewAutoMemoryMappedIterator< I, T >& i1,
                 const NewAutoMemoryMappedIterator< I, T >& i2 );
                 



//
// class NewAutoMemoryMappedVector< T >
//

template < class T >
class NewAutoMemoryMappedVector
{

  public:

    typedef uint64_t size_type;
    typedef NewAutoMemoryMappedIterator< T*, T > iterator;
    typedef NewAutoMemoryMappedIterator< const T*, T > const_iterator;

    NewAutoMemoryMappedVector();
    NewAutoMemoryMappedVector( size_type count );
    NewAutoMemoryMappedVector( const NewAutoMemoryMappedVector< T >& other );
    virtual ~NewAutoMemoryMappedVector();

    NewAutoMemoryMappedVector< T >& operator=(
                                     const NewAutoMemoryMappedVector< T >& other );

    size_type getItemCount() const;
    size_type size() const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    T& operator[]( size_type index );
    const T& operator[]( size_type index ) const;

    bool isMemoryMapped() const;
    void setVerbose();


  protected:

    friend class NewAutoMemoryMappedIterator< T*, T >;
    friend class NewAutoMemoryMappedIterator< const T*, T >;

    void allocate( size_type count );
    void deallocate();
    T* updatePage( size_type i ) const;

    size_type _itemCount;
    mutable T* _items;
    int _fileDescriptor;

    mutable uint64_t _currentOffset;

    bool _verbose;

};


}


#endif
