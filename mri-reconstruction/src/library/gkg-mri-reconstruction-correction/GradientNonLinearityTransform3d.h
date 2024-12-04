#ifndef _gkg_mri_reconstruction_correction_GradientNonLinearityTransform3d_h_
#define _gkg_mri_reconstruction_correction_GradientNonLinearityTransform3d_h_

#include <gkg-processing-transform/ParametricTransform3d.h>
#include <vector>
#include <iostream>
#include <fstream>


namespace gkg
{

//
//         _inverseParameters[  0 ] ->  aX( 1, 0 )                          
//         _inverseParameters[  1 ] ->  aY( 1, 0 )
//         _inverseParameters[  2 ] ->  aZ( 1, 0 )   = GZ lineaire
//         _inverseParameters[  3 ] ->  bX( 1, 0 )
//         _inverseParameters[  4 ] ->  bY( 1, 0 )
//         _inverseParameters[  5 ] ->  bZ( 1, 0 )   = 0
//         _inverseParameters[  6 ] ->  aX( 1, 1 )   = GX lineaire                      
//         _inverseParameters[  7 ] ->  aY( 1, 1 )   = 0
//         _inverseParameters[  8 ] ->  aZ( 1, 1 )
//         _inverseParameters[  9 ] ->  bX( 1, 1 )   = 0
//         _inverseParameters[ 10 ] ->  bY( 1, 1 )   = GY lineaire
//         _inverseParameters[ 11 ] ->  bZ( 1, 1 )
//         _inverseParameters[ 12 ] ->  aX( 2, 0 )      
//	       
//         etc...
//


class NumericalAnalysisImplementationFactory;


template < class T >
class GradientNonLinearityTransform3d : public ParametricTransform3d< T >
{
  public:
  
    GradientNonLinearityTransform3d( int32_t sphericalHarmonicOrder = 0,
                                     const Vector3d< T >& isocenter = 
                                       Vector3d< T >( 0, 0, 0 ) );
    virtual ~GradientNonLinearityTransform3d();

    RCPointer< Transform3d< T > > clone() const ;

    GradientNonLinearityTransform3d< T >& 
                 operator=( const GradientNonLinearityTransform3d< T >& other );

    int32_t getSphericalHarmonicOrder() const;
    int32_t getSphericalHarmonicCount() const;

    void setIsocenter( const Vector3d< T >& isocenter );

    void setParameters( const std::vector< T >& parameters ); 

    // distorted image --> corrected image
    void getDirect( const Vector3d< T >& from, Vector3d< T >& to ) const;
    // corrected image --> distorted image
    void getInverse( const Vector3d< T >& to, Vector3d< T >& from ) const;
                      
    void readTrm( std::istream& is );
    void writeTrm( std::ostream& os ) const;

    protected:

      void initialize( int32_t sphericalHarmonicOrder );
      Vector3d< T > getFunctionNM( const Vector3d< T >& site,
                                   int32_t n, int32_t m ) const;
      Vector3d< T > getDisplacement( const Vector3d< T >& site ) const;

      NumericalAnalysisImplementationFactory* _factory;

      int32_t _sphericalHarmonicOrder;
      Vector3d< T > _isocenter;
      std::vector< std::vector< gkg::Vector3d< T > > > _a;
      std::vector< std::vector< gkg::Vector3d< T > > > _b; 

};


}


#endif
