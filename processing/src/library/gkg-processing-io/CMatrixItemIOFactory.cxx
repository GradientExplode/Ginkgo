#include <gkg-processing-io/CMatrixItemIOFactory.h>
#include <gkg-processing-io/CMatrixAsciiItemWriter.h>
#include <gkg-processing-io/CMatrixBinaryItemWriter.h>
#include <gkg-processing-io/CMatrixBSwapItemWriter.h>
#include <gkg-processing-io/CMatrixAsciiItemReader.h>
#include <gkg-processing-io/CMatrixBinaryItemReader.h>
#include <gkg-processing-io/CMatrixBSwapItemReader.h>


gkg::CMatrixItemIOFactory::CMatrixItemIOFactory()
{
}


gkg::CMatrixItemIOFactory::~CMatrixItemIOFactory()
{
}


gkg::ItemWriter< gkg::CMatrix >& 
gkg::CMatrixItemIOFactory::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::CMatrixAsciiItemWriter::getInstance();

  }
  else if ( bswap )
  {

    return gkg::CMatrixBSwapItemWriter::getInstance();

  }
  else
  {

    return gkg::CMatrixBinaryItemWriter::getInstance();

  }

}


gkg::ItemReader< gkg::CMatrix >&
gkg::CMatrixItemIOFactory::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::CMatrixAsciiItemReader::getInstance();

  }
  else if ( bswap )
  {

    return gkg::CMatrixBSwapItemReader::getInstance();

  }
  else
  {

    return gkg::CMatrixBinaryItemReader::getInstance();

  }

}
