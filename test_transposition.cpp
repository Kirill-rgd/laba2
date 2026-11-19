#include <UnitTest++/UnitTest++.h>
#include <clocale>
#include "modeBetaCipher.h"

using namespace std;

struct GlobalInit {
    GlobalInit() { setlocale(LC_ALL, "ru_RU.UTF-8"); }
};

static GlobalInit globalInit;

SUITE(TranspositionConstructorTests) {
    TEST(ValidKey) {
        modeBetaCipher cipher(3);
        CHECK(true);
    }

    TEST(MinimumKey) {
        modeBetaCipher cipher(2);
        CHECK(true);
    }

    TEST(KeyTooSmall) {
        CHECK_THROW(modeBetaCipher cipher(1), cipher_error);
    }

    TEST(ZeroKey) {
        CHECK_THROW(modeBetaCipher cipher(0), cipher_error);
    }

    TEST(KeyTooLarge) {
        CHECK_THROW(modeBetaCipher cipher(50000), cipher_error);
    }

    TEST(MaxAllowedKey) {
        modeBetaCipher cipher(10000);
        CHECK(true);
    }
}

SUITE(TranspositionEncryptTests) {
    TEST(NormalText) {
        modeBetaCipher cipher(3);
        CHECK(cipher.encrypt(L"ПРИВЕТ") == L"ИТРЕПВ");
    }

    TEST(SingleRow) {
        modeBetaCipher cipher(3);
        CHECK(cipher.encrypt(L"АБВ") == L"ВБА");
    }

    TEST(IncompleteLastRow) {
        modeBetaCipher cipher(3);
        CHECK(cipher.encrypt(L"АБВГ") == L"ВБАГ");
    }

    TEST(EmptyText) {
        modeBetaCipher cipher(3);
        CHECK_THROW(cipher.encrypt(L""), cipher_error);
    }

    TEST(KeyGreaterThanText) {
        modeBetaCipher cipher(10);
        CHECK_THROW(cipher.encrypt(L"ПРИ"), cipher_error);
    }

    TEST(KeyEqualToTextLength) {
        modeBetaCipher cipher(3);
        CHECK(cipher.encrypt(L"АБВ") == L"ВБА");
    }

    TEST(EncryptDecryptRoundTrip) {
        modeBetaCipher cipher(4);
        wstring original = L"ТЕСТИРОВАНИЕПЕРЕСТАНОВКИ";
        wstring encrypted = cipher.encrypt(original);
        CHECK(original == cipher.decrypt(encrypted));
    }

    TEST(LargeKey) {
        modeBetaCipher cipher(100);
        wstring text(200, L'А');
        CHECK(cipher.encrypt(text).size() == 200);
    }
}

SUITE(TranspositionDecryptTests) {
    TEST(NormalText) {
        modeBetaCipher cipher(3);
        CHECK(cipher.decrypt(L"ИТРЕПВ") == L"ПРИВЕТ");
    }

    TEST(SingleRow) {
        modeBetaCipher cipher(3);
        CHECK(cipher.decrypt(L"ВБА") == L"АБВ");
    }

    TEST(EmptyText) {
        modeBetaCipher cipher(3);
        CHECK_THROW(cipher.decrypt(L""), cipher_error);
    }

    TEST(KeyGreaterThanText) {
        modeBetaCipher cipher(10);
        CHECK_THROW(cipher.decrypt(L"ПРИ"), cipher_error);
    }

    TEST(KeyEqualToTextLength) {
        modeBetaCipher cipher(3);
        CHECK(cipher.decrypt(L"ВБА") == L"АБВ");
    }

    TEST(InvalidCipherText) {
        modeBetaCipher cipher(3);
        CHECK_THROW(cipher.decrypt(L"итрепв"), cipher_error);
    }

    TEST(DigitsInCipher) {
        modeBetaCipher cipher(3);
        CHECK_THROW(cipher.decrypt(L"ИТР123"), cipher_error);
    }

    TEST(EncryptDecryptRoundTrip) {
        modeBetaCipher cipher(5);
        wstring original = L"ДЛИННОЕСЛОВОДЛЯТЕСТИРОВАНИЯ";
        wstring encrypted = cipher.encrypt(original);
        CHECK(original == cipher.decrypt(encrypted));
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    return UnitTest::RunAllTests();
}
