#ifndef _gkg_dmri_sh_basis_SymmetricalSphericalHarmonicsCache_h_
#define _gkg_dmri_sh_basis_SymmetricalSphericalHarmonicsCache_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-communication-thread/Mutex.h>
#include <vector>


namespace gkg
{


class OrientationSet;


class SymmetricalSphericalHarmonicsCache :
                          public Singleton< SymmetricalSphericalHarmonicsCache >
{

  public:

    ~SymmetricalSphericalHarmonicsCache();

    void update( const OrientationSet& orientationSet,
                 int32_t symmetricalSHCoefficientCount );

    const std::vector< std::vector< float > >& getCoefficients() const;


  protected:

    friend class Singleton< SymmetricalSphericalHarmonicsCache >;

    SymmetricalSphericalHarmonicsCache();

    const OrientationSet* _orientationSet;
    int32_t _orientationCount;
    int32_t _symmetricalSHCoefficientCount;
    std::vector< std::vector< float > > _coefficients;
    Mutex _mutex;

};



}



#endif
