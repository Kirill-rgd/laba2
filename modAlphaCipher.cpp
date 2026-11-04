#include "modAlphaCipher.h"
#include <cstddef>

namespace {
wchar_t upperRussian(wchar_t c) {
    if (c >= L'а' && c <= L'я') return static_cast<wchar_t>(c - L'а' + L'А');
    if (c == L'ё') return L'Ё';
    return c;
}
}

modAlphaCipher::modAlphaCipher(const std::wstring& skey) {
    for (std::size_t i = 0; i < numAlpha.size(); ++i)
        alphaNum[numAlpha[i]] = static_cast<int>(i);
    key = convert(getValidKey(skey));
}

std::wstring modAlphaCipher::getValidKey(const std::wstring& s) {
    if (s.empty()) throw cipher_error("Empty key");
    std::wstring tmp;
    for (wchar_t ch : s) {
        wchar_t c = upperRussian(ch);
        if (numAlpha.find(c) == std::wstring::npos)
            throw cipher_error("Invalid key: unknown symbol");
        tmp.push_back(c);
    }
    return tmp;
}

std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s) {
    std::wstring tmp;
    for (wchar_t ch : s) {
        wchar_t c = upperRussian(ch);
        if (numAlpha.find(c) != std::wstring::npos) tmp.push_back(c);
    }
    if (tmp.empty()) throw cipher_error("Empty open text");
    return tmp;
}

std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s) {
    if (s.empty()) throw cipher_error("Empty cipher text");
    for (wchar_t c : s)
        if (numAlpha.find(c) == std::wstring::npos)
            throw cipher_error("Invalid cipher text: unknown symbol");
    return s;
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text) {
    std::vector<int> work = convert(getValidOpenText(open_text));
    const int mod = static_cast<int>(numAlpha.size());
    for (std::size_t i = 0; i < work.size(); ++i)
        work[i] = (work[i] + key[i % key.size()]) % mod;
    return convert(work);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text) {
    std::vector<int> work = convert(getValidCipherText(cipher_text));
    const int mod = static_cast<int>(numAlpha.size());
    for (std::size_t i = 0; i < work.size(); ++i)
        work[i] = (work[i] + mod - key[i % key.size()]) % mod;
    return convert(work);
}

inline std::vector<int> modAlphaCipher::convert(const std::wstring& s) {
    std::vector<int> result;
    for (wchar_t c : s) result.push_back(alphaNum.at(c));
    return result;
}

inline std::wstring modAlphaCipher::convert(const std::vector<int>& v) {
    std::wstring result;
    for (int i : v) result.push_back(numAlpha.at(static_cast<std::size_t>(i)));
    return result;
}