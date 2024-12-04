#ifndef _gkg_processing_mesh_MeshSurfaceComputer_i_h_
#define _gkg_processing_mesh_MeshSurfaceComputer_i_h_


#include <gkg-processing-mesh/MeshSurfaceComputer.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-exception/Exception.h>


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshSurfaceComputer< R, S, D, Compare >::compute(
                                   const gkg::MeshMap< R, S, D, Compare >& in,
                                   std::map< R, float >& out ) const
{

  try
  {



    // looping over rank(s)
    typename gkg::SiteMap< R, S, Compare >::const_iterator
      r = in.vertices.begin(),
      re = in.vertices.end();
    while ( r != re )
    {

      // getting access to the vertices
      std::vector< gkg::Vector3d< S > > vertices( r->second.begin(),
                                                  r->second.end() );

      // then looping over the polygons
      float surfaceArea = 0.0f;
      typename std::list< gkg::Polygon< D > >::const_iterator
        p = in.polygons.getPolygons( in.vertices.getIndex( r->first ) ).begin(),
        pe = in.polygons.getPolygons( in.vertices.getIndex( r->first ) ).end();
      while ( p != pe )
      {

        surfaceArea += p->getSurfaceArea( vertices );
        ++ p;

      }

      out[ r->first ] = surfaceArea;

      ++ r;

    }      

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshSurfaceComputer::compute( "
             "const gkg::MeshMap< R, S, D, Compare >& in, "
             "std::map< R, float >& out ) const" );

}


#endif
