/*
 * Copyright 2019 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <php.h>
#include <Zend/zend_interfaces.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_clientencryption_ce;

ZEND_BEGIN_ARG_INFO_EX(ai_ClientEncryption_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_clientencryption_me[] = {
	/* clang-format off */
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_ClientEncryption_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_ClientEncryption_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\ClientEncryption object handlers */
static zend_object_handlers php_phongo_handler_clientencryption;

static void php_phongo_clientencryption_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_clientencryption_t* intern = Z_OBJ_CLIENTENCRYPTION(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->client_encryption) {
		mongoc_client_encryption_destroy(intern->client_encryption);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_clientencryption_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_clientencryption_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_clientencryption_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_clientencryption;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_clientencryption_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_clientencryption;

		return retval;
	}
#endif
} /* }}} */

static HashTable* php_phongo_clientencryption_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_clientencryption_t* intern = NULL;
	zval                           retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_CLIENTENCRYPTION_OBJ_P(object);

	array_init(&retval);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_clientencryption_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ClientEncryption", php_phongo_clientencryption_me);
	php_phongo_clientencryption_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_clientencryption_ce->create_object = php_phongo_clientencryption_create_object;
	PHONGO_CE_FINAL(php_phongo_clientencryption_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_clientencryption_ce);

	memcpy(&php_phongo_handler_clientencryption, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_clientencryption.get_debug_info = php_phongo_clientencryption_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_clientencryption.free_obj = php_phongo_clientencryption_free_object;
	php_phongo_handler_clientencryption.offset   = XtOffsetOf(php_phongo_clientencryption_t, std);
#endif
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
