#ifndef _gkg_mri_reconstruction_correction_ModulationTransferFunction_h_
#define _gkg_mri_reconstruction_correction_ModulationTransferFunction_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Volume;


template < class T >
class ModulationTransferFunction : 
                             public Singleton< ModulationTransferFunction< T > >
{

  public:

    ~ModulationTransferFunction();

    void estimate( const Volume< std::complex< T > >& data,
                   std::vector< std::complex< T > >& modulationTF,
                   const T& yScrolling = 0.0,
                   T* outShift = 0,
                   T* outPhase = 0,
                   int32_t* ghostLine = 0 ) const;
    void estimate(
                  const Volume< std::complex< T > >& data,
                  std::vector< std::vector< std::complex< T > > >& modulationTF,
                  const T& yScrolling = 0.0,
                  bool verbose = false,
                  std::vector< T >* outShift = 0,
                  std::vector< T >* outPhase = 0,
                  std::vector< int32_t >* outGhostLine = 0 ) const;

    void apply(
            Volume< std::complex< T > >& data,
            const std::vector< std::vector< std::complex< T > > >& modulationTF,
            int32_t rawLineCount,
            bool verbose ) const;

  private:

    friend class Singleton< ModulationTransferFunction< T > >;

    ModulationTransferFunction();

    int32_t 
      findGhostLine1( const gkg::Volume< std::complex< T > >& image ) const;
    int32_t 
      findGhostLine2( const gkg::Volume< std::complex< T > >& image ) const;
    T getShift( const std::vector< std::complex< T > >& vec, 
                int32_t skip ) const;

};


}


#endif
