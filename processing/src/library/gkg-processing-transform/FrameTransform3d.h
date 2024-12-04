#ifndef _gkg_processing_transform_FrameTransform3d_h_
#define _gkg_processing_transform_FrameTransform3d_h_

#include <gkg-processing-transform/CompositeTransform3d.h>
#include <vector>

namespace gkg
{

template < class T > class Translation3d;
template < class T > class HomogeneousTransform3d;

template < class T >
class FrameTransform3d : public CompositeTransform3d< T >
{

  public:
  
    FrameTransform3d( const Vector3d< T >& vectorU,
                      const Vector3d< T >& vectorV,
                      const Vector3d< T >& vectorW,
                      const Vector3d< T >& origin,
                      bool direct = true );
    FrameTransform3d( const T& uX = 0.0, const T& uY = 0.0, const T& uZ = 0.0,
                      const T& vX = 0.0, const T& vY = 0.0, const T& vZ = 0.0,
                      const T& wX = 0.0, const T& wY = 0.0, const T& wZ = 0.0,
                      const T& originX = 0.0,
                      const T& originY = 0.0,
                      const T& originZ = 0.0,
                      bool direct = true );
    FrameTransform3d( const std::vector< T >& parameters,
                      bool direct = true );
    FrameTransform3d( const FrameTransform3d< T >& other );
    virtual ~FrameTransform3d();
    
    virtual RCPointer< Transform3d< T > > clone() const;
    
    FrameTransform3d< T >& operator=( const FrameTransform3d< T >& other );
    
    void setDirectParameters( const Vector3d< T >& vectorU,
                              const Vector3d< T >& vectorV,
                              const Vector3d< T >& vectorW,
                              const Vector3d< T >& origin,
                              bool updating = true,
                              bool notify = true );
    void setDirectParameters( const T& uX, const T& uY, const T& uZ,
                              const T& vX, const T& vY, const T& vZ,
                              const T& wX, const T& wY, const T& wZ,
                              const T& originX,
                              const T& originY,
                              const T& originZ,
                              bool  updating = true,
                              bool notify = true );
    void setDirectParameters( const std::vector< T >& parameters,
                              bool updating = true,
                              bool notify = true );   

    void getDirectParameters( Vector3d< T >& vectorU,
                              Vector3d< T >& vectorV,
                              Vector3d< T >& vectorW,
                              Vector3d< T >& origin ) const;
    void getDirectParameters( T& uX, T& uY, T& uZ,
                              T& vX, T& vY, T& vZ,
                              T& wX, T& wY, T& wZ,
                              T& originX,
                              T& originY,
                              T& originZ ) const;
    void getDirectParameters( std::vector< T >& parameters ) const;   
     
    void setInverseParameters( const Vector3d< T >& vectorU,
                               const Vector3d< T >& vectorV,
                               const Vector3d< T >& vectorW,
                               const Vector3d< T >& origin,
                               bool updating = true,
                               bool notify = true );
    void setInverseParameters( const T& uX, const T& uY, const T& uZ,
                               const T& vX, const T& vY, const T& vZ,
                               const T& wX, const T& wY, const T& wZ,
                               const T& originX,
                               const T& originY,
                               const T& originZ,
                               bool  updating = true,
                               bool notify = true );
    void setInverseParameters( const std::vector< T >& parameters,
                               bool updating = true,
                               bool notify = true ); 

    void getInverseParameters( Vector3d< T >& vectorU,
                               Vector3d< T >& vectorV,
                               Vector3d< T >& vectorW,
                               Vector3d< T >& origin ) const;
    void getInverseParameters( T& uX, T& uY, T& uZ,
                               T& vX, T& vY, T& vZ ,
                               T& wX, T& wY, T& wZ,
                               T& originX,
                               T& originY,
                               T& originZ ) const;
    void getInverseParameters( std::vector< T >& parameters ) const; 
    
    const HomogeneousTransform3d< T >& getFrame3d() const;
    const Translation3d< T >& getTranslation3d() const; 
    
  protected:
  
    RCPointer< HomogeneousTransform3d< T > > _frame3d;
    RCPointer< Translation3d< T > > _translation3d;
    
};

}

#endif

  
    
    
 
