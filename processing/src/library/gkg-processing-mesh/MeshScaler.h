#ifndef _gkg_processing_mesh_MeshScaler_h_
#define _gkg_processing_mesh_MeshScaler_h_


#include <vector>
#include <list>
#include <functional>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare > class MeshMap;
template < class S > class TextureMap;

template < class R, class S, uint32_t D, class Compare = std::less< R > >
class MeshScaler
{

  public:

    void scale( const MeshMap< R, S, D, Compare >& in,
                const R& rank,
                const std::vector< S >& scaling,
                MeshMap< R, S, D, Compare >& out ) const;
    void scale( const MeshMap< R, S, D, Compare >& in,
                const R& rank,
                const std::list< S >& scaling,
                MeshMap< R, S, D, Compare >& out ) const;
    void scale( const MeshMap< R, S, D, Compare >& in,
                const R& rank,
                const TextureMap< S >& scaling,
                MeshMap< R, S, D, Compare >& out ) const;
                    

};


}


#endif
