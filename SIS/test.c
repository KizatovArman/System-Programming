#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    FILE * fptr;                    // file holder
    char c;                         // char buffer


    system("sudo iptables -L -n --line-number >> temp.txt");      // call dir and put it's contents in a temp using redirects.
    fptr = fopen("temp.txt", "r");  // open said file for reading.
                                    // oh, and check for fptr being NULL.
    while(1){
        c = fgetc(fptr);
        if(c!= EOF)
            printf("%c", c);        // do what you need to.
        else
            break;                  // exit when you hit the end of the file.
    }
    fclose(fptr);                   // don't call this is fptr is NULL.  
    remove("temp.txt");             // clean up

    getchar();                      // stop so I can see if it worked.
}