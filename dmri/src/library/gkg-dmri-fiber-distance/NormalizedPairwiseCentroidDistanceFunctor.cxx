#include <gkg-dmri-fiber-distance/NormalizedPairwiseCentroidDistanceFunctor.h>
#include <gkg-dmri-fiber-distance/FiberDistanceFunctorFactory.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


gkg::NormalizedPairwiseCentroidDistanceFunctor::
                                      NormalizedPairwiseCentroidDistanceFunctor(
                                 const std::vector< double >& scalarParameters )
                                               : gkg::FiberDistanceFunctor()
{

  try
  {

    // sanity check(s)
    if ( scalarParameters.size() != 4U )
    {

      throw std::runtime_error(
        "bad scalar parameter count, 4 expected:\n"
        "- scalarParameters[ 0 ] -> normalization factor\n"
        "- scalarParameters[ 1 ] -> lower length threshold\n"
        "- scalarParameters[ 2 ] -> upper length threshold\n"
        "- scalarParameters[ 3 ] -> default normalized distance\n" );

    }
    _normalizationFactor = ( float )scalarParameters[ 0 ];
    _lowerLengthThreshold = ( float )scalarParameters[ 1 ];
    _upperLengthThreshold = ( float )scalarParameters[ 2 ];
    _defaultNormalizedDistance = ( float )scalarParameters[ 3 ];

  }
  GKG_CATCH( "gkg::NormalizedPairwiseCentroidDistanceFunctor::"
             "NormalizedPairwiseCentroidDistanceFunctor( "
             "const std::vector< double >& scalarParameters )" );

}


gkg::NormalizedPairwiseCentroidDistanceFunctor::
                                    ~NormalizedPairwiseCentroidDistanceFunctor()
{
}


float gkg::NormalizedPairwiseCentroidDistanceFunctor::getDistance(
                             const gkg::LightCurve3d< float >& centroid1,
                             const gkg::LightCurve3d< float >& centroid2,
                             const float* centroidLength1,
                             const float* centroidLength2,
                             float* nonCorrectedDistancePointer,
                             float* distanceCorrectionPointer ) const
{

  try
  {

    // first computing Euclidean distance between the two centroids
    int32_t centroidPointCount = centroid1.getPointCount();
    if ( centroid2.getPointCount() != centroidPointCount )
    {

      throw std::runtime_error( "not the same point count" );

    }
    float distance = ( float )std::sqrt( fastComputeDistanceSquare( 
                                                   &( centroid1.begin()->x ),
                                                   &( centroid2.begin()->x ),
                                                   centroidPointCount ) );

    if ( nonCorrectedDistancePointer )
    {

      *nonCorrectedDistancePointer = distance;

    }


    // then computing the minimum length of the two centroids
    float minimumCentroidLength = 0.0;
    if ( centroidLength1 && centroidLength2 )
    {

      minimumCentroidLength = std::min( *centroidLength1, *centroidLength2 );

    }
    else
    {

      minimumCentroidLength = std::min( centroid1.getLength(),
                                        centroid2.getLength() );

    }

    float lengthRange = _upperLengthThreshold - _lowerLengthThreshold;
    float lengthStep1 = _lowerLengthThreshold + lengthRange / 3.0f;
    float lengthStep2 = _lowerLengthThreshold + 2.0f * lengthRange / 3.0f;

    float distanceCorrection = 0.0f;
    if ( minimumCentroidLength <  _lowerLengthThreshold )
    {

      distanceCorrection = 0.0f;

    }
    else if ( minimumCentroidLength < lengthStep1 )
    {

      distanceCorrection = -_normalizationFactor *
                           ( minimumCentroidLength - _lowerLengthThreshold ) /
                           lengthRange;

    }
    else if ( minimumCentroidLength < lengthStep2 )
    {

      distanceCorrection = -_normalizationFactor *
                           ( lengthStep1 - _lowerLengthThreshold ) /
                           lengthRange
                           -_normalizationFactor * 1.3 * 
                           ( minimumCentroidLength - lengthStep1 ) /
                           lengthRange;

    }
    else if ( minimumCentroidLength < _upperLengthThreshold )
    {

      distanceCorrection = -_normalizationFactor *
                           ( lengthStep1 - _lowerLengthThreshold ) /
                           lengthRange
                           -_normalizationFactor * 1.3 * 
                           ( lengthStep2 - lengthStep1 ) /
                           lengthRange
                           -_normalizationFactor * 1.6 * 
                           ( minimumCentroidLength - lengthStep2 ) /
                           lengthRange;

    }
    else
    {

      distanceCorrection = -_normalizationFactor *
                           ( lengthStep1 - _lowerLengthThreshold ) /
                           lengthRange
                           -_normalizationFactor * 1.3 * 
                           ( lengthStep2 - lengthStep1 ) /
                           lengthRange
                           -_normalizationFactor * 1.6 * 
                           ( _upperLengthThreshold - lengthStep2 ) /
                           lengthRange;

    }

    if ( distanceCorrectionPointer )
    {

      *distanceCorrectionPointer = distanceCorrection;

    }


    float normalizedDistance = distance + distanceCorrection;
    if ( normalizedDistance < 0.0f )
    {

       normalizedDistance =  _defaultNormalizedDistance;

    }

    return normalizedDistance;

  }
  GKG_CATCH( "float gkg::NormalizedPairwiseCentroidDistanceFunctor::"
             "getDistance( "
             "const gkg::LightCurve3d< float >& fiber1, "
             "const gkg::LightCurve3d< float >& fiber2, "
             "const float* centroidLength1, "
             "const float* centroidLength2 ) const" );

}


