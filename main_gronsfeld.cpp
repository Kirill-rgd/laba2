#include <iostream>
#include <locale>
#include <limits>
#include <string>
#include "modAlphaCipher.h"

using namespace std;

int main() {
    try { locale::global(locale("ru_RU.UTF-8")); } 
    catch (const exception&) { cerr << "Locale error\n"; return 1; }
    
    wcin.imbue(locale()); wcout.imbue(locale()); cerr.imbue(locale());

    wcout << L"Шифр Гронсфельда. Введите ключ: ";
    wstring skey;
    if (!(wcin >> skey)) { wcerr << L"Ошибка чтения\n"; return 1; }

    try {
        modAlphaCipher cipher(skey);
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
				std::wstring result;
				if (op == 1) {
					result = cipher.encrypt(text);
					wcout << L"Шифртекст: " << result << '\n';
				} else {
					result = cipher.decrypt(text);
					wcout << L"Открытый текст: " << result << '\n';
				}
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