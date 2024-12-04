#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-simulation-virtual-tissue/AtomMethodFactory.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::EllipsoidAtom::EllipsoidAtom( const gkg::Vector3d< float >& center, 
                                   float radius1,
                                   float radius2,
                                   float radius3,
                                   const gkg::Vector3d< float >& axis1, 
                                   const gkg::Vector3d< float >& axis2, 
                                   const gkg::Vector3d< float >& axis3,
                                   bool compressedRepresentation )
                   : gkg::Atom()
{

  try
  {

    if ( compressedRepresentation )
    {

      this->parameters.resize( 10U );

    }
    else
    {

      this->parameters.resize( 15U );

    }

    getVectorRepresentationFromStdParameters( center,
                                              radius1,
                                              radius2,
                                              radius3,
                                              axis1,
                                              axis2,
                                              axis3,
                                              &parameters[ 0 ],
                                              compressedRepresentation );

  }
  GKG_CATCH( "gkg::EllipsoidAtom::EllipsoidAtom( "
             "const gkg::Vector3d< float >& center, "
             "float radius1, "
             "float radius2, "
             "float radius3, "
             "const gkg::Vector3d< float >& axis1, "
             "const gkg::Vector3d< float >& axis2, "
             "const gkg::Vector3d< float >& axis3, "
             "bool compressedRepresentation )" );

}


gkg::EllipsoidAtom::EllipsoidAtom( 
                               const std::vector< float >& vectorRepresentation,
                               bool compressedRepresentation ) 
                   : gkg::Atom()
{

  try
  {
  
    if ( vectorRepresentation.size() == 10U )
    {
    
      this->parameters = vectorRepresentation;
      if ( compressedRepresentation == false )
      {

        switchRepresentation();

      }
    
    }
    else if ( vectorRepresentation.size() == 15U )
    {
    
      this->parameters = vectorRepresentation;
      if ( compressedRepresentation == true )
      {

        switchRepresentation();

      }
    
    }
    else
    {

      throw std::runtime_error( "bad vector representation size" );

    }

  }
  GKG_CATCH( "gkg::EllipsoidAtom::EllipsoidAtom( "
             "const std::vector< float >& vectorRepresentation, "
             "bool compressedRepresentation )" );
}


gkg::EllipsoidAtom::EllipsoidAtom( bool compressedRepresentation )
                   : gkg::Atom()
{

  try
  {

    if ( compressedRepresentation )
    {

      this->parameters.resize( 10U, 0.0f );

    }
    else
    {

      this->parameters.resize( 15U, 0.0f );

    }

  }
  GKG_CATCH( "gkg::EllipsoidAtom::EllipsoidAtom( "
             "bool compressedRepresentation )" );
}


gkg::EllipsoidAtom::EllipsoidAtom( const gkg::EllipsoidAtom& other )
                   : gkg::Atom( other )
{
}


gkg::EllipsoidAtom::~EllipsoidAtom()
{
}


gkg::EllipsoidAtom& 
gkg::EllipsoidAtom::operator=( const gkg::EllipsoidAtom& other )
{

  try
  {

    if ( other.hasCompressedRepresentation() !=
         this->hasCompressedRepresentation() )
    {

      throw std::runtime_error( "ellipsoids have different representations" );

    }
    this->gkg::Atom::operator=( other );

    return *this;

  }
  GKG_CATCH( "gkg::EllipsoidAtom& "
             "gkg::EllipsoidAtom::operator=( "
             "const gkg::EllipsoidAtom& other )" );

}


gkg::Atom* gkg::EllipsoidAtom::clone() const
{

  try
  {

    return new gkg::EllipsoidAtom( *this );

  }
  GKG_CATCH( "gkg::Atom* gkg::EllipsoidAtom::clone() const" );

}


bool gkg::EllipsoidAtom::hasCompressedRepresentation() const
{

  try
  {

    return ( parameters.size() == 10U );

  }
  GKG_CATCH( "bool gkg::EllipsoidAtom::hasCompressedRepresentation() const" );


}


