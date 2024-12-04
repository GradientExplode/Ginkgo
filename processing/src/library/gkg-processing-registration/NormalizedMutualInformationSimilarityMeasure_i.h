#ifndef _gkg_processing_registration_NormalizedMutualInformationSimilarityMeasure_i_h_
#define _gkg_processing_registration_NormalizedMutualInformationSimilarityMeasure_i_h_


#include <gkg-processing-registration/NormalizedMutualInformationSimilarityMeasure.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/Writer_i.h>


#define GKG_MUTUAL_INFO_SMOOTHING_SIGMA  1.0


template < class T1, class T2 >
inline
gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::
                                   NormalizedMutualInformationSimilarityMeasure(
           gkg::RCPointer< gkg::Volume< T1 > > reference,
           gkg::RCPointer< gkg::Volume< T2 > > floating,
           const T1& referenceLowerThreshold,
           const T2& floatingLowerThreshold,
           int32_t resamplingOrder,
           int32_t subSamplingMaximumSize,
           const std::vector< double >& scalarParameters,
           bool verbose )
                               : gkg::SimilarityMeasure< T1, T2 >(
                                   reference,
                                   floating,
                                   referenceLowerThreshold,
                                   floatingLowerThreshold,
                                   resamplingOrder,
                                   subSamplingMaximumSize,
                                   verbose )
                                 
