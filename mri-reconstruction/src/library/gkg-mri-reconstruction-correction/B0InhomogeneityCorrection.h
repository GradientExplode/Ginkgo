#ifndef _gkg_mri_reconstruction_correction_B0InhomogeneityCorrection_h_
#define _gkg_mri_reconstruction_correction_B0InhomogeneityCorrection_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;


template < class T >
class B0InhomogeneityCorrection :
                              public Singleton< B0InhomogeneityCorrection< T > >
{

  public:

    enum Method
    {

      PixelShiftMethod,
      PixelShiftIntensityCorrectedMethod,
      PhaseConjugateMethod

    };

    ~B0InhomogeneityCorrection();

    void apply( Volume< std::complex< T > >& data,
                const Volume< T >& shiftMap,
                Method method,
                bool verbose = false ) const;

  private:

    friend class Singleton< B0InhomogeneityCorrection< T > >;

    B0InhomogeneityCorrection();

    void applyPixelShiftMethod( Volume< std::complex< T > >& data,
                                const Volume< T >& shiftMap,
                                bool verbose ) const;
    void applyPixelShiftIntensityCorrectedMethod(
                                Volume< std::complex< T > >& data,
                                const Volume< T >& shiftMap,
                                bool verbose ) const;
    void applyPhaseConjugateMethod( Volume< std::complex< T > >& data,
                                    const Volume< T >& shiftMap,
                                    bool verbose ) const;

};


}


#endif
