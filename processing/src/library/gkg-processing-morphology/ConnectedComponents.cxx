#include <gkg-processing-morphology/ConnectedComponents.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>
#include <queue>
#include <iostream>


template < class IN, class OUT >
gkg::ConnectedComponents< IN, OUT >::ConnectedComponents(
                                    const gkg::TestFunction< IN >& testFunction,
                                    const OUT& background,
                                    size_t minimumComponentSize,
                                    size_t maximumComponentCount,
                                    bool binary,
                                    gkg::Neighborhood3d::Type neighborhoodType,
                                    int32_t stride )
                         : _testFunction( testFunction ),
                           _background( background ),
                           _minimumComponentSize( minimumComponentSize ),
                           _maximumComponentCount( maximumComponentCount ),
                           _stride( stride ),
                           _binary( binary )
{

  try
  {

    _neighborOffsets = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                                             neighborhoodType,
                                                             stride );


  }
  GKG_CATCH( "template < class IN, class OUT > "
             "gkg::ConnectedComponents< IN, OUT >::ConnectedComponents( "
             "const gkg::TestFunction< IN >& testFunction, "
             "const OUT& background, "
             "size_t minimumComponentSize, "
             "size_t maximumComponentCount, "
             "bool binary, "
             "Neighborhood::Type neighborhoodType, "
             "int32_t stride )" );

}


template < class IN, class OUT >
gkg::ConnectedComponents< IN, OUT >::~ConnectedComponents()
{
}


template < class IN, class OUT >
std::map< OUT, size_t > gkg::ConnectedComponents< IN, OUT >::getComponents(
                                        const gkg::Volume< IN >& inputVolume,
                                        gkg::Volume< OUT >& connectedComponents,
                                        int32_t rank,
                                        bool verbose ) const
{

  try
  {

    // sanity check
    if ( rank >= inputVolume.getSizeT() )
    {

      throw std::runtime_error( "invalid rank" );

    }

    // allocating stuff for output label volume
    reallocate( inputVolume, connectedComponents );

    // performing connected components search
    int32_t x, y, z;
    int32_t sizeX = inputVolume.getSizeX();
    int32_t sizeY = inputVolume.getSizeY();
    int32_t sizeZ = inputVolume.getSizeZ();

    // preparing a volume containing flags for use; background is set to already
    // used
    gkg::Volume< uint8_t > alreadyUsed( sizeX, sizeY, sizeZ );

    for ( z = 0; z < sizeZ; z += _stride )
    {

      for ( y = 0; y < sizeY; y += _stride )
      {

        for ( x = 0; x < sizeX; x += _stride )
        {


          bool value = _testFunction.getState( inputVolume( x, y, z, rank ) );
          alreadyUsed( x, y, z ) = ( value ? 0U : 1U );

        }

      }

    }

    // preparing temporary label volume
    gkg::Volume< OUT > tmpConnectedComponents( sizeX, sizeY, sizeZ );
    tmpConnectedComponents.fill( _background );

    // initializing label
    OUT label = _background + 1;

    // preparing a queue for front propagation
    std::queue< gkg::Vector3d< int32_t > > Q;

    // preparing and histogram of component size(s)
    std::multimap< size_t, OUT > sizeLut;

    // building complete component set
    gkg::Vector3d< int32_t > position;
    gkg::Vector3d< int32_t > newPosition;

    std::list< Vector3d< int32_t > >::const_iterator 
                                                 nb = _neighborOffsets.begin(),
                                                 ne = _neighborOffsets.end(),
                                                 n;

    for ( z = 0; z < sizeZ; z += _stride )
    {

      for ( y = 0; y < sizeY; y += _stride )
      {

        for ( x = 0; x < sizeX; x += _stride )
        {

          if ( !alreadyUsed( x, y, z ) )
          {

            IN value = inputVolume( x, y, z, rank );
            Q.push( gkg::Vector3d< int32_t >( x, y, z ) );
            alreadyUsed( x, y, z ) = true;
            size_t componentSize = 0U;
            while ( !Q.empty() )
            {

              position = Q.front();
              Q.pop();
              tmpConnectedComponents( position.x,
                                      position.y,
                                      position.z ) = label;
              ++ componentSize;
              for ( n = nb; n != ne; ++n )
              {

                newPosition = position;
                newPosition += *n;
                if ( ( newPosition.x >= 0 ) && ( newPosition.x < sizeX ) &&
                     ( newPosition.y >= 0 ) && ( newPosition.y < sizeY ) &&
                     ( newPosition.z >= 0 ) && ( newPosition.z < sizeZ ) )
                {

                  if ( !alreadyUsed( newPosition.x,
                                     newPosition.y,
                                     newPosition.z ) &&
                       ( _binary || ( inputVolume( newPosition.x,
                                                   newPosition.y,
                                                   newPosition.z,
                                                   rank ) == value) ) )
                  {

                    alreadyUsed( newPosition.x,
                                 newPosition.y,
                                 newPosition.z ) = true;
                    Q.push( newPosition );

                  }

                }

              }

            }

            sizeLut.insert( std::pair< size_t, OUT >( componentSize, label ) );
            ++ label;

          }

        }

      }

    }

    // removing too small components and building LUT that gives for a given
    // label the new label value and the corresponding component size
    std::map< OUT, OUT > validComponentLabels;
    std::map< OUT, size_t > validComponentSizes;
    typename std::multimap< size_t, OUT >::reverse_iterator s = sizeLut.rbegin(),
                                                            se = sizeLut.rend();
    label = _background + 1;
    size_t count = 0;
    while ( ( s != se ) &&
            ( s->first >= _minimumComponentSize ) &&
            ( !_maximumComponentCount || ( count < _maximumComponentCount ) ) )
    {

      validComponentLabels[ s->second ] = label;
      validComponentSizes[ s->second ] = s->first;
      ++ s;
      ++ label;
      ++ count;

    }

    // building output image
    connectedComponents.fill( _background );

    int32_t nx, ny, nz;
    typename std::map< OUT, OUT >::const_iterator
      v, ve = validComponentLabels.end();
    std::map< OUT, size_t > sizeHistogram;

    for ( z = 0; z < sizeZ; z += _stride )
    {

      for ( y = 0; y < sizeY; y += _stride )
      {

        for ( x = 0; x < sizeX; x += _stride )
        {

          v = validComponentLabels.find( tmpConnectedComponents( x, y, z ) );

          if ( v != ve )
          {

            if ( sizeHistogram.find( v->second ) != sizeHistogram.end() )
            {

              connectedComponents( x, y, z ) = v->second;

            }
            else
            {

              sizeHistogram[ v->second ] = validComponentSizes[ v->first ];
              connectedComponents( x, y, z ) = v->second;

            }

            if ( _stride > 1 )
            {

              for ( nz = 0; nz < _stride; nz++ )
              {

                for ( ny = 0; ny < _stride; ny++ )
                {

                  for ( nx = 0; nx < _stride; nx++ )
                  {

                    int32_t newX = x + nx;
                    int32_t newY = y + ny;
                    int32_t newZ = z + nz;
                    if ( ( newX >= 0 ) && ( newX < sizeX ) &&
                         ( newY >= 0 ) && ( newY < sizeY ) &&
                         ( newZ >= 0 ) && ( newZ < sizeZ ) )
                    {

                      connectedComponents( newX, newY, newZ ) = v->second;

                    }

                  }

                }

              }

            }

          }

        }

      }

    }

    if ( verbose && !sizeHistogram.empty() )
    {

      typename std::map< OUT, size_t >::const_iterator
        h = sizeHistogram.begin(),
        he = sizeHistogram.end();

      std::cout << sizeHistogram.size() << " component(s) [";

      while ( h != he )
      {

        std::cout << "(label=" << ( double )h->first
                  << ", size=" << h->second << ")";
        ++h;

      }

      std::cout << "]";

    }

    return sizeHistogram;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "std::map< OUT, size_t > "
             "gkg::ConnectedComponents< IN, OUT >::getComponents( "
             "const gkg::Volume< IN >& in, "
             "gkg::Volume< OUT >& out, "
             "int32_t rank = 0, "
             "bool verbose = false )" );

}


