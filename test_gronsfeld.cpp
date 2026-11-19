#include <UnitTest++/UnitTest++.h>
#include <clocale>
#include "modAlphaCipher.h"

using namespace std;

struct GlobalInit {
    GlobalInit() { setlocale(LC_ALL, "ru_RU.UTF-8"); }
};

static GlobalInit globalInit;

struct KeyB_fixture {
    modAlphaCipher* p;
    KeyB_fixture() { p = new modAlphaCipher(L"Б"); }
    ~KeyB_fixture() { delete p; }
};

SUITE(GronsfeldKeyTests) {
    TEST(ValidKey) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK(cipher.encrypt(L"ПРИВЕТ").size() == 6);
    }

    TEST(LowCaseKey) {
        modAlphaCipher cipher(L"ключ");
        CHECK(cipher.encrypt(L"ПРИВЕТ").size() == 6);
    }

    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"КЛ1ЮЧ"), cipher_error);
    }

    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"К,ЛЮЧ"), cipher_error);
    }

    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"К ЛЮЧ"), cipher_error);
    }

    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cipher(L""), cipher_error);
    }
}

SUITE(GronsfeldEncryptTests) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK(p->encrypt(L"ПРИВЕТ") == L"РСЙГЖУ");
    }

    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK(p->encrypt(L"привет") == L"РСЙГЖУ");
    }

    TEST_FIXTURE(KeyB_fixture, StringWithSpaces) {
        CHECK(p->encrypt(L"ПРИ ВЕТ") == L"РСЙГЖУ");
    }

    TEST_FIXTURE(KeyB_fixture, StringWithNumbers) {
        CHECK(p->encrypt(L"ПРИ123ВЕТ") == L"РСЙГЖУ");
    }

    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt(L"12345"), cipher_error);
    }

    TEST(MaxShiftKey) {
        modAlphaCipher cipher(L"Я");
        wstring r = cipher.encrypt(L"ПРИВЕТ");
        CHECK(r.size() == 6);
        CHECK(cipher.decrypt(r) == L"ПРИВЕТ");
    }
}

SUITE(GronsfeldDecryptTests) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK(p->decrypt(L"РСЙГЖУ") == L"ПРИВЕТ");
    }

    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_THROW(p->decrypt(L"рсйгжу"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, WhitespaceString) {
        CHECK_THROW(p->decrypt(L"РСЙ ГЖУ"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, DigitsString) {
        CHECK_THROW(p->decrypt(L"РСЙ123"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, PunctString) {
        CHECK_THROW(p->decrypt(L"РСЙ,ГЖУ"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }

    TEST(MaxShiftKey) {
        modAlphaCipher cipher(L"Я");
        wstring e = cipher.encrypt(L"ПРИВЕТ");
        CHECK(cipher.decrypt(e) == L"ПРИВЕТ");
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    return UnitTest::RunAllTests();
}
