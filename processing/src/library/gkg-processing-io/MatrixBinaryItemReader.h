#ifndef _gkg_processing_io_MatrixBinaryItemReader_h_
#define _gkg_processing_io_MatrixBinaryItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class MatrixBinaryItemReader : public ItemReader< Matrix >,
                               public Singleton< MatrixBinaryItemReader >
{

  public:

    virtual ~MatrixBinaryItemReader();

    virtual void read( std::istream& is, Matrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, Matrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, Matrix* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< MatrixBinaryItemReader >;

    MatrixBinaryItemReader();

};


}


#endif
