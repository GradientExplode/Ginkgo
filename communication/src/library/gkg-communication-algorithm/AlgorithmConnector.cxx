#include <gkg-communication-algorithm/AlgorithmConnector_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <complex>


//
// class AlgorithmConnectorFactory
//


gkg::AlgorithmConnectorFactory::AlgorithmConnectorFactory()
{
}


gkg::AlgorithmConnectorFactory::~AlgorithmConnectorFactory()
{
}


void gkg::AlgorithmConnectorFactory::registerAlgorithmConnector( 
                                   gkg::AlgorithmConnector* algorithmConnector )
{

  try
  {

    if ( algorithmConnector )
    {

      if ( _algorithmConnectors.find( algorithmConnector->getType() ) ==
           _algorithmConnectors.end() )
      {

        _algorithmConnectors[ algorithmConnector->getType() ] =
          algorithmConnector; 

      }

#ifdef GKG_DEBUG

      else
      {

        std::cerr << algorithmConnector->getType() +
                     " algorithm connector already registered"
                  << std::endl;

      }

#endif

    }

  }
  GKG_CATCH( "void gkg::AlgorithmConnectorFactory::"
             "registerAlgorithmConnector( "
             "gkg::AlgorithmConnector* algorithmConnector )" );

}


gkg::AlgorithmConnector* 
gkg::AlgorithmConnectorFactory::getAlgorithmConnector(
                                                 const std::string& type ) const
{

  try
  {

    std::map< std::string, gkg::AlgorithmConnector* >::const_iterator
      c = _algorithmConnectors.find( type );
    if ( c != _algorithmConnectors.end() )
    {

      return c->second;

    }
    throw std::runtime_error( std::string( "no " ) + type +
                              " algorithm connection found" );
    return 0;

  }
  GKG_CATCH( "gkg::AlgorithmConnector* "
             "gkg::AlgorithmConnectorFactory::getAlgorithmConnector( "
             "const std::string& type ) const" );

}


//
// explicit instanciation(s)
//

template class gkg::TypedAlgorithmConnector< int8_t >;
template class gkg::TypedAlgorithmConnector< uint8_t >;
template class gkg::TypedAlgorithmConnector< int16_t >;
template class gkg::TypedAlgorithmConnector< uint16_t >;
template class gkg::TypedAlgorithmConnector< int32_t >;
template class gkg::TypedAlgorithmConnector< uint32_t >;
template class gkg::TypedAlgorithmConnector< int64_t >;
template class gkg::TypedAlgorithmConnector< uint64_t >;
template class gkg::TypedAlgorithmConnector< float >;
template class gkg::TypedAlgorithmConnector< double >;
template class gkg::TypedAlgorithmConnector< bool >;
template class gkg::TypedAlgorithmConnector< std::complex<float> >;
template class gkg::TypedAlgorithmConnector< std::complex<double> >;


template class gkg::TypedAlgorithmConnector< std::vector< int8_t > >;
template class gkg::TypedAlgorithmConnector< std::vector< uint8_t > >;
template class gkg::TypedAlgorithmConnector< std::vector< int16_t > >;
template class gkg::TypedAlgorithmConnector< std::vector< uint16_t > >;
template class gkg::TypedAlgorithmConnector< std::vector< int32_t > >;
template class gkg::TypedAlgorithmConnector< std::vector< uint32_t > >;
template class gkg::TypedAlgorithmConnector< std::vector< int64_t > >;
template class gkg::TypedAlgorithmConnector< std::vector< uint64_t > >;
template class gkg::TypedAlgorithmConnector< std::vector< float > >;
template class gkg::TypedAlgorithmConnector< std::vector< double > >;
template class gkg::TypedAlgorithmConnector< std::vector< bool > >;
template class gkg::TypedAlgorithmConnector< std::vector< 
                                                        std::complex<float> > >;
template class gkg::TypedAlgorithmConnector< std::vector< 
                                                       std::complex<double> > >;


//
// registration of standard connector type(s)
//

RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_char,
  gkg::TypedAlgorithmConnector< int8_t > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_unsigned_char,
  gkg::TypedAlgorithmConnector< uint8_t > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_short,
  gkg::TypedAlgorithmConnector< int16_t > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_unsigned_short,
  gkg::TypedAlgorithmConnector< uint16_t > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_int,
  gkg::TypedAlgorithmConnector< int32_t > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_unsigned_int,
  gkg::TypedAlgorithmConnector< uint32_t > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_long,
  gkg::TypedAlgorithmConnector< int64_t > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_unsigned_long,
  gkg::TypedAlgorithmConnector< uint64_t > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_float,
  gkg::TypedAlgorithmConnector< float > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_double,
  gkg::TypedAlgorithmConnector< double > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_bool,
  gkg::TypedAlgorithmConnector< bool > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_complex_of_float,
  gkg::TypedAlgorithmConnector< std::complex< float > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_complex_of_double,
  gkg::TypedAlgorithmConnector< std::complex< double > > );


RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_char,
  gkg::TypedAlgorithmConnector< std::vector< int8_t > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_unsigned_char,
  gkg::TypedAlgorithmConnector< std::vector< uint8_t > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_short,
  gkg::TypedAlgorithmConnector< std::vector< int16_t > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_unsigned_short,
  gkg::TypedAlgorithmConnector< std::vector< uint16_t > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_int,
  gkg::TypedAlgorithmConnector< std::vector< int32_t > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_unsigned_int,
  gkg::TypedAlgorithmConnector< std::vector< uint32_t > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_long,
  gkg::TypedAlgorithmConnector< std::vector< int64_t > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_unsigned_long,
  gkg::TypedAlgorithmConnector< std::vector< uint64_t > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_float,
  gkg::TypedAlgorithmConnector< std::vector< float > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_double,
  gkg::TypedAlgorithmConnector< std::vector< double > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_bool,
  gkg::TypedAlgorithmConnector< std::vector< bool > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_std_complex_of_float,
  gkg::TypedAlgorithmConnector< std::vector< std::complex< float > > > );
RegisterAlgorithmConnector(
  gkg_TypedAlgorithmConnector_std_vector_of_std_complex_of_double,
  gkg::TypedAlgorithmConnector< std::vector< std::complex< double > > > );
