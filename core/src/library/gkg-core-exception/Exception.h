#ifndef _gkg_core_exception_Exception_h_
#define _gkg_core_exception_Exception_h_


#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <gkg-core-exception/HelpOptionException.h>


#define GKG_CATCH( S )                                            \
catch ( std::exception& e )                                       \
{                                                                 \
                                                                  \
  throw std::runtime_error( std::string( S ) + ": \n" + e.what() ); \
                                                                  \
}


#define GKG_CATCH_WITH_COMMENT( S, C )                                \
catch ( std::exception& e )                                           \
{                                                                     \
                                                                      \
  throw std::runtime_error( std::string( S ) + ": \n" + C + e.what() ); \
                                                                      \
}


#define GKG_CATCH_COMMAND( R )                               \
  catch ( gkg::HelpOptionException& )                        \
  {                                                          \
  }                                                          \
  catch ( std::exception& e )                                \
  {                                                          \
                                                             \
    std::cerr << argv[ 0 ] << ": \n" << e.what() << std::endl; \
    R = EXIT_FAILURE;                                        \
                                                             \
  }


#define GKG_CATCH_FUNCTOR( S )        \
  catch ( gkg::HelpOptionException& ) \
  {                                   \
  }                                   \
  GKG_CATCH( S ); 

#endif
