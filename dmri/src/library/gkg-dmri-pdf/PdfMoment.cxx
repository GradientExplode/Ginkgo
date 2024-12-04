#include <gkg-dmri-pdf/PdfMoment.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfMoment::PdfMoment( int32_t order, gkg::PdfMoment::BasisType basisType )
               : gkg::DiffusionFeature(),
                 _order( order ),
                 _basisType( basisType )
{

  try
  {

    if ( _order < 0 )
    {

      throw std::runtime_error( "order must be strictly positive" );

    }

  }
  GKG_CATCH( "gkg::PdfMoment::PdfMoment( int32_t order )" );

}


gkg::PdfMoment::PdfMoment( const gkg::PdfMoment& other )
               : gkg::DiffusionFeature( other ),
                 _order( other._order ),
                 _basisType( other._basisType )
{
}


gkg::PdfMoment::~PdfMoment()
{
}


gkg::PdfMoment& gkg::PdfMoment::operator=( const gkg::PdfMoment& other )
{

  try
  {

    this->gkg::DiffusionFeature::operator=( other );
    _order = other._order;
    _basisType = other._basisType;

    return *this;

  }
  GKG_CATCH( "gkg::PdfMoment& "
             "gkg::PdfMoment::operator=( const gkg::PdfMoment& other )" );

}


gkg::PdfMoment::BasisType gkg::PdfMoment::getBasisType() const
{

  try
  {

    return _basisType;

  }
  GKG_CATCH( "gkg::PdfMoment::BasisType gkg::PdfMoment::getBasisType() const" );

}


std::string gkg::PdfMoment::getBasisTypeName() const
{

  try
  {

    std::string basisTypeName = "";
    if ( _basisType == gkg::PdfMoment::Standard )
    {

      basisTypeName = "std";

    }
    return basisTypeName;

  }
  GKG_CATCH( "std::string gkg::PdfMoment::getBasisTypeName() const" );

}


void gkg::PdfMoment::setBasisType( gkg::PdfMoment::BasisType basisType )
{

  try
  {

    _basisType = basisType;

  }
  GKG_CATCH( "void gkg::PdfMoment::setBasisType( "
             "gkg::PdfMoment::BasisType basisType )" );

}


void gkg::PdfMoment::setBasisType( const std::string& basisTypeName )
{

  try
  {

    if ( basisTypeName == "std" )
    {

      _basisType = gkg::PdfMoment::Standard;

    }

  }
  GKG_CATCH( "void gkg::PdfMoment::setBasisType( "
             "const std::string& basisTypeName )" );

}


void gkg::PdfMoment::setStandardBasisType()
{

  try
  {

    if ( _basisType == gkg::PdfMoment::Standard )
    {

      // nothing special to do

    }
    _basisType = gkg::PdfMoment::Standard;

  }
  GKG_CATCH( "void gkg::PdfMoment::setStandardBasisType()" );

}


int32_t gkg::PdfMoment::getOrder() const
{

  try
  {

    return _order;

  }
  GKG_CATCH( "int32_t gkg::PdfMoment::getOrder() const" );

}



