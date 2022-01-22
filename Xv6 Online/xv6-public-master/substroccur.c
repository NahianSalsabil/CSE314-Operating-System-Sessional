#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    
    char* str1 = argv[1];
    char* str2 = argv[2];
   
    printf(1, "%d\n", substroccur(str1,str2));
    exit();
}