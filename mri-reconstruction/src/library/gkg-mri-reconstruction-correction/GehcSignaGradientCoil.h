#ifndef _gkg_mri_reconstruction_correction_GehcSignaGradientCoil_h_
#define _gkg_mri_reconstruction_correction_GehcSignaGradientCoil_h_


#include <gkg-mri-reconstruction-correction/GradientCoil.h>
#include <string>


namespace gkg
{


template < class T >
class GehcSignaGradientCoil : public GradientCoil< T >
{

  public:

    enum Type
    {

      BRM_CRM,

    };

    GehcSignaGradientCoil( const std::string& fileName );
    virtual ~GehcSignaGradientCoil();

    Type getType() const;
    const T& getScaleX( int32_t index ) const;
    const T& getScaleY( int32_t index ) const;
    const T& getScaleZ( int32_t index ) const;
    const T& getDelta() const;

    void getXYZWedges( const Vector3d< T >& phaseVector,
                       const Vector3d< T >& frequencyVector,
                       const Vector3d< T >& point,
                       std::vector< float >& wedgeX,
                       std::vector< float >& wedgeY,
                       std::vector< float >& wedgeZ ) const;

  protected:

    void getBrmCrmWedges( const Vector3d< T >& phaseVector,
                          const Vector3d< T >& frequencyVector,
                          const Vector3d< T >& point,
                          std::vector< float >& wedgeX,
                          std::vector< float >& wedgeY,
                          std::vector< float >& wedgeZ ) const;

    Type _type;
    T _scaleX[ 10 ];
    T _scaleY[ 10 ];
    T _scaleZ[ 10 ];
    T _delta;


};


}


#endif
