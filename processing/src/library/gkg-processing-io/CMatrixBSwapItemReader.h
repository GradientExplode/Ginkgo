#ifndef _gkg_processing_io_CMatrixBSwapItemReader_h_
#define _gkg_processing_io_CMatrixBSwapItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>


namespace gkg
{


class CMatrixBSwapItemReader : public ItemReader< CMatrix >,
                               public Singleton< CMatrixBSwapItemReader >
{

  public:

    virtual ~CMatrixBSwapItemReader();

    virtual void read( std::istream& is, CMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, CMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, CMatrix* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< CMatrixBSwapItemReader >;

    CMatrixBSwapItemReader();

};


}


#endif
