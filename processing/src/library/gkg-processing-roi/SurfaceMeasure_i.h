#ifndef _gkg_processing_roi_SurfaceMeasure_i_h_
#define _gkg_processing_roi_SurfaceMeasure_i_h_


#include <gkg-processing-roi/SurfaceMeasure.h>
#include <gkg-processing-container/RoiMeasure_i.h>


template < class T >
inline
gkg::SurfaceMeasure< T >::SurfaceMeasure( gkg::RCPointer< gkg::Roi< T > > roi )
                         : gkg::RoiMeasure< T >( roi )
{

  try
  {

    initialize();

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "gkg::SurfaceMeasure< T >::SurfaceMeasure( " 
             "gkg::RCPointer< gkg::Roi< T > > roi )" );

}


template < class T >
inline
gkg::SurfaceMeasure< T >::~SurfaceMeasure()
{
}


template < class T >
inline
void gkg::SurfaceMeasure< T >::initialize()
{

  try
  {

    this->_value = 0.0;

    gkg::Vector3d< double > resolution;
    this->_roi->getLabelVolume()->getResolution( resolution );

    int32_t stride = this->_roi->getStride();
    _surfaceXY = resolution.x * resolution.y * stride * stride;
    _surfaceXZ = resolution.x * resolution.z * stride * stride;
    _surfaceYZ = resolution.y * resolution.z * stride * stride;

    int32_t sizeX = this->_roi->getLabelVolume()->getSizeX();
    int32_t sizeY = this->_roi->getLabelVolume()->getSizeY();
    int32_t sizeZ = this->_roi->getLabelVolume()->getSizeZ();

    T label = this->_roi->getLabel();
    const gkg::Volume< T >& labelVolume = *this->_roi->getLabelVolume();

    _boundingBox = gkg::BoundingBox< int32_t >( 0, sizeX - 1,
                                                0, sizeY - 1,
                                                0, sizeZ - 1 );

    _neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets( 
                                          gkg::Neighborhood3d::Neighborhood3d_6,
                                          stride );
    std::list< gkg::Vector3d< int32_t > >::const_iterator 
      n = _neighbors.begin(),
      ne = _neighbors.end();

    int32_t x, y, z;
    for( z = 0; z < sizeZ; z += stride ) 
    {

      for( y = 0; y < sizeY; y += stride ) 
      {

        for( x = 0; x < sizeX; x += stride ) 
        {

          if ( labelVolume( x, y, z ) == label )
          {

            n = _neighbors.begin();
            while ( n != ne )
            {

              gkg::Vector3d< int32_t > site( x + n->x,  y + n->y, z + n->z );

              if ( _boundingBox.contains( site ) )
              {

                if ( labelVolume( site ) != label )
                {

                  if ( n->x )
                  {

                    this->_value += _surfaceYZ;

                  }
                  else if ( n->y )
                  {

                    this->_value += _surfaceXZ;

                  }
                  else if ( n->z )
                  {

                    this->_value += _surfaceXY;

                  }

	        }

              }
              ++ n;

            }

	  }

        }

      }

    }

    this->_newValue = this->_value;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::SurfaceMeasure< T >::initialize()" );

}


template < class T >
inline
void gkg::SurfaceMeasure< T >::processNewValue( 
                                           const gkg::Vector3d< int32_t >& site,
                                           bool addOrRemove )
{

  try
  {

    T roiLabel = this->_roi->getLabel();
    const gkg::Volume< T >& labelVolume = *this->_roi->getLabelVolume();

    if ( addOrRemove )
    {

      // adding a point
      std::list< gkg::Vector3d< int32_t > >::const_iterator 
        n = _neighbors.begin(),
       ne = _neighbors.end();

      while ( n != ne )
      {

        gkg::Vector3d< int32_t > neighbor( site.x + n->x,
                                           site.y + n->y,
                                           site.z + n->z );

        if ( _boundingBox.contains( neighbor ) )
        {

          if ( labelVolume( neighbor ) == roiLabel )
          {

            if ( n->x )
            {

              this->_newValue = this->_value - _surfaceYZ;

            }
            else if ( n->y )
            {

              this->_newValue = this->_value - _surfaceXZ;

            }
            else if ( n->z )
            {

              this->_newValue = this->_value - _surfaceXY;

            }

          }
          else
          {

            if ( n->x )
            {

              this->_newValue = this->_value + _surfaceYZ;

            }
            else if ( n->y )
            {

              this->_newValue = this->_value + _surfaceXZ;

            }
            else if( n->z )
            {

              this->_newValue = this->_value + _surfaceXY;

            }

          }

        }

        ++ n;

      }

    }
    else
    {

      // removing a point

      std::list< Vector3d< int32_t > >::const_iterator 
        n = _neighbors.begin(),
        ne = _neighbors.end();

      while ( n != ne )
      {

        gkg::Vector3d< int32_t > neighbor( site.x + n->x,
                                           site.y + n->y,
                                           site.z + n->z );

        if ( _boundingBox.contains( neighbor ) )
        {

          if ( labelVolume( neighbor ) == roiLabel )
          {

            if ( n->x )
            {

              this->_newValue = this->_value + _surfaceYZ;

            }
            else if ( n->y )
            {

              this->_newValue = this->_value + _surfaceXZ;

            }
            else if ( n->z )
            {

              this->_newValue = this->_value + _surfaceXY;

            }

          }
          else
          {

            if ( n->x )
            {

              this->_newValue = this->_value - _surfaceYZ;

            }
            else if ( n->y )
            {

              this->_newValue = this->_value - _surfaceXZ;

            }
            else if( n->z )
            {

              this->_newValue = this->_value - _surfaceXY;

            }

          }

        }

        ++ n;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::SurfaceMeasure< T >::processNewValue( "
             "const gkg::Vector3d< int32_t >& site, "
             "bool addOrRemove )" );

}


#endif
