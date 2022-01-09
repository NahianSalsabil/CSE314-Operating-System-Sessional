#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    
    int student_id = nahian();
    int size = 7;
    int dummy_stdid = student_id;
    int *nums = malloc(size* sizeof(int));
    for(int i = 0; i < 7; i++){
       nums[i] = student_id%10;
       student_id = student_id/10;
    }
    int sum = add(nums, size);
    int random_number = salsabil(dummy_stdid);
    int remainder = sum % random_number;
   
    printf(1, "student id: %d\n", dummy_stdid);
    printf(1, "sum: %d\n", sum);
    printf(1, "random number: %d\n", random_number);
    printf(1, "remainder: %d\n", remainder);
    
    exit();
}