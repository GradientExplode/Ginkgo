#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <complex>
#include <list>


template class gkg::DiskFormatFactory< gkg::TextureMap< int8_t > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< uint8_t > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< int16_t > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< uint16_t > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< int32_t > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< uint32_t > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< int64_t > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< uint64_t > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< float > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< double > >;
template class gkg::DiskFormatFactory< gkg::TextureMap< bool > >;

template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::complex< float > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::complex< double> > >;
template class gkg::DiskFormatFactory<
                                         gkg::TextureMap< gkg::RGBComponent > >;
template class gkg::DiskFormatFactory<
                                    gkg::TextureMap< gkg::Vector3d< float > > >;
template class gkg::DiskFormatFactory<
                                   gkg::TextureMap< gkg::Vector3d< double > > >;

template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< int8_t > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< uint8_t > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< int16_t > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< uint16_t > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< int32_t > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< uint32_t > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< int64_t > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< uint64_t > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< float > > >;
template class gkg::DiskFormatFactory<
                                     gkg::TextureMap< std::list< double > > >;


