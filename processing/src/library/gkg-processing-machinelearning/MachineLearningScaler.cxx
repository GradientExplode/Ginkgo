#include <gkg-processing-machinelearning/MachineLearningScaler.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-core-exception/Exception.h>


gkg::MachineLearningScaler::MachineLearningScaler( const gkg::Matrix& inMinMax,
                                                   double outMin,
                                                   double outMax )
                          : _outMin( outMin ),
                            _outMax( outMax )
{

  try
  {

    if ( inMinMax.getSize1() != 2 )
    {

      throw std::runtime_error( "bad matrix size" );

    }

    int32_t i, size2 = inMinMax.getSize2();

    _scale.reallocate( 2, size2 + 1 );
    _scale( 0, size2 ) = outMin;
    _scale( 1, size2 ) = outMax;

    for ( i = 0; i < size2; i++ )
    {

      _scale( 0, i ) = inMinMax( 0, i );
      _scale( 1, i ) = inMinMax( 1, i );

    }

  }
  GKG_CATCH( "gkg::MachineLearningScaler::MachineLearningScaler( "
             "const gkg::Matrix& inMinMax, double outMin, double outMax )" );

}


gkg::MachineLearningScaler::MachineLearningScaler( double outMin,
                                                   double outMax )
                          : _outMin( outMin ),
                            _outMax( outMax )
{
}


gkg::MachineLearningScaler::MachineLearningScaler( 
                                       const gkg::MachineLearningScaler& other )
                          : _scale( other._scale ),
                            _outMin( other._outMin ),
                            _outMax( other._outMax )
{
}


gkg::MachineLearningScaler::~MachineLearningScaler()
{
}


gkg::MachineLearningScaler& gkg::MachineLearningScaler::operator = (
                                       const gkg::MachineLearningScaler& other )
{

  copy( other );
  
  return *this;

}


const gkg::Matrix& gkg::MachineLearningScaler::getScaleMatrix() const
{

  return _scale;

}


void gkg::MachineLearningScaler::copy( const gkg::MachineLearningScaler& other )
{

  _scale = other._scale;
  _outMin = other._outMin;
  _outMax = other._outMax;

}
