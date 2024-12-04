#include <gkg-core-io/DefaultItemIOFactory.h>

#include <gkg-dmri-io/DefaultAsciiItemWriterOfTensor_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfTensor_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfTensor_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfTensor_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfTensor_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfTensor_i.h>

#include <gkg-dmri-io/DefaultAsciiItemWriterOfDiffusionFeature_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfDiffusionFeature_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfDiffusionFeature_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfDiffusionFeature_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfDiffusionFeature_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfDiffusionFeature_i.h>

#include <gkg-dmri-io/DefaultAsciiItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfOdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfOdf_i.h>

#include <gkg-dmri-io/DefaultAsciiItemWriterOfPdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfPdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfPdf_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfPdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfPdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfPdf_i.h>

#include <gkg-dmri-io/DefaultAsciiItemWriterOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfDiffusoid_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfDiffusoid_i.h>

#include <gkg-dmri-io/DefaultAsciiItemWriterOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfTwoPoolModel_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfTwoPoolModel_i.h>

#include <gkg-dmri-odf/OrientationDistributionFunction.h>


//
// class DefaultItemIOFactory< Tensor >
//


namespace gkg
{


template <>
DefaultItemIOFactory< Tensor >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< Tensor >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< Tensor >& 
gkg::DefaultItemIOFactory< Tensor >::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< Tensor >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< Tensor >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< Tensor >::getInstance();

  }

}


template <>
gkg::ItemReader< Tensor >& 
gkg::DefaultItemIOFactory< Tensor >::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< Tensor >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< Tensor >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< Tensor >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::Tensor >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::Tensor >& reader_tensor =
gkg::DefaultItemIOFactory< gkg::Tensor >::getInstance().getReader( false,
                                                                   false );
gkg::ItemWriter< gkg::Tensor >& writer_tensor =
gkg::DefaultItemIOFactory< gkg::Tensor >::getInstance().getWriter( false,
                                                                   false );

#endif



//
// class DefaultItemIOFactory< DiffusionFeature >
//


namespace gkg
{


template <>
DefaultItemIOFactory< DiffusionFeature >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< DiffusionFeature >::~DefaultItemIOFactory()
{
}


template <>
ItemWriter< DiffusionFeature >& 
DefaultItemIOFactory< DiffusionFeature >::getWriter(
                                                  bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< DiffusionFeature
                                 >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< DiffusionFeature
                                 >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< DiffusionFeature
                                  >::getInstance();

  }

}


