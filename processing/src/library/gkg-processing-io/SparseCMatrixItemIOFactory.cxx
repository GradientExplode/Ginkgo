#include <gkg-processing-io/SparseCMatrixItemIOFactory.h>
#include <gkg-processing-io/SparseCMatrixAsciiItemWriter.h>
#include <gkg-processing-io/SparseCMatrixBinaryItemWriter.h>
#include <gkg-processing-io/SparseCMatrixBSwapItemWriter.h>
#include <gkg-processing-io/SparseCMatrixAsciiItemReader.h>
#include <gkg-processing-io/SparseCMatrixBinaryItemReader.h>
#include <gkg-processing-io/SparseCMatrixBSwapItemReader.h>


gkg::SparseCMatrixItemIOFactory::SparseCMatrixItemIOFactory()
{
}


gkg::SparseCMatrixItemIOFactory::~SparseCMatrixItemIOFactory()
{
}


gkg::ItemWriter< gkg::SparseCMatrix >& 
gkg::SparseCMatrixItemIOFactory::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::SparseCMatrixAsciiItemWriter::getInstance();

  }
  else if ( bswap )
  {

    return gkg::SparseCMatrixBSwapItemWriter::getInstance();

  }
  else
  {

    return gkg::SparseCMatrixBinaryItemWriter::getInstance();

  }

}


gkg::ItemReader< gkg::SparseCMatrix >&
gkg::SparseCMatrixItemIOFactory::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::SparseCMatrixAsciiItemReader::getInstance();

  }
  else if ( bswap )
  {

    return gkg::SparseCMatrixBSwapItemReader::getInstance();

  }
  else
  {

    return gkg::SparseCMatrixBinaryItemReader::getInstance();

  }

}