{

  try
  {

    // in order to use the Gaussian normal distribution with the GSL 
      _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();


    std::vector< double > copyOfScalarParameters( scalarParameters );
    checkScalarParameters( copyOfScalarParameters );
    _levelCount = ( int32_t )copyOfScalarParameters[ 0 ];
    _isSmoothingApplied =
                   ( ( ( int32_t )( copyOfScalarParameters[ 1 ] + 0.5 ) == 0 ) ?
                     false : true );

    _joinProbabilities.reallocate( _levelCount, _levelCount );
    _referenceMarginalProbabilities.reallocate( _levelCount );
    _floatingMarginalProbabilities.reallocate( _levelCount );

    if ( _isSmoothingApplied )
    {

      _dericheGaussianSmoothingCoefficients = 
        new gkg::DericheGaussianCoefficients< double >(
              GKG_MUTUAL_INFO_SMOOTHING_SIGMA,
              gkg::DericheGaussianCoefficients< double >::Smoothing );
      _smoothedJoinProbabilities = new gkg::Matrix( _levelCount, _levelCount );

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >:: "
             "NormalizedMutualInformationSimilarityMeasure( "
             "gkg::RCPointer< gkg::Volume< T1 > > reference, "
             "gkg::RCPointer< gkg::Volume< T2 > > floating, "
             "int32_t resamplingOrder, "
             "int32_t subSamplingMaximumSize, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class T1, class T2 >
inline
gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::
                                 ~NormalizedMutualInformationSimilarityMeasure()
{

  if ( _isSmoothingApplied )
  {

    delete _smoothedJoinProbabilities;
    delete _dericheGaussianSmoothingCoefficients;

  }

}


template < class T1, class T2 >
inline
int32_t gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::
                                                           getLevelCount() const
{

  try
  {

    return _levelCount;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "int32_t "
             "gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::"
             "getLevelCount() const" );

}


template < class T1, class T2 >
inline
bool gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::
                                                      isSmoothingApplied() const
{

  try
  {

    return _isSmoothingApplied;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "bool "
             "gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::"
             "isSmoothingApplied() const" );

}


template < class T1, class T2 >
inline
double gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::getValueAt(
                            const gkg::Transform3d< float >& transform3d ) const
{

  try
  {

    // resampling and transforming accordingly the floating volume
    this->_resampler->resample( *this->_resampledFloatingU8,
                                transform3d,
                                0U,
                                *this->_transformedFloatingU8,
                                false );

    // processing raw join probabilities
    _joinProbabilities.setZero();
    int32_t referenceLevel;
    int32_t floatingLevel;
    double fact = double( _levelCount - 1 ) / 255.0;
    int32_t x, sizeX = this->_resampledReferenceU8->getSizeX();
    int32_t y, sizeY = this->_resampledReferenceU8->getSizeY();
    int32_t z, sizeZ = this->_resampledReferenceU8->getSizeZ();
    for ( z = 0; z < sizeZ; z++ )
    {
    
      for ( y = 0; y < sizeY; y++ )
      {
      
        for ( x = 0; x < sizeX; x++ )
        {

          uint8_t r = (*this->_resampledReferenceU8)( x, y, z );
	  uint8_t f = (*this->_transformedFloatingU8)( x, y, z );

	  if ( r && f )
          {

            referenceLevel = ( int32_t )( ( double )r * fact );
            floatingLevel = ( int32_t )( ( double )f * fact );
            ++ _joinProbabilities( referenceLevel, floatingLevel );

          }
	  
	}
	
      }

    }

    // smoothing the join probabilities
    if ( _isSmoothingApplied )
    {

      gkg::DericheGaussianFilter3d< double >::getInstance().filter(
        _joinProbabilities,
        *_dericheGaussianSmoothingCoefficients,
        gkg::DericheGaussianFilter3d< double >::X_AXIS,
        *_smoothedJoinProbabilities );
      gkg::DericheGaussianFilter3d< double >::getInstance().filter(
        *_smoothedJoinProbabilities,
        *_dericheGaussianSmoothingCoefficients,
        gkg::DericheGaussianFilter3d< double >::Y_AXIS,
        _joinProbabilities );

    }

    // processing the normalization factor
    double normalizationFactor = 0.0;
    for ( referenceLevel = 0; referenceLevel < _levelCount; referenceLevel++ )
    {

      for ( floatingLevel = 0; floatingLevel < _levelCount; floatingLevel++ )
      {

        normalizationFactor += _joinProbabilities( referenceLevel,
                                                   floatingLevel );

      }

    }

    // normalizing the join probabilities
    if ( normalizationFactor > 0.0 )
    {

      for ( referenceLevel = 0; referenceLevel < _levelCount; referenceLevel++ )
      {

        for ( floatingLevel = 0; floatingLevel < _levelCount; floatingLevel++ )
        {

          _joinProbabilities( referenceLevel,
                              floatingLevel ) /= normalizationFactor;

        }

      }

    }

    // processing the reference marginal probabilities
    _referenceMarginalProbabilities.setZero();
    _floatingMarginalProbabilities.setZero();
    double probability = 0.0;
    for ( referenceLevel = 0; referenceLevel < _levelCount; referenceLevel++ )
    {

      for ( floatingLevel = 0; floatingLevel < _levelCount; floatingLevel++ )
      {

        probability = _joinProbabilities( referenceLevel, floatingLevel );
        _referenceMarginalProbabilities( referenceLevel ) += probability;
        _floatingMarginalProbabilities( floatingLevel ) += probability;

      }

    }

    // processing the normalized mutual information
    double referenceMarginalProbability = 0.0;
    double floatingMarginalProbability = 0.0;
    double joinProbability = 0.0;
    double normalizedMutualInformationNumerator = 0.0;
    double normalizedMutualInformationDenominator = 0.0;
    double normalizedMutualInformation = 0.0;
    for ( referenceLevel = 0; referenceLevel < _levelCount; referenceLevel++ )
    {

      referenceMarginalProbability = _referenceMarginalProbabilities(
                                                               referenceLevel );
      if ( referenceMarginalProbability > 0.0 )
      {

        for ( floatingLevel = 0; floatingLevel < _levelCount; floatingLevel++ )
        {

          floatingMarginalProbability = _floatingMarginalProbabilities(
                                                                floatingLevel );
          joinProbability = _joinProbabilities( referenceLevel, floatingLevel );
           if ( ( floatingMarginalProbability > 0.0 ) &&
                ( joinProbability > 0.0 ) )
           {

             normalizedMutualInformationNumerator -=
                                      referenceMarginalProbability *
                                      std::log( referenceMarginalProbability ) +
                                      floatingMarginalProbability *
                                      std::log( floatingMarginalProbability );
             normalizedMutualInformationDenominator -=
                                      joinProbability *
                                      std::log( joinProbability );

           }

        }

      }

    }

    if ( normalizedMutualInformationDenominator > 0.0 )
    {

      normalizedMutualInformation = normalizedMutualInformationNumerator /
                                    normalizedMutualInformationDenominator;

    }

    return - normalizedMutualInformation;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "double "
             "gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::"
             "getValueAt( "
             "const gkg::Transform3d< float >& transform3d ) const" );

}


template < class T1, class T2 >
inline
void gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::
                checkScalarParameters( std::vector< double >& scalarParameters )
{

  try
  {

    if ( scalarParameters.empty() )
    {

      scalarParameters.resize( 2U );
      scalarParameters[ 0 ] = 32;
      scalarParameters[ 1 ] = 1;

    }
    else
    {

      if ( scalarParameters.size() != 2U )
      {

        throw std::runtime_error(
                   "invalid normalized mutual information scalar parameters" );

      }
      if ( ( scalarParameters[ 0 ] < 16 ) ||
           ( scalarParameters[ 0 ] > 256 ) )
      {

        throw std::runtime_error(
                                 "level count should be between 16 and 256" );

      }
      if ( ( scalarParameters[ 1 ] < 0 ) ||
           ( scalarParameters[ 1 ] > 1 ) )
      {

        throw std::runtime_error( "apply smoothing should be 0 or 1" );

      }

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void "
             "gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::"
             "checkScalarParameters( "
             "std::vector< double >& scalarParameters )" );

}


#endif
