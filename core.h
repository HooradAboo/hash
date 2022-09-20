#ifndef UTILITY_H
#define UTILITY_H

// int quit (char** args);
// int head (char** args);
// int sort (char** args);

char* func_name[] = {
                    //  "quit", 
                    //  "head",
                    //  "sort"
                    };

int (*functions[]) (char **) = {
                                    // &quit,
                                    // &head,
                                    // &sort
                                };
                                
int num_funcitons () {
    return sizeof(func_name) / sizeof(char*);
}

#endif