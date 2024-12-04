#ifndef _gkg_processing_bias_EntropyBiasSimulatedAnnealingConfiguration3d_i_h_
#define _gkg_processing_bias_EntropyBiasSimulatedAnnealingConfiguration3d_i_h_


#include <gkg-processing-bias/EntropyBiasSimulatedAnnealingConfiguration3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


#define GKG_ENTROPY_SMOOTHING_SIGMA  1.0


template < class T >
inline
gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::
                                   EntropyBiasSimulatedAnnealingConfiguration3d(
                     const gkg::Volume< T >& in,
                     const gkg::Volume< int16_t >& mask,
                     const gkg::Volume< float >& weight,
                     gkg::Volume< T >& out,
                     gkg::Volume< float >& bias,
                     const gkg::Vector3d< int32_t >& piecewiseBiasSize,
                     double entropyFactor,
                     double dataAttachmentFactor,
                     double regularizationFactor,
                     int32_t levelCount,
                     bool isSmoothingApplied )
                                       : gkg::SimulatedAnnealingConfiguration(),
                                         _in( in ),
                                         _mask( mask ),
                                         _weight( weight ),
                                         _out( out ),
                                         _bias( bias ),
                                         _time( 0 ),
                                         _mean( 0.0 ),
                                         _minimum( 0.0 ),
                                         _maximum( 0.0 ),
                                         _piecewiseBias( piecewiseBiasSize,
                                                         in.getSizeT() ),
                                         _entropyFactor( entropyFactor ),
                                         _dataAttachmentFactor(
                                                         dataAttachmentFactor ),
                                         _regularizationFactor(
                                                         regularizationFactor ),
                                         _levelCount( levelCount ),
                                         _isSmoothingApplied(
                                                           isSmoothingApplied ),
                                         _randomGenerator(
                                                    gkg::RandomGenerator::Taus )
{

  try
  {

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // collecting size and resolution
    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 0;
    _in.getSize( size, sizeT );

    gkg::Vector3d< double > resolution;
    _in.getResolution( resolution );

    // allocating output
    _out = _in;

    // allocating bias
    _bias.reallocate( size, sizeT );
    _bias.setResolution( resolution );
    _bias.fill( 1.0 );

    // checking weight size
    gkg::Vector3d< int32_t > weightSize;
    int32_t weightSizeT = 0;
    _weight.getSize( weightSize, weightSizeT );
    if ( size != weightSize )
    {

      throw std::runtime_error( "incoherent weight and input sizes" );

    }
    if ( weightSizeT != 1 )
    {

      throw std::runtime_error( "mask T size should be equal to 1" );

    }

    // initializing piecewise bias
    gkg::Vector3d< double > piecewiseResolution(
                              ( resolution.x * ( double )_bias.getSizeX() ) /
                              ( double )_piecewiseBias.getSizeX(),
                              ( resolution.y * ( double )_bias.getSizeY() ) /
                              ( double )_piecewiseBias.getSizeY(),
                              ( resolution.z * ( double )_bias.getSizeZ() ) /
                              ( double )_piecewiseBias.getSizeZ() );
    _piecewiseBias.setResolution( piecewiseResolution );
    gkg::Volume< float >::iterator b = _piecewiseBias.begin(),
                                   be = _piecewiseBias.end();
    while ( b != be )
    {

      *b = ( float )factory->getUniformRandomNumber( _randomGenerator,
                                                     0.5, 2.0 );
      ++ b;

    }

    // allocating Deriche Gaussian filter coefficients
    if ( _isSmoothingApplied )
    {

      _dericheGaussianCoefficients = 
        new gkg::DericheGaussianCoefficients< double >(
              GKG_ENTROPY_SMOOTHING_SIGMA,
              gkg::DericheGaussianCoefficients< double >::Smoothing );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::"
             "EntropyBiasSimulatedAnnealingConfiguration3d( "
             "const gkg::Volume< T >& in, "
             "const gkg::Volume< int16_t >& mask, "
             "const gkg::Volume< float >& weight, "
             "gkg::Volume< T >& out, "
             "gkg::Volume< float >& bias, "
             "const gkg::Vector3d< int32_t >& piecewiseBiasSize, "
             "double entropyFactor, "
             "double dataAttachmentFactor, "
             "double regularizationFactor, "
             "int32_t levelCount, "
             "bool isSmoothingApplied )" );

}


template < class T >
inline
gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::
                                   EntropyBiasSimulatedAnnealingConfiguration3d(
           const gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >& other )
                                       : gkg::SimulatedAnnealingConfiguration(),
                                         _in( other._in ),
                                         _mask( other._mask ),
                                         _weight( other._weight ),
                                         _out( other._out ),
                                         _bias( other._bias ),
                                         _time( other._time ),
                                         _mean( other._mean ),
                                         _minimum( other._minimum ),
                                         _maximum( other._maximum ),
                                         _piecewiseBias( other._piecewiseBias ),
                                         _entropyFactor( other._entropyFactor ),
                                         _dataAttachmentFactor(
                                                  other._dataAttachmentFactor ),
                                         _regularizationFactor(
                                                  other._regularizationFactor ),
                                         _levelCount( other._levelCount ),
                                         _isSmoothingApplied(
                                                    other._isSmoothingApplied ),
                                         _randomGenerator(
                                                    gkg::RandomGenerator::Taus )
{

  try
  {

    // allocating Deriche Gaussian filter coefficients
    if ( _isSmoothingApplied )
    {

      _dericheGaussianCoefficients = 
        new gkg::DericheGaussianCoefficients< double >(
              GKG_ENTROPY_SMOOTHING_SIGMA,
              gkg::DericheGaussianCoefficients< double >::Smoothing );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::"
             "EntropyBiasSimulatedAnnealingConfiguration3d( "
             "const gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >& "
             "other )" );

}


template < class T >
inline
gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::
                                 ~EntropyBiasSimulatedAnnealingConfiguration3d()
{

  if ( _isSmoothingApplied )
  {

    delete _dericheGaussianCoefficients;

  }

}


template < class T >
inline
void 
gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::setTime( int32_t time )
{

  try
  {

    // updating time
    _time = time;

    // processing mean of input volume for current time
    gkg::Vector3d< int32_t > size;
    _in.getSize( size );
    _mean = 0.0;
    gkg::Vector3d< int32_t > voxel;
    for ( voxel.z = 0; voxel.z < size.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < size.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < size.x; voxel.x++ )
        {

          _mean += ( double )_in( voxel, _time );

        }

      }

    }
    _mean /= ( double )size.x * ( double )size.y * ( double )size.z;


    // processing 95% boundaries
    gkg::Volume< T > temporaryVolume( size );
    for ( voxel.z = 0; voxel.z < size.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < size.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < size.x; voxel.x++ )
        {

          temporaryVolume( voxel ) = _in( voxel, _time );

        }

      }

    }
    T lowerBoundary = 0;
    T upperBoundary = 0;
    gkg::HistogramAnalyzer::getInstance().getPercentileBoundaries(
                                                            temporaryVolume,
                                                            _levelCount,
                                                            98.0,
                                                            lowerBoundary,
                                                            upperBoundary );
    _minimum = ( double )lowerBoundary;
    _maximum = ( double )upperBoundary;

    std::cout << "---->setTime(): _minimum=" << _minimum
              << "   _maximum=" << _maximum << std::endl;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::"
             "setTime( "
             "int32_t time )" );

}


