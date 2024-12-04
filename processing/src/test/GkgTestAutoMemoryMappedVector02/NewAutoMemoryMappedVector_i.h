#ifndef _NewAutoMemoryMappedVector_i_h_
#define _NewAutoMemoryMappedVector_i_h_


#include "NewAutoMemoryMappedVector.h"
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-cppext/CUniStd.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <cerrno>

#if ( defined(__APPLE__) && defined(__MACH__) )

#define ftruncate64 ftruncate

#endif

//
// class NewAutoMemoryMappedIterator< I, T >
//

template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T >::NewAutoMemoryMappedIterator()
                                      : _current( 0 ),
                                        _container( 0 ),
                                        _index( 0U )
{
}


template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T >::NewAutoMemoryMappedIterator(
                              const gkg::NewAutoMemoryMappedVector< T >* container,
                              uint64_t index )
                                      : _current(
                                               container->updatePage( index ) ),
                                        _container( container ),
                                        _index( index )
{
}


template < class I, class T > template < class J >
inline
gkg::NewAutoMemoryMappedIterator< I, T >::NewAutoMemoryMappedIterator(
                            const gkg::NewAutoMemoryMappedIterator< J, T >& other )
                                      : _current( other.getCurrent() ),
                                        _container( other.getContainer() ),
                                        _index( other.getIndex() )
{
}


template < class I, class T >
inline
typename gkg::NewAutoMemoryMappedIterator< I, T >::reference 
gkg::NewAutoMemoryMappedIterator< I, T >::operator*() const
{

  return *_current;

}


template < class I, class T >
inline
typename gkg::NewAutoMemoryMappedIterator< I, T >::pointer 
gkg::NewAutoMemoryMappedIterator< I, T >::operator->() const
{

  return _current;

}


template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T >& 
gkg::NewAutoMemoryMappedIterator< I, T >::operator++()
{

  ++ _index;
  _current = _container->updatePage( _index );

  return *this;

}


template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T > 
gkg::NewAutoMemoryMappedIterator< I, T >::operator++( int )
{

  gkg::NewAutoMemoryMappedIterator< I, T > i( _container,
                                           _index );
  ++ _index;
  _current = _container->updatePage( _index );

  return i;

}


template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T >& 
gkg::NewAutoMemoryMappedIterator< I, T >::operator--()
{

  -- _index;
  _current = _container->updatePage( _index );

  return *this;

}


template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T > 
gkg::NewAutoMemoryMappedIterator< I, T >::operator--( int )
{

  gkg::NewAutoMemoryMappedIterator< I, T > i( _container,
                                           _index );
  -- _index;
  _current = _container->updatePage( _index );

  return i;

}


template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T >& 
gkg::NewAutoMemoryMappedIterator< I, T >::operator+=(
      const typename gkg::NewAutoMemoryMappedIterator< I, T >::difference_type& n )
{

  _index += n;
  _current = _container->updatePage( _index );

  return *this;

}


template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T >& 
gkg::NewAutoMemoryMappedIterator< I, T >::operator-=(
      const typename gkg::NewAutoMemoryMappedIterator< I, T >::difference_type& n )
{

  _index -= n;
  _current = _container->updatePage( _index );

  return *this;

}


template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T >
gkg::NewAutoMemoryMappedIterator< I, T >::operator+(
const typename gkg::NewAutoMemoryMappedIterator< I, T >::difference_type& n ) const
{

  return gkg::NewAutoMemoryMappedIterator< I, T >( _container,
                                                _index + n );

}


template < class I, class T >
inline
gkg::NewAutoMemoryMappedIterator< I, T >
gkg::NewAutoMemoryMappedIterator< I, T >::operator-(
const typename gkg::NewAutoMemoryMappedIterator< I, T >::difference_type& n ) const
{

  return gkg::NewAutoMemoryMappedIterator< I, T >( _container,
                                                _index - n );

}


template < class I, class T >
inline
const I& gkg::NewAutoMemoryMappedIterator< I, T >::getCurrent() const
{

  return _current;

}


