#include <iostream>
#include <locale>
#include <limits>
#include <string>
#include "modeBetaCipher.h"

using namespace std;

int main() {
    try { locale::global(locale("ru_RU.UTF-8")); } 
    catch (const exception&) { cerr << "Locale error\n"; return 1; }
    
    wcin.imbue(locale()); wcout.imbue(locale()); cerr.imbue(locale());

    unsigned key = 0;
    wcout << L"Маршрутная перестановка. Столбцов (>=2): ";
    if (!(wcin >> key)) { wcerr << L"Ошибка ввода\n"; return 1; }

    try {
        modeBetaCipher cipher(key);
        wcout << L"Ключ принят\n";

        unsigned op = 0;
        do {
            wcout << L"\nОперация (0-выход, 1-шифр, 2-дешифр): ";
            if (!(wcin >> op)) break;
            if (op > 2) { wcout << L"Недопустимая операция\n"; continue; }
            if (op == 0) break;

            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Введите текст: ";
            wstring text;
            if (!getline(wcin, text)) break;

            try {
                if (op == 1) wcout << L"Шифртекст: " << cipher.encrypt(text) << '\n';
                else         wcout << L"Открытый текст: " << cipher.decrypt(text) << '\n';
            } catch (const cipher_error& e) {
                cerr << "Ошибка: " << e.what() << '\n';
            }
        } while (true);

    } catch (const cipher_error& e) {
        cerr << "Ошибка ключа: " << e.what() << '\n';
        return 1;
    }
    return 0;
}