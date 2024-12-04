#ifndef _gkg_processing_numericalanalysis_HypercubeIndexMapper_h_
#define _gkg_processing_numericalanalysis_HypercubeIndexMapper_h_


//
// Cyril Poupon, Maelig Chauvel, 2020
// The following code is adapted from the HPDBSCAN toolbox available
// at https://github.com/jingluis/HPDBSCAN, and published by Markus Gotz
// in order to match the API of the Ginkgo toolbox, also available freely from
// https://framagit.org/cpoupon/gkg for free for non-commercial use
//
//
//
//
// Copyright (c) 2018 Markus Götz
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
// copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE. 
//


#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/DistanceFunctor.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>
#include <map>
#include <limits>


namespace gkg
{


class HypercubeIndexMapper
{

  public:

    enum
    {

      Noise = std::numeric_limits< int32_t >::max() - 1,
      Unclassified = std::numeric_limits< int32_t >::max()

    };

    HypercubeIndexMapper( const Matrix& samples,
                          const std::vector< int32_t >& chosenGridCoordinates,
                          double epsilon,
                          double maximumDistance,
                          RCPointer< DistanceFunctor > distanceFunctor );
    virtual ~HypercubeIndexMapper();

    int32_t getSampleCount() const;
    int32_t getCoordinateCount() const;
    int32_t getChosenCoordinateCount() const;
    double getMaximumDistanceSquare() const;

    std::vector< int32_t >
      getNeighborSampleIndices( const int64_t & cellIndex ) const;

    int64_t getCellIndex( int32_t sampleIndex ) const;
    int64_t getReorderedCellIndex( int32_t sampleIndex ) const;
    const gkg::Matrix& getReorderedSamples() const;

    int32_t regionQuery( int32_t sampleIndex,
                         const std::vector< int32_t >& neighborSampleIndices,
                         const std::vector< int32_t >& clusterLabels,
                         std::vector< int32_t >& minSampleArea ) const;

    void recoverInitialOrder( std::vector< int32_t >& clusterLabels ) const;

  protected:

    int32_t _sampleCount;
    int32_t _coordinateCount;

    std::vector< int32_t > _chosenGridCoordinates;
    int32_t _chosenCoordinateCount;

    double _maximumDistanceSquare;

    RCPointer< DistanceFunctor > _distanceFunctor;
  

    std::vector< int32_t > _initialSampleOrder;

    std::vector< double > _minimumCoordinateValues;
    std::vector< double > _maximumCoordinateValues;

    std::vector< int32_t > _cellCounts;
    int64_t _globalCellCount;
    int64_t _lastCellIndex;

    std::vector< int32_t > _ascendingCellCountBasedCoordinates;

    std::map< int64_t, int32_t > _cellHistogram;
    std::vector< int64_t > _sampleToCellIndices;
    
    std::map< int64_t, std::pair< int32_t, int32_t > >
                                        _cellIndexToSampleStartingIndexAndCount;

    std::vector< int64_t > _reorderedSampleToCellIndices;
    std::vector< int32_t > _reorderedSampleOrder;
    gkg::Matrix _reorderedSamples;

};


}


#endif

