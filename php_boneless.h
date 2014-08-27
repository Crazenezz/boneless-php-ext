#ifndef PHP_BONELESS_H
#define PHP_BONELESS_H 1
#define PHP_BONELESS_VERSION "1.0"
#define PHP_BONELESS_EXTNAME "boneless"

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_FUNCTION(route_parser);

extern zend_module_entry boneless_module_entry;
#define phpext_boneless_ptr &boneless_module_entry

#endif
