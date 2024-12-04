#ifndef _gkg_communication_getopt_SeriesOption_h_
#define _gkg_communication_getopt_SeriesOption_h_


#include <gkg-communication-getopt/Option.h>


namespace gkg
{


template < class T >
class SeriesOption : public Option
{

  public:

    virtual ~SeriesOption();

    std::string getInformation() const;
    bool recognize( const std::string& name ) const;
    bool feed( const std::string& value );
    void check() const;

  protected:

    friend class OptionParser;

    SeriesOption( const std::string& name,
                  const std::string& information,
                  T& series,
                  size_t minItemCount, size_t maxItemCount );

    T& _series;
    size_t _minItemCount;
    size_t _maxItemCount;
    typename T::iterator _nextItem;
    size_t _storedItemCount;

};


}


#endif
