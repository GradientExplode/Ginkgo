#include <gkg-graphics-X11-core/GeometryRequisition.h>
#include <gkg-graphics-X11-core/Compositor.h>


static gkg::GeometryRequirement* emptyRequirement = 0;

gkg::GeometryRequisition::GeometryRequisition()
                         : _penalty( gkg::Compositor::PenaltyBad ),
                           _requirementX(),
                           _requirementY( -fil )
{
}


gkg::GeometryRequisition::GeometryRequisition( 
                                         const gkg::GeometryRequisition& other )
                         : _penalty( other._penalty ),
                           _requirementX( other._requirementX ),
                           _requirementY( other._requirementY )
{
}


gkg::GeometryRequisition& gkg::GeometryRequisition::operator = (
                                         const gkg::GeometryRequisition& other )
{

  _penalty = other._penalty;
  _requirementX = other._requirementX;
  _requirementY = other._requirementY;

  return *this;

}


void gkg::GeometryRequisition::setPenalty( int32_t penalty )
{

  _penalty = penalty;

}


int32_t gkg::GeometryRequisition::getPenalty() const
{

  return _penalty;

} 


bool gkg::GeometryRequisition::isEqualTo( const gkg::GeometryRequisition& other,
                                          float epsilon ) const
{

  return _requirementX.isEqualTo( other._requirementX, epsilon ) &&
         _requirementY.isEqualTo( other._requirementY, epsilon );

}


void gkg::GeometryRequisition::setRequirement(
                                   gkg::DimensionName dimName,
                                   const gkg::GeometryRequirement& requirement )
{

  switch ( dimName )
  {

    case gkg::DimensionX:

      _requirementX = requirement;
      break;

    case gkg::DimensionY:

      _requirementY = requirement;
      break;

  }

}


void gkg::GeometryRequisition::setRequirementX( 
                                   const gkg::GeometryRequirement& requirement )
{

  _requirementX = requirement;

}


void gkg::GeometryRequisition::setRequirementY( 
                                   const gkg::GeometryRequirement& requirement )
{

  _requirementY = requirement;

}


const gkg::GeometryRequirement& gkg::GeometryRequisition::getRequirement(
                                              gkg::DimensionName dimName ) const
{

  switch ( dimName )
  {

    case gkg::DimensionX:

      return _requirementX;
      break;

    case gkg::DimensionY:

      return _requirementY;
      break;

    default:

      if ( !emptyRequirement )
      {

        emptyRequirement = new gkg::GeometryRequirement;

      }
      return *emptyRequirement;

  }

}


const gkg::GeometryRequirement&
gkg::GeometryRequisition::getRequirementX() const
{

  return _requirementX;

}


const gkg::GeometryRequirement&
gkg::GeometryRequisition::getRequirementY() const
{

  return _requirementY;

}


gkg::GeometryRequirement& gkg::GeometryRequisition::getRequirement(
                                                    gkg::DimensionName dimName )
{

  switch ( dimName )
  {

    case gkg::DimensionX:

      return _requirementX;
      break;

    case gkg::DimensionY:

      return _requirementY;
      break;

    default:

      if ( !emptyRequirement )
      {

        emptyRequirement = new gkg::GeometryRequirement;

      }
      return *emptyRequirement;

  }

}


gkg::GeometryRequirement& gkg::GeometryRequisition::getRequirementX()
{

  return _requirementX;

}


gkg::GeometryRequirement& gkg::GeometryRequisition::getRequirementY()
{

  return _requirementY;

}


