#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    //struct mystat *ct = malloc(sizeof(struct mystat));
    //ct->sz = argc-1;
    int size = argc - 1;
    int *nums = malloc(size* sizeof(int));
    for(int i = 0; i < size; i++){
        nums[i] = atoi(argv[i+1]);
    }
    printf(1, "%d\n", add(nums, size));
    exit();
}