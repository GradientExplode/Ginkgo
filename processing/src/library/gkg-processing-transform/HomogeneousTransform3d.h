#ifndef _gkg_processing_transform_HomogeneousTransform3d_h_
#define _gkg_processing_transform_HomogeneousTransform3d_h_


#include <gkg-processing-transform/Transform3d.h>
#include <vector>
#include <iostream>
#include <fstream>


namespace gkg
{


template < class T >
class HomogeneousTransform3d : public Transform3d< T > 
{

  public:

    HomogeneousTransform3d();
    HomogeneousTransform3d( const HomogeneousTransform3d< T >& other );
    HomogeneousTransform3d( T d00, T d01, T d02, T d03,
                            T d10, T d11, T d12, T d13,
                            T d20, T d21, T d22, T d23,
                            T d30, T d31, T d32, T d33,
                            bool direct = true );
    HomogeneousTransform3d( const std::vector< T >& directCoefficients,
                            bool direct = true );
    virtual ~HomogeneousTransform3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    virtual bool operator==( const HomogeneousTransform3d< T >& other );

    const T& getDirectCoefficient( int32_t i, int32_t j ) const;
    const T& getInverseCoefficient( int32_t i, int32_t j ) const;

    void setDirectCoefficient( int32_t i, int32_t j, 
                               T value,
                               bool update = true,
                               bool notify = true );
    void setInverseCoefficient( int32_t i, int32_t j, 
                                T value,
                                bool update = true,
                                bool notify = true );

    std::vector< T > getDirectCoefficients() const;
    std::vector< T > getInverseCoefficients() const;

    virtual void getDirect( const Vector3d< T >& from, 
                            Vector3d< T >& to ) const;
    virtual void getInverse( const Vector3d< T >& to, 
                             Vector3d< T >& from ) const;

    bool isComposable() const;

    HomogeneousTransform3d< T >&
      operator=( const HomogeneousTransform3d< T >& other );

    // carreful : T1.compose( T2 ) = T2 x T1
    HomogeneousTransform3d< T > 
      compose( const HomogeneousTransform3d< T >& other ) const;

    void setIdentity( bool update = true, bool notify = true );

    HomogeneousTransform3d< T > getInverseHomogeneousTransform3d() const;

    void setDirectItems( T d00, T d01, T d02, T d03,
                         T d10, T d11, T d12, T d13,
                         T d20, T d21, T d22, T d23,
                         T d30, T d31, T d32, T d33,
                         bool update = true,
                         bool notify = true );
    void setInverseItems( T i00, T i01, T i02, T i03,
                          T i10, T i11, T i12, T i13,
                          T i20, T i21, T i22, T i23,
                          T i30, T i31, T i32, T i33,
                          bool update = true,
                          bool notify = true );
    void setDirectItems( T items[ 4 ][ 4 ],
                         bool update = true,
                         bool notify = true );
    void setInverseItems( T items[ 4 ][ 4 ],
                          bool update = true,
                          bool notify = true );

    void readTrm( std::istream& is );
    void writeTrm( std::ostream& os ) const;

    bool isIndirect() const;

  protected:

    T _directItems[ 4 ][ 4 ];
    T _inverseItems[ 4 ][ 4 ];

    void copy( const HomogeneousTransform3d< T >& other );

};


}


namespace std
{

template < class T >
std::ostream& operator << ( ostream& os,
                            const gkg::HomogeneousTransform3d< T >& thing );

}


#endif