template <>
ItemReader< DiffusionFeature >& 
DefaultItemIOFactory< DiffusionFeature >::getReader(
                                                  bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< DiffusionFeature
                                 >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< DiffusionFeature
                                 >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< DiffusionFeature
                                  >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::DiffusionFeature >;

// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::DiffusionFeature >& 
reader_diffusion_feature =
gkg::DefaultItemIOFactory< gkg::DiffusionFeature
                         >::getInstance().getReader( false, false );
gkg::ItemWriter< gkg::DiffusionFeature >& 
writer_diffusion_feature =
gkg::DefaultItemIOFactory< gkg::DiffusionFeature
                         >::getInstance().getWriter( false, false );


#endif


//
// class DefaultItemIOFactory< OrientationDistributionFunction >
//


namespace gkg
{


template <>
DefaultItemIOFactory< OrientationDistributionFunction >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< OrientationDistributionFunction >::~DefaultItemIOFactory()
{
}


template <>
ItemWriter< OrientationDistributionFunction >& 
DefaultItemIOFactory< OrientationDistributionFunction >::getWriter(
                                                  bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< OrientationDistributionFunction
                                 >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< OrientationDistributionFunction
                                 >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< OrientationDistributionFunction
                                  >::getInstance();

  }

}


template <>
ItemReader< OrientationDistributionFunction >& 
DefaultItemIOFactory< OrientationDistributionFunction >::getReader(
                                                  bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< OrientationDistributionFunction
                                 >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< OrientationDistributionFunction
                                 >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< OrientationDistributionFunction
                                  >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory<
                                        gkg::OrientationDistributionFunction >;

// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::OrientationDistributionFunction >& 
reader_odf =
gkg::DefaultItemIOFactory< gkg::OrientationDistributionFunction
                         >::getInstance().getReader( false, false );
gkg::ItemWriter< gkg::OrientationDistributionFunction >& 
writer_odf =
gkg::DefaultItemIOFactory< gkg::OrientationDistributionFunction
                         >::getInstance().getWriter( false, false );


#endif


//
// class DefaultItemIOFactory< ProbabilityDensityFunction >
//


namespace gkg
{


template <>
DefaultItemIOFactory< ProbabilityDensityFunction >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< ProbabilityDensityFunction >::~DefaultItemIOFactory()
{
}


template <>
ItemWriter< ProbabilityDensityFunction >& 
DefaultItemIOFactory< ProbabilityDensityFunction >::getWriter(
                                                  bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< ProbabilityDensityFunction
                                 >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< ProbabilityDensityFunction
                                 >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< ProbabilityDensityFunction
                                  >::getInstance();

  }

}


template <>
ItemReader< ProbabilityDensityFunction >& 
DefaultItemIOFactory< ProbabilityDensityFunction >::getReader(
                                                  bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< ProbabilityDensityFunction
                                 >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< ProbabilityDensityFunction
                                 >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< ProbabilityDensityFunction
                                  >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory<
                                        gkg::ProbabilityDensityFunction >;

// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::ProbabilityDensityFunction >& 
reader_pdf =
gkg::DefaultItemIOFactory< gkg::ProbabilityDensityFunction
                         >::getInstance().getReader( false, false );
gkg::ItemWriter< gkg::ProbabilityDensityFunction >& 
writer_pdf =
gkg::DefaultItemIOFactory< gkg::ProbabilityDensityFunction
                         >::getInstance().getWriter( false, false );


#endif


//
// class DefaultItemIOFactory< Diffusoid >
//


namespace gkg
{


template <>
DefaultItemIOFactory< Diffusoid >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< Diffusoid >::~DefaultItemIOFactory()
{
}


template <>
ItemWriter< Diffusoid >& 
DefaultItemIOFactory< Diffusoid >::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< Diffusoid >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< Diffusoid >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< Diffusoid >::getInstance();

  }

}


template <>
ItemReader< Diffusoid >& 
DefaultItemIOFactory< Diffusoid >::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< Diffusoid >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< Diffusoid >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< Diffusoid >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::Diffusoid >;

// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::Diffusoid >& reader_diffusoid =
gkg::DefaultItemIOFactory< gkg::Diffusoid
                         >::getInstance().getReader( false, false );
gkg::ItemWriter< gkg::Diffusoid >& writer_diffusoid =
gkg::DefaultItemIOFactory< gkg::Diffusoid
                         >::getInstance().getWriter( false, false );


#endif


//
// class DefaultItemIOFactory< TwoPoolModel >
//


namespace gkg
{


template <>
DefaultItemIOFactory< TwoPoolModel >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< TwoPoolModel >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< TwoPoolModel >& 
gkg::DefaultItemIOFactory< TwoPoolModel >::getWriter( bool ascii,
                                                      bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< TwoPoolModel >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< TwoPoolModel >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< TwoPoolModel >::getInstance();

  }

}


template <>
gkg::ItemReader< TwoPoolModel >& 
gkg::DefaultItemIOFactory< TwoPoolModel >::getReader( bool ascii,
                                                      bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< TwoPoolModel >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< TwoPoolModel >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< TwoPoolModel >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::TwoPoolModel >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::TwoPoolModel >& reader_two_pool_model =
gkg::DefaultItemIOFactory< gkg::TwoPoolModel >::getInstance().getReader(
                                                                        false,
                                                                        false );
gkg::ItemWriter< gkg::TwoPoolModel >& writer_two_pool_model =
gkg::DefaultItemIOFactory< gkg::TwoPoolModel >::getInstance().getWriter(
                                                                        false,
                                                                        false );

#endif




