#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "cmark_ctype.h"
#include "syntax_extension.h"
#include "config.h"
#include "parser.h"
#include "cmark-gfm.h"
#include "node.h"
#include "references.h"
#include "utf8.h"
#include "scanners.h"
#include "inlines.h"
#include "houdini.h"
#include "buffer.h"
#include "footnotes.h"
#include "../extensions/cmark-gfm-core-extensions.h"
#include "registry.h"

#include "cmarklib.h"


cmark_parser *cmark_parser_ex_new_with_mem(int options, cmark_mem *mem)
{
	cmark_llist*	ext;
	cmark_parser*	parser;

	cmark_gfm_core_extensions_ensure_registered();
  
	parser	= cmark_parser_new_with_mem(options, mem);
	if (!parser)
	{
		return	NULL;
	}

	ext		= cmark_list_syntax_extensions(mem);
	if (!ext)
	{
		cmark_parser_free(parser);
		return	NULL;
	}

	for (cmark_llist* tmp = ext; tmp; tmp = tmp->next)
	{
		cmark_syntax_extension*	e = (cmark_syntax_extension*)tmp->data;
		cmark_parser_attach_syntax_extension(parser, cmark_find_syntax_extension(e->name));
	}
	cmark_llist_free(mem, ext);
	return	parser;
}


cmark_parser *cmark_parser_ex_new(int options)
{
	return	cmark_parser_ex_new_with_mem(options, cmark_get_default_mem_allocator());
}
