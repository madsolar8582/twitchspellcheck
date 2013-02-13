# Tested on Mac OS X 10.8.2 (12C60)
# Apple LLVM version 4.2 (clang-425.0.24) (based on LLVM 3.2svn)
# Target: x86_64-apple-darwin12.2.0 Thread model: posix
# GNU Make 3.81 for i386-apple-darwin11.3.0
# GNU bash, version 3.2.48(1)-release (x86_64-apple-darwin12)
CXX=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++ -O3
CXXDEBUGFLAGS=-Weverything -std=c++11 -stdlib=libc++ -g -O0

all: spellchecker.out wordgenerator.out

debug: spellcheckerdbg.out wordgeneratordbg.out

spellchecker.out:
	$(CXX) $(CXXFLAGS) spellchecker.cpp -o spellchecker.out

wordgenerator.out:
	$(CXX) $(CXXFLAGS) wordgenerator.cpp -o wordgenerator.out

spellcheckerdbg.out:
	$(CXX) $(CXXDEBUGFLAGS) spellchecker.cpp -o spellcheckerdbg.out

wordgeneratordbg.out:
	$(CXX) $(CXXDEBUGFLAGS) wordgenerator.cpp -o wordgeneratordbg.out

.PHONY: clean
.SILENT: clean
clean:
	if [ -a spellchecker.out ]; \
	then \
	  rm spellchecker.out; \
	fi; \
	if [ -a wordgenerator.out ]; \
	then \
	  rm wordgenerator.out; \
	fi; \
	if [ -a spellcheckerdbg.out ]; \
	then \
	  rm spellcheckerdbg.out; \
	fi; \
	if [ -a wordgeneratordbg.out ]; \
	then \
	  rm wordgeneratordbg.out; \
	fi; \
	if [ -a spellcheckerdbg.out.dSYM ]; \
	then \
	  rm -r spellcheckerdbg.out.dSYM; \
	fi; \
	if [ -a wordgeneratordbg.out.dSYM ]; \
	then \
	  rm -r wordgeneratordbg.out.dSYM; \
	fi; \
	if [ -a verificationresults.txt ]; \
	then \
	  rm verificationresults.txt; \
	fi; \
	if [ -a wordsgenerated.txt ]; \
	then \
	  rm wordsgenerated.txt; \
	fi;

