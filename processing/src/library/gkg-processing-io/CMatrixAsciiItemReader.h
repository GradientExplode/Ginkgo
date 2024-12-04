#ifndef _gkg_processing_io_CMatrixAsciiItemReader_h_
#define _gkg_processing_io_CMatrixAsciiItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>


namespace gkg
{


class CMatrixAsciiItemReader : public ItemReader< CMatrix >,
                               public Singleton< CMatrixAsciiItemReader >
{

  public:

    virtual ~CMatrixAsciiItemReader();

    virtual void read( std::istream& is, CMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, CMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, CMatrix* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< CMatrixAsciiItemReader >;

    CMatrixAsciiItemReader();

};


}


#endif
