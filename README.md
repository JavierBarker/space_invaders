# space_invaders
Proyecto Final de Programacion 1

## Compilación

Este proyecto utiliza un Makefile multiplataforma para compilar en Windows y macOS.

### Requisitos Generales
- Compilador Clang++ instalado
- Librería Raylib instalada y configurada

### 🪟 Windows 11

#### Requisitos previos:
1. **MinGW-w64** (compilador g++ para Windows)
   - Descarga desde: https://github.com/msys2/msys2-installer/releases/download/2025-08-30/msys2-x86_64-20250830.exe
   - Agrega, sigue los pasos del Visual en https://code.visualstudio.com/docs/cpp/config-mingw#_prerequisites

2. **Configurar Make**
   - Abre la terminal MSYS2 CLANG64 y dirigete a la carpeta de tu proyecto (puedes colocar el cd y con un espacio, y arrastrar la carpeta)
   ```bash 
    cd 'C:/RUTA_DE_TU_PROYECTO'
   ```
3. Ejecuta los siguientes comandos
```bash
   # Clang: El Traductor (Compilador)
   pacman -S mingw-w64-clang-x86_64-clang

   # Toolchain: La Caja de Herramientas (ayuda a la compilacion)
   pacman -S mingw-w64-clang-x86_64-toolchain

   # Si no reconoce el clang --version puedes ejecutar lo siguiente 
   export PATH="/clang64/bin:$PATH"

   # Raylib: El "Motor" de piezas listas 
   pacman -S mingw-w64-clang-x86_64-raylib

```

### 🍎 macOS

#### Requisitos previos:
1. **Xcode Command Line Tools** (incluye clang++)
   ```bash
   xcode-select --install
   ```

2. **Homebrew** (gestor de paquetes)
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. **Raylib**
   ```bash
   brew install raylib
   ```

#### Verificación:
```bash
# Verificar compilador
clang++ --version

# Verificar Raylib
brew list raylib
```


## 🚀 Compilación y Ejecución

### Comandos Universales

```bash
# Compilar el proyecto
make

# Limpiar archivos compilados
make clean

# Limpiar todo (incluyendo backups)
make cleanall

# Mostrar información del proyecto
make info

# Compilar y ejecutar en un solo paso
make clean && make && ./game    # macOS
make clean && make && ./game.exe  # Windows
```


### Notas
- El Makefile detecta automáticamente el sistema operativo.
- Si hay errores de compilación, verifica que Raylib esté correctamente instalado y accesible.