double gkg::NormalizedPairwiseCentroidDistanceFunctor::getDistanceSquare(
                                                   const gkg::Matrix& samples,
                                                   int32_t sampleIndex1,
                                                   int32_t sampleIndex2 ) const
{

  try
  {

    // first computing Euclidean distance between the two centroids
    int32_t centroidPointCount = samples.getSize2() / 3;
    float distance = ( float )std::sqrt( fastComputeDistanceSquare( 
                                                    &samples( sampleIndex1, 0 ),
                                                    &samples( sampleIndex2, 0 ),
                                                    centroidPointCount ) );

    // then computing the minimum length of the two centroids
    float minimumCentroidLength = ( float )std::min(
                                             this->getFiberLength(
                                                   &samples( sampleIndex1, 0 ),
                                                   centroidPointCount ),
                                             this->getFiberLength(
                                                   &samples( sampleIndex2, 0 ),
                                                   centroidPointCount ) );

    float lengthRange = _upperLengthThreshold - _lowerLengthThreshold;
    float lengthStep1 = _lowerLengthThreshold + lengthRange / 3.0f;
    float lengthStep2 = _lowerLengthThreshold + 2.0f * lengthRange / 3.0f;

    float distanceCorrection = 0.0f;
    if ( minimumCentroidLength <  _lowerLengthThreshold )
    {

      distanceCorrection = 0.0f;

    }
    else if ( minimumCentroidLength < lengthStep1 )
    {

      distanceCorrection = -_normalizationFactor *
                           ( minimumCentroidLength - _lowerLengthThreshold ) /
                           lengthRange;

    }
    else if ( minimumCentroidLength < lengthStep2 )
    {

      distanceCorrection = -_normalizationFactor *
                           ( lengthStep1 - _lowerLengthThreshold ) /
                           lengthRange
                           -_normalizationFactor * 1.3 * 
                           ( minimumCentroidLength - lengthStep1 ) /
                           lengthRange;

    }
    else if ( minimumCentroidLength < _upperLengthThreshold )
    {

      distanceCorrection = -_normalizationFactor *
                           ( lengthStep1 - _lowerLengthThreshold ) /
                           lengthRange
                           -_normalizationFactor * 1.3 * 
                           ( lengthStep2 - lengthStep1 ) /
                           lengthRange
                           -_normalizationFactor * 1.6 * 
                           ( minimumCentroidLength - lengthStep2 ) /
                           lengthRange;

    }
    else
    {

      distanceCorrection = -_normalizationFactor *
                           ( lengthStep1 - _lowerLengthThreshold ) /
                           lengthRange
                           -_normalizationFactor * 1.3 * 
                           ( lengthStep2 - lengthStep1 ) /
                           lengthRange
                           -_normalizationFactor * 1.6 * 
                           ( _upperLengthThreshold - lengthStep2 ) /
                           lengthRange;

    }

    float normalizedDistance = distance + distanceCorrection;

    if ( normalizedDistance < 0.0f )
    {

       normalizedDistance =  _defaultNormalizedDistance;

    }

    return normalizedDistance * normalizedDistance;

  }
  GKG_CATCH( "double gkg::NormalizedPairwiseCentroidDistanceFunctor::"
             "getDistanceSquare( "
             "const gkg::Matrix& samples, "
             "int32_t sampleIndex1, "
             "int32_t sampleIndex2 ) const" );

}



