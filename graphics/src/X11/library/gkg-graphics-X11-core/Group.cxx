#include <gkg-graphics-X11-core/Group.h>
#include <gkg-graphics-X11-core/Aggregate.h>
#include <gkg-graphics-X11-core/Align.h>


gkg::Group::Group( gkg::Aggregate* aggregate,
                   gkg::DimensionName dimensionName )
           : gkg::Glyph(),
             _aggregate( aggregate ),
             _dimensionName( dimensionName ),
             _layout( new gkg::Align( dimensionName ) ),
             _map( new gkg::Glyph::Index[ 100 ] ),
             _count( 0 )
{

  if ( _aggregate )
  {

    _aggregate->addReference();

  }

}


gkg::Group::~Group()
{

  if ( _aggregate )
  {

    _aggregate->removeReference();

  }
  delete _layout;
  delete [] _map;

}


void gkg::Group::map( gkg::Glyph::Index index )
{

  _map[ _count ] = index;
  ++ _count;

}


void gkg::Group::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( _aggregate )
  {

    gkg::GeometryRequisition*
      geometryRequisitions = new gkg::GeometryRequisition[ _count ];
    for ( int32_t i = 0; i < _count; i++ )
    {

      gkg::Glyph* glyph = _aggregate->getComponent( _map[ i ] );
      if ( glyph )
      {

        glyph->request( geometryRequisitions[ i ] );

      }

    }
    _layout->request( _count, geometryRequisitions, geometryRequisition );
    delete [] geometryRequisitions;

  }

}


void gkg::Group::allocate( gkg::Canvas*,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& )
{

  if ( _aggregate )
  {

    gkg::GeometryRequisition*
      geometryRequisitions = new gkg::GeometryRequisition[ _count ];
    int32_t i;
    for ( i = 0; i < _count; i++ )
    {

      gkg::Glyph* glyph = _aggregate->getComponent( _map[ i ] );
      if ( glyph )
      {

        glyph->request( geometryRequisitions[ i ] );

      }

    }
    gkg::GeometryAllocation* 
      geometryAllocations = new gkg::GeometryAllocation[ _count ];
    _layout->allocate( geometryAllocation, _count, geometryRequisitions,
                       geometryAllocations );
    for ( i = 0; i < _count; i++ )
    {

      _aggregate->setAllotment( _map[ i ],
                                _dimensionName,
                                geometryAllocations[ i ].getAllotment(
                                                             _dimensionName ) );

    }
    delete [] geometryAllocations;
    delete [] geometryRequisitions;

  }

}
