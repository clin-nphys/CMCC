CFLAGS = 
CC = g++

CMCC : driver.o CMCController.o Utils.o
	$(CC) $(CFLAGS) driver.o CMCController.o Utils.o -o CMCC
driver.o : driver.cpp
	$(CC) $(CFLAGS) -c driver.cpp
CMCController.o : CMCController.cpp
	$(CC) $(CFLAGS) -c CMCController.cpp
Utils.o : Utils.cpp
	$(CC) $(CFLAGS) -c Utils.cpp
clean:
	rm -f core *.o
