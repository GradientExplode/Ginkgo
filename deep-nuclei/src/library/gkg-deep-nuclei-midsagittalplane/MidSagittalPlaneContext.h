#ifndef _gkg_deep_nuclei_midsagittalplane_MidSagittalPlaneContext_h_
#define _gkg_deep_nuclei_midsagittalplane_MidSagittalPlaneContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/BoundingBox.h>


namespace gkg
{


template < class T >
class MidSagittalPlaneContext : public LoopContext< int32_t >
{

  public:
  
    MidSagittalPlaneContext( const double a,
                             const double b,
                  			     const double c,
                  			     const double d,
		                  	     const double arx,
		                  	     const double bry,
                  			     const double crz,
			                       const Vector3d< double >& resolution,
	                   		     const BoundingBox< int32_t >& boundingBox,
	                  		     const Volume< T >& mask,
			                       double& energy );
			     
    void doIt( int32_t startIndex, int32_t count );
    
  private:
  
    const double _a;
    const double _b;
    const double _c;
    const double _d;
    const double _arx;
    const double _bry;
    const double _crz;
    const Vector3d< double >& _resolution;
    const BoundingBox< int32_t >& _boundingBox;
    const Volume< T >& _mask;
    double& _energy;

};


}


#endif
