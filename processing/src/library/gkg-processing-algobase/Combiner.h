#ifndef _gkg_processing_algobase_Combiner_h_
#define _gkg_processing_algobase_Combiner_h_


#include <gkg-processing-algobase/Converter.h>
#include <vector>
#include <string>


namespace gkg
{


template < class T > class TestFunction;
template < class T > class Volume;


//
// class Combiner< IN, MASK, OUT >
//

template < class IN, class MASK, class OUT >
class Combiner
{

  public:

    typedef double ( *Functor )( const double& );
    typedef void ( *Operator )( const double&, double& );

    Combiner( const std::vector< std::string >& functor1s,
              const std::vector< std::string >& functor2s,
              const std::vector< double >& numerator1s,
              const std::vector< double >& denominator1s,
              const std::vector< double >& numerator2s,
              const std::vector< double >& denominator2s,
              const std::vector< std::string >& operators,
              const TestFunction< MASK >* testFunction = 0,
              const OUT& background = 0 );

    int32_t getInputCount() const;

    void combine( const std::vector< IN >& inputValues,
                  const MASK& maskValue,
                  OUT& outputValue,
                  bool /*verbose = false*/ ) const;
    void combine( const std::vector< IN >& inputValues,
                  OUT& outputValue,
                  bool /*verbose = false*/ ) const;

  private:

    static Functor getFunctor( const std::string& functorName );
    static Operator getOperator( const std::string& operatorName );

    static void operatorPlus( const double& v1, double& v2 ); 
    static void operatorMinus( const double& v1, double& v2 ); 
    static void operatorMultiply( const double& v1, double& v2 ); 
    static void operatorDivide( const double& v1, double& v2 ); 

    static double functorId( const double& v ); 
    static double functorInv( const double& v ); 
    static double functorLn( const double& v ); 
    static double functorLog( const double& v ); 
    static double functorExp( const double& v ); 
    static double functorSqr( const double& v ); 
    static double functorCub( const double& v ); 
    static double functorSqrt( const double& v ); 
    static double functorCbrt( const double& v ); 
    static double functorCos( const double& v ); 
    static double functorSin( const double& v ); 
    static double functorTan( const double& v ); 
    static double functorAcos( const double& v ); 
    static double functorAsin( const double& v ); 
    static double functorAtan( const double& v ); 

    int32_t _inputCount;
    std::vector< Functor > _functor1s;
    std::vector< Functor > _functor2s;
    std::vector< double > _numerator1s;
    std::vector< double > _denominator1s;
    std::vector< double > _numerator2s;
    std::vector< double > _denominator2s;
    std::vector< Operator > _operators;
    const TestFunction< MASK >* _testFunction;
    OUT _background;
    Converter< IN, double > _converterInToDouble;
    Converter< double, OUT > _converterDoubleToOut;

};


//
// class Combiner< Volume< IN >, Volume< MASK >, Volume< OUT > >
//

template < class IN, class MASK, class OUT >
class Combiner< Volume< IN >, Volume< MASK >, Volume< OUT > >
{

  public:

    Combiner( const std::vector< std::string >& functor1s,
              const std::vector< std::string >& functor2s,
              const std::vector< double >& numerator1s,
              const std::vector< double >& denominator1s,
              const std::vector< double >& numerator2s,
              const std::vector< double >& denominator2s,
              const std::vector< std::string >& operators,
              const TestFunction< MASK >* testFunction = 0,
              const OUT& background = 0 );

    void combine( std::vector< Volume< IN > >& inputVolumes,
                  Volume< MASK >& maskVolume,
                  Volume< OUT >& outputVolume,
                  bool verbose = false ) const;
    void combine( std::vector< Volume< IN > >& inputVolumes,
                  Volume< OUT >& outputVolume,
                  bool verbose = false ) const;

  private:

    void reallocate( const Volume< IN >& in, Volume< OUT >& out ) const;

    Combiner< IN, MASK, OUT > _itemCombiner;

};


}


#endif
