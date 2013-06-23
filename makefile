
# Replace this line with actual credentials and/or hostname for MySQL
MYSQL=mysql
MYSQL_H=/usr/include/mysql/
MYSQL_SO=/usr/lib/mysql/plugin

all:
	gcc -I ${MYSQL_H} getutcdate.c -o getutcdate.so -fPIC -shared -Wall

clean:
	-rm *~
	-rm *.o
	-rm *.so

install:
	sudo chmod 0644 getutcdate.so
	-echo "DROP FUNCTION GETUTCDATE;" | ${MYSQL}
	sudo cp getutcdate.so ${MYSQL_SO}
	echo "CREATE FUNCTION GETUTCDATE RETURNS STRING SONAME 'getutcdate.so';" | ${MYSQL}
