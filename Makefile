GECKO_SDK_PATH=/src/firefox/xulrunner-sdk

CXX=c++ 
CPPFLAGS+=-O3 -fPIC -shared -Wl,-h,libsip.so 
 
GECKO_DEFINES=-DXPCOM_GLUE
GECKO_INCLUDES=-I $(GECKO_SDK_PATH)/include -include mozilla-config.h 
GECKO_LDFLAGS=-L$(GECKO_SDK_PATH)/lib -L$(GECKO_SDK_PATH)/bin -Wl,-rpath-link,$(GECKO_SDK_PATH)/bin -lxpcomglue_s -lxpcom -lnspr4 -lssl
PJSIP_FLAGS=`pkg-config --cflags --libs libpjproject`
SSL_FLAGS=/usr/lib/libssl.a


FILES=pjsip.o nsSIP.o nsSIPModule.o
TARGET=libsip.so



all: clean prepare build

prepare: header xpt

build:  
	$(CXX) $(CPPFLAGS) -c pjsip.cpp $(GECKO_INCLUDES) $(GECKO_CONFIG_INCLUDE)
	$(CXX) $(CPPFLAGS) -c nsSIP.cpp $(GECKO_INCLUDES) $(GECKO_CONFIG_INCLUDE)
	$(CXX) $(CPPFLAGS) -c nsSIPModule.cpp  $(GECKO_INCLUDES) $(GECKO_CONFIG_INCLUDE)
	$(CXX) $(CPPFLAGS) -o $(TARGET) $(FILES) $(GECKO_LDFLAGS) $(GECKO_DEFINES) $(PJSIP_FLAGS) $(SSL_FLAGS)
 
header:
	$(GECKO_SDK_PATH)/bin/xpidl -m header -w -v -I $(GECKO_SDK_PATH)/idl/ -o nsISIP nsISIP.idl

xpt:
	$(GECKO_SDK_PATH)/bin/xpidl -m typelib -w -v -I $(GECKO_SDK_PATH)/idl/ -o nsISIP nsISIP.idl

clean: 
	rm -rf $(TARGET)
	rm -rf *.xpt
	rm -rf *.o 
	rm -rf *.so 
