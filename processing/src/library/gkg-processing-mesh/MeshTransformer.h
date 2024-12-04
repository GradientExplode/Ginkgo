#ifndef _gkg_processing_mesh_MeshTransformer_h_
#define _gkg_processing_mesh_MeshTransformer_h_


#include <functional>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare > class MeshMap;
template < class S > class Transform3d;


template < class R, class S, uint32_t D, class Compare = std::less< R > >
class MeshTransformer
{

  public:

    void transform( const MeshMap< R, S, D, Compare >& in,
                    const Transform3d< S >& transform3d,
                    MeshMap< R, S, D, Compare >& out ) const;
                    

};


}


#endif
