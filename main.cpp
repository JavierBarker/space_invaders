#include "raylib.h"

int main() {
    // Inicializar ventana
    const int ANCHO  = 800;
    const int ALTO   = 600;
    InitWindow(ANCHO, ALTO, "Pelota Rebotando - Raylib");
    SetTargetFPS(60);

    // Estado de la pelota
    Vector2 posicion  = { ANCHO / 2.0f, ALTO / 2.0f };
    Vector2 velocidad = { 5.0f, 4.0f };
    float   radio     = 30.0f;
    Color   color     = RED;

    // Bucle principal
    while (!WindowShouldClose()) {

        // --- Actualizar ---
        posicion.x += velocidad.x;
        posicion.y += velocidad.y;

        // Rebotar en los bordes horizontales
        if (posicion.x + radio >= ANCHO || posicion.x - radio <= 0) {
            velocidad.x *= -1;
            color = { (unsigned char)GetRandomValue(50,255),
                      (unsigned char)GetRandomValue(50,255),
                      (unsigned char)GetRandomValue(50,255), 255 };
        }

        // Rebotar en los bordes verticales
        if (posicion.y + radio >= ALTO || posicion.y - radio <= 0) {
            velocidad.y *= -1;
            color = { (unsigned char)GetRandomValue(50,255),
                      (unsigned char)GetRandomValue(50,255),
                      (unsigned char)GetRandomValue(50,255), 255 };
        }

        // --- Dibujar ---
        BeginDrawing();
            ClearBackground(BLACK);

            DrawCircleV(posicion, radio, color);

            // Texto de ayuda
            DrawText("Presiona ESC para salir", 10, 10, 20, DARKGRAY);
            DrawText(TextFormat("X: %.0f  Y: %.0f", posicion.x, posicion.y),
                     10, 35, 18, GRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}