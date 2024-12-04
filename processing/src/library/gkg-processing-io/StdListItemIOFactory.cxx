#include <gkg-core-io/StdListItemIOFactory_i.h>
#include <complex>

#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-coordinates/Vector3d.h>


template class gkg::StdListItemIOFactory< gkg::Vector2d< int16_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector2d< int32_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector2d< int64_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector2d< uint16_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector2d< uint32_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector2d< uint64_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector2d< float > >;
template class gkg::StdListItemIOFactory< gkg::Vector2d< double > >;

template class gkg::StdListItemIOFactory< gkg::Vector3d< int16_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector3d< int32_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector3d< int64_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector3d< uint16_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector3d< uint32_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector3d< uint64_t > >;
template class gkg::StdListItemIOFactory< gkg::Vector3d< float > >;
template class gkg::StdListItemIOFactory< gkg::Vector3d< double > >;

