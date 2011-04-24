LOCAL_MODULE            := renderer
LOCAL_MODULE_FILENAME   := renderer
LOCAL_SRC_FILES         := renderers/AndroidGLES20/program.cpp renderers/AndroidGLES20/renderer.cpp renderers/AndroidGLES20/shader.cpp

include $(BUILD_STATIC_LIBRARY)
