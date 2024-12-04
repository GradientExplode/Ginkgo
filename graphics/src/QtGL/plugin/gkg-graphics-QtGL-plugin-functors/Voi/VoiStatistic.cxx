#include <gkg-graphics-QtGL-plugin-functors/Voi/VoiStatistic.h>
#include <gkg-processing-container/Volume_i.h>

#include <fstream>
#include <cmath>
#include <cstring>


gkg::VoiStatistic::Stat::Stat()
                       : mean( 0.0 ),
                         sigma( 0.0 ),
                         volume( 0.0 ),
                         surface( 0.0 )
{
}


gkg::VoiStatistic::VoiStatistic()
{
}


gkg::VoiStatistic::~VoiStatistic()
{
}


void gkg::VoiStatistic::compute( gkg::VoiData& d )
{

  int32_t max = d.labelInfo().getMax() + 1;
  double n[ max ], m[ max ], m2[ max ];
  Stat s[ max ];
  int32_t x, y, z, t, l;
  gkg::Volume< float >& t1 = d.getBaseFusion().getT1();
  gkg::Vector3d< double > res;
  int32_t dx = t1.getSizeX();
  int32_t dy = t1.getSizeY();
  int32_t dz = t1.getSizeZ();
  int32_t dt = t1.getSizeT();
  int32_t dx1 = dx - 1;
  int32_t dy1 = dy - 1;
  int32_t dz1 = dz - 1;

  t1.getResolution( res );

  double sxy = res.x * res.y;
  double sxz = res.x * res.z;
  double syz = res.y * res.z;
  double sxyz = sxy * res.z;

  for ( t = 0; t < dt; t++ )
  {

    std::memset( n, 0, max * sizeof( double ) );
    std::memset( m, 0, max * sizeof( double ) );
    std::memset( m2, 0, max * sizeof( double ) );
    int32_t ss = 0;
    for ( ss = 0; ss < max; ss++ )
    {

      s[ ss ].mean = 0.0;
      s[ ss ].sigma = 0.0;
      s[ ss ].volume = 0.0;
      s[ ss ].surface = 0.0;

    }

    for ( z = 0; z < dz; z++ )
    {

      for ( y = 0; y < dy; y++ )
      {

        for ( x = 0; x < dx; x++ )
        {

          l = d.label()( x, y, z );

          if ( l )
          {

            double v = (double)t1( x, y, z, t );

            n[ l ] += 1.0f;
            m[ l ] += v;
            m2[ l ] += v * v;

            if ( ( x > 0 ) && ( x < dx1 ) )
            {

              if ( d.label()( x - 1, y, z ) != l )
              {

                s[ l ].surface += syz;

              } 

              if ( d.label()( x + 1, y, z ) != l )
              {

                s[ l ].surface += syz;

              }

            }
            else
            {

              s[ l ].surface += syz;

            }

            if ( ( y > 0 ) && ( y < dy1 ) )
            {

              if ( d.label()( x, y - 1, z ) != l )
              {

                s[ l ].surface += sxz;

              } 

              if ( d.label()( x, y + 1, z ) != l )
              {

                s[ l ].surface += sxz;

              }

            }
            else
            {

              s[ l ].surface += sxz;

            }

            if ( ( z > 0 ) && ( z < dz1 ) )
            {

              if ( d.label()( x, y, z - 1 ) != l )
              {

                s[ l ].surface += sxy;

              } 

              if ( d.label()( x, y, z - 1 ) != l )
              {

                s[ l ].surface += sxy;

              }

            }
            else
            {

              s[ l ].surface += sxy;

            }

          }

        }

      }

    }

    std::map< int32_t, std::pair< std::string, Stat > > stat;

    for ( l = 0; l < max; l++ )
    {

      double count = n[ l ];
 
      if ( count > 1.0 )
      {

        s[ l ].mean = m[ l ] / count;
        s[ l ].sigma = sqrt( ( m2[ l ] - m[ l ] * m[ l ] / count ) / 
                             ( count - 1.0 ) );
        s[ l ].volume = n[ l ] * sxyz;

        stat.insert( std::make_pair( l, 
                                     std::make_pair( d.labelInfo().getName( l ),
                                                     s[ l ] ) ) );

      }

    }

    _statistics.insert( std::make_pair( t, stat ) );

  }

}


void gkg::VoiStatistic::save( const std::string& fileName )
{

  std::ofstream ofs( fileName.c_str() );

  if ( ofs.good() )
  {

    std::map< int32_t, std::map< int32_t, 
                                 std::pair< std::string, Stat > > >::iterator
      l = _statistics.begin(),
      le = _statistics.end();

    while ( l != le )
    {

      ofs << "Volume #" << l->first << std::endl;
      ofs << std::endl;

      std::map< int32_t, std::pair< std::string, Stat > >::iterator
        s = l->second.begin(),
        se = l->second.end();

      while ( s != se )
      {

        ofs << "  Label #" << s->first
            << " - " << s->second.first
            << " : mean = " << s->second.second.mean
            << " std = " << s->second.second.sigma
            << " volume = " << s->second.second.volume
            << "mm3 surface = " << s->second.second.surface
            << "mm2" << std::endl;

        ++s;

      }

      ofs << std::endl;

      ++l;

    }

    ofs.close();

  }

}


bool gkg::VoiStatistic::empty()
{

  return _statistics.empty();

}


gkg::VoiStatistic::Stat gkg::VoiStatistic::statistic( int32_t label, int32_t t )
{

  std::map< int32_t, std::map< int32_t, 
                               std::pair< std::string, Stat > > >::iterator
    l = _statistics.find( t );

  if ( l != _statistics.end() )
  {

    std::map< int32_t, std::pair< std::string, Stat > >::iterator
      s = l->second.find( label );

    if ( s != l->second.end() )
    {

      return s->second.second;

    }

  }

  return gkg::VoiStatistic::Stat();

}
