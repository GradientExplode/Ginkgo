#ifndef _gkg_processing_transform_TalairachTransform3d_i_h_
#define _gkg_processing_transform_TalairachTransform3d_i_h_


#include <gkg-processing-transform/TalairachTransform3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/AcPcIhCoordinates.h>
#include <gkg-processing-transform/TalairachNormalization3d_i.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
template < class V1, class V2 >
inline
gkg::TalairachTransform3d< T >::TalairachTransform3d( 
                                      const gkg::Volume< V1 >& anyVolume,
                                      const gkg::Volume< V2 >& talairachVolume )
                               : gkg::CompositeTransform3d< T >()
{

  try
  {

    // getting the resolution scaling for the 'any' volume 
    gkg::Vector3d< double > resolution;
    anyVolume.getResolution( resolution );    
    this->compose( gkg::Scaling3d< T >( ( T )resolution.x,
                                        ( T )resolution.y,
                                        ( T )resolution.z ) );

    // getting AC / PC/ IH coordinates from the 'any' volume
    gkg::Volume< V1 > copyOfAnyVolume( anyVolume );
    gkg::AcPcIhCoordinates acPcIhCoordinates( copyOfAnyVolume );

    // computing frame transform of the 'any' volume to ( Ac, AcPc, AcIh, u ) 
    // frame ( u is the vectorial product of AcPc and AcIh )
    this->compose( acPcIhCoordinates.getTalairachFrameTransform3d() );

    // getting Talairach normalization information from header
    gkg::TalairachNormalization3d< float > 
      talairachNormalization3d( copyOfAnyVolume );
    this->compose( talairachNormalization3d );

   // reading AC coordinate from the Talairach volume and processing
   // scaling transform for Talairach box
   if ( talairachVolume.getHeader().hasAttribute( "talairach_AC_coordinates" ) )
   {

     gkg::Vector3d< int32_t > talairachAnteriorCommissure;
     try
     {

       gkg::GenericObjectList talairach_AC_coordinates;
       talairachVolume.getHeader().getAttribute( "talairach_AC_coordinates",
                                                 talairach_AC_coordinates );
       talairachAnteriorCommissure.x =
         ( int32_t )( talairach_AC_coordinates[ 0 ]->getScalar() + 0.5 );
       talairachAnteriorCommissure.y =
         ( int32_t )( talairach_AC_coordinates[ 1 ]->getScalar() + 0.5 );
       talairachAnteriorCommissure.z =
         ( int32_t )( talairach_AC_coordinates[ 2 ]->getScalar() + 0.5 );

     }
     catch( std:: exception& )
     {

       std::vector< int32_t > talairach_AC_coordinates;
       talairachVolume.getHeader().getAttribute( "talairach_AC_coordinates",
                                                 talairach_AC_coordinates );
       talairachAnteriorCommissure.x = talairach_AC_coordinates[ 0 ];
       talairachAnteriorCommissure.y = talairach_AC_coordinates[ 1 ];
       talairachAnteriorCommissure.z = talairach_AC_coordinates[ 2 ];

     }
     this->compose( gkg::Scaling3d< T >(
                      gkg::Vector3d< T >(
                        ( T )talairachAnteriorCommissure.x,
                        ( T )( talairachVolume.getSizeY() -
                               talairachAnteriorCommissure.y ),
                        ( T )talairachAnteriorCommissure.z ) ) );

     // computing final frame transform where the origin is not AC anymore and 
     // vectors are oriented differently
     gkg::Vector3d< T > u( -1.0, 0.0, 0.0 );
     gkg::Vector3d< T > v( 0.0, -1.0, 0.0 );
     gkg::Vector3d< T > w( 0.0, 0.0, -1.0 );
     gkg::Vector3d< T > translation( ( T )( -talairachAnteriorCommissure.x ), 
                                     ( T )( -talairachAnteriorCommissure.y ),
	                             ( T )( -talairachAnteriorCommissure.z ) );
     this->compose( gkg::FrameTransform3d< T >( u, v, w, translation ) );

   }
   else
   {

     throw std::runtime_error(
        	             "'talairach_AC_coordinates' attribute not found" );

   }

  }
  GKG_CATCH( "template < class T > "
             "template < class V1, class V2 > "
             "inline "
             "gkg::TalairachTransform3d< T >::TalairachTransform3d( "
             "const gkg::Volume< V1 >& anyVolume, "
             "const gkg::Volume< V2 >& talairachVolume )" );

}


#endif
