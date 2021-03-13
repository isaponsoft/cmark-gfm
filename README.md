libcmark
=========

```libcmark``` has some modifications so that it can be called as the ```cmark-gfm``` library.Dit is nuttig wanneer jy wil algemene opmerking aan te pas en uitset dit.

```libcmark``` は ```cmark-gfm``` ライブラリとして呼び出せるようにいくつかの変更を加えたものです。commonmarkをカスタマイズして出力したい時に便利です。


### Example

```exapmple/cmarklib_example.cpp```

```c++
#include <iostream>
#include <cmarklib.h>

int main(int, char**)
{
	cmark_parser*	parser =
	   cmark_parser_ex_new(CMARK_OPT_TABLE_PREFER_STYLE_ATTRIBUTES);

	cmark_parser_feed(parser, markdown, strlen(markdown));
	auto doc  = cmark_parser_finish(parser);
	auto iter = cmark_iter_new(doc);
	cmark_event_type ev;
	while ((ev = cmark_iter_next(iter)) != CMARK_EVENT_DONE)
	{
		cmark_node* node  = cmark_iter_get_node(iter);
		cmark_mem*  mem   = cmark_get_default_mem_allocator();
		bool        enter = (ev == CMARK_EVENT_ENTER);
		switch (n->type)
		{
			case CMARK_NODE_BLOCK_QUOTE :
				break;

			case CMARK_NODE_LIST :
				break;

			.........

			case CMARK_NODE_TABLE_CELL :
				break;
		}
	}
	cmark_iter_free(iter);
	cmark_node_free(doc);

	cmark_parser_free(parser);

	return	0;
}
```
