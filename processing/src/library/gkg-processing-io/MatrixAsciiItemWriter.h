#ifndef _gkg_processing_io_MatrixAsciiItemWriter_h_
#define _gkg_processing_io_MatrixAsciiItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class MatrixAsciiItemWriter : public ItemWriter< Matrix >,
                              public Singleton< MatrixAsciiItemWriter >
{

  public:

    virtual ~MatrixAsciiItemWriter();

    virtual void write( std::ostream& os, const Matrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const Matrix* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const Matrix* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< MatrixAsciiItemWriter >;

    MatrixAsciiItemWriter();

};


}


#endif
