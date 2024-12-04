#include <gkg-deep-nuclei-midsagittalplane/MidSagittalPlaneFunction.h>
#include <gkg-deep-nuclei-midsagittalplane/MidSagittalPlaneContext_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>

#include <cmath>


template < class T >
gkg::MidSagittalPlaneFunction< T >::MidSagittalPlaneFunction(
                                          int32_t parameterCount,
				          const gkg::Vector3d< double >& point1,
				          const gkg::Vector3d< double >& point2,
				          const gkg::Vector3d< double >& point3,
				          const gkg::Volume< T >& mask )
                             : gkg::NelderMeadSimplexFunction( parameterCount ),
                               _point2( point2 ),
                               _mask( mask ),
                               _boundingBox( mask )
{

  try
  {
  
    // sanity check
    if ( ( parameterCount != 3 ) && ( parameterCount != 6 ) )
    {
  
      throw std::runtime_error( "invalid parameterCount value" );
  
    }

    _point21.x = point2.x - point1.x;
    _point21.y = point2.y - point1.y;
    _point21.z = point2.z - point1.z;
    
    _point31.x = point3.x - point1.x;
    _point31.y = point3.y - point1.y;
    _point31.z = point3.z - point1.z;
  
  }
  GKG_CATCH( "template < class T > "
             "gkg::MidSagittalPlaneFunction< T >::MidSagittalPlaneFunction( "
             "int32_t parameterCount, "
             "const gkg::Vector3d< double >& point1, "
             "const gkg::Vector3d< double >& point2, "
             "const gkg::Vector3d< double >& point3, "
             "const gkg::Volume< T >& mask )" );

}


