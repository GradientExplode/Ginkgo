#ifndef _gkg_processing_registration_MutualInformationSimilarityMeasure_i_h_
#define _gkg_processing_registration_MutualInformationSimilarityMeasure_i_h_


#include <gkg-processing-registration/MutualInformationSimilarityMeasure.h>
#include <gkg-processing-registration/SimilarityMeasure_i.h>
#include <gkg-processing-signal/DericheGaussianFilter3d.h>
#include <gkg-core-io/Writer_i.h>


#define GKG_MUTUAL_INFO_SMOOTHING_SIGMA  1.0


template < class T1, class T2 >
inline
gkg::MutualInformationSimilarityMeasure< T1, T2 >::
                                             MutualInformationSimilarityMeasure(
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

      _dericheGaussianCoefficients = 
        new gkg::DericheGaussianCoefficients< double >(
              GKG_MUTUAL_INFO_SMOOTHING_SIGMA,
              gkg::DericheGaussianCoefficients< double >::Smoothing );
      _smoothedJoinProbabilities = new gkg::Matrix( _levelCount, _levelCount );

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "gkg::MutualInformationSimilarityMeasure< T1, T2 >:: "
             "MutualInformationSimilarityMeasure( "
             "gkg::RCPointer< gkg::Volume< T1 > > reference, "
             "gkg::RCPointer< gkg::Volume< T2 > > floating, "
             "int32_t resamplingOrder, "
             "int32_t subSamplingMaximumSize, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class T1, class T2 >
inline
gkg::MutualInformationSimilarityMeasure< T1, T2 >::
                                           ~MutualInformationSimilarityMeasure()
{

  if ( _isSmoothingApplied )
  {

    delete _smoothedJoinProbabilities;
    delete _dericheGaussianCoefficients;

  }

}


template < class T1, class T2 >
inline
int32_t gkg::MutualInformationSimilarityMeasure< T1, T2 >::getLevelCount() const
{

  try
  {

    return _levelCount;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "int32_t gkg::MutualInformationSimilarityMeasure< T1, T2 >::"
             "getLevelCount() const" );

}


template < class T1, class T2 >
inline
bool 
gkg::MutualInformationSimilarityMeasure< T1, T2 >::isSmoothingApplied() const
{

  try
  {

    return _isSmoothingApplied;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "bool gkg::MutualInformationSimilarityMeasure< T1, T2 >::"
             "isSmoothingApplied() const" );

}


template < class T1, class T2 >
inline
double gkg::MutualInformationSimilarityMeasure< T1, T2 >::getValueAt(
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
    gkg::Volume< uint8_t >::const_iterator
      r = this->_resampledReferenceU8->begin(),
      re = this->_resampledReferenceU8->end();
    gkg::Volume< uint8_t >::const_iterator
      f = this->_transformedFloatingU8->begin();
    int32_t referenceLevel;
    int32_t floatingLevel;
    double levelCount = ( double )_levelCount;
    while ( r != re )
    {

      if ( ( *r > 0U ) && ( *f > 0U ) )
      {

        referenceLevel = ( int32_t )( ( ( double )*r / 255.0 ) *
                                      ( levelCount - 1 ) );
        floatingLevel = ( int32_t )( ( ( double )*f / 255.0 ) *
                                   ( levelCount - 1 ) );
        ++ _joinProbabilities( referenceLevel, floatingLevel );

      }
      ++ r;
      ++ f;

    }

    // smoothing the join probabilities
    if ( _isSmoothingApplied )
    {

      gkg::DericheGaussianFilter3d< double >::getInstance().filter(
        _joinProbabilities,
        *_dericheGaussianCoefficients,
        gkg::DericheGaussianFilter3d< double >::X_AXIS,
        *_smoothedJoinProbabilities );
      gkg::DericheGaussianFilter3d< double >::getInstance().filter(
        *_smoothedJoinProbabilities,
        *_dericheGaussianCoefficients,
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

    // processing the mutual information
    double referenceMarginalProbability = 0.0;
    double floatingMarginalProbability = 0.0;
    double joinProbability = 0.0;
    double mutualInformation = 0.0;
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

             mutualInformation += joinProbability *
                                  std::log( joinProbability /
                                            ( referenceMarginalProbability *
                                              floatingMarginalProbability ) );

           }

        }

      }

    }

    return -mutualInformation;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "double gkg::MutualInformationSimilarityMeasure< T1, T2 >::"
             "getValueAt( "
             "const gkg::Transform3d< float >& transform3d ) const" );

}


template < class T1, class T2 >
inline
void gkg::MutualInformationSimilarityMeasure< T1, T2 >::checkScalarParameters(
                                       std::vector< double >& scalarParameters )
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
                             "invalid mutual information scalar parameters" );

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
             "void gkg::MutualInformationSimilarityMeasure< T1, T2 >::"
             "checkScalarParameters( std::vector< double >& scalarParameters )" );

}


#endif
