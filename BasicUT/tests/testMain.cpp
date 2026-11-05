#include <iostream>
#include <vector>
#include <functional>
#include <string>

#include "../src/Converter.h"

class ISomeInterface
{
    public:
        ~ISomeInterface() = default;
        virtual void Method() = 0;
};

class CInterface : public ISomeInterface
{
    public:
        CInterface() = default;

        void Method() override
        {
             // OK
        };
};

struct STestCase
{
    std::string name;
    std::function<int()> func;
};

class CFixture
{
public:
    void AddTest(std::string testName, std::function<int()> test)
    {
        m_tests.push_back({std::move(testName), std::move(test)});
    }

    void Run() 
    {
        std::cout << "Running " << m_tests.size() << " tests..." << std::endl;

        for (size_t i = 0; i < m_tests.size(); ++i)
        {
            try
            {
                const auto result = m_tests[i].func();
                if (result == 0)
                {
                    m_passed++;
                    std::cout << "[PASS] " << m_tests[i].name << std::endl;
                }
                else
                {
                    m_failed++;
                    std::cout << "[FAIL] " << m_tests[i].name << ": " << result << std::endl;
                }
            }
            catch (const std::exception& e)
            {
                m_failed++;
                std::cout << "[FAIL] " << m_tests[i].name << ": " << e.what() << std::endl;
            }
        }

        std::cout << "Results: " << m_passed << " passed, " << m_failed << " failed\n";
    }

    int GetFailedCount() const
    {
        return m_failed;
    }

private:
    std::vector<STestCase> m_tests;
    int m_passed = 0;
    int m_failed = 0;
};

int TestNegativeInput()
{
    try
    {
        NConverter::Convert(-1.0, NConverter::EConversionType::MillimeterToMeter);
    }
    catch (const std::invalid_argument&)
    {
        return 0;
    }

    return 1;
}

int TestUnsupportedConversionType()
{
    try
    {
        NConverter::Convert(12., NConverter::EConversionType::UnsupportedConversion);
    }
    catch (const std::invalid_argument&)
    {
        return 0;
    }

    return 1;
}

int TestMillimeterToMeter()
{
    if (NConverter::Convert(0.0, NConverter::EConversionType::MillimeterToMeter) != 0.0) return 1;
    if (NConverter::Convert(1500.0, NConverter::EConversionType::MillimeterToMeter) != 1.5) return 1;
    return 0;
}

int TestCentimeterToMeter()
{
    if (NConverter::Convert(0.0, NConverter::EConversionType::CentimeterToMeter) != 0.0) return 1;
    if (NConverter::Convert(150.0, NConverter::EConversionType::CentimeterToMeter) != 1.5) return 1;
    return 0;
}

int TestDecimeterToMeter()
{
    if (NConverter::Convert(0.0, NConverter::EConversionType::DecimeterToMeter) != 0.0) return 1;
    if (NConverter::Convert(15.0, NConverter::EConversionType::DecimeterToMeter) != 1.5) return 1;
    return 0;
}

int TestMeterToKilometer()
{
    if (NConverter::Convert(0.0, NConverter::EConversionType::MeterToKilometer) != 0.0) return 1;
    if (NConverter::Convert(1500.0, NConverter::EConversionType::MeterToKilometer) != 1.5) return 1;
    return 0;
}

int main()
{
    CInterface i;
    CFixture f;

    f.AddTest("TestNegativeInput", TestNegativeInput);
    f.AddTest("TestUnsupportedConversionType", TestUnsupportedConversionType);
    f.AddTest("TestMillimeterToMeter", TestMillimeterToMeter);
    f.AddTest("TestCentimeterToMeter", TestCentimeterToMeter);
    f.AddTest("TestDecimeterToMeter", TestDecimeterToMeter);
    f.AddTest("TestMeterToKilometer", TestMeterToKilometer);

    f.Run();

    if (f.GetFailedCount() > 0)
    {
        std::cout << "Some tests failed." << std::endl;
        return 1;
    }
    return 0;
}