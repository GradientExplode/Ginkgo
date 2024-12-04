#ifndef _gkg_processing_io_CVectorBSwapItemWriter_h_
#define _gkg_processing_io_CVectorBSwapItemWriter_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/CVector.h>


namespace gkg
{


class CVectorBSwapItemWriter : public ItemWriter< CVector >,
                               public Singleton< CVectorBSwapItemWriter >
{

  public:

    virtual ~CVectorBSwapItemWriter();

    virtual void write( std::ostream& os, const CVector* pItem,
                        size_t count ) const;
    virtual void write( largefile_ofstream& os, const CVector* pItem,
                        size_t count ) const;
    virtual void write( largefile_fstream& fs, const CVector* pItem,
                        size_t count ) const;

  protected:

    friend class Singleton< CVectorBSwapItemWriter >;

    CVectorBSwapItemWriter();

};


}


#endif
