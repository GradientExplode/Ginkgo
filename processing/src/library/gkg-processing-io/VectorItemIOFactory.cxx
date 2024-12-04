#include <gkg-processing-io/VectorItemIOFactory.h>
#include <gkg-processing-io/VectorAsciiItemWriter.h>
#include <gkg-processing-io/VectorBinaryItemWriter.h>
#include <gkg-processing-io/VectorBSwapItemWriter.h>
#include <gkg-processing-io/VectorAsciiItemReader.h>
#include <gkg-processing-io/VectorBinaryItemReader.h>
#include <gkg-processing-io/VectorBSwapItemReader.h>


gkg::VectorItemIOFactory::VectorItemIOFactory()
{
}


gkg::VectorItemIOFactory::~VectorItemIOFactory()
{
}


gkg::ItemWriter< gkg::Vector >& 
gkg::VectorItemIOFactory::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::VectorAsciiItemWriter::getInstance();

  }
  else if ( bswap )
  {

    return gkg::VectorBSwapItemWriter::getInstance();

  }
  else
  {

    return gkg::VectorBinaryItemWriter::getInstance();

  }

}


gkg::ItemReader< gkg::Vector >& 
gkg::VectorItemIOFactory::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::VectorAsciiItemReader::getInstance();

  }
  else if ( bswap )
  {

    return gkg::VectorBSwapItemReader::getInstance();

  }
  else
  {

    return gkg::VectorBinaryItemReader::getInstance();

  }

}
