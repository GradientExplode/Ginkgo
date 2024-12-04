#ifndef _gkg_processing_io_CVectorAsciiItemReader_h_
#define _gkg_processing_io_CVectorAsciiItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CVector.h>


namespace gkg
{


class CVectorAsciiItemReader : public ItemReader< CVector >,
                               public Singleton< CVectorAsciiItemReader >
{

  public:

    virtual ~CVectorAsciiItemReader();

    virtual void read( std::istream& is, CVector* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, CVector* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, CVector* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< CVectorAsciiItemReader >;

    CVectorAsciiItemReader();

};


}


#endif
