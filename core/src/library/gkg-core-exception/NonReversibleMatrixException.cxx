#include <gkg-core-exception/NonReversibleMatrixException.h>


gkg::NonReversibleMatrixException::NonReversibleMatrixException(
                                                   const std::string& argument )
                                  : std::runtime_error( argument +
                                                     "non reversible matrix" )
{
}


gkg::NonReversibleMatrixException::~NonReversibleMatrixException() throw()
{
}


