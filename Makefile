CC=gcc
CFLAGS=-Wall

json: ./src/json.c
	$(CC) $(CFLAGS) -o json ./src/json.c

test_json: unity ./src/json.c ./test/test_json.c 
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage -DUNIT_TEST -DUNIT_TEST_NATIVE -o test_json ./unity/src/unity.c ./src/json.c ./test/test_json.c
	./test_json
	gcov -lp test_json.c

unity:
	wget https://github.com/ThrowTheSwitch/Unity/archive/master.zip
	unzip master.zip
	mv Unity-master unity

test: test_json

clean:
	rm -r test_json json unity master.zip *.gcov *.gcno *.gcda *~

