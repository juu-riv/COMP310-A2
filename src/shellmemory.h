#define MEM_SIZE 1000
#define LINE_SIZE 256
void mem_init();
char *mem_get_value(char *var);
void mem_set_value(char *var, char *value);
void mem_alloc(FILE *script);
int pid_get_value();
