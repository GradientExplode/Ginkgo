#ifndef _gkg_processing_machinelearning_MachineLearningData_i_h_
#define _gkg_processing_machinelearning_MachineLearningData_i_h_


#include <gkg-processing-machinelearning/MachineLearningData.h>
#include <gkg-processing-machinelearning/MachineLearningDataImplementation.h>
#include <gkg-processing-machinelearning/MachineLearningSelector.h>
#include <gkg-processing-machinelearning/MachineLearningScaler.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>

#include <algorithm>


template < class T, class M >
inline
gkg::MachineLearningData::MachineLearningData(
                                     const gkg::Volume< T >& volume,
                                     const gkg::Volume< M >& mask,
                                     const std::vector< uint8_t >& selectedRank,
                                     gkg::MachineLearningScaler& scaler )
{

  try
  {

    gkg::Matrix data;
    int32_t x, sizeX = volume.getSizeX();
    int32_t y, sizeY = volume.getSizeY();
    int32_t z, sizeZ = volume.getSizeZ();
    int32_t t, sizeT = volume.getSizeT();
    int32_t siteCount = 0, row = 0;
    int32_t selectionCount = (int32_t)std::count_if(
                            selectedRank.begin(),
                            selectedRank.end(),
                            std::bind2nd( std::not_equal_to< uint8_t >(), 0 ) );
    std::vector< uint8_t >::const_iterator s = selectedRank.begin();

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( mask( x, y, z ) )
          {

            siteCount++;

          }

        }

      }

    }

    data.reallocate( selectionCount, siteCount );

    for ( t = 0; t < sizeT; t++ )
    {

      if ( *s )
      {

        int32_t column = 0;

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0;  y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              if ( mask( x, y, z ) )
              {

                data( row, column++ ) = double( volume( x, y, z, t ) );

              }

            }

          }

        }

        row++;

      }

      ++s;

    }

    scaler.scale( data );
    _machineLearningDataImplementation =
      gkg::MachineLearningSelector::getInstance().getImplementationFactory()->
                          createMachineLearningDataImplementation( this, data );

  }
  GKG_CATCH( "template < class T, class M > inline "
             "gkg::MachineLearningData::MachineLearningData( "
             "const gkg::Volume< T >& volume, const gkg::Volume< M >& mask, "
             "const std::vector< uint8_t >& selectedRank, "
             "gkg::MachineLearningScaler& scaler )" );

}


template < class T >
inline
gkg::MachineLearningData::MachineLearningData( 
                           const gkg::Volume< T >& volume,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           const std::vector< int16_t >& selectedRank,
                           gkg::MachineLearningScaler& scaler )
{
  try
  {

    gkg::Matrix data( selectedRank.size(), sites.size() );
    std::vector< int16_t >::const_iterator 
      t = selectedRank.begin(),
      te = selectedRank.end();
    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      s, se = sites.end();
    int32_t row = 0;

    while ( t != te )
    {

      int32_t column = 0;

      s = sites.begin();

      while ( s != se )
      {

        data( row, column++ ) = double( volume( *s, *t ) );
        ++s;

      }

      ++row;
      ++t;

    }

    scaler.scale( data );
    _machineLearningDataImplementation =
      gkg::MachineLearningSelector::getInstance().getImplementationFactory()->
                          createMachineLearningDataImplementation( this, data );

  }
  GKG_CATCH( "template < class T > inline "
             "gkg::MachineLearningData::MachineLearningData( "
             "const gkg::Volume< T >& volume, "
             "std::vector< gkg::Vector3d< int32_t > >& sites, "
             "const std::vector< int16_t >& selectedRank, "
             "gkg::MachineLearningScaler& scaler )" );
}


#endif
