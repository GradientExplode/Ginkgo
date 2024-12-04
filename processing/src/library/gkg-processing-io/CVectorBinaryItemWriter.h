#ifndef _gkg_processing_io_CVectorBinaryItemWriter_h_
#define _gkg_processing_io_CVectorBinaryItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CVector.h>


namespace gkg
{


class CVectorBinaryItemWriter : public ItemWriter< CVector >,
                                public Singleton< CVectorBinaryItemWriter >
{

  public:

    virtual ~CVectorBinaryItemWriter();

    virtual void write( std::ostream& os, const CVector* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const CVector* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const CVector* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< CVectorBinaryItemWriter >;

    CVectorBinaryItemWriter();

};


}


#endif
