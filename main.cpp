#include "raylib.h"
#include <string>

// Prototipo de la función del juego Space Invaders
void GameSpaceInvaders();

int main() {
    // Inicializar ventana
    const int ANCHO = 800;
    const int ALTO = 600;
    InitWindow(ANCHO, ALTO, "SPACE INVADERS - MENU CLASSIC");
    SetTargetFPS(60);
    
    // Variables del menú
    int monedas = 100;  // Máximo 100 monedas
    int opcionSeleccionada = 0;
    bool enJuego = false;
    bool mostrarMensajeError = false;
    float tiempoError = 0;
    
    // Colores retro
    Color COLOR_RETRO_AMARILLO = { 255, 255, 100, 255 };
    Color COLOR_RETRO_VERDE = { 100, 255, 100, 255 };
    Color COLOR_RETRO_ROJO = { 255, 80, 80, 255 };
    
    // Bucle principal del menú
    while (!WindowShouldClose() && !enJuego) {
        float deltaTime = GetFrameTime();
        
        // Actualizar temporizador de mensaje de error
        if (mostrarMensajeError) {
            tiempoError += deltaTime;
            if (tiempoError >= 2.0f) {
                mostrarMensajeError = false;
                tiempoError = 0;
            }
        }
        
        // Controles del menú
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            opcionSeleccionada = (opcionSeleccionada - 1 + 2) % 2;
        }
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            opcionSeleccionada = (opcionSeleccionada + 1) % 2;
        }
        
        // Seleccionar opción
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            if (opcionSeleccionada == 0) { // JUGAR
                if (monedas >= 5) {
                    monedas -= 5;  // Restar 5 monedas
                    enJuego = true; // Entrar al juego
                } else {
                    mostrarMensajeError = true;
                }
            } else if (opcionSeleccionada == 1) { // SALIR
                break;
            }
        }
        
        // Dibujar menú
        BeginDrawing();
            ClearBackground(BLACK);
            
            // --- Borde decorativo retro ---
            DrawRectangleLines(10, 10, ANCHO - 20, ALTO - 20, COLOR_RETRO_AMARILLO);
            DrawRectangleLines(12, 12, ANCHO - 24, ALTO - 24, DARKGRAY);
            
            // --- Título SPACE INVADERS estilo retro ---
            const char* titulo = "SPACE INVADERS";
            int anchoTitulo = MeasureText(titulo, 60);
            DrawText(titulo, ANCHO/2 - anchoTitulo/2, 80, 60, COLOR_RETRO_VERDE);
            
            // Efecto de brillo en el título
            DrawText(titulo, ANCHO/2 - anchoTitulo/2 + 2, 82, 60, COLOR_RETRO_AMARILLO);
            DrawText(titulo, ANCHO/2 - anchoTitulo/2, 80, 60, COLOR_RETRO_VERDE);
            
            // --- Línea decorativa ---
            DrawLine(ANCHO/2 - 200, 150, ANCHO/2 + 200, 150, COLOR_RETRO_AMARILLO);
            
            // --- Opciones del menú ---
            const char* opciones[] = { "> JUGAR <", "SALIR" };
            for (int i = 0; i < 2; i++) {
                Color colorOpcion = (i == opcionSeleccionada) ? COLOR_RETRO_AMARILLO : GRAY;
                int sizeTexto = (i == opcionSeleccionada) ? 35 : 30;
                int anchoTexto = MeasureText(opciones[i], sizeTexto);
                
                if (i == opcionSeleccionada) {
                    // Efecto de parpadeo para opción seleccionada
                    if ((int)(GetTime() * 5) % 2 == 0) {
                        DrawText(opciones[i], ANCHO/2 - anchoTexto/2, 250 + i * 70, sizeTexto, COLOR_RETRO_ROJO);
                    } else {
                        DrawText(opciones[i], ANCHO/2 - anchoTexto/2, 250 + i * 70, sizeTexto, colorOpcion);
                    }
                } else {
                    DrawText(opciones[i], ANCHO/2 - anchoTexto/2, 250 + i * 70, sizeTexto, colorOpcion);
                }
            }
            
            // --- Indicador de monedas (esquina derecha) ---
            std::string textoMonedas = "TOKENS: " + std::to_string(monedas) + "/100";
            int anchoMonedas = MeasureText(textoMonedas.c_str(), 25);
            DrawRectangle(ANCHO - anchoMonedas - 25, 25, anchoMonedas + 15, 35, DARKGRAY);
            DrawRectangleLines(ANCHO - anchoMonedas - 25, 25, anchoMonedas + 15, 35, COLOR_RETRO_AMARILLO);
            DrawText(textoMonedas.c_str(), ANCHO - anchoMonedas - 20, 30, 25, COLOR_RETRO_VERDE);
            
            // Mostrar icono de moneda
            DrawCircle(ANCHO - anchoMonedas - 35, 42, 8, COLOR_RETRO_AMARILLO);
            DrawCircle(ANCHO - anchoMonedas - 35, 42, 4, GOLD);
            
            // --- Mensaje de costo ---
            std::string textoCosto = "COSTO: 5 TOKENS";
            int anchoCosto = MeasureText(textoCosto.c_str(), 18);
            DrawText(textoCosto.c_str(), ANCHO - anchoCosto - 25, 70, 18, GRAY);
            
            // --- Mensaje de error (si no hay suficientes monedas) ---
            if (mostrarMensajeError) {
                const char* errorMsg = "!TOKENS INSUFICIENTES!";
                int anchoError = MeasureText(errorMsg, 25);
                DrawRectangle(ANCHO/2 - anchoError/2 - 10, ALTO - 80, anchoError + 20, 40, RED);
                DrawText(errorMsg, ANCHO/2 - anchoError/2, ALTO - 70, 25, WHITE);
            }
            
            // --- Instrucciones ---
            DrawText("USE [W/S] o [UP/DOWN]   [ENTER] para SELECCIONAR", 
                     ANCHO/2 - 280, ALTO - 40, 15, DARKGRAY);
            
            // --- Efecto de scanline retro ---
            for (int i = 0; i < ALTO; i += 4) {
                DrawLine(0, i, ANCHO, i, { 0, 0, 0, 20 });
            }
            
        EndDrawing();
    }
    
    // Cerrar ventana del menú
    CloseWindow();
    
    // Si se seleccionó jugar, ejecutar el juego de Space Invaders
    if (enJuego) {
        GameSpaceInvaders();
    }
    
    return 0;
}