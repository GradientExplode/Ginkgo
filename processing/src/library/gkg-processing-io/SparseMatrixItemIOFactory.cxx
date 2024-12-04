#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-processing-io/SparseMatrixAsciiItemWriter.h>
#include <gkg-processing-io/SparseMatrixBinaryItemWriter.h>
#include <gkg-processing-io/SparseMatrixBSwapItemWriter.h>
#include <gkg-processing-io/SparseMatrixAsciiItemReader.h>
#include <gkg-processing-io/SparseMatrixBinaryItemReader.h>
#include <gkg-processing-io/SparseMatrixBSwapItemReader.h>


gkg::SparseMatrixItemIOFactory::SparseMatrixItemIOFactory()
{
}


gkg::SparseMatrixItemIOFactory::~SparseMatrixItemIOFactory()
{
}


gkg::ItemWriter< gkg::SparseMatrix >& 
gkg::SparseMatrixItemIOFactory::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::SparseMatrixAsciiItemWriter::getInstance();

  }
  else if ( bswap )
  {

    return gkg::SparseMatrixBSwapItemWriter::getInstance();

  }
  else
  {

    return gkg::SparseMatrixBinaryItemWriter::getInstance();

  }

}


gkg::ItemReader< gkg::SparseMatrix >&
gkg::SparseMatrixItemIOFactory::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::SparseMatrixAsciiItemReader::getInstance();

  }
  else if ( bswap )
  {

    return gkg::SparseMatrixBSwapItemReader::getInstance();

  }
  else
  {

    return gkg::SparseMatrixBinaryItemReader::getInstance();

  }

}
