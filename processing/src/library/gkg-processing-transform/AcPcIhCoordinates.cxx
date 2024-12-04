#include <gkg-processing-transform/AcPcIhCoordinates.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <iostream>


gkg::AcPcIhCoordinates::AcPcIhCoordinates()
                       : gkg::HeaderedObject(),
                         integerAnteriorCommissure( 0, 0, 0 ),
                         integerPosteriorCommissure( 0, 0, 0 ),
                         integerInterHemispheric( 0, 0, 0 ),
                         anteriorCommissure( 0.0, 0.0, 0.0 ),
                         posteriorCommissure( 0.0, 0.0, 0.0 ),
                         interHemispheric( 0.0, 0.0, 0.0 )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "AcPcIhCoordinates" ) );

  }
  GKG_CATCH( "gkg::AcPcIhCoordinates::AcPcIhCoordinates()" );

}


gkg::AcPcIhCoordinates::AcPcIhCoordinates( 
                  const gkg::Vector3d< int32_t >& theIntegerAnteriorCommissure,
                  const gkg::Vector3d< int32_t >& theIntegerPosteriorCommissure,
                  const gkg::Vector3d< int32_t >& theIntegerInterHemispheric,
                  const gkg::Vector3d< double >& resolution )
      
                       : gkg::HeaderedObject(),
                         integerAnteriorCommissure( 
			                         theIntegerAnteriorCommissure ),
 			 integerPosteriorCommissure(
			                        theIntegerPosteriorCommissure ),
 			 integerInterHemispheric( theIntegerInterHemispheric ),
 			 anteriorCommissure(
			       theIntegerAnteriorCommissure.x * resolution.x,
                               theIntegerAnteriorCommissure.y * resolution.y,
                               theIntegerAnteriorCommissure.z * resolution.z ),
 			 posteriorCommissure(
                               theIntegerPosteriorCommissure.x * resolution.x,
                               theIntegerPosteriorCommissure.y * resolution.y,
                               theIntegerPosteriorCommissure.z * resolution.z ),
 			 interHemispheric(
                               theIntegerInterHemispheric.x * resolution.x,
                               theIntegerInterHemispheric.y * resolution.y,
                               theIntegerInterHemispheric.z * resolution.z )

{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "AcPcIhCoordinates" ) );

  }
  GKG_CATCH( "gkg::AcPcIhCoordinates::AcPcIhCoordinates( "
             "const gkg::Vector3d< int32_t >& theIntegerAnteriorCommissure, "
             "const gkg::Vector3d< int32_t >& theIntegerPosteriorCommissure, "
             "const gkg::Vector3d< int32_t >& theIntegerInterHemispheric, "
             "const gkg::Vector3d< double >& resolution )" );

}


gkg::AcPcIhCoordinates::AcPcIhCoordinates(
                           const gkg::Vector3d< float >& theAnteriorCommissure,
                           const gkg::Vector3d< float >& thePosteriorCommissure,
                           const gkg::Vector3d< float >& theInterHemispheric,
                           const gkg::Vector3d< double >& resolution )
                       :gkg::HeaderedObject()
    
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "AcPcIhCoordinates" ) );


    integerAnteriorCommissure.x =
                     ( int32_t )( theAnteriorCommissure.x/resolution.x + 0.5 );
    integerAnteriorCommissure.y =
                     ( int32_t )( theAnteriorCommissure.y/resolution.y + 0.5 );
    integerAnteriorCommissure.z =
                     ( int32_t )( theAnteriorCommissure.z/resolution.z + 0.5 );
    integerPosteriorCommissure.x =
                     ( int32_t )( thePosteriorCommissure.x/resolution.x + 0.5 );
    integerPosteriorCommissure.y =
                     ( int32_t )( thePosteriorCommissure.y/resolution.y + 0.5 );
    integerPosteriorCommissure.z =
                     ( int32_t )( thePosteriorCommissure.z/resolution.z + 0.5 );
    integerInterHemispheric.x =
                     ( int32_t )( theInterHemispheric.x/resolution.x + 0.5 );
    integerInterHemispheric.y = 
                     ( int32_t )( theInterHemispheric.y/resolution.y + 0.5 );
    integerInterHemispheric.z = 
                     ( int32_t )( theInterHemispheric.z/resolution.z + 0.5 );
    anteriorCommissure = theAnteriorCommissure;
    posteriorCommissure = thePosteriorCommissure;
    interHemispheric = theInterHemispheric;

  }
  GKG_CATCH( "gkg::AcPcIhCoordinates::AcPcIhCoordinates( "
             "const gkg::Vector3d< float >& theAnteriorCommissure, "
             "const gkg::Vector3d< float >& thePosteriorCommissure, "
             "const gkg::Vector3d< float >& theInterHemispheric, "
             "const gkg::Vector3d< double >& resolution )" );

}


