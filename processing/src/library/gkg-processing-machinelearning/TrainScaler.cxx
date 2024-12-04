#include <gkg-processing-machinelearning/TrainScaler.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-core-exception/Exception.h>

#include <limits>


gkg::TrainScaler::TrainScaler( const gkg::Matrix& inMinMax,
                               double outMin,
                               double outMax )
                : gkg::MachineLearningScaler( inMinMax, outMin, outMax )
{
}


gkg::TrainScaler::TrainScaler( double outMin, double outMax )
                : gkg::MachineLearningScaler( outMin, outMax )
{
}


gkg::TrainScaler::TrainScaler( const gkg::MachineLearningScaler& other )
                : gkg::MachineLearningScaler( other )
{
}


void gkg::TrainScaler::scale( gkg::SparseMatrix& data )
{

  try
  {

    int32_t size1 = data.getSize1();
    int32_t size2 = data.getSize2();

    if ( _scale.getSize2() != ( size2 + 1 ) )
    {

      _scale.reallocate( 2, size2 + 1 );
      _scale.setZero();
      _scale( 0, size2 ) = _outMin;
      _scale( 1, size2 ) = _outMax;

      int32_t i, j;
      for ( i = 0; i < size2; i++ )
      {

        double min = std::numeric_limits< double >::max();
        double max = std::numeric_limits< double >::lowest();

        for ( j = 0; j < size1; j++ )
        {

          if ( data.hasElement( j, i ) )
          {

            double value = data( j, i );

            if ( value < min )
            {

              min = value;

            }
            if ( value > max )
            {

              max = value;

            }

          }

        }

        if ( max > min )
        {

          _scale( 0, i ) = min;
          _scale( 1, i ) = max;

        }

      }

    }

    int32_t i;
    for ( i = 0; i < size2; i++ )
    {

      double inMin = _scale( 0, i );
      double inMax = _scale( 1, i );

      if ( inMax != inMin )
      {

        double factor = ( _outMax - _outMin ) / ( inMax - inMin );
        int32_t j;

        for ( j = 0; j < size1; j++ )
        {

          if ( data.hasElement( j, i ) )
          {

            double inValue = data( j, i );
            double outValue;

            if ( inValue < inMin )
            {

              outValue = _outMin;

            }
            else if ( inValue > inMax )
            {

              outValue = _outMax;

            }
            else
            {

              outValue = _outMin + factor * ( inValue - inMin );

            }

            data( j, i ) = outValue;

          }

        }

      }

    }

  }
  GKG_CATCH( "double gkg::TrainScaler::scale( gkg::SparseMatrix& data )" );

}


void gkg::TrainScaler::scale( gkg::Matrix& data )
{

  try
  {

    int32_t size1 = data.getSize1();
    int32_t size2 = data.getSize2();

    if ( _scale.getSize2() != ( size2 + 1 ) )
    {

      _scale.reallocate( 2, size2 + 1 );
      _scale.setZero();
      _scale( 0, size2 ) = _outMin;
      _scale( 1, size2 ) = _outMax;

      int32_t i, j;
      for ( i = 0; i < size2; i++ )
      {

        double min = data( 0, i );
        double max = min;

        for ( j = 0; j < size1; j++ )
        {

          double value = data( j, i );

          if ( value < min )
          {

            min = value;

          }
          else if ( value > max )
          {

            max = value;

          }

        }

        if ( max > min )
        {

          _scale( 0, i ) = min;
          _scale( 1, i ) = max;

        }

      }

    }

    int32_t i;
    for ( i = 0; i < size2; i++ )
    {

      double inMin = _scale( 0, i );
      double inMax = _scale( 1, i );

      if ( inMax != inMin )
      {

        double factor = ( _outMax - _outMin ) / ( inMax - inMin );
        int32_t j;

        for ( j = 0; j < size1; j++ )
        {

          double inValue = data( j, i );
          double outValue;

          if ( inValue < inMin )
          {

            outValue = _outMin;

          }
          else if ( inValue > inMax )
          {

            outValue = _outMax;

          }
          else
          {

            outValue = _outMin + factor * ( inValue - inMin );

          }

          data( j, i ) = outValue;

        }

      }

    }

  }
  GKG_CATCH( "double gkg::TrainScaler::scale( gkg::Matrix& data )" );

}
