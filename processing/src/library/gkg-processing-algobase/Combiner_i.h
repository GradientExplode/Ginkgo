#ifndef _gkg_processing_algobase_Combiner_i_h_
#define _gkg_processing_algobase_Combiner_i_h_


#include <gkg-processing-algobase/Combiner.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


//
// class Combiner< IN, MASK, OUT >
//

template < class IN, class MASK, class OUT >
inline
gkg::Combiner< IN, MASK, OUT >::Combiner(
                                 const std::vector< std::string >& functor1s,
                                 const std::vector< std::string >& functor2s,
                                 const std::vector< double >& numerator1s,
                                 const std::vector< double >& denominator1s,
                                 const std::vector< double >& numerator2s,
                                 const std::vector< double >& denominator2s,
                                 const std::vector< std::string >& operators,
                                 const gkg::TestFunction< MASK >* testFunction,
                                 const OUT& background )
{

  try
  {

    // collecting input count
    _inputCount = ( int32_t )functor1s.size();

    // sanity check(s)
    if ( ( int32_t )functor2s.size() != _inputCount )
    {

      throw std::runtime_error( "incoherent functor 1 and functor 2 size(s)" );

    }
    if ( ( int32_t )numerator1s.size() != _inputCount + 1 )
    {

      throw std::runtime_error(
                              "incoherent functor 1 and numerators 1 size(s)" );

    }
    if ( ( int32_t )denominator1s.size() != _inputCount + 1 )
    {

      throw std::runtime_error(
                            "incoherent functor 1 and denominators 1 size(s)" );

    }
    if ( ( int32_t )numerator2s.size() != _inputCount )
    {

      throw std::runtime_error(
                              "incoherent functor 1 and numerators 2 size(s)" );

    }
    if ( ( int32_t )denominator2s.size() != _inputCount )
    {

      throw std::runtime_error(
                            "incoherent functor 1 and denominators 2 size(s)" );

    }
    if ( ( int32_t )operators.size() != _inputCount )
    {

      throw std::runtime_error( "incoherent functor 1 and operator size(s)" );

    }

    // filling functors 1 and 2, and operators
    _functor1s.resize( _inputCount );
    _functor2s.resize( _inputCount );
    _operators.resize( _inputCount );
    int32_t i;
    for ( i = 0; i < _inputCount; i++ )
    {

      _functor1s[ i ] = getFunctor( functor1s[ i ] );
      _functor2s[ i ] = getFunctor( functor2s[ i ] );
      _operators[ i ] = getOperator( operators[ i ] );

    }

    _numerator1s = numerator1s;
    _denominator1s = denominator1s;
    _numerator2s = numerator2s;
    _denominator2s = denominator2s;
    _testFunction = testFunction;
    _background = background;

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "gkg::Combiner< IN, MASK, OUT >::Combiner( "
             "const std::vector< std::string >& functor1s, "
             "const std::vector< std::string >& functor2s, "
             "const std::vector< double >& numerator1s, "
             "const std::vector< double >& denominator1s, "
             "const std::vector< double >& numerator2s, "
             "const std::vector< double >& denominator2s, "
             "const std::vector< std::string >& operators, "
             "const gkg::TestFunction< MASK >* testFunction, "
             "const OUT& background )" );

}


template < class IN, class MASK, class OUT >
inline
int32_t gkg::Combiner< IN, MASK, OUT >::getInputCount() const
{

  return _inputCount;

}


