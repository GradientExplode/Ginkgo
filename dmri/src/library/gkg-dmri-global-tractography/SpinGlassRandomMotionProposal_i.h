#ifndef _gkg_dmri_global_tractography_SpinGlassRandomMotionProposal_i_h_
#define _gkg_dmri_global_tractography_SpinGlassRandomMotionProposal_i_h_


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// SPIN GLASS RANDOM MOTION PROPOSAL
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


template < class L, class Compare > 
inline
bool 
gkg::GlobalTractographyAlgorithm< L, Compare >::
                                              makeSpinGlassRandomMotionProposal(
         std::map< gkg::Vector3d< float >,
                   std::pair< std::vector< float >, std::pair< float, float > >,
                   gkg::Vector3dCompare< float > >& buffer,
         gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
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

    // storing the old spin glass position and orientation
    oldSpinGlassPosition = selectedSpinGlass->getPosition();
    oldSpinGlassOrientation = selectedSpinGlass->getOrientation();

    // computing the old spin glass connection likelihood
    float oldSelectedSpinGlassForwardExtremityConnectionLikelihood =
           _anatomicalPriors->getConnectionLikelihood(
                  selectedSpinGlass->getExtremity( gkg::SpinGlass::Forward ) );
    float oldSelectedSpinGlassBackwardExtremityConnectionLikelihood =
           _anatomicalPriors->getConnectionLikelihood(
                  selectedSpinGlass->getExtremity( gkg::SpinGlass::Backward ) );

    // computing the odf corresponding to the old spin glass position
    gkg::RCPointer< gkg::OrientationDistributionFunction > oldOdf;
    this->getOdf( odfContinuousField,
                  oldSpinGlassPosition,
                  roiResolution,
                  transform3dFromRoisVoxelToOdfsReal,
                  oldOdf );

    // sanity check
    if ( oldOdf.isNull() )
    {

      // throw std::runtime_error( "null ODF at the old spin glass position" );
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

    // defining the forward spin glass connection likelihood and its associated
    // extremity type
    float forwardSpinGlassConnectionLikelihood = 0.0f;
    gkg::SpinGlass::ExtremityType
      forwardSpinGlassExtremityType = gkg::SpinGlass::Forward;

    // retrieving the putative spin glass connected at the forward extremity
    const gkg::RCPointer< gkg::SpinGlass >&
      forwardSpinGlass = selectedSpinGlass->getConnectedSpinGlass(
                                                      gkg::SpinGlass::Forward );

    // checking if the forward spin glass is null
    if ( !forwardSpinGlass.isNull() )
    {

      if ( forwardSpinGlass->getConnectedSpinGlass(
                               gkg::SpinGlass::Backward ) == selectedSpinGlass )
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
      float selectedSpinGlassExtremityDistanceToPialSurface = 1e38;
      //bool isSelectedSpinGlassExtremityCloseToCortex =
      //  _anatomicalPriors->getDistanceToPialSurface(
      //                        selectedSpinGlass->getExtremity(
      //                                              gkg::SpinGlass::Forward ),
      //                      selectedSpinGlassExtremityDistanceToPialSurface );

      float forwardSpinGlassExtremityDistanceToPialSurface = 1e38;
      //bool isForwardSpinGlassExtremityCloseToCortex =
      //  _anatomicalPriors->getDistanceToPialSurface(
      //                        forwardSpinGlass->getExtremity(
      //                                        forwardSpinGlassExtremityType ),
      //                      forwardSpinGlassExtremityDistanceToPialSurface );

      oldSpinGlassConnectionEnergy +=
        selectedSpinGlass->getInteractionPotentialWithOther(
                       gkg::SpinGlass::Forward,
                       oldSelectedSpinGlassForwardExtremityConnectionLikelihood,
                       forwardSpinGlass,
                       forwardSpinGlassExtremityType,
                       forwardSpinGlassConnectionLikelihood,
                       selectedSpinGlassExtremityDistanceToPialSurface,
                       forwardSpinGlassExtremityDistanceToPialSurface,
                       _anatomicalPriors->getMinimumDistanceToMesh(),
                       _anatomicalPriors->getMaximumDistanceToMesh() );

    }

    // defining the backward spin glass connection likelihood and its associated
    // extremity type
    float backwardSpinGlassConnectionLikelihood = 0.0f;
    gkg::SpinGlass::ExtremityType
      backwardSpinGlassExtremityType = gkg::SpinGlass::Forward;

    // retrieving the putative spin glass connected at the backward extremity
    const gkg::RCPointer< gkg::SpinGlass >&
      backwardSpinGlass = selectedSpinGlass->getConnectedSpinGlass(
                                                     gkg::SpinGlass::Backward );
    // checking if the backward spin glass is null
    if ( !backwardSpinGlass.isNull() )
    {

      if ( backwardSpinGlass->getConnectedSpinGlass(
                               gkg::SpinGlass::Backward ) == selectedSpinGlass )
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

      float selectedSpinGlassExtremityDistanceToPialSurface = 1e38;
      //bool isSelectedSpinGlassExtremityCloseToCortex =
      //  _anatomicalPriors->getDistanceToPialSurface(
      //                        selectedSpinGlass->getExtremity(
      //                                             gkg::SpinGlass::Backward ),
      //                      selectedSpinGlassExtremityDistanceToPialSurface );

      float backwardSpinGlassExtremityDistanceToPialSurface = 1e38;
      //bool isBackwardSpinGlassExtremityCloseToCortex =
      //  _anatomicalPriors->getDistanceToPialSurface(
      //                       backwardSpinGlass->getExtremity(
      //                                       backwardSpinGlassExtremityType ),
      //                     backwardSpinGlassExtremityDistanceToPialSurface );

      oldSpinGlassConnectionEnergy +=
        selectedSpinGlass->getInteractionPotentialWithOther(
                      gkg::SpinGlass::Backward,
                      oldSelectedSpinGlassBackwardExtremityConnectionLikelihood,
                      backwardSpinGlass,
                      backwardSpinGlassExtremityType,
                      backwardSpinGlassConnectionLikelihood,
                      selectedSpinGlassExtremityDistanceToPialSurface,
                      backwardSpinGlassExtremityDistanceToPialSurface,
                      _anatomicalPriors->getMinimumDistanceToMesh(),
                      _anatomicalPriors->getMaximumDistanceToMesh() );

    }

    ////////////////////////////////////////////////////////////////////////////
    // spatial and orientational discrepancies to be tuned for the random move
    ////////////////////////////////////////////////////////////////////////////
    float spatialDiscrepancy =
                            ( float )( selectedSpinGlass->getLength() / 10.0f );
    float orientationalDiscrepancy = _anatomicalPriors->getCurvatureThreshold(
                                                         oldSpinGlassPosition );

    // computing a random position and orientation for the spin glass
    this->setRandomMove( oldSpinGlassPosition,
                         oldSpinGlassOrientation,
                         newSpinGlassPosition,
                         newSpinGlassOrientation,
                         spatialDiscrepancy,
                         orientationalDiscrepancy );

    // computing the new voxel corresponding to the new spin glass position
    this->getVoxelFromPosition( newSpinGlassPosition,
                                roiResolution,
                                newVoxel );

    // nothing to do if the new voxel is out of bounds
    if ( computingMaskBoundingBox.contains( newVoxel ) &&
         computingMask( newVoxel ) )
    {

      // moving the spin glass randomly
      selectedSpinGlass->setPosition( newSpinGlassPosition );
      selectedSpinGlass->setOrientation( newSpinGlassOrientation );

      // computing the old spin glass connection likelihood
      /*float newSelectedSpinGlassConnectionLikelihood =
           _anatomicalPriors->getConnectionLikelihood( newSpinGlassPosition );*/

/////////
      float newSelectedSpinGlassForwardExtremityConnectionLikelihood =
              _anatomicalPriors->getConnectionLikelihood(
                  selectedSpinGlass->getExtremity( gkg::SpinGlass::Forward ) );
      float newSelectedSpinGlassBackwardExtremityConnectionLikelihood =
              _anatomicalPriors->getConnectionLikelihood(
                  selectedSpinGlass->getExtremity( gkg::SpinGlass::Backward ) );
/////////

      // computing the odf corresponding to the new spin glass position
      gkg::RCPointer< gkg::OrientationDistributionFunction > newOdf;
      this->getOdf( odfContinuousField,
                    newSpinGlassPosition,
                    roiResolution,
                    transform3dFromRoisVoxelToOdfsReal,
                    newOdf );

      // sanity check
      if ( newOdf.isNull() )
      {

        //throw std::runtime_error( "null ODF at the new spin glass position" );
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

      // checking if the forward spin glass is null
      if ( !forwardSpinGlass.isNull() )
      {

        // updating the new spin glass connection energy with the interaction
        // potential at the forward extremity
        float selectedSpinGlassExtremityDistanceToPialSurface = 1e38;
        //bool isSelectedSpinGlassExtremityCloseToCortex =
        //  _anatomicalPriors->getDistanceToPialSurface(
        //                      selectedSpinGlass->getExtremity(
        //                                           gkg::SpinGlass::Forward ),
        //                    selectedSpinGlassExtremityDistanceToPialSurface );

        float forwardSpinGlassExtremityDistanceToPialSurface = 1e38;
        //bool isForwardSpinGlassExtremityCloseToCortex =
        //  _anatomicalPriors->getDistanceToPialSurface(
        //                      forwardSpinGlass->getExtremity(
        //                                      forwardSpinGlassExtremityType ),
        //                    forwardSpinGlassExtremityDistanceToPialSurface );

        newSpinGlassConnectionEnergy +=
          selectedSpinGlass->getInteractionPotentialWithOther(
                       gkg::SpinGlass::Forward,
                       newSelectedSpinGlassForwardExtremityConnectionLikelihood,
                       forwardSpinGlass,
                       forwardSpinGlassExtremityType,
                       forwardSpinGlassConnectionLikelihood,
                       selectedSpinGlassExtremityDistanceToPialSurface,
                       forwardSpinGlassExtremityDistanceToPialSurface,
                       _anatomicalPriors->getMinimumDistanceToMesh(),
                       _anatomicalPriors->getMaximumDistanceToMesh() );

      }

      // checking if the backward spin glass is null
      if ( !backwardSpinGlass.isNull() )
      {

        // updating the new spin glass connection energy with the interaction
        // potential at the backward extremity
        float selectedSpinGlassExtremityDistanceToPialSurface = 1e38;
        //bool isSelectedSpinGlassExtremityCloseToCortex =
        //  _anatomicalPriors->getDistanceToPialSurface(
        //                      selectedSpinGlass->getExtremity(
        //                                           gkg::SpinGlass::Backward ),
        //                    selectedSpinGlassExtremityDistanceToPialSurface );

        float backwardSpinGlassExtremityDistanceToPialSurface = 1e38;
        //bool isBackwardSpinGlassExtremityCloseToCortex =
        //  _anatomicalPriors->getDistanceToPialSurface(
        //                     backwardSpinGlass->getExtremity(
        //                                     backwardSpinGlassExtremityType ),
        //                   backwardSpinGlassExtremityDistanceToPialSurface );

        newSpinGlassConnectionEnergy +=
          selectedSpinGlass->getInteractionPotentialWithOther(
                      gkg::SpinGlass::Backward,
                      newSelectedSpinGlassBackwardExtremityConnectionLikelihood,
                      backwardSpinGlass,
                      backwardSpinGlassExtremityType,
                      backwardSpinGlassConnectionLikelihood,
                      selectedSpinGlassExtremityDistanceToPialSurface,
                      backwardSpinGlassExtremityDistanceToPialSurface,
                      _anatomicalPriors->getMinimumDistanceToMesh(),
                      _anatomicalPriors->getMaximumDistanceToMesh() );

      }

    }
    else
    {

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "new voxel out of tractography mask" << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------
      isProposalValid = false;

    }

    // setting back the old position and orientation of the spin glass
    selectedSpinGlass->setPosition( oldSpinGlassPosition );
    selectedSpinGlass->setOrientation( oldSpinGlassOrientation );

    return isProposalValid;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool "
             "gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "makeSpinGlassRandomMotionProposal( "
             "std::map< gkg::Vector3d< float >, "
             "std::pair< std::vector< float >, std::pair< float, float > >, "
             "gkg::Vector3dCompare< float > >& buffer, "
             "gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
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
void gkg::GlobalTractographyAlgorithm< L, Compare >::setRandomMove(
                          const gkg::Vector3d< float >& oldSpinGlassPosition,
                          const gkg::Vector3d< float >& oldSpinGlassOrientation,
                          gkg::Vector3d< float >& newSpinGlassPosition,
                          gkg::Vector3d< float >& newSpinGlassOrientation,
                          float spatialDiscrepancy,
                          float orientationalDiscrepancy ) const
{

  try
  {

    // proposing a new position for the spin glass
    newSpinGlassPosition.x = ( float )_factory->getGaussianRandomNumber( 
                                               *_randomGenerator,
                                               ( double )oldSpinGlassPosition.x,
                                               spatialDiscrepancy );
    newSpinGlassPosition.y = ( float )_factory->getGaussianRandomNumber( 
                                               *_randomGenerator,
                                               ( double )oldSpinGlassPosition.y,
                                               spatialDiscrepancy );
    newSpinGlassPosition.z = ( float )_factory->getGaussianRandomNumber( 
                                               *_randomGenerator,
                                               ( double )oldSpinGlassPosition.z,
                                               spatialDiscrepancy );

    // proposing a new orientation for the spin glass
    double radial = 0;
    double azimut = 0;
    double colatitude = 0;

    // converting from Cartesian to spherical coordinates
    _factory->getCartesianToSphericalCoordinates(
                                            ( double )oldSpinGlassOrientation.x,
                                            ( double )oldSpinGlassOrientation.y,
                                            ( double )oldSpinGlassOrientation.z,
                                            radial,
                                            azimut,
                                            colatitude );

    double newAzimut = ( float )_factory->getGaussianRandomNumber(
                                                     *_randomGenerator,
                                                     azimut,
                                                     orientationalDiscrepancy );
    double newColatitude = ( float )_factory->getGaussianRandomNumber(
                                                     *_randomGenerator,
                                                     colatitude,
                                                     orientationalDiscrepancy );

    // converting back from spherical to Cartesian coordinates
    double newSpinGlassOrientationX = 0.0;
    double newSpinGlassOrientationY = 0.0;
    double newSpinGlassOrientationZ = 0.0;
    _factory->getSphericalToCartesianCoordinates( 1.0,
                                                  newAzimut,
                                                  newColatitude,
                                                  newSpinGlassOrientationX,
                                                  newSpinGlassOrientationY,
                                                  newSpinGlassOrientationZ );

    newSpinGlassOrientation.x = ( float )newSpinGlassOrientationX;
    newSpinGlassOrientation.y = ( float )newSpinGlassOrientationY;
    newSpinGlassOrientation.z = ( float )newSpinGlassOrientationZ;

    // normalizing the new spin glass orientation
    newSpinGlassOrientation.normalize();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >:: "
             "setRandomMove( "
             "const gkg::Vector3d< float >& oldSpinGlassPosition, "
             "const gkg::Vector3d< float >& oldSpinGlassOrientation, "
             "gkg::Vector3d< float >& newSpinGlassPosition, "
             "gkg::Vector3d< float >& newSpinGlassOrientation, "
             "float spatialDiscrepancy, "
             "float orientationalDiscrepancy ) const " );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::
                                            acceptSpinGlassRandomMotionProposal(
           const gkg::Vector3d< int32_t >& voxel,
           gkg::CartesianField< std::set< gkg::RCPointer< gkg::SpinGlass > > >&
                                                        spinGlassCartesianField,
           const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
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
             "acceptSpinGlassRandomMotionProposal( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "gkg::CartesianField< std::set< "
             "gkg::RCPointer< gkg::SpinGlass > > >& "
             "spinGlassCartesianField, "
             "const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
             "const gkg::Vector3d< int32_t >& newVoxel, "
             "const gkg::Vector3d< float >& newSpinGlassPosition, "
             "const gkg::Vector3d< float >& newSpinGlassOrientation ) const" );

}





#endif


