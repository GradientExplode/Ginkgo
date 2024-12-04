#ifndef _gkg_mri_reconstruction_correction_BandPassAsymmetryCorrection_h_
#define _gkg_mri_reconstruction_correction_BandPassAsymmetryCorrection_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;


template < class T >
class BandPassAsymmetryCorrection :
                            public Singleton< BandPassAsymmetryCorrection< T > >
{

  public:

    ~BandPassAsymmetryCorrection();

    void apply( Volume< std::complex< T > >& data,
                int32_t rawSizeX, int32_t rawSizeY,
                const T& receiverFrequencyStart,
                const T& receiverFrequencyEnd ) const;

  private:

    friend class Singleton< BandPassAsymmetryCorrection< T > >;

    BandPassAsymmetryCorrection();

};


}


#endif
