#ifndef _gkg_processing_transform_ParameterizedAffineWoShearingTransform3d_h_
#define _gkg_processing_transform_ParameterizedAffineWoShearingTransform3d_h_


#include <gkg-processing-transform/ParameterizedTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>


namespace gkg
{


//
//    parameters[ 0 ] -> X scaling
//    parameters[ 1 ] -> Y scaling
//    parameters[ 2 ] -> Z scaling
//    parameters[ 3 ] -> X rotation angle in radians
//    parameters[ 4 ] -> Y rotation angle in radians
//    parameters[ 5 ] -> Z rotation angle in radians
//    parameters[ 6 ] -> X translation in millimeters
//    parameters[ 7 ] -> Y translation in millimeters
//    parameters[ 8 ] -> Z translation in millimeters
//

template < class T >
class ParameterizedAffineWoShearingTransform3d :
                                           public ParameterizedTransform3d< T >
{

  public:

    ParameterizedAffineWoShearingTransform3d();
    ParameterizedAffineWoShearingTransform3d(
         const AffineWoShearingTransform3d< T >& affineWoShearingTransform3d );
    ParameterizedAffineWoShearingTransform3d(
         const ParameterizedAffineWoShearingTransform3d< T >& other );
    virtual ~ParameterizedAffineWoShearingTransform3d();

    int32_t getParameterCount() const;

    RCPointer< Transform3d< T > > clone() const;

    ParameterizedAffineWoShearingTransform3d< T >& operator=(
         const ParameterizedAffineWoShearingTransform3d< T >& other );

    const AffineWoShearingTransform3d< T >&
      getAffineWoShearingTransform3d() const;
    AffineWoShearingTransform3d< T >&
      getAffineWoShearingTransform3d();

    void getDirectParameters( std::vector< T >& parameters ) const;
    void getInverseParameters( std::vector< T >& parameters ) const;

    void readTrm( std::istream& is );
    void writeTrm( std::ostream& os ) const;

  protected:

    void setDirectParametersImpl( const std::vector< T >& parameters );
    void setInverseParametersImpl( const std::vector< T >& parameters );

    AffineWoShearingTransform3d< T >* _affineWoShearingTransform3d;

};


}


#endif
