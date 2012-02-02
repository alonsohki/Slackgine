all:
	NDK_PROJECT_PATH="`pwd`" ndk-build -j
	mkdir -p dist/include
	cp ../slackgine.h dist/include/
	for i in `cd .. && find core -type f -name "*.h"` ; do \
	    mkdir -p dist/include/`dirname $$i` ; \
        cp ../$$i dist/include/$$i ; \
	done
	for i in `cd .. && find shared -type f -name "*.h"` ; do \
        mkdir -p dist/include/`dirname $$i` ; \
        cp ../$$i dist/include/$$i ; \
    done
	for i in `cd .. && find math -type f -name "*.h"` ; do \
        mkdir -p dist/include/`dirname $$i` ; \
        cp ../$$i dist/include/$$i ; \
    done
	for i in `cd .. && find l3m -type f -name "*.h"` ; do \
        mkdir -p dist/include/`dirname $$i` ; \
        cp ../$$i dist/include/$$i ; \
    done
	for i in `cd .. && find renderer -type f -name "*.h"` ; do \
        mkdir -p dist/include/`dirname $$i` ; \
        cp ../$$i dist/include/$$i ; \
    done
	mkdir -p dist/lib && cp obj/local/armeabi/*.a dist/lib/
	mkdir -p dist/shaders && cp ../shaders/*.glsl dist/shaders/

clean:
	NDK_PROJECT_PATH="`pwd`" ndk-build clean

distclean:
	NDK_PROJECT_PATH="`pwd`" ndk-build distclean
	rm -rf dist libs obj
