#include <gkg-processing-signal/DericheGaussianFilter3d.h>
#include <gkg-processing-signal/RecursiveDericheGaussianFilter.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>
#include <vector>


template < class T >
gkg::DericheGaussianFilter3d< T >::DericheGaussianFilter3d()
{
}


template < class T >
gkg::DericheGaussianFilter3d< T >::~DericheGaussianFilter3d()
{
}


template < class T > template < class U >
void gkg::DericheGaussianFilter3d< T >::filter(
                      const gkg::Volume< U >& inVolume,
                      const gkg::DericheGaussianCoefficients< T >& coefficients,
                      gkg::DericheGaussianFilter3d< T >::Axis axis,
                      gkg::Volume< U >& outVolume ) const
{

  try
  {

    // converting U volume to T volume
    gkg::Vector3d< double > resolution;
    double resolutionT;
    gkg::Converter< gkg::Volume< U >, gkg::Volume< T > > converter_U_to_T;
    gkg::Volume< T > temporary;

    converter_U_to_T.convert( inVolume, temporary );
    inVolume.getResolution( resolution, resolutionT );

    this->floatingFilter( inVolume, coefficients, axis, temporary );

    // reallocating output volume
    outVolume.reallocate( inVolume.getSizeX(),
                          inVolume.getSizeY(),
                          inVolume.getSizeZ(),
                          inVolume.getSizeT() );
    outVolume.setResolution( resolution, resolutionT );

    // converting back to U type

    gkg::Converter< T, U > converter_T_to_U;
    typename gkg::Volume< T >::const_iterator t = temporary.begin(),
                                              te = temporary.end();
    typename gkg::Volume< U >::iterator u = outVolume.begin();
    while ( t != te )
    {

      converter_T_to_U.convert( *t, *u );
      ++ t;
      ++ u;

    }

  }
  GKG_CATCH( "template < class T > template < class U > "
             "void gkg::DericheGaussianFilter3d< T >::filter( "
             "const gkg::Volume< U >& inVolume, "
             "const gkg::DericheGaussianCoefficients< T >& coefficients, "
             "gkg::DericheGaussianFilter3d< T >::Axis axis, "
             "gkg::Volume< U >& outVolume ) const" );

}


template < class T > template < class U >
void gkg::DericheGaussianFilter3d< T >::floatingFilter(
                      const gkg::Volume< U >& inVolume,
                      const gkg::DericheGaussianCoefficients< T >& coefficients,
                      gkg::DericheGaussianFilter3d< T >::Axis axis,
                      gkg::Volume< T >& outVolume ) const
{

  try
  {

    // converting U volume to T volume
    gkg::Vector3d< double > resolution;
    double resolutionT;
    gkg::Converter< gkg::Volume< U >, gkg::Volume< T > > converter_U_to_T;
    converter_U_to_T.convert( inVolume, outVolume );
    inVolume.getResolution( resolution, resolutionT );

    gkg::RecursiveDericheGaussianFilter< T >&
      recursiveFilter = gkg::RecursiveDericheGaussianFilter< T >::getInstance();

    // filtering according to given axis
    switch ( axis )
    {

      case gkg::DericheGaussianFilter3d< T >::X_AXIS:

        {

          std::vector< T > in( outVolume.getSizeX() );
          std::vector< T > out( outVolume.getSizeX() );
          int32_t x, y, z, t;
          for ( t = 0; t < outVolume.getSizeT(); t++ )
          {

            for ( z = 0; z < outVolume.getSizeZ(); z++ )
            {

              for ( y = 0; y < outVolume.getSizeY(); y++ )
              {

                for ( x = 0; x < outVolume.getSizeX(); x++ )
                {

                  in[ x ] = outVolume( x, y, z, t );

                }

                recursiveFilter.recurse( coefficients, in, out );

                for ( x = 0; x < outVolume.getSizeX(); x++ )
                {

                  outVolume( x, y, z, t ) = out[ x ];

                }

              }

            }

          }

        }
        break;

      case gkg::DericheGaussianFilter3d< T >::Y_AXIS:

        {

          std::vector< T > in( outVolume.getSizeY() );
          std::vector< T > out( outVolume.getSizeY() );
          int32_t x, y, z, t;
          for ( t = 0; t < outVolume.getSizeT(); t++ )
          {

            for ( z = 0; z < outVolume.getSizeZ(); z++ )
            {

              for ( x = 0; x < outVolume.getSizeX(); x++ )
              {

                for ( y = 0; y < outVolume.getSizeY(); y++ )
                {

                  in[ y ] = outVolume( x, y, z, t );

                }

                recursiveFilter.recurse( coefficients, in, out );

                for ( y = 0; y < outVolume.getSizeY(); y++ )
                {

                  outVolume( x, y, z, t ) = out[ y ];

                }

              }

            }

          }

        }
        break;

      case gkg::DericheGaussianFilter3d< T >::Z_AXIS:

        {

          std::vector< T > in( outVolume.getSizeZ() );
          std::vector< T > out( outVolume.getSizeZ() );
          int32_t x, y, z, t;
          for ( t = 0; t < outVolume.getSizeT(); t++ )
          {

            for ( y = 0; y < outVolume.getSizeY(); y++ )
            {

              for ( x = 0; x < outVolume.getSizeX(); x++ )
              {

                for ( z = 0; z < outVolume.getSizeZ(); z++ )
                {

                  in[ z ] = outVolume( x, y, z, t );

                }

                recursiveFilter.recurse( coefficients, in, out );

                for ( z = 0; z < outVolume.getSizeZ(); z++ )
                {

                  outVolume( x, y, z, t ) = out[ z ];

                }

              }

            }

          }

        }
        break;

    }

  }
  GKG_CATCH( "template < class T > template < class U > "
             "void gkg::DericheGaussianFilter3d< T >::floatingFilter( "
             "const gkg::Volume< U >& inVolume, "
             "const gkg::DericheGaussianCoefficients< T >& coefficients, "
             "gkg::DericheGaussianFilter3d< T >::Axis axis, "
             "gkg::Volume< T >& outVolume ) const" );

}


