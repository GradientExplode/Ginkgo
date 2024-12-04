#ifndef _gkg_processing_io_CVectorBinaryItemReader_h_
#define _gkg_processing_io_CVectorBinaryItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CVector.h>


namespace gkg
{


class CVectorBinaryItemReader : public ItemReader< CVector >,
                                public Singleton< CVectorBinaryItemReader >
{

  public:

    virtual ~CVectorBinaryItemReader();

    virtual void read( std::istream& is, CVector* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, CVector* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, CVector* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< CVectorBinaryItemReader >;

    CVectorBinaryItemReader();

};


}


#endif
