#include <string.h>
#include "./unity/src/unity.h"
#include "json.h"

// UNIT TESTS ////////////////////////////////////////////////////////////////

void test_json_true(void) {
    json_object json[] = {
        {"key", JSON_TRUE}
    };
    TEST_ASSERT_EQUAL_INT(json[0].type, JSON_LITERAL);
    TEST_ASSERT_EQUAL_STRING(json[0].key, "key");
    TEST_ASSERT_EQUAL_STRING(json[0].value, "true");
}

void test_json_false(void) {
    json_object json[] = {
        {"key", JSON_FALSE}
    };
    TEST_ASSERT_EQUAL_INT(json[0].type, JSON_LITERAL);
    TEST_ASSERT_EQUAL_STRING(json[0].key, "key");
    TEST_ASSERT_EQUAL_STRING(json[0].value, "false");
}

void test_json_null(void) {
    json_object json[] = {
        {"key", JSON_NULL}
    };
    TEST_ASSERT_EQUAL_INT(json[0].type, JSON_LITERAL);
    TEST_ASSERT_EQUAL_STRING(json[0].key, "key");
    TEST_ASSERT_EQUAL_STRING(json[0].value, "null");
}

void test_json_number(void) {
    json_object json[] = {
        {"key", JSON_NUMBER("1")}
    };
    TEST_ASSERT_EQUAL_INT(json[0].type, JSON_LITERAL);
    TEST_ASSERT_EQUAL_STRING(json[0].key, "key");
    TEST_ASSERT_EQUAL_STRING(json[0].value, "1");
}

void test_json_string(void) {
    json_object json[] = {
        {"key", JSON_STRING("value")}
    };
    TEST_ASSERT_EQUAL_INT(json[0].type, JSON_STR);
    TEST_ASSERT_EQUAL_STRING(json[0].key, "key");
    TEST_ASSERT_EQUAL_STRING(json[0].value, "value");
}

void test_json_size(void) {
    json_object json[] = {
        {"key0", JSON_STRING("value0")}, // 16
        {"key1", JSON_NUMBER("1")},      //  9
        {"key2", JSON_TRUE},             // 12
        {"key3", JSON_FALSE},            // 13
        {"key4", JSON_NULL},             // 12
    };                                   //  62 + 2 - 1 = 63
    TEST_ASSERT_EQUAL_INT(JSON_SIZE(json),  63);
}

void test_json_size_no_elements(void) {
    TEST_ASSERT_EQUAL_INT(json_size(0, NULL, JSON_OBJ), 0);
}

void test_json_size_null_objects(void) {
    TEST_ASSERT_EQUAL_INT(json_size(2, NULL, JSON_OBJ), 0);
}

void test_json_make_no_elements(void) {
    TEST_ASSERT_EQUAL_INT(make_json_object(0, NULL, NULL, JSON_OBJ), 0);
}

void test_json_make_null_objects(void) {
    TEST_ASSERT_EQUAL_INT(make_json_object(2, NULL, NULL, JSON_OBJ), 0);
}

void test_json_make_null_buffer(void) {
    json_object json[2];
    TEST_ASSERT_EQUAL_INT(make_json_object(2, json, NULL, JSON_OBJ), 0);
}

void test_json_make(void) {
    json_object json[] = {
        {"key0", JSON_STRING("value0")},
        {"key1", JSON_NUMBER("1")},
    };
    int size = JSON_SIZE(json);
    char buffer[size];
    int n_chars = JSON_MAKE(json, buffer);
    TEST_ASSERT_EQUAL_STRING(buffer, "{\"key0\":\"value0\",\"key1\":1}");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, n_chars, "JSON_SIZE != n_chars");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, strlen(buffer), "JSON_SIZE != strlen(buffer)");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, sizeof(buffer), "JSON_SIZE != sizeof(buffer)");
}

void test_json_make_all(void) {
    json_object json[] = {
        {"key0", JSON_STRING("value0")}, // 16
        {"key1", JSON_NUMBER("1")},      //  9
        {"key2", JSON_TRUE},             // 12
        {"key3", JSON_FALSE},            // 13
        {"key4", JSON_NULL},             // 12
    };                                  //  62 + 2 - 1 = 63
    int size = JSON_SIZE(json);
    char buffer[size];
    int n_chars = JSON_MAKE(json, buffer);
    TEST_ASSERT_EQUAL_STRING(buffer, "{\"key0\":\"value0\",\"key1\":1,\"key2\":true,\"key3\":false,\"key4\":null}");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, n_chars, "JSON_SIZE != n_chars");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, strlen(buffer), "JSON_SIZE != strlen(buffer)");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, sizeof(buffer), "JSON_SIZE != sizeof(buffer)");
}

void test_json_nested_object(void) {
    json_object nested[] = {
        {"nested_key0", JSON_STRING("nested_value0")},
        {"nested_key1", JSON_NUMBER("2")},
    };
    char nested_buffer[JSON_SIZE(nested)];
    int nested_chars = JSON_MAKE(nested, nested_buffer);

    json_object json[] = {
        {"key0", JSON_STRING("value0")},
        {"nest", JSON_OBJECT(nested_buffer)},
    };
    int size = JSON_SIZE(json);
    char buffer[size];
    int n_chars = JSON_MAKE(json, buffer);
    TEST_ASSERT_EQUAL_STRING(buffer, "{\"key0\":\"value0\",\"nest\":{\"nested_key0\":\"nested_value0\",\"nested_key1\":2}}");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, n_chars, "JSON_SIZE != n_chars");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, strlen(buffer), "JSON_SIZE != strlen(buffer)");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, sizeof(buffer), "JSON_SIZE != sizeof(buffer)");
}

void test_json_nested_array(void) {
    json_element nested[] = {
        {JSON_STRING("nested_value0")},
        {JSON_NUMBER("2")},
    };
    char nested_buffer[JSON_ARRAY_SIZE(nested)];
    int nested_chars = JSON_MAKE_ARRAY(nested, nested_buffer);

    json_object json[] = {
        {"key0", JSON_STRING("value0")},
        {"nest", JSON_ARRAY(nested_buffer)},
    };
    int size = JSON_SIZE(json);
    char buffer[size];
    int n_chars = JSON_MAKE(json, buffer);
    TEST_ASSERT_EQUAL_STRING(buffer, "{\"key0\":\"value0\",\"nest\":[\"nested_value0\",2]}");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, n_chars, "JSON_SIZE != n_chars");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, strlen(buffer), "JSON_SIZE != strlen(buffer)");
    TEST_ASSERT_EQUAL_INT_MESSAGE(size, sizeof(buffer), "JSON_SIZE != sizeof(buffer)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_json_true);
    RUN_TEST(test_json_false);
    RUN_TEST(test_json_null);
    RUN_TEST(test_json_number);
    RUN_TEST(test_json_string);
    RUN_TEST(test_json_size);
    RUN_TEST(test_json_size_no_elements);
    RUN_TEST(test_json_size_null_objects);
    RUN_TEST(test_json_make_no_elements);
    RUN_TEST(test_json_make_null_objects);
    RUN_TEST(test_json_make_null_buffer);
    RUN_TEST(test_json_make);
    RUN_TEST(test_json_make_all);
    RUN_TEST(test_json_nested_object);
    RUN_TEST(test_json_nested_array);
    return UNITY_END();
}
