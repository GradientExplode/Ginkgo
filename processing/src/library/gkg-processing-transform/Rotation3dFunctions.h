#ifndef _gkg_processing_transform_Rotation3dFunctions_h_
#define _gkg_processing_transform_Rotation3dFunctions_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


template < class T >
Vector3d< T > getXYZAnglesFromTrieder( const Vector3d< T >& u,
                                       const Vector3d< T >& v,
                                       const Vector3d< T >& w,
                                       bool degree = false );

template < class T >
void getXYZScalingsAndTrieder( const Matrix& rotationAndScaling,
                               Vector3d< T >& scalings,
                               Vector3d< T >& u,
                               Vector3d< T >& v,
                               Vector3d< T >& w );

template < class T >
void getXYZScalingsAndShearingsAndTrieder(
                               const Matrix& rotationAndScalingAndShearings,
                               Vector3d< T >& scalings,
                               Vector3d< T >& shearings,
                               Vector3d< T >& u,
                               Vector3d< T >& v,
                               Vector3d< T >& w );
			       
template < class T >
void getRandomOrthonormalTriederFromFirstAxis( const Vector3d< T >& firstAxis,
                                               Vector3d< T >& secondAxis,
                                               Vector3d< T >& thirdAxis );
						


}



#endif
