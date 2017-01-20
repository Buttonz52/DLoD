include ../Makerules

CCFILES =	trimesh_bounding.cc \
		trimesh_connectivity.cc \
		trimesh_normals.cc \
		trimesh_plyio.cc \
		trimesh_remove.cc \
		trimesh_tstrips.cc

OFILES = $(addprefix $(OBJDIR)/,$(CCFILES:.cc=.o))
DESTDIR = ../lib.$(UNAME)
LIB = $(DESTDIR)/libtrimesh.a
default: $(DESTDIR) $(LIB)


$(DESTDIR) :
	-mkdir $@

$(LIB): $(OFILES)
	rm -f $@
	$(AR) $@ $^

clean:
	-rm -f $(OFILES) $(OBJDIR)/Makedepend $(OBJDIR)/*.d
	-rm -rf $(OBJDIR)/ii_files
	-rmdir $(OBJDIR)

spotless: clean
	-rm -f $(LIB)
	-rmdir $(DESTDIR)

