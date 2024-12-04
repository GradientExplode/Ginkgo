#ifndef _gkg_deep_nuclei_midsagittalplane_MidSagittalPlane_i_h_
#define _gkg_deep_nuclei_midsagittalplane_MidSagittalPlane_i_h_


#include <gkg-deep-nuclei-midsagittalplane/MidSagittalPlane.h>
#include <gkg-deep-nuclei-midsagittalplane/MidSagittalPlaneFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-transform/RotationX3d.h>
#include <gkg-processing-transform/RotationY3d.h>
#include <gkg-processing-transform/RotationZ3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


template < class T >
gkg::MidSagittalPlane< T >::MidSagittalPlane( int32_t parameterCount )
                          : _parameterCount( parameterCount )
{

  try
  {

    // sanity check
    if ( ( parameterCount != 3 ) && ( parameterCount != 6 ) )
    {
  
      throw std::runtime_error( "invalid parameterCount value" );
  
    }
    
    _coefficients.resize( 4 );
    
  }
  GKG_CATCH( "template < class T > "
             "gkg::MidSagittalPlane< T >::MidSagittalPlane( "
	     "int32_t parameterCount )" ); 

}


template < class T >
gkg::MidSagittalPlane< T >::~MidSagittalPlane()
{
}


template < class T >
void gkg::MidSagittalPlane< T >::compute( const gkg::Volume< T >& volumeIn,
                                          gkg::Volume< T >& volumeOut,
					  gkg::Volume< T >& mask,
					  bool output3D,
					  bool verbose )
{

  try
  {
  
    int32_t i, j, k;
    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    int32_t minX = sizeX - 1;
    int32_t maxX = 0;
    gkg::Vector3d< double > resolution;
    
    volumeIn.getResolution( resolution );
    
    // get min and max values in the X direction of the mask 
    // to provide an initial position of the plane as ( max + min ) / 2
    for ( k = 0; k < sizeZ; k++ )
    {
    
      for ( j = 0; j < sizeY; j++ )
      {
      
        for ( i = 0; i < sizeX; i++ )
	{
	
	  if ( mask( i, j, k ) )
	  {
	  
	    if ( i < minX )
	    {
	    
	      minX = i;
	    
	    }
	    if ( i > maxX )
	    {
	    
	      maxX = i;
	    
	    }
	  
	  }
	
	}
      
      }
    
    }

    int32_t slice = ( minX + maxX ) / 2;
    gkg::Vector parameters( _parameterCount );
    gkg::Vector deltaParameters( _parameterCount );
    
    if ( _parameterCount == 3 )
    {
    
      parameters( 0 ) = 0.0;
      parameters( 1 ) = 0.0;
      parameters( 2 ) = 0.0;
      deltaParameters( 0 ) = M_PI / 180.0;
      deltaParameters( 1 ) = M_PI / 180.0;
      deltaParameters( 2 ) = resolution.x;
    
    }
    else
    {
    
      parameters( 0 ) = 0.0;
      parameters( 1 ) = 0.0;
      parameters( 2 ) = 0.0;
      parameters( 3 ) = 0.0;
      parameters( 4 ) = 0.0;
      parameters( 5 ) = 0.0;
      deltaParameters( 0 ) = M_PI / 180.0;
      deltaParameters( 1 ) = M_PI / 180.0;
      deltaParameters( 2 ) = M_PI / 180.0;
      deltaParameters( 3 ) = resolution.x;
      deltaParameters( 4 ) = resolution.y;
      deltaParameters( 5 ) = resolution.z;

    }
    
    // define three points belonging to the plane
    double pX = (double)slice * resolution.x;
    double pY = (double)sizeY * resolution.y;
    double pZ = (double)sizeZ * resolution.z;
    gkg::Vector3d< double > p1( pX, 0.5 * pY, 0.2 * pZ );
    gkg::Vector3d< double > p2( pX, 0.2 * pY, 0.5 * pZ );
    gkg::Vector3d< double > p3( pX, 0.8 * pY, 0.8 * pZ );
    double testSize = 0.0;

    // Optimize plane orientation and position using
    // a symmetry ctriterion
    gkg::MidSagittalPlaneFunction< T > MSPFunction( _parameterCount, 
                                                    p1,  p2,  p3, mask );
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    factory->getNelderMeadSimplexOptimumParameters( 
                                         parameters,
                                         deltaParameters,
					 testSize,
                                         MSPFunction,
					 160,
					 1e-3,
					 verbose );

    // Retrieve the plane coefficients
    gkg::Vector3d< double > p1MSP, p2MSP, p3MSP;
    gkg::CompositeTransform3d< double > transform;
    
    if ( _parameterCount == 3 )
    {
    
      const gkg::Vector3d< double > vectTranslation ( parameters( 2 ),
        					      0 , 
        					      0 );
   
      gkg::Translation3d< double > translation( vectTranslation, true );
      gkg::RotationZ3d< double > rotationZ( parameters( 0 ), false, true );
      gkg::RotationY3d< double > rotationY( parameters( 1 ), false, true );
   
      transform.compose( translation );
      transform.compose( rotationY );
      transform.compose( rotationZ );
      
    }
    else
    {
    
      const gkg::Vector3d< double > vectTranslation ( parameters( 3 ),
        					      parameters( 4 ), 
        					      parameters( 5 ) );
   
      gkg::Translation3d< double > translation( vectTranslation, true );
      gkg::RotationZ3d< double > rotationZ( parameters( 0 ), false, true );
      gkg::RotationY3d< double > rotationY( parameters( 1 ), false, true );
      gkg::RotationY3d< double > rotationX( parameters( 2 ), false, true );
   
      transform.compose( translation );
      transform.compose( rotationX );
      transform.compose( rotationY );
      transform.compose( rotationZ );

    }

    _transformation = transform.getComposition();

    double tx = _transformation.getDirectCoefficient( 0, 3 );

    tx += pX;
    _transformation.setDirectCoefficient( 0, 3, tx );
   
    transform.getDirect( p1, p1MSP );
    transform.getDirect( p2, p2MSP );
    transform.getDirect( p3, p3MSP );

    gkg::Vector3d< double > v1( p2MSP - p1MSP );
    gkg::Vector3d< double > v2( p3MSP - p1MSP );
    gkg::Vector3d< double > v3 = v1.cross( v2 ).normalize();

    _coefficients[ 0 ] = v3.x;
    _coefficients[ 1 ] = v3.y;
    _coefficients[ 2 ] = v3.z;
    _coefficients[ 3 ] = -v3.x * p1MSP.x - v3.y * p1MSP.y - v3.z * p1MSP.z;
    
    if ( verbose )
    {
    
      std::cout << "Plane coefficients: " 
                << "a=" << _coefficients[ 0 ] << " "
	        << "b=" << _coefficients[ 1 ] << " "
	        << "c=" << _coefficients[ 2 ] << " "
	        << "d=" << _coefficients[ 3 ] << std::endl;
    }
    
    if ( output3D )
    {
    
      fill3d( volumeIn, volumeOut );
    
    }
    else
    {
    
      fill2d( volumeIn, volumeOut );
    
    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::MidSagittalPlane< T >::compute( "
	     "const gkg::Volume< T >& volumeIn, "
             "gkg::Volume< T >& volumeOut, "
	     "gkg::Volume< T >& mask, "
	     "bool output2D )" );
  
}


template < class T >
void gkg::MidSagittalPlane< T >::fill2d( const gkg::Volume< T >& volumeIn,
                                         gkg::Volume< T >& volumeOut ) const
{

  try
  {

    int32_t i, j, k;
    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    gkg::Vector3d< double > resolution;
    gkg::Vector3d< double > resolutionOut;
    
    volumeIn.getResolution( resolution );    
    resolutionOut.x = resolution.y;
    resolutionOut.y = resolution.z;
    resolutionOut.z = 1.0;
    volumeOut.reallocate( sizeY, sizeZ );
    volumeOut.setResolution( resolutionOut );
    volumeOut.fill( 0 );
    
    double a = _coefficients[ 0 ] * resolution.x;
    double b = _coefficients[ 1 ] * resolution.y;
    double c = _coefficients[ 2 ] * resolution.z;
    double d = _coefficients[ 3 ];
    
    for ( k = 0; k < sizeZ; k++ )
    {
    
      for ( j = 0; j < sizeY; j++ )
      {

    	for ( i = 0; i < sizeX; i++ )
    	{
    	
    	  double value = std::fabs( a * i + b * j  + c * k  + d );
    	    
    	  if ( value < resolution.x )
    	  {

    	    volumeOut( j, k ) = volumeIn( i, j, k );
    	    
    	  }

    	}

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::MidSagittalPlane< T >::fillImage( "
	     "const gkg::Volume< T >& volumeIn, "
             "gkg::Volume< T >& volumeOut ) const" );

}


template < class T >
void gkg::MidSagittalPlane< T >::fill3d( const gkg::Volume< T >& volumeIn,
                                         gkg::Volume< T >& volumeOut ) const
{

  try
  {

    int32_t i, j, k;
    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    gkg::Vector3d< double > resolution;    
    
    volumeIn.getResolution( resolution );
    volumeOut.reallocate( sizeX, sizeY, sizeZ );
    volumeOut.setResolution( resolution );
    volumeOut.fill( 0 );
    
    double a = _coefficients[ 0 ] * resolution.x;
    double b = _coefficients[ 1 ] * resolution.y;
    double c = _coefficients[ 2 ] * resolution.z;
    double d = _coefficients[ 3 ];

    for ( k = 0; k < sizeZ; k++ )
    {
    
      for ( j = 0; j < sizeY; j++ )
      {

    	for ( i = 0; i < sizeX; i++ )
    	{
    	
    	  double value = a * i + b * j  + c * k  + d;
    	    
    	  if ( ( value >= 0.0 ) && ( value < resolution.x ) )
    	  {

    	    volumeOut( i, j, k ) = volumeIn( i, j, k );
    	    
    	  }

    	}

      }

    }
    
  }
  GKG_CATCH( "template < class T > "
             "void gkg::MidSagittalPlane< T >::fillVolume( "
	     "const gkg::Volume< T >& volumeIn, "
             "gkg::Volume< T >& volumeOut ) const" );

}


template < class T >
std::vector< double > gkg::MidSagittalPlane< T >::getCoefficients()
{

  return _coefficients;

}

template < class T >
const gkg::HomogeneousTransform3d< double >& 
gkg::MidSagittalPlane< T >::getTransformation() const
{

  return _transformation;

}


#endif
