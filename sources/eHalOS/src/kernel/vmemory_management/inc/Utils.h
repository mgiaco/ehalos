#ifndef UTILS_H_
#define UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief identifies the page size of a page or frame.
 * This has effects on the Page Table Size and so on.
 */
#define	PAGESIZE_64KB

#define TRUE 1
#define FALSE 0

#include "stdlib.h"
#include <inttypes.h>

typedef uint8_t BOOL;

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*UTILS_H_*/
