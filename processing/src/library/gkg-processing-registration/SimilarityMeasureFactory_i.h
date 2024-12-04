#ifndef _gkg_processing_registration_SimilarityMeasureFactory_i_h_
#define _gkg_processing_registration_SimilarityMeasureFactory_i_h_


#include <gkg-processing-registration/SimilarityMeasureFactory.h>
#include <gkg-processing-registration/CorrelationCoefficientSimilarityMeasure_i.h>
#include <gkg-processing-registration/MutualInformationSimilarityMeasure_i.h>
#include <gkg-processing-registration/NormalizedMutualInformationSimilarityMeasure_i.h>
#include <gkg-processing-registration/CorrelationRatioSimilarityMeasure_i.h>
#include <gkg-core-exception/Exception.h>


template < class T1, class T2 >
inline
gkg::SimilarityMeasureFactory< T1, T2 >::SimilarityMeasureFactory()
{

  try
  {

    _similarityMeasureNames.insert( "correlation-coefficient" );
    _similarityMeasureNames.insert( "mutual-information" );
    _similarityMeasureNames.insert( "normalized-mutual-information" );
    _similarityMeasureNames.insert( "correlation-ratio" );

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "gkg::SimilarityMeasureFactory< T1, T2 >::"
             "SimilarityMeasureFactory()" );

}


template < class T1, class T2 >
inline
gkg::SimilarityMeasureFactory< T1, T2 >::~SimilarityMeasureFactory()
{

}


template < class T1, class T2 >
inline
const std::set< std::string >&
gkg::SimilarityMeasureFactory< T1, T2 >::getSimilarityMeasureNames() const
{

  try
  {

    return _similarityMeasureNames;

  }
  GKG_CATCH( "template < class T1, class T2 > " 
             "inline "
             "const std::set< std::string >& "
             "gkg::SimilarityMeasureFactory< T1, T2 >::"
             "getSimilarityMeasureNames() const" );

}


template < class T1, class T2 >
inline
gkg::RCPointer< gkg::SimilarityMeasure< T1, T2 > >
gkg::SimilarityMeasureFactory< T1, T2 >::create(
           const std::string& similarityMeasureName,
           gkg::RCPointer< gkg::Volume< T1 > > reference,
           gkg::RCPointer< gkg::Volume< T2 > > floating,
           const T1& referenceLowerThreshold,
           const T2& floatingLowerThreshold,
           int32_t resamplingOrder,
           int32_t subSamplingMaximumSize,
           const std::vector< double >& scalarParameters,
           bool verbose ) const
{

  try
  {

    if ( ( resamplingOrder < 0 ) || ( resamplingOrder > 7 ) )
    {

      throw std::runtime_error( "resampling order must be between 0 and 7" );

    }
    if ( ( subSamplingMaximumSize < 32 ) ||
         ( subSamplingMaximumSize > 4096 ) )
    {

      throw std::runtime_error(
                      "sub-sampling maximum size must be between 32 and 4096" );

    }

    gkg::RCPointer< gkg::SimilarityMeasure< T1, T2 > > similarityMeasure;
    if ( similarityMeasureName == "correlation-coefficient" )
    {

      similarityMeasure.reset(
                   new gkg::CorrelationCoefficientSimilarityMeasure< T1, T2 >(
                                reference,
                                floating,
                                referenceLowerThreshold,
                                floatingLowerThreshold,
                                resamplingOrder,
                                subSamplingMaximumSize,
                                scalarParameters,
                                verbose ) );

    }
    else if ( similarityMeasureName == "mutual-information" )
    {

      similarityMeasure.reset(
                          new gkg::MutualInformationSimilarityMeasure< T1, T2 >(
                                reference,
                                floating,
                                referenceLowerThreshold,
                                floatingLowerThreshold,
                                resamplingOrder,
                                subSamplingMaximumSize,
                                scalarParameters,
                                verbose ) );

    }
    else if ( similarityMeasureName == "normalized-mutual-information" )
    {

      similarityMeasure.reset(
                new gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >(
                                reference,
                                floating,
                                referenceLowerThreshold,
                                floatingLowerThreshold,
                                resamplingOrder,
                                subSamplingMaximumSize,
                                scalarParameters,
                                verbose ) );

    }
    else if ( similarityMeasureName == "correlation-ratio" )
    {

      similarityMeasure.reset(
                          new gkg::CorrelationRatioSimilarityMeasure< T1, T2 >(
                                reference,
                                floating,
                                referenceLowerThreshold,
                                floatingLowerThreshold,
                                resamplingOrder,
                                subSamplingMaximumSize,
                                scalarParameters,
                                verbose ) );

    }
    else
    {

      throw std::runtime_error( "not a valid similarity measure name" );

    }
    return similarityMeasure;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "gkg::RCPointer< gkg::SimilarityMeasure< T1, T2 > > "
             "gkg::SimilarityMeasureFactory< T1, T2 >::create( "
             "const std::string& similarityMeasureName, "
             "gkg::RCPointer< gkg::Volume< T1 > > reference, "
             "gkg::RCPointer< gkg::Volume< T2 > > floating, "
             "int32_t resamplingOrder, "
             "int32_t subSamplingMaximumSize, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose ) const" );

}


template < class T1, class T2 >
inline
void
gkg::SimilarityMeasureFactory< T1, T2 >::checkSimilarityMeasureName(
                                const std::string& similarityMeasureName ) const
{

  try
  {

    if ( _similarityMeasureNames.find( similarityMeasureName ) ==
         _similarityMeasureNames.end() )
    {

      throw std::runtime_error( "not a valid similarity measure name" );

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void "
             "gkg::SimilarityMeasureFactory< T1, T2 >::"
             "checkSimilarityMeasureName( "
             "const std::string& similarityMeasureName ) const" );

}


template < class T1, class T2 >
inline
void
gkg::SimilarityMeasureFactory< T1, T2 >::checkScalarParameters(
                                 const std::string& similarityMeasureName,
                                 std::vector< double >& scalarParameters ) const
{

  try
  {

    if ( similarityMeasureName == "correlation-coefficient" )
    {

      gkg::CorrelationCoefficientSimilarityMeasure< T1, T2 >::
                                      checkScalarParameters( scalarParameters );

    }
    else if ( similarityMeasureName == "mutual-information" )
    {

      gkg::MutualInformationSimilarityMeasure< T1, T2 >::checkScalarParameters(
                                                             scalarParameters );

    }
    else if ( similarityMeasureName == "normalized-mutual-information" )
    {

      gkg::NormalizedMutualInformationSimilarityMeasure< T1, T2 >::
                                                          checkScalarParameters(
                                                             scalarParameters );

    }
    else if ( similarityMeasureName == "correlation-ratio" )
    {

      gkg::CorrelationRatioSimilarityMeasure< T1, T2 >::checkScalarParameters(
                                                             scalarParameters );

    }
    else
    {

      throw std::runtime_error( "not a valid similarity measure name" );

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void "
             "gkg::SimilarityMeasureFactory< T1, T2 >::"
             "getDefaultScalarParameters( "
             "const std::string& similarityMeasureName,"
             "std::vector< double >& scalarParameters ) const" );

}


#endif

