#pragma once
#include <string>
#include <vector>
#include <map>
#include "cipher_error.h"

class modAlphaCipher {
private:
    std::wstring numAlpha = L"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::map<wchar_t, int> alphaNum;
    std::vector<int> key;

    // Закрытые методы преобразования
    std::vector<int> convert(const std::wstring& s);
    std::wstring convert(const std::vector<int>& v);

    // Методы валидации (Лабораторная №2)
    std::wstring getValidKey(const std::wstring& s);
    std::wstring getValidOpenText(const std::wstring& s);
    std::wstring getValidCipherText(const std::wstring& s);

public:
    modAlphaCipher() = delete;
    explicit modAlphaCipher(const std::wstring& skey);

    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};