#include <gkg-communication-algorithm/AlgorithmParameter.h>
#include <gkg-core-io/StringConverter.h>


//
// class ParameterSignature
//

gkg::ParameterSignature::ParameterSignature( const std::string& name,
                                             const std::string& type,
                                             bool isInput,
                                             bool isOutput )
                        : _name( name ),
                          _type( type ),
                          _isInput( isInput ),
                          _isOutput( isOutput )
{
}


gkg::ParameterSignature::ParameterSignature(
                                          const gkg::ParameterSignature& other )
                        : _name( other._name ),
                          _type( other._type ),
                          _isInput( other._isInput ),
                          _isOutput( other._isOutput )
{
}


gkg::ParameterSignature::~ParameterSignature()
{
}


const std::string& gkg::ParameterSignature::getName() const
{

  return _name;

}


const std::string& gkg::ParameterSignature::getType() const
{

  return _type;

}


bool gkg::ParameterSignature::isInput() const
{

  return _isInput;

}


bool gkg::ParameterSignature::isOutput() const
{

  return _isOutput;

}


std::string gkg::ParameterSignature::getCode() const
{

  return _name + ':' +
         _type + ':' +
         gkg::StringConverter::toString( _isInput ) + ':' +
         gkg::StringConverter::toString( _isOutput );

}


//
// class AlgorithmParameter
//

gkg::AlgorithmParameter::AlgorithmParameter( const std::string& name,
                                             const std::string& type,
                                             bool isInput,
                                             bool isOutput,
                                             const std::string& description )
                        : _signature( name, type, isInput, isOutput ),
                          _description( description )
{
}


gkg::AlgorithmParameter::AlgorithmParameter(
                                          const gkg::AlgorithmParameter& other )
                        : _signature( other._signature ),
                          _description( other._description )
{
}


gkg::AlgorithmParameter::~AlgorithmParameter()
{
}


const gkg::ParameterSignature& gkg::AlgorithmParameter::getSignature() const
{

  return _signature;

}


const std::string& gkg::AlgorithmParameter::getDescription() const
{

  return _description;

}


void gkg::AlgorithmParameter::setDescription( const std::string& description )
{

  _description = description;

}


std::string gkg::AlgorithmParameter::getCode() const
{

  return _signature.getCode() + ':' + _description;

}



