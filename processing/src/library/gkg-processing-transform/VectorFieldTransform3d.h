#ifndef _gkg_processing_transform_VectorFieldTransform3d_h_
#define _gkg_processing_transform_VectorFieldTransform3d_h_


#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-resampling/QuickResampler.h>


namespace gkg
{


template < class T >
class VectorFieldTransform3d : public Transform3d< T >
{

  public:

    VectorFieldTransform3d( const Volume< T >& directVectorField,
                            const Volume< T >& inverseVectorField );
    VectorFieldTransform3d( const VectorFieldTransform3d< T >& other );
    virtual ~VectorFieldTransform3d();

    RCPointer< Transform3d< T > > clone() const;

    void getDirect( const Vector3d< T >& from, 
                    Vector3d< T >& to ) const;
    void getInverse( const Vector3d< T >& to, 
                     Vector3d< T >& from ) const;

    void setDirectAndInverseVectorFields( const Volume< T >& directVectorField,
                                          const Volume< T >& inverseVectorField,
                                          bool update = true,
                                          bool notify = true );
    void setDirectVectorField( const Volume< T >& directVectorField,
                               bool update = true,
                               bool notify = true );
    void setInverseVectorField( const Volume< T >& inverseVectorField,
                                bool update = true,
                                bool notify = true );

    void setDirectAndInverseVectorFields(
                                      const Volume< T >& directFlowFieldAlongX,
                                      const Volume< T >& directFlowFieldAlongY,
                                      const Volume< T >& directFlowFieldAlongZ,
                                      const Volume< T >& inverseFlowFieldAlongX,
                                      const Volume< T >& inverseFlowFieldAlongY,
                                      const Volume< T >& inverseFlowFieldAlongZ,
                                      bool update = true,
                                      bool notify = true );
    void setDirectVectorField( const Volume< T >& directFlowFieldAlongX,
                               const Volume< T >& directFlowFieldAlongY,
                               const Volume< T >& directFlowFieldAlongZ,
                               bool update = true,
                               bool notify = true );
    void setInverseVectorField( const Volume< T >& inverseFlowFieldAlongX,
                                const Volume< T >& inverseFlowFieldAlongY,
                                const Volume< T >& inverseFlowFieldAlongZ,
                                bool update = true,
                                bool notify = true );

    void getDirectAndInverseVectorFields(
                                        Volume< T >& directVectorField,
                                        Volume< T >& inverseVectorField ) const;
    void getDirectVectorField( Volume< T >& directVectorField ) const;
    void getInverseVectorField( Volume< T >& inverseVectorField ) const;

  protected:

    Volume< T > _directVectorField;
    Volume< T > _inverseVectorField;
    Vector3d< double > _directResolution;
    Vector3d< double > _inverseResolution;
    QuickResampler< T >* _resampler;

};


}


#endif
