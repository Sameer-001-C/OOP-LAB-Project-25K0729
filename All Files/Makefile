CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

TARGET = minigit

SRCS =  main.cpp         \
        FileEntry.cpp    \
        CommitMetadata.cpp \
        Commit.cpp       \
        VCOperation.cpp  \
        Repository.cpp   \
        CLI.cpp

OBJS = $(SRCS:.cpp=.o)

# ---- Build ----
all: $(TARGET)
	@echo ""
	@echo "  Build successful! Run with: ./$(TARGET)"
	@echo ""

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---- Clean ----
clean:
	rm -f $(OBJS) $(TARGET) *_repo.dat

.PHONY: all clean
