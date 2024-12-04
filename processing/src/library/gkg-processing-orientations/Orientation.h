#ifndef _gkg_processing_orientations_Orientation_h_
#define _gkg_processing_orientations_Orientation_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <iostream>


namespace gkg
{


class Orientation
{

  public:

    Orientation( const gkg::Vector3d< float >& other,
                 bool avoidSymmetricOrientations = true );
    Orientation( bool avoidSymmetricOrientations = true );
    virtual ~Orientation();

    const Vector3d< float >& getUnitVector() const;

    float getEnergy( const Orientation& other ) const;

    void addForce( const Orientation& other, float weight );
    void resetForce();

    void move( float step );
    void undo();

  protected:

    bool _avoidSymmetricOrientations;
    Vector3d< float > _unitVector;
    Vector3d< float > _savedUnitVector;
    Vector3d< float > _force;

};


}


namespace std
{


ostream& operator << ( ostream& os, const gkg::Orientation& thing );


}


#endif