// Our correction is based on the Maximum Likelihood estimation.
// To obtain this maximum, we need to minimize a so-called energy.
// Here are the main equations of our process:
template < class T >
double gkg::MidSagittalPlaneFunction< T >::getValueAt(
                                          const gkg::Vector& parameters ) const
{

  try
  {
    
    gkg::Vector3d< double > resolution;
    int32_t sizeZ = _mask.getSizeZ();
    
    _mask.getResolution( resolution ); 
   
    gkg::Vector3d< double > p2, v1, v2;
    
    if ( parameters.getSize() == 3 )
    {
    
      double theta1 = parameters( 0 );
      double theta2 = parameters( 1 );
      double alpha = parameters( 2 );
      double c1 = std::cos( theta1 );
      double s1 = std::sin( theta1 );
      double c2 = std::cos( theta2 );
      double s2 = std::sin( theta2 );
      double c1c2 = c1 * c2;
      double c1s2 = c1 * s2;
      double s1c2 = s1 * c2;
      double s1s2 = s1 * s2;
      
      p2.x = ( _point2.x + alpha ) * c1c2 - _point2.y * s1 + _point2.z * c1s2;
      p2.y = ( _point2.x + alpha ) * s1c2 + _point2.y * c1 + _point2.z * s1s2;
      p2.z = -( _point2.x + alpha ) * s2 + _point2.z * c2;
      
      v1.x = _point21.x * c1c2 - _point21.y * s1 + _point21.z * c1s2;
      v1.y = _point21.x * s1c2 + _point21.y * c1 + _point21.z * s1s2;
      v1.z = -_point21.x * s2 + _point21.z * c2;
      
      v2.x = _point31.x * c1c2 - _point31.y * s1 + _point31.z * c1s2;
      v2.y = _point31.x * s1c2 + _point31.y * c1 + _point31.z * s1s2;
      v2.z = -_point31.x * s2 + _point31.z * c2;
    
    }
    else
    {
    
      double theta1 = parameters( 0 );
      double theta2 = parameters( 1 );
      double theta3 = parameters( 2 );
      double alpha = parameters( 3 );
      double beta = parameters( 4 );
      double delta = parameters( 5 );
      double c1 = std::cos( theta1 );
      double s1 = std::sin( theta1 );
      double c2 = std::cos( theta2 );
      double s2 = std::sin( theta2 );
      double c3 = std::cos( theta3 );
      double s3 = std::sin( theta3 );
      double c1c2 = c1 * c2;
      double s2s3 = s2 * s3;
      double s2c3 = s2 * c3;
      double c1s2s3_s1c3 = c1 * s2s3 - s1 * c3;
      double c1s2c3_s1s3 = c1 * s2c3 + s1 * s3;
      double s1c2 = s1 * c2;
      double s1s2s3_c1c3 = s1 * s2s3 + c1 * c3;
      double s1s2c3_c1s3 = s1 * s2c3 - c1 * s3;
      double c2s3 = c2 * s3;
      double c2c3 = c2 * c3;
      
      p2.x = ( _point2.x + alpha ) * c1c2 +
             ( _point2.y + beta ) * c1s2s3_s1c3 +
	     ( _point2.z + delta ) * c1s2c3_s1s3;
      p2.y = ( _point2.x + alpha ) * s1c2 +
             ( _point2.y + beta ) * s1s2s3_c1c3 +
	     ( _point2.z + delta ) * s1s2c3_c1s3;
      p2.z = -( _point2.x + alpha ) * s2 +
             ( _point2.y + beta ) * c2s3 +
	     ( _point2.z + delta ) * c2c3;
      
      v1.x = _point21.x * c1c2 + 
             _point21.y * c1s2s3_s1c3 + 
	     _point21.z * c1s2c3_s1s3;
      v1.y = _point21.x * s1c2 +
             _point21.y * s1s2s3_c1c3 +
	     _point21.z * s1s2c3_c1s3;
      v1.z = -_point21.x * s2 +
             _point21.y * c2s3 +
	     _point21.z * c2c3;
      
      v2.x = _point31.x * c1c2 + 
             _point31.y * c1s2s3_s1c3 + 
	     _point31.z * c1s2c3_s1s3;
      v2.y = _point31.x * s1c2 +
             _point31.y * s1s2s3_c1c3 +
	     _point31.z * s1s2c3_c1s3;
      v2.z = -_point31.x * s2 +
             _point31.y * c2s3 +
	     _point31.z * c2c3;

    }
      			  
    gkg::Vector3d< double > v3 = v1.cross( v2 ).normalize();
    gkg::Vector3d< int32_t > currentVoxel;  
    double energy = 0.0;
    double a = v3.x;
    double b = v3.y;
    double c = v3.z;
    double denominator = -0.5 * ( a * a + b * b + c * c );
    double d = -( a * p2.x + b * p2.y + c * p2.z ) / denominator;
    
    if ( std::fabs( denominator ) > 0.0 )
    { 

      gkg::MidSagittalPlaneContext< T > context( a, 
                                                 b, 
						 c, 
						 d,
						 a * resolution.x / denominator,
						 b * resolution.y / denominator,
						 c * resolution.z / denominator,
						 resolution,
						 _boundingBox,
						 _mask,
						 energy );
						 
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &context,
        	      0,
        	      sizeZ );

      threadedLoop.launch();

    }
   
    return  energy;

  }
  GKG_CATCH( "double gkg::MidSagittalPlaneMinimizationFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


template class gkg::MidSagittalPlaneFunction< int8_t >;
template class gkg::MidSagittalPlaneFunction< uint8_t >;
template class gkg::MidSagittalPlaneFunction< int16_t >;
template class gkg::MidSagittalPlaneFunction< uint16_t >;
template class gkg::MidSagittalPlaneFunction< int32_t >;
template class gkg::MidSagittalPlaneFunction< uint32_t >;
template class gkg::MidSagittalPlaneFunction< int64_t >;
template class gkg::MidSagittalPlaneFunction< uint64_t >;
template class gkg::MidSagittalPlaneFunction< float >;
template class gkg::MidSagittalPlaneFunction< double >;
