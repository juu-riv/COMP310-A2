#define MEM_SIZE 1000
#define LINE_SIZE 100
void mem_init();
char *mem_get_value(char *var);
void mem_set_value(char *var, char *value);
struct PCB_struct *mem_alloc(FILE *script);
char *mem_get_from(int pc);
void mem_free(struct PCB_struct *pcb);