template < class IN, class MASK, class OUT >
inline
void gkg::Combiner< IN, MASK, OUT >::combine(
                                           const std::vector< IN >& inputValues,
                                           const MASK& maskValue,
                                           OUT& outputValue,
                                           bool /*verbose*/ ) const
{

  try
  {

    if ( ( int32_t )inputValues.size() != _inputCount )
    {

      throw std::runtime_error( "bad input vector size" );

    }
    if ( _testFunction->getState( maskValue ) )
    {

      double value = _numerator1s[ _inputCount ] /
                     _denominator1s[ _inputCount ];
      double currentValue = 0.0;
      int32_t i;
      for ( i = _inputCount - 1; i >= 0; i-- )
      {

        _converterInToDouble.convert( inputValues[ i ], currentValue );
        _operators[ i ]( ( _numerator1s[ i ] /
                           _denominator1s[ i ] ) *
                             _functor1s[ i ](
                                              ( _numerator2s[ i ] /
                                                _denominator2s[ i ] ) *
                                              _functor2s[ i ]( currentValue )
                                             ),
                         value );

      }
      _converterDoubleToOut.convert( value, outputValue );

    }
    else
    {

      outputValue = _background;

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "void gkg::Combiner< IN, MASK, OUT >::combine( "
             "const std::vector< IN >& inputValues, "
             "const MASK& maskValue, "
             "OUT& outputValues, "
             "bool verbose ) const" );

}


template < class IN, class MASK, class OUT >
inline
void gkg::Combiner< IN, MASK, OUT >::combine(
                                           const std::vector< IN >& inputValues,
                                           OUT& outputValue,
                                           bool /*verbose*/ ) const
{

  try
  {

    if ( ( int32_t )inputValues.size() != _inputCount )
    {

      throw std::runtime_error( "bad input vector size" );

    }

    double value = _numerator1s[ _inputCount ] /
                   _denominator1s[ _inputCount ];
    double currentValue = 0.0;
    int32_t i;
    for ( i = _inputCount - 1; i >= 0; i-- )
    {

      _converterInToDouble.convert( inputValues[ i ], currentValue );
      _operators[ i ]( ( _numerator1s[ i ] /
                         _denominator1s[ i ] ) *
                           _functor1s[ i ](
                                            ( _numerator2s[ i ] /
                                              _denominator2s[ i ] ) *
                                            _functor2s[ i ]( currentValue )
                                           ),
                       value );

    }
    _converterDoubleToOut.convert( value, outputValue );

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "void gkg::Combiner< IN, MASK, OUT >::combine( "
             "const std::vector< IN >& inputValues, "
             "OUT& outputValues, "
             "bool verbose ) const" );

}


template < class IN, class MASK, class OUT >
inline
typename gkg::Combiner< IN, MASK, OUT >::Functor
gkg::Combiner< IN, MASK, OUT >::getFunctor( const std::string& functorName )
{

  try
  {

    if ( functorName == "id" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorId;

    }
    else if ( functorName == "inv" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorInv;

    }
    else if ( functorName == "ln" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorLn;

    }
    else if ( functorName == "log" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorLog;

    }
    else if ( functorName == "exp" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorExp;

    }
    else if ( functorName == "sqr" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorSqr;

    }
    else if ( functorName == "cub" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorCub;

    }
    else if ( functorName == "sqrt" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorSqrt;

    }
    else if ( functorName == "cbrt" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorCbrt;

    }
    else if ( functorName == "cos" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorCos;

    }
    else if ( functorName == "sin" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorSin;

    }
    else if ( functorName == "tan" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorTan;

    }
    else if ( functorName == "acos" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorAcos;

    }
    else if ( functorName == "asin" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorAsin;

    }
    else if ( functorName == "atan" )
    {

      return gkg::Combiner< IN, MASK, OUT >::functorAtan;

    }
    else
    {

      throw std::runtime_error( "unknown functor" );

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "typename gkg::Combiner< IN, MASK, OUT >::Functor "
             "gkg::Combiner< IN, MASK, OUT >::getFunctor( "
             "const std::string& functorName )" );

}


template < class IN, class MASK, class OUT >
inline
typename gkg::Combiner< IN, MASK, OUT >::Operator
gkg::Combiner< IN, MASK, OUT >::getOperator( const std::string& operatorName )
{

  try
  {

    if ( operatorName == "+" )
    {


      return gkg::Combiner< IN, MASK, OUT >::operatorPlus;

    }
    else if ( operatorName == "-" )
    {

      return gkg::Combiner< IN, MASK, OUT >::operatorMinus;

    }
    else if ( operatorName == "*" )
    {

      return gkg::Combiner< IN, MASK, OUT >::operatorMultiply;

    }
    else if ( operatorName == "/" )
    {

      return gkg::Combiner< IN, MASK, OUT >::operatorDivide;

    }
    else
    {

      throw std::runtime_error( "unknown operator" );

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "typename gkg::Combiner< IN, MASK, OUT >::Operator "
             "gkg::Combiner< IN, MASK, OUT >::getOperator( "
             "const std::string& operatorName )" );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorId( const double& v )
{

  return v;

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorInv( const double& v )
{

  return 1.0 / v;

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorLn( const double& v )
{

  return std::log( v );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorLog( const double& v )
{

  return std::log10( v );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorExp( const double& v )
{

  return std::exp( v );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorSqr( const double& v )
{

  return v * v;

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorCub( const double& v )
{

  return v * v * v;

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorSqrt( const double& v )
{

  return std::sqrt( v );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorCbrt( const double& v )
{

  return std::pow( v, 1.0 / 3.0 );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorCos( const double& v )
{

  return std::cos( v );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorSin( const double& v )
{

  return std::sin( v );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorTan( const double& v )
{

  return std::tan( v );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorAcos( const double& v )
{

  return std::acos( v );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorAsin( const double& v )
{

  return std::asin( v );

}


template < class IN, class MASK, class OUT >
inline
double gkg::Combiner< IN, MASK, OUT >::functorAtan( const double& v )
{

  return std::atan( v );

}


template < class IN, class MASK, class OUT >
inline
void gkg::Combiner< IN, MASK, OUT >::operatorPlus( const double& v1,
                                                   double& v2 )
{

  v2 = v1 + v2;

}


template < class IN, class MASK, class OUT >
inline
void gkg::Combiner< IN, MASK, OUT >::operatorMinus( const double& v1,
                                                    double& v2 )
{

  v2 = v1 - v2;

}


template < class IN, class MASK, class OUT >
inline
void gkg::Combiner< IN, MASK, OUT >::operatorMultiply( const double& v1,
                                                       double& v2 )
{

  v2 = v1 * v2;

}


template < class IN, class MASK, class OUT >
inline
void gkg::Combiner< IN, MASK, OUT >::operatorDivide( const double& v1,                                                       double& v2 )
{

  v2 = v1 / v2;

}



//
// class Combiner< Volume< IN >, Volume< MASK >, Volume< OUT > >
//

template < class IN, class MASK, class OUT >
inline
gkg::Combiner< gkg::Volume< IN >,
               gkg::Volume< MASK >,
               gkg::Volume< OUT > >::Combiner(
                                 const std::vector< std::string >& functor1s,
                                 const std::vector< std::string >& functor2s,
                                 const std::vector< double >& numerator1s,
                                 const std::vector< double >& denominator1s,
                                 const std::vector< double >& numerator2s,
                                 const std::vector< double >& denominator2s,
                                 const std::vector< std::string >& operators,
                                 const gkg::TestFunction< MASK >* testFunction,
                                 const OUT& background )
                : _itemCombiner( functor1s,
                                 functor2s,
                                 numerator1s,
                                 denominator1s,
                                 numerator2s,
                                 denominator2s,
                                 operators,
                                 testFunction,
                                 background )
{
}


template < class IN, class MASK, class OUT >
inline
void gkg::Combiner< gkg::Volume< IN >,
                    gkg::Volume< MASK >,
                    gkg::Volume< OUT > >::combine(
                                 std::vector< gkg::Volume< IN > >& inputVolumes,
                                 gkg::Volume< MASK >& maskVolume,
                                 Volume< OUT >& outputVolume,
                                 bool verbose ) const
{

  try
  {

    // collecting input volume count
    int32_t inputCount = ( int32_t )inputVolumes.size();

    // sanity check(s)
    if ( inputCount != _itemCombiner.getInputCount() )
    {

      throw std::runtime_error(
                   "input volume count and combiner input count incompatible" );

    }
    int32_t i;
    for ( i = 1; i < inputCount; i++ )
    {

      if ( !inputVolumes[ i ].hasSameSize( inputVolumes[ 0 ], true ) ||
           !inputVolumes[ i ].hasSameResolution( inputVolumes[ 0 ], true ) )
      {

        throw std::runtime_error(
                           "incoherent input volume size(s) or resolution(s)" );

      }

    }   
    if ( !( maskVolume.getSizeX() == inputVolumes[ 0 ].getSizeX() &&
            maskVolume.getSizeY() == inputVolumes[ 0 ].getSizeY() &&
            maskVolume.getSizeZ() == inputVolumes[ 0 ].getSizeZ() ) ||
         !maskVolume.hasSameResolution( inputVolumes[ 0 ], true ) )
    {

      throw std::runtime_error(
                  "incoherent mask and input volume size(s) or resolution(s)" );

    }
    // in case mask is only 3D, for t to be 0 for other times
    bool forceMaskT = ( maskVolume.getSizeT() != inputVolumes[ 0 ].getSizeT() );

    // allocating output
    reallocate( inputVolumes[ 0 ], outputVolume );

    // combining volume(s)
    std::vector< IN > inputValues( inputCount );
    int32_t sizeX = inputVolumes[ 0 ].getSizeX();
    int32_t sizeY = inputVolumes[ 0 ].getSizeY();
    int32_t sizeZ = inputVolumes[ 0 ].getSizeZ();
    int32_t sizeT = inputVolumes[ 0 ].getSizeT();
    int32_t x, y, z, t;
    for ( t = 0; t < sizeT; t++ )
    {

      if ( verbose && ( sizeT > 1 ) )
      {

        if ( t != 0 )
        {

          std::cout << gkg::Eraser( 14 );

        }
        std::cout << " [ " << std::setw( 3 ) << t + 1 
                  << " / " << std::setw( 3 ) << sizeT
                  << " ]" << std::flush;

      }
      for ( z = 0; z < sizeZ; z++ )
      {

        if ( verbose && ( sizeT == 1 ) )
        {

          if ( z != 0 )
          {

            std::cout << gkg::Eraser( 14 );

          }
          std::cout << " [ " << std::setw( 3 ) << z + 1 
                    << " / " << std::setw( 3 ) << sizeZ
                    << " ]" << std::flush;

        }
        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            for ( i = 0; i < inputCount; i++ )
            {

              if ( inputVolumes[ i ].getSizeT() == 1 )
              {

                inputValues[ i ] = inputVolumes[ i ]( x, y, z, 0 );

              }
              else
              {

                inputValues[ i ] = inputVolumes[ i ]( x, y, z, t );

              }

            }
            _itemCombiner.combine( inputValues,
                                   maskVolume( x, y, z,
                                               ( forceMaskT ? 0 : t ) ),
                                   outputVolume( x, y, z, t ),
                                   verbose );

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 14 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "void gkg::Combiner< gkg::Volume< IN >, "
             "                    gkg::Volume< MASK >, "
             "                    gkg::Volume< OUT > >::combine( "
             "std::vector< gkg::Volume< IN > >& inputVolumes, "
             "gkg::Volume< MASK >& maskVolume, "
             "Volume< OUT >& outputVolume, "
             "bool verbose ) const" );

}


template < class IN, class MASK, class OUT >
inline
void gkg::Combiner< gkg::Volume< IN >,
                    gkg::Volume< MASK >,
                    gkg::Volume< OUT > >::combine(
                           std::vector< gkg::Volume< IN > >& inputVolumes,
                           Volume< OUT >& outputVolume,
                           bool verbose ) const
{

  try
  {

    // collecting input volume count
    int32_t inputCount = ( int32_t )inputVolumes.size();

    // sanity check(s)
    if ( inputCount != _itemCombiner.getInputCount() )
    {

      throw std::runtime_error(
                   "input volume count and combiner input count incompatible" );

    }
    int32_t i;
    for ( i = 1; i < inputCount; i++ )
    {

      if ( !inputVolumes[ i ].hasSameSize( inputVolumes[ 0 ], true ) ||
           !inputVolumes[ i ].hasSameResolution( inputVolumes[ 0 ], true ) )
      {

        throw std::runtime_error(
                           "incoherent input volume size(s) or resolution(s)" );

      }

    }   

    // allocating output
    reallocate( inputVolumes[ 0 ], outputVolume );

    // combining volume(s)
    std::vector< IN > inputValues( inputCount );
    int32_t sizeX = inputVolumes[ 0 ].getSizeX();
    int32_t sizeY = inputVolumes[ 0 ].getSizeY();
    int32_t sizeZ = inputVolumes[ 0 ].getSizeZ();
    int32_t sizeT = inputVolumes[ 0 ].getSizeT();
    int32_t x, y, z, t;
    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            for ( i = 0; i < inputCount; i++ )
            {

              if ( inputVolumes[ i ].getSizeT() == 1 )
              {

                inputValues[ i ] = inputVolumes[ i ]( x, y, z, 0 );

              }
              else
              {

                inputValues[ i ] = inputVolumes[ i ]( x, y, z, t );

              }

            }
            _itemCombiner.combine( inputValues,
                                   outputVolume( x, y, z, t ),
                                   verbose );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "void gkg::Combiner< gkg::Volume< IN >, "
             "                    gkg::Volume< MASK >, "
             "                    gkg::Volume< OUT > >::combine( "
             "std::vector< gkg::Volume< IN > >& inputVolumes, "
             "Volume< OUT >& outputVolume, "
             "bool verbose ) const" );

}


template < class IN, class MASK, class OUT >
inline
void gkg::Combiner< gkg::Volume< IN >,
                    gkg::Volume< MASK >,
                    gkg::Volume< OUT > >::reallocate(
                                                 const gkg::Volume< IN >& in,
                                                 gkg::Volume< OUT >& out ) const
{

  try
  {

    if ( ( void* )&out != ( void* )&in )
    {

      // reallocating data
      out.reallocate( in.getSizeX(),
                      in.getSizeY(),
                      in.getSizeZ(),
                      in.getSizeT() );

      // duplicating header
      out.getHeader() = in.getHeader();
      out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "void gkg::Combiner< gkg::Volume< IN >, "
             "                    gkg::Volume< MASK >, "
             "                    gkg::Volume< OUT > >::reallocate( "
             "const gkg::Volume< IN >& in, "
             "gkg::Volume< OUT >& out ) const" );

}




#endif