void gkg::EllipsoidAtom::switchRepresentation()
{

  try
  {

    gkg::Vector3d< float > center;
    float radius1 = 0.0f;
    float radius2 = 0.0f;
    float radius3 = 0.0f;
    gkg::Vector3d< float > axis1;
    gkg::Vector3d< float > axis2;
    gkg::Vector3d< float > axis3;

    getStdParameters( center,
                      radius1,
                      radius2,
                      radius3,
                      axis1,
                      axis2,
                      axis3 );

    if ( hasCompressedRepresentation() )
    {


      this->parameters.resize( 15U );

      this->parameters[ 0 ] = center.x;
      this->parameters[ 1 ] = center.y; 
      this->parameters[ 2 ] = center.z;
  
      this->parameters[ 3 ] = radius1; 
      this->parameters[ 4 ] = radius2; 
      this->parameters[ 5 ] = radius3;

      this->parameters[ 6 ] = axis1.x; 
      this->parameters[ 7 ] = axis1.y;
      this->parameters[ 8 ] = axis1.z;

      this->parameters[ 9 ] = axis2.x; 
      this->parameters[ 10 ] = axis2.y;
      this->parameters[ 11 ] = axis2.z;

      this->parameters[ 12 ] = axis3.x;
      this->parameters[ 13 ] = axis3.y;
      this->parameters[ 14 ] = axis3.z;

    }
    else
    {

      std::vector< float > compressedVectorRepresentation( 10 );

      getVectorRepresentationFromStdParameters(
                                         center,
                                         radius1,
                                         radius2,
                                         radius3,
                                         axis1,
                                         axis2,
                                         axis3,
                                         &compressedVectorRepresentation[ 0 ],
                                         true );
      this->parameters = compressedVectorRepresentation;

    }

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::switchRepresentation()" );

}


uint8_t gkg::EllipsoidAtom::getType() const
{

  try
  {

    return gkg::EllipsoidAtom::getStaticType();

  }
  GKG_CATCH( "uint8_t gkg::EllipsoidAtom::getType() const" );

}


void gkg::EllipsoidAtom::setCenter( const Vector3d< float >& center )
{

  try
  {

    if ( hasCompressedRepresentation() )
    {
      // setting the center to the origin first
      this->parameters[ 6 ] = 0.f;
      this->parameters[ 7 ] = 0.f;
      this->parameters[ 8 ] = 0.f;
      this->parameters[ 9 ] = -1.f;

      // translating the ellipsoid to the center
      gkg::Matrix translation( 4, 4 );
      translation.setIdentity();
      translation( 0, 3 ) = -center.x;
      translation( 1, 3 ) = -center.y;
      translation( 2, 3 ) = -center.z;

      gkg::Matrix matrixRepresentation( 4, 4 );

      getMatrixRepresentationFromVectorRepresentation(
                                                  &parameters[ 0 ],
                                                  true,
                                                  matrixRepresentation );

      gkg::Matrix 
        translationTimesMatrixRepresentation = translation.getComposition(
                                                          matrixRepresentation );
      gkg::Matrix transposedTranslationTimesMatrixRepresentation =
        translationTimesMatrixRepresentation.getTransposition();

      gkg::Matrix newMatrixRepresentation = 
          translationTimesMatrixRepresentation.getComposition( 
                                matrixRepresentation ).getComposition(
                                transposedTranslationTimesMatrixRepresentation );

      getVectorRepresentationFromMatrixRepresentation( newMatrixRepresentation,
                                                       &parameters[ 0 ],
                                                       true );
    }
    else
    {

      this->parameters[ 0 ] = center.x;
      this->parameters[ 1 ] = center.y;
      this->parameters[ 2 ] = center.z;

    }

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::setCenter( "
             "const Vector3d< float >& center )" );


}


gkg::Vector3d< float > gkg::EllipsoidAtom::getCenter() const
{

  try
  {

    gkg::Vector3d< float > center;

    // theoretically, one could call the getStdParameters() function for both
    // compressed and uncompressed representations, but here, it remains more
    // efficient for uncompressed version not to read all the other parameters
    // and to explicitly fill the center directly from parameters[]
    if ( hasCompressedRepresentation() )
    {

      float radius1 = 0.0f;
      float radius2 = 0.0f;
      float radius3 = 0.0f;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis2;
      gkg::Vector3d< float > axis3;
    
      this->getStdParameters( center,
                              radius1,
                              radius2,
                              radius3,
                              axis1,
                              axis2,
                              axis3 );

    }
    else
    {

      center.x = this->parameters[ 0 ];
      center.y = this->parameters[ 1 ];
      center.z = this->parameters[ 2 ];

    }

    return center;

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::EllipsoidAtom::getCenter() const" );

}


float gkg::EllipsoidAtom::getSpanRadius() const
{

  try
  {


    gkg::Vector3d< float > radii = this->getRadii();

    return std::max( std::max( radii.x, radii.y ), radii.z );

  }
  GKG_CATCH( "float gkg::EllipsoidAtom::getSpanRadius() const" );

}


float gkg::EllipsoidAtom::getRadius1() const
{

  try
  {

    float radius1 = 0.0f;

    // theoretically, one could call the getStdParameters() function for both
    // compressed and uncompressed representations, but here, it remains more
    // efficient for uncompressed version not to read all the other parameters
    // and to explicitly fill the radius1 directly from parameters[]
    if ( hasCompressedRepresentation() )
    {

      gkg::Vector3d< float > center;
      float radius2 = 0.0f;
      float radius3 = 0.0f;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis2;
      gkg::Vector3d< float > axis3;
    
      this->getStdParameters( center,
                              radius1,
                              radius2,
                              radius3,
                              axis1,
                              axis2,
                              axis3 );

    }
    else
    {

      radius1 = this->parameters[ 3 ];

    }

    return radius1;

  }
  GKG_CATCH( "float gkg::EllipsoidAtom::getRadius1() const" );

}


float gkg::EllipsoidAtom::getRadius2() const
{

  try
  {

    float radius2 = 0.0f;

    // theoretically, one could call the getStdParameters() function for both
    // compressed and uncompressed representations, but here, it remains more
    // efficient for uncompressed version not to read all the other parameters
    // and to explicitly fill the radius2 directly from parameters[]
    if ( hasCompressedRepresentation() )
    {

     gkg::Vector3d< float > center;
      float radius1 = 0.0f;
      float radius3 = 0.0f;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis2;
      gkg::Vector3d< float > axis3;
    
      this->getStdParameters( center,
                              radius1,
                              radius2,
                              radius3,
                              axis1,
                              axis2,
                              axis3 );

    }
    else
    {

      radius2 = this->parameters[ 4 ];

    }

    return radius2;

  }
  GKG_CATCH( "float gkg::EllipsoidAtom::getRadius2() const" );

}


float gkg::EllipsoidAtom::getRadius3() const
{

  try
  {

    float radius3 = 0.0f;

    // theoretically, one could call the getStdParameters() function for both
    // compressed and uncompressed representations, but here, it remains more
    // efficient for uncompressed version not to read all the other parameters
    // and to explicitly fill the radius3 directly from parameters[]
    if ( hasCompressedRepresentation() )
    {

     gkg::Vector3d< float > center;
      float radius1 = 0.0f;
      float radius2 = 0.0f;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis2;
      gkg::Vector3d< float > axis3;
    
      this->getStdParameters( center,
                              radius1,
                              radius2,
                              radius3,
                              axis1,
                              axis2,
                              axis3 );

    }
    else
    {

      radius3 = this->parameters[ 5 ];

    }

    return radius3;

  }
  GKG_CATCH( "float gkg::EllipsoidAtom::getRadius3() const" );

}


gkg::Vector3d< float > 
gkg::EllipsoidAtom::getRadii() const
{

  try
  {

    gkg::Vector3d< float > radii;
 
    // theoretically, one could call the getStdParameters() function for both
    // compressed and uncompressed representations, but here, it remains more
    // efficient for uncompressed version not to read all the other parameters
    // and to explicitly fill the radii directly from parameters[]
    if ( hasCompressedRepresentation() )
    {

      gkg::Vector3d< float > center;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis2;
      gkg::Vector3d< float > axis3;
    
      this->getStdParameters( center,
                              radii.x,
                              radii.y,
                              radii.z,
                              axis1,
                              axis2,
                              axis3 );

    }
    else
    {

      radii.x = this->parameters[ 3 ];
      radii.y = this->parameters[ 4 ];
      radii.z = this->parameters[ 5 ];

    }

    return radii;

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::EllipsoidAtom::getRadii() const" );

}


gkg::Vector3d< float > 
gkg::EllipsoidAtom::getAxis1() const
{

  try
  {

    gkg::Vector3d< float > axis1;

    // theoretically, one could call the getStdParameters() function for both
    // compressed and uncompressed representations, but here, it remains more
    // efficient for uncompressed version not to read all the other parameters
    // and to explicitly fill the axis1 directly from parameters[]
    if ( hasCompressedRepresentation() )
    {

      gkg::Vector3d< float > center;
      float radius1 = 0.0f;
      float radius2 = 0.0f;
      float radius3 = 0.0f;
      gkg::Vector3d< float > axis2;
      gkg::Vector3d< float > axis3;
    
      this->getStdParameters( center,
                              radius1,
                              radius2,
                              radius3,
                              axis1,
                              axis2,
                              axis3 );

    }
    else
    {

      axis1.x = this->parameters[ 6 ];
      axis1.y = this->parameters[ 7 ];
      axis1.z = this->parameters[ 8 ];

    }

    return axis1;

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::EllipsoidAtom::getAxis1() const" );

}


gkg::Vector3d< float > 
gkg::EllipsoidAtom::getAxis2() const
{

  try
  {

    gkg::Vector3d< float > axis2;

    // theoretically, one could call the getStdParameters() function for both
    // compressed and uncompressed representations, but here, it remains more
    // efficient for uncompressed version not to read all the other parameters
    // and to explicitly fill the axis2 directly from parameters[]
    if ( hasCompressedRepresentation() )
    {

      gkg::Vector3d< float > center;
      float radius1 = 0.0f;
      float radius2 = 0.0f;
      float radius3 = 0.0f;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis3;
    
      this->getStdParameters( center,
                              radius1,
                              radius2,
                              radius3,
                              axis1,
                              axis2,
                              axis3 );

    }
    else
    {

      axis2.x = this->parameters[ 9 ];
      axis2.y = this->parameters[ 10 ];
      axis2.z = this->parameters[ 11 ];

    }

    return axis2;

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::EllipsoidAtom::getAxis2() const" );

}


gkg::Vector3d< float > 
gkg::EllipsoidAtom::getAxis3() const
{

  try
  {

    gkg::Vector3d< float > axis3;

    // theoretically, one could call the getStdParameters() function for both
    // compressed and uncompressed representations, but here, it remains more
    // efficient for uncompressed version not to read all the other parameters
    // and to explicitly fill the axis3 directly from parameters[]
    if ( hasCompressedRepresentation() )
    {

      gkg::Vector3d< float > center;
      float radius1 = 0.0f;
      float radius2 = 0.0f;
      float radius3 = 0.0f;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis2;
    
      this->getStdParameters( center,
                              radius1,
                              radius2,
                              radius3,
                              axis1,
                              axis2,
                              axis3 );

    }
    else
    {

      axis3.x = this->parameters[ 12 ];
      axis3.y = this->parameters[ 13 ];
      axis3.z = this->parameters[ 14 ];

    }

    return axis3;

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::EllipsoidAtom::getAxis3() const" );

}


gkg::Vector3d< float > 
gkg::EllipsoidAtom::getRotationAngles( bool degree ) const
{

  try
  {

    gkg::Vector3d< float > rotationAngles;

    // theoretically, one could call the getStdParameters() function for both
    // compressed and uncompressed representations, but here, it remains more
    // efficient for uncompressed version not to read all the other parameters
    // and to explicitly fill the rotationAngles directly from parameters[]
    if ( hasCompressedRepresentation() )
    {

      gkg::Vector3d< float > center;
      gkg::Vector3d< float > radii;
      this->getStdParameters( center,
                              radii,
                              rotationAngles,
                              degree );

    }
    else
    {


      gkg::Vector3d< float > axis1( this->parameters[ 6 ],
                                    this->parameters[ 7 ],
                                    this->parameters[ 8 ] );
      gkg::Vector3d< float > axis2( this->parameters[ 9 ],
                                    this->parameters[ 10 ],
                                    this->parameters[ 11 ] );
      gkg::Vector3d< float > axis3( this->parameters[ 12 ],
                                    this->parameters[ 13 ],
                                    this->parameters[ 14 ] );
      rotationAngles = gkg::getXYZAnglesFromTrieder( axis1, axis2, axis3,
                                                     degree );


    }

    return rotationAngles;

  }
  GKG_CATCH( "gkg::Vector3d< float > "
             "gkg::EllipsoidAtom::getRotationAngles() const" );

}


void gkg::EllipsoidAtom::getStdParameters( gkg::Vector3d< float >& center,
                                           float& radius1,
                                           float& radius2,
                                           float& radius3,
                                           gkg::Vector3d< float >& axis1,
                                           gkg::Vector3d< float >& axis2,
                                           gkg::Vector3d< float >& axis3 ) const
{

  try
  {

    // calling the static method
    getStdParametersFromVectorRepresentation( &parameters[ 0 ],
                                              hasCompressedRepresentation(),
                                              center,
                                              radius1,
                                              radius2,
                                              radius3,
                                              axis1,
                                              axis2,
                                              axis3 );

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::getStdParameters( "
             "gkg::Vector3d< float >& center, "
             "float& radius1, "
             "float& radius2, "
             "float& radius3, "
             "gkg::Vector3d< float >& axis1, "
             "gkg::Vector3d< float >& axis2, "
             "gkg::Vector3d< float >& axis3 ) const" );

}


void gkg::EllipsoidAtom::getStdParameters( gkg::Vector3d< float >& center,
                                           float& radius1,
                                           float& radius2,
                                           float& radius3,
                                           float& rotationAngleX,
                                           float& rotationAngleY,
                                           float& rotationAngleZ,
                                           bool degree ) const
{

  try
  {

  
    gkg::Vector3d< float > radii;
    gkg::Vector3d< float > rotationAngles;

    this->getStdParameters( center,
                            radii,
                            rotationAngles,
                            degree );

    radius1 = radii.x;
    radius2 = radii.y;
    radius3 = radii.z;
    rotationAngleX = rotationAngles.x;
    rotationAngleY = rotationAngles.y;
    rotationAngleZ = rotationAngles.z;
    
  }
  GKG_CATCH( "void gkg::EllipsoidAtom::getStdParameters( "
             "gkg::Vector3d< float >& center, "
             "float& radius1, "
             "float& radius2, "
             "float& radius3, "
             "float& rotationAngleX, "
             "float& rotationAngleY, "
             "float& rotationAngleZ, "
             "bool degree ) const" );

}


void gkg::EllipsoidAtom::getStdParameters(
                                        gkg::Vector3d< float >& center,
                                        gkg::Vector3d< float >& radii,
                                        gkg::Vector3d< float >& rotationAngles,
                                        bool degree ) const
{

  try
  {

    gkg::Vector3d< float > axis1;
    gkg::Vector3d< float > axis2;
    gkg::Vector3d< float > axis3;

    this->getStdParameters( center,
                            radii.x,
                            radii.y,
                            radii.z,
                            axis1,
                            axis2,
                            axis3 );
    rotationAngles = gkg::getXYZAnglesFromTrieder( axis1, axis2, axis3, degree );

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::getStdParameters( "
             "gkg::Vector3d< float >& center, "
             "gkg::Vector< float >& radii, "
             "gkg::Vector< float >& rotationAngles, "
             "bool degree) const" );

}


float gkg::EllipsoidAtom::getVolume() const
{

  try
  {

    gkg::Vector3d< float > radii = this->getRadii();
    return 4.0f * M_PI * radii.x * radii.y * radii.z / 3.0f;

  }
  GKG_CATCH( "float gkg::EllipsoidAtom::getVolume() const" );

}


bool gkg::EllipsoidAtom::contains(
                                  const gkg::Vector3d< float >& position ) const
{

  try
  {

    return gkg::EllipsoidAtom::belongTo( &parameters[ 0 ],
                                         hasCompressedRepresentation(), 
                                         position.x,
                                         position.y,
                                         position.z );

  }
  GKG_CATCH( "bool gkg::EllipsoidAtom::contains( "
             "const gkg::Vector3d< float >& position ) const" );

}


void gkg::EllipsoidAtom::getTranslatedParameters(
                                              float* outputVectorRepresentation,
                                              float translationX,
                                              float translationY,
                                              float translationZ ) const
{

  try
  {

    getTranslatedVectorRepresentationFromVectorRepresentation( 
                                                &parameters[ 0 ],
                                                hasCompressedRepresentation(),
                                                translationX,
                                                translationY,
                                                translationZ,
                                                outputVectorRepresentation,
                                                hasCompressedRepresentation() );

  }
  GKG_CATCH( "void gkg::SphereAtom::getTranslatedParameters( "
             "float* outputVectorRepresentation, "
             "float translationX, "
             "float translationY, "
             "float translationZ ) const" );

}




uint8_t gkg::EllipsoidAtom::getStaticType()
{

  try
  {
  
    return 2U;
     
  }
  GKG_CATCH( "uint8_t gkg::SphereAtom::getStaticType()" );

}


void gkg::EllipsoidAtom::getStdParametersFromVectorRepresentation(
                                        const float* vectorRepresentation,
                                        bool compressedRepresentation,
                                        gkg::Vector3d< float >& center,
                                        float& radius1,
                                        float& radius2,
                                        float& radius3,
                                        gkg::Vector3d< float >& axis1,
                                        gkg::Vector3d< float >& axis2,
                                        gkg::Vector3d< float >& axis3 )
{

  try
  {

    if ( compressedRepresentation )
    {

      gkg::Matrix ellipsoidMatrix( 3, 3 );
      ellipsoidMatrix( 0, 0 ) = ( double )vectorRepresentation[ 0 ];
      ellipsoidMatrix( 0, 1 ) = ( double )vectorRepresentation[ 3 ] / 2.0;
      ellipsoidMatrix( 0, 2 ) = ( double )vectorRepresentation[ 5 ] / 2.0;

      ellipsoidMatrix( 1, 0 ) = ellipsoidMatrix( 0, 1 );
      ellipsoidMatrix( 1, 1 ) = ( double )vectorRepresentation[ 1 ];
      ellipsoidMatrix( 1, 2 ) = ( double )vectorRepresentation[ 4 ] / 2.0;

      ellipsoidMatrix( 2, 0 ) = ellipsoidMatrix( 0, 2 );
      ellipsoidMatrix( 2, 1 ) = ellipsoidMatrix( 1, 2 );
      ellipsoidMatrix( 2, 2 ) = ( double )vectorRepresentation[ 2 ];


      // getting numerical analysis implementation factory
      gkg::NumericalAnalysisImplementationFactory* factory =    
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      // computing the inverse ellipsoid matrix
      gkg::Matrix inverseEllipsoidMatrix( 3, 3 );
      factory->getInverse( ellipsoidMatrix,
                           inverseEllipsoidMatrix );

      // computing the eigensystem of the ellipsoid matrix
      gkg::Matrix V( 3, 3 );
      gkg::Vector D( 3 );
      factory->getEigenSystem( ellipsoidMatrix, D, V );
 
      if ( ( D( 0 ) < 0.0 ) ||
           ( D( 1 ) < 0.0 ) ||
           ( D( 2 ) < 0.0 ) )
      {

        throw std::runtime_error( "one eigenvalue is negative or nul" );

      }

      // collecting the 3 radii
      radius1 = 1.0f / ( float )std::sqrt( D( 0 ) );
      radius2 = 1.0f / ( float )std::sqrt( D( 1 ) );
      radius3 = 1.0f / ( float )std::sqrt( D( 2 ) );

      // collecting the 3 axes
      axis1.x = V( 0 , 0 );
      axis1.y = V( 1 , 0 );
      axis1.z = V( 2 , 0 );

      axis2.x = V( 0 , 1 );
      axis2.y = V( 1 , 1 );
      axis2.z = V( 2 , 1 );

      axis3.x = V( 0 , 2 );
      axis3.y = V( 1 , 2 );
      axis3.z = V( 2 , 2 );

      // check that the frame is direct, otherwise
      // inverting the 3rd axis
      if ( axis1.cross( axis2 ).dot( axis3 ) < 0 )
      {

        axis3 *= -1.0f;

      }

      // computing the center of the ellipsoid
      gkg::Vector p( 3 );

      p( 0 ) = ( double )vectorRepresentation[ 6 ] / 2.0;
      p( 1 ) = ( double )vectorRepresentation[ 7 ] / 2.0;
      p( 2 ) = ( double )vectorRepresentation[ 8 ] / 2.0;

      p = inverseEllipsoidMatrix.getComposition( p );
      center.x = - ( float )p( 0 );
      center.y = - ( float )p( 1 );
      center.z = - ( float )p( 2 );

    }
    else
    {

      center.x = vectorRepresentation[ 0 ];
      center.y = vectorRepresentation[ 1 ];
      center.z = vectorRepresentation[ 2 ];

      radius1 = vectorRepresentation[ 3 ];
      radius2 = vectorRepresentation[ 4 ];
      radius3 = vectorRepresentation[ 5 ];

      axis1.x = vectorRepresentation[ 6 ];
      axis1.y = vectorRepresentation[ 7 ];
      axis1.z = vectorRepresentation[ 8 ];

      axis2.x = vectorRepresentation[ 9 ];
      axis2.y = vectorRepresentation[ 10 ];
      axis2.z = vectorRepresentation[ 11 ];

      axis3.x = vectorRepresentation[ 12 ];
      axis3.y = vectorRepresentation[ 13 ];
      axis3.z = vectorRepresentation[ 14 ];

    }

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::"
             "getStdParametersFromVectorRepresentation( "
             "const float* vectorRepresentation, "
             "bool compressedRepresentation, "
             "gkg::Vector3d< float >& center, "
             "float& radius1, "
             "float& radius2, "
             "float& radius3, "
             "gkg::Vector3d< float >& axis1, "
             "gkg::Vector3d< float >& axis2, "
             "gkg::Vector3d< float >& axis3 )" );

}



void gkg::EllipsoidAtom::getVectorRepresentationFromStdParameters(
                               const gkg::Vector3d< float >& center,
                               const float& radius1,
                               const float& radius2,
                               const float& radius3,
                               const gkg::Vector3d< float >& axis1,
                               const gkg::Vector3d< float >& axis2,
                               const gkg::Vector3d< float >& axis3,
                               float* vectorRepresentation,
                               bool compressedRepresentation )
{

  try
  {

    if ( compressedRepresentation )
    {

      gkg::Matrix T( 4, 4 );
      T.setIdentity();
      T( 3, 0 ) = -center.x;
      T( 3, 1 ) = -center.y;
      T( 3, 2 ) = -center.z;
 
      gkg::Matrix R( 4, 4 );
      R.setZero();
      R( 0, 0 ) = axis1.x;
      R( 1, 0 ) = axis1.y;
      R( 2, 0 ) = axis1.z;
      R( 0, 1 ) = axis2.x;
      R( 1, 1 ) = axis2.y;
      R( 2, 1 ) = axis2.z;
      R( 0, 2 ) = axis3.x;
      R( 1, 2 ) = axis3.y;
      R( 2, 2 ) = axis3.z;
      R( 3, 3 ) = 1.0;
 
      gkg::Matrix S( 4, 4 );
      S.setIdentity();
      S( 0, 0 ) = 1.0 / ( radius1 * radius1 );
      S( 1, 1 ) = 1.0 / ( radius2 * radius2 );
      S( 2, 2 ) = 1.0 / ( radius3 * radius3 );

      gkg::Matrix transposedT = T.getTransposition();
      gkg::Matrix transposedR = R.getTransposition();

      gkg::Matrix compressedMatrix = R.getComposition( S ).getComposition(
                                                                  transposedR );

      compressedMatrix( 3, 3 ) = -1.0;

      compressedMatrix = T.getComposition( compressedMatrix ).getComposition(
                                                                  transposedT );

      getVectorRepresentationFromMatrixRepresentation( compressedMatrix,
                                                       vectorRepresentation,
                                                       true );

    }
    else
    {

      vectorRepresentation[ 0 ] = center.x;
      vectorRepresentation[ 1 ] = center.y;
      vectorRepresentation[ 2 ] = center.z;

      vectorRepresentation[ 3 ] = radius1;
      vectorRepresentation[ 4 ] = radius2;
      vectorRepresentation[ 5 ] = radius3;

      vectorRepresentation[ 6 ] = axis1.x;
      vectorRepresentation[ 7 ] = axis1.y;
      vectorRepresentation[ 8 ] = axis1.z;

      vectorRepresentation[ 9 ] = axis2.x;
      vectorRepresentation[ 10 ] = axis2.y;
      vectorRepresentation[ 11 ] = axis2.z;

      vectorRepresentation[ 12 ] = axis3.x;
      vectorRepresentation[ 13 ] = axis3.y;
      vectorRepresentation[ 14 ] = axis3.z;

    }

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::"
             "getVectorRepresentationFromStdParameters( "
             "const gkg::Vector3d< float >& center, "
             "const float& radius1, "
             "const float& radius2, "
             "const float& radius3, "
             "const gkg::Vector3d< float >& axis1, "
             "const gkg::Vector3d< float >& axis2, "
             "const gkg::Vector3d< float >& axis3, "
             "float* vectorRepresentation, "
             "bool compressedRepresentation )" );

}


void gkg::EllipsoidAtom::getMatrixRepresentationFromVectorRepresentation(
                                             const float* vectorRepresentation,
                                             bool compressedRepresentation,
                                             gkg::Matrix& matrixRepresentation )
{

  try
  {

    if ( ( matrixRepresentation.getSize1() != 4 ) ||
         ( matrixRepresentation.getSize1() != 4 ) )
    {

      matrixRepresentation.reallocate( 4, 4 );

    }

    if ( compressedRepresentation )
    {

      matrixRepresentation( 0, 0 ) = vectorRepresentation[ 0 ];
      matrixRepresentation( 0, 1 ) = 0.5 * vectorRepresentation[ 3 ];
      matrixRepresentation( 0, 2 ) = 0.5 * vectorRepresentation[ 5 ];
      matrixRepresentation( 0, 3 ) = 0.5 * vectorRepresentation[ 6 ];

      matrixRepresentation( 1, 0 ) = matrixRepresentation( 0, 1 );
      matrixRepresentation( 1, 1 ) = vectorRepresentation[ 1 ];
      matrixRepresentation( 1, 2 ) = 0.5 * vectorRepresentation[ 4 ];
      matrixRepresentation( 1, 3 ) = 0.5 * vectorRepresentation[ 7 ];

      matrixRepresentation( 2, 0 ) = matrixRepresentation( 0, 2 );
      matrixRepresentation( 2, 1 ) = matrixRepresentation( 1, 2 );
      matrixRepresentation( 2, 2 ) = vectorRepresentation[ 2 ];
      matrixRepresentation( 2, 3 ) = 0.5 * vectorRepresentation[ 8 ];

      matrixRepresentation( 3, 0) = matrixRepresentation( 0, 3 );
      matrixRepresentation( 3, 1) = matrixRepresentation( 1, 3 );
      matrixRepresentation( 3, 2) = matrixRepresentation( 2, 3 );
      matrixRepresentation( 3, 3 ) = vectorRepresentation[ 9 ];

    }
    else
    {

      Vector3d< float > center( vectorRepresentation[ 0 ],
                                vectorRepresentation[ 1 ],
                                vectorRepresentation[ 2 ] );
      float radius1 = vectorRepresentation[ 3 ];
      float radius2 = vectorRepresentation[ 4 ];
      float radius3 = vectorRepresentation[ 5 ];
      Vector3d< float > axis1( vectorRepresentation[ 6 ],
                               vectorRepresentation[ 7 ],
                               vectorRepresentation[ 8 ] );
      Vector3d< float > axis2( vectorRepresentation[ 9 ],
                               vectorRepresentation[ 10 ],
                               vectorRepresentation[ 11 ] );
      Vector3d< float > axis3( vectorRepresentation[ 12 ],
                               vectorRepresentation[ 13 ],
                               vectorRepresentation[ 14 ] );

      std::vector< float > compressedVectorRepresentation( 10U );
      getVectorRepresentationFromStdParameters( 
                             center,
                              radius1,
                              radius2,
                              radius3,
                              axis1,
                              axis2,
                              axis3,
                              &compressedVectorRepresentation[ 0 ],
                              true );

      matrixRepresentation( 0, 0 ) = compressedVectorRepresentation[ 0 ];
      matrixRepresentation( 0, 1 ) = 0.5 * compressedVectorRepresentation[ 3 ];
      matrixRepresentation( 0, 2 ) = 0.5 * compressedVectorRepresentation[ 5 ];
      matrixRepresentation( 0, 3 ) = 0.5 * compressedVectorRepresentation[ 6 ];

      matrixRepresentation( 1, 0 ) = matrixRepresentation( 0, 1 );
      matrixRepresentation( 1, 1 ) = compressedVectorRepresentation[ 1 ];
      matrixRepresentation( 1, 2 ) = 0.5 * compressedVectorRepresentation[ 4 ];
      matrixRepresentation( 1, 3 ) = 0.5 * compressedVectorRepresentation[ 7 ];

      matrixRepresentation( 2, 0 ) = matrixRepresentation( 0, 2 );
      matrixRepresentation( 2, 1 ) = matrixRepresentation( 1, 2 );
      matrixRepresentation( 2, 2 ) = compressedVectorRepresentation[ 2 ];
      matrixRepresentation( 2, 3 ) = 0.5 * compressedVectorRepresentation[ 8 ];

      matrixRepresentation( 3, 0) = matrixRepresentation( 0, 3 );
      matrixRepresentation( 3, 1) = matrixRepresentation( 1, 3 );
      matrixRepresentation( 3, 2) = matrixRepresentation( 2, 3 );
      matrixRepresentation( 3, 3 ) = compressedVectorRepresentation[ 9 ];

    }

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::"
             "getMatrixRepresentationFromVectorRepresentation( "
             "const float* vectorRepresentation, "
             "bool compressedRepresentation, "
             "gkg::Matrix& matrixRepresentation )" );

}


void gkg::EllipsoidAtom::getVectorRepresentationFromMatrixRepresentation(
                                        const gkg::Matrix& matrixRepresentation,
                                        float* vectorRepresentation,
                                        bool compressedRepresentation )
{

  try
  {

    if ( ( matrixRepresentation.getSize1() != 4 ) ||
         ( matrixRepresentation.getSize2() != 4 ) )
    {

      throw std::runtime_error( "matrix representation size is not 4 x 4" );

    }

    if ( compressedRepresentation )
    {

      vectorRepresentation[ 0 ] = ( float )matrixRepresentation( 0, 0 );
      vectorRepresentation[ 1 ] = ( float )matrixRepresentation( 1, 1 );
      vectorRepresentation[ 2 ] = ( float )matrixRepresentation( 2, 2 );
      vectorRepresentation[ 3 ] = 2.0f * ( float )matrixRepresentation( 0, 1 );
      vectorRepresentation[ 4 ] = 2.0f * ( float )matrixRepresentation( 1, 2 );
      vectorRepresentation[ 5 ] = 2.0f * ( float )matrixRepresentation( 0, 2 );
      vectorRepresentation[ 6 ] = 2.0f * ( float )matrixRepresentation( 0, 3 );
      vectorRepresentation[ 7 ] = 2.0f * ( float )matrixRepresentation( 1, 3 );
      vectorRepresentation[ 8 ] = 2.0f * ( float )matrixRepresentation( 2, 3 );
      vectorRepresentation[ 9 ] = ( float )matrixRepresentation( 3, 3 );

    }
    else
    {

      std::vector< float > compressedVectorRepresentation( 10U );

      compressedVectorRepresentation[ 0 ] =
                                   ( float )matrixRepresentation( 0, 0 );
      compressedVectorRepresentation[ 1 ] =
                                   ( float )matrixRepresentation( 1, 1 );
      compressedVectorRepresentation[ 2 ] =
                                   ( float )matrixRepresentation( 2, 2 );
      compressedVectorRepresentation[ 3 ] =
                                   2.0f * ( float )matrixRepresentation( 0, 1 );
      compressedVectorRepresentation[ 4 ] =
                                   2.0f * ( float )matrixRepresentation( 1, 2 );
      compressedVectorRepresentation[ 5 ] =
                                   2.0f * ( float )matrixRepresentation( 0, 2 );
      compressedVectorRepresentation[ 6 ] =
                                   2.0f * ( float )matrixRepresentation( 0, 3 );
      compressedVectorRepresentation[ 7 ] =
                                   2.0f * ( float )matrixRepresentation( 1, 3 );
      compressedVectorRepresentation[ 8 ] =
                                   2.0f * ( float )matrixRepresentation( 2, 3 );
      compressedVectorRepresentation[ 9 ] =
                                   ( float )matrixRepresentation( 3, 3 );

      gkg::Vector3d< float > center;
      float radius1;
      float radius2;
      float radius3;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis2;
      gkg::Vector3d< float > axis3;
      getStdParametersFromVectorRepresentation(
                                           &compressedVectorRepresentation[ 0 ],
                                           true,
                                           center,
                                           radius1,
                                           radius2,
                                           radius3,
                                           axis1,
                                           axis2,
                                           axis3 );

      vectorRepresentation[ 0 ] = center.x;
      vectorRepresentation[ 1 ] = center.y;
      vectorRepresentation[ 2 ] = center.z;
      vectorRepresentation[ 3 ] = radius1;
      vectorRepresentation[ 4 ] = radius2;
      vectorRepresentation[ 5 ] = radius3;
      vectorRepresentation[ 6 ] = axis1.x;
      vectorRepresentation[ 7 ] = axis1.y;
      vectorRepresentation[ 8 ] = axis1.z;
      vectorRepresentation[ 9 ] = axis2.x;
      vectorRepresentation[ 10 ] = axis2.y;
      vectorRepresentation[ 11 ] = axis2.z;
      vectorRepresentation[ 12 ] = axis3.x;
      vectorRepresentation[ 13 ] = axis3.y;
      vectorRepresentation[ 14 ] = axis3.z;
 
    }

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::"
             "getVectorRepresentationFromMatrixRepresentation( "
             "const gkg::Matrix& matrixRepresentation, "
             "float* vectorRepresentation, "
             "bool compressedRepresentation )" );

}


void gkg::EllipsoidAtom::
                      getTranslatedVectorRepresentationFromVectorRepresentation(
                                           const float* vectorRepresentation,
                                           bool compressedRepresentation,
                                           float translationX,
                                           float translationY,
                                           float translationZ,
                                           float* outputVectorRepresentation,
                                           bool outputCompressedRepresentation )
{

  try
  {

    gkg::Matrix matrixRepresentation;
    getMatrixRepresentationFromVectorRepresentation( vectorRepresentation,
                                                     compressedRepresentation,
                                                     matrixRepresentation );

    gkg::Matrix translation( 4, 4 );
    translation.setIdentity();
    translation( 3, 0 ) = -translationX;
    translation( 3, 1 ) = -translationY;
    translation( 3, 2 ) = -translationZ;

    gkg::Matrix transposedTranslation = translation.getTransposition();

    gkg::Matrix newMatrixRepresentation = translation.getComposition(
                                matrixRepresentation ).getComposition(
                                                        transposedTranslation );

    getVectorRepresentationFromMatrixRepresentation(
                                               newMatrixRepresentation,
                                               outputVectorRepresentation,
                                               outputCompressedRepresentation );

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::"
             "getTranslatedVectorRepresentationFromVectorRepresentation( "
             "const float* vectorRepresentation, "
             "bool compressedRepresentation, "
             "float translationX, "
             "float translationY, "
             "float translationZ, "
             "float* outputVectorRepresentation, "
             "bool outputCompressedRepresentation )" );

}



void gkg::EllipsoidAtom::getAtomLutVoxel( const float* vectorRepresentation,
                                          bool compressedRepresentation,
                                          float lowerX,
                                          float lowerY,
                                          float lowerZ,
                                          float spanX,
                                          float spanY,
                                          float spanZ,
                                          int32_t lutSizeX,
                                          int32_t lutSizeY,
                                          int32_t lutSizeZ,
                                          int32_t& lutVoxelX,
                                          int32_t& lutVoxelY,
                                          int32_t& lutVoxelZ )
{

  try
  {

    gkg::Vector3d< float > center;
    float radius1 = 0.0f;
    float radius2 = 0.0f;
    float radius3 = 0.0f;
    gkg::Vector3d< float > axis1;
    gkg::Vector3d< float > axis2;
    gkg::Vector3d< float > axis3;

    getStdParametersFromVectorRepresentation( vectorRepresentation,
                                              compressedRepresentation,
                                              center,
                                              radius1,
                                              radius2,
                                              radius3,
                                              axis1,
                                              axis2,
                                              axis3 );

    lutVoxelX = ( int32_t )( ( ( center.x - lowerX ) / spanX ) *
                             ( lutSizeX - 1 ) );
    lutVoxelY = ( int32_t )( ( ( center.y - lowerY ) / spanY ) *
                             ( lutSizeY - 1 ) );
    lutVoxelZ = ( int32_t )( ( ( center.z - lowerZ ) / spanZ ) *
                             ( lutSizeZ - 1 ) );

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::getAtomLutVoxel( "
             "const float* vectorRepresentation, "
             "bool compressedRepresentation, "
             "float lowerX, "
             "float lowerY, "
             "float lowerZ, "
             "float spanX, "
             "float spanY, "
             "float spanZ, "
             "int32_t lutSizeX, "
             "int32_t lutSizeY, "
             "int32_t lutSizeZ, "
             "int32_t& lutVoxelX, "
             "int32_t& lutVoxelY, "
             "int32_t& lutVoxelZ )" );

}


void gkg::EllipsoidAtom::getBoundingBox( const float* vectorRepresentation,
                                         bool compressedRepresentation,
                                         float& lowerX,
                                         float& lowerY,
                                         float& lowerZ,
                                         float& upperX,
                                         float& upperY,
                                         float& upperZ )
{
  try
  {
    gkg::Vector3d< float > center;
    float radius1 = 0.0f;
    float radius2 = 0.0f;
    float radius3 = 0.0f;
    gkg::Vector3d< float > axis1;
    gkg::Vector3d< float > axis2;
    gkg::Vector3d< float > axis3;
    getStdParametersFromVectorRepresentation( vectorRepresentation,
                                              compressedRepresentation,
                                              center,
                                              radius1,
                                              radius2,
                                              radius3,
                                              axis1,
                                              axis2,
                                              axis3 );

    float offsetX = std::sqrt( ( ( axis1.x * axis1.x * radius1 * radius1 ) +
                                 ( axis1.y * axis1.y * radius2 * radius2 ) +
                                 ( axis1.z * axis1.z * radius3 * radius3 ) ) );

    float offsetY = std::sqrt( ( ( axis2.x * axis2.x * radius1 * radius1 ) +
                                 ( axis2.y * axis2.y * radius2 * radius2 ) +
                                 ( axis2.z * axis2.z * radius3 * radius3 ) ) );

    float offsetZ = std::sqrt( ( ( axis3.x * axis3.x * radius1 * radius1 ) +
                                 ( axis3.y * axis3.y * radius2 * radius2 ) +
                                 ( axis3.z * axis3.z * radius3 * radius3 ) ) );

    lowerX = center.x - offsetX;
    lowerY = center.y - offsetY;
    lowerZ = center.z - offsetZ;

    upperX = center.x + offsetX;
    upperY = center.y + offsetY;
    upperZ = center.z + offsetZ;

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::getBoundingBox( "
             "const float* vectorRepresentation, "
             "bool compressedRepresentation, "
             "float& lowerX, "
             "float& lowerY, "
             "float& lowerZ, "
             "float& upperX, "
             "float& upperY, "
             "float& upperZ )");

}


void gkg::EllipsoidAtom::getAtomLutVoxelSpan( const float* vectorRepresentation,
                                              bool compressedRepresentation,
                                              float lowerX,
                                              float lowerY,
                                              float lowerZ,
                                              float gridResolutionInUmX,
                                              float gridResolutionInUmY,
                                              float gridResolutionInUmZ,
                                              int32_t lutSizeX,
                                              int32_t lutSizeY,
                                              int32_t lutSizeZ,
                                              int32_t& infLutVoxelX,
                                              int32_t& supLutVoxelX,
                                              int32_t& infLutVoxelY,
                                              int32_t& supLutVoxelY,
                                              int32_t& infLutVoxelZ,
                                              int32_t& supLutVoxelZ )
{

  try
  {

    float boundingBoxLowerX = 0.0;
    float boundingBoxLowerY = 0.0;
    float boundingBoxLowerZ = 0.0;
    float boundingBoxUpperX = 0.0;
    float boundingBoxUpperY = 0.0;
    float boundingBoxUpperZ = 0.0;

    getBoundingBox( vectorRepresentation,
                    compressedRepresentation,
                    boundingBoxLowerX,
                    boundingBoxLowerY,
                    boundingBoxLowerZ,
                    boundingBoxUpperX,
                    boundingBoxUpperY,
                    boundingBoxUpperZ );

    infLutVoxelX = std::max( ( int32_t )( ( boundingBoxLowerX - lowerX ) /
                                          gridResolutionInUmX ),
                             0 );
    supLutVoxelX = std::min( ( int32_t )( ( boundingBoxUpperX - lowerX ) /
                                          gridResolutionInUmX ),
                             lutSizeX - 1 );

    infLutVoxelY = std::max( ( int32_t )( ( boundingBoxLowerY - lowerY ) /
                                          gridResolutionInUmY ),
                             0 );
    supLutVoxelY = std::min( ( int32_t )( ( boundingBoxUpperY - lowerY ) /
                                          gridResolutionInUmY ),
                             lutSizeY - 1 );

    infLutVoxelZ = std::max( ( int32_t )( ( boundingBoxLowerZ - lowerZ ) /
                                          gridResolutionInUmZ ),
                             0 );
    supLutVoxelZ = std::min( ( int32_t )( ( boundingBoxUpperZ - lowerZ ) /
                                          gridResolutionInUmZ ),
                             lutSizeZ - 1 );

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::getAtomLutVoxelSpan( "
             "const float* vectorRepresentation, "
             "bool compressedRepresentation, "
             "float lowerX, "
             "float lowerY, "
             "float lowerZ, "
             "float gridResolutionInUmX, "
             "float gridResolutionInUmY, "
             "float gridResolutionInUmZ, "
             "int32_t lutSizeX, "
             "int32_t lutSizeY, "
             "int32_t lutSizeZ, "
             "int32_t& infLutVoxelX, "
             "int32_t& supLutVoxelX, "
             "int32_t& infLutVoxelY, "
             "int32_t& supLutVoxelY, "
             "int32_t& infLutVoxelZ, "
             "int32_t& supLutVoxelZ )" );

}


float gkg::EllipsoidAtom::dotProduct( const float* vectorRepresentation,
                                      bool compressedRepresentation,
                                      const float& coordinateX,
                                      const float& coordinateY,
                                      const float& coordinateZ )
{

  try
  {

    float result = 0.0f;

    if ( compressedRepresentation )
    {

      result += vectorRepresentation[ 0 ] * coordinateX * coordinateX;
      result += vectorRepresentation[ 1 ] * coordinateY * coordinateY;
      result += vectorRepresentation[ 2 ] * coordinateZ * coordinateZ;
      result += vectorRepresentation[ 3 ] * coordinateX * coordinateY;
      result += vectorRepresentation[ 4 ] * coordinateY * coordinateZ;
      result += vectorRepresentation[ 5 ] * coordinateX * coordinateZ;
      result += vectorRepresentation[ 6 ] * coordinateX;
      result += vectorRepresentation[ 7 ] * coordinateY;
      result += vectorRepresentation[ 8 ] * coordinateZ;
      result += vectorRepresentation[ 9 ];

    }
    else
    {


      gkg::Vector3d< float > center;
      float radius1;
      float radius2;
      float radius3;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis2;
      gkg::Vector3d< float > axis3;
      getStdParametersFromVectorRepresentation( vectorRepresentation,
                                                false,
                                                center,
                                                radius1,
                                                radius2,
                                                radius3,
                                                axis1,
                                                axis2,
                                                axis3 );


      std::vector< float > compressedVectorRepresentation( 10U );

      getVectorRepresentationFromStdParameters(
                                           center,
                                           radius1,
                                           radius2,
                                           radius3,
                                           axis1,
                                           axis2,
                                           axis3,
                                           &compressedVectorRepresentation[ 0 ],
                                           true );
      result += compressedVectorRepresentation[ 0 ] * coordinateX * coordinateX;
      result += compressedVectorRepresentation[ 1 ] * coordinateY * coordinateY;
      result += compressedVectorRepresentation[ 2 ] * coordinateZ * coordinateZ;
      result += compressedVectorRepresentation[ 3 ] * coordinateX * coordinateY;
      result += compressedVectorRepresentation[ 4 ] * coordinateY * coordinateZ;
      result += compressedVectorRepresentation[ 5 ] * coordinateX * coordinateZ;
      result += compressedVectorRepresentation[ 6 ] * coordinateX;
      result += compressedVectorRepresentation[ 7 ] * coordinateY;
      result += compressedVectorRepresentation[ 8 ] * coordinateZ;
      result += compressedVectorRepresentation[ 9 ];

    }

    return result;

  }
  GKG_CATCH( "const float* vectorRepresentation, "
             "bool compressedRepresentation, "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ" );

}


float gkg::EllipsoidAtom::upperThreeByThreeMatrixBilinearProduct( 
                                                const float* atomParameters,
                                                bool compressedRepresentation,
                                                const float& firstCoordinateX,
                                                const float& firstCoordinateY,
                                                const float& firstCoordinateZ,
                                                const float& secondCoordinateX,
                                                const float& secondCoordinateY,
                                                const float& secondCoordinateZ )
{

  try
  {

    float result = 0.0f;

    if ( compressedRepresentation )
    {

      result += atomParameters[ 0 ] * firstCoordinateX * secondCoordinateX;
      result += atomParameters[ 1 ] * firstCoordinateY * secondCoordinateY;
      result += atomParameters[ 2 ] * firstCoordinateZ * secondCoordinateZ;
      result += 0.5 * atomParameters[ 3 ] * 
                ( firstCoordinateX * secondCoordinateY +
                  firstCoordinateY * secondCoordinateX );
      result += 0.5 * atomParameters[ 4 ] * 
                ( firstCoordinateY * secondCoordinateZ +
                  firstCoordinateZ * secondCoordinateY );
      result += 0.5 * atomParameters[ 5 ] * 
                ( firstCoordinateX * secondCoordinateZ +
                  firstCoordinateZ * secondCoordinateX );

    }
    else
    {

      gkg::Vector3d< float > center;
      float radius1;
      float radius2;
      float radius3;
      gkg::Vector3d< float > axis1;
      gkg::Vector3d< float > axis2;
      gkg::Vector3d< float > axis3;
      getStdParametersFromVectorRepresentation( atomParameters,
                                                false,
                                                center,
                                                radius1,
                                                radius2,
                                                radius3,
                                                axis1,
                                                axis2,
                                                axis3 );

      std::vector< float > compressedVectorRepresentation( 10U );

      getVectorRepresentationFromStdParameters(
                                           center,
                                           radius1,
                                           radius2,
                                           radius3,
                                           axis1,
                                           axis2,
                                           axis3,
                                           &compressedVectorRepresentation[ 0 ],
                                           true );

      result += atomParameters[ 0 ] * firstCoordinateX * secondCoordinateX;
      result += atomParameters[ 1 ] * firstCoordinateY * secondCoordinateY;
      result += atomParameters[ 2 ] * firstCoordinateZ * secondCoordinateZ;
      result += 0.5 * atomParameters[ 3 ] * 
                ( firstCoordinateX * secondCoordinateY +
                  firstCoordinateY * secondCoordinateX );
      result += 0.5 * atomParameters[ 4 ] * 
                ( firstCoordinateY * secondCoordinateZ +
                  firstCoordinateZ * secondCoordinateY );
      result += 0.5 * atomParameters[ 5 ] * 
                ( firstCoordinateX * secondCoordinateZ +
                  firstCoordinateZ * secondCoordinateX );

    }

    return result;

  }
  GKG_CATCH( "float EllipsoidAtom::upperThreeByThreeMatrixBilinearProduct( "
             "const float* atomParameters, "
             "bool compressedRepresentation, "
             "const float& firstCoordinateX, "
             "const float& firstCoordinateY, "
             "const float& firstCoordinateZ, "
             "const float& secondCoordinateX, "
             "const float& secondCoordinateY, "
             "const float& secondCoordinateZ )" );

}


bool gkg::EllipsoidAtom::belongTo( const float* vectorRepresentation,
                                   bool compressedRepresentation,
                                   const float& coordinateX,
                                   const float& coordinateY,
                                   const float& coordinateZ )
{

  try
  {

    return ( dotProduct( vectorRepresentation,
                         compressedRepresentation,
                         coordinateX,
                         coordinateY,
                         coordinateZ ) <= 0.0f );

  }
  GKG_CATCH( "bool gkg::EllipsoidAtom::belongTo( "
             "const float* vectorRepresentation, "
             "bool compressedRepresentation, "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ )" );

}


float gkg::EllipsoidAtom::distanceToMembrane(
                                     const float* /*vectorRepresentation*/,
                                     bool /*compressedRepresentation*/,
                                     const float& /*coordinateX*/,
                                     const float& /*coordinateY*/,
                                     const float& /*coordinateZ*/ )
{

  try
  {

    // to be filled

    return 0.0f;

  }
  GKG_CATCH( "float gkg::EllipsoidAtom::distanceToMembrane( "
             "const float* vectorRepresentation, "
             "bool compressedRepresentation, "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ )" );

}


void gkg::EllipsoidAtom::getOutwardNormalAt( const float* atomParameters,
                                             bool compressedRepresentation,
                                             const float& coordinateX,
                                             const float& coordinateY,
                                             const float& coordinateZ,
                                             float& normalX,
                                             float& normalY,
                                             float& normalZ )
{

  try
  {

    if ( compressedRepresentation )
    {

      normalX = 2.0f * atomParameters[ 0 ] * coordinateX + 
                       atomParameters[ 3 ] * coordinateY +
                       atomParameters[ 4 ] * coordinateZ +
                atomParameters[ 6 ];

      normalY = 2.0f * atomParameters[ 1 ] * coordinateY +
                       atomParameters[ 5 ] * coordinateZ +
                       atomParameters[ 3 ] * coordinateX +
                atomParameters[ 7 ];

      normalZ = 2.0f * atomParameters[ 2 ] * coordinateZ +
                       atomParameters[ 4 ] * coordinateX +
                       atomParameters[ 5 ] * coordinateY +
                atomParameters[ 8 ];

      float normalNorm = sqrt( normalX * normalX +
                               normalY * normalY + 
                               normalZ * normalZ );

      normalX /= normalNorm;
      normalY /= normalNorm;
      normalZ /= normalNorm;

    }
    else
    {

      throw std::runtime_error( "not implement yet" );

    }

  }
  GKG_CATCH( "void gkg::EllipsoidAtom::getOutwardNormalAt( "
             "const float* atomParameters, "
             "bool compressedRepresentation, "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ, "
             "float& normalX, "
             "float& normalY, "
             "float& normalZ )" );

}


bool gkg::EllipsoidAtom::isRayCollidingWithAtom(
                                             const float* atomParameters,
                                             bool compressedRepresentation,
                                             const float& coordinateX,
                                             const float& coordinateY,
                                             const float& coordinateZ,
                                             const float& stepX,
                                             const float& stepY,
                                             const float& stepZ,
                                             bool insideAtom,
                                             float& intersectionX,
                                             float& intersectionY,
                                             float& intersectionZ,
                                             float& collisionDistance )
{

  try
  {

    bool result = false;

    if ( compressedRepresentation )
    {

      float a = atomParameters[0] * stepX * stepX +
                atomParameters[1] * stepY * stepY +
                atomParameters[2] * stepZ * stepZ +
                atomParameters[3] * stepX * stepY +
                atomParameters[4] * stepY * stepZ +
                atomParameters[5] * stepX * stepZ;

      float b = 2.0 * upperThreeByThreeMatrixBilinearProduct(
                            atomParameters,
                            compressedRepresentation,
                            coordinateX,
                            coordinateY,
                            coordinateZ,
                            stepX,
                            stepY,
                            stepZ) +
                atomParameters[6] * stepX +
                atomParameters[7] * stepY +
                atomParameters[8] * stepZ;

      float c = dotProduct( atomParameters,
                            compressedRepresentation,
                            coordinateX,
                            coordinateY,
                            coordinateZ );

      float delta = b * b - 4.0f * a * c;


      // there are some collisions
      if ( delta >= 0.0f )
      {

        // collision distances
        float root1 = ( -b - std::sqrt( delta ) ) / ( 2.0f * a );
        float root2 = ( -b + std::sqrt( delta ) ) / ( 2.0f * a );

        // collision conditions
        bool firstCollisionCondition = ( ( root1 <= 1.0f ) &&
                                         ( root1 > 0.0f ) );
        bool secondCollisionCondition = ( ( root2 <= 1.0f ) &&
                                          ( root2 > 0.0f ) );

        // if collision(s) detected
        if ( firstCollisionCondition || secondCollisionCondition )
        {

          result = true;
          float ratio = 0.0f;

          if ( firstCollisionCondition && secondCollisionCondition )
          {

            ratio = ( insideAtom ? std::max( root1, root2 ) :
                                   std::min( root1, root2 ) );

          }
          else
          {

            ratio = ( firstCollisionCondition ? root1 : root2 );

          }

          float normOfStep = std::sqrt( a );

          intersectionX = coordinateX + ratio * stepX;
          intersectionY = coordinateY + ratio * stepY;
          intersectionZ = coordinateZ + ratio * stepZ;

          collisionDistance = ratio * normOfStep;

        }

      }

    }
    else
    {

      throw std::runtime_error( "not implemented yet" );

    }

    return result;

  }
  GKG_CATCH( "bool gkg::EllipsoidAtom::isRayCollidingWithAtom( "
             "const float* atomParameters, "
             "bool compressedRepresentation, "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ, "
             "const float& stepX, "
             "const float& stepY, "
             "const float& stepZ, "
             "bool insideAtom, "
             "float& intersectionX, "
             "float& intersectionY, "
             "float& intersectionZ, "
             "float& collisionDistance )" );
}


//
// adding static methods to atom method factory
//


static bool registerGetAtomLutVoxelMethodToFactory __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerGetAtomLutVoxelMethod(
    gkg::EllipsoidAtom::getStaticType(),
    gkg::EllipsoidAtom::getAtomLutVoxel );
    
static bool registerGetAtomLutVoxelSpanMethodToFactory __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerGetAtomLutVoxelSpanMethod(
    gkg::EllipsoidAtom::getStaticType(),
    gkg::EllipsoidAtom::getAtomLutVoxelSpan );
    
static bool registerBelongToMethodToFactory __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerBelongToMethod(
    gkg::EllipsoidAtom::getStaticType(),
    gkg::EllipsoidAtom::belongTo );
    
static bool registerDistanceToMembraneMethodToFactory __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerDistanceToMembraneMethod(
    gkg::EllipsoidAtom::getStaticType(),
    gkg::EllipsoidAtom::distanceToMembrane );

static bool registerOutwardNormaToMembraneMethodToFactory 
                                                      __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerOutwardNormalToMembraneMethod(
    gkg::EllipsoidAtom::getStaticType(),
    gkg::EllipsoidAtom::getOutwardNormalAt );

static bool registerIsRayCollidingWithAtomMethodToFactory
                                                      __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerIsRayCollidingWithAtomMethod(
    gkg::EllipsoidAtom::getStaticType(),
    gkg::EllipsoidAtom::isRayCollidingWithAtom );

