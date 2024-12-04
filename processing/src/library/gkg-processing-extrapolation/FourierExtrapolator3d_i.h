#ifndef _gkg_processing_extrapolation_FourierExtrapolator3d_i_h_
#define _gkg_processing_extrapolation_FourierExtrapolator3d_i_h_


#include <gkg-processing-extrapolation/FourierExtrapolator3d.h>
#include <gkg-processing-extrapolation/Extrapolator_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>
#include <numeric>
#include <iomanip>


template < class IN, class MASK, class OUT >
inline
gkg::FourierExtrapolator3d< IN, MASK, OUT >::FourierExtrapolator3d( 
                                                                int32_t degree )
                                         : gkg::Extrapolator< IN, MASK, OUT >(),
                                           _degree( degree )
{
}


template < class IN, class MASK, class OUT >
inline
gkg::FourierExtrapolator3d< IN, MASK, OUT >::~FourierExtrapolator3d()
{
}


template < class IN, class MASK, class OUT >
inline
void gkg::FourierExtrapolator3d< IN, MASK, OUT >::extrapolate(
                                  const gkg::Volume< IN >& input,
                                  const gkg::Volume< MASK >& restrictedMask,
                                  const gkg::Volume< MASK >& enlargedMask,
                                  const gkg::TestFunction< MASK >& testFunction,
                                  gkg::Volume< OUT >& output,
                                  bool verbose ) const
{

  try
  {

    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    int32_t sizeX = input.getSizeX();
    int32_t sizeY = input.getSizeY();
    int32_t sizeZ = input.getSizeZ();
    int32_t sizeT = input.getSizeT();

    // sanity checks
    if ( ( restrictedMask.getSizeX() != sizeX ) || 
         ( restrictedMask.getSizeY() != sizeY ) || 
         ( restrictedMask.getSizeZ() != sizeZ ) || 
         ( restrictedMask.getSizeT() != sizeT ) ||
         ( enlargedMask.getSizeX() != sizeX ) || 
         ( enlargedMask.getSizeY() != sizeY ) || 
         ( enlargedMask.getSizeZ() != sizeZ ) || 
         ( enlargedMask.getSizeT() != sizeT ) )
    {

      throw std::runtime_error( "inconsistent input and mask size(s)" );

    }

    // duplicating input in case output reference is equal to input reference
    gkg::Volume< IN > tmpInput( input );

    // allocating output
    output = gkg::Volume< OUT >( sizeX, sizeY, sizeZ, sizeT );
    output.getHeader() = input.getHeader();
    output.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    // generating basis vectors
    if ( verbose )
    {

      std::cout << "generating basis vectors" << std::flush;

    }
    int32_t count = ( 2 * _degree + 1 ) * ( 2 * _degree + 1 ) * 
                    ( 2 * _degree + 1 );
    std::vector< int32_t > vecX( count );
    std::vector< int32_t > vecY( count );
    std::vector< int32_t > vecZ( count );

    int32_t x, y, z, t, o, d;
    int32_t i = 0;
    for ( z = 0; z <= 2 * _degree; z++ )
    {

      for ( y = 0; y <= 2 * _degree; y++ )
      {

        for ( x = 0; x <= 2 * _degree; x++ )
        {

          vecX[ i ] = x;
          vecY[ i ] = y;
          vecZ[ i ] = z;
          ++ i;

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 24 ) << std::flush;

    }

    // generating basis volumes
    if ( verbose )
    {

      std::cout << "generating basis volumes" << std::flush;

    }
    float onSizeX = 1.0 / ( float )sizeX;
    float onSizeY = 1.0 / ( float )sizeY;
    float onSizeZ = 1.0 / ( float )sizeZ;

    std::vector< gkg::Volume< float > > basisX( 2 * _degree + 1 );
    std::vector< gkg::Volume< float > > basisY( 2 * _degree + 1 );
    std::vector< gkg::Volume< float > > basisZ( 2 * _degree + 1 );
    gkg::Volume< float > temp( sizeX, sizeY, sizeZ );

    for ( o = 1; o <= 3; o++ )
    {

      for ( d = 0; d <= 2 * _degree; d++ )
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              switch ( o )
              {

                case 1:

                  if ( d <= _degree )
                  {

                    temp( x, y, z ) = ( float )std::cos( 2 * M_PI *
                                                    ( double )x * d * onSizeX );

                  }
                  else
                  {

                    temp( x, y, z ) = ( float )std::sin( 2 * M_PI *
                                      ( double )x * ( d - _degree ) * onSizeX );

                  }
                  break;

                case 2:

                  if ( d <= _degree )
                  {

                    temp( x, y, z ) = ( float )std::cos( 2 * M_PI *
                                                    ( double )y * d * onSizeY );

                  }
                  else
                  {

                    temp( x, y, z ) = ( float )std::sin( 2 * M_PI *
                                      ( double )y * ( d - _degree ) * onSizeY );

                  }
                  break;

                case 3:

                  if ( d <= _degree )
                  {

                    temp( x, y, z ) = ( float )std::cos( 2 * M_PI *
                                                    ( double )z * d * onSizeZ );

                  }
                  else
                  {

                    temp( x, y, z ) = ( float )std::sin( 2 * M_PI *
                                      ( double )z * ( d - _degree ) * onSizeZ );

                  }
                  break;

              }

            }   

          }

        }
        switch ( o )
        {

          case 1:

            basisX[ d ] = temp;
            break;

          case 2:

            basisY[ d ] = temp;
            break;

          case 3:

            basisZ[ d ] = temp;
            break;

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 24 ) << std::flush;

    }

    // calculating dot products
    if ( verbose )
    {

      std::cout << "calculating dot products" << std::flush;

    }

    gkg::Vector IB( count );
    gkg::Matrix BB( count, count );
    gkg::Matrix U( count, count );
    gkg::Vector S( count );
    gkg::Matrix V( count, count );
    gkg::Vector Sinv( count );
    gkg::Vector C( count );

    output.fill( ( OUT )0 );

    gkg::Converter< float, OUT > converter;
    OUT outputValue;

    int32_t i1, i2;
    for ( t = 0; t < sizeT; t++ )
    {

      if ( verbose )
      {


        if ( t != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << " rank[ " << std::setw( 4 ) << t + 1 
                  << " / " << std::setw( 4 ) << sizeT
                  << " ]" << std::flush;

      }

      for ( i1 = 0; i1 < count; i1++ )
      {

        if ( verbose )
        {


          if ( i1 != 0 )
          {

            std::cout << gkg::Eraser( 22 );

          }
          std::cout << " order3[ " << std::setw( 4 ) << i1 + 1 
                    << " / " << std::setw( 4 ) << count
                    << " ]" << std::flush;

        }

        typename gkg::Volume< IN >::const_iterator
                              tI = tmpInput.begin() + t * tmpInput.getSizeXYZ();
        gkg::Volume< float >::const_iterator
                                              bX = basisX[ vecX[ i1 ] ].begin();
        gkg::Volume< float >::const_iterator
                                              bY = basisY[ vecY[ i1 ] ].begin();
        gkg::Volume< float >::const_iterator
                                              bZ = basisZ[ vecZ[ i1 ] ].begin();
        typename gkg::Volume< MASK >::const_iterator
                  rM = restrictedMask.begin() + t * restrictedMask.getSizeXYZ();
        gkg::Volume< float >::iterator it = temp.begin(),
                                       ite = temp.end();

        while ( it != ite )
        {

          if ( testFunction.getState( *rM ) )
          {

            *it = ( *tI ) * ( *bX ) * ( *bY ) * ( *bZ );

          }
          ++ it;
          ++ tI;
          ++ bX;
          ++ bY;
          ++ bZ;
          ++ rM;

        }
        IB( i1 ) = std::accumulate( temp.begin(), temp.end(), 0.0 );

        for ( i2 = i1; i2 < count; i2++ )
        {

          gkg::Volume< float >::const_iterator
                                             bX1 = basisX[ vecX[ i1 ] ].begin();
          gkg::Volume< float >::const_iterator
                                             bY1 = basisY[ vecY[ i1 ] ].begin();
          gkg::Volume< float >::const_iterator
                                             bZ1 = basisZ[ vecZ[ i1 ] ].begin();
          gkg::Volume< float >::const_iterator
                                             bX2 = basisX[ vecX[ i2 ] ].begin();
          gkg::Volume< float >::const_iterator
                                             bY2 = basisY[ vecY[ i2 ] ].begin();
          gkg::Volume< float >::const_iterator
                                             bZ2 = basisZ[ vecZ[ i2 ] ].begin();
          rM = restrictedMask.begin() + t * restrictedMask.getSizeXYZ();
          it = temp.begin();

          while ( it != ite )
          {

            if ( testFunction.getState( *rM ) )
            {

              *it = ( *bX1 ) * ( *bX2 ) *
                    ( *bY1 ) * ( *bY2 ) *
                    ( *bZ1 ) * ( *bZ2 );

            }
            ++ it;
            ++ bX1;
            ++ bY1;
            ++ bZ1;
            ++ bX2;
            ++ bY2;
            ++ bZ2;
            ++ rM;

          }
          BB( i1, i2 ) = std::accumulate( temp.begin(), temp.end(), 0.0 );
          BB( i2, i1 ) = BB( i1, i2 );

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 22 );

      }

      factory->getSingularValueDecomposition( BB, V, S );
      U = BB;
      float threshold = 1e-12 * factory->getAbsMaximumItem( S );
      for ( i = 0; i < count; i++ )
      {

        Sinv( i ) = ( std::fabs( S( i ) ) > threshold ? 1.0 / S( i ) : 0.0 ); 

      }
      C = V.getComposition(
            Sinv.getDiagonal().getComposition(
              U.getTransposition().getComposition( IB ) ) );

      for ( i = 0; i < count; i++ )
      {

        gkg::Volume< float >::const_iterator bX = basisX[ vecX[ i ] ].begin(),
                                             bXe = basisX[ vecX[ i ] ].end();
        gkg::Volume< float >::const_iterator bY = basisY[ vecY[ i ] ].begin();
        gkg::Volume< float >::const_iterator bZ = basisZ[ vecZ[ i ] ].begin();
        typename gkg::Volume< OUT >::iterator io = output.begin() +
                                                  t * output.getSizeXYZ();
        typename gkg::Volume< MASK >::const_iterator
                      eM = enlargedMask.begin() + t * enlargedMask.getSizeXYZ();

        float c = ( float )C( i );

        while ( bX != bXe )
        {

          if ( *eM )
          {

            converter.convert( ( *bX ) * ( *bY ) * ( *bZ ) * c,
                               outputValue );
            *io += outputValue;

          }
          ++ io;
          ++ bX;
          ++ bY;
          ++ bZ;
          ++ eM;

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 44 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "void gkg::FourierExtrapolator3d< IN, MASK, OUT >::extrapolate( "
             "const gkg::Volume< IN >& input, "
             "const gkg::Volume< MASK >& restrictedMask, "
             "const gkg::Volume< MASK >& enlargedMask, "
             "const gkg::TestFunction< MASK >& testFunction, "
             "gkg::Volume< OUT >& ouput, "
             "bool verbose ) const" );

}


#endif
