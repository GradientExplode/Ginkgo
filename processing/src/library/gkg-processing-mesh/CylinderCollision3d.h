#ifndef _gkg_processing_mesh_CylinderCollision3d_h_
#define _gkg_processing_mesh_CylinderCollision3d_h_


#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{



float getClosestDistanceBetweenLines(
                           const Vector3d< float >& a0,
                           const Vector3d< float >& a1,
                           const Vector3d< float >& b0,
                           const Vector3d< float >& b1,
                           bool clampAll = false,
                           bool clampA0 = false,
                           bool clampA1 = false,
                           bool clampB0 = false,
                           bool clampB1 = false,
                           Vector3d< float >* closestPointOnSegmentA = 0,
                           Vector3d< float >* closestPointOnSegmentB = 0 );



bool hasCylinderCollision3d( const gkg::Vector3d< float >& center1,
                             const gkg::Vector3d< float >& axis1,
                             float halfLength1,
                             float radius1,
                             const gkg::Vector3d< float >& center2,
                             const gkg::Vector3d< float >& axis2,
                             float halfLength2,
                             float radius2 );

}


#endif
