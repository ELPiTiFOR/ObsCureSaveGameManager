#ifndef CONFIG_H
#define CONFIG_H

// returns a pointer to a heap allocated string storing the value of the key
// `key`
char *config_get(char *key);

// modifies the value of the key `key` with `value`. 
// returns whether the modification was sucessful or not
int config_set(char *key, char *value);

void free_config(void);

#endif /* CONFIG_H */