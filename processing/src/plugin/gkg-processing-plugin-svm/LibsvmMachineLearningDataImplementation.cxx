#include <gkg-processing-plugin-svm/LibsvmMachineLearningDataImplementation.h>
#include <gkg-processing-machinelearning/MachineLearningData.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::LibsvmMachineLearningDataImplementation::
                                        LibsvmMachineLearningDataImplementation(
                                  gkg::MachineLearningData* machineLearningData,
                                  gkg::SparseMatrix& data )
                                       : gkg::MachineLearningDataImplementation(
                                                            machineLearningData,
                                                            data ),
                                         _measureCount( data.getSize1() ),
                                         _maximumIndex( 0 )
{

  try
  {

    _libsvmData = new svm_node*[ _measureCount ];

    if ( _libsvmData )
    {

      int32_t size2 = data.getSize2();
      int32_t dataIndex = 0;
      for ( dataIndex = 0; dataIndex < _measureCount; dataIndex++ )
      {

        // collecting data for the current index and setting it to
        // a vector of svm_node
        int32_t dataCountForIndex = data.getNonZeroElementCountForLine(
                                                                    dataIndex );
        _libsvmData[ dataIndex ] = new svm_node[ dataCountForIndex + 1 ];

        int32_t j, k;
        int32_t indexCount = 0;
        for ( j = 0, k = 1; j < size2; j++, k++ )
        {

          if ( data.hasElement( dataIndex, j ) )
          {

            _libsvmData[ dataIndex ][ indexCount ].index = k;
            _libsvmData[ dataIndex ][ indexCount ].value = data( dataIndex, j );
            ++ indexCount;

            if ( k > _maximumIndex )
            {

              _maximumIndex = k;

            }

          }

        }
        _libsvmData[ dataIndex ][ indexCount ].index = -1;
        _libsvmData[ dataIndex ][ indexCount ].value = 0.0;

      }

    }

  }
  GKG_CATCH( "gkg::LibsvmMachineLearningDataImplementation::"
             "LibsvmMachineLearningDataImplementation( "
             "gkg::MachineLearningData* machineLearningData, "
             "gkg::SparseMatrix& data )" );

}


gkg::LibsvmMachineLearningDataImplementation::
                                        LibsvmMachineLearningDataImplementation(
                                  gkg::MachineLearningData* machineLearningData,
                                  gkg::Matrix& data )
                                       : gkg::MachineLearningDataImplementation(
                                                            machineLearningData,
                                                            data ),
                                         _measureCount( data.getSize1() ),
                                         _maximumIndex( 0 )
{

  try
  {

    _libsvmData = new svm_node*[ _measureCount ];

    if ( _libsvmData )
    {

      int32_t size2 = data.getSize2();
      int32_t dataIndex = 0;
      for ( dataIndex = 0; dataIndex < _measureCount; dataIndex++ )
      {

        // collecting data for the current index and setting it to
        // a vector of svm_node
        int32_t j, k;
        int32_t dataCountForIndex = 0;
        for ( j = 0; j < size2; j++ )
        {

          if ( std::fabs( data( dataIndex, j ) ) > 1e-6 )
          {

            dataCountForIndex++;

          }

        }

        _libsvmData[ dataIndex ] = new svm_node[ dataCountForIndex + 1 ];

        int32_t indexCount = 0;
        for ( j = 0, k = 1; j < size2; j++, k++ )
        {

          if ( std::fabs( data( dataIndex, j ) ) > 1e-6 )
          {

            _libsvmData[ dataIndex ][ indexCount ].index = k;
            _libsvmData[ dataIndex ][ indexCount ].value = data( dataIndex, j );
            ++ indexCount;

            if ( k > _maximumIndex )
            {

              _maximumIndex = k;

            }

          }

        }
        _libsvmData[ dataIndex ][ indexCount ].index = -1;
        _libsvmData[ dataIndex ][ indexCount ].value = 0.0;

      }

    }

  }
  GKG_CATCH( "gkg::LibsvmMachineLearningDataImplementation::"
             "LibsvmMachineLearningDataImplementation( "
             "gkg::MachineLearningData* machineLearningData, "
             "gkg::Matrix& data )" );

}


gkg::LibsvmMachineLearningDataImplementation::
                                      ~LibsvmMachineLearningDataImplementation()
{

  if ( _libsvmData )
  {

    int32_t i;
    for ( i = 0; i < _measureCount; i++ )
    {

      delete [] _libsvmData[ i ];

    }

    delete [] _libsvmData;

  }

}


int32_t gkg::LibsvmMachineLearningDataImplementation::getMeasureCount() const
{

  return _measureCount;

}


int32_t gkg::LibsvmMachineLearningDataImplementation::getMaximumIndex() const
{

  return _maximumIndex;

}


svm_node**
gkg::LibsvmMachineLearningDataImplementation::getLibsvmData() const
{

  return _libsvmData;

}


gkg::LibsvmMachineLearningDataImplementation* 
gkg::LibsvmMachineLearningDataImplementation::getImplementation( 
                                 gkg::MachineLearningData* machineLearningData )
{

  return static_cast< gkg::LibsvmMachineLearningDataImplementation* >(
                                     machineLearningData->getImplementation() );

}