gkg::AcPcIhCoordinates::AcPcIhCoordinates( 
                       const Vector3d< int32_t >& theIntegerAnteriorCommissure,
                       const Vector3d< int32_t >& theIntegerPosteriorCommissure,
                       const Vector3d< int32_t >& theIntegerInterHemispheric,
                       const Vector3d< float >& theAnteriorCommissure,
                       const Vector3d< float >& thePosteriorCommissure,
                       const Vector3d< float >& theInterHemispheric )
                       : gkg::HeaderedObject(),
                         integerAnteriorCommissure(
                                                 theIntegerAnteriorCommissure ),
                         integerPosteriorCommissure(
                                                theIntegerPosteriorCommissure ),
                         integerInterHemispheric( theIntegerInterHemispheric ),
                         anteriorCommissure( theAnteriorCommissure ),
                         posteriorCommissure( thePosteriorCommissure ),
                         interHemispheric( theInterHemispheric )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "AcPcIhCoordinates" ) );

  }
  GKG_CATCH( "gkg::AcPcIhCoordinates::AcPcIhCoordinates( "
             "const Vector3d< int32_t >& theIntegerAnteriorCommissure, "
             "const Vector3d< int32_t >& theIntegerPosteriorCommissure, "
             "const Vector3d< int32_t >& theIntegerInterHemispheric, "
             "const Vector3d< float >& theAnteriorCommissure, "
             "const Vector3d< float >& thePosteriorCommissure, "
             "const Vector3d< float >& theInterHemispheric )" );

}


