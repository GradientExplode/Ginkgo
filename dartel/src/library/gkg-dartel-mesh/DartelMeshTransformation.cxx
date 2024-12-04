#include <gkg-dartel-mesh/DartelMeshTransformation.h>
#include <gkg-dartel-core/DartelExponentiation.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>


template < class L, class S >
gkg::DartelMeshTransformation< L, S >::DartelMeshTransformation( int32_t k )
                                     : gkg::DartelTransformation< S, float >(
                                                                             k )
{
}


template < class L, class S >
gkg::DartelMeshTransformation< L, S >::~DartelMeshTransformation()
{
}


template < class L, class S >
void gkg::DartelMeshTransformation< L, S >::wrap(
                                  const gkg::MeshMap< L, float, 3U >& inMeshMap,
                                  const gkg::Volume< S >& inVolumeProxy,
                                  const gkg::Volume< float >& flowField, 
                                  gkg::MeshMap< L, float, 3U >& outMeshMap )
{

  try
  {

    gkg::Volume< float > expU;
    gkg::Volume< S > templateProxy;
    int32_t m0 = flowField.getSizeX();
    int32_t m1 = flowField.getSizeY();
    int32_t m2 = flowField.getSizeZ();
    int32_t sizeXYZ = flowField.getSizeXYZ();
    gkg::Vector3d< float > p;
    gkg::Vector3d< double > resolution;
    gkg::DartelExponentiation< float > dartel( this->_k );

    // copying input to ouput before transformation
    if ( &outMeshMap != &inMeshMap )
    {

      outMeshMap = inMeshMap;

    }

    flowField.getResolution( resolution );
    dartel.getDirectVectorField( flowField, expU );

    this->affineTransformation( inVolumeProxy, flowField, 
                                templateProxy, false );

    float* expUx = &expU( 0 );
    float* expUy = expUx + sizeXYZ;
    float* expUz = expUy + sizeXYZ;
    typename gkg::SiteMap< L, float >::iterator
      r = outMeshMap.vertices.begin(),
      re = outMeshMap.vertices.end();

    // loop over ranks
    while ( r != re )
    {

      // loop over vertice(s)
      std::list< gkg::Vector3d< float > >::iterator
        v = r->second.begin(),
        ve = r->second.end();

      while ( v != ve )
      {

        this->_m.getDirect( *v, p );

        int32_t x = p.x / resolution.x;
        int32_t y = p.y / resolution.y;
        int32_t z = p.z / resolution.z;

        v->x = this->sample1( expUx, m0, m1, m2, x, y, z ) * resolution.x;
        v->y = this->sample1( expUy, m0, m1, m2, x, y, z ) * resolution.y;
        v->z = this->sample1( expUz, m0, m1, m2, x, y, z ) * resolution.z;
        ++v;

      }

      ++r;

    }

  }
  GKG_CATCH( "template < class L, class S > "
             "void gkg::DartelMeshTransformation< L, S >::wrap( "
             "const gkg::MeshMap< L, float, 3U >& inMeshMap, "
             "const gkg::Volume< S >& inVolumeProxy, "
             "const gkg::Volume< float >& flowField, "
             "gkg::MeshMap< L, float, 3U >& outMeshMap )" );

}


