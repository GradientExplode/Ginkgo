#ifndef _gkg_core_io_ItemWriter_h_
#define _gkg_core_io_ItemWriter_h_


#include <iostream>
#include <gkg-core-cppext/LargeFileStream.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


template < class T >
class ItemWriter
{

  public:

    virtual ~ItemWriter();

    virtual void write( std::ostream& os, const T& item ) const;
    virtual void write( std::ostream& os, const T* pItem,
                        size_t count ) const = 0;

    virtual void write( largefile_ofstream& os, const T& item ) const;
    virtual void write( largefile_ofstream& os, const T* pItem,
                        size_t count ) const = 0;

    virtual void write( largefile_fstream& fs, const T& item ) const;
    virtual void write( largefile_fstream& fs, const T* pItem,
                        size_t count ) const = 0;

  protected:

    ItemWriter();

};


}


#endif
