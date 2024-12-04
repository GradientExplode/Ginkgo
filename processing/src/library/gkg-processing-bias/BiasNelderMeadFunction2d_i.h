#ifndef _gkg_processing_bias_BiasNelderMeadFunction2d_i_h_
#define _gkg_processing_bias_BiasNelderMeadFunction2d_i_h_


#include <gkg-processing-bias/BiasNelderMeadFunction2d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>


//
// class BiasNelderMeadFunction2d< T >
//

template < class T >
inline
gkg::BiasNelderMeadFunction2d< T >::BiasNelderMeadFunction2d(
                     gkg::RCPointer< gkg::BiasFunctor< T > > biasFunctor,
                     const gkg::Volume< T >& in,
                     const gkg::Volume< int16_t >& mask,
                     const gkg::Volume< float >& weight,
                     gkg::Volume< T >& out,
                     gkg::Volume< float >& bias,
                     const std::string& planeAxis )
                                          : gkg::NelderMeadSimplexFunction( 1 ),
                                            _biasFunctor( biasFunctor ),
                                            _in( in ),
                                            _mask( mask ),
                                            _weight( weight ),
                                            _out( out ),
                                            _bias( bias ),
                                            _planeAxis( planeAxis ),
                                            _referenceSlice( 0 ),
                                            _biasedSlice( 0 ),
                                            _time( 0 )
{

  try
  {

    // collecting size and resolution
    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 0;
    _in.getSize( size, sizeT );

    gkg::Vector3d< double > resolution;
    _in.getResolution( resolution );

    // allocating output
    _out = _in;

    // allocating bias
    _bias.reallocate( size.x, size.y, size.z, sizeT );
    _bias.setResolution( resolution );
    _bias.fill( 0.0f );

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

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::BiasNelderMeadFunction2d< T >::"
             "BiasNelderMeadFunction2d( "
             "gkg::RCPointer< gkg::BiasFunctor< T > > biasFunctor, "
             "const gkg::Volume< T >& in, "
             "const gkg::Volume< int16_t >& mask, "
             "const gkg::Volume< float >& weight, "
             "gkg::Volume< T >& out, "
             "gkg::Volume< float >& bias, "
             "const std::string& planeAxis )" );

}


template < class T >
inline
gkg::BiasNelderMeadFunction2d< T >::~BiasNelderMeadFunction2d()
{
}


template < class T >
inline
void gkg::BiasNelderMeadFunction2d< T >::setReferenceSlice( int32_t slice )
{

  try
  {

    _referenceSlice = slice;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::BiasNelderMeadFunction2d< T >::"
             "setReferenceSlice( "
             "int32_t slice )" );

}


template < class T >
inline
void 
gkg::BiasNelderMeadFunction2d< T >::setBiasedSlice( int32_t slice )
{

  try
  {

    _biasedSlice = slice;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::BiasNelderMeadFunction2d< T >::"
             "setBiasedSlice( "
             "int32_t slice )" );

}


template < class T >
inline
void 
gkg::BiasNelderMeadFunction2d< T >::setTime( int32_t time )
{

  try
  {

    _time = time;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::BiasNelderMeadFunction2d< T >::"
             "setTime( "
             "int32_t time )" );

}


template < class T >
inline
void gkg::BiasNelderMeadFunction2d< T >::applyBiasCorrection( double parameter )
{

  try
  {

    if ( _planeAxis == "x" )
    {

      int32_t lineCount = _in.getSizeY();
      int32_t sliceCount = _in.getSizeZ();

      int32_t line, slice;
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( line = 0; line < lineCount; line++ )
        {

          _out( _biasedSlice, line, slice, _time ) = 
            ( T )( _biasFunctor->getValueAt(
                                      _in( _biasedSlice, line, slice, _time ),
                                      parameter ) );
          _bias( _biasedSlice, line, slice, _time ) = ( float )parameter; 

        }

      }

    }
    else if ( _planeAxis == "y" )
    {

      int32_t sizeX = _in.getSizeX();
      int32_t sliceCount = _in.getSizeZ();

      int32_t x, slice;
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          _out( x, _biasedSlice, slice, _time ) = 
            ( T )( _biasFunctor->getValueAt(
                                      _in( x, _biasedSlice, slice, _time ),
                                      parameter ) );
          _bias( x, _biasedSlice, slice, _time ) = ( float )parameter; 

        }

      }

    }
    else if ( _planeAxis == "z" )
    {

      int32_t sizeX = _in.getSizeX();
      int32_t lineCount = _in.getSizeY();

      int32_t x, line;
      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          _out( x, line, _biasedSlice, _time ) =
            ( T )( _biasFunctor->getValueAt(
                                      _in( x, line, _biasedSlice, _time ),
                                      parameter ) );
          _bias( x, line, _biasedSlice, _time ) = ( float )parameter; 

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void "
             "gkg::BiasNelderMeadFunction2d< T >::"
             "applyBiasCorrection( "
             "double parameter )" );

}


#endif