template < class T >
inline
const gkg::Volume< float >& 
gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::getPiecewiseBias() const
{

  try
  {

    return _piecewiseBias;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const gkg::Volume< float >& "
             "gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::"
             "getPiecewiseBias() const" );

}


template < class T >
inline
double gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::getEnergy() const
{

  try
  {


    // collecting full size and resolution
    gkg::Vector3d< int32_t > size;
    _in.getSize( size );

    gkg::Vector3d< double > resolution;
    _in.getResolution( resolution );

    // collecting piecewise size and resolution
    gkg::Vector3d< int32_t > piecewiseSize;
    _piecewiseBias.getSize( piecewiseSize );

    gkg::Vector3d< double > piecewiseResolution;
    _piecewiseBias.getResolution( piecewiseResolution );

    // processing voxel scalings from full resolution to piecewise grid
    double ratioX = resolution.x / piecewiseResolution.x;
    double ratioY = resolution.y / piecewiseResolution.y;
    double ratioZ = resolution.z / piecewiseResolution.z;

    //
    // processing temporary unbiased volume
    //
    gkg::Volume< T > unbiased( size );
    gkg::Vector3d< int32_t > voxel;
    double unbiasedMean = 0.0;
    for ( voxel.z = 0; voxel.z < size.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < size.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < size.x; voxel.x++ )
        {

          if ( _mask( voxel ) && ( _weight( voxel ) > 0.0 ) )
          {

            unbiased( voxel ) = ( T )( ( double )_in( voxel, _time ) *
                                        ( double )_piecewiseBias(
                                                ( int32_t )( voxel.x * ratioX ),
                                                ( int32_t )( voxel.y * ratioY ),
                                                ( int32_t )( voxel.z * ratioZ ),
                                                _time ) );

          }
          else
          {

            unbiased( voxel ) = _in( voxel, _time );

          }
          unbiasedMean += ( double )unbiased( voxel );

        }

      }

    }
    unbiasedMean /= ( double )size.x * ( double )size.y * ( double )size.z;


    //
    // processing entropy energy
    //

    // processing 95% boundaries
    gkg::Matrix probabilities( _levelCount, 1 );
    probabilities.setZero();
    int32_t level = 0;
    typename gkg::Volume< T >::const_iterator u = unbiased.begin(),
                                              ue = unbiased.end();
    while ( u != ue )
    {

      level = ( int32_t )( ( ( ( double )*u - _minimum ) /
                             ( _maximum - _minimum ) ) *
                           ( this->_levelCount - 1 ) );
      if ( level < 0 )
      {

         level = 0;

      }
      else if ( level >= this->_levelCount )
      {

         level = this->_levelCount - 1;

      }
      ++ probabilities( level, 0 );
      ++ u;

    }

    // smoothing the join probabilities
    if ( _isSmoothingApplied )
    {

      gkg::Matrix smoothedProbabilities;

      gkg::DericheGaussianFilter3d< double >::getInstance().filter(
        probabilities,
        *this->_dericheGaussianCoefficients,
        gkg::DericheGaussianFilter3d< double >::X_AXIS,
        smoothedProbabilities );
      probabilities = smoothedProbabilities;

    }

    // processing the normalization factor
    double normalizationFactor = 0.0;
    for ( level = 0; level < this->_levelCount; level++ )
    {

      normalizationFactor += probabilities( level, 0 );

    }

    // normalizing the join probabilities
    if ( normalizationFactor > 0.0 )
    {

      for ( level = 0; level < this->_levelCount; level++ )
      {

        probabilities( level, 0 ) /= normalizationFactor;

      }

    }

    // processing the entropy
    double probability = 0.0;
    double entropy = 0.0;
    for ( level = 0; level < this->_levelCount; level++ )
    {

      probability = probabilities( level, 0 );
      if ( probability > 0.0 )
      {

        entropy -= probability * std::log( probability );

      }

    }

    double entropyEnergy = entropy;

    //
    // processing data attachment energy
    //

    double dataAttachmentEnergy = ( unbiasedMean - _mean ) *
                                  ( unbiasedMean - _mean );


    //
    // processing regularization energy
    //

    double regularizationEnergy = 0.0;
    double membranePotential = 0.0;
    gkg::Vector3d< int32_t > neighbor;
    for ( voxel.z = 0; voxel.z < piecewiseSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < piecewiseSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < piecewiseSize.x; voxel.x++ )
        {

           neighbor.x = voxel.x + 1;
           neighbor.y = voxel.y;
           neighbor.z = voxel.z;
           if ( neighbor.x < piecewiseSize.x )
           {

             membranePotential = std::log( _piecewiseBias( voxel, _time ) /
                                           _piecewiseBias( neighbor, _time ) );
             regularizationEnergy += membranePotential * membranePotential;

           }

           neighbor.x = voxel.x - 1;
           neighbor.y = voxel.y;
           neighbor.z = voxel.z;
           if ( neighbor.x >= 0 )
           {

             membranePotential = std::log( _piecewiseBias( voxel, _time ) /
                                           _piecewiseBias( neighbor, _time ) );
             regularizationEnergy += membranePotential * membranePotential;

           }

           neighbor.x = voxel.x;
           neighbor.y = voxel.y + 1;
           neighbor.z = voxel.z;
           if ( neighbor.y < piecewiseSize.y )
           {

             membranePotential = std::log( _piecewiseBias( voxel, _time ) /
                                           _piecewiseBias( neighbor, _time ) );
             regularizationEnergy += membranePotential * membranePotential;

           }

           neighbor.x = voxel.x;
           neighbor.y = voxel.y - 1;
           neighbor.z = voxel.z;
           if ( neighbor.y >= 0 )
           {

             membranePotential = std::log( _piecewiseBias( voxel, _time ) /
                                           _piecewiseBias( neighbor, _time ) );
             regularizationEnergy += membranePotential * membranePotential;

           }

           neighbor.x = voxel.x;
           neighbor.y = voxel.y;
           neighbor.z = voxel.z + 1;
           if ( neighbor.z < piecewiseSize.z )
           {

             membranePotential = std::log( _piecewiseBias( voxel, _time ) /
                                           _piecewiseBias( neighbor, _time ) );
             regularizationEnergy += membranePotential * membranePotential;

           }

           neighbor.x = voxel.x;
           neighbor.y = voxel.y;
           neighbor.z = voxel.z - 1;
           if ( neighbor.z >= 0 )
           {

             membranePotential = std::log( _piecewiseBias( voxel, _time ) /
                                           _piecewiseBias( neighbor, _time ) );
             regularizationEnergy += membranePotential * membranePotential;

           }

        }

      }

    }

    std::cout << "---->getEnergy(): "
              << "entropy=" << _entropyFactor * entropy << "  |  "
              << "attachment=" << _dataAttachmentFactor * 
                                    dataAttachmentEnergy << "  |  "
              << "regularization=" << _regularizationFactor *
                                        regularizationEnergy << "  |  "
              << "global=" << _entropyFactor * entropyEnergy +
                              _dataAttachmentFactor * dataAttachmentEnergy +
                              _regularizationFactor * regularizationEnergy
              << std::endl;

    return _entropyFactor * entropyEnergy +
           _dataAttachmentFactor * dataAttachmentEnergy +
           _regularizationFactor * regularizationEnergy;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::"
             "getEnergy() const" );

}


