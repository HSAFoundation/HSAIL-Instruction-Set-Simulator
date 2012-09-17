EXECUTABLE=hsailcs
SOURCES=hsailcs.cpp
DESTINATION=hsailcsbin
HSATESTS=$(DESTINATION)/hsailtests
HSASCRIPTS=hsailcsscripts
INCPATH=
LIBPATH=
LIBS=
HSAILTESTS=OverviewNDRangesWorkGroupsAndWorkItems VectorCopy Floating-PointSpecialFunctions
CC=g++
DEFINES=-D BUILDFORLINUX

# Only this make file in local directory
$(EXECUTABLE): $(SOURCES)
	$(CC) $(INCPATH) $(LIBPATH) -o $(EXECUTABLE) $(SOURCES) $(LIBS) $(DEFINES)

# Just removes local binary
clean:
	rm $(EXECUTABLE)

# Builds everything and puts it in the binary directory
all: $(SOURCES)
	if test -d $(DESTINATION); \
	then \
		echo $(DESTINATION) exists; \
	else \
		echo $(DESTINATION) does not exist; \
		mkdir -p $(DESTINATION); \
	fi
	$(CC) $(INCPATH) $(LIBPATH) -o $(DESTINATION)/$(EXECUTABLE) $(SOURCES) $(LIBS) $(DEFINES)
	for d in $(HSAILTESTS); \
	do \
		$(MAKE) --directory=$$d HSATESTS=$(HSATESTS); \
	done
	cp -r $(HSASCRIPTS) $(DESTINATION)/ 

# Removes all binaries from binary folder	
cleanall:
	rm -rf $(DESTINATION)

# Builds all binaries locally
alllocal:
	$(CC) $(INCPATH) $(LIBPATH) -o $(EXECUTABLE) $(SOURCES) $(LIBS) $(DEFINES)
	for d in $(HSAILTESTS); \
	do \
		$(MAKE) --directory=$$d local HSATESTS=$(HSATESTS); \
	done

# Cleans all local files
cleanlocal:
	for d in $(HSAILTESTS); \
	do \
		$(MAKE) --directory=$$d clean; \
	done
	rm $(EXECUTABLE)



