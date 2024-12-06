# Compiler
CXX = g++

# Opțiuni de compilare
CXXFLAGS = -std=c++11 -pthread -Wall

# Fișierele sursă
SOURCES = test.cpp httpserver.cpp connection.cpp ThreadManager.cpp

# Fișierele obiect (generate automat din surse)
OBJECTS = $(SOURCES:.cpp=.o)

# Numele executabilului final
TARGET = server

# Regula implicită pentru compilare
all: $(TARGET)

# Regula de construire a executabilului
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) -lpugixml

# Regula pentru construirea fișierelor obiect
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regula pentru curățarea fișierelor generate
clean:
	rm -f $(TARGET) $(OBJECTS)
