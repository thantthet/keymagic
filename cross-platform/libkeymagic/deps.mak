KeyCodes.o: KeyCodes.cpp KeyCodes.h
KeyMagicEngine.o: KeyMagicEngine.cpp KeyMagicKeyboard.h KeyMagicTypes.h \
  KeyMagicString.h RuleInfo.h KeyMagicLogger.h KeyMagicEngine.h
KeyMagicKeyboard.o: KeyMagicKeyboard.cpp KeyMagicKeyboard.h \
  KeyMagicTypes.h KeyMagicString.h RuleInfo.h KeyMagicLogger.h
KeyMagicLogger.o: KeyMagicLogger.cpp KeyMagicLogger.h KeyMagicTypes.h \
  KeyMagicString.h
KeyMagicString.o: KeyMagicString.cpp KeyMagicString.h
RuleInfo.o: RuleInfo.cpp RuleInfo.h KeyMagicString.h KeyMagicTypes.h \
  KeyMagicLogger.h KeyCodes.h
Util.o: Util.cpp Util.h KeyMagicEngine.h KeyMagicTypes.h KeyMagicString.h \
  RuleInfo.h KeyMagicLogger.h KeyMagicKeyboard.h
