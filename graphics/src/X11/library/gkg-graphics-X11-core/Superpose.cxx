#include <gkg-graphics-X11-core/Superpose.h>


gkg::Superpose::Superpose( gkg::Layout* layout0,
                           gkg::Layout* layout1,
                           gkg::Layout* layout2,
                           gkg::Layout* layout3,
                           gkg::Layout* layout4 )
               : gkg::Layout()
{

  gkg::Layout* arg[ 6 ];
  arg[ 0 ] = layout0;
  arg[ 1 ] = layout1;
  arg[ 2 ] = layout2;
  arg[ 3 ] = layout3;
  arg[ 4 ] = layout4;
  arg[ 5 ] = 0;
  for ( _count = 0; arg[ _count ]; _count++ );
  _layouts = new gkg::Layout*[ _count ];
  for ( int32_t i = 0; i < _count; i++ )
  {

    _layouts[ i ] = arg[ i ];

  }

}


gkg::Superpose::~Superpose()
{

  for ( int32_t i = 0; i < _count; i++ )
  {

    delete _layouts[ i ];

  }
  delete [] _layouts;

}


void 
gkg::Superpose::request( gkg::Glyph::Index count,
                         const gkg::GeometryRequisition* geometryRequisition,
                         gkg::GeometryRequisition& result )
{

  for ( int32_t i = 0; i < _count; i++ )
  {

    _layouts[ i ]->request( count, geometryRequisition, result );

  }

}


void gkg::Superpose::allocate(
                            const gkg::GeometryAllocation& given,
                            gkg::Glyph::Index count,
                            const gkg::GeometryRequisition* geometryRequisition,
	                    gkg::GeometryAllocation* result )
{

  for ( int32_t i = 0; i < _count; i++ )
  {

    _layouts[ i ]->allocate( given, count, geometryRequisition, result );

  }

}
