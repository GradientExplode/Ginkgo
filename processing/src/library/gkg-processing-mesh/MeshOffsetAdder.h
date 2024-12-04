#ifndef _gkg_processing_mesh_MeshOffsetAdder_h_
#define _gkg_processing_mesh_MeshOffsetAdder_h_


#include <functional>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare > class MeshMap;

template < class R, class S, uint32_t D, class Compare = std::less< R > >
class MeshOffsetAdder
{

  public:

    void addOffset( const MeshMap< R, S, D, Compare >& in,
                    const R& rank,
                    uint32_t offset,
                    MeshMap< R, S, D, Compare >& out ) const;

};


}


#endif
