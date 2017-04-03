# emjson
emjson (as in "embedded JSON" or "make JSON") is a small JSON encoder written in C. It is intended for use in embedded applications, specifically those without access to dynamic memory allocation.

## API
emjson is lightweight, is only about 100 LOC, and includes only single source and header file. It inludes two structs used to build JSON objects and arrays:

JSON objects can be constructed using an array of json_objects:
```
struct json_object {
    const char *key;
    const char *value;
    const int type;
};
```

and JSON arrays can be constructed from an array of json_elements:
```
struct json_element {
    const char *value;
    const int type;
};
```

Typically, JSON objects are built by first sizing a `char *` buffer using `json_size` and then building the JSON string using `make_json_object`. The prototypes for these functions are:

```
int json_size(int n, void *objects, int obj_type);
int make_json_object(int n, void *objects, char *buffer, int obj_type);
```

## Examples
emjson relies on a few macros to minimize encoding errors. See json.c and json.h for more detail on why the User Macros should be used rather than calling `json_size` and `make_json_object` directly. The following example shows how make a flat JSON object:

```
json_object json[] = {
    {"key0", JSON_STRING("value")},
    {"key1", JSTON_NUMBER("1")},
    {"key2", JSON_TRUE},
    {"key3", JSON_FALSE},
    {"key4", JSON_NULL},
};
char buffer[JSON_SIZE(json)];
int n_chars = JSON_MAKE(json, buffer);
```

After the above, `buffer` would contain the following JSON string (formatted for readability):

```
{
	"key0": "value",
	"key1": 1,
	"key2": true,
	"key3": false,
	"key4": null
}
```

JSON arrays and objects can also be nested, as in this example:

```
json_element array[] = {
	{JSON_STRING("array string")},
	{JSON_NUMBER("2"},
    // ...
}
char array_buffer[JSON_ARRAY_SIZE(array)];
int array_chars = JSON_MAKE_ARRAY(array, array_buffer);

json_object json[] = {
	{"some_key, JSON_STRING("val")},
    {"array_key", JSON_ARRAY(array_buffer)},
	// ...
}
char final_json[JSON_SIZE(json)];
int n_chars = JSON_MAKE(json, final_json);
```

The above would create the following JSON:

```
{
	"some_key": "val",
	[
		"array string",
		2
	]
}
```

## Testing
A set of [Unity](http://www.throwtheswitch.org/unity/) unit tests has been provided to verify correctness. They can be run using `make test`.

## Limitations
emjson was conceptualized overnight and coded even faster. For this reason it is has much room for improvement (could be simpler, more organzied, faster, use less memory, etc.) If you have any suggestions, feel free to submit them via a pull request.

## License
emjson is distributed under the [MIT License](https://opensource.org/licenses/MIT).