template < class I, class T >
inline
const gkg::NewAutoMemoryMappedVector< T >* 
gkg::NewAutoMemoryMappedIterator< I, T >::getContainer() const
{

  return _container;

}


template < class I, class T >
inline
const uint64_t& gkg::NewAutoMemoryMappedIterator< I, T >::getIndex() const
{

  return _index;

}


template < class I, class T >
inline
bool gkg::operator==( const gkg::NewAutoMemoryMappedIterator< I, T >& i1,
                      const gkg::NewAutoMemoryMappedIterator< I, T >& i2 )
{

  return ( i1.getIndex() == i2.getIndex() );

}

                 
template < class I, class T >
inline
bool gkg::operator!=( const gkg::NewAutoMemoryMappedIterator< I, T >& i1,
                      const gkg::NewAutoMemoryMappedIterator< I, T >& i2 )
{

  return ( i1.getIndex() != i2.getIndex() );

}


//
// class NewAutoMemoryMappedVector< T >
//

template < class T >
inline
gkg::NewAutoMemoryMappedVector< T >::NewAutoMemoryMappedVector()
                                 : _itemCount( 0U ),
                                   _items( 0 ),
                                   _fileDescriptor( -1 ),
                                   _currentOffset( 0U ),
                                   _verbose( false )
{
}


