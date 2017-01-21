include ../Makerules

GLSOURCES =	plyv.cc

OTHERSOURCES =	plycat.cc \
		plymunge.cc

OFILES = $(addprefix $(OBJDIR)/,$(GLSOURCES:.cc=.o) $(OTHERSOURCES:.cc=.o))
DESTDIR=../bin.$(UNAME)
GLPROGS = $(addsuffix $(EXE), $(addprefix $(DESTDIR)/, $(GLSOURCES:.cc=)))
OTHERPROGS = $(addsuffix $(EXE), $(addprefix $(DESTDIR)/, $(OTHERSOURCES:.cc=)))
default: $(DESTDIR) $(OTHERPROGS) $(GLPROGS)


$(DESTDIR) :
	-mkdir $@

$(GLPROGS) : $(DESTDIR)/%$(EXE) : $(OBJDIR)/%.o
	rm -f $@
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(GLLIBS) $(LIBS) -o $@

$(OTHERPROGS) : $(DESTDIR)/%$(EXE) : $(OBJDIR)/%.o
	rm -f $@
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

clean :
	-rm -f $(OFILES) $(OBJDIR)/Makedepend $(OBJDIR)/*.d
	-rm -rf $(OBJDIR)/ii_files
	-rmdir $(OBJDIR)

spotless : clean
	-rm -f $(GLPROGS) $(OTHERPROGS)
	-rmdir $(DESTDIR)

