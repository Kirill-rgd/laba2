#include "modeBetaCipher.h"
#include <vector>
#include <cstddef>

namespace {
wchar_t upperRussian(wchar_t c) {
    if (c >= L'а' && c <= L'я') return static_cast<wchar_t>(c - L'а' + L'А');
    if (c == L'ё') return L'Ё';
    return c;
}
}

modeBetaCipher::modeBetaCipher(unsigned key) {
    if (key < 2) throw cipher_error("Invalid key: columns count must be >= 2");
    cols = key;
}

std::wstring modeBetaCipher::getValidOpenText(const std::wstring& s) {
    std::wstring tmp;
    for (wchar_t ch : s) {
        wchar_t c = upperRussian(ch);
        if (c >= L'А' && c <= L'Я' && c != L'Ё') tmp.push_back(c);
    }
    if (tmp.empty()) throw cipher_error("Empty open text");
    return tmp;
}

std::wstring modeBetaCipher::getValidCipherText(const std::wstring& s) {
    if (s.empty()) throw cipher_error("Empty cipher text");
    for (wchar_t c : s)
        if (c < L'А' || c > L'Я' || c == L'Ё')
            throw cipher_error("Invalid cipher text: unknown symbol");
    return s;
}

std::wstring modeBetaCipher::encrypt(const std::wstring& text) {
    const std::wstring open = getValidOpenText(text);
    const std::size_t n = open.size();
    const std::size_t rows = (n + cols - 1) / cols;

    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(cols, L'_'));
    std::size_t k = 0;
    for (std::size_t r = 0; r < rows; ++r)
        for (std::size_t c = 0; c < cols; ++c)
            if (k < n) table[r][c] = open[k++];

    std::wstring result;
    result.reserve(n);
    for (std::size_t c = cols; c > 0; --c)
        for (std::size_t r = 0; r < rows; ++r)
            if (table[r][c - 1] != L'_') result.push_back(table[r][c - 1]);
    
    return result;
}

std::wstring modeBetaCipher::decrypt(const std::wstring& text) {
    const std::wstring cipher = getValidCipherText(text);
    const std::size_t n = cipher.size();
    const std::size_t rows = (n + cols - 1) / cols;
    const std::size_t rest = n % cols;
    const std::size_t fullColumns = (rest == 0) ? cols : rest;

    std::vector<std::size_t> height(cols);
    for (std::size_t c = 0; c < cols; ++c)
        height[c] = (c < fullColumns) ? rows : rows - 1;

    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(cols, L'_'));
    std::size_t k = 0;
    for (std::size_t c = cols; c > 0; --c)
        for (std::size_t r = 0; r < height[c - 1]; ++r)
            table[r][c - 1] = cipher[k++];

    std::wstring result;
    result.reserve(n);
    for (std::size_t r = 0; r < rows; ++r)
        for (std::size_t c = 0; c < cols; ++c)
            if (table[r][c] != L'_') result.push_back(table[r][c]);

    return result;
}