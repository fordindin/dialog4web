#include <stdio.h>

#include "dialog4web.h"

const char *bodytemplate = "<html><head>"
"<style type='text/css'>"
"div.itemgroup {border: 1px solid black; padding: 5px; margin: 5px}"
"div.itemgroupheader {color: blue;}"
"div.new:before {content:'+'; color:red;}"
"div.old:before {content:'\\00a0\\00a0' }"
"</style></head>"
"<body><div class='content'><form action='/input' method='POST'>%s"
"<input type='submit' name='Submit' value='Submit' />"
"<input type='submit' name='Cancel' value='Cancel' />"
"</form></div></body></html>";

const static int bufsize = 262144;

char *
InputItemStr(InputItem *item){
		char *out = malloc(bufsize);
		sprintf(out,"<div class='%s'><input name='%s' type='%s' id='%s' value='%s' %s>%s</input></div>\n",
						item->isnew ? "new" : "old",
						item->group, item->type,
						item->id, item->value,
						item->checked ? "checked" : "", item->desc );
		return (out);
}

char *
GroupListStr(ItemGroupList grouplist){
		char *out = malloc(bufsize);
		for (int i=0; i < grouplist.nitems; i++){
				strncat(out, ItemGroupStr(grouplist.groups[i]), bufsize);
		}
		return (out);
}

char *
ItemGroupStr(ItemGroup *group){
		char *items_str = malloc(bufsize);
		char *out = malloc(bufsize);
		for (int i=0; i < group->nitems; i++){
				strncat(items_str, InputItemStr(&group->items[i]), bufsize);
		}
		sprintf (out, "<div id='%s' class='itemgroup'><div class='itemgroupheader'>%s</div>%s</div>",
						group->name,
						group->desc,
						items_str);
		return (out);
}
