UNAME = $(shell uname)
ifeq ($(UNAME),IRIX64)
  ifneq ($(ABI),64)
        UNAME = IRIX
  endif
endif

ifeq ($(CROSS),Win32)
        UNAME = Win32
	EXE = .exe
endif


VERSION = 1.1

DIRECTORY = trimesh-$(VERSION)

FILES_FOR_SRCTAR = \
	$(DIRECTORY)/trimesh.html \
	$(DIRECTORY)/Make* \
	$(DIRECTORY)/include/*.h \
	$(DIRECTORY)/libsrc/Makefile \
	$(DIRECTORY)/libsrc/*.cc \
	$(DIRECTORY)/utilsrc/Makefile \
	$(DIRECTORY)/utilsrc/plycat.cc \
	$(DIRECTORY)/utilsrc/plymunge.cc \
	$(DIRECTORY)/utilsrc/plyv.cc

FILES_FOR_BINTAR = \
	$(FILES_FOR_SRCTAR) \
	$(DIRECTORY)/lib.$(UNAME)/libtrimesh.a \
	$(DIRECTORY)/bin.$(UNAME)/plycat$(EXE) \
	$(DIRECTORY)/bin.$(UNAME)/plymunge$(EXE) \
	$(DIRECTORY)/bin.$(UNAME)/plyv$(EXE)

default:
	$(MAKE) -C libsrc
	$(MAKE) -C utilsrc

clean:
	$(MAKE) -C libsrc clean
	$(MAKE) -C utilsrc clean

tar: default
	cd .. && tar zcvf trimesh-$(VERSION)-src.tar.gz $(FILES_FOR_SRCTAR)
	cd .. && tar zcvf trimesh-$(VERSION)-$(UNAME).tar.gz $(FILES_FOR_BINTAR)

