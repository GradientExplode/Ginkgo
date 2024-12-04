# ---------------------------------------
# add myself to dependency list
# ---------------------------------------
MAKEFILE_CHAIN += $(IDEA_BASE)/n4/pkg/MrServers/Gkg/makefiles/MakeGkgVxWorks.mk

# 
# GKG for VXWORKS
#
CPPI86FLAGS += -I$(IDEA_BASE)/n4/pkg/MrServers/Gkg
CPP603FLAGS += -I$(IDEA_BASE)/n4/pkg/MrServers/Gkg
CPP750FLAGS += -I$(IDEA_BASE)/n4/pkg/MrServers/Gkg
CPP751FLAGS += -I$(IDEA_BASE)/n4/pkg/MrServers/Gkg
