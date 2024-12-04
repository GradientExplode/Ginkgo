#ifndef _gkg_processing_topology_HomotopicHoleFiller_i_h_
#define _gkg_processing_topology_HomotopicHoleFiller_i_h_

#include <gkg-processing-topology/HomotopicHoleFiller.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Front_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-topology/TopologyClassifier_i.h>
#include <gkg-core-exception/Exception.h>


template < class IN >
inline
gkg::HomotopicHoleFiller< IN >::HomotopicHoleFiller( const IN& label )
                               : _label( label )
{
}


template < class IN >
inline
gkg::HomotopicHoleFiller< IN >::~HomotopicHoleFiller()
{
}


template < class IN >
inline
void 
gkg::HomotopicHoleFiller< IN >::fillHoles( gkg::Volume< IN >& inputVolume )
{

  try
  {

    // collecting sizes of the volume
    int32_t sizeX = inputVolume.getSizeX();
    int32_t sizeY = inputVolume.getSizeY();
    int32_t sizeZ = inputVolume.getSizeZ();

    // computing initial bounding box
    gkg::Vector3d< int32_t > site;
    gkg::BoundingBox< int32_t > boundingBox( sizeX - 1, 0,
                                             sizeY - 1, 0,
                                             sizeZ - 1, 0 );
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( inputVolume( site ) == _label )
          {

            if ( site.x < boundingBox.getLowerX() )
            {

              boundingBox.setLowerX( site.x );

            }
            else if ( site.x > boundingBox.getUpperX() )
            {

              boundingBox.setUpperX( site.x );

            }

            if ( site.y < boundingBox.getLowerY() )
            {

              boundingBox.setLowerY( site.y );

            }
            else if ( site.y > boundingBox.getUpperY() )
            {

              boundingBox.setUpperY( site.y );

            }

            if ( site.z < boundingBox.getLowerZ() )
            {

              boundingBox.setLowerZ( site.z );

            }
            else if ( site.z > boundingBox.getUpperZ() )
            {

              boundingBox.setUpperZ( site.z );

            }

          }

        }

      }

    }

    // allocating and filling initial bounding box volume
    gkg::RCPointer< gkg::Volume< int8_t > >
      initialBoundingBoxVolume( new gkg::Volume< int8_t >( sizeX, sizeY, sizeZ ) );

    initialBoundingBoxVolume->fill( 0 );

    for ( site.z = boundingBox.getLowerZ();
          site.z <= boundingBox.getUpperZ(); site.z++ )
    {

      for ( site.y = boundingBox.getLowerY();
            site.y <= boundingBox.getUpperY(); site.y++ )
      {

        for ( site.x = boundingBox.getLowerX();
              site.x <= boundingBox.getUpperX(); site.x++ )
        {

          ( *initialBoundingBoxVolume )( site ) = 1;

        }

      }

    }

    // creating initial front
    gkg::Front< int8_t > front( initialBoundingBoxVolume, 1, 1 );
    gkg::EqualToFunction< int8_t > equalToFunction( 1 );

    // performing homotopic erosion
    bool isASimplePoint = false;
    int32_t transitionCount = 0;
    do
    {

      transitionCount = 0;

      std::set< gkg::Vector3d< int32_t >, gkg::Vector3dCompare< int32_t > >
        frontSites = front.getUniqueSites();

      std::set< gkg::Vector3d< int32_t >, 
                gkg::Vector3dCompare< int32_t > >::const_iterator
        s = frontSites.begin(),
        se = frontSites.end();
      while ( s != se )
      {

        isASimplePoint = gkg::TopologyClassifier::getInstance().isSimplePoint(
                                                     *initialBoundingBoxVolume,
                                                     equalToFunction,
                                                     *s );
        if ( isASimplePoint && ( inputVolume( *s ) != _label ) )
        {

          front.removeSite( *s );
          ( *initialBoundingBoxVolume )( *s ) = 0;
          ++ transitionCount;

        }
        ++s;

      }

    }
    while ( transitionCount );

    // copying solution to output
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( ( *initialBoundingBoxVolume )( site ) )
          {

            inputVolume( site ) = _label;

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class IN >"
             "inline"
             "void gkg::HomotopicHoleFiller< IN >::fillHoles( "
             "gkg::Volume< IN >& inputVolume )" );

}

#endif
