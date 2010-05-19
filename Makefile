GECKO_SDK_PATH=/src/firefox/xulrunner-sdk

CXX=c++ -O3 
CPPFLAGS+=-fPIC -shared -Wl,-h,libsip.so 
 
GECKO_CONFIG_INCLUDE=-include mozilla-config.h 
 
GECKO_DEFINES=-DXPCOM_GLUE
 
GECKO_INCLUDES=-I $(GECKO_SDK_PATH)/include 
GECKO_LDFLAGS=-L$(GECKO_SDK_PATH)/lib -L$(GECKO_SDK_PATH)/sdk/lib  $(GECKO_SDK_PATH)/sdk/lib/libxpcomglue_s.a -lxpcomglue -lxpcom -lnspr4  
PJSIP_FLAGS=-DPJ_AUTOCONF=1 -O2 -I/usr/local/include  -L/usr/local/lib -lpjsua-i686-pc-linux-gnu -lpjsip-ua-i686-pc-linux-gnu -lpjsip-simple-i686-pc-linux-gnu -lpjsip-i686-pc-linux-gnu -lpjmedia-codec-i686-pc-linux-gnu -lpjmedia-i686-pc-linux-gnu -lpjmedia-audiodev-i686-pc-linux-gnu -lpjnath-i686-pc-linux-gnu -lpjlib-util-i686-pc-linux-gnu -lresample-i686-pc-linux-gnu -lmilenage-i686-pc-linux-gnu -lsrtp-i686-pc-linux-gnu -lgsmcodec-i686-pc-linux-gnu -lspeex-i686-pc-linux-gnu -lilbccodec-i686-pc-linux-gnu -lg7221codec-i686-pc-linux-gnu -lportaudio-i686-pc-linux-gnu -lpj-i686-pc-linux-gnu -lm -lnsl -lrt -lpthread -lasound -lcrypto -lssl


FILES=nsSIP.o nsSIPModule.o

TARGET=libsip.so



all: clean prepare build

prepare: header xpt

build:  
	$(CXX) -fPIC -shared -Wl,-h,libpjsip.so -o libpjsip.so pjsip.cpp $(PJSIP_FLAGS)
	$(CXX) $(CPPFLAGS) -c nsSIP.cpp $(GECKO_INCLUDES) $(GECKO_CONFIG_INCLUDE)
	$(CXX) $(CPPFLAGS) -c nsSIPModule.cpp  $(GECKO_INCLUDES) $(GECKO_CONFIG_INCLUDE)
	$(CXX) $(CPPFLAGS) -o $(TARGET) $(FILES) $(GECKO_LDFLAGS) $(GECKO_DEFINES) -L. -lpjsip
 
header:
	$(GECKO_SDK_PATH)/bin/xpidl -m header -w -v -I $(GECKO_SDK_PATH)/idl/ -o nsISIP nsISIP.idl

xpt:
	$(GECKO_SDK_PATH)/bin/xpidl -m typelib -w -v -I $(GECKO_SDK_PATH)/idl/ -o nsISIP nsISIP.idl

clean: 
	rm -rf $(TARGET)
	rm -rf *.xpt
	rm -rf *.o 
	rm -rf *.so 
