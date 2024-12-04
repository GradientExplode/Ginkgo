#ifndef _gkg_processing_algobase_MinMaxFilterWithMask_i_h_
#define _gkg_processing_algobase_MinMaxFilterWithMask_i_h_


#include <gkg-processing-algobase/MinMaxFilterWithMask.h>
#include <gkg-processing-algobase/VolumeMinMaxFilterWithMaskContext_i.h>
#include <gkg-processing-algobase/FilterWithMaskFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class MinMaxFilterWithMask< IN,
//                             MASK,
//                             OUT >
//

template < class IN, class MASK, class OUT >
inline
void gkg::MinMaxFilterWithMask< IN,
                                MASK,
                                OUT >::filter(
                                  const IN& in,
                                  const MASK& mask,
                                  const gkg::TestFunction< MASK >& testFunction,
                                  std::pair< OUT,
                                             OUT >& out ) const
{

  try
  {

    if ( testFunction.getState( mask ) )
    {

      gkg::Converter< IN, OUT > converter;
      converter.convert( in, out.first );
      converter.convert( in, out.second );

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "void gkg::MinMaxFilterWithMask< IN, "
             "MASK, "
             "OUT >::filter( const IN& in, "
             "const MASK& mask, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxFilterWithMask< std::vector< IN >,
//                             MASK,
//                             OUT >
//

template < class IN, class MASK, class OUT >
inline
void gkg::MinMaxFilterWithMask< std::vector< IN >,
                                MASK,
                                OUT >::filter(
                                  const std::vector< IN >& in,
                                  const std::vector< MASK >& mask,
                                  const gkg::TestFunction< MASK >& testFunction,
                                  std::pair< OUT,
                                             OUT >& out ) const
{

  try
  {

    if ( in.empty() )
    {

      throw std::runtime_error( "empty vector" );

    }

    if ( in.size() != mask.size() )
    {


      throw std::runtime_error( "incoherent input and mask size(s)" );

    }

    typename std::vector< IN >::const_iterator i = in.begin(),
                                               ie = in.end();
    typename std::vector< MASK >::const_iterator m = mask.begin();
    IN minimum = *i;
    IN maximum = *i;
    while ( i != ie )
    {

      if ( testFunction.getState( *m ) )
      {

        if ( *i < minimum )
        {

          minimum = *i;

        }
        else if ( *i > maximum )
        {

          maximum = *i;

        }

      }
      ++ m;
      ++ i;

    }
    gkg::Converter< IN, OUT > converter;
    converter.convert( minimum, out.first );
    converter.convert( maximum, out.second );

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "void gkg::MinMaxFilterWithMask< std::vector< IN >, "
             "MASK, "
             "OUT >::filter( "
             "const std::vector< IN >& in, "
             "const std::vector< MASK >& mask, "
             "const gkg::TestFunction< MASK >& testFunction, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxFilterWithMask< Vector,
//                             MASK,
//                             OUT >
//

template < class MASK, class OUT >
inline
void gkg::MinMaxFilterWithMask< gkg::Vector,
                                MASK,
                                OUT >::filter(
                                  const gkg::Vector& in,
                                  const std::vector< MASK >& mask,
                                  const gkg::TestFunction< MASK >& testFunction,
                                  std::pair< OUT,
                                             OUT >& out ) const
{

  try
  {

    if ( in.getSize() != ( int32_t )mask.size() )
    {


      throw std::runtime_error( "incoherent input and mask size(s)" );

    }


    int32_t i = 0,
            ie = in.getSize();
    typename std::vector< MASK >::const_iterator m = mask.begin();
    double minimum = in( 0 );
    double maximum = in( 0 );
    while ( i != ie )
    {

      if ( testFunction.getState( *m ) )
      {

        if ( in( i ) < minimum )
        {

          minimum = in( i );

        }
        else if ( in( i ) > maximum )
        {

          maximum = in( i );

        }

      }
      ++ i;
      ++ m;

    }
    gkg::Converter< double, OUT > converter;
    converter.convert( minimum, out.first );
    converter.convert( maximum, out.second );

  }
  GKG_CATCH( "template < class MASK, class OUT > "
             "inline "
             "void gkg::MinMaxFilterWithMask< gkg::Vector, "
             "MASK, "
             "OUT >::filter( "
             "const gkg::Vector& in, "
             "const std::vector< MASK >& mask, "
             "const gkg::TestFunction< MASK >& testFunction, "
             "std::pair< OUT, OUT >& out ) const" );

}


//
// class MinMaxFilterWithMask< Volume< IN >,
//                             MASK,
//                             OUT >
//

template < class IN, class MASK, class OUT >
inline
void gkg::MinMaxFilterWithMask< gkg::Volume< IN >,
                                MASK,
                                OUT >::filter(
                                  const gkg::Volume< IN >& in,
                                  const gkg::Volume< MASK >& mask,
                                  const gkg::TestFunction< MASK >& testFunction,
                                  std::pair< OUT,
                                             OUT >& out ) const
{

  try
  {

    gkg::Vector3d< int32_t > inSize;
    gkg::Vector3d< double > inResolution;

    gkg::Vector3d< int32_t > maskSize;
    gkg::Vector3d< double > maskResolution;

    in.getSize( inSize );
    in.getResolution( inResolution );

    mask.getSize( maskSize );
    mask.getResolution( maskResolution );

    if ( ( inSize.x != maskSize.x ) ||
         ( inSize.y != maskSize.y ) ||
         ( inSize.z != maskSize.z ) ||
         ( std::fabs( inResolution.x - maskResolution.x ) >= 1e-3 ) ||
         ( std::fabs( inResolution.y - maskResolution.y ) >= 1e-3 ) ||
         ( std::fabs( inResolution.z - maskResolution.z ) >= 1e-3 ) )
    {

      throw std::runtime_error(
                 "input volume and mask have incoherent sizes or resolutions" );

    }

    IN minimum = in( 0 );
    IN maximum = minimum;

    if ( in.isMemoryMapped() )
    {

      int32_t sizeX = in.getSizeX();
      int32_t sizeY = in.getSizeY();
      int32_t sizeZ = in.getSizeZ();
      int32_t sizeT = in.getSizeT();
      int32_t x, y, z, t;

      for ( t = 0; t < sizeT; t++ )
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( testFunction.getState( mask( x, y, z ) ) )
              {

                IN value = in( x, y, z, t );

                if ( value < minimum )
                {

                  minimum = value;

                }
                else if ( value > maximum )
                {

                  maximum = value;

                }

              }

            }

          }

        }

      }

    }
    else
    {

      gkg::VolumeMinMaxFilterWithMaskContext< IN, MASK >
        context( in, mask, testFunction, minimum, maximum );
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &context, 0, in.getSizeXYZT() );

      threadedLoop.launch();

    }

    gkg::Converter< IN, OUT > converter;
    converter.convert( minimum, out.first );
    converter.convert( maximum, out.second );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::MinMaxFilterWithMask< gkg::Volume< IN >, OUT >::filter( "
             "const gkg::Volume< IN >& in, "
             "std::pair< OUT, OUT >& out ) const" );

}


#endif
