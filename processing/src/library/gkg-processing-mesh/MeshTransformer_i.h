#ifndef _gkg_processing_mesh_MeshTransformer_i_h_
#define _gkg_processing_mesh_MeshTransformer_i_h_


#include <gkg-processing-mesh/MeshTransformer.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-core-exception/Exception.h>


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshTransformer< R, S, D, Compare >::transform(
                                   const gkg::MeshMap< R, S, D, Compare >& in,
                                   const gkg::Transform3d< S >& transform3d,
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
    typename gkg::SiteMap< R, S, Compare >::iterator
      r = out.vertices.begin(),
      re = out.vertices.end();

    uint32_t index = 0U;

    while ( r != re )
    {

      // looping over vertice(s)
      typename std::list< gkg::Vector3d< S > >::iterator
        v = r->second.begin(),
        ve = r->second.end();
      while ( v != ve )
      {

        transform3d.getDirect( *v, *v );
        ++ v;

      }


      // inverting polygons if transformation is indirect
      if ( transform3d.isIndirect() )
      {

        typename std::list< gkg::Polygon< D > >::const_iterator
          pi = in.polygons.getPolygons( 
                                     in.vertices.getIndex( r->first ) ).begin(),
          pie = in.polygons.getPolygons(
                                     in.vertices.getIndex( r->first ) ).end();

        typename std::list< gkg::Polygon< D > >::iterator
          po = out.polygons.lower_bound( 
                            out.vertices.getIndex( r->first ) )->second.begin();

        while ( pi != pie )
        {

          for ( index = 0U; index < D; index ++ )
          {

            po->indices[ index ] = pi->indices[ D - 1 - index ];

          }

          ++ pi;
          ++ po;

        }

      }

      ++ r;

    }      


  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshTransformer::transform( "
             "const gkg::MeshMap< R, S, D, Compare >& in, "
             "const gkg::Transform3d< S >& transform3d, "
             "gkg::MeshMap< R, S, D, Compare >& out ) const" );

}


#endif
