CC=gcc
CFLAGS=-Wall -O2

json: json.c
	$(CC) $(CFLAGS) -o json json.c
	
test_json: ./unity/src/unity.c json.c json_unit_tests.c 
	$(CC) $(CFLAGS) -DTEST -o json_unit_tests ./unity/src/unity.c json.c json_unit_tests.c
	./json_unit_tests
	
test: test_json

clean:
	rm json_unit_tests json *~
