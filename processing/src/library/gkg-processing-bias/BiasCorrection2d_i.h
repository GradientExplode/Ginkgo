#ifndef _gkg_processing_bias_BiasCorrection2d_i_h_
#define _gkg_processing_bias_BiasCorrection2d_i_h_


#include <gkg-processing-bias/BiasCorrection2d.h>
#include <gkg-processing-bias/BiasCorrection_i.h>
#include <gkg-processing-bias/EntropyBiasFunction2d_i.h>
#include <gkg-processing-bias/MeanSquareDifferenceBiasFunction2d_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>
#include <map>

#if 0

#include <fstream>

#endif

//
// class BiasCorrection2d< T >
//

// scalarParameters[ 0 ] -> reference slice (-1 means middle slice)
// scalarParameters[ 1 ] -> initial parameter
// scalarParameters[ 2 ] -> delta parameter
// scalarParameters[ 3 ] -> maximum Nelder-Mead iteration count 
// scalarParameters[ 4 ] -> maximum Nelder-Mead test size
// scalarParameters[ 5 ] -> verbose Nelder-Mead
// scalarParameters[ 6 ] -> level count if entropy type
// scalarParameters[ 7 ] -> smooth if entropy type

// stringParameters[ 0 ] -> measure type mean-square-difference / entropy
// stringParameters[ 1 ] -> bias type multiplicative / additive
// stringParameters[ 2 ] -> plane axis x / y / z

template < class T >
inline
gkg::BiasCorrection2d< T >::BiasCorrection2d(
                            const std::vector< double >& scalarParameters,
                            const std::vector< std::string >& stringParameters )
                           : gkg::BiasCorrection< T >()
{

  try
  {

    std::vector< double > theScalarParameters;
    std::vector< std::string > theStringParameters;

    //
    // sanity check(s)
    //
    if ( scalarParameters.empty() ||
         stringParameters.empty() )
    {

      theScalarParameters.resize( 6U );
      theScalarParameters[ 0 ] = -1;
      theScalarParameters[ 1 ] = 1.0;
      theScalarParameters[ 2 ] = 0.05;
      theScalarParameters[ 3 ] = 1000;
      theScalarParameters[ 4 ] = 1e-4;
      theScalarParameters[ 5 ] = 0;


      theStringParameters.resize( 3U );
      theStringParameters[ 0 ] = "mean-square-difference";
      theStringParameters[ 1 ] = "multiplicative";
      theStringParameters[ 2 ] = "z";

    }
    else
    {

      theScalarParameters = scalarParameters;
      theStringParameters = stringParameters;

    }

    //
    // dealing with string parameters
    //

    if ( theStringParameters.size() != 3U )
    {

      throw std::runtime_error( "bad number of string parameters" );

    }

    _measureType = theStringParameters[ 0 ];
    if ( ( _measureType != "mean-square-difference" ) &&
         ( _measureType != "entropy" ) )
    {

      throw std::runtime_error( "bad measure type" );

    }

    if ( theStringParameters[ 1 ] == "multiplicative" )
    {

      _biasFunctor.reset( new gkg::MultiplicativeBiasFunctor< T > );

    }
    else if ( theStringParameters[ 1 ] == "additive" )
    {

      _biasFunctor.reset( new gkg::AdditiveBiasFunctor< T > );

    }
    else
    {

      throw std::runtime_error( "bad bias type" );

    }

    _planeAxis = theStringParameters[ 2 ];
    if ( ( _planeAxis != "x" ) &&
         ( _planeAxis != "y" ) &&
         ( _planeAxis != "z" ) )
    {

      throw std::runtime_error( "bad plane axis" );

    }    

    //
    // dealing with scalar parameters
    //

    if ( ( ( _measureType == "mean-square-difference" ) &&
           ( theScalarParameters.size() != 6U ) ) ||
         ( ( _measureType == "entropy" ) &&
           ( theScalarParameters.size() != 8U ) ) )
    {

      throw std::runtime_error( "bad number of scalar parameters" );

    }

    if ( theScalarParameters[ 0 ] < 0 )
    {

      _referenceSlice = -1;

    }
    else
    {

      _referenceSlice = ( int32_t )( theScalarParameters[ 0 ] + 0.5 );

    }
    _initialParameter = theScalarParameters[ 1 ];
    _deltaParameter = theScalarParameters[ 2 ];
    _maximumIterationCount = ( int32_t )( theScalarParameters[ 3 ] + 0.5 );
    _maximumTestSize = theScalarParameters[ 4 ];
    _verboseNelderMead = ( int32_t )( theScalarParameters[ 5 ] + 0.5 ) ? true:
                                                                         false;
    if ( _measureType == "entropy" )
    {

      _entropyLevelCount = ( int32_t )( theScalarParameters[ 6 ] + 0.5 );
      _entropyIsSmoothingApplied = ( int32_t )( theScalarParameters[ 7 ] +
                                                0.5 ) ?
                                                true:
                                                false;

    }
    else
    {

      _entropyLevelCount = 0;
      _entropyIsSmoothingApplied = false;

    }


  }
  GKG_CATCH( "gkg::BiasCorrection2d< T >::BiasCorrection2d( "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters )" );

}


