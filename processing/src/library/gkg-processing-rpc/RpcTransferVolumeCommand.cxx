#include <gkg-processing-rpc/RpcTransferVolumeCommand_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>


template class gkg::RpcTransferVolumeCommand< uint8_t >;
template class gkg::RpcTransferVolumeCommand< int8_t >;
template class gkg::RpcTransferVolumeCommand< uint16_t >;
template class gkg::RpcTransferVolumeCommand< int16_t >;
template class gkg::RpcTransferVolumeCommand< uint32_t >;
template class gkg::RpcTransferVolumeCommand< int32_t >;
template class gkg::RpcTransferVolumeCommand< uint64_t >;
template class gkg::RpcTransferVolumeCommand< int64_t >;
template class gkg::RpcTransferVolumeCommand< float >;
template class gkg::RpcTransferVolumeCommand< double >;
template class gkg::RpcTransferVolumeCommand< gkg::RGBComponent >;
