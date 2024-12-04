#ifndef _gkg_processing_bias_BiasCorrectionPoly2d_i_h_
#define _gkg_processing_bias_BiasCorrectionPoly2d_i_h_


#include <gkg-processing-bias/BiasCorrectionPoly2d.h>
#include <gkg-processing-bias/BiasCorrection_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/Polynomial2dFitter.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>
#include <map>

//
// class BiasCorrectionPoly2d< T >
//

// scalarParameters[ 0 ] -> maximum polynom fit order
// scalarParameters[ 1 ] -> maximum bias polynom order

// stringParameters[ 0 ] -> plane axis x / y / z


template < class T >
inline
gkg::BiasCorrectionPoly2d< T >::BiasCorrectionPoly2d(
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

      theScalarParameters.resize( 2U );
      theScalarParameters[ 0 ] = 8;
      theScalarParameters[ 1 ] = 8;


      theStringParameters.resize( 1U );
      theStringParameters[ 1 ] = "z";

    }
    else
    {

      theScalarParameters = scalarParameters;
      theStringParameters = stringParameters;

    }

    //
    // dealing with string parameters
    //

    if ( theStringParameters.size() != 1U )
    {

      throw std::runtime_error( "bad number of string parameters" );

    }

    _planeAxis = theStringParameters[ 0 ];
    if ( ( _planeAxis != "x" ) &&
         ( _planeAxis != "y" ) &&
         ( _planeAxis != "z" ) )
    {

      throw std::runtime_error( "bad plane axis" );

    }    

    //
    // dealing with scalar parameters
    //

    if ( theScalarParameters.size() != 2U )
    {

      throw std::runtime_error( "bad number of scalar parameters" );

    }

    if ( theScalarParameters[ 0 ] <= 0 )
    {

      throw std::runtime_error( "maximum fit order must be positive" );

    }
    else
    {

      _maximumFitOrder = ( int32_t )( theScalarParameters[ 0 ] + 0.5 );

    }

    if ( theScalarParameters[ 1 ] <= 0 )
    {

      throw std::runtime_error( "maximum bias order must be positive" );

    }
    else
    {

      _maximumBiasOrder = ( int32_t )( theScalarParameters[ 1 ] + 0.5 );

    }

  }
  GKG_CATCH( "gkg::BiasCorrectionPoly2d< T >::BiasCorrectionPoly2d( "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters )" );

}


template < class T >
inline
gkg::BiasCorrectionPoly2d< T >::~BiasCorrectionPoly2d()
{
}


template < class T >
std::string gkg::BiasCorrectionPoly2d< T >::getName() const
{

  return getStaticName();

}


template < class T >
std::string gkg::BiasCorrectionPoly2d< T >::getStaticName()
{

  return "bias-correction-poly-2d";

}


