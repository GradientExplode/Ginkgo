#ifndef _gkg_processing_signal_HammingApodization_h_
#define _gkg_processing_signal_HammingApodization_h_


#include <gkg-processing-signal/Apodization.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


template < class T >
class HammingApodization : public Apodization< T >,
                           public Creator2Arg< HammingApodization< T >,
                                               Apodization< T >,
                                               int32_t,
                                               typename Apodization< T >::Type >
{

  public:


    ~HammingApodization();

    std::string getName() const;

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< HammingApodization< T >,
                              Apodization< T >,
                              int32_t,
                              typename Apodization< T >::Type >;

    HammingApodization( int32_t size, typename Apodization< T >::Type type );

};


}


#endif
