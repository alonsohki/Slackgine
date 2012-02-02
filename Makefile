all:
	cd shared && make -f Makefile all CONF=Release DEFAULTCONF=Release
	cd math && make -f Makefile all CONF=Release DEFAULTCONF=Release
	cd l3m && make -f Makefile all CONF=Release DEFAULTCONF=Release
	cd core && make -f Makefile all CONF=Release DEFAULTCONF=Release
	cd renderer && make -f Makefile all CONF=Release DEFAULTCONF=Release
	cd import && make -f Makefile all CONF=Release DEFAULTCONF=Release
	cd model_optimizer && make -f Makefile all CONF=Release DEFAULTCONF=Release
	cd model_splitter && make -f Makefile all CONF=Release DEFAULTCONF=Release
	mkdir -p dist/include
	mkdir -p dist/lib
	mkdir -p dist/bin
	mkdir -p dist/shaders
	cp shaders/* dist/shaders/
	cp slackgine.h dist/include/
	for i in `find core -type f -name "*.h"` ; do \
	    mkdir -p dist/include/`dirname $$i` ; \
        cp $$i dist/include/$$i ; \
	done
	for i in `find shared -type f -name "*.h"` ; do \
        mkdir -p dist/include/`dirname $$i` ; \
        cp $$i dist/include/$$i ; \
    done
	for i in `find math -type f -name "*.h"` ; do \
        mkdir -p dist/include/`dirname $$i` ; \
        cp $$i dist/include/$$i ; \
    done
	for i in `find l3m -type f -name "*.h"` ; do \
        mkdir -p dist/include/`dirname $$i` ; \
        cp $$i dist/include/$$i ; \
    done
	for i in `find renderer -type f -name "*.h"` ; do \
        mkdir -p dist/include/`dirname $$i` ; \
        cp $$i dist/include/$$i ; \
    done
	cp `find shared/dist -name "libshared.a" | grep Release` dist/lib/
	cp `find math/dist -name "libmath.a" | grep Release` dist/lib/
	cp `find l3m/dist -name "libl3m.a" | grep Release` dist/lib/
	cp `find core/dist -name "libcore.a" | grep Release` dist/lib/
	cp `find renderer/dist -name "librenderer.a" | grep Release` dist/lib/
	cp `find import/dist -name "import" | grep Release` dist/bin/
	cp `find model_optimizer/dist -name "model_optimizer" | grep Release` dist/bin/
	cp `find model_splitter/dist -name "model_splitter" | grep Release` dist/bin/

clean:
	cd shared && make -f Makefile clean CONF=Release DEFAULTCONF=Release
	cd math && make -f Makefile clean CONF=Release DEFAULTCONF=Release
	cd l3m && make -f Makefile clean CONF=Release DEFAULTCONF=Release
	cd core && make -f Makefile clean CONF=Release DEFAULTCONF=Release
	cd renderer && make -f Makefile clean CONF=Release DEFAULTCONF=Release
	cd import && make -f Makefile clean CONF=Release DEFAULTCONF=Release
	cd model_optimizer && make -f Makefile clean CONF=Release DEFAULTCONF=Release
	cd model_splitter && make -f Makefile clean CONF=Release DEFAULTCONF=Release

distclean: clean
	rm -rf dist

