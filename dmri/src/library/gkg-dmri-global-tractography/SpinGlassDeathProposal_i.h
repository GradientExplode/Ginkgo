#ifndef _gkg_dmri_global_tractography_SpinGlassDeathProposal_i_h_
#define _gkg_dmri_global_tractography_SpinGlassDeathProposal_i_h_


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// SPIN GLASS DEATH PROPOSAL
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::makeSpinGlassDeathProposal(
       std::map< gkg::Vector3d< float >,
                 std::pair< std::vector< float >, std::pair< float, float > >,
                 gkg::Vector3dCompare< float > >& buffer,
       gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
       const gkg::Vector3d< double >& roiResolution,
       gkg::OdfContinuousField& odfContinuousField,
       const gkg::Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
       float globalMinimumOdfProbability,
       float globalMaximumOdfProbability,
       float& spinGlassDensity,
       float& spinGlassExternalEnergy ) const
{

  try
  {

    // computing the odf corresponding to the selected spin glass position
    gkg::RCPointer< gkg::OrientationDistributionFunction > odf;
    this->getOdf( odfContinuousField,
                  selectedSpinGlass->getPosition(),
                  roiResolution,
                  transform3dFromRoisVoxelToOdfsReal,
                  odf );

    // sanity check
    if ( odf.isNull() )
    {

      throw std::runtime_error(
                               "null ODF at the selected spin glass position" );
    }

    // retrieving the spin glass local density
    spinGlassDensity = _anatomicalPriors->getSpinGlassDensity(
                                             selectedSpinGlass->getPosition() );

    // computing the spin glass external energy
    spinGlassExternalEnergy =
      this->getSpinGlassExternalEnergy( buffer,
                                        selectedSpinGlass->getPosition(),
                                        selectedSpinGlass->getOrientation(),
                                        *odf,
                                        globalMinimumOdfProbability,
                                        globalMaximumOdfProbability );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "makeSpinGlassDeathProposal( "
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
             "float& spinGlassDensity, "
             "float& spinGlassExternalEnergy ) const" );

}


template < class L, class Compare > 
inline
void 
gkg::GlobalTractographyAlgorithm< L, Compare >::
                                              removeSpinGlassFromCartesianField(
       const gkg::Vector3d< int32_t >& voxel,
       const gkg::RCPointer< gkg::SpinGlass >& spinGlass,
       gkg::CartesianField< std::set< gkg::RCPointer< gkg::SpinGlass > > >&
                                                 spinGlassCartesianField ) const
{

  try
  {

    // retrieving the spin glass set of the voxel
    std::set< gkg::RCPointer< gkg::SpinGlass > >*
      spinGlassSet = spinGlassCartesianField.getItem( voxel );

    // removing the spin glass from the spin glasses Cartesian field
    if ( spinGlassSet )
    {

      spinGlassSet->erase( spinGlass );

    }
    else
    {

      throw std::runtime_error( "no spin glass set in the chosen voxel" );

    }

    // creating the spin glass deletion command and sending it to the writer
    // if connected
    if ( _writer->isConnected() )
    {

      gkg::SpinGlassDeletionCommand spinGlassDeletionCommand( spinGlass );
      _writer->send( spinGlassDeletionCommand );
      _writer->flush();

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "removeSpinGlassFromCartesianField( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "const gkg::RCPointer< gkg::SpinGlass >& spinGlass, "
             "gkg::CartesianField< std::set< "
             "gkg::RCPointer< gkg::SpinGlass > > >& "
             "spinGlassCartesianField ) const" );

}



#endif


