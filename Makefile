# Makefile multiplataforma para Raylib (macOS y Windows)
# Detectar sistema operativo
ifeq ($(OS),Windows_NT)
    # Configuración para Windows
    CXX = clang++
    TARGET = space_invaders.exe
    RM = rm -f

    # Librerías Windows
    LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm

    # Flags de compilación Windows (agregado -mwindows para ocultar terminal)
    CXXFLAGS = -std=c++17 -Wall -I. -I./include -mwindows

else
    # Configuración para macOS/Linux
    CXX = clang++
    TARGET = space_invaders
    RM = rm -f

    # Frameworks macOS
    LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

    # Flags de compilación macOS
    CXXFLAGS = -std=c++17 -Wall -I. -I./include
endif

# Encontrar automáticamente todos los archivos .cpp en el directorio
SOURCES = $(wildcard *.cpp)

# Regla principal
all: $(TARGET)

# Regla para compilar el ejecutable final directamente desde los fuentes
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

# Limpiar archivos compilados
clean:
	$(RM) $(TARGET)
	$(RM) *.o

.PHONY: all clean