KeyCodes.o: KeyCodes.cpp KeyCodes.h
KeyMagicEngine.o: KeyMagicEngine.cpp KeyMagicKeyboard.h KeyMagicTypes.h \
  KeyMagicString.h RuleInfo.h KeyMagicLogger.h LogFileWriter.h \
  KeyMagicErrorLogger.h KeyMagicEngine.h
KeyMagicErrorLogger.o: KeyMagicErrorLogger.cpp KeyMagicErrorLogger.h \
  LogFileWriter.h
KeyMagicKeyboard.o: KeyMagicKeyboard.cpp KeyMagicKeyboard.h \
  KeyMagicTypes.h KeyMagicString.h RuleInfo.h KeyMagicLogger.h \
  LogFileWriter.h KeyMagicErrorLogger.h
KeyMagicLogger.o: KeyMagicLogger.cpp KeyMagicLogger.h LogFileWriter.h
KeyMagicString.o: KeyMagicString.cpp KeyMagicString.h
LogFileWriter.o: LogFileWriter.cpp LogFileWriter.h
RuleInfo.o: RuleInfo.cpp RuleInfo.h KeyMagicString.h KeyMagicTypes.h \
  KeyMagicLogger.h LogFileWriter.h KeyCodes.h
Util.o: Util.cpp Util.h KeyMagicEngine.h KeyMagicTypes.h KeyMagicString.h \
  RuleInfo.h KeyMagicLogger.h LogFileWriter.h KeyMagicKeyboard.h \
  KeyMagicErrorLogger.h
