#include <stdio.h>
#include <stdlib.h>
#include "config.h"

static int count[BUFFER_SIZE];

struct char_text {
    char data; 
    int present; 
};

int process_text(char *filename, char **data, float **freq, int **present);