gkg::AcPcIhCoordinates::AcPcIhCoordinates( gkg::HeaderedObject& headeredObject )
                       : gkg::HeaderedObject()
{

  try
  {
    
    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "AcPcIhCoordinates" ) );
  
    double xResolution = 1.0, yResolution =1.0, zResolution = 1.0;
    if ( headeredObject.getHeader().hasAttribute( "resolutionX" ) )
    {
    
      headeredObject.getHeader().getAttribute( "resolutionX", xResolution );
    
    }
    else
    {
    
      throw std::runtime_error( "resolutionX attribute not found" );      
    
    }
    
    if ( headeredObject.getHeader().hasAttribute( "resolutionY" ) )
    {

      headeredObject.getHeader().getAttribute( "resolutionY", yResolution );
  
    }
    else
    {
    
      throw std::runtime_error( "resolutionY attribute not found" );      
    
    }
    
    if ( headeredObject.getHeader().hasAttribute( "resolutionZ" ) )
    {
    
      headeredObject.getHeader().getAttribute( "resolutionZ", zResolution );
 
    }
    else
    {
    
      throw std::runtime_error( "resolutionZ attribute not found" );      
    
    }
      
    gkg::AcPcIhCoordinates::addSemantic( headeredObject );
    gkg::GenericObjectList genericObjectList;
 
    if ( headeredObject.getHeader().hasAttribute( "AC_coordinate" ) )
    {

      try
      {

        headeredObject.getHeader().getAttribute( "AC_coordinate",
                                                 genericObjectList );
        integerAnteriorCommissure.x =
          ( int32_t )genericObjectList[ 0 ]->getScalar();
        integerAnteriorCommissure.y =
          ( int32_t )genericObjectList[ 1 ]->getScalar();
        integerAnteriorCommissure.z =
          ( int32_t )genericObjectList[ 2 ]->getScalar();

      }
      catch ( std::exception& )
      {

        std::vector< int32_t > tmp;
        headeredObject.getHeader().getAttribute( "AC_coordinate",
                                                 tmp );
        integerAnteriorCommissure.x = tmp[ 0 ];
        integerAnteriorCommissure.y = tmp[ 1 ];
        integerAnteriorCommissure.z = tmp[ 2 ];
     


      }

    }
    else
    {
 
      throw std::runtime_error( "'AC_coordinate' attribute not found" );
 
    }
 
    if ( headeredObject.getHeader().hasAttribute( "PC_coordinate" ) )
    {

      try
      {

        headeredObject.getHeader().getAttribute( "PC_coordinate",
                                                 genericObjectList );
        integerPosteriorCommissure.x =
          ( int32_t )genericObjectList[ 0 ]->getScalar();
        integerPosteriorCommissure.y =
          ( int32_t )genericObjectList[ 1 ]->getScalar();
        integerPosteriorCommissure.z =
          ( int32_t )genericObjectList[ 2 ]->getScalar();

      }
      catch ( std::exception& )
      {

        std::vector< int32_t > tmp;
        headeredObject.getHeader().getAttribute( "PC_coordinate",
                                                 tmp );
        integerPosteriorCommissure.x = tmp[ 0 ];
        integerPosteriorCommissure.y = tmp[ 1 ];
        integerPosteriorCommissure.z = tmp[ 2 ];
     


      }
 
    }
    else
    {
 
      throw std::runtime_error( "'PC_coordinate' attribute not found" );
 
    }
 
    if ( headeredObject.getHeader().hasAttribute( "IH_coordinate" ) )
    {

      try
      {

        headeredObject.getHeader().getAttribute( "IH_coordinate",
                                                 genericObjectList );
        integerInterHemispheric.x =
          ( int32_t )genericObjectList[ 0 ]->getScalar();
        integerInterHemispheric.y =
          ( int32_t )genericObjectList[ 1 ]->getScalar();
        integerInterHemispheric.z =
          ( int32_t )genericObjectList[ 2 ]->getScalar();

      }
      catch ( std::exception& )
      {

        std::vector< int32_t > tmp;
        headeredObject.getHeader().getAttribute( "IH_coordinate",
                                                 tmp );
        integerInterHemispheric.x = tmp[ 0 ];
        integerInterHemispheric.y = tmp[ 1 ];
        integerInterHemispheric.z = tmp[ 2 ];

      }
 
    }
    else
    {
 
      throw std::runtime_error( "'IH_coordinate' attribute not found" );
 
    }
 
    anteriorCommissure.x = integerAnteriorCommissure.x * xResolution;
    anteriorCommissure.y = integerAnteriorCommissure.y * yResolution;
    anteriorCommissure.z = integerAnteriorCommissure.z * zResolution;
    posteriorCommissure.x = integerPosteriorCommissure.x * xResolution;
    posteriorCommissure.y = integerPosteriorCommissure.y * yResolution;
    posteriorCommissure.z = integerPosteriorCommissure.z * zResolution;
    interHemispheric.x = integerInterHemispheric.x * xResolution;
    interHemispheric.y = integerInterHemispheric.y * yResolution;
    interHemispheric.z = integerInterHemispheric.z * zResolution;

  }
  GKG_CATCH( "gkg::AcPcIhCoordinates::AcPcIhCoordinates( "
             "gkg::HeaderedObject& headeredObject )" );

}
  

gkg::AcPcIhCoordinates::~AcPcIhCoordinates()
{
}


