#ifndef _gkg_processing_transform_ParametricTransform3d_h_
#define _gkg_processing_transform_ParametricTransform3d_h_

#include <gkg-processing-transform/Transform3d.h>
#include <vector>


namespace gkg
{


template <class T>
class ParametricTransform3d : public Transform3d<T>
{

  public:
    
    ParametricTransform3d( int32_t parameterCount );
    ParametricTransform3d( const ParametricTransform3d< T >& other);
    virtual ~ParametricTransform3d();

    ParametricTransform3d< T >& 
                           operator=( const ParametricTransform3d< T >& other );
    
    virtual void setParameters( const std::vector< T >& parameters ) = 0; 
    void getParameters( std::vector< T >& parameters ) const;

  protected:
    
    std::vector< T > _parameters; 

};


}


#endif
