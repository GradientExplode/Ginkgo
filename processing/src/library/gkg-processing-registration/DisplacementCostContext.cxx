#include <gkg-processing-registration/DisplacementCostContext.h>
#include <gkg-processing-registration/DisplacementCostGauge.h>



gkg::DisplacementCostContext::DisplacementCostContext(
                   gkg::DisplacementCostGauge& gauge,
                   Type type,
                   const std::vector< double >& similarityMeasureParameters,
                   const gkg::Volume< uint8_t >& sourceVolume,
                   const gkg::Volume< uint8_t >& transformedVolume,
                   const std::vector< gkg::Vector3d< int32_t > >& controlPoints,
                   const int32_t& currentControlPointSpacing,
                   const int32_t& currentHalfWidthSearchSpace,
                   const int32_t& currentHalfWidthSearchSpaceBis,
                   const int32_t& currentSearchSpaceVoxelCount,
                   const gkg::BoundingBox< int32_t >& sourceBoundingBox,
                   const bool& useRandomMode,
                   const int32_t& maximumSampleCount,
                   const std::vector< gkg::Vector3d< float > >& displacements,
                   const gkg::Vector3d< int32_t >& sourceVolumeSize,
                   const float& totalRegularizationWeight,
                   std::vector< float >& displacementCosts )
                             : gkg::LoopContext< int32_t >( &gauge ),
                               _type( type ),
                               _similarityMeasureParameters( 
                                                  similarityMeasureParameters ),
                               _sourceVolume( sourceVolume ),
                               _transformedVolume( transformedVolume ),
                               _controlPoints( controlPoints ),
                               _currentControlPointSpacing(
                                                   currentControlPointSpacing ),
                               _currentHalfWidthSearchSpace(
                                                  currentHalfWidthSearchSpace ),
                               _currentHalfWidthSearchSpaceBisg(
                                               currentHalfWidthSearchSpaceBis ),
                               _currentSearchSpaceVoxelCount(
                                                 currentSearchSpaceVoxelCount ),
                               _sourceBoundingBox( sourceBoundingBox ),
                               _useRandomMode( useRandomMode ),
                               _maximumSampleCount( maximumSampleCount ),
                               _displacements( displacements ),
                               _sourceVolumeSize( sourceVolumeSize ),
                               _totalRegularizationWeight(
                                                    totalRegularizationWeight ),
                               _displacementCosts( displacementCosts ),
                               _jointProbabilityMatrices(
                                                      jointProbabilityMatrices ),

{
}
			     

