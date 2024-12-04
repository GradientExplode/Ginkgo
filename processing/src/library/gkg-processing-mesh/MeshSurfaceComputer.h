#ifndef _gkg_processing_mesh_MeshSurfaceComputer_h_
#define _gkg_processing_mesh_MeshSurfaceComputer_h_


#include <functional>
#include <map>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare > class MeshMap;
template < class S > class Transform3d;


template < class R, class S, uint32_t D, class Compare = std::less< R > >
class MeshSurfaceComputer
{

  public:

    void compute( const MeshMap< R, S, D, Compare >& in,
                  std::map< R, float >& out ) const;
                    

};


}


#endif
