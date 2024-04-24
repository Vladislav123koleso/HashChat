#include "cpp/chat.cpp"
#include "cpp/sha1.cpp"

void test(Chat& c) {
    char login1[LOGINLENGTH] = "user1";
    char pass1[] = "password1";
    c.reg(login1, pass1, strlen(pass1));

    char login2[LOGINLENGTH] = "user2";
    char pass2[] = "password2";
    c.reg(login2, pass2, strlen(pass2));

    c.login(login1, pass1, strlen(pass1));
    c.login(login2, pass2, strlen(pass2));
};


int main()
{
    Chat chat;
    test(chat);

    return 0;
}