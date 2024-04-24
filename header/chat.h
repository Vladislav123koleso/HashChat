#pragma once
#include <iostream>
#include <string.h>
#include "sha1.h"


#define SIZE 10
#define LOGINLENGTH 10

class Chat {
public:
    Chat();
    void reg(char _login[LOGINLENGTH], char _pass[], int pass_length);
    bool login(char _login[LOGINLENGTH], char _pass[], int pass_length);

    friend void test(Chat& c);

private:
    struct AuthData {
        AuthData():
            login(""),
            pass_sha1_hash(nullptr) {
        }
        ~AuthData() {
            if (pass_sha1_hash != nullptr)
                delete [] pass_sha1_hash;
        }
        AuthData(char _login[LOGINLENGTH], uint* sh1) {
            strcpy(login, _login);
            pass_sha1_hash = sh1;
        }
        AuthData& operator = (const AuthData& other) {
            strcpy(login, other.login);

            if (pass_sha1_hash != nullptr)
                delete [] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);

            return *this;
        }           
        char login[LOGINLENGTH];
        uint* pass_sha1_hash;
    };

    AuthData data[SIZE];
    int data_count;

    // Хеш-таблица для хранения логинов и хешей паролей
    AuthData* hash_table[SIZE];
    int hash_table_size;

    // Хеш-функция методом умножения
    int hash_function(const char* key);
    // Квадратичное пробирование для разрешения коллизий
    int quadratic_probe(int index, int attempt);
};