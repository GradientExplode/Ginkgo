#ifndef _gkg_processing_algobase_NonLocalMeansFilter_i_h_
#define _gkg_processing_algobase_NonLocalMeansFilter_i_h_


#include <gkg-processing-algobase/NonLocalMeansFilter.h>
#include <gkg-processing-algobase/NLMNoiseModel.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/NonLocalMeansContextMmap_i.h>
#include <gkg-processing-algobase/NonLocalMeansContext_i.h>
#include <gkg-processing-algobase/NLMContributionContext_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>
#include <cstring>
#include <iomanip>
#include <vector>


template < class IN, class OUT >
inline
gkg::NonLocalMeansFilter< IN, OUT >::NonLocalMeansFilter( 
                                           const gkg::NLMNoiseModel& noiseModel,
                                           int32_t halfSearchBlockSize, 
                                           int32_t halfNeighborhoodSize, 
                                           float degreeOfFiltering )
                                   : _noiseModel( noiseModel ),
                                     _halfSearchBlockSize(
                                                          halfSearchBlockSize ),
                                     _halfNeighborhoodSize( 
                                                         halfNeighborhoodSize ),
                                     _degreeOfFiltering( degreeOfFiltering )
{
}


template < class IN, class OUT >
inline
gkg::NonLocalMeansFilter< IN, OUT >::~NonLocalMeansFilter()
{
}


template < class IN, class OUT >
inline
void gkg::NonLocalMeansFilter< IN, OUT >::filter( 
                                                 const gkg::Volume< IN >& in, 
                                                 gkg::Volume< OUT >& out ) const
{

  try
  {

    int32_t x, sizeX = in.getSizeX();
    int32_t y, sizeY = in.getSizeY();
    int32_t z, sizeZ = in.getSizeZ();
    int32_t t, sizeT = in.getSizeT();
    gkg::Vector3d< double > resolution;
    double resolutionT;

    in.getResolution( resolution, resolutionT );
    out.reallocate( sizeX, sizeY, sizeZ, sizeT );
    out.setResolution( resolution, resolutionT );
    out.fill( (OUT)0 );

    // length of search window
    int32_t hX = std::min( _halfSearchBlockSize, ( sizeX - 1 ) / 2 );
    int32_t hY = std::min( _halfSearchBlockSize, ( sizeY - 1 ) / 2 );
    int32_t hZ = std::min( _halfSearchBlockSize, ( sizeZ - 1 ) / 2 );
    int32_t bWidth = -( 2 * hX + 1 ) * ( hZ * ( 2 * hY + 1 ) + hY + hX );
    std::vector< gkg::Vector3d< int32_t > > block;

    for ( z = -hZ; z <= hZ; z++ )
    {

      for ( y = -hY; y <= hY; y++ )
      {

        for ( x = -hX; x <= hX; x++, bWidth++ )
        {

          if ( bWidth < 0 )
          {

            block.push_back( gkg::Vector3d< int32_t >( x, y, z ) );

          }

        }

      }

    }

    if ( in.isMemoryMapped() || out.isMemoryMapped() )
    {

      gkg::Volume< float > volOut( sizeX, sizeY, sizeZ );
      gkg::Volume< float > M( sizeX, sizeY, sizeZ );
      gkg::Volume< float > Z( sizeX, sizeY, sizeZ );

      for ( t = 0; t < sizeT; t++ )
      {

        if ( t )
        {

          std::cout << gkg::Eraser( 7 );

        }
        std::cout << "t = " << std::setw( 3 ) << t << std::flush;

        volOut.fill( 0.0f );
        M.fill( 0.0f );
        Z.fill( 0.0f );

        gkg::NonLocalMeansContextMmap< IN, OUT > process( in,
                                                          out,
                                                          volOut,
                                                          M,
                                                          Z,
                                                          _noiseModel,
                                                          block,
                                                          _halfNeighborhoodSize,
                                                          _degreeOfFiltering,
                                                          t );

        process.execute();

      }

      std::cout << gkg::Eraser( 7 ) << std::endl;

    }
    else
    {

      int32_t sizeXYZ = in.getSizeXYZ();
      float* volOut = new float[ sizeXYZ ];
      float* Z = new float[ sizeXYZ ];
      float* M = new float[ sizeXYZ ];

      if ( !volOut || !Z || !M )
      {

        delete[] volOut;
        delete[] Z;
        delete[] M;
        throw std::runtime_error( "not enough memory" );

      }

      for ( t = 0; t < sizeT; t++ )
      {

        if ( t )
        {

          std::cout << gkg::Eraser( 7 );

        }
        std::cout << "t = " << std::setw( 3 ) << t << std::flush;

        std::memset( volOut, 0, sizeXYZ * sizeof( float ) );
        std::memset( Z, 0, sizeXYZ * sizeof( float ) );
        std::memset( M, 0, sizeXYZ * sizeof( float ) );

        gkg::NonLocalMeansContext< IN > context( in,
                                                 volOut,
                                                 M,
                                                 Z,
                                                 _noiseModel,
                                                 block,
                                                 _halfNeighborhoodSize,
                                                 _degreeOfFiltering,
                                                 t );
        gkg::ThreadedLoop< int32_t >
          threadedLoop( &context,
                        0,
                        int32_t( block.size() ) );

        threadedLoop.launch();

        gkg::NLMContributionContext< IN, OUT > context2( in,
                                                         out,
                                                         volOut,
                                                         M,
                                                         Z,
                                                         _noiseModel,
                                                         t );
        gkg::ThreadedLoop< int32_t >
          threadedLoop2( &context2,
                         0,
                         in.getSizeXYZ() );

        threadedLoop2.launch();

      }

      std::cout << gkg::Eraser( 7 ) << std::endl;
      delete[] M;
      delete[] Z;
      delete[] volOut;

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::NonLocalMeansFilter< IN, OUT >::filter( "
             "const gkg::Volume< IN >& in, gkg::Volume< OUT >& out ) const" );

}

#endif
