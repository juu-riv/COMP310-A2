#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shellmemory.h"
#include "pcb.h"
#include "queue.h"

struct memory_struct {
    char *var;
    char *value;
};

struct memory_struct shellmemory[MEM_SIZE];

struct load_struct {
    char *lines[MEM_SIZE];  // lines in memory
    int count;              // line count in memory
};

struct load_struct loadmemory;

// Helper functions
int match(char *model, char *var) {
    int i, len = strlen(var), matchCount = 0;
    for (i = 0; i < len; i++) {
        if (model[i] == var[i])
            matchCount++;
    }
    if (matchCount == len) {
        return 1;
    } else
        return 0;
}

// Shell memory functions

void mem_init() {
    int i;
    for (i = 0; i < MEM_SIZE; i++) {
        shellmemory[i].var = "none";
        shellmemory[i].value = "none";
    }
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
    int i;

    for (i = 0; i < MEM_SIZE; i++) {
        if (strcmp(shellmemory[i].var, var_in) == 0) {
            shellmemory[i].value = strdup(value_in);
            return;
        }
    }

    //Value does not exist, need to find a free spot.
    for (i = 0; i < MEM_SIZE; i++) {
        if (strcmp(shellmemory[i].var, "none") == 0) {
            shellmemory[i].var = strdup(var_in);
            shellmemory[i].value = strdup(value_in);
            return;
        }
    }

    return;
}

//get value based on input key
char *mem_get_value(char *var_in) {
    int i;

    for (i = 0; i < MEM_SIZE; i++) {
        if (strcmp(shellmemory[i].var, var_in) == 0) {
            return strdup(shellmemory[i].value);
        }
    }
    return NULL;
}

struct PCB_struct *mem_alloc(FILE *script) {
    struct PCB_struct *pcb = PCB_init(loadmemory.count);

    char buf[LINE_SIZE];
    while (loadmemory.count < MEM_SIZE && fgets(buf, LINE_SIZE, script) != NULL) {
        loadmemory.lines[loadmemory.count] = strdup(buf);
        loadmemory.count++;
    }

    pcb->length = loadmemory.count - pcb->start;
    return pcb;
}

char *mem_get_from(int pc) {
    return loadmemory.lines[pc];
}

void mem_free(struct PCB_struct *pcb) {
    for (int i = pcb->start; i < pcb->start + pcb->length; i++) {
        if (loadmemory.lines[i] != NULL) {
            free(loadmemory.lines[i]);
            loadmemory.lines[i] = NULL;
        }
    }
}