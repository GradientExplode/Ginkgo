#ifndef _gkg_dmri_container_DiffusionCartesianFieldLoopContext_i_h_
#define _gkg_dmri_container_DiffusionCartesianFieldLoopContext_i_h_


#include <gkg-dmri-container/DiffusionCartesianFieldLoopContext.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-container/DiffusionCartesianFieldGauge.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


template < class T >
inline
gkg::DiffusionCartesianFieldLoopContext< T >::
                                             DiffusionCartesianFieldLoopContext(
                     gkg::DiffusionCartesianFieldGauge& gauge,
                     const std::vector< gkg::Vector3d< int32_t > >& sites,
                     const gkg::BoundingBox< int32_t >& boundingBox,
                     gkg::DiffusionCartesianField< T >& diffusionCartesianField,
                     bool& verbose )
                                             : gkg::LoopContext< int32_t >(
                                                                       &gauge ),
                                               _sites( sites ),
                                               _boundingBoxInt32( boundingBox ),
                                               _boundingBoxFloat( 0, 0, 0, 0 ),
                                               _superResolution( 0, 0, 0 ),
                                               _diffusionCartesianField(
                                                      diffusionCartesianField ),
                                               _verbose( verbose )
{

  try
  {

    _type = gkg::DiffusionCartesianFieldLoopContext< T >::CalculateOnGrid;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::DiffusionCartesianFieldLoopContext< T >:: "
             "DiffusionCartesianFieldLoopContext( "
             "gkg::DiffusionCartesianFieldGauge& gauge, "
             "const std::vector< gkg::Vector3d< int32_t > >& sites, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "gkg::DiffusionCartesianField< T >& diffusionCartesianField, "
             "bool& verbose )" );

}


template < class T >
inline
gkg::DiffusionCartesianFieldLoopContext< T >::
                                             DiffusionCartesianFieldLoopContext(
                     gkg::DiffusionCartesianFieldGauge& gauge,
                     const std::vector< gkg::Vector3d< int32_t > >& sites,
                     const gkg::BoundingBox< float >& boundingBox,
                     const gkg::Vector3d< float >& superResolution,
                     gkg::DiffusionCartesianField< T >& diffusionCartesianField,
                     bool& verbose )
                                             : gkg::LoopContext< int32_t >(
                                                                       &gauge ),
                                               _sites( sites ),
                                               _boundingBoxInt32( 0, 0, 0, 0 ),
                                               _boundingBoxFloat( boundingBox ),
                                               _superResolution(
                                                              superResolution ),
                                               _diffusionCartesianField(
                                                      diffusionCartesianField ),
                                               _verbose( verbose )
{

  try
  {

    _type = gkg::DiffusionCartesianFieldLoopContext< T >::CalculateOnSubGrid;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::DiffusionCartesianFieldLoopContext< T >:: "
             "DiffusionCartesianFieldLoopContext( "
             "gkg::DiffusionCartesianFieldGauge& gauge, "
             "const std::vector< gkg::Vector3d< int32_t > >& sites, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "gkg::DiffusionCartesianField< T >& diffusionCartesianField, "
             "bool& verbose )" );

}


template < class T >
inline
gkg::DiffusionCartesianFieldLoopContext< T >::
                                           ~DiffusionCartesianFieldLoopContext()
{
}


template < class T >
inline
void gkg::DiffusionCartesianFieldLoopContext< T >::doIt( int32_t startIndex,
                                                         int32_t count )
{

  try
  {

    T* items = new T[ count ]; 
    int32_t N = 100;
    int32_t g;
    const gkg::Vector3d< int32_t >* sitePtr = 
    		       ( const gkg::Vector3d< int32_t >* )&_sites[ startIndex ];
    
    if ( _type ==
         gkg::DiffusionCartesianFieldLoopContext< T >::CalculateOnGrid )
    {
    
      int32_t bBoxX = _boundingBoxInt32.getLowerX();
      int32_t bBoxY = _boundingBoxInt32.getLowerY();
      int32_t bBoxZ = _boundingBoxInt32.getLowerZ();
     
      for ( g = 0; g < count; g++ )
      {

        if ( _verbose && _gauge )
        {

          if ( g && !( g % N ) )
          {

            lock();
            _gauge->add( N );
            unlock();

          }

        }
	
        gkg::Vector3d< int32_t > lutSite( sitePtr->x - bBoxX,
                                          sitePtr->y - bBoxY,
                                          sitePtr->z - bBoxZ );

        items[ g ] = _diffusionCartesianField.newItem( *sitePtr++, lutSite );

      }
      
    }
    else
    {
    
      float bBoxX = _boundingBoxFloat.getLowerX();
      float bBoxY = _boundingBoxFloat.getLowerY();
      float bBoxZ = _boundingBoxFloat.getLowerZ();
    
      for ( g = 0; g < count; g++ )
      {

        if ( _verbose && _gauge )
        {

          if ( g && !( g % N ) )
          {

            lock();
            _gauge->add( N );
            unlock();

          }

        }

        gkg::Vector3d< float > originSite( bBoxX +
        				   sitePtr->x * _superResolution.x,
        				   bBoxY +
        				   sitePtr->y * _superResolution.y,
        				   bBoxZ +
        				   sitePtr->z * _superResolution.z );
					   
        items[ g ] = _diffusionCartesianField.newItem( originSite, *sitePtr++ );

      }

    }

    T* itemPtr = items;
    sitePtr = (const gkg::Vector3d< int32_t >*)&_sites[ startIndex ];
    
    lock();
    for ( g = 0; g < count; g++ )
    {
    
      _diffusionCartesianField.setItemFast( *sitePtr++, *itemPtr++ );
      
    }

    if ( _verbose && _gauge )
    {

      _gauge->add( count % N );

    }
    
    unlock();

    delete [] items;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::DiffusionCartesianFieldLoopContext< T >::doIt( "
             "int32_t startIndex, "
             "int32_t count )" );

}


#endif
