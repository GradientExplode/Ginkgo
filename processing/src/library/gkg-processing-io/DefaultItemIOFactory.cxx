#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Polygon.h>
#include <gkg-processing-colorimetry/RGBComponent.h>


template class gkg::DefaultItemIOFactory< gkg::Vector2d< int16_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector2d< int32_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector2d< int64_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector2d< uint16_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector2d< uint32_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector2d< uint64_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector2d< float > >;
template class gkg::DefaultItemIOFactory< gkg::Vector2d< double > >;


template class gkg::DefaultItemIOFactory< gkg::Vector3d< int16_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector3d< int32_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector3d< int64_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector3d< uint16_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector3d< uint32_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector3d< uint64_t > >;
template class gkg::DefaultItemIOFactory< gkg::Vector3d< float > >;
template class gkg::DefaultItemIOFactory< gkg::Vector3d< double > >;

template class gkg::DefaultItemIOFactory< gkg::Polygon< 2U > >;
template class gkg::DefaultItemIOFactory< gkg::Polygon< 3U > >;
template class gkg::DefaultItemIOFactory< gkg::Polygon< 4U > >;

template class gkg::DefaultItemIOFactory< gkg::RGBComponent >;
