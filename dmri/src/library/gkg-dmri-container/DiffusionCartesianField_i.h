#ifndef _gkg_dmri_container_DiffusionCartesianField_i_h_
#define _gkg_dmri_container_DiffusionCartesianField_i_h_


#include <gkg-dmri-container/DiffusionCartesianField.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianFieldLoopContext_i.h>
#include <gkg-dmri-container/DiffusionCartesianFieldGauge.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <list>
#include <iomanip>



template < class T >
inline
gkg::DiffusionCartesianField< T >::DiffusionCartesianField(
                                    int32_t sizeX,
                                    int32_t sizeY,
                                    int32_t sizeZ,
                                    const gkg::Vector3d< double >& resolution,
                                    const gkg::SiteMap< int32_t,
                                                        int32_t >& siteMap,
                                    int32_t offsetX,
                                    int32_t offsetY,
                                    int32_t offsetZ )
                                  : gkg::CartesianField< T >( sizeX, 
							      sizeY, sizeZ,
                                                              resolution,
                                                              siteMap,
                                                              offsetX,
                                                              offsetY,
                                                              offsetZ ),
                                    _functor( 0 )
{
}


template < class T >
inline
gkg::DiffusionCartesianField< T >::DiffusionCartesianField(
                                int32_t sizeX,
				int32_t sizeY,
				int32_t sizeZ,
                                const gkg::Vector3d< double >& resolution,
                                const gkg::SiteMap< int32_t, int32_t >& siteMap,
				const gkg::TextureMap< T >& items,
				int32_t offsetX,
				int32_t offsetY,
				int32_t offsetZ )
  : gkg::CartesianField< T >( sizeX, sizeY, sizeZ,
			      resolution,
			      siteMap,
			      items,
			      offsetX,
			      offsetY,
			      offsetZ ),
  _functor( 0 )
{
}


template < class T >
inline
gkg::DiffusionCartesianField< T >::DiffusionCartesianField(
			       const gkg::SiteMap< int32_t, int32_t >& siteMap,
			       const gkg::TextureMap< T >& items )
  : gkg::CartesianField< T >( siteMap, items ),
  _functor( 0 )
{
}


template < class T >
inline
gkg::DiffusionCartesianField< T >::DiffusionCartesianField(
                                 gkg::RCPointer< gkg::Volume< float > > t2,
                                 gkg::RCPointer< gkg::Volume< float > > dw,
                                 gkg::RCPointer< gkg::Volume< int16_t > > mask )
                                  : gkg::CartesianField< T >(),
                                    _t2( t2 ),
                                    _dw( dw ),
                                    _mask( mask ),
                                    _functor( 0 )
{

  try
  {

    // T2/DW/Mask sanity checks
    doT2DwMaskSanityChecks();

    // collecting DW gradient orientation set
    setInputOrientationSet();

    // allocating LUT volume
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    if ( _t2->getHeader().hasAttribute( "resolutionX" ) )
    {

      _t2->getHeader().getAttribute( "resolutionX", resolution.x );

    }
    if ( _t2->getHeader().hasAttribute( "resolutionY" ) )
    {

      _t2->getHeader().getAttribute( "resolutionY", resolution.y );

    }
    if ( _t2->getHeader().hasAttribute( "resolutionZ" ) )
    {

      _t2->getHeader().getAttribute( "resolutionZ", resolution.z );

    }
    this->CartesianField< T >::allocateLut( _t2->getSizeX(),
                                            _t2->getSizeY(),
                                            _t2->getSizeZ(),
                                            resolution );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::DiffusionCartesianField< T >::DiffusionCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask )" );

}


