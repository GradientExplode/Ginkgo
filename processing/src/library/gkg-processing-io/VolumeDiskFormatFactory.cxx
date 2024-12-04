#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <complex>


template class gkg::DiskFormatFactory< gkg::Volume< int8_t > >;
template class gkg::DiskFormatFactory< gkg::Volume< uint8_t > >;
template class gkg::DiskFormatFactory< gkg::Volume< int16_t > >;
template class gkg::DiskFormatFactory< gkg::Volume< uint16_t > >;
template class gkg::DiskFormatFactory< gkg::Volume< int32_t > >;
template class gkg::DiskFormatFactory< gkg::Volume< uint32_t > >;
template class gkg::DiskFormatFactory< gkg::Volume< int64_t > >;
template class gkg::DiskFormatFactory< gkg::Volume< uint64_t > >;
template class gkg::DiskFormatFactory< gkg::Volume< float > >;
template class gkg::DiskFormatFactory< gkg::Volume< double > >;
template class gkg::DiskFormatFactory< gkg::Volume< bool > >;

template class gkg::DiskFormatFactory< gkg::Volume< std::complex< float > > >;
template class gkg::DiskFormatFactory< gkg::Volume< std::complex< double> > >;
template class gkg::DiskFormatFactory< gkg::Volume< gkg::RGBComponent > >;
template class gkg::DiskFormatFactory< gkg::Volume< gkg::Vector3d< float> > >;
template class gkg::DiskFormatFactory< gkg::Volume< gkg::Vector3d<double> > >;


