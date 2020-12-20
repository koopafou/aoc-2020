
CXXFLAGS=-std=c++20 -mfpmath=sse -msse2 -g
PKGLIST=fmt
-include ${SRC}/Makefile

#######

PKG_CFLAGS=$(foreach PKG,$(PKGLIST),$(shell pkg-config --cflags $(PKG)))
PKG_LIBS=$(foreach PKG,$(PKGLIST),$(shell pkg-config --libs $(PKG)))

OBJ_LIST=$(addprefix ${DST}/,$(addsuffix .o,$(BASE)))
DEP_LIST=$(addprefix ${DST}/,$(addsuffix .d,$(BASE)))

${DST}/run: $(OBJ_LIST)
	g++ $(PKG_LIBS) $(CXXFLAGS) -o $@ $^

${DST}/%.d: ${SRC}/%.cc
	mkdir -p `dirname $@`
	g++ $(PKG_CFLAGS) -MM -MG -MT $@ -MF $@ $<

${DST}/Makefile-deps: $(DEP_LIST)
	cat $^ > $@

-include ${DST}/Makefile-deps

${DST}/%.o: ${SRC}/%.cc ${DST}/%.d
	mkdir -p `dirname $@`
	g++ $(PKG_CFLAGS) $(CXXFLAGS) -c -o $@ $<
