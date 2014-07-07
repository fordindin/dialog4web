#include "dialog4web.h"

#include <wchar.h>
static StringList *enable_items = NULL;
static StringList *new_items = NULL;

static bool
is_enable(const char *name)
{
	return (sl_find(enable_items, (char *)name) != NULL);
}

static bool
is_new(const char *name)
{
	return (sl_find(new_items, (char *)name) != NULL);
}


const char *
type_map(const char *input){
		if (strcmp(input, "GROUP") == 0)
				return "checkbox";
		if (strcmp(input, "MULTI") == 0)
				return "checkbox";
		if (strcmp(input, "SINGLE") == 0)
				return "radio";
		if (strcmp(input, "RADIO") == 0)
				return "radio";
		if (strcmp(input, "BUTTON") == 0)
				return "button";
		return "button";
}

static StringList *
parse_env_sl(char const *env_name)
{
	StringList *sl;
	char *env;
	char *temp;
	char *token;

	sl = sl_init();
	env = getenv(env_name);
	if (env != NULL) {
		temp = strdup(env);
		while ((token = strsep(&temp, " \t")) != NULL) {
			if (token[0] == '\0')
				continue;
			sl_add(sl, strdup(token));
		}
		free(temp);
	}
	return (sl);
}

static char const *
get_desc(char const *res, char const *fallback)
{
  char buf[256];
  char *desc;

  if (snprintf(buf, sizeof(buf), "%s_DESC", res) >= (int)sizeof(buf))
    warnx("Description for %s has been truncated", res);

  desc = getenv(buf);
  if (desc == NULL || desc[0] == '\0')
    return (fallback);

  return (desc);
}

int get_items(ItemList *items, StringList *itemnames, char *type, char *groupname){
		const char *desc, *name;
		int sl_len, nitems;
		nitems = 0;
		sl_len = itemnames->sl_cur;
		if ( strncmp(type, "SINGLE", strlen("SINGLE") ) != 0)
				*items = (ItemList) calloc (sl_len, sizeof(InputItem));
		else
				*items = (ItemList) calloc (sl_len + 1, sizeof(InputItem));

		for (int i=0; i < sl_len; i++){
				name = strdup(itemnames->sl_str[i]);
				desc = get_desc(itemnames->sl_str[i], "");
				(*items)[i] = (InputItem) {
						.name = name,
						.desc = desc,
						.type = type_map(type),
						.id = name,
						.value = name,
						.isnew = is_new(name),
						.group = groupname,
						.checked = is_enable(name)
				};
				nitems++;
		}
		if ( strncmp(type, "SINGLE", strlen("SINGLE") ) == 0){
				(*items)[nitems] = (InputItem) {
						.name = "None",
						.desc = "None of them",
						.type = type_map(type),
						.id = "None",
						.value = "None",
						.isnew = false,
						.group = groupname,
						.checked = false
				};
				nitems++;
		}
		return nitems;
		
}

ItemGroup *
env_get_group(char *name, char *type){
		char buf[256];
		const char *desc;
		StringList *itemnames = NULL;
		ItemGroup *itemgroup;
		ItemList items;
		int nitems;
		itemgroup = (ItemGroup *)malloc(sizeof(ItemGroup));

		/* there is special case for ALL_OPTIONS: it is a checkbox group without
		 * description and with non-standart option list 
		 */
		if (strcmp(name, "ALL") == 0){
				sprintf(buf,"ALL_OPTIONS");
				desc = "All options";
		}
		else {
				if (snprintf(buf, sizeof(buf), "OPTIONS_%s_%s", type, name) >= (int)sizeof(buf))
						warnx("group list %s has been truncated", name);
				desc = get_desc(name, "");
		}
		itemnames = parse_env_sl(buf);
		nitems = get_items(&items, itemnames, type, name);
		*itemgroup = (ItemGroup){
				.name = name,
				.desc = desc,
				.items = (ItemList)items,
				.nitems = nitems
		};
		return (itemgroup);
}

ItemGroupList
get_group_list(){
	enable_items = parse_env_sl("PORT_OPTIONS");
	new_items = parse_env_sl("NEW_OPTIONS");
	StringList *tmp_sl = NULL;
	ItemGroups *grouplist = NULL;
	int groupcount;
	char buf[256];
	static const int ngroups = 5;
	char *typenames[ngroups] = {
			"ALL",
			"RADIO",
			"SINGLE",
			"GROUP",
			"MULTI"
	};

	grouplist = (ItemGroups *) malloc (sizeof(ItemGroups));
	groupcount = 0;

	for (int i=0; i < ngroups; i++)
			if (strncmp(typenames[i], "ALL", strlen("ALL")) == 0){
					grouplist = (ItemGroups *) realloc (grouplist, sizeof(ItemGroups)*groupcount+1);
					grouplist[groupcount] = env_get_group("ALL", "MULTI");
					groupcount++;
			} else {
					if (snprintf(buf, sizeof(buf), "OPTIONS_%s", typenames[i]) >= (int)sizeof(buf))
						warnx("Parameters for %s have been truncated", typenames[i]);
					tmp_sl = parse_env_sl(buf);
					for (int j=0; j < tmp_sl->sl_cur; j++){
							grouplist = (ItemGroups *) realloc (grouplist, sizeof(ItemGroups)*groupcount+1);
							grouplist[groupcount] = env_get_group(tmp_sl->sl_str[j], typenames[i]);
							groupcount++;
					}
			}
	return (ItemGroupList) {
			.nitems = groupcount,
			.groups = grouplist
	};
}

/*
int main(){
	ItemGroupList test;
	
	test = get_group_list();
	for (int i=0; i < test.nitems; i++){
			printf ("%d\n", test.groups[i]->nitems);
			for (int j=0; j < test.groups[i]->nitems; j++){
					printf ("%d\n", test.groups[i]->nitems);
					printf("\t%s\n", test.groups[i]->items[j].name);
			}
	}
}
*/