template < class IN, class OUT >
void gkg::ConnectedComponents< IN, OUT >::reallocate(
                                                 const gkg::Volume< IN >& in,
                                                 gkg::Volume< OUT >& out ) const
{

  try
  {

    // reallocating data
    out.reallocate( in.getSizeX(),
                    in.getSizeY(),
                    in.getSizeZ(),
                    1 );

    // duplicating header
    out.getHeader() = in.getHeader();
    out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::ConnectedComponents< IN, OUT >::reallocate( "
             "const gkg::Volume< IN >& in, "
             "gkg::Volume< OUT >& out )" );

}


template class gkg::ConnectedComponents< int8_t, uint8_t >;
template class gkg::ConnectedComponents< uint8_t, uint8_t >;
template class gkg::ConnectedComponents< int16_t, uint8_t >;
template class gkg::ConnectedComponents< uint16_t, uint8_t >;
template class gkg::ConnectedComponents< int32_t, uint8_t >;
template class gkg::ConnectedComponents< uint32_t, uint8_t >;
template class gkg::ConnectedComponents< int64_t, uint8_t >;
template class gkg::ConnectedComponents< uint64_t, uint8_t >;
template class gkg::ConnectedComponents< float, uint8_t >;
template class gkg::ConnectedComponents< double, uint8_t >;

template class gkg::ConnectedComponents< int8_t, int16_t >;
template class gkg::ConnectedComponents< uint8_t, int16_t >;
template class gkg::ConnectedComponents< int16_t, int16_t >;
template class gkg::ConnectedComponents< uint16_t, int16_t >;
template class gkg::ConnectedComponents< int32_t, int16_t >;
template class gkg::ConnectedComponents< uint32_t, int16_t >;
template class gkg::ConnectedComponents< int64_t, int16_t >;
template class gkg::ConnectedComponents< uint64_t, int16_t >;
template class gkg::ConnectedComponents< float, int16_t >;
template class gkg::ConnectedComponents< double, int16_t >;

template class gkg::ConnectedComponents< int8_t, int32_t >;
template class gkg::ConnectedComponents< uint8_t, int32_t >;
template class gkg::ConnectedComponents< int16_t, int32_t >;
template class gkg::ConnectedComponents< uint16_t, int32_t >;
template class gkg::ConnectedComponents< int32_t, int32_t >;
template class gkg::ConnectedComponents< uint32_t, int32_t >;
template class gkg::ConnectedComponents< int64_t, int32_t >;
template class gkg::ConnectedComponents< uint64_t, int32_t >;
template class gkg::ConnectedComponents< float, int32_t >;
template class gkg::ConnectedComponents< double, int32_t >;

