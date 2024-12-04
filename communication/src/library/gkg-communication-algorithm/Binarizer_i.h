#ifndef _gkg_communication_algorithm_Binarizer_i_h_
#define _gkg_communication_algorithm_Binarizer_i_h_


#include <gkg-communication-algorithm/Binarizer.h>
#include <gkg-communication-algorithm/TypedAlgorithmParameter_i.h>


template < class T >
inline
gkg::Binarizer< T >::Binarizer()
                    : gkg::Algorithm( "binarizer" )
{

  _inputVector = new gkg::TypedAlgorithmParameter< std::vector< T > >(
                                                  "input_vector",
                                                  true,
                                                  false,
                                                  "this is the input vector" );
  _outputVector = new gkg::TypedAlgorithmParameter< std::vector< T > >(
                                                  "output_vector",
                                                  false,
                                                  true,
                                                  "this is the output vector" );
  _threshold = new gkg::TypedAlgorithmParameter< T >(
                                                  "threshold",
                                                  true,
                                                  false,
                                                  "this is the threshold" );
  _background = new gkg::TypedAlgorithmParameter< T >(
                                                  "background",
                                                  true,
                                                  false,
                                                  "this is the background" );
  _foreground = new gkg::TypedAlgorithmParameter< T >(
                                                  "foreground",
                                                  true,
                                                  false,
                                                  "this is the foreground" );

  this->addParameter( _inputVector );
  this->addParameter( _outputVector );
  this->addParameter( _threshold );
  this->addParameter( _background );
  this->addParameter( _foreground );

}


template < class T >
inline
gkg::Binarizer< T >::~Binarizer()
{
}


template < class T >
inline
bool gkg::Binarizer< T >::check()
{

  // to be implemented

  // check memory capacity,...

  return true;

}


template < class T >
inline
void gkg::Binarizer< T >::run()
{

  // allocating output
  *_outputVector->getPointee() = std::vector< T >( 
                _inputVector->getPointee()->size() );

  // binarizing
  typename std::vector< T >::const_iterator
    i = _inputVector->getPointee()->begin(),
    ie = _inputVector->getPointee()->end();
  typename std::vector< T >::iterator o = _outputVector->getPointee()->begin();

  while ( i != ie )
  {

    if ( *i >= *_threshold->getPointee() )
    {

      *o = *_foreground->getPointee();

    }
    else
    {

      *o = *_background->getPointee();

    }
    ++ i;
    ++ o;

  }

}


#endif
