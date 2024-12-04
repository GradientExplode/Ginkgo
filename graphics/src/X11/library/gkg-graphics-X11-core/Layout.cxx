#include <gkg-graphics-X11-core/Layout.h>


gkg::Layout::Layout()
{
}


gkg::Layout::~Layout()
{
}


void gkg::Layout::request( gkg::Glyph::Index,
                           const gkg::GeometryRequisition*,
                           gkg::GeometryRequisition& )
{
}


void gkg::Layout::allocate( const gkg::GeometryAllocation&,
                            gkg::Glyph::Index,
                            const gkg::GeometryRequisition*,
                            gkg::GeometryAllocation* )
{
}
