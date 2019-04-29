#ifndef ADBLOCK_TEST_FILTER_FILE_HPP
#define ADBLOCK_TEST_FILTER_FILE_HPP

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

class FilterFile : public std::ofstream
{
public:
    FilterFile()
        : m_path { std::tmpnam(nullptr) }
    {
        this->open(m_path);
        if (this->fail()) {
            std::cout << m_path << "\n";
            throw std::runtime_error { "file open error" };
        }

        *this << "[Adblock Plus 2.0]\n";
    }

    ~FilterFile()
    {
        fs::remove(m_path);
    }

    auto const& path() const { return m_path; }

private:
    fs::path m_path;
};

#endif // ADBLOCK_TEST_FILTER_FILE_HPP
