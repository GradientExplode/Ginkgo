#ifndef _gkg_dmri_sh_basis_PolarHarmonicsCache_h_
#define _gkg_dmri_sh_basis_PolarHarmonicsCache_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-qspace-sampling/MultipleShellQSpaceSampling.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-communication-thread/Mutex.h>
#include <map>
#include <vector>
#include <complex>


namespace gkg
{


class OrientationSet;


class PolarHarmonicsCache :
                          public Singleton< PolarHarmonicsCache >
{

  public:

    ~PolarHarmonicsCache();

    void update( int32_t order );

    int32_t getOrder() const;
    int32_t getCoefficientCount() const;
    const std::vector< Vector3d< int32_t > >& getBasisIndices() const;
    const std::vector< float >& getSqrtOfFactorialOfN1N2N3s() const;
    const std::vector< float >& getBN1N2N3s() const;

    float getRSpaceBasisFunction3dAt(
                   const std::vector< Vector3d< float > >& rotatedDisplacements,
                   const Vector3d< float >& scaling,
                   int32_t displacementIndex,
                   int32_t basisIndex ) const;

    float getQSpaceBasisFunction3dAt(
                   const std::vector< Vector3d< float > >& rotatedQSpacePoints,
                   const Vector3d< float >& scaling,
                   int32_t qSpaceIndex,
                   int32_t basisIndex ) const;

    double getTmn( int32_t m, int32_t n, float u, float v ) const;

  protected:

    friend class Singleton< PolarHarmonicsCache >;

    PolarHarmonicsCache();

    void computeBasisIndices();
    void computeSqrtOfFactorialOfN1N2N3s();
    void computeBN1N2N3s();

    float getRSpaceBasisFunction1dXAt(
                   int32_t order,
                   float scaling,
                   const std::vector< Vector3d< float > >& rotatedDisplacements,
                   int32_t displacementIndex ) const;
    float getRSpaceBasisFunction1dYAt(
                   int32_t order,
                   float scaling,
                   const std::vector< Vector3d< float > >& rotatedDisplacements,
                   int32_t displacementIndex ) const;
    float getRSpaceBasisFunction1dZAt(
                   int32_t order,
                   float scaling,
                   const std::vector< Vector3d< float > >& rotatedDisplacements,
                   int32_t displacementIndex ) const;

    std::complex< float > getQSpaceBasisFunction1dXAt(
                   int32_t order,
                   float scaling,
                   const std::vector< Vector3d< float > >& rotatedQSpacePoints,
                   int32_t qSpaceIndex ) const;
    std::complex< float > getQSpaceBasisFunction1dYAt(
                   int32_t order,
                   float scaling,
                   const std::vector< Vector3d< float > >& rotatedQSpacePoints,
                   int32_t qSpaceIndex ) const;
    std::complex< float > getQSpaceBasisFunction1dZAt(
                   int32_t order,
                   float scaling,
                   const std::vector< Vector3d< float > >& rotatedQSpacePoints,
                   int32_t qSpaceIndex ) const;
    double getDoubleFactorialFunction( double r ) const;

    int32_t _order;
    int32_t _coefficientCount;

    NumericalAnalysisImplementationFactory* _factory;

    std::vector< Vector3d< int32_t > > _basisIndices;
    std::vector< float > _sqrtOfFactorialOfN1N2N3s;
    std::vector< float > _bN1N2N3s;

    Mutex _mutex;

};



}



#endif
