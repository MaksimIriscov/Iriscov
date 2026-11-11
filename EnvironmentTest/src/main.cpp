#include <iostream>

int main()
{
    std::cout << "EnvironmentTest OK" << std::endl;

#if defined(_clang_)
    std::cout << "Compiler: Clang" << _clang_major_ << "." << _clang_minor_ << "." << _clang_patchlevel_ << std::endl;
#elif defined(__GNUC__)
    std::cout << "Compiler: GCC" << _GNUC_ << "." << _GNUC_MINOR_ << "." << _GNUC_PATCHLEVEL_ << std::endl;
#elif defined(_MSC_VER)
    std::cout << "Compiler: MSVC" << _MSC_VER << std::endl;
#else
    std::cout << "Compiler: unknown" << std::endl;
#endif

    std::cout << "C++ Standard (_cplusplus):  " << __cplusplus << std::endl;

#ifdef NDEBUG
    std::cout << "Build type: Release" << std::endl;
#else
    std::cout << "Build type: Debug" << std::endl;
#endif

    std::cout << "Architecture: " << (sizeof(void*) * 8) << "-bit" << std::endl;

    return 0;
}