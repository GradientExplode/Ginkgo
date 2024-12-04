#ifndef _gkg_processing_transform_AcPcIhCoordinates_h_
#define _gkg_processing_transform_AcPcIhCoordinates_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/FrameTransform3d.h>


namespace gkg
{


class AcPcIhCoordinates : public HeaderedObject
{

  public:

    AcPcIhCoordinates();
    AcPcIhCoordinates( const Vector3d< int32_t >& theIntegerAnteriorCommissure,
                       const Vector3d< int32_t >& theIntegerPosteriorCommissure,
                       const Vector3d< int32_t >& theIntegerInterHemispheric,
                       const Vector3d< double >& resolution );
    AcPcIhCoordinates( const Vector3d< float >& theAnteriorCommissure,
                       const Vector3d< float >& thePosteriorCommissure,
                       const Vector3d< float >& theInterHemispheric,
                       const Vector3d< double >& resolution );
    AcPcIhCoordinates( const Vector3d< int32_t >& theIntegerAnteriorCommissure,
                       const Vector3d< int32_t >& theIntegerPosteriorCommissure,
                       const Vector3d< int32_t >& theIntegerInterHemispheric,
	  	       const Vector3d< float >& theAnteriorCommissure,
                       const Vector3d< float >& thePosteriorCommissure,
                       const Vector3d< float >& theInterHemispheric );
    AcPcIhCoordinates( HeaderedObject& headeredObject );
    virtual ~AcPcIhCoordinates();


    AcPcIhCoordinates& operator=( const AcPcIhCoordinates& other );

    FrameTransform3d< float > getTalairachFrameTransform3d() const;

    void addToHeaderedObject( HeaderedObject& headeredObject ) const;

    static void addSemantic( HeaderedObject& headeredObject );

    Vector3d< int32_t > integerAnteriorCommissure;
    Vector3d< int32_t > integerPosteriorCommissure;
    Vector3d< int32_t > integerInterHemispheric;

    Vector3d< float > anteriorCommissure;
    Vector3d< float > posteriorCommissure;
    Vector3d< float > interHemispheric;

};


}


#endif
