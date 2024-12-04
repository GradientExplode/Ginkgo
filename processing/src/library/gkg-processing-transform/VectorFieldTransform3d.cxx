#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-resampling/QuickResamplerFactory.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::VectorFieldTransform3d< T >::VectorFieldTransform3d(
                                    const gkg::Volume< T >& directVectorField,
                                    const gkg::Volume< T >& inverseVectorField )
                                 : gkg::Transform3d< T >()
{

  try
  {

    setDirectAndInverseVectorFields( directVectorField, inverseVectorField,
                                     true, false );

    _resampler =
      gkg::QuickResamplerFactory< T >::getInstance().getQuickResampler(
                                            "Partial Voluming QuickResampler" );


  }
  GKG_CATCH( "template < class T > "
             "gkg::VectorFieldTransform3d< T >::VectorFieldTransform3d( "
             "const gkg::Volume< T >& directVectorField, "
             "const gkg::Volume< T >& inverseVectorField )" );

}


template < class T >
gkg::VectorFieldTransform3d< T >::VectorFieldTransform3d(
                                 const gkg::VectorFieldTransform3d< T >& other )
                            : gkg::Transform3d< T >(),
                              _directVectorField( other._directVectorField ),
                              _inverseVectorField( other._inverseVectorField ),
                              _directResolution( other._directResolution ),
                              _inverseResolution( other._inverseResolution ),
                              _resampler( other._resampler )
{
}


