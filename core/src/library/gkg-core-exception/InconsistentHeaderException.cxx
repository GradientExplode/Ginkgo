#include <gkg-core-exception/InconsistentHeaderException.h>


gkg::InconsistentHeaderException::InconsistentHeaderException(
                                                   const std::string& argument,
                                                   const std::string& fileName )
                                 : std::runtime_error( argument ),
                                   _fileName( fileName )
{
}


gkg::InconsistentHeaderException::~InconsistentHeaderException() throw()
{
}


const char* gkg::InconsistentHeaderException::what() const throw()
{

  if ( !_fileName.empty() )
  {

    _formedMessage = _fileName + " : inconsistent argument '" +
                     std::runtime_error::what() + "'";
    return _formedMessage.c_str();

  }
  else
  {

    return std::runtime_error::what();

  }

}


const std::string& gkg::InconsistentHeaderException::getFileName() const
{

  return _fileName;

}


