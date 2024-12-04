#ifndef _gkg_processing_denoising_deblurring_FastIterativeShrinkageThresholdingAlgorithm_i_h_
#define _gkg_processing_denoising_deblurring_FastIterativeShrinkageThresholdingAlgorithm_i_h_

#include <gkg-processing-denoising-deblurring/FastIterativeShrinkageThresholdingAlgorithm.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>


template < class T, class M, class K >
inline
gkg::FastIterativeShrinkageThresholdingAlgorithm< T, M, K >::
                                   FastIterativeShrinkageThresholdingAlgorithm()
{

  try
  {

    _factory = 
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

  }
  GKG_CATCH( "" );

}


template < class T, class M, class K >
inline
gkg::FastIterativeShrinkageThresholdingAlgorithm< T, M, K >::
                                  ~FastIterativeShrinkageThresholdingAlgorithm()
{




}


template < class T, class M, class K >
inline
void gkg::FastIterativeShrinkageThresholdingAlgorithm< T, M, K >::solve(
                                     const gkg::Volume< T >& input,
                                     const gkg::Volume< M >& mask,
                                     const gkg::Volume< K >& kernel,
                                     const gkg::TestFunction< M >& testFunction,
                                     const T& background,
                                     int32_t maximumIterationCount,
                                     double tolerance,
                                     double shrinkageStep,
                                     int32_t skrinkageMaximumIterationCount,
                                     double shrinkageTolerance,
                                     gkg::Volume< T >& output ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting input volume size and resolution
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > inputSize;
    int32_t inputSizeT = 0;
    input.getSize( inputSize, inputSizeT );
    int32_t inputSizeXY = inputSize.x * inputSize.y;

    if ( inputSizeT != 1 )
    {

      throw std::runtime_error( "input cannot be a 4D volume" );

    }    

    gkg::Vector3d< double > inputResolution;
    double inputResolutionT = 0;
    input.getResolution( inputResolution, inputResolutionT );

    gkg::BoundingBox< int32_t > inputBoundingBox( input );


    ////////////////////////////////////////////////////////////////////////////
    // sanity check
    ////////////////////////////////////////////////////////////////////////////

    if ( mask.getSizeT() != 1 )
    {

      throw std::runtime_error( "mask cannot be a 4D volume" );

    }    
    if ( ( mask.getSizeX() != inputSize.x ) ||
         ( mask.getSizeY() != inputSize.y ) ||
         ( mask.getSizeZ() != inputSize.z ) )
    {

      throw std::runtime_error( "inconsistent mask and input volume size(s)" );


    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting the kernel size and half-size
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > kernelSize;
    int32_t kernelSizeT = 1;
    kernel.getSize( kernelSize, kernelSizeT );

    if ( kernelSizeT != 1 )
    {

      throw std::runtime_error( "kernel cannot be a 4D volume" );

    }    
    if ( ( kernelSize.x % 2 == 0 ) ||
         ( kernelSize.y % 2 == 0 ) ||
         ( kernelSize.z % 2 == 0 ) )
    {

      throw std::runtime_error( "kernel size must be odd" );

    }
    gkg::Vector3d< int32_t > halfKernelSize( kernelSize );
    halfKernelSize /= 2;


    ////////////////////////////////////////////////////////////////////////////
    // reallocating output volume
    ////////////////////////////////////////////////////////////////////////////

    output.reallocate( inputSize, inputSizeT );
    output.getHeader() = input.getHeader();
    output.fill( background );


    ////////////////////////////////////////////////////////////////////////////
    // creating the index lut from absolute to masked voxel
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector absoluteToMaskedVoxelIndexLut( inputSizeXY * inputSize.z );
    int32_t absoluteIndex = 0;
    int32_t maskedIndex = 0;
    gkg::Vector3d< int32_t > voxel;

    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          if ( testFunction.getState( mask( voxel ) ) )
          {

            absoluteToMaskedVoxelIndexLut( absoluteIndex ) = maskedIndex;
            ++ maskedIndex;

          }
          else
          {

            absoluteToMaskedVoxelIndexLut( absoluteIndex ) = -1;

          }
          ++ absoluteIndex;
 
        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // exploring the mask and precomputing the index of neighbors with the
    // associated weights
    ////////////////////////////////////////////////////////////////////////////

    std::list< std::list< std::pair< int32_t, K > > >
      neighborIndicesAndBlurringWeights;
    gkg::Vector3d< int32_t > neighbor;
    gkg::Vector3d< int32_t > offset;
    int32_t voxelIndex = 0;
    int32_t neighborIndex = 0;
    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          if ( testFunction.getState( mask( voxel ) ) )
          {

            std::list< std::pair< int32_t, K > > 
              neighborIndicesAndBlurringWeightsForCurrentVoxel;

            for ( offset.z = -halfKernelSize.z; offset.z <= +halfKernelSize.z;
                  offset.z++ )
            {

              for ( offset.y = -halfKernelSize.y; offset.y <= +halfKernelSize.y;
                    offset.y++ )
              {

                for ( offset.x = -halfKernelSize.x;
                      offset.x <= +halfKernelSize.x;
                      offset.x++ )
                {

                  neighbor.x = voxel.x + offset.x;
                  neighbor.y = voxel.y + offset.y;
                  neighbor.z = voxel.z + offset.z;

                  if ( inputBoundingBox.contains( neighbor ) &&
                       testFunction.getState( mask( neighbor ) ) )
                  {

                    neighborIndex = neighbor.z * inputSizeXY +
                                    neighbor.y * inputSize.x + neighbor.x;
                    neighborIndicesAndBlurringWeightsForCurrentVoxel.push_back(
                      std::pair< int32_t, K >( 
                        absoluteToMaskedVoxelIndexLut( neighborIndex ),
                        kernel( halfKernelSize.x +
                                offset.x,
                                halfKernelSize.y +
                                offset.y,
                                halfKernelSize.z +
                                offset.z ) ) );

                  }

                }

              }

            }

            neighborIndicesAndBlurringWeights.push_back(
                             neighborIndicesAndBlurringWeightsForCurrentVoxel );

          }

          ++ voxelIndex;
 
        }

      }

    }
    int32_t voxelCount = ( int32_t )neighborIndicesAndBlurringWeights.size();


    ////////////////////////////////////////////////////////////////////////////
    // computing the blurring operator
    ////////////////////////////////////////////////////////////////////////////

    std::cout << std::endl << "voxelCount=" << voxelCount << std::endl;

    gkg::Matrix A( voxelCount, voxelCount );
    gkg::Matrix At( voxelCount, voxelCount );
    A.fill( 0.0 );
    At.fill( 0.0 );

    typename std::list< std::list< std::pair< int32_t, K > > >::const_iterator
      l = neighborIndicesAndBlurringWeights.begin();
    int32_t v = 0;
    for ( v = 0; v < voxelCount; v++ )
    {

      const std::list< std::pair< int32_t, K > >& 
        neighborIndicesAndBlurringWeightsForCurrentVoxel = *l;

      typename std::list< std::pair< int32_t, K > >::const_iterator
        n = neighborIndicesAndBlurringWeightsForCurrentVoxel.begin(),
        ne = neighborIndicesAndBlurringWeightsForCurrentVoxel.end();
      while ( n != ne )
      {

        A( v, n->first ) = ( double )n->second;
        At( n->first, v ) = ( double )n->second;
        ++ n;

      }

      ++ l;

    }


    //std::cout << "A=" << std::endl << A << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // computing the Lipschitz constant of the ( Ay - b )
    ////////////////////////////////////////////////////////////////////////////

    std::cout << "computing the Lipschitz constant L :" << std::endl;

    std::cout << " --> computing AtA " << std::endl;
    gkg::Matrix AtA = At.getComposition( A );
    //std::cout << AtA << std::endl;
    std::cout << " --> computing eigenvalues " << std::endl;


