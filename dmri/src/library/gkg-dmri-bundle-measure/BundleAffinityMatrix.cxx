#include <gkg-dmri-bundle-measure/BundleAffinityMatrix.h>
#include <gkg-dmri-bundle-measure/BundleMeasure_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <iomanip>
#include <omp.h>


///////////////////////////////////////////////////////////////////////////////
// class BundleAffinityMatrixGauge
///////////////////////////////////////////////////////////////////////////////

gkg::BundleAffinityMatrixGauge::BundleAffinityMatrixGauge( uint64_t maxCount )
                               : _count( 0U ),
                                 _maxCount( maxCount ),
                                 _pairIndexAlreadyDisplayed( false )
{
}


void gkg::BundleAffinityMatrixGauge::add( uint64_t value )
{

  try
  {
  
    _count += value;

    display();  
    
  }
  GKG_CATCH( "void gkg::BundleAffinityMatrixGauge::add( uint64_t value )" );

}


void gkg::BundleAffinityMatrixGauge::reset()
{

  try
  {

    _count = 0U;

    display();  
    
  }
  GKG_CATCH( "void gkg::BundleAffinityMatrixGauge::reset()" );

}


void gkg::BundleAffinityMatrixGauge::display()
{
 
  try
  {
   
    if ( _pairIndexAlreadyDisplayed )
    {

      std::cout << gkg::Eraser( 38 );

    }

    std::cout << " [ " << std::setw( 15 ) << _count 
              << " / " << std::setw( 15 ) << _maxCount
              << " ]" << std::flush;

    _pairIndexAlreadyDisplayed = true;
    
  }
  GKG_CATCH( "void gkg::BundleAffinityMatrixGauge::display()" );

}


///////////////////////////////////////////////////////////////////////////////
// class BundleAffinityMatrixContext
///////////////////////////////////////////////////////////////////////////////

gkg::BundleAffinityMatrixContext::BundleAffinityMatrixContext(
      gkg::BundleAffinityMatrixGauge& gauge,
      const std::vector< std::pair< int32_t, int32_t > >&
                               sparseMatrixIndexToSubjectIndexAndCentroidIndex,
      const std::vector< std::vector< gkg::LightCurve3d< float > > >&
                                                        resampledLightCurve3ds,
      const std::vector< std::vector< float > >& resampledCentroidLengths,
      const float& fourTimesMaximumDistance,
      const float& minimumAffinity,
      const float& variance,
      const gkg::RCPointer< gkg::FiberDistanceFunctor >& fiberDistanceFunctor,
      gkg::SparseMatrix& affinityMatrix,
      gkg::SparseMatrix& distanceMatrix,
      std::list< double >* nonCorrectedDistances,
      std::list< double >* distanceCorrections,
      std::list< double >* correctedDistances,
      std::list< double >* correctedAffinities,
      std::list< double >* thresholdedCorrectedDistances,
      std::list< double >* thresholdedCorrectedAffinities,
      const bool& verbose )
                       : gkg::LoopContext< uint64_t >( &gauge ),
                         _sparseMatrixIndexToSubjectIndexAndCentroidIndex(
                           sparseMatrixIndexToSubjectIndexAndCentroidIndex ),
                         _resampledLightCurve3ds( resampledLightCurve3ds ),                             
                         _resampledCentroidLengths( resampledCentroidLengths ),
                         _fourTimesMaximumDistance( fourTimesMaximumDistance ),
                         _minimumAffinity( minimumAffinity ),
                         _variance( variance ),
                         _fiberDistanceFunctor( fiberDistanceFunctor ),
                         _affinityMatrix( affinityMatrix ),
                         _distanceMatrix( distanceMatrix ),
                         _nonCorrectedDistances( nonCorrectedDistances ),
                         _distanceCorrections( distanceCorrections ),
                         _correctedDistances( correctedDistances ),
                         _correctedAffinities( correctedAffinities ),
                         _thresholdedCorrectedDistances(
                                               thresholdedCorrectedDistances ),
                         _thresholdedCorrectedAffinities(
                                               thresholdedCorrectedAffinities ),
                         _verbose( verbose )
{

  try
  {
  
    // computing the pair count to be explored
    int32_t fiberCount = affinityMatrix.getSize1();
    uint64_t pairCount = ( uint64_t )fiberCount *
                         ( uint64_t )( fiberCount - 1 ) / 2U;

    // computing the gauge update period, eg 1% of the total number of pairs
    _gaugeUpdatePeriod = ( uint64_t )( 0.01f * ( float )pairCount );

  }
  GKG_CATCH( "gkg::BundleAffinityMatrixContext::BundleAffinityMatrixContext( "
             "gkg::BundleAffinityMatrixGauge& gauge, "
             "const std::vector< std::pair< int32_t, int32_t > >& "
             "sparseMatrixIndexToSubjectIndexAndCentroidIndex, "
             "const std::vector< std::vector< gkg::LightCurve3d< float > > >& "
             "resampledLightCurve3ds, "
             "const std::vector< std::vector< float > >& "
             "resampledCentroidLengths, "
             "const float& fourTimesMaximumDistance, "
             "const float& minimumAffinity, "
             "const float& variance, "
             "const gkg::RCPointer< gkg::FiberDistanceFunctor >& "
             "fiberDistanceFunctor, "
             "gkg::SparseMatrix& affinityMatrix, "
             "gkg::SparseMatrix& distanceMatrix, "
             "std::list< double >* nonCorrectedDistances, "
             "std::list< double >* distanceCorrections, "
             "std::list< double >* correctedDistances, "
             "std::list< double >* correctedAffinities, "
             "std::list< double >* thresholdedCorrectedDistances, "
             "std::list< double >* thresholdedCorrectedAffinities,"
             "cont bool& verbose )" );

}
                                                             
