#include <gkg-anatomist-plugin-io/AFiberReader.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-dmri-container/BundleMap_i.h>

#include <iostream>
#include <sstream>
#include <limits>


gkg::AFiberReader::AFiberReader( const std::string& fileNameBundleMap )
                 : _fileNameBundleMap( fileNameBundleMap )
{

  _boundingBox = gkg::BoundingBox< float >( 
                                       std::numeric_limits< float >::max(),
                                       std::numeric_limits< float >::lowest(),
                                       std::numeric_limits< float >::max(),
                                       std::numeric_limits< float >::lowest(),
                                       std::numeric_limits< float >::max(),
                                       std::numeric_limits< float >::lowest() );

}


gkg::AFiberReader::~AFiberReader()
{
}


const gkg::BoundingBox< float >& gkg::AFiberReader::getBoundingBox() const
{

  try
  {

    return _boundingBox;

  }
  GKG_CATCH( "const gkg::BoundingBox< float >& "
             "gkg::AFiberReader::getBoundingBox() const" );

}


void gkg::AFiberReader::getFiberCoordinates(
                 int32_t samplingValue,
                 std::vector< QVector3D* >& coordinates,
                 std::vector< int32_t >& fiberCount,
                 std::vector< std::pair< std::string, int32_t > >& bundleNames )
{

  try
  {

    gkg::RCPointer< gkg::BundleMap< std::string > > bundleMapString;
    gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMapInt16;
    int32_t fiberIndex = 0;

    try
    {

      gkg::BundleMap< std::string >* 
         bundleMap = new gkg::BundleMap< std::string >;
      gkg::Reader::getInstance().read( _fileNameBundleMap, *bundleMap );
      bundleMapString.reset( bundleMap );

      gkg::BundleMap< std::string >::const_iterator
        b = bundleMapString->begin(),
        be = bundleMapString->end();

      while ( b != be )
      {

        const gkg::BundleMap< std::string >::Bundle& bundle = b->second;
        bundleNames.push_back( std::make_pair( b->first, fiberIndex ) );
        gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                                 fe = bundle.end();
        fiberCount.push_back( 0 );

        while ( f != fe )
        {

          const gkg::LightCurve3d< float >& fiber = f->getEquidistantCurve( 
                                                                samplingValue );

          gkg::LightCurve3d< float >::const_iterator
            s = fiber.begin(),
            se = fiber.end();
          coordinates.push_back( new QVector3D[ samplingValue ] );
          int32_t pointIndex = 0;

          while ( s != se )
          {

            coordinates[ fiberIndex ][ pointIndex ].setX( s->x );
            coordinates[ fiberIndex ][ pointIndex ].setY( s->y );
            coordinates[ fiberIndex ][ pointIndex ].setZ( s->z );

            if ( s->x < _boundingBox.getLowerX() )
            {

              _boundingBox.setLowerX( s->x );

            }
            if ( s->y < _boundingBox.getLowerY() )
            {

              _boundingBox.setLowerY( s->y );

            }
            if ( s->z < _boundingBox.getLowerZ() )
            {

              _boundingBox.setLowerZ( s->z );

            }
            if ( s->x > _boundingBox.getUpperX() )
            {

              _boundingBox.setUpperX( s->x );

            }
            if ( s->y > _boundingBox.getUpperY() )
            {

              _boundingBox.setUpperY( s->y );

            }
            if ( s->z > _boundingBox.getUpperZ() )
            {

              _boundingBox.setUpperZ( s->z );

            }
            ++ pointIndex;
            ++ s;

          }

          ++ fiberCount[ fiberCount.size() - 1 ];
          ++ fiberIndex;
          ++ f;

        }

        ++ b;

      }

    }
    catch ( std::exception& )
    {

      try
      {

        // second case where the file format on disk fit with a bundle map
        // labelled with int16_t
        gkg::BundleMap< int16_t >*
          bundleMap = new gkg::BundleMap< int16_t >;
        gkg::Reader::getInstance().read( _fileNameBundleMap, *bundleMap );
        bundleMapInt16.reset( bundleMap );

        gkg::BundleMap< int16_t >::const_iterator
          b = bundleMapInt16->begin(),
          be = bundleMapInt16->end();

        while ( b != be )
        {

          std::ostringstream oss;

          oss << b->first;

          const gkg::BundleMap< int16_t >::Bundle& bundle = b->second;
          bundleNames.push_back( std::make_pair( oss.str(), fiberIndex ) );
          gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                                   fe = bundle.end();
          fiberCount.push_back( 0 );

          while ( f != fe )
          {

            const gkg::LightCurve3d< float >& fiber = f->getEquidistantCurve( 
                                                                samplingValue );

            gkg::LightCurve3d< float >::const_iterator
              s = fiber.begin(),
              se = fiber.end();
            coordinates.push_back( new QVector3D[ samplingValue ] );
            int32_t pointIndex = 0;

            while ( s != se )
            {

              coordinates[ fiberIndex ][ pointIndex ].setX( s->x );
              coordinates[ fiberIndex ][ pointIndex ].setY( s->y );
              coordinates[ fiberIndex ][ pointIndex ].setZ( s->z );

              if ( s->x < _boundingBox.getLowerX() )
              {

                _boundingBox.setLowerX( s->x );

              }
              if ( s->y < _boundingBox.getLowerY() )
              {

                _boundingBox.setLowerY( s->y );

              }
              if ( s->z < _boundingBox.getLowerZ() )
              {

                _boundingBox.setLowerZ( s->z );

              }
              if ( s->x > _boundingBox.getUpperX() )
              {

                _boundingBox.setUpperX( s->x );

              }
              if ( s->y > _boundingBox.getUpperY() )
              {

                _boundingBox.setUpperY( s->y );

              }
              if ( s->z > _boundingBox.getUpperZ() )
              {

                _boundingBox.setUpperZ( s->z );

              }
              ++ pointIndex;
              ++ s;

            }

            ++ fiberCount[ fiberCount.size() - 1 ];
            ++ fiberIndex;
            ++ f;

          }

          ++ b;

        }

      }
      catch ( std::exception& )
      {

        throw std::runtime_error( "unable to read bundle map" );

      }

    }

  }
  GKG_CATCH( "void gkg::AFiberReader::getFiberCoordinates( "
             "int32_t samplingValue, "
             "std::vector< QVector3D* >& coordinates, "
             "std::vector< int32_t >& fiberCount, "
             "std::vector< std::pair< std::string, int32_t > >& "
             "bundleNames )" );

}

