CC     = g++
CFLAGS = -Wall
BIN    = jkeymagickb

$(BIN) : main.cpp keyboard.cpp keyboard.h ConvertUTF.cpp ConvertUTF.h
	$(CC) $(CFLAGS) main.cpp keyboard.cpp ConvertUTF.cpp libkeymagic.a -o $(BIN)

.PHONY : clean
clean :
# remove compiled files
	rm -rf $(BIN) *.o

