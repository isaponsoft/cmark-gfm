#include <iostream>
#include "../extensions/cmarklib.h"


char const markdown[] = R"(
# Title

```shell
> ls
```

|a|b|
|--|--|
|c|d|

)";


static void procnode(cmark_node* n, bool entering);
static std::string decode(cmark_chunk const& c);
static std::string tostring(cmark_chunk const& c);


int main(int, char**)
{
	cmark_parser*	parser	= cmark_parser_ex_new(CMARK_OPT_TABLE_PREFER_STYLE_ATTRIBUTES);

	// parse
	cmark_parser_feed(parser, markdown, strlen(markdown));
	auto				doc		= cmark_parser_finish(parser);
	cmark_iter*			iter	= cmark_iter_new(doc);
	cmark_event_type	ev;
	while ((ev = cmark_iter_next(iter)) != CMARK_EVENT_DONE)
	{
		cmark_node*		node	= cmark_iter_get_node(iter);
		cmark_mem*		mem		= cmark_get_default_mem_allocator();
		bool			enter	= (ev == CMARK_EVENT_ENTER);

		procnode(node, enter);
	}
	cmark_iter_free(iter);
	cmark_node_free(doc);

	cmark_parser_free(parser);

	return	0;
}



void procnode(cmark_node* n, bool entering)
{
	cmark_mem*		mem		= cmark_get_default_mem_allocator();
	switch (n->type)
	{
		case CMARK_NODE_BLOCK_QUOTE :
		{
			if (entering)
					std::cout << "<blockquote>\n";
			else	std::cout << "</blockquote>\n";
			break;
		}

		case CMARK_NODE_LIST :
		{
			if ((cmark_list_type)n->as.list.list_type == CMARK_BULLET_LIST)
			{
				if (entering)
						std::cout << "<ul>\n";
				else	std::cout << "</ul>\n";
			}
			else
			{
				if (entering)
						std::cout << "<ol start=\"" << n->as.list.start << "\">\n";
				else	std::cout << "</ol>\n";
			}
			break;
		}
		
		case CMARK_NODE_ITEM :
		{
			if (entering)
					std::cout << "<li>\n";
			else	std::cout << "</li>\n";
			break;
		}
		
		case CMARK_NODE_HEADING :
		{
			if (entering)
					std::cout << "<h" << n->as.heading.level << ">";
			else	std::cout << "</h" << n->as.heading.level << ">\n";
			break;
		}

		case CMARK_NODE_CODE_BLOCK :
		{
			std::cout << "<pre><code language=\"" << decode(n->as.code.info) << "\">\n";
			std::cout << decode(n->as.code.literal);
			std::cout << "</code></pre>\n";
			break;
		}

		case CMARK_NODE_HTML_BLOCK :
		{
			std::cout << decode(n->as.literal);
			break;
		}

		case CMARK_NODE_CUSTOM_BLOCK :
		{
			if (entering)
			{
				std::cout << decode(n->as.custom.on_enter);
			}
			else
			{
				std::cout << decode(n->as.custom.on_exit);
			}
			break;
		}
		
		case CMARK_NODE_THEMATIC_BREAK :
		{
			std::cout << "<hr />\n";
			break;
		}

		case CMARK_NODE_PARAGRAPH :
		{
			cmark_node*	grandparent	= cmark_node_parent(cmark_node_parent(n));
			if (!(grandparent && grandparent->type == CMARK_NODE_LIST && grandparent->as.list.tight))
			{
				if (entering)
						std::cout << "<p>\n";
				else	std::cout << "</p>\n";
			}
			break;
		}
		
		case CMARK_NODE_TEXT :
		{
			std::cout << decode(n->as.literal);
			break;
		}

		case CMARK_NODE_LINEBREAK :
		{
			std::cout << "<br />\n";
			break;
		}

		case CMARK_NODE_SOFTBREAK :
		{
			std::cout << "<br />\n";
			break;
		}

		case CMARK_NODE_CODE :
		{
			std::cout << "<code>" << decode(n->as.literal) << "</code>\n";
			break;
		}

		case CMARK_NODE_HTML_INLINE :
		{
			std::cout << decode(n->as.literal);
			break;
		}
		
		case CMARK_NODE_CUSTOM_INLINE :
		{
			std::cout << decode(entering ? n->as.custom.on_enter : n->as.custom.on_exit);
			break;
		}
		
		case CMARK_NODE_STRONG :
		{
			if (entering)
					std::cout << "<strong>\n";
			else	std::cout << "</strong>\n";
			break;
		}
		
		case CMARK_NODE_EMPH :
		{
			if (entering)
					std::cout << "<em>\n";
			else	std::cout << "</em>\n";
			break;
		}

		case CMARK_NODE_LINK :
		{
			if (entering)
			{
				std::cout << "<a";
				if (n->as.link.url.len)
				{
					std::cout << " href=\"" << tostring(n->as.link.url) << "\"";
				}
				if (n->as.link.title.len)
				{
					std::cout << " title=\"" << tostring(n->as.link.title) << "\"";
				}
			}
			else
			{
				std::cout << "</a>\n";
			}
			break;
		}

		case CMARK_NODE_IMAGE :
		{
			if (entering)
			{
				std::cout << "<img src=\"";
				if (n->as.link.url.len)
				{
					std::cout << tostring(n->as.link.url);
				}
				std::cout << "\" alt=\"";
			}
			else
			{
				if (n->as.link.title.len)
				{
					std::cout << "\" title=\"" << decode(n->as.link.title);
				}
				std::cout << "\" />";
			}
			break;
		}

		case CMARK_NODE_TABLE :
		{
			if (entering)
			{
				std::cout << "<table>\n";
			}
			else
			{
				std::cout << "</table>\n";
			}
			break;
		}

		case CMARK_NODE_TABLE_ROW :
		{
			if (entering)
			{
				if (((cmark_table_row*)n->as.opaque)->is_header)
				{
					std::cout << "<thead>\n";
				}
				std::cout << "<tr>\n";
			}
			else
			{
				std::cout << "</tr>\n";
				if (((cmark_table_row*)n->as.opaque)->is_header)
				{
					std::cout << "</thead>\n";
				}
			}
			break;
		}

		case CMARK_NODE_TABLE_CELL :
		{
			uint8_t*	alignments	= ((cmark_table*) n->parent->parent->as.opaque)->alignments;
			if (entering)
			{
				std::cout << "<td>";
			}
			else
			{
				std::cout << "</td>\n";
			}
			break;
		}

		default :
		{
			break;
		}
	}
}


std::string decode(cmark_chunk const& c)
{
	cmark_strbuf	buff;
	cmark_strbuf_init(cmark_get_default_mem_allocator(), &buff, 0);
	houdini_escape_html0(&buff, (uint8_t const*)c.data, c.len, 0);
	std::string	r((char const*)buff.ptr, buff.size);
	cmark_strbuf_free(&buff);
	return	r;
}


std::string tostring(cmark_chunk const& c)
{
	std::string	r((char const*)c.data, c.len);
	return	r;
}
