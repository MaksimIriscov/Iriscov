#include <stdexcept>

#include "Converter.h"

namespace NConverter
{

double Convert(double in, EConversionType ct)
{
    if (in < 0)
        throw std::invalid_argument("Input value must be greater than 0");

    switch (ct)
    {
        case EConversionType::MillimeterToMeter:
            return in / 1000;
        case EConversionType::CentimeterToMeter:
            return in / 100;
        case EConversionType::DecimeterToMeter:
            return in / 10;
        case EConversionType::MeterToKilometer:
            return in / 1000;
        default:
            throw std::invalid_argument("Invalid conversion type");
    }
}
}