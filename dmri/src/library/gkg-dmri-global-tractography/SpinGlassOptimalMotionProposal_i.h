#ifndef _gkg_dmri_global_tractography_SpinGlassOptimalMotionProposal_i_h_
#define _gkg_dmri_global_tractography_SpinGlassOptimalMotionProposal_i_h_


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// SPIN GLASS OPTIMAL MOTION PROPOSAL
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


template < class L, class Compare > 
inline
bool 
gkg::GlobalTractographyAlgorithm< L, Compare >::
                                             makeSpinGlassOptimalMotionProposal(
         std::map< gkg::Vector3d< float >,
                   std::pair< std::vector< float >, std::pair< float, float > >,
                   gkg::Vector3dCompare< float > >& buffer,
         const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
         gkg::RCPointer< gkg::SpinGlass >& alternativeSpinGlass,
         const gkg::Vector3d< double >& roiResolution,
         gkg::OdfContinuousField& odfContinuousField,
         const gkg::Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
         float globalMinimumOdfProbability,
         float globalMaximumOdfProbability,
         const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
         const gkg::Volume< int16_t >& computingMask,
         gkg::Vector3d< float >& oldSpinGlassPosition,
         gkg::Vector3d< float >& oldSpinGlassOrientation,
         gkg::Vector3d< int32_t >& newVoxel,
         gkg::Vector3d< float >& newSpinGlassPosition,
         gkg::Vector3d< float >& newSpinGlassOrientation,
         float& oldSpinGlassExternalEnergy,
         float& newSpinGlassExternalEnergy,
         float& oldSpinGlassConnectionEnergy,
         float& newSpinGlassConnectionEnergy ) const
{

  try
  {

    bool isProposalValid = true;

    // find the best spin glass to optimally move it
    this->chooseSpinGlassAmongClique( selectedSpinGlass,
                                      roiResolution,
                                      computingMaskBoundingBox,
                                      computingMask,
                                      alternativeSpinGlass );

    // sanity check : should never happen
    if ( alternativeSpinGlass.isNull() )
    {

      // throw std::runtime_error( "alternative spin glass is null" );
      isProposalValid = true;
      return isProposalValid;

    }

    // storing the old spin glass position and orientation
    oldSpinGlassPosition = alternativeSpinGlass->getPosition();
    oldSpinGlassOrientation = alternativeSpinGlass->getOrientation();

    bool isFarFromCortex = true;
    if ( _anatomicalPriors->hasPialSurface() )
    {

      //checking if the alternative spin glass is far from the cortex
      float distanceToPialSurface = 0.0f;
      bool isAlternativeSpinGlassCloseToCortex =
        _anatomicalPriors->getDistanceToPialSurface( oldSpinGlassPosition,
                                                     distanceToPialSurface );

      isFarFromCortex = !isAlternativeSpinGlassCloseToCortex ||
                        ( distanceToPialSurface >
                          2.0 * _anatomicalPriors->getMaximumDistanceToMesh() );

    }


    // adding condition for the spin to be in the middle of some spins
    std::list< gkg::RCPointer< gkg::SpinGlass > >
      alternativeSpinGlassNeighborhood =
        gkg::SpinGlass::getNeighborhood( alternativeSpinGlass, -1 );


    // proposing the optimal move only if the spin glass is far from the cortex
    if ( isFarFromCortex &&
         ( alternativeSpinGlassNeighborhood.size() >= _cliqueNeighborCount ) &&
         !alternativeSpinGlass->isAnExtremity() )
    {

      // computing the odf corresponding to the old spin glass position
      gkg::RCPointer< gkg::OrientationDistributionFunction > oldOdf;
      this->getOdf( odfContinuousField,
                    oldSpinGlassPosition,
                    roiResolution,
                    transform3dFromRoisVoxelToOdfsReal,
                    oldOdf );

      // sanity check : should never happen
      if ( oldOdf.isNull() )
      {

        //throw std::runtime_error( "odf null at the old spin glass position" );
        isProposalValid = false;
        return isProposalValid;

      }

      // computing the old spin glass external energy
      oldSpinGlassExternalEnergy =
        this->getSpinGlassExternalEnergy( buffer,
                                          oldSpinGlassPosition,
                                          oldSpinGlassOrientation,
                                          *oldOdf,
                                          globalMinimumOdfProbability,
                                          globalMaximumOdfProbability );

      // computing the old spin glass connection likelihood
      /*float oldSelectedSpinGlassConnectionLikelihood =
           _anatomicalPriors->getConnectionLikelihood( oldSpinGlassPosition );*/

///////
      float oldSelectedSpinGlassForwardExtremityConnectionLikelihood =
             _anatomicalPriors->getConnectionLikelihood(
                alternativeSpinGlass->getExtremity( gkg::SpinGlass::Forward ) );
      float oldSelectedSpinGlassBackwardExtremityConnectionLikelihood =
             _anatomicalPriors->getConnectionLikelihood(
               alternativeSpinGlass->getExtremity( gkg::SpinGlass::Backward ) );
///////

      // defining the forward spin glass connection likelihood and its
      // associated extremity type
      float forwardSpinGlassConnectionLikelihood = 0.0f;
      gkg::SpinGlass::ExtremityType
        forwardSpinGlassExtremityType = gkg::SpinGlass::Forward;

      // retrieving the putative spin glass connected at the forward extremity
      const gkg::RCPointer< gkg::SpinGlass >&
        forwardSpinGlass = alternativeSpinGlass->getConnectedSpinGlass(
                                                      gkg::SpinGlass::Forward );

      // checking if the forward spin glass is null
      if ( !forwardSpinGlass.isNull() )
      {

        if ( forwardSpinGlass->getConnectedSpinGlass(
                            gkg::SpinGlass::Backward ) == alternativeSpinGlass )
        {

          forwardSpinGlassExtremityType = gkg::SpinGlass::Backward;

///////
          forwardSpinGlassConnectionLikelihood =
            _anatomicalPriors->getConnectionLikelihood( 
                   forwardSpinGlass->getExtremity( gkg::SpinGlass::Backward ) );
///////

        }

        // computing the spin glass connection likelihood for the forward spin
        // glass
        /*forwardSpinGlassConnectionLikelihood =
          _anatomicalPriors->getConnectionLikelihood( 
                                            forwardSpinGlass->getPosition() );*/

///////
          forwardSpinGlassConnectionLikelihood =
            _anatomicalPriors->getConnectionLikelihood( 
                    forwardSpinGlass->getExtremity( gkg::SpinGlass::Forward ) );
///////


        // updating the new spin glass connection energy with the interaction
        // potential at the forward extremity
        oldSpinGlassConnectionEnergy +=
          alternativeSpinGlass->getInteractionPotentialWithOther(
                       gkg::SpinGlass::Forward,
                       oldSelectedSpinGlassForwardExtremityConnectionLikelihood,
                       forwardSpinGlass,
                       forwardSpinGlassExtremityType,
                       forwardSpinGlassConnectionLikelihood );

      }

      // defining the backward spin glass connection likelihood and its
      // associated extremity type
      float backwardSpinGlassConnectionLikelihood = 0.0f;
      gkg::SpinGlass::ExtremityType
        backwardSpinGlassExtremityType = gkg::SpinGlass::Forward;

      // retrieving the putative spin glass connected at the backward extremity
      const gkg::RCPointer< gkg::SpinGlass >&
        backwardSpinGlass = alternativeSpinGlass->getConnectedSpinGlass(
                                                     gkg::SpinGlass::Backward );
      // checking if the backward spin glass is null
      if ( !backwardSpinGlass.isNull() )
      {

        if ( backwardSpinGlass->getConnectedSpinGlass(
                            gkg::SpinGlass::Backward ) == alternativeSpinGlass )
        {

          backwardSpinGlassExtremityType = gkg::SpinGlass::Backward;

///////
          backwardSpinGlassConnectionLikelihood =
            _anatomicalPriors->getConnectionLikelihood( 
                  backwardSpinGlass->getExtremity( gkg::SpinGlass::Backward ) );
///////

        }

        // computing the spin glass connection likelihood for the backward spin
        // glass
        /*backwardSpinGlassConnectionLikelihood =
          _anatomicalPriors->getConnectionLikelihood( 
                                           backwardSpinGlass->getPosition() );*/

///////
        backwardSpinGlassConnectionLikelihood =
          _anatomicalPriors->getConnectionLikelihood( 
                   backwardSpinGlass->getExtremity( gkg::SpinGlass::Forward ) );
///////

        // updating the new spin glass connection energy with the interaction
        // potential at the backward extremity
        oldSpinGlassConnectionEnergy +=
          alternativeSpinGlass->getInteractionPotentialWithOther(
                      gkg::SpinGlass::Backward,
                      oldSelectedSpinGlassBackwardExtremityConnectionLikelihood,
                      backwardSpinGlass,
                      backwardSpinGlassExtremityType,
                      backwardSpinGlassConnectionLikelihood );

      }

      // computing the optimal spin glass position and orientation
      alternativeSpinGlass->getOptimalPositionAndOrientation(
                                                      newSpinGlassPosition,
                                                      newSpinGlassOrientation );

      // computing the new voxel corresponding to the new spin glass position
      this->getVoxelFromPosition( newSpinGlassPosition,
                                  roiResolution,
                                  newVoxel );

      // nothing to do if the new voxel is out of bounds
      if ( computingMaskBoundingBox.contains( newVoxel ) &&
           computingMask( newVoxel ) )
      {

        // moving the spin glass in an optimal way
        alternativeSpinGlass->setPosition( newSpinGlassPosition );
        alternativeSpinGlass->setOrientation( newSpinGlassOrientation );

        // computing the odf for the new spin glass position
        gkg::RCPointer< gkg::OrientationDistributionFunction > newOdf;
        this->getOdf( odfContinuousField,
                      newSpinGlassPosition,
                      roiResolution,
                      transform3dFromRoisVoxelToOdfsReal,
                      newOdf );

        // sanity check : should never happen
        if ( newOdf.isNull() )
        {

          //throw std::runtime_error( "odf null at the new spin glass position" );
          isProposalValid = false;
          return isProposalValid;

        }

        // computing the new spin glass external energy
        newSpinGlassExternalEnergy =
          this->getSpinGlassExternalEnergy( buffer,
                                            newSpinGlassPosition,
                                            newSpinGlassOrientation,
                                            *newOdf,
                                            globalMinimumOdfProbability,
                                            globalMaximumOdfProbability );

        // computing the new spin glass connection likelihood
        float newSelectedSpinGlassForwardExtremityConnectionLikelihood =
               _anatomicalPriors->getConnectionLikelihood(
                alternativeSpinGlass->getExtremity( gkg::SpinGlass::Forward ) );
        float newSelectedSpinGlassBackwardExtremityConnectionLikelihood =
             _anatomicalPriors->getConnectionLikelihood(
               alternativeSpinGlass->getExtremity( gkg::SpinGlass::Backward ) );

        // checking if the forward spin glass is null
        if ( !forwardSpinGlass.isNull() )
        {

          // updating the new spin glass connection energy with the interaction
          // potential at the forward extremity
          newSpinGlassConnectionEnergy +=
            alternativeSpinGlass->getInteractionPotentialWithOther(
                       gkg::SpinGlass::Forward,
                       newSelectedSpinGlassForwardExtremityConnectionLikelihood,
                       forwardSpinGlass,
                       forwardSpinGlassExtremityType,
                       forwardSpinGlassConnectionLikelihood );

        }

        // checking if the backward spin glass is null
        if ( !backwardSpinGlass.isNull() )
        {

          // updating the new spin glass connection energy with the interaction
          // potential at the backward extremity
          newSpinGlassConnectionEnergy +=
            alternativeSpinGlass->getInteractionPotentialWithOther(
                      gkg::SpinGlass::Backward,
                      newSelectedSpinGlassBackwardExtremityConnectionLikelihood,
                      backwardSpinGlass,
                      backwardSpinGlassExtremityType,
                      backwardSpinGlassConnectionLikelihood );

        }

        // setting back to the old position and orientation of the spin glass
        alternativeSpinGlass->setPosition( oldSpinGlassPosition );
        alternativeSpinGlass->setOrientation( oldSpinGlassOrientation );

      }
      else
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "new voxel out of tractography mask" << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------
        isProposalValid = false;

      }

    }
    else
    {

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "selected spin glass too close to the cortex" << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------
      isProposalValid = false;

    }

    return isProposalValid;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool "
             "gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "makeSpinGlassOptimalMotionProposal( "
             "std::map< gkg::Vector3d< float >, "
             "std::pair< std::vector< float >, std::pair< float, float > >, "
             "gkg::Vector3dCompare< float > >& buffer, "
             "const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
             "gkg::RCPointer< gkg::SpinGlass >& alternativeSpinGlass, "
             "const gkg::Vector3d< double >& roiResolution, "
             "gkg::OdfContinuousField& odfContinuousField, "
             "const gkg::Transform3d< float >& "
             "transform3dFromRoisVoxelToOdfsReal, "
             "float globalMinimumOdfProbability, "
             "float globalMaximumOdfProbability, "
             "const gkg::BoundingBox< int32_t >& computingMaskBoundingBox, "
             "const gkg::Volume< int16_t >& computingMask, "
             "gkg::Vector3d< float >& oldSpinGlassPosition, "
             "gkg::Vector3d< float >& oldSpinGlassOrientation, "
             "gkg::Vector3d< int32_t >& newVoxel, "
             "gkg::Vector3d< float >& newSpinGlassPosition, "
             "gkg::Vector3d< float >& newSpinGlassOrientation, "
             "float& oldSpinGlassExternalEnergy, "
             "float& newSpinGlassExternalEnergy, "
             "float& oldSpinGlassConnectionEnergy, "
             "float& newSpinGlassConnectionEnergy ) const" );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::chooseSpinGlassAmongClique(
                  const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
                  const gkg::Vector3d< double >& roiResolution,
                  const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
                  const gkg::Volume< int16_t >& computingMask,
                  gkg::RCPointer< gkg::SpinGlass >& alternativeSpinGlass ) const
{

  try
  {

    bool isSpinGlassCloseToCortex = false;
    float distanceToPialSurface = 0.0;
    gkg::Vector3d< float > normalToPialSurface;

    // extracting neighborhood around selected spin glass
    std::list< gkg::RCPointer< gkg::SpinGlass > >
      neighborhood = gkg::SpinGlass::getNeighborhood( selectedSpinGlass,
                                                      _cliqueNeighborCount );

    //--------------------------------------------------------------------------
    BEGIN_DEBUG
    os << "neighborhood size = " << neighborhood.size() << std::endl;
    END_DEBUG
    //--------------------------------------------------------------------------

    // if the clique size is less than 2, we automatically choose the selected
    // spin glass
    if ( neighborhood.size() > 2U )
    {

      gkg::Vector3d< float > oldSpinGlassPosition;
      gkg::Vector3d< float > oldSpinGlassOrientation;
      gkg::Vector3d< float > newSpinGlassPosition;
      gkg::Vector3d< float > newSpinGlassOrientation;
      gkg::Vector3d< int32_t > newVoxel;

      gkg::RCPointer< gkg::SpinGlass >
        optimalSelectedSpinGlass = selectedSpinGlass;
      float tortuosity = 1e38;

      std::list< gkg::RCPointer< gkg::SpinGlass > >::const_iterator
        s = neighborhood.begin(),
        se = neighborhood.end();

      while ( s != se )
      {

        gkg::RCPointer< gkg::SpinGlass > currentSelectedSpinGlass = *s;
        const gkg::Vector3d< float >&
          currentSpinGlassPosition = currentSelectedSpinGlass->getPosition();

        // computing the distance and normal to cortex only if the pial surface
        // is given
        isSpinGlassCloseToCortex =
          _anatomicalPriors->getDistanceToPialSurface( currentSpinGlassPosition,
                                                       distanceToPialSurface );

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "distanceToPialSurface = "
           << distanceToPialSurface << std::endl;
        END_DEBUG
       //----------------------------------------------------------------------

        // selecting the best spin glass regarding its impact on tortuosity
        if ( ( !_anatomicalPriors->hasPialSurface() ) ||
             ( !isSpinGlassCloseToCortex ||
               ( distanceToPialSurface >
                 _anatomicalPriors->getMaximumDistanceToMesh() ) ) )
        {

          // storing the old spin glass position and orientation
          const gkg::Vector3d< float >& currentSelectedSpinGlassPosition =
                                        currentSelectedSpinGlass->getPosition();
          oldSpinGlassPosition.x = currentSelectedSpinGlassPosition.x;
          oldSpinGlassPosition.y = currentSelectedSpinGlassPosition.y;
          oldSpinGlassPosition.z = currentSelectedSpinGlassPosition.z;

          const gkg::Vector3d< float >& currentSelectedSpinGlassOrientation =
                                     currentSelectedSpinGlass->getOrientation();
          oldSpinGlassOrientation.x = currentSelectedSpinGlassOrientation.x;
          oldSpinGlassOrientation.y = currentSelectedSpinGlassOrientation.y;
          oldSpinGlassOrientation.z = currentSelectedSpinGlassOrientation.z;

          // computing the optimal spin glass position and orientation
          currentSelectedSpinGlass->getOptimalPositionAndOrientation(
                                                      newSpinGlassPosition,
                                                      newSpinGlassOrientation );

          // computing the new voxel corresponding to the new spin glass
          // position
          this->getVoxelFromPosition( newSpinGlassPosition,
                                      roiResolution,
                                      newVoxel );

          // nothing to do if the new voxel is out of bounds
          if ( computingMaskBoundingBox.contains( newVoxel ) &&
               computingMask( newVoxel ) )
          {

            // moving the spin glass in an optimal way
            currentSelectedSpinGlass->setPosition( newSpinGlassPosition );
            currentSelectedSpinGlass->setOrientation( newSpinGlassOrientation );

            gkg::LightCurve3d< float > fiberPortion;
            selectedSpinGlass->track( fiberPortion, _cliqueNeighborCount );

            float currentTortuosity = fiberPortion.getTortuosity();
            if ( currentTortuosity < tortuosity )
            {

              optimalSelectedSpinGlass = currentSelectedSpinGlass;
              tortuosity = currentTortuosity;

            }
            currentSelectedSpinGlass->setPosition( oldSpinGlassPosition );
            currentSelectedSpinGlass->setOrientation( oldSpinGlassOrientation );

          }

        }
        ++ s;

      }
      alternativeSpinGlass = optimalSelectedSpinGlass;

    }
    else
    {

      alternativeSpinGlass = selectedSpinGlass;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "chooseSpinGlassAmongClique( "
             "const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
             "const gkg::Vector3d< double >& roiResolution, "
             "const gkg::BoundingBox< int32_t >& computingMaskBoundingBox, "
             "const gkg::Volume< int16_t >& computingMask, "
             "gkg::RCPointer< gkg::SpinGlass >& alternativeSpinGlass ) const" );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::
                                           acceptSpinGlassOptimalMotionProposal(
      const gkg::Vector3d< int32_t >& voxel,
      gkg::CartesianField< std::set< gkg::RCPointer< gkg::SpinGlass > > >&
                                                        spinGlassCartesianField,
      gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
      const gkg::Vector3d< int32_t >& newVoxel,
      const gkg::Vector3d< float >& newSpinGlassPosition,
      const gkg::Vector3d< float >& newSpinGlassOrientation ) const
{

  try
  {

    // setting the new position and orientation of the spin glass
    selectedSpinGlass->setPosition( newSpinGlassPosition );
    selectedSpinGlass->setOrientation( newSpinGlassOrientation );

    // checking if the old and new spin glasses belong to the same voxel
    if ( newVoxel != voxel )
    {

      if ( !spinGlassCartesianField.getItem( voxel ) ||
           !spinGlassCartesianField.getItem( newVoxel ) )
      {

        throw std::runtime_error( "null spin glass set : should never happen" );

      }

      // erasing the spin glass from the spin glass set corresponding to the
      // old voxel...
      if ( spinGlassCartesianField.getItem( voxel )->erase( selectedSpinGlass )
           != 1U )
      {

        throw std::runtime_error( 
                   "selected spin glass not removed from the Cartesian field" );

      }

      // and inserting it in the spin glass set corresponding to the new voxel
      spinGlassCartesianField.getItem( newVoxel )->insert( selectedSpinGlass );

    }

    // sending the spin glass move command to the writer if connected
    if ( _writer->isConnected() )
    {

      gkg::SpinGlassMoveCommand spinGlassMoveCommand( selectedSpinGlass );
      _writer->send( spinGlassMoveCommand );
      _writer->flush();

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "acceptSpinGlassOptimalMotionProposal( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "gkg::CartesianField< std::set< "
             "gkg::RCPointer< gkg::SpinGlass > > >& "
             "spinGlassCartesianField, "
             "gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
             "const gkg::Vector3d< int32_t >& newVoxel, "
             "const gkg::Vector3d< float >& newSpinGlassPosition, "
             "const gkg::Vector3d< float >& newSpinGlassOrientation ) const" );

}




#endif


