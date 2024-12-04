#ifndef _gkg_processing_io_SparseCMatrixBinaryItemReader_h_
#define _gkg_processing_io_SparseCMatrixBinaryItemReader_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/SparseCMatrix.h>


namespace gkg
{


class SparseCMatrixBinaryItemReader :
                               public ItemReader< SparseCMatrix >,
                               public Singleton< SparseCMatrixBinaryItemReader >
{

  public:

    virtual ~SparseCMatrixBinaryItemReader();

    virtual void read( std::istream& is, SparseCMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_ifstream& is, SparseCMatrix* pItem,
                       size_t count ) const;
    virtual void read( largefile_fstream& fs, SparseCMatrix* pItem,
                       size_t count ) const;

  protected:

    friend class Singleton< SparseCMatrixBinaryItemReader >;

    SparseCMatrixBinaryItemReader();

};


}


#endif
