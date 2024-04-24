#include "../header/chat.h"

#include "iostream"
#include "string.h"

Chat::Chat() {
    data_count = 0;
    hash_table_size = SIZE;
    for (int i = 0; i < SIZE; ++i) {
        hash_table[i] = nullptr;
    }
}

void Chat::reg(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    if (data_count < SIZE) {
        // Хэшируем пароль
        uint* pass_hash = sha1(_pass, pass_length);

        // Добавляем данные в хеш-таблицу
        int index = hash_function(_login);
        int probe_index = index;
        int attempt = 0;
        while (hash_table[probe_index] != nullptr) {
            attempt++;
            probe_index = quadratic_probe(index, attempt);
            if (attempt > SIZE) {
                std::cout << "Ошибка: не удалось добавить данные в хеш-таблицу!" << std::endl;
                return;
            }
        }
        AuthData* new_data = new AuthData(_login, pass_hash);
        hash_table[probe_index] = new_data;
        data_count++;

        

        // Отладочное сообщение
        // std::cout << "Зарегистрирован новый пользователь: " << _login << std::endl;
        // std::cout << "Хеш пароля: ";
        // for (int i = 0; i < SHA1HASHLENGTHUINTS; ++i) {
        //     std::cout << hash_table[probe_index]->pass_sha1_hash[i] << " ";
        // }
        // std::cout << std::endl;
    } else {
        std::cout << "Ошибка: превышен лимит регистраций!" << std::endl;
    }
}

bool Chat::login(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    // Ищем данные в хеш-таблице
    int index = hash_function(_login);
    int probe_index = index;
    int attempt = 0;
    while (hash_table[probe_index] != nullptr) {
        if (strcmp(hash_table[probe_index]->login, _login) == 0) {
            // Хэшируем введенный пароль
            uint* pass_hash = sha1(_pass, pass_length);

            // Отладочное сообщение
            // std::cout << "Хеш пароля для входа: ";
            // for (int i = 0; i < SHA1HASHLENGTHUINTS; ++i) {
            //     std::cout << pass_hash[i] << " ";
            // }
            // std::cout << std::endl;

            // Сравниваем хэши паролей
            bool match = true;
            for (int j = 0; j < SHA1HASHLENGTHUINTS; ++j) {
                if (*(hash_table[probe_index]->pass_sha1_hash + j) != *(pass_hash + j)) {
                    match = false;
                    break;
                }
            }

            // Очищаем за собой память
            delete[] pass_hash;

            // Если пароль совпадает, возвращаем true
            if (match) {
                std::cout << "Пользователь " << _login << " вошел в систему." << std::endl;
                return true;
            } else {
                std::cout << "Ошибка: неверный пароль!" << std::endl;
                return false;
            }
        }
        attempt++;
        probe_index = quadratic_probe(index, attempt);
        if (attempt > SIZE) {
            std::cout << "Ошибка: пользователь с логином '" << _login << "' не найден!" << std::endl;
            return false;
        }

        
    }

    // Если пользователь не найден, выводим сообщение об ошибке
    std::cout << "Ошибка: пользователь с логином '" << _login << "' не найден!" << std::endl;
    return false;
}

int Chat::hash_function(const char* key) {
    unsigned int hash = 0;
    int c;
    while (c = *key++) {
        hash = hash * 33 + c;
    }
    return hash % SIZE;
}

int Chat::quadratic_probe(int index, int attempt) {
    return (index + attempt * attempt) % SIZE;
}