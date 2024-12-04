#ifndef _gkg_dmri_global_tractography_SpinGlassConnectionProposal_i_h_
#define _gkg_dmri_global_tractography_SpinGlassConnectionProposal_i_h_


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// SPIN GLASS CONNECTION PROPOSAL
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template < class L, class Compare > 
inline
typename gkg::GlobalTractographyAlgorithm< L, Compare >::ConnectionType
gkg::GlobalTractographyAlgorithm< L, Compare >::makeSpinGlassConnectionProposal(
     std::map< gkg::Vector3d< float >,
               std::pair< std::vector< float >, std::pair< float, float > >,
               gkg::Vector3dCompare< float > >& buffer,
     gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
     gkg::SpinGlass::ExtremityType& selectedSpinGlassExtremityType,
     const gkg::Vector3d< int32_t >& voxel,
     const gkg::CartesianField< std::set< gkg::RCPointer< gkg::SpinGlass > > >&
                                                        spinGlassCartesianField,
     float temperature,
     gkg::RCPointer< gkg::SpinGlassConnection >& oldSpinGlassConnection,
     gkg::RCPointer< gkg::SpinGlass >& oldConnectedSpinGlass,
     gkg::SpinGlass::ExtremityType& oldConnectedSpinGlassExtremityType,
     const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
     const gkg::Volume< int16_t >& computingMask,
     const gkg::Vector3d< double >& roiResolution,
     gkg::OdfContinuousField& odfContinuousField,
     const gkg::Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
     const gkg::OrientationSet& outputOrientationSet,
     float globalMinimumOdfProbability,
     float globalMaximumOdfProbability,
     const gkg::RCPointer< gkg::UniqueIdentityGenerator >&
                                               spinGlassUniqueIdentityGenerator,
     std::deque< std::pair< gkg::RCPointer< gkg::SpinGlass >,
                            gkg::SpinGlass::ExtremityType > >&
                                        neighboringSpinGlassesAndExtremityTypes,
    gkg::Vector3d< int32_t >& successorVoxel,
    gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass,
    gkg::SpinGlass::ExtremityType& successorSpinGlassExtremityType,
    std::set< gkg::RCPointer< gkg::SpinGlass > >& /* isolatedSpinGlasses */,
    float& oldConnectionEnergy,
    float& newSpinGlassExternalEnergy,
    float& newConnectionEnergy ) const
{

  try
  {

    typename gkg::GlobalTractographyAlgorithm< L, Compare >::ConnectionType
      connectionType = 
            gkg::GlobalTractographyAlgorithm< L, Compare >::UndefinedConnection;

    ////////////////////////////////////////////////////////////////////////////
    // randomly choosing the extremity type only if the spin glass is isolated
    // or fully attached, else taking the free extremity
    ////////////////////////////////////////////////////////////////////////////


    if ( !selectedSpinGlass->isAnExtremity( &selectedSpinGlassExtremityType ) )
    {


      this->getRandomSpinGlassExtremityType( selectedSpinGlassExtremityType );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      if ( selectedSpinGlassExtremityType == gkg::SpinGlass::Forward )
      {

        os << "selectedSpinGlassExtremityType : FORWARD" << std::endl;

      }
      else
      {

        os << "selectedSpinGlassExtremityType : BACKWARD" << std::endl;

      }
      END_DEBUG
      //------------------------------------------------------------------------

    }

    // checking if the chosen extremity is too close to the cortex
    const gkg::Vector3d< float >&
      selectedSpinGlassExtremity = selectedSpinGlass->getExtremity(
                                               selectedSpinGlassExtremityType );

    float distanceFromChosenExtremityToPialSurface = 0.0f;
    bool isChosenExtremityCloseToCortex = 
                                   _anatomicalPriors->getDistanceToPialSurface(
                                     selectedSpinGlassExtremity,
                                     distanceFromChosenExtremityToPialSurface );

    //------------------------------------------------------------------------
    BEGIN_DEBUG
    os << "distanceFromChosenExtremityToPialSurface : "
       << distanceFromChosenExtremityToPialSurface << std::endl;
    END_DEBUG
    //------------------------------------------------------------------------

    // if the distance is higher than the fixed threshold and the anatomical
    // priors are not given, we stay with this proposition
    if ( ( ( isChosenExtremityCloseToCortex &&
             ( distanceFromChosenExtremityToPialSurface >
               _anatomicalPriors->getMinimumDistanceToMesh() ) ) ||
           !isChosenExtremityCloseToCortex ) ||
         ( !_anatomicalPriors->hasPialSurface() ) )
    {

      //////////////////////////////////////////////////////////////////////////
      // checking if the selected spin glass is connected at the chosen
      // extremity
      //////////////////////////////////////////////////////////////////////////

      if ( selectedSpinGlass->isConnected( selectedSpinGlassExtremityType ) )
      {

        // storing the old spin glass connection at the chosen extremity
        oldSpinGlassConnection = selectedSpinGlass->getSpinGlassConnection(
                                               selectedSpinGlassExtremityType );

        // storing the old connected spin glass
        oldConnectedSpinGlass = selectedSpinGlass->getConnectedSpinGlass(
                                               selectedSpinGlassExtremityType );

        // storing the old connected spin glass extremity type
        oldSpinGlassConnection->getExtremityTypeOfConnectedSpinGlass(
                                           selectedSpinGlass,
                                           oldConnectedSpinGlassExtremityType );

        // disconnecting both spin glasses through the old spin glass connection
        oldSpinGlassConnection->disconnect();

      }

      //////////////////////////////////////////////////////////////////////////
      // computing the neighboring spin glasses (only the isolated ones...)
      //////////////////////////////////////////////////////////////////////////

      this->getNeighboringSpinGlassesWithinSphere(
                                      selectedSpinGlass,
                                      selectedSpinGlassExtremityType,
                                      spinGlassCartesianField,
                                      voxel,
                                      neighboringSpinGlassesAndExtremityTypes );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "neighboring spin glasses and extremity types size : "
         << neighboringSpinGlassesAndExtremityTypes.size() << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

      //////////////////////////////////////////////////////////////////////////
      // if there is at least one neighboring spin glass ...
      //////////////////////////////////////////////////////////////////////////
      if ( !neighboringSpinGlassesAndExtremityTypes.empty() )
      {

        // ... selecting the "best" neighboring spin glass
        this->getSuccessorSpinGlassAndComputeEnergies(
                                        selectedSpinGlass,
                                        selectedSpinGlassExtremityType,
                                        oldConnectedSpinGlass,
                                        neighboringSpinGlassesAndExtremityTypes,
                                        temperature,
                                        successorSpinGlass,
                                        successorSpinGlassExtremityType,
                                        oldConnectionEnergy,
                                        newConnectionEnergy );

        // sanity check
        if ( successorSpinGlass.isNull() )
        {

          throw std::runtime_error( "the successor spin glass is null..." );

        }


        //----------------------------------------------------------------------
        BEGIN_DEBUG
        // update: checking the signed dot product
        const gkg::Vector3d< float >&
          selectedSpinGlassOrientation = selectedSpinGlass->getOrientation();
        const gkg::Vector3d< float >&
          successorSpinGlassOrientation = successorSpinGlass->getOrientation();
        float dotProduct =
              selectedSpinGlassOrientation.dot( successorSpinGlassOrientation );
        os << "selected spin glass id : "
           << selectedSpinGlass->getIdentity() << std::endl;
        os << "selected spin glass position : "
           << selectedSpinGlass->getPosition() << std::endl;
        os << "selected spin glass orientation : "
           << selectedSpinGlassOrientation << std::endl;
        os << "successor spin glass id : "
           << successorSpinGlass->getIdentity() << std::endl;
        os << "successor spin glass position : "
           << successorSpinGlass->getPosition() << std::endl;
        os << "successor spin glass orientation : "
           << successorSpinGlassOrientation << std::endl;
        os << "dotProduct = " << dotProduct << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        // if the successor spin glass is the old one, we just reconnect them
        if ( successorSpinGlass == oldConnectedSpinGlass )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "successor spin glass is the old one"
             << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          connectionType =
            gkg::GlobalTractographyAlgorithm< L, Compare >::ReconnectionToOlder;

        }
        else
        {

          // to prevent the spin glass from returning from where it came from
          if ( newConnectionEnergy < 0.0f )
          {

            connectionType =
               gkg::GlobalTractographyAlgorithm< L, Compare >::SimpleConnection;

          }
          else
          {

            if ( oldConnectedSpinGlass.isNull() )
            {

              //----------------------------------------------------------------
              BEGIN_DEBUG
              os << "old connected spin glass is null -> creation connection"
                 << std::endl;
              END_DEBUG
              //----------------------------------------------------------------

              // creating a new spin glass, connecting the selected one to it
              connectionType = this->createSuccessorSpinGlassAndComputeEnergies(
                                             buffer,
                                             selectedSpinGlass,
                                             selectedSpinGlassExtremityType,
                                             roiResolution,
                                             computingMaskBoundingBox,
                                             computingMask,
                                             odfContinuousField,
                                             transform3dFromRoisVoxelToOdfsReal,
                                             outputOrientationSet,
                                             globalMinimumOdfProbability,
                                             globalMaximumOdfProbability,
                                             spinGlassUniqueIdentityGenerator,
                                             successorVoxel,
                                             successorSpinGlass,
                                             successorSpinGlassExtremityType,
                                             newSpinGlassExternalEnergy,
                                             newConnectionEnergy );

            }
            else
            {

              // should lead to reconnecting the old connected spin glass
              connectionType =
                gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                            ReconnectionToOlder;

            }

          }

        }

      }
      //////////////////////////////////////////////////////////////////////////
      // if there is no neighboring spin glass and no old connected spin glass
      //////////////////////////////////////////////////////////////////////////
      else if ( neighboringSpinGlassesAndExtremityTypes.empty() &&
                oldConnectedSpinGlass.isNull() )
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "neighboring empty and old connected spin glass is null"
           << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        // ... creating a new spin glass, connecting the selected one to it
        connectionType = this->createSuccessorSpinGlassAndComputeEnergies(
                                             buffer,
                                             selectedSpinGlass,
                                             selectedSpinGlassExtremityType,
                                             roiResolution,
                                             computingMaskBoundingBox,
                                             computingMask,
                                             odfContinuousField,
                                             transform3dFromRoisVoxelToOdfsReal,
                                             outputOrientationSet,
                                             globalMinimumOdfProbability,
                                             globalMaximumOdfProbability,
                                             spinGlassUniqueIdentityGenerator,
                                             successorVoxel,
                                             successorSpinGlass,
                                             successorSpinGlassExtremityType,
                                             newSpinGlassExternalEnergy,
                                             newConnectionEnergy );

      }
      //////////////////////////////////////////////////////////////////////////
      // if there is no neighboring spin glass but an old connected spin glass
      //////////////////////////////////////////////////////////////////////////
      else
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "neighboring empty and old connected spin glass present"
           << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        // sanity check
        if ( oldConnectedSpinGlass.isNull() )
        {

          throw std::runtime_error( "old connected spin glass is null" );

        }

        // no valid proposal ... should lead to reconnecting the old connected
        // spin glass
        connectionType =
            gkg::GlobalTractographyAlgorithm< L, Compare >::ReconnectionToOlder;

      }

    }
    else
    {

      // the chosen extremity is too close to the cortex, not a valid
      // proposition
      connectionType =
        gkg::GlobalTractographyAlgorithm< L, Compare >::InvalidConnection;

    }

    return connectionType;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "typename "
             "gkg::GlobalTractographyAlgorithm< L, Compare >::ConnectionType "
             "gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "makeSpinGlassConnectionProposal( "
             "std::map< gkg::Vector3d< float >, "
             "std::pair< std::vector< float >, std::pair< float, float > >, "
             "gkg::Vector3dCompare< float > >& buffer, "
             "gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
             "gkg::SpinGlass::ExtremityType& selectedSpinGlassExtremityType, "
             "const gkg::Vector3d< int32_t >& voxel, "
             "const gkg::CartesianField< std::set< "
             "gkg::RCPointer< gkg::SpinGlass > > >& spinGlassCartesianField, "
             "float temperature, "
             "gkg::RCPointer< gkg::SpinGlassConnection >& "
             "oldSpinGlassConnection, "
             "gkg::RCPointer< gkg::SpinGlass >& oldConnectedSpinGlass, "
             "gkg::SpinGlass::ExtremityType& "
             "oldConnectedSpinGlassExtremityType, "
             "const gkg::BoundingBox< int32_t >& computingMaskBoundingBox, "
             "const gkg::Volume< int16_t >& computingMask, "
             "const gkg::Vector3d< double >& roiResolution, "
             "gkg::OdfContinuousField& odfContinuousField, "
             "const gkg::Transform3d< float >& "
             "transform3dFromRoisVoxelToOdfsReal, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "float globalMinimumOdfProbability, "
             "float globalMaximumOdfProbability, "
             "const gkg::RCPointer< gkg::UniqueIdentityGenerator >& "
             "spinGlassUniqueIdentityGenerator, "
             "std::deque< std::pair< gkg::RCPointer< gkg::SpinGlass >, "
             "gkg::SpinGlass::ExtremityType > >& "
             "neighboringSpinGlassesAndExtremityTypes, "
             "gkg::Vector3d< int32_t >& successorVoxel, "
             "gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass, "
             "gkg::SpinGlass::ExtremityType& successorSpinGlassExtremityType, "
             "std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "isolatedSpinGlasses, "
             "float& oldConnectionEnergy, "
             "float& newSpinGlassExternalEnergy, "
             "float& newConnectionEnergy ) const" );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::
                                        getSuccessorSpinGlassAndComputeEnergies(
     const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
     const gkg::SpinGlass::ExtremityType& selectedSpinGlassExtremityType,
     const gkg::RCPointer< gkg::SpinGlass >& oldConnectedSpinGlass,
     const std::deque< std::pair< gkg::RCPointer< gkg::SpinGlass >,
                                  gkg::SpinGlass::ExtremityType > >&
                                        neighboringSpinGlassesAndExtremityTypes,
     float temperature,
     gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass,
     gkg::SpinGlass::ExtremityType& successorSpinGlassExtremityType,
     float& oldConnectionEnergy,
     float& newConnectionEnergy ) const
{

  try
  {

    // collecting the connection likelihood of the selected spin glass
    float selectedSpinGlassConnectionLikelihood =
      _anatomicalPriors->getConnectionLikelihood(
        selectedSpinGlass->getExtremity( selectedSpinGlassExtremityType ) );


    float selectedSpinGlassExtremityDistanceToPialSurface = 1e38;
    //bool isSelectedSpinGlassExtremityCloseToCortex =
    //    _anatomicalPriors->getDistanceToPialSurface(
    //                          selectedSpinGlass->getExtremity(
    //                                         selectedSpinGlassExtremityType ),
    //                        selectedSpinGlassExtremityDistanceToPialSurface );

    // allocating the vector of connection energies for all the neighbors
    std::vector< float > neighboringConnectionEnergies(
                               neighboringSpinGlassesAndExtremityTypes.size() );
    std::vector< float > neighboringConnectionProbabilities(
                               neighboringSpinGlassesAndExtremityTypes.size() );
    float sumOfConnectionProbabilities = 1.0f;

    std::deque< std::pair< gkg::RCPointer< gkg::SpinGlass >,
                           gkg::SpinGlass::ExtremityType > >::const_iterator
      n = neighboringSpinGlassesAndExtremityTypes.begin(),
      ne = neighboringSpinGlassesAndExtremityTypes.end();
    std::vector< float >::iterator
      e = neighboringConnectionEnergies.begin();
    std::vector< float >::iterator
      p = neighboringConnectionProbabilities.begin();


    // computing the connection energies and unnormalized probabilities
    while ( n != ne )
    {

      const gkg::RCPointer< gkg::SpinGlass >& neighboringSpinGlass = n->first;
      const gkg::SpinGlass::ExtremityType&
        neighboringSpinGlassExtremityType = n->second;

      // collecting the connection likelihood of the neighboring spin glass
      float neighboringSpinGlassConnectionLikelihood =
        _anatomicalPriors->getConnectionLikelihood(
           neighboringSpinGlass->getExtremity( 
                                          neighboringSpinGlassExtremityType ) );


      float neighboringSpinGlassExtremityDistanceToPialSurface = 1e38;
      //bool isNeighboringSpinGlassExtremityCloseToCortex =
      //  _anatomicalPriors->getDistanceToPialSurface(
      //                     neighboringSpinGlass->getExtremity(
      //                                    neighboringSpinGlassExtremityType ),
      //                   neighboringSpinGlassExtremityDistanceToPialSurface );


      // computing the interaction potential between the selected spin glass and
      // the neighboring one
      *e = selectedSpinGlass->getInteractionPotentialWithOther(
                             selectedSpinGlassExtremityType,
                             selectedSpinGlassConnectionLikelihood,
                             neighboringSpinGlass,
                             neighboringSpinGlassExtremityType,
                             neighboringSpinGlassConnectionLikelihood,
                             selectedSpinGlassExtremityDistanceToPialSurface,
                             neighboringSpinGlassExtremityDistanceToPialSurface,
                             _anatomicalPriors->getMinimumDistanceToMesh(),
                             _anatomicalPriors->getMaximumDistanceToMesh() );

      if ( neighboringSpinGlass == oldConnectedSpinGlass )
      {

        oldConnectionEnergy = *e;

      }

      *p = std::exp( - *e / temperature );
      sumOfConnectionProbabilities += *p;

      ++ n;
      ++ e;
      ++ p;

    }

    // normalizing probabilities
    p = neighboringConnectionProbabilities.begin();
    std::vector< float >::iterator
      pe = neighboringConnectionProbabilities.end();
    while ( p != pe )
    {

      *p /=  sumOfConnectionProbabilities;
      ++ p;

    }

    // randomly selecting the successor according to a Gibb's sampler based on
    // the precomputed connection probabilities
    int32_t successorIndex = this->getRandomSelectionFromGibbsSampler(
                                             neighboringConnectionProbabilities,
                                             this->_gibbsTemperature );

    successorSpinGlass = neighboringSpinGlassesAndExtremityTypes[
                                                         successorIndex ].first;
    successorSpinGlassExtremityType = neighboringSpinGlassesAndExtremityTypes[
                                                        successorIndex ].second;

    // collecting new connection energy
    newConnectionEnergy = neighboringConnectionEnergies[ successorIndex ];

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "getSuccessorSpinGlassAndComputeEnergies( "
             "const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
             "const gkg::SpinGlass::ExtremityType& "
             "selectedSpinGlassExtremityType, "
             "const gkg::RCPointer< gkg::SpinGlass >& oldConnectedSpinGlass, "
             "const std::deque< std::pair< gkg::RCPointer< gkg::SpinGlass >, "
             "gkg::SpinGlass::ExtremityType > >& "
             "neighboringSpinGlassesAndExtremityTypes, "
             "float temperature, "
             "gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass, "
             "gkg::SpinGlass::ExtremityType& successorSpinGlassExtremityType, "
             "float& oldConnectionEnergy, "
             "float& newConnectionEnergy ) const" );

}


