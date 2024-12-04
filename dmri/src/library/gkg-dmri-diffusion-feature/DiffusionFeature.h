#ifndef _gkg_dmri_diffusion_feature_DiffusionFeature_h_
#define _gkg_dmri_diffusion_feature_DiffusionFeature_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;
template < class T > class Vector3d;


class DiffusionFeature
{

  public:

    DiffusionFeature( const DiffusionFeature& other );
    virtual ~DiffusionFeature();

    DiffusionFeature& operator=( const DiffusionFeature& other );

    virtual std::string getBasisTypeName() const;
    virtual void setBasisType( const std::string& basisTypeName );

    virtual int32_t getValueCount() const;
    virtual const std::vector< float >& getValues() const;
    virtual float getValue( int32_t index ) const;
    virtual void setValues( const std::vector< float >& values );
    virtual void setValues( const gkg::Vector& values );
    virtual void setValue( int32_t index, float value );

  protected:

    DiffusionFeature();

    bool computeNormalizedSignal( const Volume< float >& t2,
                                  const Volume< float >& dw,
                                  const Vector3d< int32_t >& site,
                                  float& averageT2,
                                  Vector& normalizedSignal ) const;

    bool computeNormalizedSignal( const Volume< float >& t2,
                                  const Volume< float >& dw,
                                  const Vector3d< float >& site,
                                  float& averageT2,
                                  Vector& normalizedSignal ) const;


    std::vector< float > _values;

};


}





#endif


