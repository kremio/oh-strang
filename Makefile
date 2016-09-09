#Adapted from https://gist.github.com/funkaster/5199237
#GNU Make docs: http://www.gnu.org/software/make/manual/make.html
#Emscripten: http://kripken.github.io/emscripten-site/docs/
EMSDK_HOME = ~/playground/emscripten/emsdk_portable
EMSCRIPTEN_HOME = $(EMSDK_HOME)/emscripten/1.35.0
CLANG = /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang

SOURCES = $(wildcard src/*) $(wildcard src/*.c)
BINDINGS =  $(wildcard binding/*) $(wildcard src/*.c)
OBJECTS = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(SOURCES)))
BINDING_OBJECTS = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(BINDINGS)))
TESTS = $(wildcard test/*.cpp) $(wildcard test/*.c)
TESTS_OBJECTS = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(TESTS)))

#Sets up the EMSDK environment inside make.
#See gdw2 answer at https://stackoverflow.com/questions/7507810/howto-source-a-script-from-makefile/16490872#16490872
IGNORE := $(shell bash -c "source  $(EMSDK_HOME)/emsdk_env.sh; env | sed 's/=/:=/' | sed 's/^/export /' > makeenv")                         
include makeenv 

#This target is broken at the moment. Anyway, it would not make much sense in this example to try to compile a native app	
set-native:
	$(eval CXX := $(CLANG) -x c++)
	$(eval CC := $(CLANG))
	$(eval CPPFLAGS := -g -O2 -DHAS_TR1)
	$(eval CXXFLAGS := -v -std=c++11 -stdlib=libc++)
	$(eval LDFLAGS := -lstdc++)
	$(eval TARGET := native)

set-js:
	$(eval CXX := em++)
	$(eval CC := emcc)
	$(eval CXXFLAGS := -std=c++11 -s ASM_JS=1)
	$(eval LDFLAGS := -lc++ -O0)
	$(eval TARGET := js)

set-html: set-js
	$(eval TARGET := html)

compile: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $(PROJECT).$(TARGET)
	
compile-js : $(OBJECTS) $(BINDING_OBJECTS)
	$(CC) $(LDFLAGS) $^ --post-js glue.js -o js/$(PROJECT).$(TARGET)

compile-test: $(TESTS_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $(PROJECT).test

webidl-binding: matrix.idl
	 $(shell bash -c "python  $(EMSCRIPTEN_HOME)/tools/webidl_binder.py matrix.idl glue")

native: set-native compile

test: set-native compile-test

js: set-js show-vars webidl-binding compile-js

js-html: set-html show-vars compile

show-vars:
	echo $(PATH)
	echo $(SOURCES)
	echo $(OBJECTS)

clean:
	rm -f */*.o
	rm -f glue.*
	rm -rf $(PROJECT).* */*.dSYM
	rm -f makeenv