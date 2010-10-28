GECKO_SDK_PATH=/src/firefox/xulrunner-sdk
LINPHONE_SDK=./linphone-sdk-amd64/

CXX=c++ 
CPPFLAGS+=-O3 -fPIC -shared -DDEBUG
LDFLAGS+=-Wl,-h,libsip.so 
 
GECKO_DEFINES=-DXPCOM_GLUE
GECKO_INCLUDES=-I $(GECKO_SDK_PATH)/include -I$(LINPHONE_SDK)/include -include mozilla-config.h
GECKO_LDFLAGS=-L$(GECKO_SDK_PATH)/lib -L$(GECKO_SDK_PATH)/bin -Wl,-rpath-link,$(GECKO_SDK_PATH)/bin -lxpcomglue_s -lxpcom -lnspr4 -lssl

# DYNAMIC COMPILATION
LINPHONE_FLAGS=`pkg-config --cflags --libs linphone`

# STATIC COMPILATION WITH VIDEO SUPPORT
#LINPHONE_FLAGS=-D_REENTRANT -DORTP_INET6 -L/usr/local/lib  -lpthread -lssl -lcrypto  $(LINPHONE_SDK)/lib/liblinphone.a  $(LINPHONE_SDK)/lib/libmediastreamer.a $(LINPHONE_SDK)/lib/libortp.a /usr/lib/libeXosip2.a /usr/lib/libosip2.a /usr/lib/libosipparser2.a /usr/lib/libv4l2.a  /usr/lib/libtheora.a /usr/lib/libtheoraenc.a /usr/lib/libavcodec.a /usr/lib/libavutil.a  /usr/lib/libgsm.a /usr/lib/libschroedinger-1.0.a /usr/lib/libspeex.a /usr/lib/libspeexdsp.a /usr/lib/libvorbis.a /usr/lib/libvorbisenc.a /usr/lib/libvorbisfile.a /usr/lib/libswscale.a  /usr/lib/libv4lconvert.a /usr/lib/libogg.a /usr/lib/liboil-0.3.a -lSDL

# STATIC COMPILATION WITHOUT VIDEO SUPPORT
# REQUIRE LIBLINPHONE CONFIGURED WITH --disable-video
#LINPHONE_FLAGS=-D_REENTRANT -DORTP_INET6  -L/usr/local/lib  -lpthread -lssl -lcrypto $(LINPHONE_SDK)/lib/liblinphone.a  $(LINPHONE_SDK)/lib/libmediastreamer.a $(LINPHONE_SDK)/lib/libortp.a /usr/lib/libeXosip2.a /usr/lib/libosip2.a /usr/lib/libosipparser2.a /usr/lib/libspeex.a /usr/lib/libspeexdsp.a -lasound

FILES=nsProxyConfig.o nsCallLog.o nsSIP.o nsSIPModule.o
TARGET=libsip.so



all: clean prepare build

prepare: header xpt

build:  
	$(CXX) $(CPPFLAGS) -c nsProxyConfig.cpp $(GECKO_INCLUDES) $(GECKO_CONFIG_INCLUDE)
	$(CXX) $(CPPFLAGS) -c nsCallLog.cpp $(GECKO_INCLUDES) $(GECKO_CONFIG_INCLUDE)
	$(CXX) $(CPPFLAGS) -c nsSIP.cpp $(GECKO_INCLUDES) $(GECKO_CONFIG_INCLUDE)
	$(CXX) $(CPPFLAGS) -c nsSIPModule.cpp  $(GECKO_INCLUDES) $(GECKO_CONFIG_INCLUDE)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $(TARGET) $(FILES) $(GECKO_LDFLAGS) $(GECKO_DEFINES) $(LINPHONE_FLAGS)
	strip $(TARGET)
 
header:
	$(GECKO_SDK_PATH)/bin/xpidl -m header -w -v -I $(GECKO_SDK_PATH)/idl/ -o nsISIP nsISIP.idl

xpt:
	$(GECKO_SDK_PATH)/bin/xpidl -m typelib -w -v -I $(GECKO_SDK_PATH)/idl/ -o nsISIP nsISIP.idl

clean: 
	rm -rf $(TARGET)
	rm -rf *.xpt
	rm -rf *.o 
	rm -rf *.so 
