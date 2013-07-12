# ProofAna makefile
# B. Butler

# --- External configuration ----------------------------------
ARCH_LOC_1 := $(wildcard $(shell root-config --prefix)/etc/Makefile.arch)
ARCH_LOC_2 := $(wildcard $(shell root-config --prefix)/test/Makefile.arch)
ARCH_LOC_3 := $(wildcard $(shell root-config --prefix)/share/root/test/Makefile.arch)
ARCH_LOC_4 := $(wildcard $(shell root-config --prefix)/share/doc/root/test/Makefile.arch)

ifneq ($(strip $(ARCH_LOC_1)),)
  $(info Using $(ARCH_LOC_1))
  include $(ARCH_LOC_1)
else
  ifneq ($(strip $(ARCH_LOC_2)),)
    $(info Using $(ARCH_LOC_2))
    include $(ARCH_LOC_2)
  else
    ifneq ($(strip $(ARCH_LOC_3)),)
      $(info Using $(ARCH_LOC_3))
      include $(ARCH_LOC_3)
    else
      ifneq ($(strip $(ARCH_LOC_4)),)
         $(info Using $(ARCH_LOC_4))
         include $(ARCH_LOC_4)
      else
         $(error Could not find Makefile.arch! ROOT not set up?)
      endif
    endif
  endif
endif




# -------------------------------------------------------------
# General flags
# -------------------------------------------------------------
PACKAGE    = ProofAna
OUTPUTDIR  = lib

# glibc memcmp much faster (factor of 3) than gcc builtin for gcc 4.0-4.7, we use it heavily for comparing strings
ifneq ($(CXX),clang++)
	CXXFLAGS += -fno-builtin-memcmp
endif

INCLUDES += -I. -Icore -Ianalyses -Ieventbuilders -Iutils -I${ROOTSYS}/include -Ihelpers -Iutils/RootCore/include

# Need these to avoid loading dependent libraries when ROOT starts
LINKLIBS = -L${ROOTSYS}/lib -lHist -lMatrix -lTree -lNet -lProofPlayer -lProof -lRIO -lThread -lMathCore -lPhysics

# -------------------------------------------------------------
# ROOT Cint
# -------------------------------------------------------------
CINT       = ProofAnaCint
LDEFFILE   = lib/LinkDef.h
CINTFILE   = lib/$(CINT).cxx
CINTOBJ    = $(patsubst %.cxx,%.o,$(CINTFILE))

EBCXXLIST = $(filter-out %Cint.cxx,$(wildcard eventbuilders/EventBuilder_*.cxx))
ANACXXLIST = $(filter-out %Cint.cxx,$(wildcard analyses/Analysis_*.cxx))

EBHLIST = $(patsubst %.cxx,%.h,$(EBCXXLIST))
ANAHLIST = $(patsubst %.cxx,%.h,$(ANACXXLIST))

