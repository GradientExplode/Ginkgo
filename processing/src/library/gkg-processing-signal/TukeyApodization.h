#ifndef _gkg_processing_signal_TukeyApodization_h_
#define _gkg_processing_signal_TukeyApodization_h_


#include <gkg-processing-signal/Apodization.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


template < class T >
class TukeyApodization : public Apodization< T >,
                         public Creator2Arg< TukeyApodization< T >,
                                             Apodization< T >,
                                             int32_t,
                                             typename Apodization< T >::Type >
{

  public:

    ~TukeyApodization();

    void setAlpha( double alpha );

    std::string getName() const;

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TukeyApodization< T >,
                              Apodization< T >,
                              int32_t,
                              typename Apodization< T >::Type >;

    TukeyApodization( int32_t count, typename Apodization< T >::Type type );

    void computeCoefficients();

    typename gkg::Apodization< T >::Type _type;
    double _alpha;

};


}


#endif