template < class T >
void gkg::DericheGaussianFilter3d< T >::filter(
                      const gkg::Matrix& inMatrix,
                      const gkg::DericheGaussianCoefficients< T >& coefficients,
                      gkg::DericheGaussianFilter3d< T >::Axis axis,
                      gkg::Matrix& outMatrix ) const
{

  try
  {

    gkg::RecursiveDericheGaussianFilter< T >&
      recursiveFilter = gkg::RecursiveDericheGaussianFilter< T >::getInstance();

    int32_t size1 = inMatrix.getSize1();
    int32_t size2 = inMatrix.getSize2();

    // reallocating output matrix
    outMatrix.reallocate( size1, size2 );

    // filtering according to given axis
    switch ( axis )
    {

      case gkg::DericheGaussianFilter3d< T >::X_AXIS:

        {

          std::vector< T > in( size1 );
          std::vector< T > out( size1 );
          int32_t s1, s2;
          for ( s2 = 0; s2 < size2; s2++ )
          {

            for ( s1 = 0; s1 < size1; s1++ )
            {

              in[ s1 ] = ( T )inMatrix( s1, s2 );

            }

            recursiveFilter.recurse( coefficients, in, out );

            for ( s1 = 0; s1 < size1; s1++ )
            {

              outMatrix( s1, s2 ) = ( double )out[ s1 ];

            }

          }

        }
        break;

      case gkg::DericheGaussianFilter3d< T >::Y_AXIS:

        {

          std::vector< T > in( size2 );
          std::vector< T > out( size2 );
          int32_t s1, s2;
          for ( s1 = 0; s1 < size1; s1++ )
          {

            for ( s2 = 0; s2 < size2; s2++ )
            {

              in[ s2 ] = ( T )inMatrix( s1, s2 );

            }

            recursiveFilter.recurse( coefficients, in, out );

            for ( s2 = 0; s2 < size2; s2++ )
            {

              outMatrix( s1, s2 ) = ( double )out[ s2 ];

            }

          }

        }
        break;


      case gkg::DericheGaussianFilter3d< T >::Z_AXIS:

        {

          throw std::runtime_error( "Z axis filtering impossible on a matrix" );

        }
        break;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DericheGaussianFilter3d< T >::filter( "
             "const gkg::Matrix& inMatrix, "
             "const gkg::DericheGaussianCoefficients< T >& coefficients, "
             "gkg::DericheGaussianFilter3d< T >::Axis axis, "
             "gkg::Matrix& outMatrix ) const" );

}


namespace gkg
{

#if  ( __GNUC__-0 >= 4 )
template <> 
#endif
template <>
void DericheGaussianFilter3d< float >::filter< float >(
                       const Volume< float >& inVolume,
                       const DericheGaussianCoefficients< float >& coefficients,
                       DericheGaussianFilter3d< float >::Axis axis,
                       Volume< float >& outVolume ) const
{

  try
  {

    gkg::Vector3d< double > resolution;
    double resolutionT;
    RecursiveDericheGaussianFilter< float >& recursiveFilter =
      RecursiveDericheGaussianFilter< float >::getInstance();

    // reallocating output volume
    inVolume.getResolution( resolution, resolutionT );
    outVolume.reallocate( inVolume.getSizeX(),
                          inVolume.getSizeY(),
                          inVolume.getSizeZ(),
                          inVolume.getSizeT() );
    outVolume.setResolution( resolution, resolutionT );

    // filtering according to given axis
    switch ( axis )
    {

      case DericheGaussianFilter3d< float >::X_AXIS:

        {

          std::vector< float > in( inVolume.getSizeX() );
          std::vector< float > out( inVolume.getSizeX() );
          int32_t x, y, z, t;
          for ( t = 0; t < inVolume.getSizeT(); t++ )
          {

            for ( z = 0; z < inVolume.getSizeZ(); z++ )
            {

              for ( y = 0; y < inVolume.getSizeY(); y++ )
              {

                for ( x = 0; x < inVolume.getSizeX(); x++ )
                {

                  in[ x ] = inVolume( x, y, z, t );

                }

                recursiveFilter.recurse( coefficients, in, out );

                for ( x = 0; x < inVolume.getSizeX(); x++ )
                {

                  outVolume( x, y, z, t ) = out[ x ];

                }

              }

            }

          }

        }
        break;

      case DericheGaussianFilter3d< float >::Y_AXIS:

        {

          std::vector< float > in( inVolume.getSizeY() );
          std::vector< float > out( inVolume.getSizeY() );
          int32_t x, y, z, t;
          for ( t = 0; t < inVolume.getSizeT(); t++ )
          {

            for ( z = 0; z < inVolume.getSizeZ(); z++ )
            {

              for ( x = 0; x < inVolume.getSizeX(); x++ )
              {

                for ( y = 0; y < inVolume.getSizeY(); y++ )
                {

                  in[ y ] = inVolume( x, y, z, t );

                }

                recursiveFilter.recurse( coefficients, in, out );

                for ( y = 0; y < inVolume.getSizeY(); y++ )
                {

                  outVolume( x, y, z, t ) = out[ y ];

                }

              }

            }

          }

        }
        break;

      case DericheGaussianFilter3d< float >::Z_AXIS:

        {

          std::vector< float > in( inVolume.getSizeZ() );
          std::vector< float > out( inVolume.getSizeZ() );
          int32_t x, y, z, t;
          for ( t = 0; t < inVolume.getSizeT(); t++ )
          {

            for ( y = 0; y < inVolume.getSizeY(); y++ )
            {

              for ( x = 0; x < inVolume.getSizeX(); x++ )
              {

                for ( z = 0; z < inVolume.getSizeZ(); z++ )
                {

                  in[ z ] = inVolume( x, y, z, t );

                }

                recursiveFilter.recurse( coefficients, in, out );

                for ( z = 0; z < inVolume.getSizeZ(); z++ )
                {

                  outVolume( x, y, z, t ) = out[ z ];

                }

              }

            }

          }

        }
        break;

    }

  }
  GKG_CATCH( "void gkg::DericheGaussianFilter3d< float >::filter( "
             "const gkg::Volume< float >& inVolume, "
             "const gkg::DericheGaussianCoefficients< float >& coefficients, "
             "gkg::DericheGaussianFilter3d< float >::Axis axis, "
             "gkg::Volume< float >& outVolume ) const" );

}


#if  ( __GNUC__-0 >= 4 )
template <> 
#endif
template <>
void DericheGaussianFilter3d< double >::filter< double >(
                      const Volume< double >& inVolume,
                      const DericheGaussianCoefficients< double >& coefficients,
                      DericheGaussianFilter3d< double >::Axis axis,
                      Volume< double >& outVolume ) const
{

  try
  {

    gkg::Vector3d< double > resolution;
    double resolutionT;
    RecursiveDericheGaussianFilter< double >& recursiveFilter =
      RecursiveDericheGaussianFilter< double >::getInstance();

    // reallocating output volume
    inVolume.getResolution( resolution, resolutionT );
    outVolume.reallocate( inVolume.getSizeX(),
                          inVolume.getSizeY(),
                          inVolume.getSizeZ(),
                          inVolume.getSizeT() );
    outVolume.setResolution( resolution, resolutionT );

    // filtering according to given axis
    switch ( axis )
    {

      case DericheGaussianFilter3d< double >::X_AXIS:

        {

          std::vector< double > in( inVolume.getSizeX() );
          std::vector< double > out( inVolume.getSizeX() );
          int32_t x, y, z, t;
          for ( t = 0; t < inVolume.getSizeT(); t++ )
          {

            for ( z = 0; z < inVolume.getSizeZ(); z++ )
            {

              for ( y = 0; y < inVolume.getSizeY(); y++ )
              {

                for ( x = 0; x < inVolume.getSizeX(); x++ )
                {

                  in[ x ] = inVolume( x, y, z, t );

                }

                recursiveFilter.recurse( coefficients, in, out );

                for ( x = 0; x < inVolume.getSizeX(); x++ )
                {

                  outVolume( x, y, z, t ) = out[ x ];

                }

              }

            }

          }

        }
        break;

      case DericheGaussianFilter3d< double >::Y_AXIS:

        {

          std::vector< double > in( inVolume.getSizeY() );
          std::vector< double > out( inVolume.getSizeY() );
          int32_t x, y, z, t;
          for ( t = 0; t < inVolume.getSizeT(); t++ )
          {

            for ( z = 0; z < inVolume.getSizeZ(); z++ )
            {

              for ( x = 0; x < inVolume.getSizeX(); x++ )
              {

                for ( y = 0; y < inVolume.getSizeY(); y++ )
                {

                  in[ y ] = inVolume( x, y, z, t );

                }

                recursiveFilter.recurse( coefficients, in, out );

                for ( y = 0; y < inVolume.getSizeY(); y++ )
                {

                  outVolume( x, y, z, t ) = out[ y ];

                }

              }

            }

          }

        }
        break;

      case DericheGaussianFilter3d< double >::Z_AXIS:

        {

          std::vector< double > in( inVolume.getSizeZ() );
          std::vector< double > out( inVolume.getSizeZ() );
          int32_t x, y, z, t;
          for ( t = 0; t < inVolume.getSizeT(); t++ )
          {

            for ( y = 0; y < inVolume.getSizeY(); y++ )
            {

              for ( x = 0; x < inVolume.getSizeX(); x++ )
              {

                for ( z = 0; z < inVolume.getSizeZ(); z++ )
                {

                  in[ z ] = inVolume( x, y, z, t );

                }

                recursiveFilter.recurse( coefficients, in, out );

                for ( z = 0; z < inVolume.getSizeZ(); z++ )
                {

                  outVolume( x, y, z, t ) = out[ z ];

                }

              }

            }

          }

        }
        break;

    }

  }
  GKG_CATCH( "void gkg::DericheGaussianFilter3d< double >::filter( "
             "const gkg::Volume< double >& inVolume, "
             "const gkg::DericheGaussianCoefficients< double >& coefficients, "
             "gkg::DericheGaussianFilter3d< double >::Axis axis, "
             "gkg::Volume< double >& outVolume ) const" );

}


}


template class gkg::DericheGaussianFilter3d< float >;
template class gkg::DericheGaussianFilter3d< double >;


#define InstanciateDericheGaussianFilter3d( T, U )                            \
template void gkg::DericheGaussianFilter3d< T >::filter(                      \
                   const gkg::Volume< U >& inVolume,                          \
                   const gkg::DericheGaussianCoefficients< T >& coefficients, \
                   gkg::DericheGaussianFilter3d< T >::Axis axis,              \
                   gkg::Volume< U >& outVolume ) const;


InstanciateDericheGaussianFilter3d( float, int8_t );
InstanciateDericheGaussianFilter3d( float, uint8_t );
InstanciateDericheGaussianFilter3d( float, int16_t );
InstanciateDericheGaussianFilter3d( float, uint16_t );
InstanciateDericheGaussianFilter3d( float, int32_t );
InstanciateDericheGaussianFilter3d( float, uint32_t );
InstanciateDericheGaussianFilter3d( float, int64_t );
InstanciateDericheGaussianFilter3d( float, uint64_t );
InstanciateDericheGaussianFilter3d( float, float );
InstanciateDericheGaussianFilter3d( float, double );

InstanciateDericheGaussianFilter3d( double, int8_t );
InstanciateDericheGaussianFilter3d( double, uint8_t );
InstanciateDericheGaussianFilter3d( double, int16_t );
InstanciateDericheGaussianFilter3d( double, uint16_t );
InstanciateDericheGaussianFilter3d( double, int32_t );
InstanciateDericheGaussianFilter3d( double, uint32_t );
InstanciateDericheGaussianFilter3d( double, int64_t );
InstanciateDericheGaussianFilter3d( double, uint64_t );
InstanciateDericheGaussianFilter3d( double, float );
InstanciateDericheGaussianFilter3d( double, double );


#undef InstanciateDericheGaussianFilter3d
