#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    
    char* str = argv[1];
    int start_ind = atoi(argv[2]);
    int len = atoi(argv[3]);
   
    printf(1, "%s\n", substr(str, start_ind, len));
    exit();
}