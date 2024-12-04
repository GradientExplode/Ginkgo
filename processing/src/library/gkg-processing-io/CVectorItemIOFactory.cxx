#include <gkg-processing-io/CVectorItemIOFactory.h>
#include <gkg-processing-io/CVectorAsciiItemWriter.h>
#include <gkg-processing-io/CVectorBinaryItemWriter.h>
#include <gkg-processing-io/CVectorBSwapItemWriter.h>
#include <gkg-processing-io/CVectorAsciiItemReader.h>
#include <gkg-processing-io/CVectorBinaryItemReader.h>
#include <gkg-processing-io/CVectorBSwapItemReader.h>


gkg::CVectorItemIOFactory::CVectorItemIOFactory()
{
}


gkg::CVectorItemIOFactory::~CVectorItemIOFactory()
{
}


gkg::ItemWriter< gkg::CVector >& 
gkg::CVectorItemIOFactory::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::CVectorAsciiItemWriter::getInstance();

  }
  else if ( bswap )
  {

    return gkg::CVectorBSwapItemWriter::getInstance();

  }
  else
  {

    return gkg::CVectorBinaryItemWriter::getInstance();

  }

}


gkg::ItemReader< gkg::CVector >&
gkg::CVectorItemIOFactory::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::CVectorAsciiItemReader::getInstance();

  }
  else if ( bswap )
  {

    return gkg::CVectorBSwapItemReader::getInstance();

  }
  else
  {

    return gkg::CVectorBinaryItemReader::getInstance();

  }

}
