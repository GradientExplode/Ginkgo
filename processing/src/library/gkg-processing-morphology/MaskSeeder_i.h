#ifndef _gkg_processing_morphology_MaskSeeder_i_h_
#define _gkg_processing_morphology_MaskSeeder_i_h_


#include <gkg-processing-morphology/MaskSeeder.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


template < class IN, class OUT >
inline
gkg::MaskSeeder< IN, OUT >::MaskSeeder(
                                    const gkg::TestFunction< IN >& testFunction,
                                    const OUT& foreground,
                                    const OUT& background,
                                    const OUT& startLabel )
                           : _testFunction( testFunction ),
                             _foreground( foreground ),
                             _background( background ),
                             _startLabel( startLabel )
{
}


template < class IN, class OUT >
inline
gkg::MaskSeeder< IN, OUT >::~MaskSeeder()
{
}


template < class IN, class OUT >
inline
int64_t gkg::MaskSeeder< IN, OUT >::seed( const gkg::Volume< IN >& maskVolume,
                                          int32_t parcelVoxelCount,
                                          int32_t kMeansIterationCount,
                                          float ratioOfDistanceVariance,
                                          gkg::Volume< OUT >& seedVolume ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check
    ////////////////////////////////////////////////////////////////////////////
    if ( maskVolume.getSizeT() != 1 )
    {

      throw std::runtime_error( "sizeT of mask volume must be equal to 1" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reallocating seed volume similarly to mask volume
    ////////////////////////////////////////////////////////////////////////////
    this->reallocate( maskVolume, seedVolume );


    ////////////////////////////////////////////////////////////////////////////
    // computing number of voxels in the mask and preparing the output seed
    // volume by initializing the voxels not contained in the mask to
    // background value and the voxels included in the mask to foreground
    ////////////////////////////////////////////////////////////////////////////
    int64_t voxelCountInMask = 0;
    typename gkg::Volume< IN >::const_iterator
      m = maskVolume.begin(),
      me = maskVolume.end();
    typename gkg::Volume< OUT >::iterator
      o = seedVolume.begin();
    while ( m != me )
    {

      if ( _testFunction.getState( *m ) )
      {

        ++ voxelCountInMask;
        *o = _foreground;

      }
      else
      {

        *o = _background;

      }
      ++ m;
      ++ o;

    }

    if ( voxelCountInMask == ( int64_t )0 )
    {

      return ( int64_t )0;
      //throw std::runtime_error( "empty mask" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the seed count
    ////////////////////////////////////////////////////////////////////////////
    int64_t seedCount = voxelCountInMask / parcelVoxelCount;

    if ( seedCount == ( int64_t )0 )
    {

      return ( int64_t )0;
      //throw std::runtime_error( "seed count is nul!" );

    }

    if ( seedCount > voxelCountInMask )
    {

      return ( int64_t )0;
      //throw std::runtime_error( 
      //                  "more seeds required than the voxel count in mask!" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating the vector of voxel coordinates
    ////////////////////////////////////////////////////////////////////////////
    std::vector< gkg::Vector3d< float > > voxels( voxelCountInMask );

    int32_t sizeX = maskVolume.getSizeX();
    int32_t sizeY = maskVolume.getSizeY();
    int32_t sizeZ = maskVolume.getSizeZ();
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    int64_t voxelIndex = 0;
    float sumOfX = 0.0;
    float sumOfY = 0.0;
    float sumOfZ = 0.0;
    float sumOfXSquare = 0.0;
    float sumOfYSquare = 0.0;
    float sumOfZSquare = 0.0;

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( _testFunction.getState( maskVolume( x, y, z ) ) )
          {

            gkg::Vector3d< float >& voxel = voxels[ voxelIndex ];
            voxel.x = ( float )x;
            voxel.y = ( float )y;
            voxel.z = ( float )z;

            sumOfX += voxel.x;
            sumOfY += voxel.y;
            sumOfZ += voxel.z;

            sumOfXSquare += voxel.x * voxel.x;
            sumOfYSquare += voxel.y * voxel.y;
            sumOfZSquare += voxel.z * voxel.z;

            ++ voxelIndex;

          }

        }

      }

    }

    float globalVariance = ( ( sumOfXSquare - sumOfX * sumOfX / 
                               voxelCountInMask ) +
                             ( sumOfYSquare - sumOfY * sumOfY / 
                               voxelCountInMask ) +
                             ( sumOfZSquare - sumOfZ * sumOfZ / 
                               voxelCountInMask )
                            ) / 3.0;

    float meanOfX = sumOfX / voxelCountInMask;
    float meanOfY = sumOfY / voxelCountInMask;
    float meanOfZ = sumOfZ / voxelCountInMask;

    gkg::Vector3d< float > meanVoxel( meanOfX, meanOfY, meanOfZ );

    
    ////////////////////////////////////////////////////////////////////////////
    // creating a vector of random indices between 0 and voxelCountInMask - 1
    ////////////////////////////////////////////////////////////////////////////
    std::vector< int64_t > randomIndices( seedCount );
    gkg::getRandomIndexVector( randomIndices, voxelCountInMask - 1 );

    // ordering selected indices
    std::sort( randomIndices.begin(), randomIndices.end() );


    ////////////////////////////////////////////////////////////////////////////
    // initializing the seeds before running the kmeans
    ////////////////////////////////////////////////////////////////////////////

    std::vector< gkg::Vector3d< float > > seeds( seedCount );
    std::vector< gkg::Vector3d< float > > oldSeeds = seeds;

    std::vector< int64_t >::const_iterator
      ri = randomIndices.begin(),
      rie = randomIndices.end();
    std::vector< gkg::Vector3d< float > >::iterator s = seeds.begin();
    while ( ri != rie )
    {

      *s = voxels[ *ri ];
      ++ ri;
      ++ s;

    }


    ////////////////////////////////////////////////////////////////////////////
    // running the kmeans
    ////////////////////////////////////////////////////////////////////////////

    // looping over kmeans iteration(s)
    int32_t i = 0;
    int64_t seedIndex = 0;
    for ( i = 0; i < kMeansIterationCount; i++ )
    {

      // computing for each voxel its closest seed and the distance to this seed
      std::vector< int64_t > closestSeedIndices( voxelCountInMask, 0 );
      std::vector< float > closestSeedSquareDistances( voxelCountInMask, 1e38 );

      // looping over seed(s)
      for ( seedIndex = 0; seedIndex < seedCount; seedIndex++ )
      {

        // looping over voxel(s)
        for ( voxelIndex = 0; voxelIndex < voxelCountInMask; voxelIndex++ )
        {

          float squareDistance = ( voxels[ voxelIndex ] -
                                   seeds[ seedIndex ] ).getNorm2();

          if ( squareDistance < closestSeedSquareDistances[ voxelIndex ] )
          {

            closestSeedIndices[ voxelIndex ] = seedIndex;
            closestSeedSquareDistances[ voxelIndex ] = squareDistance;

          }

        }

      }

      // computing the energy of the k-means, eg the sum of distances for all 
      // the voxels
      float energy = 0.0;
      std::vector< float >::const_iterator
        d = closestSeedSquareDistances.begin(),
        de = closestSeedSquareDistances.end();
      while ( d != de )
      {

        energy += *d;
        ++ d;

      }

      // resetting all centers to the mean x, y, and z voxel coordinates
      std::fill( seeds.begin(), seeds.end(), meanVoxel );

      // computing the mean coordinates of voxels attached to a seed, and
      // replace the seed coordinates by the mean coordinates
      std::vector< gkg::Vector3d< float > >
      meanSeedCoordinates( seedCount, gkg::Vector3d< float >( 0.0, 0.0, 0.0 ) );
      std::vector< int64_t > seedVoxelCounts( seedCount, 0 );
    
      for ( voxelIndex = 0; voxelIndex < voxelCountInMask; voxelIndex++ )
      {

        meanSeedCoordinates[ closestSeedIndices[ voxelIndex ] ] +=
          voxels[ voxelIndex ];
        ++ seedVoxelCounts[ closestSeedIndices[ voxelIndex ] ];

      }

      for ( seedIndex = 0; seedIndex < seedCount; seedIndex++ )
      {

        if ( seedVoxelCounts[ seedIndex ] )
        {

          seeds[ seedIndex ] = meanSeedCoordinates[ seedIndex ] /
                               ( float )seedVoxelCounts[ seedIndex ];

        }

      }

      // computing the square difference of seed displacements
      std::vector< gkg::Vector3d< float > >::const_iterator
        cs = seeds.begin(),
        cse = seeds.end();
      std::vector< gkg::Vector3d< float > >::const_iterator
        cos = oldSeeds.begin();
      float sumOfSeedDisplacements = 0.0;
      while ( cs != cse )
      {

        sumOfSeedDisplacements += ( *cos - *cs ).getNorm2();
        ++ cs;
        ++ cos;

      }

      if ( sumOfSeedDisplacements <
           ( globalVariance * ratioOfDistanceVariance ) )
      {

        break;

      }

      // copying seeds to old seeds
      oldSeeds = seeds;

    }

    ////////////////////////////////////////////////////////////////////////////
    // filling the output seed volume with the seeds
    ////////////////////////////////////////////////////////////////////////////

    std::vector< gkg::Vector3d< float > >::const_iterator
      cs = seeds.begin(),
      cse = seeds.end();
    gkg::Vector3d< int32_t > seedVoxel;
    int32_t seedLabel = _startLabel;
    while ( cs != cse )
    {

      seedVoxel.x = gkg::round( cs->x );
      seedVoxel.y = gkg::round( cs->y );
      seedVoxel.z = gkg::round( cs->z );

      seedVolume( seedVoxel ) = seedLabel;

      seedLabel += ( OUT )1;

      ++ cs;

    }

    return seedCount;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "int64_t gkg::MaskSeeder< IN, OUT >::seed( "
             "const gkg::Volume< IN >& maskVolume, "
             "int32_t seedCount, "
             "gkg::Volume< OUT >& seedVolume ) const" );

}


template < class IN, class OUT >
inline
void gkg::MaskSeeder< IN, OUT >::reallocate( const gkg::Volume< IN >& in,
                                             gkg::Volume< OUT >& out ) const
{

  try
  {

    if ( ( void* )&out != ( void* )&in )
    {

      // reallocating data
      out.reallocate( in.getSizeX(),
                      in.getSizeY(),
                      in.getSizeZ() );

      // duplicating header
      out.getHeader() = in.getHeader();
      out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::Binarizer< gkg::Volume< IN >, gkg::Volume< OUT > >::"
             "reallocate( "
             "const gkg::Volume< IN >& in, "
             "gkg::Volume< OUT >& out ) const" );

}




#endif

