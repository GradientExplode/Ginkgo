#include <gkg-dmri-odf/PartialVolumingOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


//
// inline function for PV interpolation
//

inline
float partialVolumeInterpolation( float inferior,
                                  float superior,
                                  float weight )
{

  return inferior + ( superior - inferior ) * weight;

}


//
// class PartialVolumingOdf
//

gkg::PartialVolumingOdf::PartialVolumingOdf(
                        const gkg::OdfCartesianField& odfCartesianField,
                        const gkg::Vector3d< float >& site,
                        const gkg::Vector3d< int32_t >& sizes,
                        const gkg::Vector3d< double >& resolution )
                        : gkg::OrientationDistributionFunction(
                            odfCartesianField.begin()->second.getBasisType(),
                            &odfCartesianField.getOutputOrientationSet() )
{

  try
  {


    // setting the appropriate basis type : copying type from input ODF 
    // cartesian field

    // processing reference voxel coordinates and weigths
    gkg::Vector3d< int32_t > inferiorCoordinate;
    gkg::Vector3d< int32_t > superiorCoordinate;
    gkg::Vector3d< float > weight;

    partialVolumingCoordinateAndWeight( sizes.x,
                                        site.x,
                                        resolution.x,
                                        inferiorCoordinate.x,
                                        superiorCoordinate.x,
                                        weight.x );
    partialVolumingCoordinateAndWeight( sizes.y,
                                        site.y,
                                        resolution.y,
                                        inferiorCoordinate.y,
                                        superiorCoordinate.y,
                                        weight.y );
    partialVolumingCoordinateAndWeight( sizes.z,
                                        site.z,
                                        resolution.z,
                                        inferiorCoordinate.z,
                                        superiorCoordinate.z,
                                        weight.z );


    // ODF1
    const gkg::OrientationDistributionFunction* 
      odf1 = odfCartesianField.getItemFast( inferiorCoordinate.x,
                                            inferiorCoordinate.y,
                                            inferiorCoordinate.z );
    const float* item1 = 0;
    if ( odf1 )
    {
    
      item1 = &odf1->getValues()[ 0 ];

    }

    // ODF2
    const gkg::OrientationDistributionFunction* 
      odf2 = ( ( superiorCoordinate.x == sizes.x ) ?
               0 : 
               odfCartesianField.getItemFast( superiorCoordinate.x,
                                              inferiorCoordinate.y,
                                              inferiorCoordinate.z ) );
    const float* item2 = 0;
    if ( odf2 )
    {
    
      item2 = &odf2->getValues()[ 0 ];

    }

    // ODF3
    const gkg::OrientationDistributionFunction* 
      odf3 = ( ( superiorCoordinate.y == sizes.y ) ?
               0 : 
               odfCartesianField.getItemFast( inferiorCoordinate.x,
                                              superiorCoordinate.y,
                                              inferiorCoordinate.z ) );
    const float* item3 = 0;
    if ( odf3 )
    {
    
      item3 = &odf3->getValues()[ 0 ];

    }

    // ODF4
    const gkg::OrientationDistributionFunction* 
      odf4 = ( ( ( superiorCoordinate.x == sizes.x ) ||
                 ( superiorCoordinate.y == sizes.y ) ) ?
               0 : 
               odfCartesianField.getItemFast( superiorCoordinate.x,
                                              superiorCoordinate.y,
                                              inferiorCoordinate.z ) );
    const float* item4 = 0;
    if ( odf4 )
    {
    
      item4 = &odf4->getValues()[ 0 ];

    }

    // ODF5
    const gkg::OrientationDistributionFunction* 
      odf5 = ( ( superiorCoordinate.z == sizes.z ) ?
               0 : 
               odfCartesianField.getItemFast( inferiorCoordinate.x,
                                              inferiorCoordinate.y,
                                              superiorCoordinate.z ) );
    const float* item5 = 0;
    if ( odf5 )
    {
    
      item5 = &odf5->getValues()[ 0 ];

    }

    // ODF6
    const gkg::OrientationDistributionFunction* 
      odf6 = ( ( ( superiorCoordinate.x == sizes.x ) ||
                 ( superiorCoordinate.z == sizes.z ) ) ?
               0 : 
               odfCartesianField.getItemFast( superiorCoordinate.x,
                                              inferiorCoordinate.y,
                                              superiorCoordinate.z ) );
    const float* item6 = 0;
    if ( odf6 )
    {
    
      item6 = &odf6->getValues()[ 0 ];

    }

    // ODF7
    const gkg::OrientationDistributionFunction* 
      odf7 = ( ( ( superiorCoordinate.y == sizes.y ) ||
                 ( superiorCoordinate.z == sizes.z ) ) ?
               0 : 
               odfCartesianField.getItemFast( inferiorCoordinate.x,
                                              superiorCoordinate.y,
                                              superiorCoordinate.z ) );
    const float* item7 = 0;
    if ( odf7 )
    {
    
      item7 = &odf7->getValues()[ 0 ];

    }

    // ODF8
    const gkg::OrientationDistributionFunction*  
      odf8 = ( ( ( superiorCoordinate.x == sizes.x ) ||
                 ( superiorCoordinate.y == sizes.y ) ||
                 ( superiorCoordinate.z == sizes.z ) ) ?
               0 : 
               odfCartesianField.getItemFast( superiorCoordinate.x,
                                              superiorCoordinate.y,
                                              superiorCoordinate.z ) );
    const float* item8 = 0;
    if ( odf8 )
    {
    
      item8 = &odf8->getValues()[ 0 ];

    }

    // processing values with PV interpolator
    int32_t valueCount = odfCartesianField.begin()->second.getValueCount();
    _values.resize( valueCount );
    //std::fill( _values.begin(), _values.end(), 0.0f );

    // getting access to the values of the interpolated ODF
    float* v = (float*)&_values[ 0 ];

    // in case all the ODF are existing
    if ( odf1 && odf2 && odf3 && odf4 && odf5 && odf6 && odf7 && odf8 )
    {
    
      while ( valueCount-- )
      {

        *v++ = partialVolumeInterpolation(
                                    partialVolumeInterpolation(
                                           partialVolumeInterpolation( *item1++,
                                                                       *item2++,
                                                                       weight.x ),
                                           partialVolumeInterpolation( *item3++,
                                                                       *item4++,
                                                                       weight.x ),
                                           weight.y ),
                                    partialVolumeInterpolation(
                                           partialVolumeInterpolation( *item5++,
                                                                       *item6++,
                                                                       weight.x ),
                                           partialVolumeInterpolation( *item7++,
                                                                       *item8++,
                                                                       weight.x ),
                                           weight.y ),
                                    weight.z );

      }

    }
    // in case not all the ODF exist
    else
    {

      float w1 = 0.0;
      if ( item1 )
      {

        w1 = ( 1.0 - weight.x ) * ( 1 - weight.y ) * ( 1 - weight.z );

      }
      float w2 = 0.0;
      if ( item2 )
      {

        w2 = weight.x * ( 1 - weight.y ) * ( 1 - weight.z );

      }
      float w3 = 0.0;
      if ( item3 )
      {

        w3 = ( 1.0 - weight.x ) * weight.y * ( 1 - weight.z );

      }
      float w4 = 0.0;
      if ( item4 )
      {

        w4 = weight.x * weight.y * ( 1 - weight.z );

      }
      float w5 = 0.0;
      if ( item5 )
      {

        w5 = ( 1.0 - weight.x ) * ( 1 - weight.y ) * weight.z;

      }
      float w6 = 0.0;
      if ( item6 )
      {

        w6 = weight.x * ( 1 - weight.y ) * weight.z;

      }
      float w7 = 0.0;
      if ( item7 )
      {

        w7 = ( 1.0 - weight.x ) * weight.y * weight.z;

      }
      float w8 = 0.0;
      if ( item8 )
      {

        w8 = weight.x * weight.y * weight.z;

      }

      float sumOfWeight = 0.0;

      while ( valueCount-- )
      {


        *v = 0.0;
        

        if ( item1 )
        {

          *v += *item1 * w1;
          sumOfWeight += w1;
          ++ item1;

        }
        if ( item2 )
        {

          *v += *item2 * w2;
          sumOfWeight += w2;
          ++ item2;

        }
        if ( item3 )
        {

          *v += *item3 * w3;
          sumOfWeight += w3;
          ++ item3;

        }
        if ( item4 )
        {

          *v += *item4 * w4;
          sumOfWeight += w4;
          ++ item4;

        }
        if ( item5 )
        {

          *v += *item5 * w5;
          sumOfWeight += w5;
          ++ item5;

        }
        if ( item6 )
        {

          *v += *item6 * w6;
          sumOfWeight += w6;
          ++ item6;

        }
        if ( item7 )
        {

          *v += *item7 * w7;
          sumOfWeight += w7;
          ++ item7;

        }
        if ( item8 )
        {

          *v += *item8 * w8;
          sumOfWeight += w8;
          ++ item8;

        }

        if ( sumOfWeight > 0.0 )
        {

          *v /= sumOfWeight;

        }

        ++ v;

      }      


    }

  }
  GKG_CATCH( "gkg::PartialVolumingOdf::PartialVolumingOdf( "
             "const gkg::OdfCartesianField& odfCartesianField, "
             "const gkg::Vector3d< float >& site )" );


}


