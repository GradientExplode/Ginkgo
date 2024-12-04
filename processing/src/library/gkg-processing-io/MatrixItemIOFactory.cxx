#include <gkg-processing-io/MatrixItemIOFactory.h>
#include <gkg-processing-io/MatrixAsciiItemWriter.h>
#include <gkg-processing-io/MatrixBinaryItemWriter.h>
#include <gkg-processing-io/MatrixBSwapItemWriter.h>
#include <gkg-processing-io/MatrixAsciiItemReader.h>
#include <gkg-processing-io/MatrixBinaryItemReader.h>
#include <gkg-processing-io/MatrixBSwapItemReader.h>


gkg::MatrixItemIOFactory::MatrixItemIOFactory()
{
}


gkg::MatrixItemIOFactory::~MatrixItemIOFactory()
{
}


gkg::ItemWriter< gkg::Matrix >& 
gkg::MatrixItemIOFactory::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::MatrixAsciiItemWriter::getInstance();

  }
  else if ( bswap )
  {

    return gkg::MatrixBSwapItemWriter::getInstance();

  }
  else
  {

    return gkg::MatrixBinaryItemWriter::getInstance();

  }

}


gkg::ItemReader< gkg::Matrix >&
gkg::MatrixItemIOFactory::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::MatrixAsciiItemReader::getInstance();

  }
  else if ( bswap )
  {

    return gkg::MatrixBSwapItemReader::getInstance();

  }
  else
  {

    return gkg::MatrixBinaryItemReader::getInstance();

  }

}
