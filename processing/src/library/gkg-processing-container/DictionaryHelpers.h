#ifndef _gkg_processing_container_DictionaryHelpers_h_
#define _gkg_processing_container_DictionaryHelpers_h_


#include <gkg-core-object/Dictionary.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <string>


namespace gkg
{


//
// for scalar int32_t / float / double parameters
//

std::string getStringValue( const Dictionary& dictionary,
                            const std::string& key,
                            bool isRequired = true,
                            const std::string& defaultValue = "" );


//
// for scalar int32_t / float / double parameters
//

int32_t getIntegerValue( const Dictionary& dictionary,
                         const std::string& key,
                         bool isRequired = true,
                         int32_t defaultValue = 0 );
float getFloatValue( const Dictionary& dictionary,
                     const std::string& key,
                     bool isRequired = true,
                     float defaultValue = 0.0f );
double getDoubleValue( const Dictionary& dictionary,
                       const std::string& key,
                       bool isRequired = true,
                       double defaultValue = 0.0 );


//
// for Vector3d< int32_t / float / double >
//

Vector3d< int32_t > getVector3dOfIntegerValue(
                                      const Dictionary& dictionary,
                                      const std::string& key,
                                      bool isRequired = true,
                                      const Vector3d< int32_t >& defaultValue =
                                               Vector3d< int32_t >( 0, 0, 0 ) );
Vector3d< float > getVector3dOfFloatValue(
                                      const Dictionary& dictionary,
                                      const std::string& key,
                                      bool isRequired = true,
                                      const Vector3d< float >& defaultValue = 
                                        Vector3d< float >( 0.0f, 0.0f, 0.0f ) );
Vector3d< double > getVector3dOfDoubleValue(
                                     const Dictionary& dictionary,
                                     const std::string& key,
                                     bool isRequired = true,
                                     const Vector3d< double >& defaultValue = 
                                       Vector3d< double >( 0.0f, 0.0f, 0.0f ) );


//
// for std::vector< int32_t / float / double >
//

std::vector< int32_t > getVectorOfIntegerValue(
                                               const Dictionary& dictionary,
                                               const std::string& key,
                                               int32_t expectedItemCount = 0 );
std::vector< float > getVectorOfFloatValue( const Dictionary& dictionary,
                                            const std::string& key,
                                            int32_t expectedItemCount = 0 );
std::vector< double > getVectorOfDoubleValue( const Dictionary& dictionary,
                                              const std::string& key,
                                              int32_t expectedItemCount = 0 );


//
// for std::vector< Vector3d< int32_t / float / double > >
//

std::vector< Vector3d< int32_t > > getVectorOfVector3dOfIntegerValue(
                                                const Dictionary& dictionary,
                                                const std::string& key,
                                                int32_t expectedItemCount = 0 );
std::vector< Vector3d< float > > getVectorOfVector3dOfFloatValue(
                                                const Dictionary& dictionary,
                                                const std::string& key,
                                                int32_t expectedItemCount = 0 );
std::vector< Vector3d< double > > getVectorOfVector3dOfDoubleValue(
                                                const Dictionary& dictionary,
                                                const std::string& key,
                                                int32_t expectedItemCount = 0 );


//
// for Dictionary
//

Dictionary getDictionaryValue( const Dictionary& dictionary,
                               const std::string& key,
                               bool isRequired = true );


}


#endif
