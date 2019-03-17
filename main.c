#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md4.h>

#define STR_SIZE 100

char * str2md4(char * text);
char * passcheck(const char * passlist, char * hash);

char * passcheck(const char * passlist, char * hash){
    FILE *passtxt = fopen(passlist, "r");
    if(!passtxt){
        printf("Error due to openning '%s'\n", passlist);
        exit(1);
    }
    char password[STR_SIZE];
    char * ntlmpass;
    char * result = malloc(STR_SIZE);
    while(!feof(passtxt)){
        fgets(password, sizeof(password), passtxt);
        char newline = password[strlen(password)-1];
        if(newline == '\n'){
            password[strlen(password)-1] = '\0';
        }
        ntlmpass = str2md4(password);
        //printf("%s\n'%s'\n\n", ntlmpass, password);
        int compresult = strcmp(ntlmpass, hash);
        free(ntlmpass);
        if (compresult == 0){
            sprintf(result, "%s", password);
            return result;
        }
    }
    free(result);
    return NULL;
}

char * str2md4(char * text){
    unsigned char digest[MD4_DIGEST_LENGTH];
//    char md4str[33];
    char * md4str;
    md4str = malloc(33);
    //char text[] = "text";
    MD4((unsigned char*)text, strlen(text), digest);
    
    for(int i = 0; i < 16; i++)
         sprintf(&md4str[i*2], "%02x", (unsigned int)digest[i]);
    for(int i =0; i < 33; i++){
        md4str[i] = toupper(md4str[i]);
    }
    return md4str;

}

int main(int argc, char * argv[]){
    if(argc < 3){
        exit(0);
    }
    FILE *fp = fopen((const char *)argv[1], "r");
    if(!fp){
        printf("Error due to openning '%s'\n", argv[1]);
        exit(1);
    }
    char a[STR_SIZE];
    int dotpos;
    char *str;
    while(!feof(fp)){
        fgets(a, sizeof(a), fp);
        str = a;
        for(int i =0; i < STR_SIZE; i++){
            if(a[i] == ':'){
                dotpos = i;
                break;
            }
        }
        for(int i =0; i < dotpos+1; i++){
            str++;
        }
        if(str[strlen(str)-1] == '\n'){
            str[strlen(str)-1] = '\0';
        }
        char *result;
        result = passcheck(argv[2], str);
        if(result == NULL){
            puts("not found");
        }else{
            printf("%s\n", result);
            free(result);
        }    
    }
    fclose(fp);

    return 0;
}