#include <gkg-dmri-tractography/AnatomicallyInformedStoppingStrategy.h>
#include <gkg-dmri-tractography/AnatomicallyConstrainedSeedingStrategy.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::AnatomicallyInformedStoppingStrategy::AnatomicallyInformedStoppingStrategy(
        gkg::OdfContinuousField& odfContinuousField,
        const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
        const gkg::Volume< int16_t >& computingMask,
        const gkg::Transform3d< float >& transform3dFromOdfsRealToMaskVoxel,
        float step,
        float maximumFiberLength,
        bool usingGFAThreshold,
        float gfaThreshold )
                                          : gkg::StoppingStrategy(
                                             odfContinuousField,
                                             computingMaskBoundingBox,
                                             computingMask,
                                             transform3dFromOdfsRealToMaskVoxel,
                                             step,
                                             maximumFiberLength ),
                                            _usingGFAThreshold(
                                                            usingGFAThreshold ),
                                            _gfaThreshold( gfaThreshold )
{
}


gkg::AnatomicallyInformedStoppingStrategy::
                                         ~AnatomicallyInformedStoppingStrategy()
{
}


bool gkg::AnatomicallyInformedStoppingStrategy::stop(
                              const gkg::Vector3d< float >& pointOdfs,
                              gkg::OrientationDistributionFunction*& odf,
                              float& gfa,
                              float& fiberLength,
                              std::list< gkg::Vector3d< float > >& trajectory,
                              bool& keepFiber )
{

  try
  {


    // computing the new voxel in the computing mask frame
    gkg::Vector3d< int32_t > voxelMask;
    _transform3dFromOdfsRealToMaskVoxel.getIntegerDirect( pointOdfs,
                                                          voxelMask );

    bool stop = false;
    // check if voxel mask get out of the computing mask
    if ( !_computingMaskBoundingBox.contains( voxelMask ) )
    {

      stop = true;

    }
    else
    {

      int16_t maskValue = _computingMask( voxelMask );
      if ( maskValue == OUTSIDE_BRAIN_ID )
      {

        stop = true;

      }
      else
      {

        if ( maskValue == CORTEX_ID )
        {

          stop = true;

        }
        else if ( maskValue == DEEP_GRAY_MATTER_ID )
        {

          stop = true;

        }
        else if ( maskValue == CSF_ID )
        {

          stop = true;
          keepFiber = false;

        }


        if ( _odfContinuousField.isValid( pointOdfs ) )
        {

          // getting pointer to the new ODF
          odf = _odfContinuousField.getItem( pointOdfs );

          // processing the new GFA
          gfa = odf->getGeneralizedFractionalAnisotropy();

          if ( _usingGFAThreshold && ( gfa <= _gfaThreshold  ) )
          {

            stop = true;
            if ( ! ( ( maskValue == CORTEX_ID ) ||
                     ( maskValue == DEEP_GRAY_MATTER_ID ) ) )
            {

              keepFiber = false;

            }

          }
          else
          {

            // updating fiber length
            fiberLength += this->_step;

            // if fiber length exceeds the maximum allowed fiber length,
            // then stop and return false
            if ( fiberLength > this->_maximumFiberLength )
            {

              keepFiber = false;
              stop = true;

            }
            else
            {

              // adding current point to the trajectory
              trajectory.push_back( pointOdfs );

            }

          }

        }
        else
        {

          stop = true;

        }

      }

    }

    return stop;

  }
  GKG_CATCH( "bool gkg::AnatomicallyInformedStoppingStrategy::stop( "
             "const gkg::Vector3d< float >& pointOdfs, "
             "gkg::OrientationDistributionFunction*& odf, "
             "float& gfa, "
             "float& fiberLength, "
             "std::list< gkg::Vector3d< float > >& trajectory, "
             "bool& keepFiber )" );

}


bool gkg::AnatomicallyInformedStoppingStrategy::usingGFAThreshold() const
{

  try
  {

    return _usingGFAThreshold;

  }
  GKG_CATCH( "bool "
             "gkg::AnatomicallyInformedStoppingStrategy::usingGFAThreshold() const" );

}


float gkg::AnatomicallyInformedStoppingStrategy::getGFAThreshold() const
{

  try
  {

    return _gfaThreshold;

  }
  GKG_CATCH( "float "
             "gkg::AnatomicallyInformedStoppingStrategy::getGFAThreshold() const" );


}


