/* *INDENT-OFF* */
#ifndef _ENCLAVE_API_H_
#define _ENCLAVE_API_H_
/* *INDENT-ON* */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <enclave-tls/err.h>

#define TLS_TYPE_NAME_SIZE      32
#define QUOTE_TYPE_NAME_SIZE    32
#define ENCLAVE_SGX_SPID_LENGTH 16
#define SHA256_HASH_SIZE        32

typedef enum {
	ENCLAVE_TLS_LOG_LEVEL_DEBUG,
	ENCLAVE_TLS_LOG_LEVEL_INFO,
	ENCLAVE_TLS_LOG_LEVEL_WARN,
	ENCLAVE_TLS_LOG_LEVEL_ERROR,
	ENCLAVE_TLS_LOG_LEVEL_DEFAULT = ENCLAVE_TLS_LOG_LEVEL_ERROR,
	ENCLAVE_TLS_LOG_LEVEL_FATAL,
	ENCLAVE_TLS_LOG_LEVEL_NONE,
	ENCLAVE_TLS_LOG_LEVEL_MAX
} enclave_tls_log_level_t;

typedef struct etls_core_context_t *enclave_tls_handle;

typedef enum {
	ENCLAVE_TLS_CERT_ALGO_RSA_3072_SHA256,
	ENCLAVE_TLS_CERT_ALGO_DEFAULT = ENCLAVE_TLS_CERT_ALGO_RSA_3072_SHA256,
	ENCLAVE_TLS_CERT_ALGO_MAX
} enclave_tls_cert_algo_t;

/* FIXME: add the type of ecdsa verification */
typedef enum {
	VERIFICATION_TYPE_QVL,
	VERIFICATION_TYPE_QEL
} quote_sgx_ecdsa_verification_type_t;

typedef struct {
	unsigned int api_version;
	unsigned long flags;
	int fd;
	enclave_tls_log_level_t log_level;
	char tls_type[TLS_TYPE_NAME_SIZE];
	char attester_type[QUOTE_TYPE_NAME_SIZE];
	char verifier_type[QUOTE_TYPE_NAME_SIZE];
	enclave_tls_cert_algo_t cert_algo;

	/* FIXME: SGX EPID quote type specific parameters */
	struct {
		bool valid;
		uint8_t spid[ENCLAVE_SGX_SPID_LENGTH];
		bool linkable;
	} quote_sgx_epid;

	/* FIXME: SGX ECDSA quote type specific parameters */
	struct {
		bool valid;
		uint8_t cert_type;
		quote_sgx_ecdsa_verification_type_t verification_type;
	} quote_sgx_ecdsa;
} enclave_tls_conf_t;

#define ENCLAVE_TLS_API_VERSION_1                   1
#define ENCLAVE_TLS_API_VERSION_DEFAULT             ENCLAVE_TLS_API_VERSION_1

#define ENCLAVE_TLS_CONF_FLAGS_GLOBAL_MASK_SHIFT    0
#define ENCLAVE_TLS_CONF_FLAGS_PRIVATE_MASK_SHIFT   32

#define ENCLAVE_TLS_CONF_FLAGS_GLOBAL_MASK          (ENCLAVE_TLS_CONF_FLAGS_GLOBAL_MASK << ENCLAVE_TLS_CONF_FLAGS_PRIVATE_MASK_SHIFT

#define ENCLAVE_TLS_CONF_FLAGS_MUTUAL               (1UL << ENCLAVE_TLS_CONF_FLAGS_GLOBAL_MASK_SHIFT)

#define ENCLAVE_TLS_CONF_FLAGS_SERVER               (1UL << ENCLAVE_TLS_CONF_FLAGS_PRIVATE_MASK_SHIFT)

/* *INDENT-OFF* */
enclave_tls_err_t enclave_tls_init(const enclave_tls_conf_t *conf,
				   enclave_tls_handle *handle);
enclave_tls_err_t enclave_tls_negotiate(enclave_tls_handle handle, int fd);
enclave_tls_err_t enclave_tls_receive(enclave_tls_handle handle, void *buf,
				      size_t *buf_size);
enclave_tls_err_t enclave_tls_transmit(enclave_tls_handle handle, void *buf,
				       size_t *buf_size);
enclave_tls_err_t enclave_tls_cleanup(enclave_tls_handle handle);

#endif
/* *INDENT-ON* */
