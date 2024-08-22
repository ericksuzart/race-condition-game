# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++20

# Directories
SRCDIR = src
BUILDDIR = build
INCDIR = include

# Executables
SERVER = server.exe
CLIENT = client.exe

# Source files
SRCSERVER = $(SRCDIR)/Server.cpp $(SRCDIR)/main_server.cpp
SRCCLIENT = $(SRCDIR)/Client.cpp $(SRCDIR)/main_client.cpp

# Object files
OBJSERVER = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCSERVER))
OBJCLIENT = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCCLIENT))

# Include directories
INCLUDES = -I$(INCDIR)

# Libraries
LIBS = -lSDL2
LIBS_PTHREAD = -pthread

# Targets
all: $(SERVER) $(CLIENT)

$(SERVER): $(OBJSERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS_PTHREAD)

$(CLIENT): $(OBJCLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BUILDDIR) $(SERVER) $(CLIENT)

# Phony targets
.PHONY: all clean