template < class T >
gkg::VectorFieldTransform3d< T >::~VectorFieldTransform3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > 
gkg::VectorFieldTransform3d< T >::clone() const
{

  try
  {

    gkg::RCPointer< Transform3d< T > > transform;
    transform.reset( new gkg::VectorFieldTransform3d< T >( *this ) );
    return transform;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::Transform3d< T > > "
             "gkg::VectorFieldTransform3d< T >::clone() const" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::getDirect(
                                               const gkg::Vector3d< T >& from, 
                                               gkg::Vector3d< T >& to ) const
{

  try
  {

    gkg::Vector3d< T > deformation;
    _resampler->resample( _directVectorField,
                          T( 0 ),
                          from,
                          deformation.x,
                          &_directResolution,
                          0 );
    _resampler->resample( _directVectorField,
                          T( 0 ),
                          from,
                          deformation.y,
                          &_directResolution,
                          1 );
    _resampler->resample( _directVectorField,
                          T( 0 ),
                          from,
                          deformation.z,
                          &_directResolution,
                          2 );
    to.x = from.x + deformation.x;
    to.y = from.y + deformation.y;
    to.z = from.z + deformation.z;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::getDirect( "
             "const gkg::Vector3d< T >& from, "
             "gkg::Vector3d< T >& to ) const" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::getInverse(
                                              const gkg::Vector3d< T >& to, 
                                              gkg::Vector3d< T >& from ) const
{

  try
  {

    gkg::Vector3d< T > deformation;
    _resampler->resample( _inverseVectorField,
                          T( 0 ),
                          to,
                          deformation.x,
                          &_inverseResolution,
                          0 );
    _resampler->resample( _inverseVectorField,
                          T( 0 ),
                          to,
                          deformation.y,
                          &_inverseResolution,
                          1 );
    _resampler->resample( _inverseVectorField,
                          T( 0 ),
                          to,
                          deformation.z,
                          &_inverseResolution,
                          2 );
    from.x = to.x + deformation.x;
    from.y = to.y + deformation.y;
    from.z = to.z + deformation.z;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::getInverse( "
             "const gkg::Vector3d< T >& to, "
             "gkg::Vector3d< T >& from ) const" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::setDirectAndInverseVectorFields(
                                     const gkg::Volume< T >& directVectorField,
                                     const gkg::Volume< T >& inverseVectorField,
                                     bool update,
                                     bool notify )
{

  try
  {

    this->setDirectVectorField( directVectorField, false, false );
    this->setInverseVectorField( inverseVectorField, update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::"
             "setDirectAndInverseVectorFields( "
             "const gkg::Volume< T >& directAndInverseVectorFields, "
             "bool update, "
             "bool notify ))" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::setDirectVectorField(
                                     const gkg::Volume< T >& directVectorField,
                                     bool update,
                                     bool notify )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // setting direct vector field
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > directVectorFieldSize;
    int32_t directVectorFieldSizeT = 0;

    directVectorField.getSize( directVectorFieldSize, directVectorFieldSizeT );
    directVectorField.getResolution( _directResolution );

    if ( directVectorFieldSizeT != 3 )
    {

      throw std::runtime_error( "direct vector field must contain 3 volumes" );

    }

    _directVectorField = directVectorField;

    ////////////////////////////////////////////////////////////////////////////
    // updating and notifying upon request
    ////////////////////////////////////////////////////////////////////////////

    if ( update )
    {

      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::"
             "setDirectVectorField( "
             "const gkg::Volume< T >& directVectorField, "
             "bool update, "
             "bool notify ))" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::setInverseVectorField(
                                     const gkg::Volume< T >& inverseVectorField,
                                     bool update,
                                     bool notify )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // setting inverse vector field
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > inverseVectorFieldSize;
    int32_t inverseVectorFieldSizeT = 0;

    inverseVectorField.getSize( inverseVectorFieldSize,
                                inverseVectorFieldSizeT );
    inverseVectorField.getResolution( _inverseResolution );

    if ( inverseVectorFieldSizeT != 3 )
    {

      throw std::runtime_error( "inverse vector field must contain 3 volumes" );

    }

    _inverseVectorField = inverseVectorField;

    ////////////////////////////////////////////////////////////////////////////
    // updating and notifying upon request
    ////////////////////////////////////////////////////////////////////////////

    if ( update )
    {

      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::"
             "setInverseVectorField( "
             "const gkg::Volume< T >& inverseVectorField, "
             "bool update, "
             "bool notify ))" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::setDirectAndInverseVectorFields(
                                 const gkg::Volume< T >& directFlowFieldAlongX,
                                 const gkg::Volume< T >& directFlowFieldAlongY,
                                 const gkg::Volume< T >& directFlowFieldAlongZ,
                                 const gkg::Volume< T >& inverseFlowFieldAlongX,
                                 const gkg::Volume< T >& inverseFlowFieldAlongY,
                                 const gkg::Volume< T >& inverseFlowFieldAlongZ,
                                 bool update,
                                 bool notify )
{

  try
  {

    this->setDirectVectorField( directFlowFieldAlongX,
                                directFlowFieldAlongY,
                                directFlowFieldAlongZ,
                                false, false );
    this->setInverseVectorField( inverseFlowFieldAlongX,
                                 inverseFlowFieldAlongY,
                                 inverseFlowFieldAlongZ,
                                 update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::"
             "setDirectAndInverseVectorFields( "
             "const gkg::Volume< T >& directFlowFieldAlongX, "
             "const gkg::Volume< T >& directFlowFieldAlongY, "
             "const gkg::Volume< T >& directFlowFieldAlongZ, "
             "const gkg::Volume< T >& inverseFlowFieldAlongX, "
             "const gkg::Volume< T >& inverseFlowFieldAlongY, "
             "const gkg::Volume< T >& inverseFlowFieldAlongZ, "
             "bool update, "
             "bool notify ))" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::setDirectVectorField(
                                 const gkg::Volume< T >& directFlowFieldAlongX,
                                 const gkg::Volume< T >& directFlowFieldAlongY,
                                 const gkg::Volume< T >& directFlowFieldAlongZ,
                                 bool update,
                                 bool notify )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting direct flow field size and resolution along X
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > directFlowFieldAlongXSize;
    int32_t directFlowFieldAlongXSizeT = 0;

    directFlowFieldAlongX.getSize( directFlowFieldAlongXSize,
                                   directFlowFieldAlongXSizeT );
    directFlowFieldAlongX.getResolution( _directResolution );

    if ( directFlowFieldAlongXSizeT != 1 )
    {

      throw std::runtime_error(
                            "direct flow field along X must contain 1 volume" );

    }

    // sanity check for direct flow field along Y
    gkg::Vector3d< int32_t > directFlowFieldAlongYSize;
    int32_t directFlowFieldAlongYSizeT = 0;

    gkg::Vector3d< double > directResolution;
    directFlowFieldAlongY.getSize( directFlowFieldAlongYSize,
                                   directFlowFieldAlongYSizeT );
    directFlowFieldAlongY.getResolution( directResolution );


    if ( ( directFlowFieldAlongYSize != directFlowFieldAlongXSize ) ||
         ( directFlowFieldAlongYSizeT != directFlowFieldAlongXSizeT ) ||
         ( directResolution != _directResolution ) )
    {

      throw std::runtime_error( 
                "direct X and Y flow fields do not share similar size or"
                " resolution" );

    }

    // sanity check for direct flow field along Z
    gkg::Vector3d< int32_t > directFlowFieldAlongZSize;
    int32_t directFlowFieldAlongZSizeT = 0;

    directFlowFieldAlongZ.getSize( directFlowFieldAlongZSize,
                                   directFlowFieldAlongZSizeT );
    directFlowFieldAlongZ.getResolution( directResolution );


    if ( ( directFlowFieldAlongZSize != directFlowFieldAlongXSize ) ||
         ( directFlowFieldAlongZSizeT != directFlowFieldAlongXSizeT ) ||
         ( directResolution != _directResolution ) )
    {

      throw std::runtime_error( 
                "direct X and Z flow fields do not share similar size or"
                " resolution" );

    }


    // reallocating the direct and inverse flow field volume
    _directVectorField.reallocate( directFlowFieldAlongXSize, 3 );
    _directVectorField.setResolution( _directResolution );


    // copying input data to the direct and inverse flow field volume
    gkg::Vector3d< int32_t > voxel;
    for ( voxel.z = 0; voxel.z < directFlowFieldAlongXSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < directFlowFieldAlongXSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < directFlowFieldAlongXSize.x; voxel.x++ )
        {

          _directVectorField( voxel, 0 ) = directFlowFieldAlongX( voxel, 0 );
          _directVectorField( voxel, 1 ) = directFlowFieldAlongY( voxel, 0 );
          _directVectorField( voxel, 2 ) = directFlowFieldAlongZ( voxel, 0 );

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // updating and notifying upon request
    ////////////////////////////////////////////////////////////////////////////

    if ( update )
    {

      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::"
             "setDirectVectorField( "
             "const gkg::Volume< T >& directFlowFieldAlongX, "
             "const gkg::Volume< T >& directFlowFieldAlongY, "
             "const gkg::Volume< T >& directFlowFieldAlongZ, "
             "bool update, "
             "bool notify ))" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::setInverseVectorField(
                                 const gkg::Volume< T >& inverseFlowFieldAlongX,
                                 const gkg::Volume< T >& inverseFlowFieldAlongY,
                                 const gkg::Volume< T >& inverseFlowFieldAlongZ,
                                 bool update,
                                 bool notify )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting inverse flow field size and resolution along X
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > inverseFlowFieldAlongXSize;
    int32_t inverseFlowFieldAlongXSizeT = 0;

    inverseFlowFieldAlongX.getSize( inverseFlowFieldAlongXSize,
                                    inverseFlowFieldAlongXSizeT );
    inverseFlowFieldAlongX.getResolution( _inverseResolution );

    if ( inverseFlowFieldAlongXSizeT != 1 )
    {

      throw std::runtime_error(
                           "inverse flow field along X must contain 1 volume" );

    }

    // sanity check for inverse flow field along Y
    gkg::Vector3d< int32_t > inverseFlowFieldAlongYSize;
    int32_t inverseFlowFieldAlongYSizeT = 0;

    gkg::Vector3d< double > inverseResolution;
    inverseFlowFieldAlongY.getSize( inverseFlowFieldAlongYSize,
                                    inverseFlowFieldAlongYSizeT );
    inverseFlowFieldAlongY.getResolution( inverseResolution );


    if ( ( inverseFlowFieldAlongYSize != inverseFlowFieldAlongXSize ) ||
         ( inverseFlowFieldAlongYSizeT != inverseFlowFieldAlongXSizeT ) ||
         ( inverseResolution != _inverseResolution ) )
    {

      throw std::runtime_error( 
                "inverse X and Y flow fields do not share similar size or"
                " resolution" );

    }

    // sanity check for direct flow field along Z
    gkg::Vector3d< int32_t > inverseFlowFieldAlongZSize;
    int32_t inverseFlowFieldAlongZSizeT = 0;

    inverseFlowFieldAlongZ.getSize( inverseFlowFieldAlongZSize,
                                    inverseFlowFieldAlongZSizeT );
    inverseFlowFieldAlongZ.getResolution( inverseResolution );


    if ( ( inverseFlowFieldAlongZSize != inverseFlowFieldAlongXSize ) ||
         ( inverseFlowFieldAlongZSizeT != inverseFlowFieldAlongXSizeT ) ||
         ( inverseResolution != _inverseResolution ) )
    {

      throw std::runtime_error( 
                "inverse X and Z flow fields do not share similar size or"
                " resolution" );

    }


    // reallocating the direct and inverse flow field volume
    _inverseVectorField.reallocate( inverseFlowFieldAlongXSize, 3 );
    _inverseVectorField.setResolution( _inverseResolution );


    // copying input data to the direct and inverse flow field volume
    gkg::Vector3d< int32_t > voxel;
    for ( voxel.z = 0; voxel.z < inverseFlowFieldAlongXSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < inverseFlowFieldAlongXSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inverseFlowFieldAlongXSize.x; voxel.x++ )
        {

          _inverseVectorField( voxel, 0 ) = inverseFlowFieldAlongX( voxel, 0 );
          _inverseVectorField( voxel, 1 ) = inverseFlowFieldAlongY( voxel, 0 );
          _inverseVectorField( voxel, 2 ) = inverseFlowFieldAlongZ( voxel, 0 );

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // updating and notifying upon request
    ////////////////////////////////////////////////////////////////////////////


    if ( update )
    {

      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::"
             "setInverseVectorField( "
             "const gkg::Volume< T >& inverseFlowFieldAlongX, "
             "const gkg::Volume< T >& inverseFlowFieldAlongY, "
             "const gkg::Volume< T >& inverseFlowFieldAlongZ, "
             "bool update, "
             "bool notify ))" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::getDirectAndInverseVectorFields( 
                                    gkg::Volume< T >& directVectorField,
                                    gkg::Volume< T >& inverseVectorField ) const
{

  try
  {

    directVectorField = _directVectorField;
    inverseVectorField = _inverseVectorField;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::"
             "getDirectAndInverseVectorFields( "
             "gkg::Volume< T >& directVectorField, "
             "gkg::Volume< T >& inverseVectorField ) const" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::getDirectVectorField( 
                                     gkg::Volume< T >& directVectorField ) const
{

  try
  {

    directVectorField = _directVectorField;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::"
             "getDirectVectorField( "
             "gkg::Volume< T >& directVectorField ) const" );

}


template < class T >
void gkg::VectorFieldTransform3d< T >::getInverseVectorField( 
                                    gkg::Volume< T >& inverseVectorField ) const
{

  try
  {

    inverseVectorField = _inverseVectorField;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VectorFieldTransform3d< T >::"
             "getInverseVectorField( "
             "gkg::Volume< T >& inverseVectorField ) const" );

}


template class gkg::VectorFieldTransform3d< float >;
template class gkg::VectorFieldTransform3d< double >;
