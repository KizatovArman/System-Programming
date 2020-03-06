#include <stdio.h>
#include <stdlib.h>
#include <string.h>      
#include <stdarg.h>       

int main(int argc, char *argv[]) {
    char * str1 = "hello world hello my name is arman!";
    char *str2 = "hello";
    if(strstr(str1, str2) != NULL) {
        printf("conatins");
    } else {
        printf("not contains");
    }
    return 0;
}