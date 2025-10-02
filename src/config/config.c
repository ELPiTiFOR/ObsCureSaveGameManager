#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "pointers.h"
#include "my_string.h"

char ***config = NULL;
char config_str[1024];

char **equal_from_line(char *str, size_t *index_ptr)
{
    char **couple = calloc(3, sizeof(char *));
    if (!couple)
    {
        //log(ERROR, "Couldn't calloc() the couple\n");
        return NULL;
    }
    couple[2] = NULL;

    size_t index = 0;
    char *key = duplicate_string_until(str, '=', &index);
    if (!key)
    {
        free_argv(couple);
        return NULL;
    }
    couple[0] = key;

    index++;
    char *value = duplicate_string_until(str + index, '\n', &index);
    if (!value)
    {
        free_argv(couple);
        return NULL;
    }
    couple[1] = value;
    *index_ptr += index + 1;

    return couple;
}

char ***config_from_str(char *str)
{
    size_t nb_couples = 0;
    size_t index = 0;
    char ***config = calloc(1, sizeof(char **));
    if (!config)
    {
        return NULL;
    }
    config[0] = NULL;

    do
    {
        nb_couples += 1;

        char ***config2 = realloc(config, sizeof(char **) * (nb_couples + 1));
        if (!config2)
        {
            free_triple_char_pointer(config);
            return NULL;
        }
        config = config2;
        config[nb_couples] = NULL;

        char **couple = equal_from_line(str + index, &index);
        if (!couple)
        {
            /*
            char buf_log[512];
            sprintf(buf_log, "Couldn't equal_from_line(%s)\n", str);
            log(ERROR, buf_log);
            */
            free_triple_char_pointer(config);
            return NULL;
        }

        config[nb_couples - 1] = couple;
    } while (str[index] && nb_couples < 10);

    return config;
}

void setup_config(void)
{
    size_t len = 0;
    char *config_str = str_from_file("OCSGM.config", &len);
    if (!config_str)
    {
        // let's create the file
        file_from_string("OCSGM.config", "path_saves=C:\\\npath_backups=C:\\\n", 32);
        config_str = str_from_file("OCSGM.config", &len);
    }
    config = config_from_str(config_str);
    free(config_str);
}

char *config_get(char *key)
{
    if (!config)
    {
        setup_config();
    }

    for (int i = 0; config[i]; i++)
    {
        if (strcmp(config[i][0], key) == 0)
        {
            return config[i][1];
        }
    }

    return NULL;
}

void config_to_str(char ***config)
{
    memset(config_str, 0, 1024);
    char *p = config_str;
    for (size_t i = 0; config[i]; i++)
    {
        int r = sprintf(p, "%s=%s\n", config[i][0], config[i][1]);
        p += r;
    }
}

int overwrite_config_file(char ***config)
{
    config_to_str(config);
    return file_from_string("OCSGM.config", config_str, strlen(config_str));
}

int config_set(char *key, char *value)
{
    if (!config)
    {
        setup_config();
    }

    for (int i = 0; config[i]; i++)
    {
        if (strcmp(config[i][0], key) == 0)
        {
            char *new_value = malloc(strlen(value));
            if (!new_value)
            {
                return 1;
            }

            strcpy(new_value, value);
            free(config[i][1]);
            config[i][1] = new_value;

            if (overwrite_config_file(config))
            {
                free_triple_char_pointer(config);
                config = NULL;
                setup_config();
                return 1;
            }
            return 0;
        }
    }

    return 1;
}

void free_config(void)
{
    if (config)
    {
        free_triple_char_pointer(config);
    }
}