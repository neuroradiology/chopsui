#ifndef _SUI_SUI_H
#define _SUI_SUI_H

#include "node.h"
#include "util/errors.h"

sui_node_t *sui_parse(const char *source, errors_t **errs);
sui_node_t *sui_load(FILE *source, errors_t **errs);
void sui_free(sui_node_t *stylesheet);

#endif
