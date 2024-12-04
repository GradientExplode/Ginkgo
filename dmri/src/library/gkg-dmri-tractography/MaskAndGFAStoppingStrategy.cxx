#include <gkg-dmri-tractography/MaskAndGFAStoppingStrategy.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::MaskAndGFAStoppingStrategy::MaskAndGFAStoppingStrategy(
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
                                  _usingGFAThreshold( usingGFAThreshold ),
                                  _gfaThreshold( gfaThreshold )
{
}


gkg::MaskAndGFAStoppingStrategy::~MaskAndGFAStoppingStrategy()
{
}


bool gkg::MaskAndGFAStoppingStrategy::stop(
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

      if ( !_computingMask( voxelMask ) )
      {

        stop = true;

      }
      else
      {

        if ( _odfContinuousField.isValid( pointOdfs ) )
        {

          // getting pointer to the new ODF
          odf = _odfContinuousField.getItem( pointOdfs );

          // processing the new GFA
          gfa = odf->getGeneralizedFractionalAnisotropy();

          if ( _usingGFAThreshold && ( gfa <= _gfaThreshold  ) )
          {

            stop = true;

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
  GKG_CATCH( "bool gkg::MaskAndGFAStoppingStrategy::stop( "
             "const gkg::Vector3d< float >& pointOdfs, "
             "gkg::OrientationDistributionFunction*& odf, "
             "float& gfa, "
             "float& fiberLength, "
             "std::list< gkg::Vector3d< float > >& trajectory, "
             "bool& keepFiber )" );

}


bool gkg::MaskAndGFAStoppingStrategy::usingGFAThreshold() const
{

  try
  {

    return _usingGFAThreshold;

  }
  GKG_CATCH( "bool "
             "gkg::MaskAndGFAStoppingStrategy::usingGFAThreshold() const" );

}


float gkg::MaskAndGFAStoppingStrategy::getGFAThreshold() const
{

  try
  {

    return _gfaThreshold;

  }
  GKG_CATCH( "float "
             "gkg::MaskAndGFAStoppingStrategy::getGFAThreshold() const" );


}


