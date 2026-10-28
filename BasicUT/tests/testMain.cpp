#include <iostream>
#include <vector>
#include <functional>

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

class CFixture
{
public:
    void AddTest(std::function<int()> test)
    {
        m_tests.emplace_back(std::move(test));
    }

    void Run() 
    {
        std::cout << "Running " << m_tests.size() << " tests..." << std::endl;

        for (size_t i = 0; i < m_tests.size(); ++i)
        {
            try
            {
                const auto result = m_tests[i]();
                if (result == 0)
                {
                    m_passed++;
                    std::cout << "[PASS] Test " << (i + 1) << std::endl;
                }
                else
                {
                    m_failed++;
                    std::cout << "[FAIL] Test " << (i + 1) << ": " << result << std::endl;
                }
            }
            catch (const std::exception& e)
            {
                m_failed++;
                std::cout << "[FAIL] Test " << (i + 1) << ": " << e.what() << std::endl;
            }
        }

        std::cout << "Results: " << m_passed << " passed, " << m_failed << " failed\n";
    }

private:
    std::vector<std::function<int()>> m_tests;
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

int main()
{
    CInterface i;
    CFixture f;

    f.AddTest(TestNegativeInput);
    f.AddTest(TestUnsupportedConversionType);

    f.Run();

    return 0;
}