template < class L, class S >
void gkg::DartelMeshTransformation< L, S >::unwrap(
                                  const gkg::MeshMap< L, float, 3U >& inMeshMap,
                                  const gkg::Volume< S >& inVolumeProxy,
                                  const gkg::Volume< float >& flowField, 
                                  const gkg::Volume< S >& /* templateProxy */,
                                  gkg::MeshMap< L, float, 3U >& outMeshMap )
{

  try
  {

    gkg::Volume< float > expU;
    gkg::Volume< S > templateProxy;
    int32_t m0 = flowField.getSizeX();
    int32_t m1 = flowField.getSizeY();
    int32_t m2 = flowField.getSizeZ();
    int32_t sizeXYZ = flowField.getSizeXYZ();
    gkg::Vector3d< float > p;
    gkg::Vector3d< double > resolution;
    gkg::DartelExponentiation< float > dartel( this->_k );

    // copying input to ouput before transformation
    if ( &outMeshMap != &inMeshMap )
    {

      outMeshMap = inMeshMap;

    }

    flowField.getResolution( resolution );
    dartel.getInverseVectorField( flowField, expU );

    this->affineTransformation( inVolumeProxy, flowField, 
                                templateProxy, false );

    float* expUx = &expU( 0 );
    float* expUy = expUx + sizeXYZ;
    float* expUz = expUy + sizeXYZ;
    typename gkg::SiteMap< L, float >::iterator
      r = outMeshMap.vertices.begin(),
      re = outMeshMap.vertices.end();

    // loop over ranks
    while ( r != re )
    {

      // loop over vertice(s)
      std::list< gkg::Vector3d< float > >::iterator
        v = r->second.begin(),
        ve = r->second.end();

      while ( v != ve )
      {

        gkg::Vector3d< float > q( v->x / resolution.x,
        			  v->y / resolution.y,
        			  v->z / resolution.z );

        p.x = this->sample1( expUx, m0, m1, m2, q.x, q.y, q.z ) * 
              resolution.x;
        p.y = this->sample1( expUy, m0, m1, m2, q.x, q.y, q.z ) * 
              resolution.y;
        p.z = this->sample1( expUz, m0, m1, m2, q.x, q.y, q.z ) * 
              resolution.z;
        this->_m.getInverse( p, *v );
        ++ v;

      }

      ++r;

    }

  }
  GKG_CATCH( "template < class L, class S > "
             "void gkg::DartelMeshTransformation< L, S >::unwrap( "
             "const gkg::MeshMap< L, float, 3U >& inMeshMap, "
             "const gkg::Volume< S >& inVolumeProxy, "
             "const gkg::Volume< float >& flowField, "
             "const gkg::Volume< S >& templateProxy, "
             "gkg::MeshMap< L, float, 3U >& outMeshMap )" );

}


template class gkg::DartelMeshTransformation< uint8_t, uint8_t >;
template class gkg::DartelMeshTransformation< uint8_t, int8_t >;
template class gkg::DartelMeshTransformation< uint8_t, uint16_t >;
template class gkg::DartelMeshTransformation< uint8_t, int16_t >;
template class gkg::DartelMeshTransformation< uint8_t, uint32_t >;
template class gkg::DartelMeshTransformation< uint8_t, int32_t >;
template class gkg::DartelMeshTransformation< uint8_t, uint64_t >;
template class gkg::DartelMeshTransformation< uint8_t, int64_t >;
template class gkg::DartelMeshTransformation< uint8_t, float >;
template class gkg::DartelMeshTransformation< uint8_t, double >;

template class gkg::DartelMeshTransformation< int8_t, uint8_t >;
template class gkg::DartelMeshTransformation< int8_t, int8_t >;
template class gkg::DartelMeshTransformation< int8_t, uint16_t >;
template class gkg::DartelMeshTransformation< int8_t, int16_t >;
template class gkg::DartelMeshTransformation< int8_t, uint32_t >;
template class gkg::DartelMeshTransformation< int8_t, int32_t >;
template class gkg::DartelMeshTransformation< int8_t, uint64_t >;
template class gkg::DartelMeshTransformation< int8_t, int64_t >;
template class gkg::DartelMeshTransformation< int8_t, float >;
template class gkg::DartelMeshTransformation< int8_t, double >;

template class gkg::DartelMeshTransformation< uint16_t, uint8_t >;
template class gkg::DartelMeshTransformation< uint16_t, int8_t >;
template class gkg::DartelMeshTransformation< uint16_t, uint16_t >;
template class gkg::DartelMeshTransformation< uint16_t, int16_t >;
template class gkg::DartelMeshTransformation< uint16_t, uint32_t >;
template class gkg::DartelMeshTransformation< uint16_t, int32_t >;
template class gkg::DartelMeshTransformation< uint16_t, uint64_t >;
template class gkg::DartelMeshTransformation< uint16_t, int64_t >;
template class gkg::DartelMeshTransformation< uint16_t, float >;
template class gkg::DartelMeshTransformation< uint16_t, double >;

