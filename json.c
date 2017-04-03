#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

/* 
	builds string of json object in the form {"key1":"string","key2":literal,...}
 	into string buffer. returns number of characters written to buffer.
 	note: only supports making json strings and json literals (number, true, false, or null)
	** this method is error prone as the correct number of json objecs (n) must
	** be passed in. use JSON_MAKE macro whenever possible to minimize these errors

	a json object can be easily initialized in the following manner:
	json_object json[] = {
		{"key0", JSON_STRING("value")},
		{"key1", JSTON_NUMBER("1")},
		{"key2", JSON_TRUE},
		{"key3", JSON_FALSE},
		{"key4", JSON_NULL},
	};
	char buffer[JSON_SIZE(json)];
	int n_chars = JSON_MAKE(json, buffer);

	a json array can be easily initialized like so:
	json_element array[] = {
		{JSON_STRING("array_value")},
		{JSON_NUMBER("2"},
		...
	}
	char array_buffer[JSON_ARRAY_SIZE(array)];
	int array_chars = JSON_MAKE_ARRAY(array, array_buffer);
	
	The array can then be nested in another object in this manner:
	json_object json_nest[] = {
		{"array_key", JSON_ARRAY(array_buffer)},
		...
	}

	Similarily, an object can be nested using JSON_OBJECT(json_object_str).
*/
int make_json_object(int n, void *objects, char *buffer, int obj_type) {
    if (n <= 0 || !objects || !buffer) return 0;
	char opens = '{';
	char ends  = '}';
	int adjust = 0;
	if (obj_type == JSON_ARR) {
		opens = '[';
		ends = ']';
		adjust = JSON_ARR;
	}
    buffer[0] = opens;
    buffer[1] = 0;
    int n_chars = 2; // +1 for \0
    int i;
    for (i=0; i < n; i++) {
		const char *key = ((json_object *) objects)[i].key;
		const char *val = ((json_object *) objects)[i].value;
		int type = ((json_object *) objects)[i].type;
		char *str_format = "%s\"%s\":\"%s\"%c";
		char *lit_format = "%s\"%s\":%s%c";
		if (obj_type == JSON_ARR) {
			key = "";
			val = ((json_element *) objects)[i].value;
			type = ((json_element *) objects)[i].type;
			str_format = "%s%s\"%s\"%c"; // key will be blank
			lit_format = "%s%s%s%c"; // key will be blank
		}
        int obj_overhead = strlen(key) + strlen(val);
		char end = ',';
		if (i == (n-1)) {
	  		end = 0; // JSON spec does not permit trailing comma
	  		obj_overhead -= 1; // prevent double null termination
		}
        if (type == JSON_STR) { // value is quoted
	    	n_chars = snprintf(buffer, n_chars + obj_overhead + JSON_STR - adjust, str_format, buffer, key, val, end) + 1; // +1 for \0
	    	if (n_chars < 0)
				return n_chars;
        }
        else if (type == JSON_LITERAL) { // value is not quoted
	    	n_chars = snprintf(buffer, n_chars + obj_overhead + JSON_LITERAL - adjust, lit_format, buffer, key, val, end) + 1; // +1 for \0
	    if (n_chars < 0)
			return n_chars;
        }
    }
    n_chars = snprintf(buffer, n_chars + 1, "%s%c", buffer, ends); // +1 for } or ]
    return n_chars;
}

// returns size of json object, in bytes, including overhead
// ** this method is error prone as the correct number of json objecs (n) must
// ** be passed in. use JSON_SIZE macro whenever possible to minimize these errors
int json_size(int n, void *objects, int obj_type) {
	if (n <= 0 || !objects) return 0;
    int json_size = 2; // for {} or []
    int i;
    for (i=0; i < n; i++) {
        int obj_overhead = 0;
		switch (obj_type) {

			case JSON_OBJ:
	        	if (((json_object *) objects)[i].type == JSON_STR) 
    	        	obj_overhead = JSON_STR; // 6 for "":"",
	    	    else if (((json_object *) objects)[i].type == JSON_LITERAL) 
           			obj_overhead = JSON_LITERAL; // 4 for "":,
				json_size += obj_overhead + strlen(((json_object *) objects)[i].key) + strlen(((json_object *) objects)[i].value);
				break;

			case JSON_ARR:
				if (((json_element *) objects)[i].type == JSON_STR)
                	obj_overhead = JSON_STR - JSON_ARR; // 3 for "",
	            else if (((json_element *) objects)[i].type == JSON_LITERAL)
    	            obj_overhead = JSON_LITERAL - JSON_ARR; // 1 for ,
				json_size += obj_overhead + strlen(((json_element *) objects)[i].value);
				break;
		}
    }
    return json_size-1; // -1 for removal of final ,
}

#ifndef TEST
int main(void) {
    // initializing json objects in this way minimizes size errors
    json_object json[] = {
		    {"opening", JSON_STRING("A long time ago in a galaxy far away...")},
		    {"paragraphs", JSON_NUMBER("3")},
    };
    char buffer[JSON_SIZE(json)];
    int n_chars = JSON_MAKE(json, buffer);
    printf("%s\n", buffer);
    return 0;
}
#endif
