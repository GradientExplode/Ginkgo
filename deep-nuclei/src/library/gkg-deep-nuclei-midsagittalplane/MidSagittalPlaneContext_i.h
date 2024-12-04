#ifndef _gkg_deep_nuclei_midsagittalplane_MidSagittalPlaneContext_i_h_
#define _gkg_deep_nuclei_midsagittalplane_MidSagittalPlaneContext_i_h_


#include <gkg-deep-nuclei-midsagittalplane/MidSagittalPlaneContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::MidSagittalPlaneContext< T >::MidSagittalPlaneContext(
                             	      const double a,
                             	      const double b,
			     	      const double c,
			     	      const double d,
			     	      const double arx,
			     	      const double bry,
			     	      const double crz,
			     	      const Vector3d< double >& resolution,
			     	      const BoundingBox< int32_t >& boundingBox,
			     	      const Volume< T >& mask,
			     	      double& energy )
				 : gkg::LoopContext< int32_t >(),
                                   _a( a ),
				   _b( b ),
				   _c( c ),
				   _d( d ),
				   _arx( arx ),
				   _bry( bry ),
				   _crz( crz ),
				   _resolution( resolution ),
				   _boundingBox( boundingBox ),
				   _mask( mask ),
				   _energy( energy )
{
}


template < class T >
void gkg::MidSagittalPlaneContext< T >::doIt( int32_t startIndex, 
                                              int32_t count )
{

  try
  {
  
    int32_t stopIndex = startIndex + count;
    int32_t sizeX = _mask.getSizeX();
    int32_t sizeY = _mask.getSizeY();
    int32_t i, j, k;
    gkg::Vector3d< int32_t > currentVoxel;  
    double coeffSym;
    double energy = 0;

    for ( k = startIndex; k < stopIndex ; k++ )
    {
    
      for ( j = 0; j < sizeY; j++ )
      {

    	for ( i = 0; i < sizeX; i++ )
        {
        
    	  if ( _mask( i, j, k ) )
          {

            coeffSym = _arx * i + _bry * j + _crz * k + _d;
    	    currentVoxel.x = ( int32_t )( coeffSym * _a + i * _resolution.x ) /
        				_resolution.x;
            currentVoxel.y = ( int32_t )( coeffSym * _b + j * _resolution.y ) /
        				_resolution.y;
            currentVoxel.z = ( int32_t )( coeffSym * _c + k * _resolution.z ) /
        				_resolution.z;

    	    if ( _boundingBox.contains( currentVoxel ) && 
        	 _mask( currentVoxel ) )
    	    {
            
    	      energy -= 1.0;
              
    	    }

          }

        }
        
      }

    }
    
    lock();
    _energy += energy;
    unlock();
      
  }
  GKG_CATCH( "template < class T > "
             "void gkg::MidSagittalPlaneContext< T >::doIt( "
	     "int32_t startIndex, int32_t count )" );
  
}


#endif