gkg::PartialVolumingOdf::PartialVolumingOdf( const gkg::PartialVolumingOdf& other )
                        : gkg::OrientationDistributionFunction( other )
{
}


gkg::PartialVolumingOdf::~PartialVolumingOdf()
{
}


void gkg::PartialVolumingOdf::partialVolumingCoordinateAndWeight(
                                             const int32_t& size,
                                             const float& realCoordinate,
                                             const double& resolution,
                                             int32_t& inferiorIntegerCoordinate,
                                             int32_t& superiorIntegerCoordinate,
                                             float& weight ) const
{

  try
  {

    float coordinate = realCoordinate / resolution;

    inferiorIntegerCoordinate = ( int32_t )coordinate;
    superiorIntegerCoordinate = inferiorIntegerCoordinate + 1;

    if ( coordinate < 0 )
    {

       inferiorIntegerCoordinate =
       superiorIntegerCoordinate = 0;
       weight = 1.0;

    }
    else if ( coordinate >= ( float )size )
    {

       inferiorIntegerCoordinate =
       superiorIntegerCoordinate = size - 1;
       weight = 0.0;

    }
    else
    {

      weight = coordinate - ( float )inferiorIntegerCoordinate;

    }

  }
  GKG_CATCH( "void "
             "gkg::PartialVolumingOdf::partialVolumingInterpolation( "
             "const int32_t& size, "
             "const float& realCoordinate, "
             "const double& resolution, "
             "int32_t& inferiorIntegerCoordinate, "
             "int32_t& superiorIntegerCoordinate, "
             "float& weight ) const" );

}