std::string gkg::NormalizedPairwiseCentroidDistanceFunctor::getStaticName()
{

  try
  {

    return "normalized-pairwise-centroid-distance";

  }
  GKG_CATCH( "std::string gkg::NormalizedPairwiseCentroidDistanceFunctor::"
             "getStaticName()" );

}


std::string 
gkg::NormalizedPairwiseCentroidDistanceFunctor::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: normalization factor (human:10mm)\n"
      ".  <P2>: lower length threshold (human:5mm)\n"
      ".  <P3>: upper length threshold (human:300mm)\n"
      ".  <P4>: default normalized distance (human: 0.1mm)\n" );

  }
  GKG_CATCH( "std::string "
             "gkg::NormalizedPairwiseCentroidDistanceFunctor::"
             "getStringParameterHelp()" );

}


double 
gkg::NormalizedPairwiseCentroidDistanceFunctor::fastComputeDistanceSquare(
                                                           const double* s1,
                                                           const double* s2,
                                                           int32_t count ) const
{

  try
  {

    double distanceSquare = 0.0f;

    double firstMaximumDistanceSquare = 0.0f;


    const double* p1 = s1;
    const double* p2 = s2;

    int32_t index = 0;
    while ( index < count )
    {

      

      distanceSquare = ( p1[ 0 ] - p2[ 0 ] ) * ( p1[ 0 ] - p2[ 0 ] ) +
                       ( p1[ 1 ] - p2[ 1 ] ) * ( p1[ 1 ] - p2[ 1 ] ) +
                       ( p1[ 2 ] - p2[ 2 ] ) * ( p1[ 2 ] - p2[ 2 ] );
      if ( distanceSquare > firstMaximumDistanceSquare )
      {

        firstMaximumDistanceSquare = distanceSquare;

      }
      p1 += 3;
      p2 += 3;
      ++ index;

    }

    double secondMaximumDistanceSquare = 0.0f;
    p1 = s1;
    p2 = s2 + ( count - 1 ) * 3;
    index = 0;
    while ( index < count )
    {

      distanceSquare = ( p1[ 0 ] - p2[ 0 ] ) * ( p1[ 0 ] - p2[ 0 ] ) +
                       ( p1[ 1 ] - p2[ 1 ] ) * ( p1[ 1 ] - p2[ 1 ] ) +
                       ( p1[ 2 ] - p2[ 2 ] ) * ( p1[ 2 ] - p2[ 2 ] );
      if ( distanceSquare > secondMaximumDistanceSquare )
      {

        secondMaximumDistanceSquare = distanceSquare;

      }
      p1 += 3;
      p2 -= 3;
      ++ index;

    }

    return std::min( firstMaximumDistanceSquare, secondMaximumDistanceSquare );

  }
  GKG_CATCH( "double gkg::NormalizedPairwiseCentroidDistanceFunctor::"
             "fastComputeDistanceSquare( "
             "const double* s1, "
             "const double* s2, "
             "int32_t count ) const" );

}


