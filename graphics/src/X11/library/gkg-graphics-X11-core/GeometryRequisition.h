#ifndef _gkg_graphics_X11_core_GeometryRequisition_h_
#define _gkg_graphics_X11_core_GeometryRequisition_h_


#include <gkg-graphics-X11-core/GeometryRequirement.h>


namespace gkg
{


class GeometryRequisition
{

  public:

    GeometryRequisition();
    GeometryRequisition( const GeometryRequisition& other );
    
    GeometryRequisition& operator = ( const GeometryRequisition& other );

    void setPenalty( int32_t penalty );
    int32_t getPenalty() const;

    bool isEqualTo( const GeometryRequisition& other, float epsilon ) const;

    void setRequirement( DimensionName dimName,
                         const GeometryRequirement& requirement );
    void setRequirementX( const GeometryRequirement& requirement );
    void setRequirementY( const GeometryRequirement& requirement );

    const GeometryRequirement& getRequirement( DimensionName dimName ) const;
    const GeometryRequirement& getRequirementX() const;
    const GeometryRequirement& getRequirementY() const;

    GeometryRequirement& getRequirement( DimensionName dimName );
    GeometryRequirement& getRequirementX();
    GeometryRequirement& getRequirementY();

  private:

    int32_t _penalty;
    GeometryRequirement _requirementX;
    GeometryRequirement _requirementY;

};


}


#endif
