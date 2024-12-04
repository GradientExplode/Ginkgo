#ifndef _gkg_processing_io_CMatrixBinaryItemWriter_h_
#define _gkg_processing_io_CMatrixBinaryItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>


namespace gkg
{


class CMatrixBinaryItemWriter : public ItemWriter< CMatrix >,
                                public Singleton< CMatrixBinaryItemWriter >
{

  public:

    virtual ~CMatrixBinaryItemWriter();

    virtual void write( std::ostream& os, const CMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const CMatrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const CMatrix* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< CMatrixBinaryItemWriter >;

    CMatrixBinaryItemWriter();

};


}


#endif