/*
    gkg:Vector eigenValues( voxelCount );
    _factory->getEigenValues(
                       AtA, eigenValues, 
                       gkg::NumericalAnalysisImplementationFactory::Ascending );
    gkg::MaximumFilter< gkg::Vector, double > maximumFilter;
    double lipschitzConstant = 0.0;
    maximumFilter.filter( eigenValues, lipschitzConstant );
    std::cout << "eigenValues=" << std::endl << eigenValues << std::endl;

    std::cout << "=> L = " << lipschitzConstant << std::endl;

    std::ostringstream buffer;
    buffer.precision( 10 );
    buffer.width( 30 );
    buffer << lipschitzConstant;

    std::cout << buffer.str() << std::endl;*/
    

    double lipschitzConstant = 0.07334405006;


    ////////////////////////////////////////////////////////////////////////////
    // computing the initial x vector
    ////////////////////////////////////////////////////////////////////////////

    std::cout << "computing the initial x vector : " << std::flush;

    gkg::Vector X0( voxelCount );

    voxelIndex = 0;
    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          if ( testFunction.getState( mask( voxel ) ) )
          {

            X0( voxelIndex ) = input( voxel );
            ++ voxelIndex;

          }

        }

      }

    }

    std::cout << "done" << std::endl;

    gkg::Vector Atb = At.getComposition( X0 );


    ////////////////////////////////////////////////////////////////////////////
    // iterating to refine solution
    ////////////////////////////////////////////////////////////////////////////

    std::cout << "iterating to refine solution : " << std::flush;

    gkg::Vector currentX = X0;
    gkg::Vector currentY = currentX;
    gkg::Vector previousX;
    gkg::Vector nextY;

    double currentT = 1.0;
    double nextT = 0.0;

    int32_t iteration = 1;
    double error = 1e38;
    while ( ( iteration <= maximumIterationCount ) &&
            ( error > tolerance ) )
    {

      std::cout << "main iteration " << iteration << std::endl;
      std::cout << "======================================================="
                << std::endl;
      previousX = currentX;

      std::cout << "running applyIterativeShrinkageOperator" << std::endl;
      //std::cout << "before=" << previousX << std::endl;
      this->applyIterativeShrinkageOperator( AtA,
                                             Atb,
                                             lipschitzConstant,
                                             currentY,
                                             currentX,
                                             shrinkageStep,
                                             skrinkageMaximumIterationCount,
                                             shrinkageTolerance );
      //std::cout << "after =" << currentX << std::endl;

      //std::cout << "computing nextT=" << std::flush;

      nextT = 0.5 * ( 1.0 + std::sqrt( 1.0 + 4.0 * currentT * currentT ) );

      //std::cout << nextT << std::endl;

      //std::cout << "computing nextY=" << std::flush;

      nextY = currentX + ( ( currentT - 1 ) / nextT ) * 
                           ( currentX - previousX );
      //std::cout << nextY << std::endl;

      error = std::sqrt( ( currentX - previousX ).getSumOfSquareValues() );

      std::cout << "computing error=" << std::flush;
      std::cout << error << " (tolerance=" << tolerance << ")" << std::endl;

      currentT = nextT;
      currentY = nextY;

      ++ iteration;

    }

    std::cout << "done" << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // filling output volume
    ////////////////////////////////////////////////////////////////////////////

    voxelIndex = 0;
    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          if ( testFunction.getState( mask( voxel ) ) )
          {

            output( voxel ) = ( T )currentX( voxelIndex );
            ++ voxelIndex;

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T, class M, class K > "
             "inline "
             "void "
             "gkg::FastIterativeShrinkageThresholdingAlgorithm< T, M, K >::"
             "solve( "
             "const gkg::Volume< T >& input, "
             "const gkg::Volume< M >& mask, "
             "const gkg::Volume< K >& kernel, "
             "const gkg::TestFunction< M >& testFunction, "
             "int32_t maximumIterationCount, "
             "double tolerance, "
             "double shrinkageStep, "
             "int32_t skrinkageMaximumIterationCount, "
             "double shrinkageTolerance, "
             "gkg::Volume< T >& output ) const" );

}


