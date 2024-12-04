#ifndef _gkg_communication_getopt_OptionParser_i_h_
#define _gkg_communication_getopt_OptionParser_i_h_


#include <gkg-communication-getopt/OptionParser.h>
#include <gkg-communication-getopt/SingleOption_i.h>
#include <gkg-communication-getopt/SeriesOption_i.h>


template < class T >
inline
void gkg::OptionParser::addSingleOption( const std::string& name,
                                         const std::string& information,
                                         T& value,
                                         bool optional )
{

  _options.push_back( new gkg::SingleOption< T >( name,
                                                  information,
                                                  value,
                                                  optional ) );

}


template < class T >
inline
void gkg::OptionParser::addSeriesOption( const std::string& name,
                                         const std::string& information,
                                         T& series,
                                         size_t minItemCount,
                                         size_t maxItemCount )
{

  _options.push_back( new gkg::SeriesOption< T >( name,
                                                  information,
                                                  series,
                                                  minItemCount,
                                                  maxItemCount ) );

}


#endif
