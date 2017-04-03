// Helper macros 
// !WARNING! Do not use to initiliaze JSON objects
// These macros are use to define JSON paramaters found in
// User macros and JSON size and make functions
#define JSON_STR                6 // character overhead, in bytes, i.e. "":"",
#define JSON_LITERAL            4 // number, true, false, or null, character overhead, in bytes, i.e. "":,
#define JSON_OBJ                0 // number of bytes to subtract from STR or LITERAL when sizing
#define JSON_ARR                3 // number of bytes to subtract from STR or LITERAL when sizing (i.e. array elements are either STR => "", or LITERAL => ,)

// User MACROS
#define len(a)                  (sizeof(a) / sizeof(a[0]))
#define JSON_SIZE(j)            (json_size(len(j), j, JSON_OBJ)) // wrapper to minimize size errors when using json_size
#define JSON_ARRAY_SIZE(a)      (json_size(len(a), a, JSON_ARR))
#define JSON_MAKE(j, b)         (make_json_object(len(j), j, b, JSON_OBJ)) // wrapper to minimize size errors when using make_json
#define JSON_MAKE_ARRAY(a, b)   (make_json_object(len(a), a, b, JSON_ARR))
#define JSON_TRUE  "true",      JSON_LITERAL
#define JSON_FALSE "false",     JSON_LITERAL
#define JSON_NULL  "null",      JSON_LITERAL
#define JSON_NUMBER(n)          n, JSON_LITERAL
#define JSON_STRING(n)          n, JSON_STR
#define JSON_OBJECT(n)          n, JSON_LITERAL
#define JSON_ARRAY(n)           n, JSON_LITERAL

typedef struct json_element json_element;
typedef struct json_object json_object;

/*
 *  Used to fill JSON arrays
 */
struct json_element {
    const char *value;
    const int type;
};

/*
 *  Used to fill JSON objects
 */
struct json_object {
    const char *key;
    const char *value;
    const int type; // string, number, true, false, or null
}; // example: (cmd, move, JSON_STR), (opponent, 1, JSON_NUMBER), etc.

int make_json_object(int n, void *objects, char *buffer, int type);
int json_size(int n, void *objects, int type);
