#ifndef _gkg_mri_reconstruction_correction_GradientWarping2dCorrection_h_
#define _gkg_mri_reconstruction_correction_GradientWarping2dCorrection_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-mri-reconstruction-correction/GradientCoil.h>
#include <vector>
#include <complex>


namespace gkg
{


template < class T > class Volume;


template < class T >
class GradientWarping2dCorrection :
                            public Singleton< GradientWarping2dCorrection< T > >
{

  public:


    struct Plane
    {

      Vector3d< T > point1;
      Vector3d< T > point2;
      Vector3d< T > point3;

    };


    struct Wedge2d
    {

      std::vector< float > row;
      std::vector< float > column;

    };



    ~GradientWarping2dCorrection();

    void estimate( const GradientCoil< T >& gradientCoil,
                   const std::vector< Plane >& planes,
                   std::vector< Wedge2d >& wedge2ds,
                   int32_t sizeX,
                   int32_t sizeY,
                   bool verbose = false ) const;
                   
    void apply( Volume< T >& data,
                const std::vector< Wedge2d >& wedge2ds,
                bool swapXY,
                bool verbose = false ) const;

    void apply( Volume< std::complex< T > >& data,
                const std::vector< Wedge2d >& wedge2ds,
                bool swapXY,
                bool verbose = false ) const;

  private:

    friend class Singleton< GradientWarping2dCorrection< T > >;

    GradientWarping2dCorrection();

    void warp2d( const Volume< T >& sliceIn,
                 const std::vector< float >& wedge,
                 const Volume< float >& interpolationTable,
                 const std::vector< float >& power1Table,
                 const std::vector< float >& power2Table,
                 const std::vector< float >& power3Table,
                 const std::vector< float >& power4Table,
                 const std::vector< float >& power5Table,
                 Volume< T >& sliceOut ) const;
    void warp1d( const std::vector< float >& lineIn,
                 const std::vector< float >& params,
                 const Volume< float >& interpolationTable,
                 const std::vector< float >& power1Table,
                 const std::vector< float >& power2Table,
                 const std::vector< float >& power3Table,
                 const std::vector< float >& power4Table,
                 const std::vector< float >& power5Table,
                 std::vector< float >& lineOut ) const;

};


}


#endif