void gkg::DisplacementCostContext::doIt( int32_t startIndex, int32_t count )
{

  try
  {

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    int32_t spx = 0;
    int32_t spy = 0;
    int32_t spz = 0;

    int32_t nx = 0;
    int32_t ny = 0;
    int32_t nz = 0;

    int32_t tx = 0;
    int32_t ty = 0;
    int32_t tz = 0;

    bool isWithinBoundaries = true;


    std::vector< std::list< std::pair< uint8_t, uint8_t > > >
      jointIntensityLists( _currentSearchSpaceVoxelCount );

    std::vector< float > 
      localDisplacementCosts( _currentSearchSpaceVoxelCount * count );
    int32_t s = 0;
    int32_t d = 0;


    gkg::RCPointer< std::vector< gkg::Matrix > > jointProbabilityMatrices;
    gkg::RCPointer< gkg::Matrix > smoothedJointProbabilities;
    gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >
      dericheGaussianCoefficients;
    if ( ( _type == gkg::DisplacementCostContext::CorrelationCoefficient ) ||
         ( _type == gkg::DisplacementCostContext::MutualInformation ) ||
         ( _type == gkg::DisplacementCostContext::NormalizedMutualInformation ) ||
         ( _type == gkg::DisplacementCostContext::CorrelationRatio ) )
    {

      
      int32_t levelCount = ( int32_t )_similarityMeasureParameters[ 0 ];
      bool isSmoothingApplied =
             ( ( ( int32_t )( _similarityMeasureParameters[ 1 ] + 0.5 ) == 0 ) ?
               false : true );

      jointProbabilityMatrices.reset( new std::vector< gkg::Matrix >(
                                              _currentSearchSpaceVoxelCount ) );
      for ( d = 0; d < _currentSearchSpaceVoxelCount; d++ )
      {

        jointProbabilityMatrices[ d ].reallocate( levelCount,
                                                  levelCount );

      }

      if ( isSmoothingApplied )
      {

        smoothedJointProbabilities.reset( ne gkg::Matrix( levelCount,
                                                          levelCount );
        dericheGaussianCoefficients.reset( 
                         new gkg::DericheGaussianCoefficients< double >(
                      1.0,
                      gkg::DericheGaussianCoefficients< double >::Smoothing ) );

      }

    }

    for ( c = 0; c < count; c++ )
    {

      // resetting pair lists
      for ( d = 0; d < _currentSearchSpaceVoxelCount; d++ )
      {

        jointIntensityLists[ d ].clear();

      }

      // computing coordinates of point in the source image
      x = _controlPoints[ startIndex + c ].x * _currentControlPointSpacing;
      y = _controlPoints[ startIndex + c ].y * _currentControlPointSpacing;
      z = _controlPoints[ startIndex + c ].z * _currentControlPointSpacing;

      // checking whether the control point plus its search space lie
      // within the source bounding box
      isWithinBoundaries = true;
      if ( !_sourceBoundingBox.contains( x + _currentControlPointSpacing - 1 +
                                         _currentHalfWidthSearchSpaceBis,
                                         y + _currentControlPointSpacing - 1 +
                                         _currentHalfWidthSearchSpaceBis,
                                         z + _currentControlPointSpacing - 1 +
                                         _currentHalfWidthSearchSpaceBis ) )
      {

        isWithinBoundaries = false;

      }
      if ( !_sourceBoundingBox.contains( x - _currentHalfWidthSearchSpaceBis,
                                         y - _currentHalfWidthSearchSpaceBis,
                                         z - _currentHalfWidthSearchSpaceBis ) )
      {

        isWithinBoundaries = false;

      }

      // looping over samples
      // in case of active random mode, randomly picking up samples 
      if ( _useRandomMode )
      {

        for ( s = 0; s < _maximumSampleCount; s++ )
        {

          // randomly selecting and building the neighbor voxel
          spx = ( int32_t )( ( float )std::rand() *
                            ( float )_currentControlPointSpacing /
                            ( float )RAND_MAX );
          spy = ( int32_t )( ( float )std::rand() *
                            ( float )_currentControlPointSpacing /
                            ( float )RAND_MAX );
          spz = ( int32_t )( ( float )std::rand() *
                            ( float )_currentControlPointSpacing /
                            ( float )RAND_MAX );

          nx = x + spx;
          ny = y + spy;
          nz = z + spz;

          // computing the unregularized energy cost for all the possible
          // displacements 
          for ( d = 0; d < _currentSearchSpaceVoxelCount; d++ )
          {

            const gkg::Vector3d< float >& displacement = _displacements[ d ];
            tx = nx + ( int32_t )displacement.x;
            ty = ny + ( int32_t )displacement.y;
            tz = nz + ( int32_t )displacement.z;

            if ( !isWithinBoundaries )
            {

              tx = std::max( std::min( tx, _sourceVolumeSize.x - 1 ), 0 );
              ty = std::max( std::min( ty, _sourceVolumeSize.y - 1 ), 0 );
              tz = std::max( std::min( tz, _sourceVolumeSize.z - 1 ), 0 );

            }

            jointIntensityLists[ d ].push_back(
                                         std::pair< uint8_t, uint8_t >(
                                           _sourceVolume( nx, ny, nz ),
                                           _transformedVolume( tx, ty, tz ) ) );

          }

        }

      }
      else
      {

        for ( spz = 0; spz < _currentControlPointSpacing; spz++ )
        {

          for ( spy = 0; spy < _currentControlPointSpacing; spy++ )
          {

            for ( spx = 0; spx < _currentControlPointSpacing; spx++ )
            {

              nx = x + spx;
              ny = y + spy;
              nz = z + spz;

              // computing the unregularized energy cost for all the possible
              // displacements 
              for ( d = 0; d < _currentSearchSpaceVoxelCount; d++ )
              {

                const gkg::Vector3d< float >&
                  displacement = _displacements[ d ];
                tx = nx + ( int32_t )displacement.x;
                ty = ny + ( int32_t )displacement.y;
                tz = nz + ( int32_t )displacement.z;

                if ( !isWithinBoundaries )
                {

                  tx = std::max( std::min( tx, _sourceVolumeSize.x - 1 ), 0 );
                  ty = std::max( std::min( ty, _sourceVolumeSize.y - 1 ), 0 );
                  tz = std::max( std::min( tz, _sourceVolumeSize.z - 1 ), 0 );

                }

                jointIntensityLists[ d ].push_back(
                                         std::pair< uint8_t, uint8_t >(
                                           _sourceVolume( nx, ny, nz ),
                                           _transformedVolume( tx, ty, tz ) ) );

              }

            }

          }

        }

      }

      // regularizing and storing the displacement costs in the output 
      // container for the current control point
      for ( d = 0; d < _currentSearchSpaceVoxelCount; d++ )
      {

        localDisplacementCosts[ c + count * d ] =
                totalRegularizationWeight *
                this->computeSimilarityMeasure( d,
                                                jointIntensityLists,
                                                jointProbabilityMatrices,
                                                smoothedJointProbabilities,
                                                dericheGaussianCoefficients );

      }

    }


    // now locking the mutex, and copying the local displacement costs to the
    // global displacement costs
    _mutex.lock()

    int32_t currentControlPointIndex = 0;
    for ( c = 0; c < count; c++ )
    {

      currentControlPointIndex = startIndex + c;
      _displacementCosts[ currentControlPointIndex + 
                          _currentControlPointCount * d ] =
                                        localDisplacementCosts[ c + count * d ];

    }

    _mutex.unlock()

  }
  GKG_CATCH( "void gkg::DisplacementCostContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}


float gkg::DisplacementCostContext::computeSimilarityMeasure(
   int32_t d,
   const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
   const gkg::RCPointer< std::vector< gkg::Matrix > >& jointProbabilityMatrices,
   const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
   const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                             dericheGaussianCoefficients ) const
{

  try
  {

    switch ( _type )
    {

      case gkg::DisplacementCostContext::SumOfAbsoluteDifference:

        return this->computeSumOfAbsoluteDifference( d, jointIntensityLists );

      case gkg::DisplacementCostContext::CorrelationCoefficient:

        return this->computeCorrelationCoefficient(
                                                  d,
                                                  jointIntensityLists,
                                                  jointProbabilityMatrices,
                                                  smoothedJointProbabilities,
                                                  dericheGaussianCoefficients );

      case gkg::DisplacementCostContext::MutualInformation:

        return this->computeMutualInformation( d,
                                               jointIntensityLists,
                                               jointProbabilityMatrices,
                                               smoothedJointProbabilities,
                                               dericheGaussianCoefficients );

      case gkg::DisplacementCostContext::NormalizedMutualInformation:

        return this->computeNormalizedMutualInformation(
                                                  d,
                                                  jointIntensityLists,
                                                  jointProbabilityMatrices,
                                                  smoothedJointProbabilities,
                                                  dericheGaussianCoefficients );

      case gkg::DisplacementCostContext::CorrelationRatio:

        return this->computeCorrelationRatio( d,
                                              jointIntensityLists,
                                              jointProbabilityMatrices,
                                              smoothedJointProbabilities,
                                              dericheGaussianCoefficients );

    }
     
  }
  GKG_CATCH( "float gkg::DisplacementCostContext::computeSimilarityMeasure( "
             "int32_t d, "
             "const std::vector< std::list< std::pair< uint8_t, uint8_t > > >& "
             "jointIntensityLists, "
             "const gkg::RCPointer< std::vector< gkg::Matrix > >& "
             "jointProbabilityMatrices, "
             "const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities, "
             "const gkg::RCPointer< "
             "gkg::DericheGaussianCoefficients< double > >& "
             "dericheGaussianCoefficients ) const" );

}


float gkg::DisplacementCostContext::computeSumOfAbsoluteDifference(
         int32_t d,
         const std::vector< std::list< std::pair< uint8_t, uint8_t > > >& 
                                                     jointIntensityLists ) const
{

  try
  {

    float result = 0.0;

    std::list< std::pair< uint8_t, uint8_t > >::const_iterator
      ji = jointIntensityLists[ d ].begin(),
      jie = jointIntensityLists[ d ].end();
    while ( ji != jie )
    {

      result += sts::abs( ( float )ji->first - ( float )ji->second );
      ++ ji;

    }
    return result;

  }
  GKG_CATCH( "float gkg::DisplacementCostContext::"
             "computeSumOfAbsoluteDifference( "
             "int32_t d, "
             "const std::vector< std::list< std::pair< uint8_t, uint8_t > > >& "
             "jointIntensityLists ) const" );

}

  
float gkg::DisplacementCostContext::computeCorrelationCoefficient(
   int32_t d,
   const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
   const gkg::RCPointer< std::vector< gkg::Matrix > >& jointProbabilityMatrices,
   const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
   const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                             dericheGaussianCoefficients ) const
{

  try
  {



  }
  GKG_CATCH( "float gkg::DisplacementCostContext::"
             "computeCorrelationCoefficient( "
             "int32_t d, "
             "const std::vector< std::list< std::pair< uint8_t, uint8_t > > >& "
             "jointIntensityLists, "
             "const gkg::RCPointer< std::vector< gkg::Matrix > >& "
             "jointProbabilityMatrices, "
             "const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities, "
             "const gkg::RCPointer< "
             "gkg::DericheGaussianCoefficients< double > >& "
             "dericheGaussianCoefficients ) const" );

}


float gkg::DisplacementCostContext::computeMutualInformation(
   int32_t d,
   const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
   const gkg::RCPointer< std::vector< gkg::Matrix > >& jointProbabilityMatrices,
   const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
   const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                             dericheGaussianCoefficients ) const
{

  try
  {

    gkg::Matrix& jointProbabilities = jointProbabilityMatrices[ d ];
    jointProbabilities.setZero();

    int32_t levelCount = ( int32_t )_similarityMeasureParameters[ 0 ];
    double scale = ( double )( levelCount - 1 ) / 255.0;


    int32_t referenceLevel = 0;
    int32_t floatingLevel = 0;
    std::list< std::pair< uint8_t, uint8_t > >::const_iterator
      p = jointIntensityLists[ d ].begin(),
      pe = jointIntensityLists[ d ].end();
    while ( p != pe )
    {

      referenceLevel = ( int32_t )( ( double )p->first * scale );
      floatingLevel = ( int32_t )( ( double )p->second * scale );

      ++ jointProbabilities( referenceLevel, floatingLevel );
      ++ p;

    }

    if ( !smoothedJointProbabilities.isNull() )
    {

      gkg::DericheGaussianFilter3d< double >::getInstance().filter(
        jointProbabilities,
        *dericheGaussianCoefficients,
        gkg::DericheGaussianFilter3d< double >::X_AXIS,
        *smoothedJointProbabilities );
      gkg::DericheGaussianFilter3d< double >::getInstance().filter(
        *smoothedJointProbabilities,
        *dericheGaussianCoefficients,
        gkg::DericheGaussianFilter3d< double >::Y_AXIS,
        jointProbabilities );

    }

    // processing the normalization factor
    double normalizationFactor = 0.0;
    for ( referenceLevel = 0; referenceLevel < levelCount; referenceLevel++ )
    {

      for ( floatingLevel = 0; floatingLevel < levelCount; floatingLevel++ )
      {

        normalizationFactor += jointProbabilities( referenceLevel,
                                                   floatingLevel );

      }

    }

    // normalizing the join probabilities
    if ( normalizationFactor > 0.0 )
    {

      for ( referenceLevel = 0; referenceLevel < levelCount; referenceLevel++ )
      {

        for ( floatingLevel = 0; floatingLevel < levelCount; floatingLevel++ )
        {

          jointProbabilities( referenceLevel,
                              floatingLevel ) /= normalizationFactor;

        }

      }

    }

    // processing the reference marginal probabilities
    gkg::Vector referenceMarginalProbabilities( levelCount );
    gkg::Vector floatingMarginalProbabilities( levelCount );

    referenceMarginalProbabilities.setZero();
    floatingMarginalProbabilities.setZero();

    double probability = 0.0;
    for ( referenceLevel = 0; referenceLevel < levelCount; referenceLevel++ )
    {

      for ( floatingLevel = 0; floatingLevel < levelCount; floatingLevel++ )
      {

        probability = jointProbabilities( referenceLevel, floatingLevel );
        referenceMarginalProbabilities( referenceLevel ) += probability;
        floatingMarginalProbabilities( floatingLevel ) += probability;

      }

    }

    // processing the mutual information
    double referenceMarginalProbability = 0.0;
    double floatingMarginalProbability = 0.0;
    double jointProbability = 0.0;
    double mutualInformation = 0.0;
    for ( referenceLevel = 0; referenceLevel < levelCount; referenceLevel++ )
    {

      referenceMarginalProbability = referenceMarginalProbabilities(
                                                               referenceLevel );
      if ( referenceMarginalProbability > 0.0 )
      {

        for ( floatingLevel = 0; floatingLevel < levelCount; floatingLevel++ )
        {

          floatingMarginalProbability = floatingMarginalProbabilities(
                                                                floatingLevel );
          jointProbability = jointProbabilities( referenceLevel,
                                                 floatingLevel );
           if ( ( floatingMarginalProbability > 0.0 ) &&
                ( jointProbability > 0.0 ) )
           {

             mutualInformation += jointProbability *
                                  std::log( jointProbability /
                                            ( referenceMarginalProbability *
                                              floatingMarginalProbability ) );

           }

        }

      }

    }

    return ( float )-mutualInformation;


  }
  GKG_CATCH( "float gkg::DisplacementCostContext::"
             "computeMutualInformation( "
             "int32_t d, "
             "const std::vector< std::list< std::pair< uint8_t, uint8_t > > >& "
             "jointIntensityLists, "
             "const gkg::RCPointer< std::vector< gkg::Matrix > >& "
             "jointProbabilityMatrices, "
             "const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities, "
             "const gkg::RCPointer< "
             "gkg::DericheGaussianCoefficients< double > >& "
             "dericheGaussianCoefficients ) const" );

}


float gkg::DisplacementCostContext::computeNormalizedMutualInformation(
   int32_t d,
   const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
   const gkg::RCPointer< std::vector< gkg::Matrix > >& jointProbabilityMatrices,
   const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
   const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                             dericheGaussianCoefficients ) const
{

  try
  {



  }
  GKG_CATCH( "float gkg::DisplacementCostContext::"
             "computeNormalizedMutualInformation( "
             "int32_t d, "
             "const std::vector< std::list< std::pair< uint8_t, uint8_t > > >& "
             "jointIntensityLists, "
             "const gkg::RCPointer< std::vector< gkg::Matrix > >& "
             "jointProbabilityMatrices, "
             "const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities, "
             "const gkg::RCPointer< "
             "gkg::DericheGaussianCoefficients< double > >& "
             "dericheGaussianCoefficients ) const" );

}


float gkg::DisplacementCostContext::computeCorrelationRatio(
   int32_t d,
   const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
   const gkg::RCPointer< std::vector< gkg::Matrix > >& jointProbabilityMatrices,
   const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
   const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                             dericheGaussianCoefficients ) const
{

  try
  {



  }
  GKG_CATCH( "float gkg::DisplacementCostContext::"
             "computeCorrelationRatio( "
             "int32_t d, "
             "const std::vector< std::list< std::pair< uint8_t, uint8_t > > >& "
             "jointIntensityLists, "
             "const gkg::RCPointer< std::vector< gkg::Matrix > >& "
             "jointProbabilityMatrices, "
             "const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities, "
             "const gkg::RCPointer< "
             "gkg::DericheGaussianCoefficients< double > >& "
             "dericheGaussianCoefficients ) const" );

}