template class gkg::DartelMeshTransformation< int16_t, uint8_t >;
template class gkg::DartelMeshTransformation< int16_t, int8_t >;
template class gkg::DartelMeshTransformation< int16_t, uint16_t >;
template class gkg::DartelMeshTransformation< int16_t, int16_t >;
template class gkg::DartelMeshTransformation< int16_t, uint32_t >;
template class gkg::DartelMeshTransformation< int16_t, int32_t >;
template class gkg::DartelMeshTransformation< int16_t, uint64_t >;
template class gkg::DartelMeshTransformation< int16_t, int64_t >;
template class gkg::DartelMeshTransformation< int16_t, float >;
template class gkg::DartelMeshTransformation< int16_t, double >;

template class gkg::DartelMeshTransformation< uint32_t, uint8_t >;
template class gkg::DartelMeshTransformation< uint32_t, int8_t >;
template class gkg::DartelMeshTransformation< uint32_t, uint16_t >;
template class gkg::DartelMeshTransformation< uint32_t, int16_t >;
template class gkg::DartelMeshTransformation< uint32_t, uint32_t >;
template class gkg::DartelMeshTransformation< uint32_t, int32_t >;
template class gkg::DartelMeshTransformation< uint32_t, uint64_t >;
template class gkg::DartelMeshTransformation< uint32_t, int64_t >;
template class gkg::DartelMeshTransformation< uint32_t, float >;
template class gkg::DartelMeshTransformation< uint32_t, double >;

template class gkg::DartelMeshTransformation< int32_t, uint8_t >;
template class gkg::DartelMeshTransformation< int32_t, int8_t >;
template class gkg::DartelMeshTransformation< int32_t, uint16_t >;
template class gkg::DartelMeshTransformation< int32_t, int16_t >;
template class gkg::DartelMeshTransformation< int32_t, uint32_t >;
template class gkg::DartelMeshTransformation< int32_t, int32_t >;
template class gkg::DartelMeshTransformation< int32_t, uint64_t >;
template class gkg::DartelMeshTransformation< int32_t, int64_t >;
template class gkg::DartelMeshTransformation< int32_t, float >;
template class gkg::DartelMeshTransformation< int32_t, double >;

template class gkg::DartelMeshTransformation< uint64_t, uint8_t >;
template class gkg::DartelMeshTransformation< uint64_t, int8_t >;
template class gkg::DartelMeshTransformation< uint64_t, uint16_t >;
template class gkg::DartelMeshTransformation< uint64_t, int16_t >;
template class gkg::DartelMeshTransformation< uint64_t, uint32_t >;
template class gkg::DartelMeshTransformation< uint64_t, int32_t >;
template class gkg::DartelMeshTransformation< uint64_t, uint64_t >;
template class gkg::DartelMeshTransformation< uint64_t, int64_t >;
template class gkg::DartelMeshTransformation< uint64_t, float >;
template class gkg::DartelMeshTransformation< uint64_t, double >;

template class gkg::DartelMeshTransformation< int64_t, uint8_t >;
template class gkg::DartelMeshTransformation< int64_t, int8_t >;
template class gkg::DartelMeshTransformation< int64_t, uint16_t >;
template class gkg::DartelMeshTransformation< int64_t, int16_t >;
template class gkg::DartelMeshTransformation< int64_t, uint32_t >;
template class gkg::DartelMeshTransformation< int64_t, int32_t >;
template class gkg::DartelMeshTransformation< int64_t, uint64_t >;
template class gkg::DartelMeshTransformation< int64_t, int64_t >;
template class gkg::DartelMeshTransformation< int64_t, float >;
template class gkg::DartelMeshTransformation< int64_t, double >;
