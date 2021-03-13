#ifndef CMARK_CMARKLIB__H
#define CMARK_CMARKLIB__H

#include "cmark-gfm_export.h"
#include "cmark-gfm_version.h"
#include "cmark-gfm-extensions_export.h"
#include "houdini.h"
#include "node.h"

#ifdef __cplusplus
extern "C" {
#endif

CMARK_GFM_EXTENSIONS_EXPORT
cmark_parser *cmark_parser_ex_new(int options);

CMARK_GFM_EXTENSIONS_EXPORT
cmark_parser *cmark_parser_ex_new_with_mem(int options, cmark_mem *mem);

#ifdef __cplusplus
}
#endif

#endif
