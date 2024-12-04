#ifndef _gkg_processing_transform_TalairachNormalization3d_i_h_
#define _gkg_processing_transform_TalairachNormalization3d_i_h_


#include <gkg-processing-transform/TalairachNormalization3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-transform/AcPcIhCoordinates.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <vector>
#include <gkg-core-exception/Exception.h>


template < class T >
template < class V > 
gkg::TalairachNormalization3d< T >::TalairachNormalization3d(
                               const gkg::Volume< V >& volume,
                               const gkg::TestFunction< V >& testFunction,
                               const gkg::AcPcIhCoordinates& acPcIhCoordinates )
                                   : gkg::Scaling3d< T >()
{

  try
  {

    gkg::FrameTransform3d< T > talairachFrameTransform3d =
                               acPcIhCoordinates.getTalairachFrameTransform3d();
						 
    
    T resolutionX = ( T )acPcIhCoordinates.anteriorCommissure.x / 
                    ( T )acPcIhCoordinates.integerAnteriorCommissure.x;
    T resolutionY = ( T )acPcIhCoordinates.anteriorCommissure.y / 
                    ( T )acPcIhCoordinates.integerAnteriorCommissure.y;
    T resolutionZ = ( T )acPcIhCoordinates.anteriorCommissure.z /
                    ( T )acPcIhCoordinates.integerAnteriorCommissure.z;
  
    int32_t i, j, k;
    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    
    T xmax = ( T )std::numeric_limits< int32_t >::lowest() * resolutionX;
    T ymin = ( T )std::numeric_limits< int32_t >::max() * resolutionY; 
    T zmax = ( T )std::numeric_limits< int32_t >::lowest() * resolutionZ;
    
    gkg::Vector3d< T > to;
    gkg::Vector3d< T > from;
    for( k = 0; k < sizeZ; k++ )
    {
    
      for( j = 0; j < sizeY; j++ )
      {
      
        for( i = 0; i < sizeX; i++ )
	{
	
	  if ( testFunction.getState( volume( i, j, k ) ) )
	  {
	  
	    from.x = ( T ) i * resolutionX;
	    from.y = ( T ) j * resolutionY;
	    from.z = ( T ) k * resolutionZ;

	    talairachFrameTransform3d.getDirect( from, to );

            to.x = std::fabs( to.x );
	    
	    if ( to.x > xmax )
	    { 
	    
	      xmax = to.x;
	      
	    }
	    if ( to.y < ymin )
	    { 
	    
	      ymin = to.y;
	      
	    }
	    if ( to.z > zmax )
	    { 
	    
	      zmax = to.z;

	    }

	  }

        }

      }
	
    }
    if ( ( xmax <= 0 ) || ( ymin == 0 ) || ( zmax <= 0 ) )
    {

      throw std::runtime_error( "one of the bounding box size is nul" );

    }
    this->setDirectScaling( ( T )( 1.0 / xmax ),
                            ( T )( 1.0 / std::fabs( ymin ) ),
                            ( T )( 1.0 / zmax ),
                            true,
                            false );

  }
  GKG_CATCH( "template < class T > "
             "template < class V > "
             "gkg::TalairachNormalization3d< T >::TalairachNormalization3d( "
             "const gkg::Volume< V >& volume, "
             "const gkg::TestFunction< V >& testFunction, "
             "const gkg::AcPcIhCoordinates& acPcIhCoordinates )" );

}


#endif