template < class L, class Compare > 
inline
typename gkg::GlobalTractographyAlgorithm< L, Compare >::ConnectionType
gkg::GlobalTractographyAlgorithm< L, Compare >::
                                     createSuccessorSpinGlassAndComputeEnergies(
     std::map< gkg::Vector3d< float >,
               std::pair< std::vector< float >, std::pair< float, float > >,
               gkg::Vector3dCompare< float > >& buffer,
     const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
     const gkg::SpinGlass::ExtremityType& selectedSpinGlassExtremityType,
     const gkg::Vector3d< double >& roiResolution,
     const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
     const gkg::Volume< int16_t >& computingMask,
     gkg::OdfContinuousField& odfContinuousField,
     const gkg::Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
     const gkg::OrientationSet& outputOrientationSet,
     float globalMinimumOdfProbability,
     float globalMaximumOdfProbability,
     const gkg::RCPointer< gkg::UniqueIdentityGenerator >& 
                                               spinGlassUniqueIdentityGenerator,
     gkg::Vector3d< int32_t >& successorVoxel,
     gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass,
     gkg::SpinGlass::ExtremityType& successorSpinGlassExtremityType,
     float& newSpinGlassExternalEnergy,
     float& newConnectionEnergy ) const
{

  try
  {

    typename gkg::GlobalTractographyAlgorithm< L, Compare >::ConnectionType
      connectionType = 
            gkg::GlobalTractographyAlgorithm< L, Compare >::UndefinedConnection;

    // computing the sign corresponding to the selected spin glass extremity
    // type
    float sign = +1.0f;
    if ( selectedSpinGlassExtremityType == gkg::SpinGlass::Backward )
    {

      sign = -1.0f;

    }

    // getting access to the selected spin glass position
    const gkg::Vector3d< float >&
      selectedSpinGlassPosition = selectedSpinGlass->getPosition();

    // getting access to the selected spin glass orientation
    const gkg::Vector3d< float >&
      selectedSpinGlassOrientation = selectedSpinGlass->getOrientation();

    // getting access to the selected spin glass length information
    float selectedSpinGlassLength = selectedSpinGlass->getLength();
    float selectedSpinGlassHalfLength = selectedSpinGlassLength / 2.0f;
    float selectedSpinGlassQuarterOfLength = selectedSpinGlassLength / 4.0f;

    // getting access to the selected spin glass chosen extremity
    gkg::Vector3d< float >
      selectedSpinGlassExtremity =
              selectedSpinGlass->getExtremity( selectedSpinGlassExtremityType );

    // defining the successor spin glass orientation
    gkg::Vector3d< float > successorSpinGlassOrientation;

    // getting access to the successor spin glass length information
    float successorSpinGlassLength = _spinGlassLength;
    float successorSpinGlassHalfLength = successorSpinGlassLength / 2.0f;
    float successorSpinGlassQuarterOfLength = successorSpinGlassLength / 4.0f;

    // defining the successor spin glass forward extremity
    gkg::Vector3d< float > successorSpinGlassForwardExtremity;

    // defining the successor spin glass backward extremity
    gkg::Vector3d< float > successorSpinGlassBackwardExtremity;

    // defining the forward delta extremities
    gkg::Vector3d< float > forwardDeltaExtremities;

    // defining the backward delta extremities
    gkg::Vector3d< float > backwardDeltaExtremities;

    // defining the radius information
    float radius = 0.0;
    float squaredRadius = 0.0;

    // verifying the existence of anatomical priors
    if ( !_anatomicalPriors->hasPialSurface() )
    {

      // computing the putative successor spin glass position
      float distance = sign * 1.3f * selectedSpinGlassLength;
      gkg::Vector3d< float > successorSpinGlassPosition( 
                                    selectedSpinGlassPosition.x +
                                    selectedSpinGlassOrientation.x * distance,
                                    selectedSpinGlassPosition.y +
                                    selectedSpinGlassOrientation.y * distance,
                                    selectedSpinGlassPosition.z +
                                    selectedSpinGlassOrientation.z * distance );

      // computing the successor voxel
      this->getVoxelFromPosition( successorSpinGlassPosition,
                                  roiResolution,
                                  successorVoxel );

      // need to check that the successor voxel is not out of bounds
      if ( computingMaskBoundingBox.contains( successorVoxel ) &&
           computingMask( successorVoxel ) )
      {

        // computing the odf corresponding to the successor spin glass position
        gkg::RCPointer< gkg::OrientationDistributionFunction > successorOdf;
        this->getOdf( odfContinuousField,
                      successorSpinGlassPosition,
                      roiResolution,
                      transform3dFromRoisVoxelToOdfsReal,
                      successorOdf );

        // checking the existence of the successor odf
        if ( !successorOdf.isNull() )
        {

          // computing the successor spin glass orientation
         this->getSpinGlassOrientationAndLength( selectedSpinGlassOrientation,
                                                 outputOrientationSet,
                                                 *successorOdf,
                                                 successorSpinGlassPosition,
                                                 successorSpinGlassOrientation,
                                                 successorSpinGlassLength );


          float dx = successorSpinGlassOrientation.x *
                     successorSpinGlassHalfLength;
          float dy = successorSpinGlassOrientation.y *
                     successorSpinGlassHalfLength;
          float dz = successorSpinGlassOrientation.z *
                     successorSpinGlassHalfLength;

          // computing the associated successor spin glass forward extremity
          successorSpinGlassForwardExtremity.x =
                                              successorSpinGlassPosition.x + dx;
          successorSpinGlassForwardExtremity.y =
                                              successorSpinGlassPosition.y + dy;
          successorSpinGlassForwardExtremity.z =
                                              successorSpinGlassPosition.z + dz;

          // computing the associated successor spin glass backward extremity
          successorSpinGlassBackwardExtremity.x =
                                              successorSpinGlassPosition.x - dx;
          successorSpinGlassBackwardExtremity.y =
                                              successorSpinGlassPosition.y - dy;
          successorSpinGlassBackwardExtremity.z =
                                              successorSpinGlassPosition.z - dz;

          // defining the forward and backward extremities
          forwardDeltaExtremities.x = selectedSpinGlassExtremity.x -
                                      successorSpinGlassForwardExtremity.x;
          forwardDeltaExtremities.y = selectedSpinGlassExtremity.y -
                                      successorSpinGlassForwardExtremity.y;
          forwardDeltaExtremities.z = selectedSpinGlassExtremity.z -
                                      successorSpinGlassForwardExtremity.z;

          backwardDeltaExtremities.x = selectedSpinGlassExtremity.x -
                                       successorSpinGlassBackwardExtremity.x;
          backwardDeltaExtremities.y = selectedSpinGlassExtremity.y -
                                       successorSpinGlassBackwardExtremity.y;
          backwardDeltaExtremities.z = selectedSpinGlassExtremity.z -
                                       successorSpinGlassBackwardExtremity.z;

          // computing the radius and the squared radius
          radius = selectedSpinGlassQuarterOfLength +
                   successorSpinGlassQuarterOfLength;
          squaredRadius = radius * radius;

          float forwardNorm2 = forwardDeltaExtremities.getNorm2();
          float backwardNorm2 = backwardDeltaExtremities.getNorm2();

          // sanity check
          if ( ( forwardNorm2 > squaredRadius ) && 
               ( backwardNorm2 > squaredRadius ) )
          {

            throw std::runtime_error( "Both extremities are still too far!" );

          }

          // checking which extremity is the closest
          if ( forwardNorm2 <= backwardNorm2 )
          {

            // setting the successor spin glass extremity type to forward
            successorSpinGlassExtremityType = gkg::SpinGlass::Forward;

          }
          else
          {

            // setting the successor spin glass extremity type to backward
            successorSpinGlassExtremityType = gkg::SpinGlass::Backward;

          }

          // creating the successor spin glass
          successorSpinGlass.reset(
            new gkg::SpinGlass( spinGlassUniqueIdentityGenerator->getIdentity(),
                                successorSpinGlassPosition,
                                successorSpinGlassOrientation,
                                successorSpinGlassLength ) );

          // computing the external energy of the new spin glass
          newSpinGlassExternalEnergy = this->getSpinGlassExternalEnergy(
                                                  buffer,
                                                  successorSpinGlassPosition,
                                                  successorSpinGlassOrientation,
                                                  *successorOdf,
                                                  globalMinimumOdfProbability,
                                                  globalMaximumOdfProbability );

          // getting access to the connection likelihoods for both spin glasses
          float selectedSpinGlassConnectionLikelihood =
            _anatomicalPriors->getConnectionLikelihood(
                                                    selectedSpinGlassPosition );
          float successorSpinGlassConnectionLikelihood =
            _anatomicalPriors->getConnectionLikelihood(
                                                   successorSpinGlassPosition );

          // computing the new spin glass connection energy
          newConnectionEnergy =
            selectedSpinGlass->getInteractionPotentialWithOther(
                                selectedSpinGlassExtremityType,
                                selectedSpinGlassConnectionLikelihood,
                                successorSpinGlass,
                                successorSpinGlassExtremityType,
                                successorSpinGlassConnectionLikelihood );

          // setting the connection type to a valid creation connection
          connectionType = gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                        ValidCreationConnection;

        }
        else
        {

          //throw std::runtime_error( "invalid point in the ODF field" );
          // if the successor voxel is out of bounds, setting the connection 
          // type to an invalid creation connection
          connectionType = 
          gkg::GlobalTractographyAlgorithm< L,
                                           Compare >::InvalidCreationConnection;

        }

      }
      else
      {

        // if the successor voxel is out of bounds, setting the connection type
        // to an invalid creation connection
        connectionType = 
         gkg::GlobalTractographyAlgorithm< L,
                                           Compare >::InvalidCreationConnection;

      }

    }
    else
    {

      bool isSelectedSpinGlassExtremityCloseToCortex = false;
      float selectedSpinGlassExtremityDistanceToPialSurface = 10.0;
      gkg::Vector3d< float > selectedSpinGlassExtremityNormalToPialSurface;

      float selectedSpinGlassDistanceToPialSurface = 1e38;
      gkg::Vector3d< float > selectedSpinGlassNormalToPialSurface;
      //bool isSelectedSpinGlassCloseToCortex =
      //  _anatomicalPriors->getDistanceAndNormalToPialSurface(
      //                                   selectedSpinGlassExtremity,
      //                                 selectedSpinGlassDistanceToPialSurface,
      //                                 selectedSpinGlassNormalToPialSurface );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "selectedSpinGlassDistanceToPialSurface = "
         << selectedSpinGlassDistanceToPialSurface << std::endl;
      os << "selectedSpinGlassNormalToPialSurface : "
         << selectedSpinGlassNormalToPialSurface << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

      gkg::Vector3d< int32_t > selectedSpinGlassVoxel;
      this->getVoxelFromPosition( selectedSpinGlassPosition,
                                  roiResolution,
                                  selectedSpinGlassVoxel );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "selectedSpinGlassVoxel : " << selectedSpinGlassVoxel << std::endl;
      os << "selectedSpinGlassPosition : "
         << selectedSpinGlassPosition << std::endl;
      os << "selectedSpinGlassOrientation : "
         << selectedSpinGlassOrientation << std::endl;
      os << "selectedSpinGlassExtremity : "
         << selectedSpinGlassExtremity << std::endl;
      if ( selectedSpinGlassExtremityType == gkg::SpinGlass::Forward )
      {

        os << "selectedSpinGlassOtherExtremity : "
           << selectedSpinGlass->getExtremity( gkg::SpinGlass::Backward )
           << std::endl;

      }
      else
      {

        os << "selectedSpinGlassOtherExtremity : "
           << selectedSpinGlass->getExtremity( gkg::SpinGlass::Forward )
           << std::endl;

      }
      os << "selectedSpinGlassLength : "
         << selectedSpinGlassLength << std::endl;
      os << "selectedSpinGlassHalfLength : "
         << selectedSpinGlassHalfLength << std::endl;
      os << "selectedSpinGlassQuarterOfLength : "
         << selectedSpinGlassQuarterOfLength << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

      isSelectedSpinGlassExtremityCloseToCortex =
        _anatomicalPriors->getDistanceAndNormalToPialSurface(
                                selectedSpinGlassExtremity,
                                selectedSpinGlassExtremityDistanceToPialSurface,
                                selectedSpinGlassExtremityNormalToPialSurface );


      if ( ( isSelectedSpinGlassExtremityCloseToCortex &&
             ( selectedSpinGlassExtremityDistanceToPialSurface >
               _anatomicalPriors->getMinimumDistanceToMesh() ) ) ||
           !isSelectedSpinGlassExtremityCloseToCortex )
      {

        // computing the odf corresponding to the successor spin glass position
        gkg::RCPointer< gkg::OrientationDistributionFunction > 
          selectedSpinGlassExtremityOdf;
        this->getOdf( odfContinuousField,
                      selectedSpinGlassExtremity,
                      roiResolution,
                      transform3dFromRoisVoxelToOdfsReal,
                      selectedSpinGlassExtremityOdf );

        // computing the orientation driven by the Gibb's temperature and the
        // spin glass external energy only if the odf is valid
        if ( !selectedSpinGlassExtremityOdf.isNull() )
        {

          // computing the successor spin glass orientation
          this->getSpinGlassOrientationAndLength(
                                                 selectedSpinGlassOrientation,
                                                 outputOrientationSet,
                                                 *selectedSpinGlassExtremityOdf,
                                                 selectedSpinGlassExtremity,
                                                 successorSpinGlassOrientation,
                                                 successorSpinGlassLength );
          float distance = successorSpinGlassLength * 0.5f;
          float signedDistance = sign * distance;
          gkg::Vector3d< float > successorSpinGlassPosition( 
                             selectedSpinGlassExtremity.x +
                             successorSpinGlassOrientation.x * signedDistance,
                             selectedSpinGlassExtremity.y +
                             successorSpinGlassOrientation.y * signedDistance,
                             selectedSpinGlassExtremity.z +
                             successorSpinGlassOrientation.z * signedDistance );

          this->getVoxelFromPosition( successorSpinGlassPosition,
                                      roiResolution,
                                      successorVoxel );

          // computing extremities and associated norms
          gkg::Vector3d< float > successorSpinGlassFwdExtremity(
                                   successorSpinGlassPosition.x +
                                   successorSpinGlassOrientation.x * distance,
                                   successorSpinGlassPosition.y +
                                   successorSpinGlassOrientation.y * distance,
                                   successorSpinGlassPosition.z +
                                   successorSpinGlassOrientation.z * distance );
          gkg::Vector3d< float > successorSpinGlassBwdExtremity(
                                   successorSpinGlassPosition.x -
                                   successorSpinGlassOrientation.x * distance,
                                   successorSpinGlassPosition.y -
                                   successorSpinGlassOrientation.y * distance,
                                   successorSpinGlassPosition.z -
                                   successorSpinGlassOrientation.z * distance );

          float fx = selectedSpinGlassExtremity.x -
                     successorSpinGlassFwdExtremity.x;
          float fy = selectedSpinGlassExtremity.y -
                     successorSpinGlassFwdExtremity.y;
          float fz = selectedSpinGlassExtremity.z -
                     successorSpinGlassFwdExtremity.z;
          float normFwd = fx * fx + fy * fy + fz * fz;

          float bx = selectedSpinGlassExtremity.x -
                     successorSpinGlassBwdExtremity.x;
          float by = selectedSpinGlassExtremity.y -
                     successorSpinGlassBwdExtremity.y;
          float bz = selectedSpinGlassExtremity.z -
                     successorSpinGlassBwdExtremity.z;
          float normBwd = bx * bx + by * by + bz * bz;

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "principalOrientation : "
             << selectedSpinGlassExtremityOdf->getPrincipalOrientation()
             << std::endl;
          os << "successorSpinGlassOrientation : "
             << successorSpinGlassOrientation << std::endl;
          os << "successorSpinGlassLength : "
             << successorSpinGlassLength << std::endl;
          os << "successorSpinGlassPosition : "
             << successorSpinGlassPosition << std::endl;
          os << "successorSpinGlassFwdExtremity : "
             << successorSpinGlassFwdExtremity << std::endl;
          os << "successorSpinGlassBwdExtremity : "
             << successorSpinGlassBwdExtremity << std::endl;
          os << "normFwd : " << normFwd << std::endl;
          os << "normBwd : " << normBwd << std::endl;
          os << "successorVoxel : " << successorVoxel << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------


          if ( computingMaskBoundingBox.contains( successorVoxel ) &&
               computingMask( successorVoxel ) )
          {

            // creating the successor spin glass
            successorSpinGlass.reset(
              new gkg::SpinGlass(
                            spinGlassUniqueIdentityGenerator->getIdentity(),
                            successorSpinGlassPosition,
                            successorSpinGlassOrientation,
                            successorSpinGlassLength ) );

            successorSpinGlassExtremityType =
                                     gkg::SpinGlass::getOppositeExtremityType(
                                               selectedSpinGlassExtremityType );

            //------------------------------------------------------------------
            BEGIN_DEBUG
            if ( successorSpinGlassExtremityType == gkg::SpinGlass::Forward )
            {

              os << "FORWARD" << std::endl;

            }
            else
            {

              os << "BACKWARD" << std::endl;

            }
            END_DEBUG
            //------------------------------------------------------------------


            // computing the external energy of the new spin glass
            newSpinGlassExternalEnergy = this->getSpinGlassExternalEnergy(
                                                buffer,
                                                successorSpinGlassPosition,
                                                successorSpinGlassOrientation,
                                                *selectedSpinGlassExtremityOdf,
                                                globalMinimumOdfProbability,
                                                globalMaximumOdfProbability );

            //----------------------------------------------------------------
            BEGIN_DEBUG
            os << "newSpinGlassExternalEnergy = "
               << newSpinGlassExternalEnergy  << std::endl;
            END_DEBUG
            //----------------------------------------------------------------

            // getting access to the connection likelihoods for both spin
            // glasses
            float selectedSpinGlassConnectionLikelihood =
              _anatomicalPriors->getConnectionLikelihood(
                                                   selectedSpinGlassExtremity );
            float successorSpinGlassConnectionLikelihood =
              _anatomicalPriors->getConnectionLikelihood(
                       successorSpinGlass->getExtremity( 
                                            successorSpinGlassExtremityType ) );

            //----------------------------------------------------------------
            BEGIN_DEBUG
            os << "selectedSpinGlassConnectionLikelihood = "
               << selectedSpinGlassConnectionLikelihood << std::endl;
            os << "successorSpinGlassConnectionLikelihood = "
               << successorSpinGlassConnectionLikelihood << std::endl;
            END_DEBUG
            //----------------------------------------------------------------

            float successorSpinGlassExtremityDistanceToPialSurface = 1e38;
            //bool isSucessorSpinGlassExtremityCloseToCortex =
            //  _anatomicalPriors->getDistanceToPialSurface(
            //                  successorSpinGlass->getExtremity( 
            //                                successorSpinGlassExtremityType ),
            //                selectedSpinGlassExtremityDistanceToPialSurface );

            // computing the new spin glass connection energy
            newConnectionEnergy =
              selectedSpinGlass->getInteractionPotentialWithOther(
                                     selectedSpinGlassExtremityType,
                                     selectedSpinGlassConnectionLikelihood,
                                     successorSpinGlass,
                                     successorSpinGlassExtremityType,
                                     successorSpinGlassConnectionLikelihood,
                             selectedSpinGlassExtremityDistanceToPialSurface,
                             successorSpinGlassExtremityDistanceToPialSurface,
                             _anatomicalPriors->getMinimumDistanceToMesh(),
                             _anatomicalPriors->getMaximumDistanceToMesh() );

            //----------------------------------------------------------------
            BEGIN_DEBUG
            os << "newConnectionEnergy = "
               << newConnectionEnergy  << std::endl;
            END_DEBUG
            //----------------------------------------------------------------

            // setting the connection type to a valid creation connection
            connectionType = gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                        ValidCreationConnection;

          }
          else
          {

            // if the successor voxel is out of bounds, setting the connection
            // type to an invalid creation connection
            connectionType = 
              gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                      InvalidCreationConnection;

          }

        }
        else
        {

          //throw std::runtime_error( "invalid point in the ODF field" );
          // if the successor voxel is out of bounds, setting the connection type
          // to an invalid creation connection
          connectionType = 
           gkg::GlobalTractographyAlgorithm< L,
                                           Compare >::InvalidCreationConnection;

        }

      }
      else
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "Successor spin glass out of bounds or too close to the cortex!"
           << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        // if the successor voxel is out of bounds, setting the connection type
        // to an invalid creation connection
        connectionType = 
          gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                      InvalidCreationConnection;

      }

    }

    return connectionType;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "typename gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "ConnectionType"
             "gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "createSuccessorSpinGlassAndComputeEnergies( "
             "std::map< gkg::Vector3d< float >, "
             "std::pair< std::vector< float >, std::pair< float, float > >, "
             "gkg::Vector3dCompare< float > >& buffer, "
             "const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
             "const gkg::SpinGlass::ExtremityType& "
             "selectedSpinGlassExtremityType, "
             "const gkg::Vector3d< double >& roiResolution, "
             "const gkg::BoundingBox< int32_t >& computingMaskBoundingBox, "
             "const gkg::Volume< int16_t >& computingMask, "
             "gkg::OdfContinuousField& odfContinuousField, "
             "const gkg::Transform3d< float >& "
             "transform3dFromRoisVoxelToOdfsReal, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "float globalMinimumOdfProbability, "
             "float globalMaximumOdfProbability, "
             "const gkg::RCPointer< gkg::UniqueIdentityGenerator >& "
             "spinGlassUniqueIdentityGenerator, "
             "gkg::Vector3d< int32_t >& successorVoxel, "
             "gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass, "
             "gkg::SpinGlass::ExtremityType& successorSpinGlassExtremityType, "
             "float& newSpinGlassExternalEnergy, "
             "float& newConnectionEnergy ) const" );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::createSpinGlassConnection(
     const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
     const gkg::SpinGlass::ExtremityType& selectedSpinGlassExtremityType,
     const gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass,
     const gkg::SpinGlass::ExtremityType& successorSpinGlassExtremityType,
     const gkg::RCPointer< gkg::UniqueIdentityGenerator >&
                                     spinGlassConnectionUniqueIdentityGenerator,
     std::set< gkg::RCPointer< gkg::SpinGlassConnection > >&
                                                    spinGlassConnections ) const
{

  try
  {

    // creating the new spin glass connection
    gkg::RCPointer< gkg::SpinGlassConnection > 
      newSpinGlassConnection(
        new gkg::SpinGlassConnection(
                      spinGlassConnectionUniqueIdentityGenerator->getIdentity(),
                      std::make_pair( selectedSpinGlass,
                                      selectedSpinGlassExtremityType ),
                      std::make_pair( successorSpinGlass,
                                      successorSpinGlassExtremityType ) ) );

    /////////////////////////---
    selectedSpinGlass->setSpinGlassConnection( selectedSpinGlassExtremityType,
                                               newSpinGlassConnection );
    successorSpinGlass->setSpinGlassConnection( successorSpinGlassExtremityType,
                                                newSpinGlassConnection );

    // checking consistency for both spin glasses
    selectedSpinGlass->checkConsistency();
    successorSpinGlass->checkConsistency();

    // inserting the new spin glass connection in the set
    spinGlassConnections.insert( newSpinGlassConnection );

    // sending the spin glass connection creation command to the viewer if
    // connected
    if ( _writer->isConnected() )
    {

      gkg::SpinGlassConnectionCreationCommand
        spinGlassConnectionCreationCommand( newSpinGlassConnection );
      _writer->send( spinGlassConnectionCreationCommand );
      _writer->flush();

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "createSpinGlassConnection( "
             "const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
             "const gkg::SpinGlass::ExtremityType& "
             "selectedSpinGlassExtremityType, "
             "const gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass, "
             "const gkg::SpinGlass::ExtremityType& "
             "successorSpinGlassExtremityType, "
             "const gkg::RCPointer< gkg::UniqueIdentityGenerator >& "
             "spinGlassConnectionUniqueIdentityGenerator, "
             "std::set< gkg::RCPointer< gkg::SpinGlassConnection > >& "
             "spinGlassConnections ) const" );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::removeSpinGlassConnection(
       const gkg::RCPointer< gkg::SpinGlassConnection >& spinGlassConnection,
       std::set< gkg::RCPointer< gkg::SpinGlassConnection > >&
                                                    spinGlassConnections ) const
{

  try
  {

    // checking that the given spin glass connection is not null and then
    // erasing it from the set of spin glass connections
    if ( !spinGlassConnection.isNull() )
    {

      if ( spinGlassConnections.find( spinGlassConnection ) !=
           spinGlassConnections.end() )
      {

        spinGlassConnections.erase( spinGlassConnection );

        //keyboardHit();

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "old spin glass connection removed" << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

      }
      else
      {

        // we should never enter this 
        throw std::runtime_error( "spin glass connection not found" );

      }

      // sending the spin glass connection deletion command to the writer if
      // connected 
      if ( _writer->isConnected() )
      {

        gkg::SpinGlassConnectionDeletionCommand
          spinGlassConnectionDeletionCommand( spinGlassConnection );
        _writer->send( spinGlassConnectionDeletionCommand );
        _writer->flush();

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "removeSpinGlassConnection( "
             "const gkg::RCPointer< gkg::SpinGlassConnection >& "
             "spinGlassConnection, "
             "std::set< gkg::RCPointer< gkg::SpinGlassConnection > >& "
             "spinGlassConnections ) const" );

}


#endif


