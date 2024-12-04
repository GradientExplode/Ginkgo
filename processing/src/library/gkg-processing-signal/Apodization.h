#ifndef _gkg_processing_signal_Apodization_h_
#define _gkg_processing_signal_Apodization_h_


#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <string>


namespace gkg
{


template < class T >
class Apodization
{

  public:

    enum Type
    {

      Standard,
      Centered

    };

    virtual ~Apodization();

    virtual std::string getName() const = 0;

    int32_t getCount() const;
    const T& getCoefficient( int32_t index ) const;    
    const std::vector< T >& getCoefficients() const;    

  protected:

    Apodization( int32_t count );

    void rotateCoefficients();


    std::vector< T > _coefficients;

};


}


#endif
