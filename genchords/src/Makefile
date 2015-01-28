########################################
#
# Genchords Makefile
#
# Veronika Zenz
#

CC = gcc
CCC = g++

# CFLAGS are specific to C.
override CFLAGS += -g -O2 

# CXXFLAGS are specific to C++.
override CXXFLAGS +=  -I../lib-src/libsndfile/src

# CPPFLAGS are for both C and C++.
override CPPFLAGS += -g -Wall -Iinclude  -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES 
#-DDEBUG

# Files that must exist in order to compile a C/C++ files
CDEPEND = 

# For precompiled header support
PRECOMP_CFLAGS = 

EXTRAOBJS =  ../lib-src/libsndfile/src/.libs/libsndfile.a
OPTOBJS =  
LIBS =  
EXTRATARGETS = ../labeldiff ../learnchords

########################################
# ALL OBJECT FILES

OBJDIR = obj

OBJS = \
	$(OBJDIR)/utilities.o \
	$(OBJDIR)/FFT.o \
	$(OBJDIR)/Key.o \
	$(OBJDIR)/Labelfile.o \
	$(OBJDIR)/PitchName.o \
	$(OBJDIR)/Sounddata.o \
	$(OBJDIR)/PCP.o \
	$(OBJDIR)/Chordtype.o	\
	$(OBJDIR)/Chordsequence.o \
	$(OBJDIR)/BeatDetection.o \
	


#
# We stick object files in separate directories for convenience
#

DIRS = \
	$(OBJDIR) 

########################################
# DEPENDENCIES

DEPS = $(OBJS:$(OBJDIR)/%.o=%.cpp) $(OPTOBJS:$(OBJDIR)/%.o=%.cpp)

########################################

all: $(DIRS) ../genchords $(EXTRATARGETS)

../genchords: $(DIRS)  genchords.cpp $(OBJS) $(EXTRAOBJS) $(OPTOBJS) Makefile
	$(CCC) -c $(CXXFLAGS) $(CPPFLAGS) $(PRECOMP_CFLAGS) genchords.cpp -o $(OBJDIR)/genchords.o	
	$(CCC) -o ../genchords $(OBJS) $(OPTOBJS) $(EXTRAOBJS) $(OBJDIR)/genchords.o
	
../learnchords: $(DIRS)  learnchords.cpp $(OBJS) $(EXTRAOBJS) $(OPTOBJS) Makefile
	$(CCC) -c $(CXXFLAGS) $(CPPFLAGS) $(PRECOMP_CFLAGS) learnchords.cpp -o $(OBJDIR)/learnchords.o	
	$(CCC) -o ../learnchords $(OBJS) $(OPTOBJS) $(EXTRAOBJS) $(OBJDIR)/learnchords.o	
	
../labeldiff: labeldiff.cpp Makefile Labelfile.h Labelfile.cpp
	$(CCC) -c $(CXXFLAGS) $(CPPFLAGS) $(PRECOMP_CFLAGS) labeldiff.cpp -o $(OBJDIR)/labeldiff.o
	$(CCC) -o ../labeldiff $(OBJDIR)/labeldiff.o $(OBJDIR)/Labelfile.o

clean:
	rm -rf obj/

#
# Rule for compiling C++ files
#

$(OPTOBJS) $(OBJS) $(LIBAUDACITY_OBJS): $(OBJDIR)/%.o: %.cpp %.h $(CONFIGHEADER) $(CDEPEND) Makefile
	$(CCC) -c $(CXXFLAGS) $(CPPFLAGS) $(PRECOMP_CFLAGS) $< -o $@

$(DIRS): %: 
	-test -d $@ || mkdir $@

