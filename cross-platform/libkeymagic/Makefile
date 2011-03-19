CC     = g++
CFLAGS = -Wall
LIBTOOL = libtool
BIN    = libkeymagic.a

$(BIN) : KeyMagicEngine.o KeyMagicString.o KeyMagicKeyboard.o KeyCodes.o KeyMagicLogger.o RuleInfo.o Util.o
	$(LIBTOOL) -static -o $(BIN) $^


.PHONY : clean
clean :
# remove compiled files
	rm -rf $(BIN) *.o

include deps.mak