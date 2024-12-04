#include <gkg-processing-morphology/WatershededTransformation.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-core-cppext/Limits.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <queue>
#include <cmath>
#include <iomanip>


#include <gkg-core-io/Writer_i.h>


template < class IN, class OUT >
gkg::WatershededTransformation< IN, OUT >::WatershededTransformation(
                                  gkg::Neighborhood3d::Type neighborhoodType,
                                  const gkg::TestFunction< IN >& testFunction,
                                  const OUT& foreground,
                                  const OUT& background )
                                          : _neighborhoodType(
                                                             neighborhoodType ),
                                            _testFunction( testFunction ),
                                            _foreground( foreground ),
                                            _background( background )

{

  try
  {

    if ( ( _neighborhoodType != gkg::Neighborhood3d::Neighborhood3d_6 ) &&
         ( _neighborhoodType != gkg::Neighborhood3d::Neighborhood3d_18 ) &&
         ( _neighborhoodType != gkg::Neighborhood3d::Neighborhood3d_26 ) )
    {

       throw std::runtime_error( "neighborhood type must be one of  "
                                 "Neighborhood3d_6/Neighborhood3d_18/"
                                 "Neighborhood3d_26" );

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "gkg::WatershededTransformation< IN, OUT >::"
             "WatershededTransformation( "
             "gkg::Neighborhood3d::Type neighborhoodType, "
             "const gkg::TestFunction< IN >& testFunction, "
             "const OUT& foreground, "
             "const OUT& background )" );

}


template < class IN, class OUT >
gkg::WatershededTransformation< IN, OUT >::~WatershededTransformation()
{
}



template < class IN, class OUT >
void gkg::WatershededTransformation< IN, OUT >::watershed(
                                          const gkg::Volume< IN >& inputVolume,
                                          gkg::Volume< OUT >& watershededVolume,
                                          bool verbose ) const
{


  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting input volume size and resolution
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > inputVolumeSize;
    int32_t inputVolumeSizeT = 1;
    gkg::Vector3d< double > inputVolumeResolution;
    double inputVolumeResolutionT = 1.0;

    inputVolume.getSize( inputVolumeSize, inputVolumeSizeT );
    inputVolume.getResolution( inputVolumeResolution, inputVolumeResolutionT );

    // allocating a bounding box to check that any neighbor belongs to it
    gkg::BoundingBox< int32_t > boundingBox( inputVolume );


    ////////////////////////////////////////////////////////////////////////////
    // getting list of neighbor offset(s)
    ////////////////////////////////////////////////////////////////////////////

    const std::list< gkg::Vector3d< int32_t > >& neighborOffsets = 
      gkg::Neighborhood3d::getInstance().getNeighborOffsets( 
                                                            _neighborhoodType );

    std::list< gkg::Vector3d< int32_t > > forwardNeighborOffsets;
    std::list< gkg::Vector3d< int32_t > > backwardNeighborOffsets;

    std::vector< float > offsetDistances( neighborOffsets.size() );

    gkg::Vector3dCompare< int32_t > vector3dCompare;
    gkg::Vector3d< int32_t > centralVoxel( 0, 0, 0 );
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      o = neighborOffsets.begin(),
      oe = neighborOffsets.end();
    std::vector< float >::iterator
      d = offsetDistances.begin();
    while ( o != oe )
    {

      if ( vector3dCompare( centralVoxel, *o ) )
      {

        forwardNeighborOffsets.push_back( *o );

      }
      if ( vector3dCompare( *o, centralVoxel ) ) 
      {

        backwardNeighborOffsets.push_back( *o );

      }
      *d = ( float )o->getDoubleNorm();
      ++ o;
      ++ d;

    }
          

    ////////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ////////////////////////////////////////////////////////////////////////////

    // reallocating data
    watershededVolume.reallocate( inputVolumeSize, inputVolumeSizeT );

    // duplicating header
    watershededVolume.getHeader() = inputVolume.getHeader();
    watershededVolume.getHeader()[ "item_type" ] =
                                                  gkg::TypeOf< OUT >::getName();

    // reallocating data
    gkg::Volume< float > distanceVolume( inputVolumeSize );


    ////////////////////////////////////////////////////////////////////////////
    // looping over volumes
    ////////////////////////////////////////////////////////////////////////////

    int32_t t = 0;

    if ( verbose )
    {

      std::cout << "[ " << std::setw( 3 ) << t
                << " / " << std::setw( 3 ) << inputVolumeSizeT
                << " ]" << std::flush;

    }

    for ( t = 0; t < inputVolumeSizeT; t++ )
    {

      if ( verbose )
      {

        std::cout << gkg::Eraser( 13 );
        std::cout << "[ " << std::setw( 3 ) << t + 1
                  << " / " << std::setw( 3 ) << inputVolumeSizeT
                  << " ]" << std::flush;


      }


      //////////////////////////////////////////////////////////////////////////
      // computing the lower complete volume at time t
      //////////////////////////////////////////////////////////////////////////

      gkg::Volume< float > lowerCompleteVolume( inputVolumeSize );
      lowerCompleteVolume.getHeader().addAttribute( "resolutionX",
                                                    inputVolumeResolution.x );
      lowerCompleteVolume.getHeader().addAttribute( "resolutionY",
                                                    inputVolumeResolution.y );
      lowerCompleteVolume.getHeader().addAttribute( "resolutionZ",
                                                    inputVolumeResolution.z );

      std::vector< gkg::Vector3d< int32_t > > voxels;
      std::vector< const float* > completeVolumePointers;
      std::vector< float > lowerSlopes;
      std::vector< std::vector< std::pair< int32_t, int32_t > > > 
        lutWithAllNeighbors;
      std::vector< std::vector< std::pair< int32_t, int32_t > > >
        lutWithForwardNeighbors;
      std::vector< std::vector< std::pair< int32_t, int32_t > > >
        lutWithBackwardNeighbors;

      this->computeCompleteVolume( inputVolume,
                                   inputVolumeSize,
                                   t,
                                   neighborOffsets,
                                   boundingBox,
                                   offsetDistances,
                                   lowerCompleteVolume,
                                   voxels,
                                   completeVolumePointers,
                                   lowerSlopes,
                                   lutWithAllNeighbors,
                                   lutWithForwardNeighbors,
                                   lutWithBackwardNeighbors );


      //////////////////////////////////////////////////////////////////////////
      // computing the list of voxels corresponding to minima
      //////////////////////////////////////////////////////////////////////////

      std::list< int32_t > minimaVoxelIndices;

      int32_t currentVoxelIndex = 0;
      int32_t voxelCount = ( int32_t )voxels.size();
      for ( currentVoxelIndex = 0; currentVoxelIndex < voxelCount;
            currentVoxelIndex++ )
      {

        bool isAMinima = true;

        const float& currentCompleteValue =
                                   *completeVolumePointers[ currentVoxelIndex ];

        const std::vector< std::pair< int32_t, int32_t > >&
          neighbors = lutWithAllNeighbors[ currentVoxelIndex ];

        std::vector< std::pair< int32_t, int32_t > >::const_iterator
          n = neighbors.begin(),
          ne = neighbors.end();
        while ( n != ne )
        {

          const float& 
            neighborCompleteValue = *completeVolumePointers[ n->first ];
          if ( neighborCompleteValue <= currentCompleteValue )
          {

            isAMinima = false;
            break;

          }

          ++ n;

        } 

        if ( isAMinima )
        {

          minimaVoxelIndices.push_back( currentVoxelIndex );

        }

      }      


      //////////////////////////////////////////////////////////////////////////
      // initializing distance with values of minima
      //////////////////////////////////////////////////////////////////////////

      watershededVolume.fill( ( OUT )0 );
      distanceVolume.fill( std::numeric_limits< float >::max() );

      std::list< int32_t >::const_iterator
        vi = minimaVoxelIndices.begin(),
        vie = minimaVoxelIndices.end();
      int32_t label = 1;
      while ( vi != vie )
      {

        const gkg::Vector3d< int32_t >& minimumVoxel = voxels[ *vi ];
        watershededVolume( minimumVoxel, t ) = label;
        distanceVolume( minimumVoxel ) = *( completeVolumePointers[ *vi ] );

        ++ label;
        ++ vi;

      }


      //////////////////////////////////////////////////////////////////////////
      // creating a structure to speed-up access to watershed and distance
      // element(s)
      //////////////////////////////////////////////////////////////////////////

      std::vector< OUT* > watershedVolumePointers( voxelCount );
      std::vector< float* > distancePointers( voxelCount );

      for ( currentVoxelIndex = 0; currentVoxelIndex < voxelCount;
            currentVoxelIndex++ )
      {

        watershedVolumePointers[ currentVoxelIndex ] =
          &watershededVolume( voxels[ currentVoxelIndex ], t );
        distancePointers[ currentVoxelIndex ] =
          &distanceVolume( voxels[ currentVoxelIndex ] );

      }


      //////////////////////////////////////////////////////////////////////////
      // initializing watershed and distance volume(s)
      //////////////////////////////////////////////////////////////////////////

      bool stable = true;
      int32_t iteration = 1;
      do
      {

        stable = true;

        // forward raster scanning
        for ( currentVoxelIndex = 0; currentVoxelIndex < voxelCount;
              currentVoxelIndex++ )
        {

          const float& currentDistance = *distancePointers[ currentVoxelIndex ];

          const std::vector< std::pair< int32_t, int32_t > >&
            forwardNeighbors = lutWithForwardNeighbors[ currentVoxelIndex ];

          std::vector< std::pair< int32_t, int32_t > >::const_iterator
            n = forwardNeighbors.begin(),
            ne = forwardNeighbors.end();
          while ( n != ne )
          {

            const int32_t& neighborVoxelIndex = n->first;
            const int32_t& offsetIndex = n->second;
            float& neighborDistance = *distancePointers[ neighborVoxelIndex ];


            if ( *completeVolumePointers[ neighborVoxelIndex ] >
                 *completeVolumePointers[ currentVoxelIndex ] )
            {

              float cost = getCost(
                   completeVolumePointers,
                   lowerSlopes,
                   offsetDistances[ offsetIndex ],
                   currentVoxelIndex,
                   neighborVoxelIndex );

              if ( ( currentDistance + cost ) < neighborDistance )
              {

                neighborDistance = currentDistance + cost;
                *watershedVolumePointers[ neighborVoxelIndex ] =
                  *watershedVolumePointers[ currentVoxelIndex ];
                stable = false;

              }
              else if ( ( *watershedVolumePointers[ neighborVoxelIndex ] !=
                          ( OUT )0 ) &&
                        ( ( currentDistance + cost ) == neighborDistance ) &&
                        ( *watershedVolumePointers[ neighborVoxelIndex ] !=
                          *watershedVolumePointers[ currentVoxelIndex ] ) )
              {

                *watershedVolumePointers[ neighborVoxelIndex ] = ( OUT )0;
                stable = false;

              }

            }

            ++ n;

          } 

        }

        // backward raster scanning
        for ( currentVoxelIndex = voxelCount - 1; currentVoxelIndex >= 0;
              currentVoxelIndex-- )
        {

          const gkg::Vector3d< int32_t >& 
            currentVoxel = voxels[ currentVoxelIndex ];
          const float& currentDistance = distanceVolume( currentVoxel );

          const std::vector< std::pair< int32_t, int32_t > >&
            backwardNeighbors = lutWithBackwardNeighbors[ currentVoxelIndex ];

          std::vector< std::pair< int32_t, int32_t > >::const_reverse_iterator
            n = backwardNeighbors.rbegin(),
            ne = backwardNeighbors.rend();
          while ( n != ne )
          {

            const int32_t& neighborVoxelIndex = n->first;
            const int32_t& offsetIndex = n->second;
            const gkg::Vector3d< int32_t >&
              neighborVoxel = voxels[ neighborVoxelIndex ];
            float& neighborDistance = distanceVolume( neighborVoxel );

            if ( *completeVolumePointers[ neighborVoxelIndex ] >
                 *completeVolumePointers[ currentVoxelIndex ] )
            {

              float cost = getCost(
                   completeVolumePointers,
                   lowerSlopes,
                   offsetDistances[ offsetIndex ],
                   currentVoxelIndex,
                   neighborVoxelIndex );

              if ( ( currentDistance + cost ) < neighborDistance )
              {

                neighborDistance = currentDistance + cost;
                *watershedVolumePointers[ neighborVoxelIndex ] =
                  *watershedVolumePointers[ currentVoxelIndex ];
                stable = false;

              }
              else if ( ( *watershedVolumePointers[ neighborVoxelIndex ] !=
                          ( OUT )0 ) &&
                        ( ( currentDistance + cost ) == neighborDistance ) &&
                        ( *watershedVolumePointers[ neighborVoxelIndex ] !=
                          *watershedVolumePointers[ currentVoxelIndex ] ) )
              {

                *watershedVolumePointers[ neighborVoxelIndex ] = ( OUT )0;
                stable = false;

              }

            }

            ++ n;

          } 

        }

        ++ iteration;

      }
      while ( !stable );

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 13 );

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::WatershededTransformation< IN, OUT >::watershed( "
             "const gkg::Volume< IN >& inputVolume, "
             "gkg::Volume< OUT >& watershededVolume, "
             "bool verbose ) const" );

}


template < class IN, class OUT >
void gkg::WatershededTransformation< IN, OUT >::computeCompleteVolume(
   const gkg::Volume< IN >& inputVolume,
   const gkg::Vector3d< int32_t >& inputVolumeSize,
   int32_t t,
   const std::list< gkg::Vector3d< int32_t > >& neighborOffsets,
   const gkg::BoundingBox< int32_t >& boundingBox,
   const std::vector< float >& offsetDistances,
   gkg::Volume< float >& lowerCompleteVolume,
   std::vector< gkg::Vector3d< int32_t > >& voxels,
   std::vector< const float* >& completeVolumePointers,
   std::vector< float >& lowerSlopes,
   std::vector< std::vector< std::pair< int32_t, int32_t > > >& 
                                                lutWithAllNeighbors,
   std::vector< std::vector< std::pair< int32_t, int32_t > > >&
                                                lutWithForwardNeighbors,
   std::vector< std::vector< std::pair< int32_t, int32_t > > >&
                                                lutWithBackwardNeighbors ) const
{

  try
  {

    gkg::Vector3d< int32_t > centralVoxel( 0, 0, 0 );
    gkg::Vector3dCompare< int32_t > vector3dCompare;

    // first, setting the whole volume to nul value
    lowerCompleteVolume.fill( 0.0f );

    // allocating the list of voxels having a lower neighbor
    std::list< gkg::Vector3d< int32_t > > listOfVoxelsHavingALowerNeighbor;


    // counting the number of valid voxels
    gkg::Vector3d< int32_t > currentVoxel;
    gkg::Volume< int32_t > voxelLut( inputVolumeSize );
    voxelLut.fill( 0 );
    int32_t voxelCount = 0;
    for ( currentVoxel.z = 0; currentVoxel.z < inputVolumeSize.z;
          currentVoxel.z++ )
    {

      for ( currentVoxel.y = 0; currentVoxel.y < inputVolumeSize.y;
            currentVoxel.y++ )
      {

        for ( currentVoxel.x = 0; currentVoxel.x < inputVolumeSize.x;
              currentVoxel.x++ )
        {

          const IN& currentValue = inputVolume( currentVoxel, t );
          if ( _testFunction.getState( currentValue ) )
          {

            voxelLut( currentVoxel ) = voxelCount;
            ++ voxelCount;

          }

        }

      }

    }

    // resizing the container(s)
    voxels.resize( voxelCount );
    completeVolumePointers.resize( voxelCount );
    lowerSlopes.resize( voxelCount );
    lutWithAllNeighbors.resize( voxelCount );
    lutWithForwardNeighbors.resize( voxelCount );
    lutWithBackwardNeighbors.resize( voxelCount );


    // filling the voxel container
    int32_t currentVoxelIndex = 0;
    for ( currentVoxel.z = 0; currentVoxel.z < inputVolumeSize.z;
          currentVoxel.z++ )
    {

      for ( currentVoxel.y = 0; currentVoxel.y < inputVolumeSize.y;
            currentVoxel.y++ )
      {

        for ( currentVoxel.x = 0; currentVoxel.x < inputVolumeSize.x;
              currentVoxel.x++ )
        {

          const IN& currentValue = inputVolume( currentVoxel, t );
          if ( _testFunction.getState( currentValue ) )
          {

            voxels[ currentVoxelIndex ] = currentVoxel;
            ++ currentVoxelIndex;

          }
          else
          {

            lowerCompleteVolume( currentVoxel ) = -2.0f;

          }

        }

      }

    }
    
    std::vector< bool > isForward( neighborOffsets.size(), true );

    std::list< gkg::Vector3d< int32_t > >::const_iterator
      o = neighborOffsets.begin(),
      oe = neighborOffsets.end();
    std::vector< bool >::iterator
      isF = isForward.begin();
    while ( o != oe )
    {

      *isF = vector3dCompare( centralVoxel, *o );
      ++ o;
      ++ isF;

    }

    // looping over voxel(s)
    gkg::Vector3d< int32_t > neighborVoxel;
    int32_t offsetIndex = 0;
    for ( currentVoxelIndex = 0; currentVoxelIndex < voxelCount;
          currentVoxelIndex++ )
    {

      const gkg::Vector3d< int32_t >&
        currentVoxel = voxels[ currentVoxelIndex ];
      const IN& currentValue = inputVolume( currentVoxel, t );

      const float* currentLowerCompleteVolumePointer =
                                     &lowerCompleteVolume( currentVoxel );
      completeVolumePointers[ currentVoxelIndex ] = 
                                        currentLowerCompleteVolumePointer;

      std::vector< std::pair< int32_t, int32_t > >&
        allNeighbors = lutWithAllNeighbors[ currentVoxelIndex ];
      std::vector< std::pair< int32_t, int32_t > >&
        forwardNeighbors = lutWithForwardNeighbors[ currentVoxelIndex ];
      std::vector< std::pair< int32_t, int32_t > >&
        backwardNeighbors = lutWithBackwardNeighbors[ currentVoxelIndex ];

      allNeighbors.reserve( neighborOffsets.size() );
      forwardNeighbors.reserve( neighborOffsets.size() / 2U );
      backwardNeighbors.reserve( neighborOffsets.size() / 2U );

      bool hasALowerNeighbor = false;
      o = neighborOffsets.begin();
      isF = isForward.begin();
      offsetIndex = 0;
      std::pair< int32_t, int32_t > neighborVoxelIndexAndOffsetIndex;
      while ( o != oe )
      {

        neighborVoxel.x = currentVoxel.x + o->x;
        neighborVoxel.y = currentVoxel.y + o->y;
        neighborVoxel.z = currentVoxel.z + o->z;

        if ( boundingBox.contains( neighborVoxel ) )
        {

          const IN& neighborValue = inputVolume( neighborVoxel, t );

          if ( _testFunction.getState( neighborValue ) )
          {

            neighborVoxelIndexAndOffsetIndex.first = voxelLut( neighborVoxel );
            neighborVoxelIndexAndOffsetIndex.second = offsetIndex;

            allNeighbors.push_back( neighborVoxelIndexAndOffsetIndex );

            if ( *isF )
            {

              forwardNeighbors.push_back( neighborVoxelIndexAndOffsetIndex );

            }
            else
            {

              backwardNeighbors.push_back( neighborVoxelIndexAndOffsetIndex );

            }

            if ( neighborValue < currentValue )
            {

              hasALowerNeighbor = true;

            }

          }

        }

        ++ o;
        ++ isF;
        ++ offsetIndex;
      
      }

      if ( hasALowerNeighbor )
      {

        listOfVoxelsHavingALowerNeighbor.push_back( currentVoxel );
        lowerCompleteVolume( currentVoxel ) = -1.0f;

      }


    }


    // allocating a integer distance variable
    int32_t distance = 1;

    gkg::Vector3d< int32_t > fictitiousVoxel( -1, -1, -1 );

    // inserting fictitious voxel
    listOfVoxelsHavingALowerNeighbor.push_back( fictitiousVoxel );

    while ( !listOfVoxelsHavingALowerNeighbor.empty() )
    {

      currentVoxel = listOfVoxelsHavingALowerNeighbor.front();
      listOfVoxelsHavingALowerNeighbor.pop_front();

      if ( currentVoxel == fictitiousVoxel )
      {

        if ( !listOfVoxelsHavingALowerNeighbor.empty() )
        {

          listOfVoxelsHavingALowerNeighbor.push_back( fictitiousVoxel );
          distance += 1;

        }

      }
      else
      {

        lowerCompleteVolume( currentVoxel ) = distance;

        std::list< gkg::Vector3d< int32_t > >::const_iterator
          o = neighborOffsets.begin(),
          oe = neighborOffsets.end();
        while ( o != oe )
        {

          neighborVoxel.x = currentVoxel.x + o->x;
          neighborVoxel.y = currentVoxel.y + o->y;
          neighborVoxel.z = currentVoxel.z + o->z;

          if ( boundingBox.contains( neighborVoxel ) )
          {

            const IN& currentValue = inputVolume( currentVoxel, t );
            const IN& neighborValue = inputVolume( neighborVoxel, t );
            if ( ( currentValue == neighborValue ) &&
                 ( lowerCompleteVolume( neighborVoxel ) == 0 ) )
            {

              listOfVoxelsHavingALowerNeighbor.push_back( neighborVoxel );
              lowerCompleteVolume( neighborVoxel ) = -1;

            } 

          }
          ++ o;

        }

      }

    }

    // put the lower complete values in the output image
    for ( currentVoxel.z = 0; currentVoxel.z < inputVolumeSize.z;
          currentVoxel.z++ )
    {

      for ( currentVoxel.y = 0; currentVoxel.y < inputVolumeSize.y;
            currentVoxel.y++ )
      {

        for ( currentVoxel.x = 0; currentVoxel.x < inputVolumeSize.x;
              currentVoxel.x++ )
        {

          const IN& currentValue = inputVolume( currentVoxel, t );
          if ( _testFunction.getState( currentValue ) )
          {

            if ( lowerCompleteVolume( currentVoxel ) != 0 )
            {

              lowerCompleteVolume( currentVoxel ) =
                                           distance * ( float )currentValue +
                                           lowerCompleteVolume( currentVoxel ) -
                                           1;

            }
            else
            {

              lowerCompleteVolume( currentVoxel ) =
                                               distance * ( float )currentValue;

            }

          }
          else 
          {

             lowerCompleteVolume( currentVoxel ) = 0.0f;           

          }

        }

      }

    }

    // computing lower slope(s)
    for ( currentVoxelIndex = 0; currentVoxelIndex < voxelCount;
          currentVoxelIndex++ )
    {

      lowerSlopes[ currentVoxelIndex ] = this->getLowerSlope(
                                     completeVolumePointers,
                                     offsetDistances,
                                     currentVoxelIndex,
                                     lutWithAllNeighbors[ currentVoxelIndex ] );

    }

  }
  GKG_CATCH( "template < class IN, class OUT  >"
             "void gkg::WatershededTransformation< IN, OUT >::"
             "computeCompleteVolume( "
             "const gkg::Volume< IN >& inputVolume, "
             "const gkg::Vector3d< int32_t >& inputVolumeSize, "
             "int32_t t, "
             "const std::list< gkg::Vector3d< int32_t > >& neighborOffsets, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const std::vector< float >& offsetDistances, "
             "gkg::Volume< float >& lowerCompleteVolume, "
             "std::vector< gkg::Vector3d< int32_t > >& voxels, "
             "std::vector< const float* >& completeVolumePointers, "
             "std::vector< float >& lowerSlopes, "
             "std::vector< std::vector< std::pair< int32_t, int32_t > > >& "
             "lutWithAllNeighbors, "
             "std::vector< std::vector< std::pair< int32_t, int32_t > > >& "
             "lutWithForwardNeighbors, "
             "std::vector< std::vector< std::pair< int32_t, int32_t > > >& "
             "lutWithBackwardNeighbors ) const" );

}



template < class IN, class OUT >
float gkg::WatershededTransformation< IN, OUT >::getCost(
  const std::vector< const float* >& completeVolumePointers,
  const std::vector< float >& lowerSlopes,
  float offsetDistance,
  int32_t currentVoxelIndex,
  int32_t neighborVoxelIndex ) const
{

  try
  {

    const float&
      currentCompleteValue = *( completeVolumePointers[ currentVoxelIndex ] );
    const float&
      neighborCompleteValue = *( completeVolumePointers[ neighborVoxelIndex ] );

    float cost = 0.0f;
    if ( currentCompleteValue > neighborCompleteValue )
    {

      cost = lowerSlopes[ currentVoxelIndex ];
 
    }
    else if ( currentCompleteValue < neighborCompleteValue )
    {

      cost = lowerSlopes[ neighborVoxelIndex ];

    }
    else
    {

      cost = 0.5f * ( lowerSlopes[ currentVoxelIndex ] +
                      lowerSlopes[ neighborVoxelIndex ] );

    }

    return cost * offsetDistance;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "float gkg::WatershededTransformation< IN, OUT >::getCost( "
             "const std::vector< const float* >& completeVolumePointers, "
             "const std::vector< float >& lowerSlopes, "
             "float offsetDistance, "
             "int32_t currentVoxelIndex, "
             "int32_t neighborVoxelIndexl ) const" );



}


template < class IN, class OUT > 
float gkg::WatershededTransformation< IN, OUT >::getLowerSlope(
           const std::vector< const float* >& completeVolumePointers,
           const std::vector< float >& offsetDistances,
           int32_t currentVoxelIndex,
           const std::vector< std::pair< int32_t, int32_t > >& neighbors ) const
{

  try
  {


    float slope = 0.0f;
    float lowerSlope = -1e38;
    float distance = 0.0f;

    const float& 
      currentCompleteValue = *( completeVolumePointers[ currentVoxelIndex ] );

    std::vector< std::pair< int32_t, int32_t > >::const_iterator
      n = neighbors.begin(),
      ne = neighbors.end();
    while ( n != ne )
    {

      const float& 
        neighborCompleteValue = *( completeVolumePointers[ n->first ] );
      distance = offsetDistances[ n->second ];

      slope = ( currentCompleteValue - neighborCompleteValue ) / distance;
        
      if ( slope > lowerSlope )
      {

        lowerSlope = slope;

      } 

      ++ n;

    }
    return std::max( lowerSlope, 0.0f );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "float gkg::WatershededTransformation< IN, OUT >::getLowerSlope( "
             "const std::vector< const float* >& completeVolumePointers, "
             "const std::vector< float >& offsetDistances, "
             "int32_t currentVoxelIndex, "
             "const std::vector< std::pair< int32_t, int32_t > >& neighbors ) "
             "const" );



}


template class gkg::WatershededTransformation< int8_t, uint8_t >;
template class gkg::WatershededTransformation< uint8_t, uint8_t >;
template class gkg::WatershededTransformation< int16_t, uint8_t >;
template class gkg::WatershededTransformation< uint16_t, uint8_t >;
template class gkg::WatershededTransformation< int32_t, uint8_t >;
template class gkg::WatershededTransformation< uint32_t, uint8_t >;
template class gkg::WatershededTransformation< int64_t, uint8_t >;
template class gkg::WatershededTransformation< uint64_t, uint8_t >;
template class gkg::WatershededTransformation< float, uint8_t >;
template class gkg::WatershededTransformation< double, uint8_t >;

template class gkg::WatershededTransformation< int8_t, int16_t >;
template class gkg::WatershededTransformation< uint8_t, int16_t >;
template class gkg::WatershededTransformation< int16_t, int16_t >;
template class gkg::WatershededTransformation< uint16_t, int16_t >;
template class gkg::WatershededTransformation< int32_t, int16_t >;
template class gkg::WatershededTransformation< uint32_t, int16_t >;
template class gkg::WatershededTransformation< int64_t, int16_t >;
template class gkg::WatershededTransformation< uint64_t, int16_t >;
template class gkg::WatershededTransformation< float, int16_t >;
template class gkg::WatershededTransformation< double, int16_t >;

template class gkg::WatershededTransformation< int8_t, int32_t >;
template class gkg::WatershededTransformation< uint8_t, int32_t >;
template class gkg::WatershededTransformation< int16_t, int32_t >;
template class gkg::WatershededTransformation< uint16_t, int32_t >;
template class gkg::WatershededTransformation< int32_t, int32_t >;
template class gkg::WatershededTransformation< uint32_t, int32_t >;
template class gkg::WatershededTransformation< int64_t, int32_t >;
template class gkg::WatershededTransformation< uint64_t, int32_t >;
template class gkg::WatershededTransformation< float, int32_t >;
template class gkg::WatershededTransformation< double, int32_t >;

template class gkg::WatershededTransformation< int8_t, float >;
template class gkg::WatershededTransformation< uint8_t, float >;
template class gkg::WatershededTransformation< int16_t, float >;
template class gkg::WatershededTransformation< uint16_t, float >;
template class gkg::WatershededTransformation< int32_t, float >;
template class gkg::WatershededTransformation< uint32_t, float >;
template class gkg::WatershededTransformation< int64_t, float >;
template class gkg::WatershededTransformation< uint64_t, float >;
template class gkg::WatershededTransformation< float, float >;
template class gkg::WatershededTransformation< double, float >;

template class gkg::WatershededTransformation< int8_t, double >;
template class gkg::WatershededTransformation< uint8_t, double >;
template class gkg::WatershededTransformation< int16_t, double >;
template class gkg::WatershededTransformation< uint16_t, double >;
template class gkg::WatershededTransformation< int32_t, double >;
template class gkg::WatershededTransformation< uint32_t, double >;
template class gkg::WatershededTransformation< int64_t, double >;
template class gkg::WatershededTransformation< uint64_t, double >;
template class gkg::WatershededTransformation< float, double >;
template class gkg::WatershededTransformation< double, double >;

