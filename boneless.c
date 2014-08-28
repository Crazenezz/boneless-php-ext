#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <php.h>
#include "php_boneless.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

static function_entry boneless_functions[] = {
    PHP_FE(route_parser, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry boneless_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_BONELESS_EXTNAME,
    boneless_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_BONELESS_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_BONELESS
ZEND_GET_MODULE(boneless)
#endif

PHP_FUNCTION(route_parser)
{
    zval *params;
    char *uri, *root, *full_path, *token, *string;
    char *string_cat = NULL, *new_string_cat = NULL;
    int uri_len, root_len;
    struct stat s;
    int is_param = 0;
    
    
    if (zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC, "ss", 
        &uri, &uri_len, &root, &root_len) == FAILURE)
    {
        RETURN_NULL();
    }
    
    ALLOC_INIT_ZVAL(params);
    array_init(params);
    
    string = strdup(uri);
    while ((token = strsep(&string, "/")) != NULL) 
    {
        if (!is_param)
        {
            if (string_cat == NULL)
            {
                string_cat = malloc(strlen(token) + 2);
                strcpy(string_cat, token);
            }
            else
            {
                new_string_cat = realloc(string_cat, 
                    (strlen(string_cat) + strlen(token) + 3));
                string_cat = new_string_cat;
                strcat(string_cat, "/");
                strcat(string_cat, token);
            }
            
            full_path = malloc(strlen(root) + strlen(string_cat) + 6);
            strcpy(full_path, root);
            strcat(full_path, string_cat);

            if (stat(strcat(full_path, ".php"), &s) == 0)
            {
                if(s.st_mode & S_IFREG)
                {   
                    array_init(return_value);

                    add_assoc_stringl(return_value, "controller", 
                        string_cat, strlen(string_cat) + 1, 1);
                    
                    is_param = 1;
                }
            }
        }
        else
        {
            add_next_index_string(params, token, 1);
        }
    }
    
    add_assoc_zval(return_value, "params", params);
}
