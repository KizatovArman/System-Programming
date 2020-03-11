#include <stdio.h>
#include <stdlib.h>
#include <string.h>      
#include <stdarg.h>       

#define programPreString "programSIS>"
#define TRUE 1
#define FALSE 0

//sudo iptables -A OUTPUT -p tcp --dport 443 -d 157.240.0.0/16 -j DROP to unblock
//sudo iptables -A OUTPUT -d 157.240.0.0/16 -j DROP to block
//157.240.0.0/16 - FACEBOOK
//64.233.160.0/19 - YOUTUBE(NOT WELL WORKING)
//140.82.112.0/20 - GITHUB(???)

char* concat(int count, ...)
{
    va_list ap;
    int i;
    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);
    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;
    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);
    return merged;
}

//sudo iptables -A OUTPUT -p tcp --dport 443 -d 157.240.0.0/16 -j DROP
void performTask(char *block, char* input, char* ip, char* port_ip) {
    char *iptables_command = "sudo iptables ";
    iptables_command = concat(4, iptables_command, block, input, "-p tcp --dport "); // "sudo iptables -A OUTPUT -p tcp --dport "
    iptables_command = concat(2, iptables_command, port_ip);
    iptables_command = concat(4, iptables_command, " -d ", ip, " -j DROP");
    // printf("%s",iptables_command);
    system(iptables_command);
    system("sudo iptables -L -n --line-number >> temp.txt");
}

void printPreString(){
    printf("%s", programPreString);
}

int main (int argc, char *argv[]) {
    int option1 = FALSE;
    int option2 = FALSE;
    int option3 = FALSE;
    char *line = NULL;  /* forces getline to allocate with malloc */
    char *temp_line = NULL;
    char *block_option = "";
    char *input_option = "";
    char *ip_port_value = "";
    char *ip_port_port = "";
    size_t len = 0;     /* ignored when line = NULL */
    ssize_t read;

    printf("\n\t\tHello!\n\t\tThis is my System Programming Course SIS Task!\n");
    printf("\t\tMy task is: Firewall where we can add or delete blocking rules\n\t\tto incoming or out packets specified by their\n\t\tdestination and source port number or ip address.\n\n\n");
    printf("\t\tBasically, I just wait for your commands,\n\t\tgenerate string according to your instructions and run command\n\t\tusing system() function available in C.\n");
    printf("\t\tAs a first command you have options: BLOCK or UNBLOCK.\n\n\n");
    printf("\t\tIf you want to quit you must press CONTROL + C/CONTROL + D.\n");
    printf("\t\tJust type BLOCK/UNBLOCK and follow further instructions!\n");
    printf("\t\tGood luck!\n\n\n");
    printPreString();
 
    while ((read = getline(&line, &len, stdin)) != -1) {
        temp_line = line;
        if(strcmp(temp_line, "BLOCK\n") == 0){
            option1 = TRUE;
            block_option = concat(2, block_option, "-A ");
            printf("%sNow choose option from OUTPUT or INPUT.\n", programPreString);
            printf("%s", programPreString);
        }
        else if(strcmp(temp_line, "OUTPUT\n") == 0){
            option2 = TRUE;
            input_option = concat(2, input_option, "OUTPUT ");
            printf("%sOk. Write IP-ADDRESS. Ex: '157.240.0.0/16'.\n", programPreString);
            printf("%s", programPreString);  
        }
        else if(strcmp(temp_line, "UNBLOCK\n") == 0) {
            option1 = TRUE;
            block_option = concat(2, block_option, "-D ");
            printf("%sNow choose option from OUTPUT or INPUT.\n", programPreString);
            printf("%s", programPreString);
        }
        else if(strcmp(temp_line, "INPUT\n") == 0){
            option2 = TRUE;
            input_option = concat(2, input_option, "INPUT ");
            printf("%sOk. Write IP-ADDRESS. Ex: '157.240.0.0/16'\n", programPreString);
            printf("%s", programPreString);  
        }
        else if(option1 == TRUE && option2 == TRUE && option3 == FALSE) {
            option3 = TRUE;
            ip_port_value = concat(2, ip_port_value, line);
            ip_port_value[strlen(ip_port_value)-1] = '\0';
            printPreString();
            printf("Finally, write port that needs to be bclocked. Ex: '443'. PS: Strongly recommend you to block 443 port.\n");
            printPreString();
        }
        else if(option1 == TRUE && option2 == TRUE && option3 == TRUE) {
            ip_port_port = concat(2, ip_port_port, line);
            ip_port_port[strlen(ip_port_port)-1] = '\0';
            printPreString();
            performTask(block_option, input_option, ip_port_value, ip_port_port);
            option1 = FALSE;
            option2 = FALSE;
            option3 = FALSE;
            ip_port_port = "";
            ip_port_value = "";
            block_option = "";
            input_option = "";
            block_option = "";
        }
    }
    free (line);
    free(temp_line);
    free(block_option);
    free(ip_port_value);
    free(input_option);
    free(ip_port_port);
    return 0;
}
