CC=cc
EXECUTABLE=OverviewNDRangesWorkGroupsAndWorkItems
HSA_KERNEL=OverviewNDRangesWorkGroupsAndWorkItems.brigg
SOURCES=OverviewNDRangesWorkGroupsAndWorkItems.cpp
INCPATH=-I /opt/AMDAPP/include -I ../amdlibm-3-0-2/include
LIBPATH=-L /opt/AMDAPP/lib -L ../amdlibm-3-0-2/lib/static
LIBS=-l OpenCL -l amdlibm
HSATESTSDIR=../$(HSATESTS)
	
# Puts Binary in Binary folder
$(EXECUTABLE): $(SOURCES)
	if test -d $(HSATESTSDIR); \
	then \
		echo $(HSATESTSDIR) exists; \
	else \
		echo $(HSATESTSDIR) does not exist; \
		mkdir -p $(HSATESTSDIR); \
    fi
	$(CC) $(INCPATH) $(LIBPATH) -o $(HSATESTSDIR)/$(EXECUTABLE) $(SOURCES) $(LIBS)
	cp $(HSA_KERNEL) $(HSATESTSDIR)

# Binary stays in local folder
local:
	$(CC) $(INCPATH) $(LIBPATH) -o $(EXECUTABLE) $(SOURCES) $(LIBS)

# Removes local binary	
clean:
	rm $(EXECUTABLE)
