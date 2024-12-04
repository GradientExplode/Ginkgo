#include <gkg-mri-reconstruction-parallel/PreLearnReconstructor.h>
#include <gkg-processing-numericalanalysis/IllMatrixInverser.h>
#include <gkg-processing-numericalanalysis/CNormWeighting.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-resampling/QuickResampler.h>
#include <gkg-processing-resampling/Spreader.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>



template < class T >
gkg::PreLearnReconstructor< T >::PreLearnReconstructor()
                             : _quickResampler( 0 ),
                               _spreader( 0 ),
                               _cNormWeighting( 0 ),
                               _unfoldingMatricesProcessed( false )
{
}


template < class T >
gkg::PreLearnReconstructor< T >::~PreLearnReconstructor()
{
}


template < class T >
void gkg::PreLearnReconstructor< T >::setReference( 
                             const gkg::Volume< std::complex< T > >& reference )
{

  try
  {

    if ( reference.getSizeT() != 1 )
    {

      throw std::runtime_error( "T size of the reference must be equal to 1" );

    }
    _reference = reference;
    _unfoldingMatricesProcessed = true;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PreLearnReconstructor< T >::setReference( "
             "const gkg::Volume< std::complex< T > >& reference )" );

}


template < class T >
void gkg::PreLearnReconstructor< T >::setFoldedReference( 
                       const gkg::Volume< std::complex< T > >& foldedReference )
{

  try
  {

    _foldedReference = foldedReference;
    _unfoldingMatricesProcessed = true;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PreLearnReconstructor< T >::setFoldedReferenceImage( "
             "const gkg::Volume< std::complex< T > >& foldedReference )" );

}


template < class T >
void gkg::PreLearnReconstructor< T >::setNeighborhood(
                                  gkg::Neighborhood3d::Type neighborhood3dType )
  std::list< Vector3d< int > > neighborhood3d )
{

  try
  {

    _neighborOffsets = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                                           neighborhood3dType );
    _unfoldingMatricesProcessed = true;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PreLearnReconstructor< T >::setNeighborhood( "
             "gkg::Neighborhood3d::Type neighborhood3dType )" );

}


template < class T >
void gkg::PreLearnReconstructor< T >::setQuickResampler(
                const gkg::QuickResampler< std::complex< T > >& quickResampler )
{

  try
  {

    _quickResampler = &quickResampler;
    _unfoldingMatricesProcessed = true;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PreLearnReconstructor< T >::setQuickResampler( "
             "const gkg::QuickResampler< std::complex< T > >& "
             "quickResampler )" );

}


template < class T >
void gkg::PreLearnReconstructor< T >::setSpreader(
                            const gkg::Spreader< std::complex< T > >& spreader )
{

  try
  {

    _spreader = &spreader;
    _unfoldingMatricesProcessed = true;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PreLearnReconstructor< T >::setSpreader( "
             "const gkg::Spreader< std::complex< T > >& spreader )" );

}


template < class T >
void gkg::PreLearnReconstructor< T >::setCNormWeighting(
                                     const gkg::CNormWeighting& cNormWeighting )
{

  try
  {

    _cNormWeighting = &cNormWeighting;
    _unfoldingMatricesProcessed = true;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PreLearnReconstructor< T >::setCNormWeighting( "
             "const gkg::CNormWeighting& cNormWeighting )" );

}



template < class T >
void gkg::PreLearnReconstructor< T >::processUnfoldingMatrices( bool verbose )
{

  try
  {

    // sanity check
    if ( reconstructed.getSizeT() != 1 )
    {

      throw std::runtime_error(
                      "T size of the reconstructed output must be equal to 1" );

    }
    if ( !_quickResampler )
    {

      throw std::runtime_error( "not a valid quick resampler" );

    }
    if ( !_spreader )
    {

      throw std::runtime_error( "not a valid spreader" );

    }
    if ( _neighborOffets.empty() )
    {

      throw std::runtime_error( "not a valid neighborhood" );

    }

    int32_t referenceSizeX = _reference.getSizeX();
    int32_t referenceSizeY = _reference.getSizeY();
    int32_t referenceSizeZ = _reference.getSizeZ();

    int32_t foldedReferenceSizeX = _foldedReference.getSizeX();
    int32_t foldedReferenceSizeY = _foldedReference.getSizeY();
    int32_t foldedReferenceSizeZ = _foldedReference.getSizeZ();

    int32_t coilCount = _foldedReference.getSizeT();

    int32_t unfoldedXPositionCount =
      this->ParallelReconstructor< T >::_xReductionStrategy.
                                                     getUnfoldedPositionCount();
    int32_t unfoldedYPositionCount =
      this->ParallelReconstructor< T >::_yReductionStrategy.
                                                     getUnfoldedPositionCount();
    int32_t unfoldedZPositionCount =
      this->ParallelReconstructor< T >::_zReductionStrategy.
                                                     getUnfoldedPositionCount();

    int32_t neighborCount = ( int32_t )_neighborOffsets.size();

    gkg::CMatrix F( coilCount, neighborCount + 1 );
    gkg::CMatrix R( unfoldedXPositionCount *
                    unfoldedYPositionCount * 
                    unfoldedZPositionCount,
		    neighborCount + 1 );

    _unfoldingMatrices.reallocate( foldedReferenceSizeX,
                                   foldedReferenceSizeY,
                                   foldedReferenceSizeZ );

    std::vector< T > unfoldedXPositions;
    std::vector< T > unfoldedYPositions;
    std::vector< T > unfoldedZPositions;

    int32_t x, y, z;
    for ( z = 0; z < foldedReferenceSizeZ; z++ )
    {

      if ( verbose )
      {


        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1
                  << " / " << std::setw( 4 ) << foldedReferenceSizeZ
                  << " ]" << std::flush;

      }

      this->ParallelReconstructor< T >::_zReductionStrategy.
                          getUnfoldedPositions( ( T )z,
                                                referenceSizeZ,
                                                unfoldedZPositions );

      for ( y = 0; y < foldedReferenceSizeY; y++ )
      {

        this->ParallelReconstructor< T >::_yReductionStrategy.
                            getUnfoldedPositions( ( float )y,
                                                  referenceSizeY,
                                                  unfoldedYPositions );

        for ( x = 0; x < foldedReferenceSizeX; x++ )
        {

          this->ParallelReconstructor< T >::_xReductionStrategy.
                             getUnfoldedPositions( ( float )x,
                                                    referenceSizeX,
                                                    unfoldedXPositions );

          R = buildR( unfoldedXPositions,
                      unfoldedYPositions, 
	              unfoldedZPositions );
	  F = buildF( x, y, z );


///////////////////////////////////////////////////////////////////////:

du code a mettre la pour construire _unfoldingMatrices

////////////////////////////////////////////////////////////////////////


        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 );

    }

    _unfoldingMatricesProcessed = true;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PreLearnReconstructor< T >::"
             "processUnfoldingMatrices( bool verbose )" );

}


template < class T >
void gkg::PreLearnReconstructor< T >::reconstruct(
                                const gkg::Volume< std::complex< T > >& raw,
                                gkg::Volume< std::complex< T > >& reconstructed,
                                bool verbose ) const
{

  try
  {

    if ( !_unfoldingMatricesProcessed )
    {

      throw std::runtime_error( "need to call processUnfoldingMatrices() "
                                "before launching reconstruction" );

    }

    int32_t rawSizeX = raw.getSizeX();
    int32_t rawSizeY = raw.getSizeY();
    int32_t rawSizeZ = raw.getSizeZ();
    int32_t coilCount = raw.getSizeT();

    int32_t reconstructedSizeX = reconstructed.getSizeX();
    int32_t reconstructedSizeY = reconstructed.getSizeY();
    int32_t reconstructedSizeZ = reconstructed.getSizeZ();

    gkg::Vector3d< double > outResolution( 1.0, 1.0, 1.0 );

    // clearing unfolded volume
    reconstructed.fill( 0.0 );

    int32_t x, y, z, c, ux, uy, uz, index;

    std::vector< T > unfoldedXPositions;
    std::vector< T > unfoldedYPositions;
    std::vector< T > unfoldedZPositions;

    int32_t unfoldedXPositionCount =
      this->ParallelReconstructor< T >::_xReductionStrategy.
                                                     getUnfoldedPositionCount();
    int32_t unfoldedYPositionCount =
      this->ParallelReconstructor< T >::_yReductionStrategy.
                                                     getUnfoldedPositionCount();
    int32_t unfoldedZPositionCount =
      this->ParallelReconstructor< T >::_zReductionStrategy.
                                                     getUnfoldedPositionCount();
    
    gkg::CVector folded( coilCount );
    gkg::CVector unfolded( unfoldedXPositionCount *
                           unfoldedYPositionCount * 
                           unfoldedZPositionCount );
    gkg::Vector3d< double > outLocation;
    std::complex< T > value;

    for ( z = 0; z < rawSizeZ; z++ )
    {

      if ( verbose )
      {


        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1
                  << " / " << std::setw( 4 ) << rawSizeZ
                  << " ]" << std::flush;

      }

      this->ParallelReconstructor< T >::_zReductionStrategy.
                          getUnfoldedPositions( ( T )z,
                                                reconstructedSizeZ,
                                                unfoldedZPositions );

      for ( y = 0; y < rawSizeY; y++ )
      {

        this->ParallelReconstructor< T >::_yReductionStrategy.
                            getUnfoldedPositions( ( float )y,
                                                  reconstructedSizeY,
                                                  unfoldedYPositions );

        for ( x = 0; x < rawSizeX; x++ )
        {

          this->ParallelReconstructor< T >::_xReductionStrategy.
                             getUnfoldedPositions( ( float )x,
                                                    reconstructedSizeX,
                                                    unfoldedXPositions );

          // Filling folded voxels to be reconstructed
          for ( c = 0; c < coilCount; c++ )
          {

            folded( c ) = raw( x, y, z, c );
	  
	  }


          if ( _cNormWeighting )
          {
	  
            folded = _cNormWeighting->getWeighting().getComposition( folded );

          }
          unfolded = _operators( x, y, z ).getComposition( folded );

          index = 0;
          for ( uz = 0; uz < unfoldedZPositionCount; uz++ )
          {

            for ( uy = 0; uy < unfoldedYPositionCount; uy++ )
            {

              for ( ux = 0; ux < unfoldedXPositionCount; ux++ )
              {

                outLocation.x = ( double )unfoldedXPositions[ ux ];
                outLocation.y = ( double )unfoldedYPositions[ uy ];
                outLocation.z = ( double )unfoldedZPositions[ uz ];
                value = std::complex< T >( unfolded( index ) );

                _spreader->spread( value,
                                   outLocation,
                                   reconstructed,
                                   &outResolution );
                ++ index;

              }

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PreLearnReconstructor< T >::reconstruct( "
             "const gkg::Volume< std::complex< T > >& raw, "
             "gkg::Volume< std::complex< T > >& reconstructed, "
             "bool verbose ) const" );

}


template class gkg::PreLearnReconstructor< float >;
template class gkg::PreLearnReconstructor< double >;
