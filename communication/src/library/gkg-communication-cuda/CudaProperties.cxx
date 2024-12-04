#include <gkg-communication-cuda/CudaProperties.h>
#include <gkg-core-exception/Exception.h>

#ifdef GKG_CUDA
#include <cuda.h>
#include <cuda_runtime.h>
#endif

#include <cstring>


#ifdef GKG_CUDA
gkg::CudaProperties::CudaProperties( int32_t device )
#else
gkg::CudaProperties::CudaProperties( int32_t /* device */ )
#endif
                   : _name( "Unknown" ),
                     _globalMemory( 0 ),
                     _multiprocessors( 0 ),
                     _coresPerMP( 0 ),
                     _cores( 0 ),
                     _clockRate( 0 ),
                     _warpSize( 0 ),
                     _maxThreadsPerBlock( 0 )
{

  try
  {

#ifdef GKG_CUDA

    cudaDeviceProp properties;

    cudaSetDevice( device );
    cudaGetDeviceProperties( &properties, device );

    int32_t sm = ( properties.major << 4 ) + properties.minor;

    switch ( sm )
    {

      case 0x20:
        _coresPerMP = 32;
        break;

      case 0x21:
        _coresPerMP = 48;
        break;

      case 0x30:
      case 0x35:
        _coresPerMP = 192;
        break;

      case 0x10:
      case 0x11:
      case 0x12:
      case 0x13:
      default:
        _coresPerMP = 8;
        break;

    }

    _name = properties.name;
    _globalMemory = properties.totalGlobalMem;
    _multiprocessors = properties.multiProcessorCount;
    _cores = _coresPerMP * _multiprocessors;
    _clockRate = properties.clockRate;
    _warpSize = properties.warpSize;
    _maxThreadsPerBlock = properties.maxThreadsPerBlock;

    std::memcpy( _maxBlockDim,
                 properties.maxThreadsDim, 
                 3 * sizeof( int32_t ) );
    std::memcpy( _maxGridDim, properties.maxGridSize, 3 * sizeof( int32_t ) );

    cudaDeviceReset();

#else

    std::memset( _maxBlockDim, 0, 3 * sizeof( int32_t ) );
    std::memset( _maxGridDim, 0, 3 * sizeof( int32_t ) );

#endif

  }
  GKG_CATCH( "gkg::CudaProperties::CudaProperties( int32_t device )" );

}


gkg::CudaProperties::CudaProperties( const gkg::CudaProperties& other )
{

  copy( other );

}


gkg::CudaProperties& gkg::CudaProperties::operator = ( 
                                              const gkg::CudaProperties& other )
{

  copy( other );

  return *this;

}


void gkg::CudaProperties::copy( const gkg::CudaProperties& other )
{

  try
  {

    _name = other._name;
    _globalMemory = other._globalMemory;
    _multiprocessors = other._multiprocessors;
    _coresPerMP = other._coresPerMP;
    _cores = other._cores;
    _clockRate = other._clockRate;
    _warpSize = other._warpSize;
    _maxThreadsPerBlock = other._maxThreadsPerBlock;
    std::memcpy( _maxBlockDim, other._maxBlockDim, 3 * sizeof( int32_t ) );
    std::memcpy( _maxGridDim, other._maxGridDim, 3 * sizeof( int32_t ) );

  }
  GKG_CATCH( "void gkg::CudaProperties::copy( "
             "const gkg::CudaProperties& other )" );


}


std::ostream& std::operator << ( std::ostream& os, 
                                 const gkg::CudaProperties& properties )
{

  try
  {

    os << "Device name: " << properties._name << std::endl;
    os << "\tGlobalMemory: " << (int32_t)( properties._globalMemory / 
                                           1048576.0f + 0.5f )
       << "MBytes" << std::endl;
    os << "\tMultiprocessors: " << properties._multiprocessors << std::endl;
    os << "\tCores per multiprocessor: " 
       << properties._coresPerMP << std::endl;
    os << "\tCores: " << properties._cores << std::endl;
    os << "\tClock rate: " << properties._clockRate * 1e-3f
       << "MHz" << std::endl;
    os << "\tWarp size: " << properties._warpSize << std::endl;
    os << "\tMax threads per block: " 
       << properties._maxThreadsPerBlock << std::endl;
    os << "\tMax block dim: (" << properties._maxBlockDim[ 0 ]
       << "," << properties._maxBlockDim[ 1 ]
       << "," << properties._maxBlockDim[ 2 ] 
       << ")" << std::endl;
    os << "\tMax grid dim: (" << properties._maxGridDim[ 0 ]
       << "," << properties._maxGridDim[ 1 ]
       << "," << properties._maxGridDim[ 2 ] 
       << ")" << std::endl;

    return os;

  }
  GKG_CATCH( "std::ostream& std::operator << ( "
             "std::ostream& os, const gkg::CudaProperties& properties )" );

}