void gkg::BundleAffinityMatrixContext::doIt( uint64_t startIndex, 
                                             uint64_t count )
{

  try
  {

    std::list< std::pair< int32_t, int32_t > > fiberIndexPairs;
    std::list< float > affinities;
    std::list< float > distances;

    std::list< double > nonCorrectedDistances;
    std::list< double > distanceCorrections;
    std::list< double > correctedDistances;
    std::list< double > correctedAffinities;
    std::list< double > thresholdedCorrectedDistances;
    std::list< double > thresholdedCorrectedAffinities;

    float nonCorrectedDistance = 0.0f;
    float distanceCorrection = 0.0f;
    float distance = 0.0f;
    float affinity = 0.0f;

    int32_t fiberCount = _affinityMatrix.getSize1();
    int32_t f1 = 0;
    int32_t f2 = 0;
    uint64_t fiberPairIndex = 0U;
    for ( f1 = 0; f1 < fiberCount; f1++ )
    {
    
      for ( f2 = 0; f2 < f1; f2++ )
      {

        if ( ( fiberPairIndex >= startIndex ) &&
             ( fiberPairIndex < ( startIndex + count ) ) )
        {

          if ( _verbose && _gauge )
          {
      
            if ( ( fiberPairIndex - startIndex ) &&
                 !( ( fiberPairIndex - startIndex ) % _gaugeUpdatePeriod ) )
            {

              lock();
              _gauge->add( _gaugeUpdatePeriod );
              unlock();

            }
      
          }

          const std::pair< int32_t, int32_t >&
            subjectIndexAndCentroidIndex1 = 
               _sparseMatrixIndexToSubjectIndexAndCentroidIndex[ f1 ];

          const std::pair< int32_t, int32_t >&
            subjectIndexAndCentroidIndex2 = 
               _sparseMatrixIndexToSubjectIndexAndCentroidIndex[ f2  ];

          const gkg::LightCurve3d< float >&
            centroid1 = _resampledLightCurve3ds
                                       [ subjectIndexAndCentroidIndex1.first ]
                                       [ subjectIndexAndCentroidIndex1.second ];
          const gkg::LightCurve3d< float >&
            centroid2 = _resampledLightCurve3ds
                                       [ subjectIndexAndCentroidIndex2.first ]
                                       [ subjectIndexAndCentroidIndex2.second ];
          const float& length1 = _resampledCentroidLengths
                                       [ subjectIndexAndCentroidIndex1.first ]
                                       [ subjectIndexAndCentroidIndex1.second ];
          const float& length2 = _resampledCentroidLengths
                                       [ subjectIndexAndCentroidIndex2.first ]
                                       [ subjectIndexAndCentroidIndex2.second ];
          if ( std::fabs( length1 - length2 ) < _fourTimesMaximumDistance )
          {

            distance = _fiberDistanceFunctor->getDistance(
                                                          centroid1,
                                                          centroid2,
                                                          &length1,
                                                          &length2,
                                                          &nonCorrectedDistance,
                                                          &distanceCorrection );
            affinity = ( float )std::exp( -( distance * distance ) /
                                          _variance );
            if ( _nonCorrectedDistances )
            {

              nonCorrectedDistances.push_back( nonCorrectedDistance );

            }
            if ( _distanceCorrections )
            {

              distanceCorrections.push_back( distanceCorrection );

            }
            if ( _correctedAffinities )
            {

              correctedAffinities.push_back( affinity );

            }

            if ( affinity >= _minimumAffinity )
            {

              fiberIndexPairs.push_back(
                                      std::pair< int32_t, int32_t >( f1, f2 ) );
              affinities.push_back( affinity );
              distances.push_back( distance );
              if ( _thresholdedCorrectedDistances )
              {

                thresholdedCorrectedDistances.push_back( distance );

              }
              if ( _thresholdedCorrectedAffinities )
              {

                thresholdedCorrectedAffinities.push_back( affinity );

              }


            }

          }

        } 
        if ( f1 != f2 )
        {

          ++ fiberPairIndex;

        }

      }
    
    }

    // now locking to fill the output structures
    lock();

    if ( _verbose && _gauge )
    {

      _gauge->add( count % _gaugeUpdatePeriod );

    }

    std::list< std::pair< int32_t, int32_t > >::const_iterator
      p = fiberIndexPairs.begin(),
      pe = fiberIndexPairs.end();
    std::list< float >::const_iterator
      a = affinities.begin();
    std::list< float >::const_iterator
      d = distances.begin();
    while ( p != pe )
    {

      _affinityMatrix( p->first, p->second ) = ( double )*a;
      _distanceMatrix( p->first, p->second ) = ( double )*d;
      ++ p;
      ++ a;
      ++ d;

    }

    if ( _nonCorrectedDistances )
    {

      _nonCorrectedDistances->insert( _nonCorrectedDistances->end(),
                                      nonCorrectedDistances.begin(),
                                      nonCorrectedDistances.end() );

    }
    if ( _distanceCorrections )
    {

      _distanceCorrections->insert( _distanceCorrections->end(),
                                    distanceCorrections.begin(),
                                    distanceCorrections.end() );

    }
    if ( _correctedDistances )
    {

      _correctedDistances->insert( _correctedDistances->end(),
                                   correctedDistances.begin(),
                                   correctedDistances.end() );

    }
    if ( _correctedAffinities )
    {

      _correctedAffinities->insert( _correctedAffinities->end(),
                                    correctedAffinities.begin(),
                                    correctedAffinities.end() );

    }
    if ( _thresholdedCorrectedDistances )
    {

      _thresholdedCorrectedDistances->insert(
                                          _thresholdedCorrectedDistances->end(),
                                          thresholdedCorrectedDistances.begin(),
                                          thresholdedCorrectedDistances.end() );

    }
    if ( _thresholdedCorrectedAffinities )
    {

      _thresholdedCorrectedAffinities->insert(
                                         _thresholdedCorrectedAffinities->end(),
                                         thresholdedCorrectedAffinities.begin(),
                                         thresholdedCorrectedAffinities.end() );

    }

    unlock();

  }
  GKG_CATCH( "void gkg::BundleAffinityMatrixContext::doIt( "
             "int32_t startIndex, "
             "int32_t count )" );
  
}