template < class T, class M, class K >
inline
void gkg::FastIterativeShrinkageThresholdingAlgorithm< T, M, K >::
        applyIterativeShrinkageOperator( const gkg::Matrix& AtA,
                                         const gkg::Vector& Atb,
                                         double lipschitzConstant,
                                         const gkg::Vector& currentY,
                                         gkg::Vector& currentX,
                                         double shrinkageStep,
                                         int32_t shrinkageMaximumIterationCount,
                                         double shrinkageTolerance ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing the L1 norm of current X vector
    ////////////////////////////////////////////////////////////////////////////

    //double normL1 = currentX.sumOfAbsoluteValues();


    ////////////////////////////////////////////////////////////////////////////
    // computing bPrime
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector bPrime = ( currentY - 
                           ( AtA.getComposition( currentY ) - Atb ) *
                           ( 1.0 / lipschitzConstant ) );


    ////////////////////////////////////////////////////////////////////////////
    // computing the size of the currentX vector
    ////////////////////////////////////////////////////////////////////////////

    int32_t size = currentX.getSize();


    ////////////////////////////////////////////////////////////////////////////
    // iterating to reach optimal currentX vector
    ////////////////////////////////////////////////////////////////////////////

    int32_t iteration = 0;
    double u = 0;
    gkg::Vector nextX( size );
    double error = 1e38;
    int32_t v = 0;
    double lambda = 2.0 / lipschitzConstant;
    while ( ( iteration < shrinkageMaximumIterationCount ) &&
            ( error > shrinkageTolerance ) )
    {

      for ( v = 0; v < size; v++ )
      {
 
        u = currentX( v ) * ( 1 - 2.0 * shrinkageStep ) + 
            bPrime( v ) * 2.0 * shrinkageStep;
        nextX( v ) = ( std::fabs( u ) - lambda * shrinkageStep ) +
                     this->sgn( u );

      }

      error = std::sqrt( ( nextX - currentX ).getSumOfSquareValues() );

      currentX = nextX;
      ++ iteration;

    }

    std::cout << "shrinkage iterationCount=" << iteration << std::endl;

  }
  GKG_CATCH( "void "
             "gkg::FastIterativeShrinkageThresholdingAlgorithm< T, M, K >::"
             "applyIterativeShrinkageOperator( const gkg::Matrix& AtA, "
             "const gkg::Vector& Atb, "
             "double lipschitzConstant, "
             "const gkg::Vector& currentY, "
             "gkg::Vector& currentX, "
             "double t, "
             "int32_t maximumIterationCount, "
             "double tolerance ) const" );

}


template < class T, class M, class K >
inline
double gkg::FastIterativeShrinkageThresholdingAlgorithm< T, M, K >::sgn(
                                                                double u ) const
{

  try
  {

    double result = 0.0;
    if ( u > 0.0 )
    {

      result = +1.0;

    }
    else if ( u < 0.0 )
    {

      result = -1.0;

    }
    return result;

  }
  GKG_CATCH( "double "
             "gkg::FastIterativeShrinkageThresholdingAlgorithm< T, M, K >::"
             "sgn( double u ) const" );


}


#endif
