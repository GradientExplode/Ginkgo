#include <gkg-communication-cuda/CudaDevices.h>
#include <gkg-core-exception/Exception.h>


gkg::CudaDevices::CudaDevices()
{

#ifdef GKG_CUDA
  try
  {

    int32_t deviceCount = 0;

    cudaError_t errorId = cudaGetDeviceCount( &deviceCount );

    if ( errorId == cudaSuccess )
    {

      int32_t d;

      for ( d = 0; d < deviceCount; d++ )
      {

        _devices.push_back( gkg::CudaProperties( d ) );

      }

    }

  }
  GKG_CATCH( "gkg::CudaDevices::CudaDevices()" );
#endif

}


size_t gkg::CudaDevices::size()
{

  try
  {

    return _devices.size();

  }
  GKG_CATCH( "size_t gkg::CudaDevices::size()" );

}


int32_t gkg::CudaDevices::getBestDeviceId( 
#ifdef GKG_CUDA
                                       const struct cudaDeviceProp* properties )
#else
                                 const struct cudaDeviceProp* /* properties */ )
#endif
{

  try
  {

    int32_t id = -1;

#ifdef GKG_CUDA
    if ( _devices.size() )
    {

      if ( properties )
      {

        int32_t cudaId = -1;
        cudaError_t errorId = cudaChooseDevice( &cudaId, properties );

        if ( errorId == cudaSuccess )
        {

          id = cudaId;

        }

      }
      else
      {

        float maxPerf = 0.0f;
        int i, maxI = (int32_t)_devices.size();

        for ( i = 0, id = 0; i < maxI; i++ )
        {

          float perf = float( _devices[ i ]._cores ) * 
                       float( _devices[ i ]._clockRate );

          if ( perf > maxPerf )
          {

            maxPerf = perf;
            id = i;

          }

        }

      }

    }
#endif

    return id;

  }
  GKG_CATCH( "int32_t gkg::CudaDevices::getBestDeviceId( "
             "const struct cudaDeviceProp* properties )" );

}


gkg::CudaProperties gkg::CudaDevices::getProperties( int32_t deviceId )
{

  try
  {

    gkg::CudaProperties properties;


    if ( ( deviceId < 0 ) || ( deviceId >= int32_t( _devices.size() ) ) )
    {

      std::cerr << "Wrong CUDA devide ID value" << std::endl;

    }

    if ( _devices.size() )
    {

      properties = _devices[ deviceId ];

    }

    return properties;

  }
  GKG_CATCH( "gkg::CudaProperties gkg::CudaDevices::getProperties( "
             "int32_t deviceId )" );

}


void gkg::CudaDevices::print()
{

  try
  {

    if ( _devices.size() )
    {

      std::vector< gkg::CudaProperties >::iterator
        d = _devices.begin(),
        de = _devices.end();

      while ( d != de )
      {

        std::cout << *d << std::endl;
        ++d;

      }

    }

  }
  GKG_CATCH( "void gkg::CudaDevices::print()" );

}