template < class T >
inline
gkg::NewAutoMemoryMappedVector< T >::NewAutoMemoryMappedVector( 
                    typename gkg::NewAutoMemoryMappedVector< T >::size_type count )
{

  try
  {

    // allocating with the item count
    allocate( count );
    _verbose = false;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::NewAutoMemoryMappedVector< T >::NewAutoMemoryMappedVector( "
             "typename gkg::NewAutoMemoryMappedVector< T >::size_type count )" );

}


template < class T >
inline
gkg::NewAutoMemoryMappedVector< T >::NewAutoMemoryMappedVector(
                                 const gkg::NewAutoMemoryMappedVector< T >& other )
{

  try
  {

    if ( other.getItemCount() == 0U )
    {

      _itemCount = 0U;
      _items = 0;
      _fileDescriptor = -1;
      _currentOffset = 0U;
      _verbose = false;

    }
    else
    {

      // allocating with the other item count
      allocate( other.getItemCount() );
      _verbose = other._verbose;

      // copying items
      typename gkg::NewAutoMemoryMappedVector< T >::iterator
        i = this->begin(),
        ie = this->end();
      typename gkg::NewAutoMemoryMappedVector< T >::const_iterator
        o = other.begin();
      while ( i != ie )
      {

        *i = *o;
        ++ i;
        ++ o;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::NewAutoMemoryMappedVector< T >::NewAutoMemoryMappedVector( "
             "const gkg::NewAutoMemoryMappedVector< T >& other )" )

}


template < class T >
inline
gkg::NewAutoMemoryMappedVector< T >::~NewAutoMemoryMappedVector()
{

  // deallocating previously allocated items
  deallocate();

}


template < class T >
inline
gkg::NewAutoMemoryMappedVector< T >&
gkg::NewAutoMemoryMappedVector< T >::operator=(
                              const gkg::NewAutoMemoryMappedVector< T >& other )
{

  try
  {

    if ( &other != this )
    {

      // Reallocate items only if vector sizes are different
      if ( this->size() != other.size() )
      {

        // deallocating previously allocated items
        deallocate();

        // allocating with the good item count
        allocate( other.getItemCount() );

      }

      if( this->isMemoryMapped() || other.isMemoryMapped() )
      {

        // copying items
        typename gkg::NewAutoMemoryMappedVector< T >::iterator
          i = this->begin(),
          ie = this->end();
        typename gkg::NewAutoMemoryMappedVector< T >::const_iterator
          o = other.begin();
        while ( i != ie )
        {

          *i = *o;
          ++ i;
          ++ o;

        }

      }
      else
      {

        memcpy( &( *this->begin() ), &( *other.begin() ),
                other.getItemCount() * sizeof( T ) );

      }

    }
    return *this;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::NewAutoMemoryMappedVector< T >& "
             "gkg::NewAutoMemoryMappedVector< T >::operator=( "
             "const gkg::NewAutoMemoryMappedVector< T >& other )" );

}


template < class T >
inline
typename gkg::NewAutoMemoryMappedVector< T >::size_type 
gkg::NewAutoMemoryMappedVector< T >::getItemCount() const
{

  return _itemCount;

}


template < class T >
inline
typename gkg::NewAutoMemoryMappedVector< T >::size_type 
gkg::NewAutoMemoryMappedVector< T >::size() const
{

  return _itemCount;

}


template < class T >
inline
typename gkg::NewAutoMemoryMappedVector< T >::iterator 
gkg::NewAutoMemoryMappedVector< T >::begin()
{

  return typename gkg::NewAutoMemoryMappedVector< T >::iterator( this,
                                                              0U );

}


template < class T >
inline
typename gkg::NewAutoMemoryMappedVector< T >::iterator 
gkg::NewAutoMemoryMappedVector< T >::end()
{

  return typename gkg::NewAutoMemoryMappedVector< T >::iterator( this,
                                                              _itemCount );

}


template < class T >
inline
typename gkg::NewAutoMemoryMappedVector< T >::const_iterator 
gkg::NewAutoMemoryMappedVector< T >::begin() const
{

  return typename gkg::NewAutoMemoryMappedVector< T >::const_iterator( this,
                                                                    0U );

}


template < class T >
inline
typename gkg::NewAutoMemoryMappedVector< T >::const_iterator 
gkg::NewAutoMemoryMappedVector< T >::end() const
{

  return typename gkg::NewAutoMemoryMappedVector< T >::const_iterator(
                                                                   this,
                                                                   _itemCount );

}


template < class T >
inline
T& gkg::NewAutoMemoryMappedVector< T >::operator[]( 
                    typename gkg::NewAutoMemoryMappedVector< T >::size_type index )
{

  try
  {

    if ( isMemoryMapped() )
    {

      return *updatePage( index );

    }
    return _items[ index ];

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T& gkg::NewAutoMemoryMappedVector< T >::operator[]( "
             "typename gkg::NewAutoMemoryMappedVector< T >::size_type index )" );

}


template < class T >
inline
const T& gkg::NewAutoMemoryMappedVector< T >::operator[]( 
              typename gkg::NewAutoMemoryMappedVector< T >::size_type index ) const
{

  try
  {

    if ( isMemoryMapped() )
    {

      return *updatePage( index );

    }
    return _items[ index ];

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const T& gkg::NewAutoMemoryMappedVector< T >::operator[]( "
             "typename gkg::NewAutoMemoryMappedVector< T >::size_type index ) "
             "const" );

}


template < class T >
inline
bool gkg::NewAutoMemoryMappedVector< T >::isMemoryMapped() const
{

  return _fileDescriptor != -1;

}


template < class T >
inline
void gkg::NewAutoMemoryMappedVector< T >::setVerbose()
{

  _verbose = true;

}


template < class T >
inline
void gkg::NewAutoMemoryMappedVector< T >::allocate(
                    typename gkg::NewAutoMemoryMappedVector< T >::size_type count )
{

  try
  {

    // collecting memory mapping page size (default is 1 MBytes)
    static uint64_t memoryMappingPageSize =
      gkg::ConfigurationInfo::getInstance().getMemoryMappingPageSize();

    // collecting the security factor set on memory allocation in RAM
    static double memoryMappingSwitchOnFactor =
      gkg::ConfigurationInfo::getInstance().getMemoryMappingSwitchOnFactor();

    // collecting memory mapping verbose parameter
    static bool memoryMappingVerbose =
      gkg::ConfigurationInfo::getInstance().isMemoryMappingVerbose();

    // collecting memory mapping disabled parameter
    static bool memoryMappingDisabled =
      gkg::ConfigurationInfo::getInstance().isMemoryMappingDisabled();

    // processing the required amount of memory
    uint64_t requiredMemory = ( uint64_t )count * ( uint64_t )sizeof( T );

    // getting information about the available free memory
    uint64_t ram = 0;
    uint64_t freeRam = 0;
    uint64_t swap = 0;
    gkg::SystemInfo::getInstance().getMemory( ram, freeRam, swap );

    // getting information of the process memory usage and limits
    uint64_t processMemoryUsage =
                         gkg::SystemInfo::getInstance().getProcessMemoryUsage();
    uint64_t maximumMemoryPerProcess =
                    gkg::SystemInfo::getInstance().getMaximumMemoryPerProcess();

    if ( memoryMappingVerbose )
    {

      std::cout << "required memory = "
                << requiredMemory
                << " / RAM = "
                << ram
                << " / free RAM = "
                << freeRam
                << " / process RAM usage = "
                << processMemoryUsage
                << " / maximum RAM per process = "
                << maximumMemoryPerProcess
                << " -> " << std::flush;

    }

    // if required memory is greater than safe free RAM or future free RAM would
    // be lower than the safe maximum allocated memory, then use memory mapping
    if ( !memoryMappingDisabled &&
         ( ( requiredMemory >
           ( uint64_t )( ( double )freeRam * memoryMappingSwitchOnFactor ) ) ||
         ( ( freeRam - requiredMemory ) <
           ( uint64_t )( ( double )ram *
                         ( 1.0 - memoryMappingSwitchOnFactor ) ) ) ||
         ( ( processMemoryUsage + requiredMemory ) > maximumMemoryPerProcess ) )
       )
    {

      // creating a filename in the memory mapping path
/*      std::string name =
        gkg::ConfigurationInfo::getInstance().getMemoryMappingPath() +
        "gkg_auto_memory_mapped_vector" +
        gkg::TypeOf< T >::getName() +
        "_XXXXXX";
*/
      char name[512] = "/tmp/gkg_auto_memory_mapped_vector_XXXXXX";

      // creating the temporary memory mapped file
      _fileDescriptor = mkstemp( ( char* )name/*.c_str()*/ );

      std::cout << "=====================>mapping name : " << name << std::endl;


      // if the file was not successfully created
      if ( _fileDescriptor == -1 )
      {


        _items = 0;
        _itemCount = 0;
        _currentOffset = 0U;
        throw std::runtime_error( "failed to open a file for memory mapping" );

      }
      // if successfully created the file
      else
      {

        // unlinking file to remove it automatically after deallocating it
        unlink( name/*.c_str()*/ );

        // if cannot change the file permissions
        if ( fchmod( _fileDescriptor, S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH ) ==
             -1 )
        {

          close( _fileDescriptor );
          _items = 0;
          _itemCount = 0;
          _currentOffset = 0;
          throw std::runtime_error( "could not change file permissions" );

        }
        // if successfully changed the file permissions
        else
        {

          // if the file could not be truncated to the good size
          if ( ::ftruncate64( _fileDescriptor, ( uint64_t )requiredMemory ) ==
               - 1 )
          {

            close( _fileDescriptor );
            _items = 0;
            _itemCount = 0;
            _currentOffset = 0;
            throw std::runtime_error( "could not truncate file" );

          }
          // if the file was successfully truncated to the good size
          else
          {

            char* buffer = ( char* )mmap( 0U,
                                          ( size_t )std::min(
                                              memoryMappingPageSize,
                                              count * sizeof( T ) ),
                                          PROT_READ | PROT_WRITE,
                                          MAP_SHARED,
                                          _fileDescriptor,
                                          ( off_t )0U );
            // if memory mapping was successful
            if ( buffer != MAP_FAILED )
            {

              _items = ( T* )buffer;
              _itemCount = ( uint64_t )count;
              _currentOffset = 0;

              if ( memoryMappingVerbose )
              {

                std::cout << "memory mapped" << std::endl;

              }

            }
            // if memory mapping was not successful
            else
            {


              close( _fileDescriptor );
              _items = 0;
              _itemCount = 0;
              _currentOffset = 0;

              // try to get the reason of for defect ... is it a memory lack ?
              int err = errno;
              if ( err == ENOMEM )
              {

                throw std::runtime_error(
                  "could not memory map the opened file (not enough memory)" );

              }
              else
              {


                throw std::runtime_error(
                  "could not memory map the opened file" );

              }

            }

          }

        }

      }

    }
    else if( memoryMappingDisabled &&
       ( ( requiredMemory > freeRam ) ||
       ( ( processMemoryUsage + requiredMemory ) > maximumMemoryPerProcess ) ) )
    {

      _items = 0;
      _itemCount = 0;
      _currentOffset = 0;
      _fileDescriptor = -1;

      // Cannot allocate new vector : not enough RAM and memory mapping disabled
      throw std::runtime_error( "could not allocate items in memory:"
                                " not enough RAM and memory mapping disabled" );

    }
    // if required memory is lower than safe free RAM, then use RAM memory
    else
    {

      _items = new T[ count ];
      _itemCount = count;
      _fileDescriptor = -1;
      _currentOffset = 0;

      // if memory allocating was not successful
      if ( !_items )
      {

        _itemCount = 0;
        _currentOffset = 0;
        throw std::runtime_error( "could not allocate items in memory" );

      }
      else
      {

        uint64_t i = 0U;
        for ( i = 0U; i < _itemCount; i++ )
        {

          _items[ i ] = T();

        }
        if ( memoryMappingVerbose )
        {

          std::cout << "allocated in physical memory" << std::endl;

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::NewAutoMemoryMappedVector< T >::allocate( "
             "typename gkg::NewAutoMemoryMappedVector< T >::size_type count )" );

}


template < class T >
inline
void gkg::NewAutoMemoryMappedVector< T >::deallocate()
{

  try
  {

    // collecting memory mapping page size (default is 1 MBytes)
    //static uint64_t memoryMappingPageSize =
      gkg::ConfigurationInfo::getInstance().getMemoryMappingPageSize();

    // in case of allocation in memory
    if ( _fileDescriptor != -1 )
    {

      // unmapping the mapped file
/*
      munmap( _items, std::min( memoryMappingPageSize,
                                _itemCount * sizeof( T ) ) );
      close( _fileDescriptor );*/
      _items = 0;
      _itemCount = 0;
      _fileDescriptor = -1;
      _currentOffset = 0;

    }
    else
    {

      // deleting the allocated items
      delete [] _items;
      _items = 0;
      _itemCount = 0;
      _fileDescriptor = -1;
      _currentOffset = 0;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::NewAutoMemoryMappedVector< T >::deallocate()" );

}


template < class T >
inline
T* gkg::NewAutoMemoryMappedVector< T >::updatePage(
              typename gkg::NewAutoMemoryMappedVector< T >::size_type index ) const
{

  try
  {

    // collecting memory mapping page size (default is 1 MBytes)
    static uint64_t memoryMappingPageSize =
      gkg::ConfigurationInfo::getInstance().getMemoryMappingPageSize();


    // when end iterator is initialized, its index is set to the item count
    // and this case must be considered specifically
    if ( index == _itemCount )
    {

      return 0;

    }

    ////////////////////////// in physical memory //////////////////////////////

    // return directly the pointer to the corresponding index
    if ( !isMemoryMapped() )
    {

      return _items + index;

    }

    ///////////////////////////// memory mapped ////////////////////////////////

    // in case the required memory size is not bigger than the page size, we
    // know that we don't have to remap the next page
    if ( _itemCount * sizeof( T ) <= memoryMappingPageSize )
    {

      return _items + ( index - _currentOffset );

    }


    // processing page and offset corresponding to the provided index
    uint64_t newPage = ( index * sizeof( T ) ) / memoryMappingPageSize;
    uint64_t newOffset = newPage * memoryMappingPageSize / sizeof( T );
    uint64_t newByteOffset = newPage * memoryMappingPageSize;

    if ( newOffset != _currentOffset )
    {

      if ( _verbose )
      {

        std::cout << "j'update la page pour l'index " << index << std::flush;

      }


      // unmapping the previous page
      munmap( ( char* )_items, ( off_t )memoryMappingPageSize );

      // and mapping the new one
      char* buffer = ( char* )mmap( 0U,
                                    ( size_t )memoryMappingPageSize,
                                    PROT_READ | PROT_WRITE,
                                    MAP_SHARED,
                                    _fileDescriptor,
                                    ( off_t )newByteOffset );

      // if memory mapping was successful
      if ( buffer != MAP_FAILED )
      {

        _items = ( T* )buffer;
        _currentOffset = newOffset;

      }
      else
      {

        throw std::runtime_error( "failed to memory map the new page" );

      }
 
    }

    return _items + ( index - _currentOffset );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T* gkg::NewAutoMemoryMappedVector< T >::updatePage( "
             "typename gkg::NewAutoMemoryMappedVector< T >::size_type index )" );

}


namespace gkg
{

template <>
inline
NewAutoMemoryMappedVector< Vector3d< float > >&
NewAutoMemoryMappedVector< Vector3d< float > >::operator=(
                   const NewAutoMemoryMappedVector< Vector3d< float > >& other )
{

  try
  {

    std::cout << "BisAutoMem:op= 1" << std::endl;
    if ( &other != this )
    {

      std::cout << "BisAutoMem:op= 2" << std::endl;
      // Reallocate items only if vector sizes are different
      if ( this->size() != other.size() )
      {

        std::cout << "BisAutoMem:op= 3" << std::endl;

        // deallocating previously allocated items
        deallocate();

        std::cout << "BisAutoMem:op= 4" << std::endl;

        // allocating with the good item count
        allocate( other.getItemCount() );

        std::cout << "BisAutoMem:op= 5" << std::endl;

      }

      if( this->isMemoryMapped() || other.isMemoryMapped() )
      {

        std::cout << "this->isMemoryMapped() : " << this->isMemoryMapped() << std::endl;
        std::cout << "this->getItemCount() : " << this->getItemCount() << std::endl;
        std::cout << "other.isMemoryMapped() : " << other.isMemoryMapped() << std::endl;
        std::cout << "other.getItemCount() : " << other.getItemCount() << std::endl;
        std::cout << "BisAutoMem:op= 6" << std::endl;

        // copying items
        NewAutoMemoryMappedVector< Vector3d< float > >::iterator
          i = this->begin(),
          ie = this->end();
        NewAutoMemoryMappedVector< Vector3d< float > >::const_iterator
          o = other.begin(),
          oe = other.end();
        std::cout << "BisAutoMem:op= 7" << std::endl;

        uint64_t count = 0U;

        while ( ( i != ie ) && ( o != oe ) )
        {

          //std::cout << "*i[ " << count << "] = " << *i << std::endl;
          //std::cout << "*o[ " << count << "] = " << *o << std::endl;
          *i = *o;
          ++ i;
          ++ o;
          ++ count;

        }
        std::cout << "BisAutoMem:op= 8" << std::endl;

      }
      else
      {

        std::cout << "BisAutoMem:op= 9" << std::endl;

        memcpy( (void*)&( *this->begin() ), (const void*)&( *other.begin() ),
                other.getItemCount() * sizeof( Vector3d< float > ) );
        std::cout << "BisAutoMem:op= 10" << std::endl;

      }

      std::cout << "BisAutoMem:op= 11" << std::endl;

    }
    std::cout << "BisAutoMem:op= 12" << std::endl;

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::AutoMemoryMappedVector< T >& "
             "gkg::AutoMemoryMappedVector< T >::operator=( "
             "const gkg::AutoMemoryMappedVector< T >& other )" );

}


}



#endif
