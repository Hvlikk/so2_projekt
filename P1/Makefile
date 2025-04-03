# Makefile dla projektu C++

# Kompilator
CXX = g++

# Flagi kompilatora
CXXFLAGS = -Wall -g

# Pliki źródłowe
SRC = solution.cpp

# Nazwa pliku wykonywalnego
OUT = program2

# Reguła domyślna
all: $(OUT)

# Reguła kompilacji
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

# Usuwanie plików tymczasowych
clean:
	rm -f $(OUT)
