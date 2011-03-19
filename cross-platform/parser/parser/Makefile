CC     = g++
CFLAGS = -Wall
BIN    = parser

$(BIN) : Main.cpp keymagic-parser.o keymagic-scanner.o keymagic-driver.o location.hh position.hh stack.hh
	$(CC) $(CFLAGS) Main.cpp ConvertUTF.cpp keymagic-driver.o keymagic-scanner.o keymagic-parser.o -o $(BIN)

keymagic-driver.o : keymagic-driver.cpp keymagic-driver.hpp
	$(CC) $(CFLAGS) -c -o keymagic-driver.o keymagic-driver.cpp

keymagic-parser.o : keymagic-parser.ypp
	bison -l keymagic-parser.ypp
	$(CC) $(CFLAGS) -c -o keymagic-parser.o keymagic-parser.tab.cpp

keymagic-scanner.o : keymagic-parser.ypp keymagic-scanner.lpp
	flex -L --nounistd -olex.yy.cpp keymagic-scanner.lpp
	$(CC) $(CFLAGS) -c -o keymagic-scanner.o lex.yy.cpp

.PHONY : clean
clean :
# remove bison generated files
	rm -rf keymagic-parser.tab.cpp keymagic-parser.tab.hpp location.hh position.hh stack.hh
# remove flex generated files
	rm -rf lex.yy.cpp
# remove compiled files
	rm -rf $(BIN) *.o