///////////////////////////////////////////////////////////////////////////////
// class BundleAffinityMatrix
///////////////////////////////////////////////////////////////////////////////


gkg::BundleAffinityMatrix::BundleAffinityMatrix(
              gkg::RCPointer< gkg::FiberDistanceFunctor > fiberDistanceFunctor,
              float maximumDistance,
              float variance,
              int32_t resamplingPointCount )
                          : gkg::BundleMeasure< gkg::SparseMatrix >(),
                            _fiberDistanceFunctor( fiberDistanceFunctor ),
                            _maximumDistance( maximumDistance ),
                            _variance( variance ),
                            _resamplingPointCount( resamplingPointCount )
{
}


gkg::BundleAffinityMatrix::~BundleAffinityMatrix()
{
}


gkg::SparseMatrix gkg::BundleAffinityMatrix::get(
                         const gkg::BundleAffinityMatrix::Bundle& bundle ) const
{

  try
  {

    gkg::SparseMatrix affinityMatrix;
    this->get( bundle, affinityMatrix );

    return affinityMatrix;

  }
  GKG_CATCH( "gkg::SparseMatrix gkg::BundleAffinityMatrix::get( "
             "const gkg::BundleAffinityMatrix::Bundle& bundle ) const" );

}


void gkg::BundleAffinityMatrix::get(
                         const gkg::BundleAffinityMatrix::Bundle& bundle,
                         gkg::SparseMatrix& affinityMatrix  ) const
{

  try
  {

    int32_t fiberCount = bundle.getCurve3dCount();
    affinityMatrix.reallocate( fiberCount, fiberCount );

    float fourTimesMaximumDistance = 4.0f * _maximumDistance;
    float minimumAffinity = ( float )std::exp( -( _maximumDistance *
                                                  _maximumDistance ) /
                                               _variance );

    int32_t f1 = 0;
    int32_t f2 = 0;

    float distance = 0.0f;
    float affinity = 0.0f;

    for ( f1 = 0; f1 < fiberCount; f1++ )
    {

      const LightCurve3d< float >& centroid1 = bundle.getCurve3d( f1 );
      float length1 = centroid1.getLength();

      for ( f2 = 0; f2 < f1; f2++ )
      {

        const LightCurve3d< float >& centroid2 = bundle.getCurve3d( f2 );
        float length2 = centroid2.getLength();

        if ( std::fabs( length1 - length2 ) < fourTimesMaximumDistance )
        {
 
          distance = _fiberDistanceFunctor->getDistance( centroid1,
                                                         centroid2,
                                                         &length1,
                                                         &length2 );
          affinity = ( float )std::exp( -( distance * distance ) / _variance );

          if ( affinity >= minimumAffinity )
          {

            affinityMatrix( f1, f2 ) = affinity;

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::BundleAffinityMatrix::get( "
             "const gkg::BundleAffinityMatrix::Bundle& bundle, "
             "gkg::SparseMatrix& affinityMatrix ) const" );

}




void gkg::BundleAffinityMatrix::get(
  const std::vector< std::vector< gkg::LightCurve3d< float > > >& lightCurve3ds,
  const std::vector< std::pair< int32_t, int32_t > >&
                                sparseMatrixIndexToSubjectIndexAndCentroidIndex,
   int32_t sparseMatrixSize,
   gkg::SparseMatrix& affinityMatrix,
   gkg::SparseMatrix& distanceMatrix,
   bool verbose,
   gkg::Matrix* centroidMatrix,
   std::list< double >* nonCorrectedDistances,
   std::list< double >* distanceCorrections,
   std::list< double >* correctedDistances,
   std::list< double >* correctedAffinities,
   std::list< double >* thresholdedCorrectedDistances,
   std::list< double >* thresholdedCorrectedAffinities ) const
{

  try
  {

    int32_t subjectIndex = 0;
    int32_t subjectCount = ( int32_t )lightCurve3ds.size();

    std::vector< std::vector< gkg::LightCurve3d< float > > >
      resampledLightCurve3ds( subjectCount );

    std::vector< std::vector< float > >
      resampledCentroidLengths( subjectCount );

    if ( centroidMatrix )
    {

      centroidMatrix->reallocate( sparseMatrixSize, _resamplingPointCount * 3 );

    }

    int32_t globalCentroidIndex = 0;
    int32_t coordinateIndex = 0;
    int32_t coordinateCount = _resamplingPointCount * 3;

    for ( subjectIndex = 0; subjectIndex < subjectCount; subjectIndex++ )
    {

      int32_t centroidCount = ( int32_t )lightCurve3ds[ subjectIndex ].size();

      resampledLightCurve3ds[ subjectIndex ].resize( centroidCount );
      resampledCentroidLengths[ subjectIndex ].resize( centroidCount );
      
      int32_t centroidIndex = 0;
      for ( centroidIndex = 0; centroidIndex < centroidCount; centroidIndex++ )
      {

        gkg::LightCurve3d< float >& 
          resampledCentroid = resampledLightCurve3ds[ subjectIndex ]
                                                    [ centroidIndex ];
        
        resampledCentroid = lightCurve3ds[ subjectIndex ][ centroidIndex ].
                                   getEquidistantCurve( _resamplingPointCount );
        resampledCentroidLengths[ subjectIndex ][ centroidIndex ] =
                                                  resampledCentroid.getLength();                                                  
                                                  
        if ( centroidMatrix )
        {

          const float* source = &( resampledCentroid.begin()->x );
          double* destination = &( ( *centroidMatrix )( globalCentroidIndex,
                                                        0 ) );

          for ( coordinateIndex = 0; coordinateIndex < coordinateCount;
                coordinateIndex++ )
          {

            *destination = ( double )*source;
            ++ source;
            ++ destination;

          }
          ++ globalCentroidIndex;

        }                                         

      }

    }

    affinityMatrix.reallocate( sparseMatrixSize, sparseMatrixSize );
    distanceMatrix.reallocate( sparseMatrixSize, sparseMatrixSize );

    float fourTimesMaximumDistance = 4.0f * _maximumDistance;
    float minimumAffinity = ( float )std::exp( -( _maximumDistance *
                                                  _maximumDistance ) /
                                               _variance );

    uint64_t totalElementCount = ( uint64_t )sparseMatrixSize *
                                 ( uint64_t )( sparseMatrixSize - 1 ) / 2U;

    // creating threaded gauge 
    gkg::BundleAffinityMatrixGauge
      bundleAffinityMatrixGauge( ( int32_t )totalElementCount );

    // creating threaded loop context
    gkg::BundleAffinityMatrixContext
      bundleAffinityMatrixContext(
        bundleAffinityMatrixGauge,
        sparseMatrixIndexToSubjectIndexAndCentroidIndex,
        resampledLightCurve3ds,
        resampledCentroidLengths,
        fourTimesMaximumDistance,
        minimumAffinity,
        _variance,
        _fiberDistanceFunctor,
        affinityMatrix,
        distanceMatrix,
        nonCorrectedDistances,
        distanceCorrections,
        correctedDistances,
        correctedAffinities,
        thresholdedCorrectedDistances,
        thresholdedCorrectedAffinities,
        verbose );

    // creating a threaded loop
    gkg::ThreadedLoop< uint64_t >
      threadedLoop( &bundleAffinityMatrixContext,
                    0U,
                    totalElementCount );
    // launching the threaded loop
    threadedLoop.launch();

  }
  GKG_CATCH( "void gkg::BundleAffinityMatrix::get( "
             "const std::vector< std::vector< gkg::LightCurve3d< float > > >& "
             "lightCurve3ds, "
             "const std::vector< std::pair< int32_t, int32_t > >& "
             "sparseMatrixIndexToSubjectIndexAndCentroidIndex, "
             "int32_t sparseMatrixSize, "
             "gkg::SparseMatrix& affinityMatrix, "
             "gkg::SparseMatrix& distanceMatrix, "
             "bool verbose, "
             "std::list< double >* nonCorrectedDistances, "
             "std::list< double >* distanceCorrections, "
             "std::list< double >* correctedDistances, "
             "std::list< double >* correctedAffinities, "
             "std::list< double >* thresholdedCorrectedDistances, "
             "std::list< double >* thresholdedCorrectedAffinities ) const" );

}
