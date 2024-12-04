# Definirea variabilelor
CXX = g++                # Compilerul C++
SOURCES = test.cpp httpserver.cpp connection.cpp  # Fișierele sursă
TARGET = server           # Numele executabilului final

# Regula implicită (când se rulează `make` fără argumente)
all: $(TARGET)

# Cum să construim executabilul din sursele date
$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) -o $(TARGET)

# Regula pentru curățarea fișierelor generate
clean:
	rm -f $(TARGET) *.o
