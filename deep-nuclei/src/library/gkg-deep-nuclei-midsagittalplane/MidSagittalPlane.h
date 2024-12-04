#ifndef _gkg_deep_nuclei_midsagittalplane_MidSagittalPlane_h_
#define _gkg_deep_nuclei_midsagittalplane_MidSagittalPlane_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>

#include <vector>


namespace gkg
{


template < class T > class Volume;


template < class T >
class MidSagittalPlane
{

  public:
  
    MidSagittalPlane( int32_t parameterCount );
    virtual ~MidSagittalPlane();
    
    void compute( const Volume< T >& volumeIn, 
                  Volume< T >& volumeOut, 
		  Volume< T >& mask,
		  bool output3D = false,
		  bool verbose = false );

    std::vector< double > getCoefficients();
    const HomogeneousTransform3d< double >& getTransformation() const;

  private:
  
    void fill2d( const Volume< T >& volumeIn, Volume< T >& volumeOut ) const;
    void fill3d( const Volume< T >& volumeIn, Volume< T >& volumeOut ) const;
  
    int32_t _parameterCount;
    std::vector< double > _coefficients;
    gkg::HomogeneousTransform3d< double > _transformation;
  
};


}


#endif