template < class T >
inline
gkg::DiffusionCartesianField< T >::DiffusionCartesianField(
                                 gkg::RCPointer< gkg::Volume< float > > t2,
                                 gkg::RCPointer< gkg::Volume< float > > dw,
                                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                                 const gkg::BoundingBox< float >& boundingBox,
                                 const gkg::Vector3d< float >& superResolution )
                                  : gkg::CartesianField< T >(),
                                    _t2( t2 ),
                                    _dw( dw ),
                                    _mask( mask ),
                                    _functor( 0 )
{

  try
  {

    // T2/DW/Mask sanity checks
    doT2DwMaskSanityChecks();

    // collecting DW gradient orientation set
    setInputOrientationSet();

    // allocating ODF volume
    int32_t sizeX = ( int32_t )( ( boundingBox.getUpperX() -
                                   boundingBox.getLowerX() ) /
                                 superResolution.x ) + 1;
    int32_t sizeY = ( int32_t )( ( boundingBox.getUpperY() -
                                   boundingBox.getLowerY() ) /
                                 superResolution.y ) + 1;
    int32_t sizeZ = ( int32_t )( ( boundingBox.getUpperZ() -
                                   boundingBox.getLowerZ() ) /
                                 superResolution.z ) + 1;
    if ( ( boundingBox.getLowerT() != 0.0 ) ||
         ( boundingBox.getUpperT() != 0.0 ) )
    {

      throw std::runtime_error( "bounding box upper and lower T must be nil" );

    }
    this->CartesianField< T >::allocateLut(
                                 sizeX, sizeY, sizeZ,
                                 gkg::Vector3d< double >( superResolution.x,
                                                          superResolution.y,
                                                          superResolution.z ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::DiffusionCartesianField< T >::DiffusionCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution )" );

}


template < class T >
inline
gkg::DiffusionCartesianField< T >::DiffusionCartesianField(
                                gkg::RCPointer< gkg::Volume< float > > t2,
                                gkg::RCPointer< gkg::Volume< float > > dw,
                                gkg::RCPointer< gkg::Volume< int16_t > > mask,
                                const gkg::BoundingBox< int32_t >& boundingBox )
                                 : gkg::CartesianField< T >(
                                                      boundingBox.getLowerX(),
                                                      boundingBox.getLowerY(),
                                                      boundingBox.getLowerZ() ),
                                   _t2( t2 ),
                                   _dw( dw ),
                                   _mask( mask ),
                                   _functor( 0 )
{

  try
  {

    // T2/DW/Mask sanity checks
    doT2DwMaskSanityChecks();

    // collecting DW gradient orientation set
    setInputOrientationSet();

    // allocating ODF volume
    int32_t sizeX = boundingBox.getUpperX() - boundingBox.getLowerX() + 1;
    int32_t sizeY = boundingBox.getUpperY() - boundingBox.getLowerY() + 1;
    int32_t sizeZ = boundingBox.getUpperZ() - boundingBox.getLowerZ() + 1;
    if ( ( boundingBox.getLowerT() != 0 ) ||
         ( boundingBox.getUpperT() != 0 ) )
    {

      throw std::runtime_error( "bounding box upper and lower T must be nil" );

    }
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    if ( _t2->getHeader().hasAttribute( "resolutionX" ) )
    {

      _t2->getHeader().getAttribute( "resolutionX", resolution.x );

    }
    if ( _t2->getHeader().hasAttribute( "resolutionY" ) )
    {

      _t2->getHeader().getAttribute( "resolutionY", resolution.y );

    }
    if ( _t2->getHeader().hasAttribute( "resolutionZ" ) )
    {

      _t2->getHeader().getAttribute( "resolutionZ", resolution.z );

    }
    this->CartesianField< T >::_offsetX = boundingBox.getLowerX();
    this->CartesianField< T >::_offsetY = boundingBox.getLowerY();
    this->CartesianField< T >::_offsetZ = boundingBox.getLowerZ();
    this->CartesianField< T >::allocateLut( sizeX, sizeY, sizeZ, resolution );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::DiffusionCartesianField< T >::DiffusionCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox )" );

}


template < class T >
inline
gkg::DiffusionCartesianField< T >::~DiffusionCartesianField( )
{

  delete _functor;

}


template < class T >
inline
const gkg::Volume< float >& 
gkg::DiffusionCartesianField< T >::getT2Volume() const
{

  return *_t2;

}


template < class T >
inline
const gkg::Volume< float >& 
gkg::DiffusionCartesianField< T >::getDWVolume() const
{

  return *_dw;

}


template < class T >
inline
const gkg::Volume< int16_t >& 
gkg::DiffusionCartesianField< T >::getMaskVolume() const
{

  return *_mask;

}


template < class T >
inline
const gkg::RCPointer< gkg::Volume< float > >& 
gkg::DiffusionCartesianField< T >::getRCPointerOfT2Volume() const
{

  try
  {

    return _t2;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const gkg::RCPointer< gkg::Volume< float > >& "
             "gkg::DiffusionCartesianField< T >::getRCPointerOfT2Volume() "
             "const" );

}


template < class T >
inline
const gkg::RCPointer< gkg::Volume< float > >& 
gkg::DiffusionCartesianField< T >::getRCPointerOfDWVolume() const
{

  try
  {

    return _dw;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const gkg::RCPointer< gkg::Volume< float > >& "
             "gkg::DiffusionCartesianField< T >::getRCPointerOfDWVolume() "
             "const" );

}


template < class T >
inline
const gkg::RCPointer< gkg::Volume< int16_t > >& 
gkg::DiffusionCartesianField< T >::getRCPointerOfMaskVolume() const
{

  try
  {

    return _mask;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const gkg::RCPointer< gkg::Volume< int16_t > >& "
             "gkg::DiffusionCartesianField< T >::getRCPointerOfMaskVolume() "
             "const" );

}


template < class T >
inline
const gkg::OrientationSet& 
gkg::DiffusionCartesianField< T >::getInputOrientationSet() const
{

  return _inputOrientationSet;

}


template < class T >
inline
const std::vector< float >& 
gkg::DiffusionCartesianField< T >::getBValues() const
{

  return _bValues;

}


template < class T >
inline
void gkg::DiffusionCartesianField< T >::setFunctor(
                             gkg::DiffusionCartesianFieldFunctor< T >* functor )
{

  try
  {

    _functor = functor;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::DiffusionCartesianField< T >::setFunctor( "
             "gkg::DiffusionCartesianFieldFunctor< T >* functor )" );

}


template < class T >
inline
gkg::DiffusionCartesianFieldFunctor< T >* 
gkg::DiffusionCartesianField< T >::getFunctor() const
{

  try
  {

    return _functor;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::DiffusionCartesianFieldFunctor* "
             "gkg::DiffusionCartesianField< T >::getFunctor() const" );

}


template < class T >
inline
void gkg::DiffusionCartesianField< T >::doT2DwMaskSanityChecks()
{

  try
  {

    int32_t sizeX = _t2->getSizeX();
    int32_t sizeY = _t2->getSizeY();
    int32_t sizeZ = _t2->getSizeZ();

    double resolutionX = 0.0;
    double resolutionY = 0.0;
    double resolutionZ = 0.0;
    if ( _t2->getHeader().hasAttribute( "resolutionX" ) )
    {

      _t2->getHeader().getAttribute( "resolutionX", resolutionX );

    }
    if ( _t2->getHeader().hasAttribute( "resolutionY" ) )
    {

      _t2->getHeader().getAttribute( "resolutionY", resolutionY );

    }
    if ( _t2->getHeader().hasAttribute( "resolutionZ" ) )
    {

      _t2->getHeader().getAttribute( "resolutionZ", resolutionZ );

    }

    if ( ( _dw->getSizeX() != sizeX ) ||
         ( _dw->getSizeY() != sizeY ) ||
         ( _dw->getSizeZ() != sizeZ ) ||
         ( _mask->getSizeX() != sizeX ) ||
         ( _mask->getSizeY() != sizeY ) ||
         ( _mask->getSizeZ() != sizeZ ) ||
         ( _t2->getSizeT() != 1 ) ||
         ( _mask->getSizeT() != 1 ) )
    {

      throw std::runtime_error( "bad T2/DW/Mask size(s)" );

    }

    double tmpResolutionX = 0.0;
    double tmpResolutionY = 0.0;
    double tmpResolutionZ = 0.0;
    if ( _dw->getHeader().hasAttribute( "resolutionX" ) )
    {

      _dw->getHeader().getAttribute( "resolutionX", tmpResolutionX );

    }
    if ( _dw->getHeader().hasAttribute( "resolutionY" ) )
    {

      _dw->getHeader().getAttribute( "resolutionY", tmpResolutionY );

    }
    if ( _dw->getHeader().hasAttribute( "resolutionZ" ) )
    {

      _dw->getHeader().getAttribute( "resolutionZ", tmpResolutionZ );

    }
    if ( ( tmpResolutionX != resolutionX ) ||
         ( tmpResolutionY != resolutionY ) ||
         ( tmpResolutionZ != resolutionZ ) )
    {

      throw std::runtime_error( "DW and T2 avec different resolution(s)" );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::DiffusionCartesianField< T >::"
             "doT2DwMaskSanityChecks()" );

}


template < class T >
inline
void gkg::DiffusionCartesianField< T >::setInputOrientationSet()
{

  try
  {

    int32_t orientationCount = _dw->getSizeT();

    if ( _dw->getHeader().hasAttribute( "bvalues" ) )
    {

      try
      {

        std::vector< float > bValues( orientationCount );
        _dw->getHeader().getAttribute( "bvalues", bValues );

        _bValues = std::vector< float >( orientationCount );
        int32_t o = 0;
        for ( o = 0; o < orientationCount; o++ )
        {

          //_bValues[ o ] = ( float )genericObjectListBValues[ o ]->getScalar();
          _bValues[ o ] = bValues[ o ];

        }

      }
      catch ( std::exception& )
      {

        std::vector< double > bValues( orientationCount );
        _dw->getHeader().getAttribute( "bvalues", bValues );

        _bValues = std::vector< float >( orientationCount );
        int32_t o = 0;
        for ( o = 0; o < orientationCount; o++ )
        {

          _bValues[ o ] = ( float )bValues[ o ];

        }

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'bvalues' in DW" );

    }

    if ( _dw->getHeader().hasAttribute( "diffusion_gradient_orientations" ) )
    {

      gkg::GenericObjectList genericObjectListOrientations( orientationCount );
      _dw->getHeader().getAttribute( "diffusion_gradient_orientations",
                                     genericObjectListOrientations );
      std::vector< gkg::Vector3d< float > > orientations( orientationCount );
      int32_t o = 0;
      for ( o = 0; o < orientationCount; o++ )
      {

        gkg::GenericObjectList tmp = genericObjectListOrientations[ o ]->
                                  template getValue< gkg::GenericObjectList >();
        orientations[ o ].x = ( float )tmp[ 0 ]->getScalar();
        orientations[ o ].y = ( float )tmp[ 1 ]->getScalar();
        orientations[ o ].z = ( float )tmp[ 2 ]->getScalar();

      }

      _inputOrientationSet = gkg::OrientationSet( orientations );

    }
    else
    {

      throw std::runtime_error(
                        "missing attribute 'diffusion_gradient_orientations'" );

    }


  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::DiffusionCartesianField< T >::"
             "setInputOrientationSet()" );

}


template < class T >
inline
void gkg::DiffusionCartesianField< T >::calculatingOnGrid(
                                 const gkg::BoundingBox< int32_t >& boundingBox,
                                 bool verbose )

{

  try
  {
    int32_t sizeX = _t2->getSizeX();
    int32_t sizeY = _t2->getSizeY();
    int32_t sizeZ = _t2->getSizeZ();

    int32_t x, y, z;
    
    // counting the number of sites
    int32_t siteCount = 0;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( ( *_mask )( x, y, z ) && boundingBox.contains( x, y, z ) )
          {

            ++ siteCount;

          }

        }

      }

    }

    // filling sites
    std::vector< gkg::Vector3d< int32_t > > sites( siteCount );

    // filling sites
    int32_t siteIndex = 0;
    
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {


          if ( ( *_mask )( x, y, z ) && boundingBox.contains( x, y, z ) )
          {

            gkg::Vector3d< int32_t >& site = sites[ siteIndex ];
            site.x = x;
            site.y = y;
            site.z = z;
            siteIndex ++;

          }

        }

      }

    }

    // adding sites to Cartesian field 
    this->CartesianField< T >::_siteMap.addSites( 0, sites );

    // creating (rank,site)<->index LUT
    this->CartesianField< T >::_rankSiteLut = 
      gkg::RankSiteLut< int32_t, int32_t >(
                                          this->CartesianField< T >::_siteMap );

    // creating items and filling LUT
    if ( !_dw->isMemoryMapped() )
    {

      if ( verbose )
      {

        std::cout << "in memory " << std::flush;

      }
      // creating threaded loop context
      gkg::DiffusionCartesianFieldGauge gauge( ( int32_t )sites.size() );
      gkg::DiffusionCartesianFieldLoopContext< T >
        diffusionCartesianFieldLoopContext( gauge,
                                            sites,
                                            boundingBox,
                                            *this,
                                            verbose );

      // creating a threaded loop
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &diffusionCartesianFieldLoopContext,
                      0,                       // start index
                      ( int32_t )sites.size()
                    );

      // launching the threaded loop
      threadedLoop.launch();

    }
    else
    {

      if ( verbose )
      {

        std::cout << "on disk " << std::flush;

      }

      std::list< gkg::Vector3d< int32_t > >::const_iterator
        s = this->CartesianField< T >::_siteMap.getSites( 0 ).begin(),
        se = this->CartesianField< T >::_siteMap.getSites( 0 ).end();
      int32_t count = 0;
      while ( s != se )
      {

        if ( verbose )
        {

          if ( ( ( count + 1 ) % 100 == 0 ) ||
               ( count == 0 ) ||
               ( count == siteCount - 1 ) )
          {

            if ( count )
            {

              std::cout << gkg::Eraser( 28 );

            }
            std::cout << " [ " << std::setw( 10 ) << count + 1
                      << " / " << std::setw( 10 ) << siteCount
                      << " ]" << std::flush;

          }

        }
 

        gkg::Vector3d< int32_t > site( s->x - boundingBox.getLowerX(),
                                       s->y - boundingBox.getLowerY(),
                                       s->z - boundingBox.getLowerZ() );

        int32_t index = this->CartesianField< T >::_rankSiteLut.getIndex( 0,
                                                                          *s );

        T& item = this->CartesianField< T >::_items.addTexture( index,
                                                                newItem(
                                                                       *s,
                                                                       site ) );
        this->CartesianField< T >::_lut( site ) = &item;
        ++ count;
        ++ s;

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 28 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::DiffusionCartesianField< T >::calculatingOnGrid( "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "bool verbose )" );

}


template < class T >
inline
void gkg::DiffusionCartesianField< T >::calculatingOnSubGrid(
                                  const gkg::BoundingBox< float >& boundingBox,
                                  const gkg::Vector3d< float >& superResolution,
                                  bool verbose )

{

  try
  {

    // getting T2/DW resolution
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    if ( _t2->getHeader().hasAttribute( "resolutionX" ) )
    {

      _t2->getHeader().getAttribute( "resolutionX", resolution.x );

    }
    if ( _t2->getHeader().hasAttribute( "resolutionY" ) )
    {

      _t2->getHeader().getAttribute( "resolutionY", resolution.y );

    }
    if ( _t2->getHeader().hasAttribute( "resolutionZ" ) )
    {

      _t2->getHeader().getAttribute( "resolutionZ", resolution.z );

    }
    gkg::BoundingBox< int32_t > t2DwBoundingBox( 0, _t2->getSizeX() - 1,
                                                 0, _t2->getSizeY() - 1,
                                                 0, _t2->getSizeZ() - 1 );
                                                 
    int32_t sizeX = this->CartesianField< T >::_lut.getSizeX();
    int32_t sizeY = this->CartesianField< T >::_lut.getSizeY();
    int32_t sizeZ = this->CartesianField< T >::_lut.getSizeZ();
    int32_t x, y, z;

    // counting the number of sites
    int32_t siteCount = 0;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          gkg::Vector3d< float > site( boundingBox.getLowerX() +
                                       x * superResolution.x,
                                       boundingBox.getLowerY() +
                                       y * superResolution.y,
                                       boundingBox.getLowerZ() +
                                       z * superResolution.z );

          int32_t fromX = ( int32_t )( site.x / resolution.x );
          int32_t fromY = ( int32_t )( site.y / resolution.y );
          int32_t fromZ = ( int32_t )( site.z / resolution.z );

          if ( t2DwBoundingBox.contains( fromX, fromY, fromZ ) &&
               t2DwBoundingBox.contains( fromX, fromY, fromZ + 1 ) &&
               t2DwBoundingBox.contains( fromX, fromY + 1, fromZ ) &&
               t2DwBoundingBox.contains( fromX, fromY + 1, fromZ + 1 ) &&
               t2DwBoundingBox.contains( fromX + 1, fromY, fromZ ) &&
               t2DwBoundingBox.contains( fromX + 1, fromY, fromZ + 1 ) &&
               t2DwBoundingBox.contains( fromX + 1, fromY + 1, fromZ ) &&
               t2DwBoundingBox.contains( fromX + 1, fromY + 1, fromZ + 1 ) &&
               ( *_mask )( fromX, fromY, fromZ ) &&
               ( *_mask )( fromX, fromY, fromZ + 1 ) &&
               ( *_mask )( fromX, fromY + 1, fromZ ) &&
               ( *_mask )( fromX, fromY + 1, fromZ + 1 ) &&
               ( *_mask )( fromX + 1, fromY, fromZ ) &&
               ( *_mask )( fromX + 1, fromY, fromZ + 1 ) &&
               ( *_mask )( fromX + 1, fromY + 1, fromZ ) &&
               ( *_mask )( fromX + 1, fromY + 1, fromZ + 1 ) )
          {

            ++ siteCount;

          }

        }

      }

    }

    // filling sites
    std::vector< gkg::Vector3d< int32_t > > sites( siteCount );
    int32_t siteIndex = 0;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          gkg::Vector3d< float > site( boundingBox.getLowerX() +
                                       x * superResolution.x,
                                       boundingBox.getLowerY() +
                                       y * superResolution.y,
                                       boundingBox.getLowerZ() +
                                       z * superResolution.z );

          int32_t fromX = ( int32_t )( site.x / resolution.x );
          int32_t fromY = ( int32_t )( site.y / resolution.y );
          int32_t fromZ = ( int32_t )( site.z / resolution.z );

          if ( t2DwBoundingBox.contains( fromX, fromY, fromZ ) &&
               t2DwBoundingBox.contains( fromX, fromY, fromZ + 1 ) &&
               t2DwBoundingBox.contains( fromX, fromY + 1, fromZ ) &&
               t2DwBoundingBox.contains( fromX, fromY + 1, fromZ + 1 ) &&
               t2DwBoundingBox.contains( fromX + 1, fromY, fromZ ) &&
               t2DwBoundingBox.contains( fromX + 1, fromY, fromZ + 1 ) &&
               t2DwBoundingBox.contains( fromX + 1, fromY + 1, fromZ ) &&
               t2DwBoundingBox.contains( fromX + 1, fromY + 1, fromZ + 1 ) &&
               ( *_mask )( fromX, fromY, fromZ ) &&
               ( *_mask )( fromX, fromY, fromZ + 1 ) &&
               ( *_mask )( fromX, fromY + 1, fromZ ) &&
               ( *_mask )( fromX, fromY + 1, fromZ + 1 ) &&
               ( *_mask )( fromX + 1, fromY, fromZ ) &&
               ( *_mask )( fromX + 1, fromY, fromZ + 1 ) &&
               ( *_mask )( fromX + 1, fromY + 1, fromZ ) &&
               ( *_mask )( fromX + 1, fromY + 1, fromZ + 1 ) )
          {

            sites[ siteIndex ] = gkg::Vector3d< int32_t >( x, y, z );
            siteIndex ++;

          }

        }

      }

    }

    // adding sites to Cartesian field 
    this->CartesianField< T >::_siteMap.addSites( 0, sites );

    // creating (rank,site)<->index LUT
    this->CartesianField< T >::_rankSiteLut =
      gkg::RankSiteLut< int32_t, int32_t >( 
                                          this->CartesianField< T >::_siteMap );

    // creating items and filling LUT
    if ( !_dw->isMemoryMapped() )
    {

      // creating threaded loop context
      gkg::DiffusionCartesianFieldGauge gauge( ( int32_t )sites.size() );
      gkg::DiffusionCartesianFieldLoopContext< T >
        diffusionCartesianFieldLoopContext( gauge,
                                            sites,
                                            boundingBox,
                                            superResolution,
                                            *this,
                                            verbose );

      // creating a threaded loop
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &diffusionCartesianFieldLoopContext,
                      0,                       // start index
                      ( int32_t )sites.size() );

      // launching the threaded loop
      threadedLoop.launch();

    }
    else
    {

      // creating items and filling LUT
      std::list< gkg::Vector3d< int32_t > >::const_iterator
        s = this->CartesianField< T >::_siteMap.getSites( 0 ).begin(),
        se = this->CartesianField< T >::_siteMap.getSites( 0 ).end();
      int32_t count = 0;
      while ( s != se )
      {

        if ( verbose )
        {

          if ( ( ( count + 1 ) % 100 == 0 ) ||
               ( count == 0 ) ||
               ( count == siteCount - 1 ) )
          {

            if ( count )
            {

              std::cout << gkg::Eraser( 28 );

            }
            std::cout << " [ " << std::setw( 10 ) << count + 1
                      << " / " << std::setw( 10 ) << siteCount
                      << " ]" << std::flush;

          }

        }

        gkg::Vector3d< float > site( boundingBox.getLowerX() +
                                     s->x * superResolution.x,
                                     boundingBox.getLowerY() +
                                     s->y * superResolution.y,
                                     boundingBox.getLowerZ() +
                                     s->z * superResolution.z );

        int32_t index = this->CartesianField< T >::_rankSiteLut.getIndex( 0,
                                                                          *s );
        T& item = this->CartesianField< T >::_items.addTexture( index,
                                                                newItem( site,
                                                                         *s ) );
        this->CartesianField< T >::_lut( *s ) = &item;
        ++ count;
        ++ s;

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 28 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::DiffusionCartesianField< T >::calculatingOnSubGrid( "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "bool verbose )" );

}


template < class T >
inline
float 
gkg::DiffusionCartesianField< T >::getMinimumAngleOfInputOrientationSet() const
{

  try
  {

    float minimumAngle = 0.0;

    gkg::GenericObjectList genericObjectListOrientations( _dw->getSizeT() );
    if ( _dw->getHeader().hasAttribute( "diffusion_gradient_orientations" ) )
    {

      _dw->getHeader().getAttribute( "diffusion_gradient_orientations",
                                    genericObjectListOrientations );

    }
    else
    {

      throw std::runtime_error(
          "missing attribute 'diffusion_gradient_orientations' in DW data" );

    }
    std::vector< gkg::Vector3d< float > > orientations( _dw->getSizeT() );
    int32_t o;
    for ( o = 0; o < _dw->getSizeT(); o++ )
    {

      gkg::GenericObjectList tmp =
      genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList >();
      orientations[ o ].x = ( float )tmp[ 0 ]->getScalar();
      orientations[ o ].y = ( float )tmp[ 1 ]->getScalar();
      orientations[ o ].z = ( float )tmp[ 2 ]->getScalar();

    }
    gkg::OrientationSet 
      inputOrientationSet = gkg::OrientationSet( orientations );
    inputOrientationSet.getStatistics( &minimumAngle, 0, 0, 0 );

    return minimumAngle;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "float  "
             "gkg::DiffusionCartesianField< T >::"
             "getMinimumAngleOfInputOrientationSet() const" );

}


template < class T >
inline
T gkg::DiffusionCartesianField< T >::newItem(
                                 const gkg::Vector3d< int32_t >& /*originSite*/,
                                 const gkg::Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    throw std::runtime_error(
                             "this function must not be used in this context" );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T gkg::DiffusionCartesianField< T >::newItem( "
             "const gkg::Vector3d< int32_t >& /*originSite*/, "
             "const Vector3d< int32_t >& /*lutSite*/ )" );

}


template < class T >
inline
T gkg::DiffusionCartesianField< T >::newItem(
                                   const gkg::Vector3d< float >& /*originSite*/,
                                   const gkg::Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    throw std::runtime_error(
                             "this function must not be used in this context" );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T gkg::DiffusionCartesianField< T >::newItem( "
             "const gkg::Vector3d< float >& /*originSite*/, "
             "const gkg::Vector3d< int32_t >& /*lutSite*/ )" );

}


#endif
