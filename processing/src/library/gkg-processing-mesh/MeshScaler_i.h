#ifndef _gkg_processing_mesh_MeshScaler_i_h_
#define _gkg_processing_mesh_MeshScaler_i_h_


#include <gkg-processing-mesh/MeshScaler.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-core-exception/Exception.h>


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshScaler< R, S, D, Compare >::scale(
                                   const gkg::MeshMap< R, S, D, Compare >& in,
                                   const R& rank,
                                   const std::vector< S >& scaling,
                                   gkg::MeshMap< R, S, D, Compare >& out ) const
{

  try
  {

    // copying input to ouput before transformation
    if ( &out != &in )
    {

      out = in;

    }

    // finding appropriate rank
    typename gkg::SiteMap< R, S, Compare >::iterator
      r = out.vertices.find( rank );

    // sanity check(s)
    if ( r == out.vertices.end() )
    {

      throw std::runtime_error( "unable to find rank" );

    }
    if ( r->second.size() != scaling.size() )
    {

      throw std::runtime_error(
                               "incompatible scaling and vertex list size(s)" );

    }

    // looping over vertice(s)
    typename std::list< gkg::Vector3d< S > >::iterator
      v = r->second.begin(),
      ve = r->second.end();
    typename std::vector< S >::const_iterator
      s = scaling.begin();
    while ( v != ve )
    {

      *v *= *s;
      ++ v;
      ++ s;

    }

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshScaler::scale( "
             "const gkg::MeshMap< R, S, D, Compare >& in, "
             "const R& rank, "
             "const std::vector< S >& scaling, "
             "gkg::MeshMap< R, S, D, Compare >& out ) const" );

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshScaler< R, S, D, Compare >::scale(
                                   const gkg::MeshMap< R, S, D, Compare >& in,
                                   const R& rank,
                                   const std::list< S >& scaling,
                                   gkg::MeshMap< R, S, D, Compare >& out ) const
{

  try
  {

    // copying input to ouput before transformation
    if ( &out != &in )
    {

      out = in;

    }

    // finding appropriate rank
    typename gkg::SiteMap< R, S, Compare >::iterator
      r = out.vertices.find( rank );

    // sanity check(s)
    if ( r == out.vertices.end() )
    {

      throw std::runtime_error( "unable to find rank" );

    }
    if ( r->second.size() != scaling.size() )
    {

      throw std::runtime_error(
                               "incompatible scaling and vertex list size(s)" );

    }

    // looping over vertice(s)
    typename std::list< gkg::Vector3d< S > >::iterator
      v = r->second.begin(),
      ve = r->second.end();
    typename std::list< S >::const_iterator
      s = scaling.begin();
    while ( v != ve )
    {

      *v *= *s;
      ++ v;
      ++ s;

    }

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshScaler::scale( "
             "const gkg::MeshMap< R, S, D, Compare >& in, "
             "const R& rank, "
             "const std::list< S >& scaling, "
             "gkg::MeshMap< R, S, D, Compare >& out ) const" );

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshScaler< R, S, D, Compare >::scale(
                                   const gkg::MeshMap< R, S, D, Compare >& in,
                                   const R& rank,
                                   const gkg::TextureMap< S >& scaling,
                                   gkg::MeshMap< R, S, D, Compare >& out ) const
{

  try
  {

    // copying input to ouput before transformation
    if ( &out != &in )
    {

      out = in;

    }

    // finding appropriate rank
    typename gkg::SiteMap< R, S, Compare >::iterator
      r = out.vertices.find( rank );

    // sanity check(s)
    if ( r == out.vertices.end() )
    {

      throw std::runtime_error( "unable to find rank" );

    }
    if ( ( int32_t )r->second.size() != scaling.getCount() )
    {

      throw std::runtime_error(
                               "incompatible scaling and vertex list size(s)" );

    }

    // looping over vertice(s)
    typename std::list< gkg::Vector3d< S > >::iterator
      v = r->second.begin(),
      ve = r->second.end();
    typename gkg::TextureMap< S >::const_iterator
      s = scaling.begin();
    while ( v != ve )
    {

      *v *= s->second;
      ++ v;
      ++ s;

    }

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshScaler::scale( "
             "const gkg::MeshMap< R, S, D, Compare >& in, "
             "const R& rank, "
             "const gkg::TextureMap< S >& scaling, "
             "gkg::MeshMap< R, S, D, Compare >& out ) const" );

}


#endif
