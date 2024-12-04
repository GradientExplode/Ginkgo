#ifndef _gkg_processing_io_MatrixAsciiItemReader_h_
#define _gkg_processing_io_MatrixAsciiItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class MatrixAsciiItemReader : public ItemReader< Matrix >,
                              public Singleton< MatrixAsciiItemReader >
{

  public:

    virtual ~MatrixAsciiItemReader();

    virtual void read( std::istream& is, Matrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, Matrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, Matrix* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< MatrixAsciiItemReader >;

    MatrixAsciiItemReader();

};


}


#endif
