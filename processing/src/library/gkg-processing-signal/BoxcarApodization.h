#ifndef _gkg_processing_signal_BoxcarApodization_h_
#define _gkg_processing_signal_BoxcarApodization_h_


#include <gkg-processing-signal/Apodization.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


template < class T >
class BoxcarApodization : public Apodization< T >,
                          public Creator2Arg< BoxcarApodization< T >,
                                              Apodization< T >,
                                              int32_t,
                                              typename Apodization< T >::Type >
{

  public:

    ~BoxcarApodization();

    std::string getName() const;

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BoxcarApodization< T >,
                              Apodization< T >,
                              int32_t,
                              typename Apodization< T >::Type >;

    BoxcarApodization( int32_t size, typename Apodization< T >::Type type );

};


}


#endif
