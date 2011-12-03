all:
	NDK_DEBUG=1 NDK_PROJECT_PATH="`pwd`" /soft/dev/android-ndk/ndk-build -j4
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

clean:
	NDK_DEBUG=1 NDK_PROJECT_PATH="`pwd`" /soft/dev/android-ndk/ndk-build clean

distclean:
	NDK_DEBUG=1 NDK_PROJECT_PATH="`pwd`" /soft/dev/android-ndk/ndk-build distclean
	rm -rf dist libs obj
