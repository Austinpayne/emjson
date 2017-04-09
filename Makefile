CC=gcc
CFLAGS=-Wall -g

json: json.c
	$(CC) $(CFLAGS) -o json json.c

test_json: unity json.c json_unit_tests.c 
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage -DTEST -o json_unit_tests ./unity/src/unity.c json.c json_unit_tests.c
	./json_unit_tests
	gcov -lp json_unit_tests.c

unity:
	wget https://github.com/ThrowTheSwitch/Unity/archive/master.zip
	unzip master.zip
	mv Unity-master unity

test: test_json

clean:
	rm -r json_unit_tests json unity master.zip *.gcov *.gcno *.gcda *.dSYM *~

