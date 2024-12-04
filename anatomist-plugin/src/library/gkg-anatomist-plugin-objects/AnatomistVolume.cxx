#include <gkg-anatomist-plugin-objects/AnatomistVolume.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-cppext/StdInt.h>

#include <cartodata/volume/volume.h>
#include <aims/rgb/rgb.h>


template < class T, class U >
gkg::AnatomistVolume< T, U >::AnatomistVolume( 
                                     gkg::RCPointer< gkg::Volume< T > > volume )
                            : anatomist::AVolume< U >( 
                                carto::rc_ptr< carto::Volume< U > >( 
                                  new carto::Volume< U >(
                                                   volume->getSizeX(),
                                                   volume->getSizeY(),
                                                   volume->getSizeZ(),
                                                   volume->getSizeT(),
                                                   ( U* )&( *volume )( 0 ) ) ) )
{

  try
  {

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    double resolutionT = 1.0;

    volume->getResolution( resolution, resolutionT );

    std::vector< float > voxelSize;
    voxelSize.push_back( resolution.x );
    voxelSize.push_back( resolution.y );
    voxelSize.push_back( resolution.z );
    voxelSize.push_back( resolutionT );
    this->volume()->header().setProperty( "voxel_size", voxelSize );

  }
  GKG_CATCH( "template < class T, class U > "
             "gkg::AnatomistVolume< T, U >::AnatomistVolume( "
             "gkg::RCPointer< gkg::Volume< T > > volume )" );

}


template < class T, class U >
int gkg::AnatomistVolume< T, U >::CanBeDestroyed()
{

  try
  {

    return 0;

  }
  GKG_CATCH( "template < class T, class U > "
             "int gkg::AnatomistVolume< T, U >::CanBeDestroyed()" );

}


//
//  AnatomistVolume instanciation
//


template class gkg::AnatomistVolume< int8_t >;
template class gkg::AnatomistVolume< uint8_t >;
template class gkg::AnatomistVolume< int16_t >;
template class gkg::AnatomistVolume< uint16_t >;
template class gkg::AnatomistVolume< int32_t >;
template class gkg::AnatomistVolume< uint32_t >;
//template class gkg::AnatomistVolume< int64_t >;
//template class gkg::AnatomistVolume< uint64_t >;
template class gkg::AnatomistVolume< float >;
template class gkg::AnatomistVolume< double >;
template class gkg::AnatomistVolume< gkg::RGBComponent, AimsRGB >;