template < class T >
inline
void gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::step(
                                   double uniformRandomNumber, double stepSize )
{

  try
  {

    // getting a pointer to the numerical analysis factory
    static gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // choosing randomly a voxel of the piecewise bias
    gkg::Vector3d< int32_t >
      voxel( ( int32_t )factory->getUniformRandomUInt32(
                                              _randomGenerator,
                                              _piecewiseBias.getSizeX() - 1 ),
             ( int32_t )factory->getUniformRandomUInt32(
                                              _randomGenerator,
                                              _piecewiseBias.getSizeY() - 1 ),
             ( int32_t )factory->getUniformRandomUInt32(
                                              _randomGenerator,
                                              _piecewiseBias.getSizeZ() - 1 ) );

    // modifying randomly the bias value at this voxel
    _piecewiseBias( voxel, _time ) *=
      ( float )( uniformRandomNumber * ( 1 / stepSize - stepSize ) + stepSize );

    std::cout << "====>step(): voxel = " << voxel << " bias = "
              << _piecewiseBias( voxel, _time )
              << std::endl;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void "
             "gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::step( "
             "double uniformRandomNumber, double stepSize )" );

}


template < class T >
inline
double gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::getDistance(
                       const gkg::SimulatedAnnealingConfiguration& other ) const
{

  try
  {


    const gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >*
      otherConfiguration =
    static_cast< const gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >* >
                                                                     ( &other );

    gkg::Volume< float >::const_iterator
      b1 = _piecewiseBias.begin(),
      b1e = _piecewiseBias.end();
    gkg::Volume< float >::const_iterator
      b2 = otherConfiguration->getPiecewiseBias().begin();
    double distance = 0.0;
    double discrepancy = 0.0;
    while ( b1 != b1e )
    {

      discrepancy = *b1 - *b2;
      distance += discrepancy * discrepancy;
      ++ b1;
      ++ b2;

    }
    distance = std::sqrt( distance );

    std::cout << "---->getDistance(): distance = " << distance << std::endl;

    return distance;

  }
  GKG_CATCH( "template < class T > "
             "inline "
            "double gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::"
            "getDistance( "
            "const gkg::SimulatedAnnealingConfiguration& other ) const" );

}


