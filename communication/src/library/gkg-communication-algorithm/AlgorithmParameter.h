#ifndef _gkg_communication_algorithm_AlgorithmParameter_h_
#define _gkg_communication_algorithm_AlgorithmParameter_h_


#include <string>


namespace gkg
{


class ParameterSignature
{

  public:

    ParameterSignature( const std::string& name,
                        const std::string& type,
                        bool isInput,
                        bool isOutput );
    ParameterSignature( const ParameterSignature& other );
    virtual ~ParameterSignature();

    const std::string& getName() const;
    const std::string& getType() const;
    bool isInput() const;
    bool isOutput() const;

    std::string getCode() const;

  protected:

    std::string _name;
    std::string _type;
    bool _isInput;
    bool _isOutput;

};


class AlgorithmParameter
{


  public:

    AlgorithmParameter( const std::string& name,
                        const std::string& type,
                        bool isInput,
                        bool isOutput,
                        const std::string& description = "" );
    AlgorithmParameter( const AlgorithmParameter& other );
    virtual ~AlgorithmParameter();

    const ParameterSignature& getSignature() const;
    const std::string& getDescription() const;

    void setDescription( const std::string& description );

    std::string getCode() const;

    template < class T >
    void set( const T& value );

    void connect( AlgorithmParameter* other );

  protected:

    ParameterSignature _signature;
    std::string _description;


};


}


#endif
