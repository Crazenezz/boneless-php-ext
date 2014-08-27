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
    zval *route;
    char *uri, *root, *full_path, *token, *string, *string_cat = NULL, *new_string_cat = NULL;
    int uri_len, root_len;
    struct stat s;
    
    if (zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC, "ss", 
        &uri, &uri_len, &root, &root_len) == FAILURE)
    {
        RETURN_NULL();
    }
    
    string = strdup(uri);
    while ((token = strsep(&string, "/")) != NULL) {
        full_path = malloc(strlen(root) + strlen(token) + 2);
        
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
        
        strcpy(full_path, root);
        strcat(full_path, string_cat);

        if (stat(full_path, &s) == 0)
        {
            if( s.st_mode & S_IFREG )
            {   
                array_init(return_value);

                add_assoc_stringl(return_value, "file", full_path, strlen(full_path) + 1, 1);
                break;
            }
        }
    }
}