float 
gkg::NormalizedPairwiseCentroidDistanceFunctor::fastComputeDistanceSquare(
                                                           const float* s1,
                                                           const float* s2,
                                                           int32_t count ) const
{

  try
  {

    float distanceSquare = 0.0;

    float firstMaximumDistanceSquare = 0.0;


    const float* p1 = s1;
    const float* p2 = s2;

    int32_t index = 0;
    while ( index < count )
    {

      distanceSquare = ( p1[ 0 ] - p2[ 0 ] ) * ( p1[ 0 ] - p2[ 0 ] ) +
                       ( p1[ 1 ] - p2[ 1 ] ) * ( p1[ 1 ] - p2[ 1 ] ) +
                       ( p1[ 2 ] - p2[ 2 ] ) * ( p1[ 2 ] - p2[ 2 ] );
      if ( distanceSquare > firstMaximumDistanceSquare )
      {

        firstMaximumDistanceSquare = distanceSquare;

      }
      p1 += 3;
      p2 += 3;
      ++ index;

    }

    float secondMaximumDistanceSquare = 0.0;
    p1 = s1;
    p2 = s2 + ( count - 1 ) * 3;
    index = 0;
    while ( index < count )
    {

      distanceSquare = ( p1[ 0 ] - p2[ 0 ] ) * ( p1[ 0 ] - p2[ 0 ] ) +
                       ( p1[ 1 ] - p2[ 1 ] ) * ( p1[ 1 ] - p2[ 1 ] ) +
                       ( p1[ 2 ] - p2[ 2 ] ) * ( p1[ 2 ] - p2[ 2 ] );
      if ( distanceSquare > secondMaximumDistanceSquare )
      {

        secondMaximumDistanceSquare = distanceSquare;

      }
      p1 += 3;
      p2 -= 3;
      ++ index;

    }

    return std::min( firstMaximumDistanceSquare, secondMaximumDistanceSquare );

  }
  GKG_CATCH( "float gkg::NormalizedPairwiseCentroidDistanceFunctor::"
             "fastComputeDistanceSquare( "
             "const float* s1, "
             "const float* s2, "
             "int32_t count ) const" );

}


float gkg::NormalizedPairwiseCentroidDistanceFunctor::getFiberLength( 
                                           const float* s, int32_t count ) const
{

  try
  {

    const float* p = s;
    const float* nextP = p + 3;

    float length = 0.0f;

    int32_t index = 1;
    while ( index < count )
    {

      length += std::sqrt( ( nextP[ 0 ] - p[ 0 ] ) * ( nextP[ 0 ] - p[ 0 ] ) +
                           ( nextP[ 1 ] - p[ 1 ] ) * ( nextP[ 1 ] - p[ 1 ] ) +
                           ( nextP[ 2 ] - p[ 2 ] ) * ( nextP[ 2 ] - p[ 2 ] ) );

      p += 3;
      nextP += 3;
                           
      ++ index;

    }
    return length;

  }
  GKG_CATCH( "float gkg::NormalizedPairwiseCentroidDistanceFunctor::"
             "getFiberLength( const float* s, int32_t count ) const" );

}


double gkg::NormalizedPairwiseCentroidDistanceFunctor::getFiberLength( 
                                          const double* s, int32_t count ) const
{

  try
  {

    const double* p = s;
    const double* nextP = p + 3;

    float length = 0.0;

    int32_t index = 1;
    while ( index < count )
    {

      length += std::sqrt( ( nextP[ 0 ] - p[ 0 ] ) * ( nextP[ 0 ] - p[ 0 ] ) +
                           ( nextP[ 1 ] - p[ 1 ] ) * ( nextP[ 1 ] - p[ 1 ] ) +
                           ( nextP[ 2 ] - p[ 2 ] ) * ( nextP[ 2 ] - p[ 2 ] ) );

      p += 3;
      nextP += 3;
                           
      ++ index;

    }
    return length;

  }
  GKG_CATCH( "double gkg::NormalizedPairwiseCentroidDistanceFunctor::"
             "getFiberLength( const double* s, int32_t count ) const" );

}





static bool init_NormalizedPairwiseCentroidDistanceFunctor
                                                       __attribute__((unused)) =
  gkg::FiberDistanceFunctorFactory::getInstance().registerFiberDistanceFunctor(
    gkg::NormalizedPairwiseCentroidDistanceFunctor::getStaticName(),
    &gkg::NormalizedPairwiseCentroidDistanceFunctor::createInstance,
    gkg::NormalizedPairwiseCentroidDistanceFunctor::getScalarParameterHelp() );
