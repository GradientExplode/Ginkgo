#ifndef _gkg_processing_transform_ParameterizedAffineTransform3d_h_
#define _gkg_processing_transform_ParameterizedAffineTransform3d_h_


#include <gkg-processing-transform/ParameterizedTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>


namespace gkg
{


//
//    parameters[ 0 ] -> X scaling
//    parameters[ 1 ] -> Y scaling
//    parameters[ 2 ] -> Z scaling
//    parameters[ 3 ] -> XY shearing
//    parameters[ 4 ] -> XZ shearing
//    parameters[ 5 ] -> YZ shearing
//    parameters[ 6 ] -> X rotation angle in radians
//    parameters[ 7 ] -> Y rotation angle in radians
//    parameters[ 8 ] -> Z rotation angle in radians
//    parameters[ 9 ] -> X translation in millimeters
//    parameters[ 10 ] -> Y translation in millimeters
//    parameters[ 11 ] -> Z translation in millimeters
//

template < class T >
class ParameterizedAffineTransform3d : public ParameterizedTransform3d< T >
{

  public:

    ParameterizedAffineTransform3d();
    ParameterizedAffineTransform3d( 
         const AffineTransform3d< T >& affineTransform3d );
    ParameterizedAffineTransform3d(
         const ParameterizedAffineTransform3d< T >& other );
    virtual ~ParameterizedAffineTransform3d();

    int32_t getParameterCount() const;

    RCPointer< Transform3d< T > > clone() const;

    ParameterizedAffineTransform3d< T >& operator=(
         const ParameterizedAffineTransform3d< T >& other );

    const AffineTransform3d< T >& getAffineTransform3d() const;
    AffineTransform3d< T >& getAffineTransform3d();

    void getDirectParameters( std::vector< T >& parameters ) const;
    void getInverseParameters( std::vector< T >& parameters ) const;

    void readTrm( std::istream& is );
    void writeTrm( std::ostream& os ) const;

  protected:

    void setDirectParametersImpl( const std::vector< T >& parameters );
    void setInverseParametersImpl( const std::vector< T >& parameters );

    AffineTransform3d< T >* _affineTransform3d;

};


}


#endif
