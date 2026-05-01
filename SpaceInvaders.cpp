#include "raylib.h"
#include "./headers/SpaceInvaders.h"
#include <string>

void GameSpaceInvaders(int& monedas) {
    SetWindowTitle("SPACE INVADERS - GAME");

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Estado del juego
    int puntuacion = 0;
    int monedasExtraGanadas = 0;
    bool mostrandoGameOver = false; // cambia a true para ir a game over
    bool victoria = false;          // cambia a true para ir a victoria
    bool volverAlMenu = false;

    // Loop principal
    while (!WindowShouldClose() && !volverAlMenu) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        // Dibujado
        BeginDrawing();
        ClearBackground(BLACK);

        // Pantalla de juego
        if (!mostrandoGameOver && !victoria) {
            //LOGICA DE JUEGO
            DrawText(TextFormat("SCORE: %06d", puntuacion), 10, 10, 20, WHITE);
            DrawText(TextFormat("MONEDAS: %d", monedas), screenWidth - 160, 10, 20, YELLOW);

            if (IsKeyPressed(KEY_G)) mostrandoGameOver = true;
            if (IsKeyPressed(KEY_V)) victoria = true;
        }

        // Pantalla de Game Over
        else if (mostrandoGameOver) {
            const char *mensaje = "GAME OVER";
            int ancho = MeasureText(mensaje, 50);
            DrawText(mensaje, screenWidth/2 - ancho/2, screenHeight/2 - 80, 50, RED);

            std::string scoreText = "PUNTUACION FINAL: " + std::to_string(puntuacion);
            int anchoScore = MeasureText(scoreText.c_str(), 25);
            DrawText(scoreText.c_str(), screenWidth/2 - anchoScore/2, screenHeight/2 - 20, 25, WHITE);

            std::string perdidaText = "MONEDAS RESTANTES: " + std::to_string(monedas);
            int anchoPerdida = MeasureText(perdidaText.c_str(), 20);
            DrawText(perdidaText.c_str(), screenWidth/2 - anchoPerdida/2, screenHeight/2 + 20, 20, RED);

            DrawText("PRESIONA [ENTER] PARA VOLVER AL MENU",
                screenWidth/2 - MeasureText("PRESIONA [ENTER] PARA VOLVER AL MENU", 20)/2,
                screenHeight/2 + 80, 20, YELLOW);

            DrawText("PRESIONA [ESC] PARA SALIR",
                screenWidth/2 - MeasureText("PRESIONA [ESC] PARA SALIR", 20)/2,
                screenHeight/2 + 120, 20, GRAY);

            if (IsKeyPressed(KEY_ENTER))
                volverAlMenu = true;
            if (IsKeyPressed(KEY_ESCAPE)) {
                CloseWindow();
                exit(0);
            }
        }

        // Pantalla de Victoria
        else if (victoria) {
            const char *mensaje = "VICTORIA!";
            int ancho = MeasureText(mensaje, 50);
            DrawText(mensaje, screenWidth/2 - ancho/2, screenHeight/2 - 80, 50, GREEN);

            std::string scoreText = "PUNTUACION FINAL: " + std::to_string(puntuacion);
            int anchoScore = MeasureText(scoreText.c_str(), 25);
            DrawText(scoreText.c_str(), screenWidth/2 - anchoScore/2, screenHeight/2 - 20, 25, WHITE);

            std::string recompensaText = "RECOMPENSA: +" + std::to_string(monedasExtraGanadas) + " MONEDAS";
            int anchoRecompensa = MeasureText(recompensaText.c_str(), 20);
            DrawText(recompensaText.c_str(), screenWidth/2 - anchoRecompensa/2, screenHeight/2 + 20, 20, YELLOW);

            std::string totalText = "TOTAL MONEDAS: " + std::to_string(monedas);
            int anchoTotal = MeasureText(totalText.c_str(), 20);
            DrawText(totalText.c_str(), screenWidth/2 - anchoTotal/2, screenHeight/2 + 50, 20, GREEN);

            DrawText("PRESIONA [ENTER] PARA VOLVER AL MENU",
                screenWidth/2 - MeasureText("PRESIONA [ENTER] PARA VOLVER AL MENU", 20)/2,
                screenHeight/2 + 80, 20, YELLOW);

            DrawText("PRESIONA [ESC] PARA SALIR",
                screenWidth/2 - MeasureText("PRESIONA [ESC] PARA SALIR", 20)/2,
                screenHeight/2 + 120, 20, GRAY);

            if (IsKeyPressed(KEY_ENTER))
                volverAlMenu = true;
            if (IsKeyPressed(KEY_ESCAPE)) {
                CloseWindow();
                exit(0);
            }
        }

        EndDrawing();
    }

    // Restaurar titulo al volver al menu
    SetWindowTitle("SPACE INVADERS - MENU CLASSIC");
    ClearBackground(BLACK);
}