
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>
#include <microhttpd.h>
#include <stdio.h>
#include <err.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stringlist.h>

#define PORT 8888
#define MIMETYPE "text/html"
#define MAXDOCSIZE 1048576
#define POSTBUFFERSIZE 512

#define ITEM_CHECK      1
#define ITEM_RADIO      2

#define MAXNAMESIZE 20
#define MAXANSWERSIZE 512
#define GET 0
#define POST 1

typedef struct InputItem {
		const char *name;
		const char *desc;
		const char *type;
		const char *id;
		const char *value;
		bool isnew;
		const char *group;
		bool checked;
} InputItem;

typedef InputItem * ItemList;

typedef struct ItemGroup {
		const char *name;
		const char *desc;
		ItemList items;
		int nitems;
} ItemGroup;

typedef ItemGroup * ItemGroups;

typedef struct ItemGroupList {
		int nitems;
		ItemGroups *groups;
} ItemGroupList;


char *InputItemStr(InputItem *item );
char *DocumentBody();
char *GroupListStr(ItemGroupList grouplist);
char *ItemGroupStr(ItemGroup *group);
ItemGroupList get_group_list();

const char *bodytemplate;