gkg::AcPcIhCoordinates&
gkg::AcPcIhCoordinates::operator=( const gkg::AcPcIhCoordinates& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    
    
    integerAnteriorCommissure = other.integerAnteriorCommissure;
    integerPosteriorCommissure = other.integerPosteriorCommissure;
    integerInterHemispheric = other.integerInterHemispheric;

    anteriorCommissure = other.anteriorCommissure;
    posteriorCommissure = other.posteriorCommissure;
    interHemispheric = other.interHemispheric;

    return *this;

  }
  GKG_CATCH( "gkg::AcPcIhCoordinates&"
             "gkg::AcPcIhCoordinates::operator=( "
             "const gkg::AcPcIhCoordinates& other )");

}

 
gkg::FrameTransform3d< float > 
gkg::AcPcIhCoordinates::getTalairachFrameTransform3d() const
{

  try
  {

    gkg::Vector3d< float > translationVector;

    translationVector = -anteriorCommissure;
  
    gkg::Vector3d< float > acpcVector;
    gkg::Vector3d< float > hemiVector;
    gkg::Vector3d< float > crossVector;

    acpcVector = anteriorCommissure - posteriorCommissure ;
    acpcVector.normalize(); 
    hemiVector = interHemispheric - anteriorCommissure;

    hemiVector = hemiVector - ( acpcVector * hemiVector.dot( acpcVector ) );
    hemiVector.normalize();
    
    crossVector = acpcVector.cross( hemiVector );

    gkg::FrameTransform3d< float > 
      talairachFrameTransform3d( -crossVector,
                                 acpcVector,
				 hemiVector,
                                 translationVector );

    return talairachFrameTransform3d;
  
  }
  GKG_CATCH( "gkg::FrameTransform3d< float > "
             "gkg::AcPcIhCoordinates::getTalairachFrameTransform3d() const" );


}


void gkg::AcPcIhCoordinates::addSemantic( gkg::HeaderedObject& headeredObject )
{

  try
  {
  
    headeredObject.addSemantic(
            "__acpcih_coordinates__",
            "AC_coordinate",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
    headeredObject.addSemantic(
            "__acpcih_coordinates__",
            "PC_coordinate",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
    headeredObject.addSemantic(
            "__acpcih_coordinates__",
            "IH_coordinate",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );

  }
  GKG_CATCH( "void gkg::AcPcIhCoordinates::addSemantic( "
             "gkg::HeaderedObject& headeredObject )" );

}


void gkg::AcPcIhCoordinates::addToHeaderedObject(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {
  
    gkg::AcPcIhCoordinates::addSemantic( headeredObject );
    
    // adding AC coordinate to header
    std::vector< int32_t > ac( 3 );
    ac[ 0 ] = integerAnteriorCommissure.x;
    ac[ 1 ] = integerAnteriorCommissure.y;
    ac[ 2 ] = integerAnteriorCommissure.z;
    headeredObject.getHeader().addAttribute( "AC_coordinate", ac );    

    // adding PC coordinate to header
    std::vector< int32_t > pc( 3 );
    pc[ 0 ] = integerPosteriorCommissure.x;
    pc[ 1 ] = integerPosteriorCommissure.y;
    pc[ 2 ] = integerPosteriorCommissure.z;
    headeredObject.getHeader().addAttribute( "PC_coordinate", pc );    

    // adding IH coordinate to header
    std::vector< int32_t > ih( 3 );
    ih[ 0 ] = integerInterHemispheric.x;
    ih[ 1 ] = integerInterHemispheric.y;
    ih[ 2 ] = integerInterHemispheric.z;
    headeredObject.getHeader().addAttribute( "IH_coordinate", ih );    

  }
  GKG_CATCH( "void gkg::AcPcIhCoordinates::addAcPcIhToHeaderedObject( "
             "gkg::HeaderedObject& headeredObject ) const" );
  
}