template < class T >
inline
void gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::display() const
{

  try
  {

    // do not display anything as a volume is huge to display

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::"
             "display() const" );

}


template < class T >
inline
void gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::copy(
                             const gkg::SimulatedAnnealingConfiguration& other )
{

  try
  {

    std::cout << "---->copy()" << std::endl;

    const gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >*
      otherConfiguration =
    static_cast< const gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >* >
                                                                     ( &other );

    _piecewiseBias = otherConfiguration->getPiecewiseBias();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void "
             "gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::copy( "
             "const gkg::SimulatedAnnealingConfiguration& other )" );

}


template < class T >
inline
gkg::SimulatedAnnealingConfiguration* 
gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::copyConstruct() const
{

  try
  {

    std::cout << "---->copyConstruct()" << std::endl;
    return new gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >( *this );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::SimulatedAnnealingConfiguration* "
             "gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::"
             "copyConstruct() const" );

}


template < class T >
inline
void 
gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::applyBiasCorrection()
{

  try
  {

    // reinterpolating field using splines
    gkg::Resampler< float >*
    resampler = gkg::ResamplerFactory< float >::getInstance().getResampler( 3 );

    // allocating 3D identity tranform
    gkg::IdentityTransform3d< float > identityTransform3d;

    // collecting piecewise size and resolution
    gkg::Vector3d< int32_t > piecewiseSize;
    int32_t piecewiseSizeT;
    gkg::Vector3d< double > piecewiseResolution;
    _piecewiseBias.getSize( piecewiseSize, piecewiseSizeT );
    _piecewiseBias.getResolution( piecewiseResolution );

    // collecting full size and resolution
    gkg::Vector3d< int32_t > size;
    gkg::Vector3d< double > resolution;
    _bias.getSize( size );
    _bias.getResolution( resolution );

    // allocating temporary piecewise volume
    gkg::Volume< float > tmpPiecewiseBias( piecewiseSize );
    tmpPiecewiseBias.setResolution( piecewiseResolution );

    // allocating temporary full resolution volume
    gkg::Volume< float > tmpBias( size );
    tmpBias.setResolution( resolution );

    // looping over time(s)
    gkg::Vector3d< int32_t > voxel;
    int32_t t = 0;
    for ( t = 0; t < piecewiseSizeT; t++ )
    {

      // copying current bias volume at t to the temporary piecewise volume
      for ( voxel.z = 0; voxel.z < piecewiseSize.z; voxel.z++ )
      {

        for ( voxel.y = 0; voxel.y < piecewiseSize.y; voxel.y++ )
        {

          for ( voxel.x = 0; voxel.x < piecewiseSize.x; voxel.x++ )
          {

            tmpPiecewiseBias( voxel ) = _piecewiseBias( voxel, t );

          }

        }

      }

      // resampling piecewise bias to full resolution bias
      resampler->resample( tmpPiecewiseBias,
                           identityTransform3d,
                           1.0f,
                           tmpBias,
                           false );

      // copying resampled bias volume at t to the destination bias volume
      for ( voxel.z = 0; voxel.z < size.z; voxel.z++ )
      {

        for ( voxel.y = 0; voxel.y < size.y; voxel.y++ )
        {

          for ( voxel.x = 0; voxel.x < size.x; voxel.x++ )
          {

            _bias( voxel, t ) = tmpBias( voxel );

          }

        }

      }

    }

    // performing correction using reinterpolated bias field
    typename gkg::Volume< T >::iterator o = _out.begin(),
                                        oe = _out.end();
    typename gkg::Volume< T >::const_iterator i = _in.begin();
    gkg::Volume< float >::const_iterator b = _bias.begin();

    while ( o != oe )
    {

      *o = ( T )( ( double )*i * ( double )*b );
      ++ o;
      ++ i;
      ++ b;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void "
             "gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >::"
             "applyBiasCorrection()" );

}


#undef GKG_ENTROPY_SMOOTHING_SIGMA


#endif