PAHLIST = $(wildcard core/*.h)

FECINTOBJLIST = $(patsubst %.h,%_Cint.o,$(EBHLIST))
ANACINTOBJLIST = $(patsubst %.h,%_Cint.o,$(ANAHLIST))

# -------------------------------------------------------------
# ProofAna source
# -------------------------------------------------------------
# add util subdirectories if needed
CXXLIST = $(wildcard core/*.cxx) $(EBCXXLIST) $(ANACXXLIST) $(wildcard helpers/*.cxx)
HLIST   = $(PAHLIST) $(EBHLIST) $(ANAHLIST)
OLIST   = $(patsubst %.cxx,%.o,$(CXXLIST)) $(FECINTOBJLIST) $(ANACINTOBJLIST) $(CINTOBJ)
DLIST   = $(patsubst %.h,%.d,$(HLIST))


# -------------------------------------------------------------
# Libraries
# -------------------------------------------------------------
LIBFILE  = $(OUTPUTDIR)/libProofAna.so
PARFILE  = $(OUTPUTDIR)/libProofAna.par
TMNFILE  = $(OUTPUTDIR)/libTriggerMenuNtuple.so
MCTFILE  = $(OUTPUTDIR)/libMctLib.so
QGFILE = $(OUTPUTDIR)/libqgTag.so
FASTJETFILE  = $(OUTPUTDIR)/libFastjet.so
FASTJETTOOLS = $(OUTPUTDIR)/libFastjetTools.so
FASTJETPLUGINFILE  = $(OUTPUTDIR)/libFastjetPlugins.so
ROOTCORE = $(OUTPUTDIR)/RootCore_compiled.lock

ifeq ($(PLATFORM),macosx)
EXTRALDFLAGS = -install_name @rpath/libProofAna.so
endif

# get libraries of ROOT
define ldlinksuffixROOT
   $(addsuffix $(LDLINKSUFFIX),$(Lib)) $(shell if [ "$(findstring -Ldlink2,$(OPTIONS))" ]; then echo $(addsuffix _pkgid_$(ROOTVER),$(Lib)); fi)
endef

# -------------------------------------------------------------
# Compilation
# -------------------------------------------------------------

default: shlib

# Rule to compile qgTag
$(QGFILE) :
	@echo "Compiling qgTag package"
	@make -C utils/qgTag -f makefile
	@ln -s ../utils/qgTag/lib/libqgTag.so lib/libqgTag.so

# Rule to compile TMNUtil
$(TMNFILE) :
	@echo "Compiling TMNUtil package"
	@make -C utils/TriggerMenuNtuple/cmd -f Makefile
	@ln -s ../utils/TriggerMenuNtuple/$(TMNFILE) lib/libTriggerMenuNtuple.so

# Rule to compile mctlib
$(MCTFILE) :
	@echo "Compiling mctlib package"
	@make -C utils/mctlib/ -f Makefile
	@ln -s ../utils/mctlib/libMctLib.so lib/libMctLib.so
	
# Rule to compile fastjet
$(FASTJETFILE) :
	@echo "Compiling fastjet package"
	@make -C utils/fastjet/ -f Makefile.ProofAna
ifeq ($(PLATFORM),macosx)
	@ln -s ../utils/fastjet/install-dir/lib/libfastjet.dylib lib/libFastjet.so
	@ln -s ../utils/fastjet/install-dir/lib/libfastjetplugins.dylib lib/libFastjetPlugins.so
	@ln -s ../utils/fastjet/install-dir/lib/libfastjettools.dylib lib/libFastjetTools.so
endif
ifneq ($(PLATFORM),macosx)
	@ln -s ../utils/fastjet/install-dir/lib/libfastjet.so lib/libFastjet.so
	@ln -s ../utils/fastjet/install-dir/lib/libfastjetplugins.so lib/libFastjetPlugins.so
	@ln -s ../utils/fastjet/install-dir/lib/libfastjettools.so lib/libFastjetTools.so
endif

# Rule to run RootCore compilation
$(ROOTCORE) :
	@echo "Compiling RootCore packages"
	@sh scripts/RootCore_compile.sh
	@echo "compiled" > $(ROOTCORE)

# Implicit rule to compile all classes
%.o : %.cxx
	@echo "Compiling $<"
	@$(CXX) -MD $(CXXFLAGS) $(INCLUDES) -c $< -o $*.o -g 
	
# Implicit rule to write ROOTCINT dictionary classes for Analyses, EventBuilders
%_Cint.cxx : %.h $(PAHLIST) %_LinkDef.h
	@echo "Running rootcint for $*"
	@$(ROOTSYS)/bin/rootcint -f $@ -c -p  $(CXXFLAGS) $(INCLUDES) $^

# Implicit rule to write LinkDef.h files for Analyses, EventBuilders	
%_LinkDef.h :
	@echo "Creating LinkDef file for class $(patsubst analyses/%,%,$(patsubst eventbuilders/%,%,$*))"
	@echo "#ifndef __$(patsubst analyses/%,%,$(patsubst eventbuilders/%,%,$*))__" >> $@
	@echo "#define __$(patsubst analyses/%,%,$(patsubst eventbuilders/%,%,$*))__" >> $@
	@echo "" >> $@
	@echo "#ifdef __CINT__" >> $@
	@echo "" >> $@
	@echo "#pragma link off all globals;" >> $@
	@echo "#pragma link off all defined_ines;" >> $@
	@echo "#pragma link off all functions;" >> $@
	@echo "#pragma link C++ nestedclasses;" >> $@
	@echo "" >> $@
	@echo "#pragma link C++ class $(patsubst analyses/%,%,$(patsubst eventbuilders/%,%,$*))+;" >> $@	
	@echo "" >> $@
	@echo "#endif" >> $@
	@echo "#endif" >> $@

# Rules to make ROOTCINT output files
$(CINTOBJ) : $(PAHLIST) $(LDEFFILE)
	@echo "Running rootcint for $(CINT)"
	@$(ROOTSYS)/bin/rootcint -f $(CINTFILE) -c -p  $(CXXFLAGS) $(INCLUDES) $(PAHLIST) $(LDEFFILE)
	@echo "Compiling $(CINTFILE)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(CINTFILE) -o $@

# Rule to combine objects into a shared library
$(LIBFILE): $(OLIST)
	@echo "Linking $(LIBFILE)"
	@mkdir -p $(OUTPUTDIR)
	@rm -f $(LIBFILE)
	@$(LD) $(CXXFLAGS) $(SOFLAGS) $(LINKLIBS) $(EXTRALDFLAGS) $(OLIST) -o $(LIBFILE) 

$(PARFILE): $(CXXLIST) $(HLIST) $(LDEFFILE)
	@echo "Making lib/libProofAna.par"
	@sh scripts/make_ProofAna_par.sh

-include $(DLIST)

ProofAnaCint: $(CINTOBJ) $(ANACINTOBJLIST) $(FECINTOBJLIST)
#shlib: solib parlib
shlib: solib
#solib: $(LIBFILE) $(QGFILE) $(TMNFILE) $(MCTFILE) $(FASTJETFILE) $(ROOTCORE)
solib: $(LIBFILE) $(ROOTCORE)
parlib: $(PARFILE)

clean:
	@rm -f */*.o */*.d */*Cint* */*.so */*.lock */*.par
	@sh scripts/RootCore_clean.sh
#	@make -C utils/TriggerMenuNtuple/cmd -f Makefile cleanall
#	@make -C utils/mctlib/ -f Makefile clean
#	@make -C utils/fastjet/ -f Makefile clean
#	@make -C utils/qgTag -f makefile clean

