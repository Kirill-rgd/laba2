#pragma once
#include <string>
#include "cipher_error.h"

class modeBetaCipher {
private:
    unsigned cols;
    static const unsigned MAX_COLUMNS = 10000;

    std::wstring getValidOpenText(const std::wstring& s);
    std::wstring getValidCipherText(const std::wstring& s);

public:
    modeBetaCipher() = delete;
    explicit modeBetaCipher(unsigned key);

    std::wstring encrypt(const std::wstring& text);
    std::wstring decrypt(const std::wstring& text);
};