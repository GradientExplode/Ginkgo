#include <gkg-processing-mask/MaskAlgorithm.h>
#include <gkg-core-cppext/StdInt.h>


template < class IN, class OUT >
gkg::MaskAlgorithm< IN, OUT >::MaskAlgorithm()
{
}


template < class IN, class OUT >
gkg::MaskAlgorithm< IN, OUT >::~MaskAlgorithm()
{
}


// force instanciation for standard types

template class gkg::MaskAlgorithm< int8_t, uint8_t >;
template class gkg::MaskAlgorithm< int8_t, int16_t >;
template class gkg::MaskAlgorithm< int8_t, float >;
template class gkg::MaskAlgorithm< int8_t, double >;

template class gkg::MaskAlgorithm< uint8_t, uint8_t >;
template class gkg::MaskAlgorithm< uint8_t, int16_t >;
template class gkg::MaskAlgorithm< uint8_t, float >;
template class gkg::MaskAlgorithm< uint8_t, double >;

template class gkg::MaskAlgorithm< int16_t, uint8_t >;
template class gkg::MaskAlgorithm< int16_t, int16_t >;
template class gkg::MaskAlgorithm< int16_t, float >;
template class gkg::MaskAlgorithm< int16_t, double >;

template class gkg::MaskAlgorithm< uint16_t, uint8_t >;
template class gkg::MaskAlgorithm< uint16_t, int16_t >;
template class gkg::MaskAlgorithm< uint16_t, float >;
template class gkg::MaskAlgorithm< uint16_t, double >;

template class gkg::MaskAlgorithm< int32_t, uint8_t >;
template class gkg::MaskAlgorithm< int32_t, int16_t >;
template class gkg::MaskAlgorithm< int32_t, float >;
template class gkg::MaskAlgorithm< int32_t, double >;

template class gkg::MaskAlgorithm< uint32_t, uint8_t >;
template class gkg::MaskAlgorithm< uint32_t, int16_t >;
template class gkg::MaskAlgorithm< uint32_t, float >;
template class gkg::MaskAlgorithm< uint32_t, double >;

template class gkg::MaskAlgorithm< int64_t, uint8_t >;
template class gkg::MaskAlgorithm< int64_t, int16_t >;
template class gkg::MaskAlgorithm< int64_t, float >;
template class gkg::MaskAlgorithm< int64_t, double >;

template class gkg::MaskAlgorithm< uint64_t, uint8_t >;
template class gkg::MaskAlgorithm< uint64_t, int16_t >;
template class gkg::MaskAlgorithm< uint64_t, float >;
template class gkg::MaskAlgorithm< uint64_t, double >;

template class gkg::MaskAlgorithm< float, uint8_t >;
template class gkg::MaskAlgorithm< float, int16_t >;
template class gkg::MaskAlgorithm< float, float >;
template class gkg::MaskAlgorithm< float, double >;

template class gkg::MaskAlgorithm< double, uint8_t >;
template class gkg::MaskAlgorithm< double, int16_t >;
template class gkg::MaskAlgorithm< double, float >;
template class gkg::MaskAlgorithm< double, double >;

