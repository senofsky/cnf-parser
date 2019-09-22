#include <stdio.h>

#include "cnf.h"

int main (void) {

    int rc = 0;
    char *filename = "./sat_probs/small.cnf";
    
    struct cnf cnf = {0};

    rc = parse_cnf_file(filename, &cnf);
    printf("rc = %d\n", rc);

    print_cnf(&cnf);
    
    return 0;
}
