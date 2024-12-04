#include <gkg-dmri-container/OdfContinuousField.h>
#include <gkg-dmri-container/DiffusionContinuousField_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/TypeOf.h>


gkg::OdfContinuousField::OdfContinuousField( bool storeItems )
                        : gkg::DiffusionContinuousField<
                            gkg::OrientationDistributionFunction >( storeItems )
{
}


gkg::OdfContinuousField::~OdfContinuousField()
{
}


void gkg::OdfContinuousField::updateCache( float /* apertureAngle */, 
                                           bool /* cosine */ ) const
{
}
