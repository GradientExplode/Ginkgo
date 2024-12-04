#ifndef _gkg_dmri_diffusoid_Diffusoid_h_
#define _gkg_dmri_diffusoid_Diffusoid_h_


#include <vector>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Vector3d;
class OrientationSet;
template < class T > class  Transform3d;
template < class R, class S, uint32_t D, class Compare > class MeshMap;
class HeaderedObject;
class Matrix;
class SO3Interpolator;


class Diffusoid
{

  public:

    Diffusoid( const OrientationSet* orientationSet = 0,
               const SO3Interpolator* so3interpolator = 0 );
    Diffusoid( const Diffusoid& other );
    virtual ~Diffusoid();

    Diffusoid& operator=( const Diffusoid& other );

    virtual void setMagnitudes( const std::vector< float >& attenuations );
    virtual void setOrientationSet( const OrientationSet* orientationSet );

    virtual const std::vector< float >& getMagnitudes() const;
    virtual float getMagnitude( int32_t index ) const;
    virtual float getMagnitude( 
                            const Vector3d< float >& unitaryOrientation ) const;

    virtual float getMinimumMagnitude() const;
    virtual float getMaximumMagnitude() const;

    virtual const Vector3d< float >& getMinimumOrientation() const;
    virtual const Vector3d< float >& getMaximumOrientation() const;

    virtual float getStandardDeviation() const;

    template < class R, class Compare >
    void addSymmetricalMesh( const Transform3d< float >& transform,
                             const R& rank,
                             MeshMap< R, float, 3U, Compare >& meshMap ) const;


  protected:

    void getReconstructionMatrix( const HeaderedObject& headeredObject,
                                  const OrientationSet& outputOrientationSet,
                                  Matrix& reconstructionMatrix );

    const OrientationSet* _orientationSet;
    const SO3Interpolator* _so3Interpolator;
    std::vector< float > _magnitudes;

};


}


////////////////////////////////////////////////////////////////////////////////
// creating type name for Diffusoid
////////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::Diffusoid,
                  gkg_Diffusoid );


#endif
