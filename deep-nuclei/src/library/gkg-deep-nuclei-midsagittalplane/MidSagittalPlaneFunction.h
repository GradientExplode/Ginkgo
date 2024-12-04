#ifndef _gkg_deep_nuclei_midsagittalplane_MidSagittalPlaneFunction_h_
#define _gkg_deep_nuclei_midsagittalplane_MidSagittalPlaneFunction_h_

#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/BoundingBox.h>


namespace gkg
{


template < class T >
class MidSagittalPlaneFunction : public NelderMeadSimplexFunction
{

  public:

    MidSagittalPlaneFunction( int32_t parameterCount,
                              const Vector3d< double >& point1,
			      const Vector3d< double >& point2,
			      const Vector3d< double >& point3,
			      const Volume< T >& mask );

    double getValueAt( const Vector& parameters ) const;

  private:

    Vector3d< double > _point2;
    Vector3d< double > _point21;
    Vector3d< double > _point31;
    Volume< T > _mask;
    BoundingBox< int32_t > _boundingBox;
    
};

    
}


#endif
