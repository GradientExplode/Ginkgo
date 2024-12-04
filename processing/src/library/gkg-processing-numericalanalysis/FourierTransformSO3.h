#ifndef _gkg_processing_signal_FourierTransformSO3_h_
#define _gkg_processing_signal_FourierTransformS03_h_


#include <vector>
#include <complex>


namespace gkg
{


class FourierTransformSO3
{

  public:

    FourierTransformSO3( int32_t bandwidth );
    virtual ~FourierTransformSO3();

    void applyDirect( const std::vector< std::complex< double > >& input,
                      const std::vector< std::complex< double > >& output,
                      bool verbose = false ) const;
    void applyDirect( const std::vector< double >& input,
                      const std::vector< std::complex< double > >& output,
                      bool verbose = false ) const;

    void applyInverse( const std::vector< std::complex< double > >& input,
                       const std::vector< std::complex< double > >& output,
                       bool verbose = false ) const;
    void applyInverse( const std::vector< double >& input,
                       const std::vector< std::complex< double > >& output,
                       bool verbose = false ) const;

  protected:

    int32_t _bandwidth;
    std::vector< double > _workspace1;
    std::vector< double > _workspace2;

};


}


#endif
