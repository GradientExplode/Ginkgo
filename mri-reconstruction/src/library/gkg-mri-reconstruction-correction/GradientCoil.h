#ifndef _gkg_mri_reconstruction_correction_GradientCoil_h_
#define _gkg_mri_reconstruction_correction_GradientCoil_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <vector>


#define GRADIENT_COIL_WEDGE_COUNT   21


namespace gkg
{


template < class T >
class GradientCoil
{

  public:

    virtual ~GradientCoil();

    // cosinusoidal harmonic(s)
    void getHarmonicCosinus11( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;
    void getHarmonicCosinus21( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;
    void getHarmonicCosinus31( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;
    void getHarmonicCosinus41( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;
    void getHarmonicCosinus51( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;

    void getHarmonicCosinus10( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;
    void getHarmonicCosinus20( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;
    void getHarmonicCosinus30( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;
    void getHarmonicCosinus40( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;
    void getHarmonicCosinus50( std::vector< float >& wedge,
                               const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               const T& coefficient ) const;

    virtual void getXYZWedges( const Vector3d< T >& phaseVector,
                               const Vector3d< T >& frequencyVector,
                               const Vector3d< T >& point,
                               std::vector< float >& wedgeX,
                               std::vector< float >& wedgeY,
                               std::vector< float >& wedgeZ ) const = 0;


/*
    // sinusoidal harmonic(s)
    void getHarmonicSinus11( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getHarmonicSinus21( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getHarmonicSinus31( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getHarmonicSinus41( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getHarmonicSinus51( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;

    void getHarmonicSinus10( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getHarmonicSinus20( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getHarmonicSinus30( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getHarmonicSinus40( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getHarmonicSinus50( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;

    void getCoefficientX( std::vector< float >& wedge,
                          const Vector3d< T >& phaseVector,
                          const Vector3d< T >& frequencyVector,
                          const Vector3d< T >& point,
                          const T& coefficient ) const;
    void getCoefficientX3( std::vector< float >& wedge,
                           const Vector3d< T >& phaseVector,
                           const Vector3d< T >& frequencyVector,
                           const Vector3d< T >& point,
                           const T& coefficient ) const;
    void getCoefficientXY2( std::vector< float >& wedge,
                            const Vector3d< T >& phaseVector,
                            const Vector3d< T >& frequencyVector,
                            const Vector3d< T >& point,
                            const T& coefficient ) const;
    void getCoefficientXZ2( std::vector< float >& wedge,
                            const Vector3d< T >& phaseVector,
                            const Vector3d< T >& frequencyVector,
                            const Vector3d< T >& point,
                            const T& coefficient ) const;
    void getCoefficientX5( std::vector< float >& wedge,
                           const Vector3d< T >& phaseVector,
                           const Vector3d< T >& frequencyVector,
                           const Vector3d< T >& point,
                           const T& coefficient ) const;
    void getCoefficientXY4( std::vector< float >& wedge,
                            const Vector3d< T >& phaseVector,
                            const Vector3d< T >& frequencyVector,
                            const Vector3d< T >& point,
                            const T& coefficient ) const;
    void getCoefficientXZ4( std::vector< float >& wedge,
                            const Vector3d< T >& phaseVector,
                            const Vector3d< T >& frequencyVector,
                            const Vector3d< T >& point,
                            const T& coefficient ) const;
    void getCoefficientX3Y2( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getCoefficientX3Z2( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient ) const;
    void getCoefficientXY2Z2( std::vector< float >& wedge,
                              const Vector3d< T >& phaseVector,
                              const Vector3d< T >& frequencyVector,
                              const Vector3d< T >& point,
                              const T& coefficient ) const;

    void get5thOrderPolynom( std::vector< float >& wedge,
                             const Vector3d< T >& phaseVector,
                             const Vector3d< T >& frequencyVector,
                             const Vector3d< T >& point,
                             const T& coefficient1,
                             const T& coefficient2,
                             const T& coefficient3,
                             const T& coefficient4,
                             const T& coefficient5,
                             const T& coefficient6,
                             const T& coefficient7,
                             const T& coefficient8,
                             const T& coefficient9,
                             const T& coefficient10 ) const;
*/

  protected:

    GradientCoil();


};


}


#endif


