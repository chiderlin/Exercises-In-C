#include "rot18.h"
#define MAX_LENGTH 100

enum encryptType { lower, upper, digit };
typedef enum encryptType encryptType;
int encrypt(encryptType type, char word);

void rot(char str[]){
    // A -> N, a -> n. B -> O, b -> o.
    // 0 -> 5, 1 -> 6, 2->7, 3->8, 4->9, 5->0
    // aren't letters or digits must be unaffected.
    // and ecrypted second time, it reverts to the orignal string.
    int len = strlen(str);
    for(int i=0; i<=len; ++i){
        char eachWord = str[i];
        int isAlpha = isalpha(eachWord);
        int isDigit = isdigit(eachWord);
        if(isAlpha){
            int isLower = islower(eachWord);
            int isUpper = isupper(eachWord);
            if(isLower){
                str[i] = encrypt(lower, eachWord);
            }
            if(isUpper){
                str[i] = encrypt(upper, eachWord);
            }
            
        } else if (isDigit){
            int tmpInt = encrypt(digit, eachWord);
            char intToStr = tmpInt + '0';
            str[i] = intToStr;
        }else {
            str[i] = eachWord;
        }
    }
}

int encrypt(encryptType type, char word){
    unsigned int transStr;
    unsigned int transDigit = word - '0';
    transStr = word + 13;
    unsigned int defaultDigitEncrypt = transDigit + 5;
    switch(type){
        case digit:
            if(defaultDigitEncrypt >= 10){
                transDigit = transDigit - 5;
                transStr = transDigit;
            } else {
                transStr = defaultDigitEncrypt;
            }
            break;
        case lower:
            if (transStr > 'z') {
                transStr = word - 13;
            }
            break;
        case upper:
            if (transStr > 'Z') {
                transStr = word - 13;
            }
            break;
        default:
            printf("this type not exists: %d",type);
    }
    return transStr;
}

// any other functions here

void your_tests(void){
    // your tests here
    char str[MAX_LENGTH];
    strcpy(str, "5566");
    rot(str);
    assert(strcmp(str, "0011") == 0);
    strcpy(str, "How are you?");
    rot(str);
    assert(strcmp(str, "Ubj ner lbh?") == 0);
    rot(str);
    assert(strcmp(str, "How are you?") == 0);
    strcpy(str, "*-12345+");
    rot(str);
    assert(strcmp(str, "*-67890+") == 0);
    rot(str);
    assert(strcmp(str, "*-12345+") == 0);

}
