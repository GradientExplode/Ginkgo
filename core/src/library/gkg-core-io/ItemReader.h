#ifndef _gkg_core_io_ItemReader_h_
#define _gkg_core_io_ItemReader_h_


#include <iostream>
#include <gkg-core-cppext/LargeFileStream.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


template < class T >
class ItemReader
{

  public:

    virtual ~ItemReader();    

    virtual void read( std::istream& is, T& item ) const;
    virtual void read( std::istream& is, T* pItem, size_t count ) const = 0;

    virtual void read( largefile_ifstream& is, T& item ) const;
    virtual void read( largefile_ifstream& is, T* pItem,
                       size_t count ) const = 0;

    virtual void read( largefile_fstream& fs, T& item ) const;
    virtual void read( largefile_fstream& fs, T* pItem,
                       size_t count ) const = 0;

    virtual void read( const std::string& s, T& item ) const;
    virtual void read( const std::string& s, T* pItem, size_t count ) const;

  protected:

    ItemReader();

};


}


#endif
