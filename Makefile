CC=gcc
CFLAGS=-Wall -O2

json: json.c
	$(CC) $(CFLAGS) -o json json.c

test_json: unity json.c json_unit_tests.c 
	$(CC) $(CFLAGS) -DTEST -o json_unit_tests ./unity/src/unity.c json.c json_unit_tests.c
	./json_unit_tests

unity:
	wget https://github.com/ThrowTheSwitch/Unity/archive/master.zip
	unzip master.zip
	mv Unity-master unity

test: test_json

clean:
	rm -r json_unit_tests json unity master.zip *~

