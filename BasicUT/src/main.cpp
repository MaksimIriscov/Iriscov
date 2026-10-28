#include <iostream>
#include <string>

#include "Converter.h"

std::ostream& operator<<(std::ostream& os, NConverter::EConversionType ct)
{
    switch (ct)
    {
        case NConverter::EConversionType::MillimeterToMeter:
            return os << "MillimeterToMeter";
        case NConverter::EConversionType::CentimeterToMeter:
            return os << "CentimeterToMeter";
        case NConverter::EConversionType::DecimeterToMeter:
            return os << "DecimeterToMeter";
        case NConverter::EConversionType::MeterToKilometer:
            return os << "MeterToKilometer";
        default:
            return os << "Unknown conversion type";
    }

}

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: --in <input> --ct <conversion_type>" << std::endl;
        std::cerr << "Conversion types: 0=MillimeterToMeter, 1=CentimeterToMeter, 2=DecimeterToMeter, 3=MeterToKilometer" << std::endl;
        return 1;
    }
    
    double input_value = 0.0;
    int conversion_type = -1;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i += 2)
    {
        if (i + 1 >= argc)
        {
            std::cerr << "Error: Missing value for argument " << argv[i] << std::endl;
            return 1;
        }
        
        std::string arg = argv[i];
        std::string value = argv[i + 1];
        
        if (arg == "--in")
        {
            try
            {
                input_value = std::stod(value);
            }
            catch (const std::exception&)
            {
                std::cerr << "Error: Invalid input value '" << value << "'. Must be a number." << std::endl;
                return 1;
            }
        }
        else if (arg == "--ct")
        {
            try
            {
                conversion_type = std::stoi(value);
                if (conversion_type < 0 || conversion_type > 3)
                {
                    std::cerr << "Error: Invalid conversion type '" << value << "'. Must be 0, 1, 2, or 3." << std::endl;
                    return 1;
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error: Invalid conversion type '" << value << "'. Must be a number." << std::endl;
                return 1;
            }
        }
        else
        {
            std::cerr << "Error: Unknown argument '" << arg << "'" << std::endl;
            return 1;
        }
    }
    
    auto ct = static_cast<NConverter::EConversionType>(conversion_type);
    std::cout << input_value << " " << ct << " equals " << NConverter::Convert(input_value, ct) << std::endl;

    return 0;
}