template < class T >
inline
void gkg::BiasCorrectionPoly2d< T >::correct( 
                                          const gkg::Volume< T >& in,
                                          const gkg::Volume< int16_t >& mask,
                                          const gkg::Volume< float >& weight,
                                          gkg::Volume< T >& out,
                                          gkg::Volume< float >& bias,
                                          bool verbose ) const
{

  try
  {

    // collecting size(s)
    int32_t sizeX = in.getSizeX();
    int32_t sizeY = in.getSizeY();
    int32_t sizeZ = in.getSizeZ();
    int32_t sizeT = in.getSizeT();

    // allocating bias
    gkg::Vector3d< double > resolution;
    in.getResolution( resolution );
    bias.reallocate( sizeX, sizeY, sizeZ, sizeT );
    bias.setResolution( resolution );
    bias.fill( 0.0f );

    // allocating output
    out.reallocate( sizeX, sizeY, sizeZ, sizeT );
    out.setResolution( resolution );
    out.fill( T( 0 ) );

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

    // looping over times
    int32_t slice, t;
    for ( t = 0; t < sizeT; t++ )
    {

      if ( verbose )
      {

        if ( t != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << " time[ " << std::setw( 4 ) << t + 1
                  << " / " << std::setw( 4 ) << sizeT
                  << " ]" << std::flush;

      }

      // correcting bias of each slice
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        if ( verbose )
        {

          if ( slice )
          {

            std::cout << gkg::Eraser( 21 );

          }
          std::cout << " slice[ " << std::setw( 4 ) << slice + 1
                    << " / " << std::setw( 4 ) << sliceCount
                    << " ]" << std::flush;

        }

        if ( _planeAxis == "x" )
        {

          // computing site count
          int32_t y, z;
          int32_t siteCount = 0;
          for ( z = 0; z < sizeZ; z++ )
          {

            for ( y = 0; y < sizeY; y++ )
            {

              if ( mask( slice, y, z ) )
              {

                ++ siteCount;

              }

            }

          }

          // allocating polynom structures
          gkg::Vector X( siteCount );
          gkg::Vector Y( siteCount );
          gkg::Vector W( siteCount );
          gkg::Vector F( siteCount );

          // filling structures
          int32_t n = 0;
          for ( z = 0; z < sizeZ; z++ )
          {

            for ( y = 0; y < sizeY; y++ )
            {

              if ( mask( slice, y, z ) )
              {

                X( n ) = ( double )y / ( double )sizeY;
                Y( n ) = ( double )z / ( double )sizeZ;
                W( n ) = ( double )weight( slice, y, z );
                F( n ) = ( double )in( slice, y, z, t );
                ++ n;

              }

            }

          }

          // fitting polynomial
          gkg::Polynomial2dFitter 
            polynomial2dFitter( X, Y, W, F, _maximumFitOrder );
          float biasValue;
          for ( z = 0; z < sizeZ; z++ )
          {

            for ( y = 0; y < sizeY; y++ )
            {

              if ( mask( slice, y, z ) )
              {

                biasValue = polynomial2dFitter.getValueAt(
                                                  ( double )y / ( double )sizeY,
                                                  ( double )z / ( double )sizeZ,
                                                  _maximumBiasOrder );
                bias( slice, y, z, t ) = biasValue;
                out( slice, y, z, t ) = in( slice, y, z, t ) - ( T )biasValue;

              }
              else
              {

                out( slice, y, z, t ) = ( T )0.0;

              }

            }

          }

        }
        else if ( _planeAxis == "y" )
        {

          // computing site count
          int32_t x, z;
          int32_t siteCount = 0;
          for ( z = 0; z < sizeZ; z++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( mask( x, slice, z ) )
              {

                ++ siteCount;

              }

            }

          }

          // allocating polynom structures
          gkg::Vector X( siteCount );
          gkg::Vector Y( siteCount );
          gkg::Vector W( siteCount );
          gkg::Vector F( siteCount );

          // filling structures
          int32_t n = 0;
          for ( z = 0; z < sizeZ; z++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( mask( x, slice, z ) )
              {

                X( n ) = ( double )x / ( double )sizeX;
                Y( n ) = ( double )z / ( double )sizeZ;
                W( n ) = ( double )weight( x, slice, z );
                F( n ) = ( double )in( x, slice, z, t );
                ++ n;

              }

            }

          }

          // fitting polynomial
          gkg::Polynomial2dFitter 
            polynomial2dFitter( X, Y, W, F, _maximumFitOrder );
          float biasValue;
          for ( z = 0; z < sizeZ; z++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( mask( x, slice, z ) )
              {

                biasValue = polynomial2dFitter.getValueAt(
                                                  ( double )x / ( double )sizeX,
                                                  ( double )z / ( double )sizeZ,
                                                  _maximumBiasOrder );
                bias( x, slice, z, t ) = biasValue;
                out( x, slice, z, t ) = in( x, slice, z, t ) - ( T )biasValue;

              }
              else
              {

                out( x, slice, z, t ) = ( T )0.0;

              }

            }

          }

        }
        else if ( _planeAxis == "z" )
        {

          // computing site count
          int32_t x, y;
          int32_t siteCount = 0;
          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( mask( x, y, slice ) )
              {

                ++ siteCount;

              }

            }

          }

          // allocating polynom structures
          gkg::Vector X( siteCount );
          gkg::Vector Y( siteCount );
          gkg::Vector W( siteCount );
          gkg::Vector F( siteCount );

          // filling structures
          int32_t n = 0;
          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( mask( x, y, slice ) )
              {

                X( n ) = ( double )x / ( double )sizeX;
                Y( n ) = ( double )y / ( double )sizeY;
                W( n ) = ( double )weight( x, y, slice );
                F( n ) = ( double )in( x, y, slice, t );
                ++ n;

              }

            }

          }

          // fitting polynomial
          gkg::Polynomial2dFitter 
            polynomial2dFitter( X, Y, W, F, _maximumFitOrder );
          float biasValue;
          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( mask( x, y, slice ) )
              {

                biasValue = polynomial2dFitter.getValueAt(
                                                  ( double )x / ( double )sizeX,
                                                  ( double )y / ( double )sizeY,
                                                  -1/*_maximumBiasOrder + 1*/ );
                bias( x, y, slice, t ) = biasValue;
                out( x, y, slice, t ) = in( x, y, slice, t ) - ( T )biasValue;

              }
              else
              {

                out( x, y, slice, t ) = ( T )0.0;

              }

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 41 );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::BiasCorrectionPoly2d< T >::correct( "
             "const gkg::Volume< T >& in, "
             "const gkg::Volume< int16_t >& mask, "
             "const gkg::Volume< float >& weight, "
             "gkg::Volume< T >& out, "
             "gkg::Volume< float >& bias, "
             "bool verbose ) const" );

}


#endif


