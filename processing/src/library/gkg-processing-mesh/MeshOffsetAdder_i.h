#ifndef _gkg_processing_mesh_MeshOffsetAdder_i_h_
#define _gkg_processing_mesh_MeshOffsetAdder_i_h_


#include <gkg-processing-mesh/MeshOffsetAdder.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-exception/Exception.h>


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshOffsetAdder< R, S, D, Compare >::addOffset(
                                   const gkg::MeshMap< R, S, D, Compare >& in,
                                   const R& rank,
                                   uint32_t offset,
                                   gkg::MeshMap< R, S, D, Compare >& out ) const
{

  try
  {

    // copying input to ouput before transformation
    if ( &out != &in )
    {

      out = in;

    }

    // looping over rank(s)
    typename gkg::PolygonMap< D >::iterator
      r = out.polygons.find( out.vertices.getIndex( rank ) );

    // sanity check(s)
    if ( r == out.polygons.end() )
    {

      throw std::runtime_error( "unable to find rank" );

    }
 
    // looping over polygon(s)
    typename std::list< gkg::Polygon< D > >::iterator
      p = r->second.begin(),
      pe = r->second.end();
    uint32_t d = 0;
    while ( p != pe )
    {

      for ( d = 0; d < D; d++ )
      {

        p->indices[ d ] += offset;

      }
      ++ p;

    }

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "void gkg::MeshOffsetAdder< R, S, D, Compare >::addOffset( "
             "const gkg::MeshMap< R, S, D, Compare >& in, "
             "const R& rank, "
             "uint32_t offset, "
             "gkg::MeshMap< R, S, D, Compare >& out ) const" );

}


#endif
