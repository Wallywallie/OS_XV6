
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {

    //case: error
    if (argc < 2) {
        fprintf(2, "Sleep: please input time\n");
        exit(1);
    }

    //handle the input arguments
    if (argc >= 2) {
        int i = atoi(argv[2]);
        sleep(i); 
    }

    exit(0);

}