template < class T >
inline
gkg::BiasCorrection2d< T >::~BiasCorrection2d()
{
}


template < class T >
std::string gkg::BiasCorrection2d< T >::getName() const
{

  return getStaticName();

}


template < class T >
std::string gkg::BiasCorrection2d< T >::getStaticName()
{

  return "bias-correction-2d";

}


template < class T >
inline
void gkg::BiasCorrection2d< T >::correct( const gkg::Volume< T >& in,
                                          const gkg::Volume< int16_t >& mask,
                                          const gkg::Volume< float >& weight,
                                          gkg::Volume< T >& out,
                                          gkg::Volume< float >& bias,
                                          bool verbose ) const
{

  try
  {

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // allocating map for correction factor(s) and mean correction
    std::map< std::string, double > correction;
    double meanCorrection = 0.0;

    // creating bias Nelder-Mead Simplex function
    gkg::RCPointer< gkg::BiasNelderMeadFunction2d< T > >
      biasNelderMeadFunction2d;
    if ( _measureType == "mean-square-difference" )
    {

      biasNelderMeadFunction2d.reset(
               new gkg::MeanSquareDifferenceBiasFunction2d< T >( _biasFunctor,
                                                                 in,
                                                                 mask,
                                                                 weight,
                                                                 out,
                                                                 bias,
                                                                 _planeAxis ) );

    }
    else if ( _measureType == "entropy" )
    {

      biasNelderMeadFunction2d.reset(
            new gkg::EntropyBiasFunction2d< T >( _biasFunctor,
                                                 in,
                                                 mask,
                                                 weight,
                                                 out,
                                                 bias,
                                                 _planeAxis,
                                                 _entropyLevelCount,
                                                 _entropyIsSmoothingApplied ) );

    }

    // processing real slice count according to plane axis
    int32_t sliceCount = 0;
    if ( _planeAxis == "x" )
    {

      sliceCount = in.getSizeX();

    }
    else if ( _planeAxis == "y" )
    {

      sliceCount = in.getSizeY();

    }
    else if ( _planeAxis == "z" )
    {

      sliceCount = in.getSizeZ();

    }

    // sanity check
    if ( _referenceSlice >= sliceCount )
    {

      throw std::runtime_error( "bad reference slice" );

    }
    int32_t referenceSlice = _referenceSlice;
    if ( referenceSlice < 0 )
    {

      referenceSlice = sliceCount / 2;

    }


    // looping over times
    int32_t slice, t;
    int32_t sizeT = in.getSizeT();
    int32_t sliceCounter = 0;
    for ( t = 0; t < sizeT; t++ )
    {

      if ( verbose && !_verboseNelderMead )
      {

        if ( t != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << " time[ " << std::setw( 4 ) << t + 1
                  << " / " << std::setw( 4 ) << sizeT
                  << " ]" << std::flush;

      }

      biasNelderMeadFunction2d->setTime( t );

      // upward estimation
      for ( slice = referenceSlice; slice < sliceCount - 1; slice++,
                                                            sliceCounter++ )
      {

        if ( verbose )
        {

          if ( !_verboseNelderMead )
          {

            if ( slice != referenceSlice )
            {

              std::cout << gkg::Eraser( 21 );

            }
            std::cout << " slice[ " << std::setw( 4 ) << sliceCounter + 1
                      << " / " << std::setw( 4 ) << sliceCount - 1
                      << " ]" << std::flush;

          }
          else
          {


            std::cout << " time=" << t
                      << " reference-slice=" << slice
                      << " biased-slice=" << slice + 1
                      << std::endl;

          }

        }

        biasNelderMeadFunction2d->setReferenceSlice( slice );
        biasNelderMeadFunction2d->setBiasedSlice( slice + 1 );

        gkg::Vector parameter( 1 );
        parameter( 0 ) = _initialParameter;

        gkg::Vector deltaParameter( 1 );
        deltaParameter( 0 ) = _deltaParameter;

        double testSize = 0.0;

        factory->getNelderMeadSimplexOptimumParameters(
                                             parameter,
                                             deltaParameter,
                                             testSize,
                                             *biasNelderMeadFunction2d,
                                             _maximumIterationCount,
                                             _maximumTestSize,
                                             _verboseNelderMead );

        std::string key1 = gkg::StringConverter::toString( slice );
        if ( key1.length() == 1 )
        {

          key1 = std::string( "00" ) + key1;

        }
        if ( key1.length() == 2 )
        {

          key1 = std::string( "0" ) + key1;

        }
        std::string key2 = gkg::StringConverter::toString( slice + 1 );
        if ( key2.length() == 1 )
        {

          key2 = std::string( "00" ) + key2;

        }
        if ( key2.length() == 2 )
        {

          key2 = std::string( "0" ) + key2;

        }
        meanCorrection += parameter( 0 );
        correction[ getCode( t, slice, slice + 1 ) ] = parameter( 0 );

      }

      // backward estimation
      for ( slice = referenceSlice; slice > 0; slice--,
                                               sliceCounter++ )
      {

        if ( verbose )
        {


          if ( !_verboseNelderMead )
          {

            std::cout << gkg::Eraser( 21 );
            std::cout << " slice[ " << std::setw( 4 ) << sliceCounter + 1
                      << " / " << std::setw( 4 ) << sliceCount - 1
                      << " ]" << std::flush;

          }
          else
          {

            std::cout << " time=" << t
                      << " reference-slice=" << slice
                      << " biased-slice=" << slice - 1
                      << std::endl;

          }

        }

        biasNelderMeadFunction2d->setReferenceSlice( slice );
        biasNelderMeadFunction2d->setBiasedSlice( slice - 1 );

        gkg::Vector parameter( 1 );
        parameter( 0 ) = _initialParameter;

        gkg::Vector deltaParameter( 1 );
        deltaParameter( 0 ) = _deltaParameter;

        double testSize = 0.0;

        factory->getNelderMeadSimplexOptimumParameters(
                                             parameter,
                                             deltaParameter,
                                             testSize,
                                             *biasNelderMeadFunction2d,
                                             _maximumIterationCount,
                                             _maximumTestSize,
                                             _verboseNelderMead );

        meanCorrection += parameter( 0 );
        correction[ getCode( t, slice, slice - 1 ) ] = parameter( 0 );

      }
      meanCorrection /= ( double )correction.size();

      // upward correction
      float scale = _initialParameter;
      for ( slice = referenceSlice; slice < sliceCount - 1; slice++ )
      {

        biasNelderMeadFunction2d->setReferenceSlice( slice );
        biasNelderMeadFunction2d->setBiasedSlice( slice + 1 );
        scale *= correction[ getCode( t, slice, slice + 1 ) ] / meanCorrection;
        biasNelderMeadFunction2d->applyBiasCorrection( scale );

      }

      // backward correction
      scale = _initialParameter;
      for ( slice = referenceSlice; slice > 0; slice-- )
      {

        biasNelderMeadFunction2d->setReferenceSlice( slice );
        biasNelderMeadFunction2d->setBiasedSlice( slice - 1 );
        scale *= correction[ getCode( t, slice, slice - 1 ) ] / meanCorrection;
        biasNelderMeadFunction2d->applyBiasCorrection( scale );

      }

    }

    if ( verbose && !_verboseNelderMead )
    {

      std::cout << gkg::Eraser( 41 );

    }

#if 0

    std::ofstream os1( "correction_slices.txt" );
    std::ofstream os2( "correction_value.txt" );
    std::ofstream os3( "correction_mean.txt" );
    std::map< std::string, double >::const_iterator
      c = correction.begin(),
      ce = correction.end();
    while ( c != ce )
    {

      os1 << c->first << std::endl;
      os2 << c->second << std::endl;
      os3 << meanCorrection << std::endl;
      ++ c;

    }
    os1.close();
    os2.close();
    os3.close();

#endif



  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::BiasCorrection2d< T >::correct( "
             "const gkg::Volume< T >& in, "
             "const gkg::Volume< float >& weight, "
             "gkg::Volume< T >& out, "
             "gkg::Volume< float >& bias, "
             "bool verbose ) const" );

}


template < class T >
inline
std::string 
gkg::BiasCorrection2d< T >::getCode( int32_t time,
                                     int32_t slice1,
                                     int32_t slice2 ) const
{

  try
  {

    std::string code;

    std::string zero = "0000";

    std::string timeString = gkg::StringConverter::toString( time );
    std::string keyTime = zero.substr( 0, 4U - timeString.length() ) + timeString;

    std::string slice1String = gkg::StringConverter::toString( slice1 );
    std::string keySlice1 = zero.substr( 0, 4U - slice1String.length() ) + slice1String;

    std::string slice2String = gkg::StringConverter::toString( slice2 );
    std::string keySlice2 = zero.substr( 0, 4U - slice2String.length() ) + slice2String;

    return keyTime + '-' + keySlice1 + '-' + keySlice2;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "std::string "
             "gkg::BiasCorrection2d< T >::getCode( "
             "int32_t time, int32_t slice1, int32_t slice2 ) const" );

}


#endif


