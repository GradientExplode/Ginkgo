#ifndef _gkg_processing_io_CMatrixBSwapItemWriter_h_
#define _gkg_processing_io_CMatrixBSwapItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>


namespace gkg
{


class CMatrixBSwapItemWriter : public ItemWriter< CMatrix >,
                               public Singleton< CMatrixBSwapItemWriter >
{

  public:

    virtual ~CMatrixBSwapItemWriter();

    virtual void write( std::ostream& os, const CMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const CMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const CMatrix* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< CMatrixBSwapItemWriter >;

    CMatrixBSwapItemWriter();

};


}


#endif
