#include "raylib.h"
#include "SpaceInvaders.h"
#include <vector>
#include <string>

struct Invasor {
    Vector2 posicion;
    bool activo;
    int tipo;
};

struct Disparo {
    Vector2 posicion;
    bool activo;
    bool esEnemigo;
};

void GameSpaceInvaders(int& monedas) {
    SetWindowTitle("SPACE INVADERS - GAME");
    
    // Jugador
    Vector2 jugadorPos = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() - 50 };
    float jugadorVelocidad = 5.0f;
    float jugadorAncho = 40;
    
    // Disparos
    std::vector<Disparo> disparos;
    float tiempoEntreDisparos = 0.3f;
    float tiempoUltimoDisparo = 0;
    
    // Invasores
    std::vector<Invasor> invasores;
    int filas = 5;
    int columnas = 11;
    float invasorVelocidad = 80.0f;
    float direccionX = 1.0f;
    float tiempoEntreMovimientos = 0.7f;
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Inicializar invasores
    int startX = (screenWidth - (columnas * 60)) / 2;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            invasores.push_back({
                { (float)(startX + j * 60), (float)(50 + i * 50) },
                true,
                i % 3
            });
        }
    }
    
    // Puntuación
    int puntuacion = 0;
    bool juegoTerminado = false;
    bool victoria = false;
    bool mostrandoGameOver = false;
    float temporizadorPausa = 0;
    bool enPausa = false;
    int tokensExtraGanados = 0;
    bool volverAlMenu = false;  // NUEVA VARIABLE PARA CONTROLAR EL RETORNO
    
    // Bucle del juego
    while (!WindowShouldClose() && !volverAlMenu) {  // MODIFICADO: añadir condición volverAlMenu
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        
        float deltaTime = GetFrameTime();
        
        // --- Actualización ---
        if (!victoria && !juegoTerminado && !enPausa) {
            // Movimiento del jugador
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                jugadorPos.x -= jugadorVelocidad;
            }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                jugadorPos.x += jugadorVelocidad;
            }
            
            if (jugadorPos.x < jugadorAncho/2) jugadorPos.x = jugadorAncho/2;
            if (jugadorPos.x > screenWidth - jugadorAncho/2) jugadorPos.x = screenWidth - jugadorAncho/2;
            
            // Disparo del jugador
            tiempoUltimoDisparo += deltaTime;
            if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && tiempoUltimoDisparo >= tiempoEntreDisparos) {
                disparos.push_back({ { jugadorPos.x, jugadorPos.y - 15 }, true, false });
                tiempoUltimoDisparo = 0;
            }
            
            // Movimiento de invasores
            static float movimientoTimer = 0;
            movimientoTimer += deltaTime;
            if (movimientoTimer >= tiempoEntreMovimientos) {
                movimientoTimer = 0;
                
                bool tocarBorde = false;
                for (auto& invasor : invasores) {
                    if (invasor.activo) {
                        invasor.posicion.x += invasorVelocidad * direccionX * deltaTime;
                        if (invasor.posicion.x <= 30 || invasor.posicion.x >= screenWidth - 30) {
                            tocarBorde = true;
                        }
                    }
                }
                
                if (tocarBorde) {
                    direccionX *= -1;
                    for (auto& invasor : invasores) {
                        if (invasor.activo) {
                            invasor.posicion.y += 20;
                        }
                    }
                }
            }
            
            // Disparos enemigos
            if (GetRandomValue(0, 100) < 2) {
                for (auto& invasor : invasores) {
                    if (invasor.activo && GetRandomValue(0, (int)invasores.size()) < 3) {
                        disparos.push_back({ { invasor.posicion.x, invasor.posicion.y + 20 }, true, true });
                        break;
                    }
                }
            }
            
            // Actualizar disparos
            for (int i = 0; i < (int)disparos.size(); i++) {
                if (disparos[i].esEnemigo) {
                    disparos[i].posicion.y += 4;
                } else {
                    disparos[i].posicion.y -= 7;
                }
                
                if (disparos[i].posicion.y < 0 || disparos[i].posicion.y > screenHeight) {
                    disparos.erase(disparos.begin() + i);
                    i--;
                    continue;
                }
                
                // Colisión con invasores
                if (!disparos[i].esEnemigo) {
                    for (int j = 0; j < (int)invasores.size(); j++) {
                        if (invasores[j].activo &&
                            CheckCollisionCircles(disparos[i].posicion, 5, 
                                                  invasores[j].posicion, 20)) {
                            invasores[j].activo = false;
                            disparos.erase(disparos.begin() + i);
                            puntuacion += 100;
                            i--;
                            break;
                        }
                    }
                }
                
                // Colisión con jugador
                if (i >= 0 && i < (int)disparos.size() && disparos[i].esEnemigo && 
                    CheckCollisionCircles(disparos[i].posicion, 5,
                                          { jugadorPos.x, jugadorPos.y }, 25)) {
                    juegoTerminado = true;
                }
            }
            
            // Verificar victoria
            bool todosMuertos = true;
            for (auto& invasor : invasores) {
                if (invasor.activo) {
                    todosMuertos = false;
                    break;
                }
            }
            if (todosMuertos) {
                victoria = true;
                tokensExtraGanados = (puntuacion / 100) * 3 + 10;
                monedas += tokensExtraGanados;
            }
            
            // Verificar invasores en el fondo
            for (auto& invasor : invasores) {
                if (invasor.activo && invasor.posicion.y >= screenHeight - 80) {
                    juegoTerminado = true;
                }
            }
        }
        
        // Manejar pausa después de Game Over o Victoria
        if ((juegoTerminado || victoria) && !mostrandoGameOver) {
            mostrandoGameOver = true;
            temporizadorPausa = 0;
            enPausa = true;
        }
        
        // Actualizar temporizador de pausa
        if (enPausa) {
            temporizadorPausa += deltaTime;
            if (temporizadorPausa >= 2.0f) {
                enPausa = false;
            }
        }
        
        // --- Dibujado ---
        BeginDrawing();
            ClearBackground(BLACK);
            
            if (!victoria && !juegoTerminado) {
                // Dibujar jugador
                DrawTriangle(
                    { jugadorPos.x, jugadorPos.y - 15 },
                    { jugadorPos.x - 20, jugadorPos.y + 10 },
                    { jugadorPos.x + 20, jugadorPos.y + 10 },
                    GREEN
                );
                
                // Dibujar invasores
                for (auto& invasor : invasores) {
                    if (invasor.activo) {
                        Color colorInvasor;
                        switch (invasor.tipo) {
                            case 0: colorInvasor = RED; break;
                            case 1: colorInvasor = ORANGE; break;
                            default: colorInvasor = YELLOW; break;
                        }
                        DrawRectangle(invasor.posicion.x - 15, invasor.posicion.y - 15, 
                                     30, 30, colorInvasor);
                        DrawRectangle(invasor.posicion.x - 10, invasor.posicion.y - 20, 
                                     20, 10, colorInvasor);
                    }
                }
                
                // Dibujar disparos
                for (auto& disparo : disparos) {
                    DrawCircle(disparo.posicion.x, disparo.posicion.y, 4, 
                              disparo.esEnemigo ? RED : WHITE);
                }
                
                // Mostrar puntuación y tokens
                DrawText(TextFormat("SCORE: %06d", puntuacion), 10, 10, 20, WHITE);
                DrawText(TextFormat("TOKENS: %d", monedas), screenWidth - 150, 10, 20, YELLOW);
            } 
            else if (enPausa) {
                const char *mensaje = juegoTerminado ? "GAME OVER" : "VICTORIA!";
                Color color = juegoTerminado ? RED : GREEN;
                int fontSize = 50;
                int ancho = MeasureText(mensaje, fontSize);
                DrawText(mensaje, screenWidth/2 - ancho/2, screenHeight/2 - 80, fontSize, color);
                
                std::string scoreText = "PUNTUACION FINAL: " + std::to_string(puntuacion);
                int fontSizeScore = 25;
                int anchoScore = MeasureText(scoreText.c_str(), fontSizeScore);
                DrawText(scoreText.c_str(), screenWidth/2 - anchoScore/2, screenHeight/2 - 20, fontSizeScore, WHITE);
                
                if (victoria) {
                    std::string tokensText = "HAS GANADO +" + std::to_string(tokensExtraGanados) + " TOKENS!";
                    int anchoTokens = MeasureText(tokensText.c_str(), 20);
                    DrawText(tokensText.c_str(), screenWidth/2 - anchoTokens/2, screenHeight/2 + 20, 20, YELLOW);
                } else {
                    std::string tokensText = "HAS PERDIDO 15 TOKENS";
                    int anchoTokens = MeasureText(tokensText.c_str(), 20);
                    DrawText(tokensText.c_str(), screenWidth/2 - anchoTokens/2, screenHeight/2 + 20, 20, RED);
                }
                
                std::string tiempoText = "REGRESANDO AL MENU EN " + std::to_string((int)(3 - temporizadorPausa)) + "...";
                int anchoTiempo = MeasureText(tiempoText.c_str(), 20);
                DrawText(tiempoText.c_str(), screenWidth/2 - anchoTiempo/2, screenHeight/2 + 80, 20, YELLOW);
            }
            else if (mostrandoGameOver && !enPausa) {
                const char *mensaje = juegoTerminado ? "GAME OVER" : "VICTORIA!";
                Color color = juegoTerminado ? RED : GREEN;
                int fontSize = 50;
                int ancho = MeasureText(mensaje, fontSize);
                DrawText(mensaje, screenWidth/2 - ancho/2, screenHeight/2 - 80, fontSize, color);
                
                std::string scoreText = "PUNTUACION FINAL: " + std::to_string(puntuacion);
                int fontSizeScore = 25;
                int anchoScore = MeasureText(scoreText.c_str(), fontSizeScore);
                DrawText(scoreText.c_str(), screenWidth/2 - anchoScore/2, screenHeight/2 - 20, fontSizeScore, WHITE);
                
                if (victoria) {
                    std::string recompensaText = "RECOMPENSA: +" + std::to_string(tokensExtraGanados) + " TOKENS";
                    int anchoRecompensa = MeasureText(recompensaText.c_str(), 20);
                    DrawText(recompensaText.c_str(), screenWidth/2 - anchoRecompensa/2, screenHeight/2 + 20, 20, YELLOW);
                    
                    std::string totalText = "TOTAL TOKENS: " + std::to_string(monedas);
                    int anchoTotal = MeasureText(totalText.c_str(), 20);
                    DrawText(totalText.c_str(), screenWidth/2 - anchoTotal/2, screenHeight/2 + 50, 20, GREEN);
                } else {
                    std::string perdidaText = "TOKENS RESTANTES: " + std::to_string(monedas);
                    int anchoPerdida = MeasureText(perdidaText.c_str(), 20);
                    DrawText(perdidaText.c_str(), screenWidth/2 - anchoPerdida/2, screenHeight/2 + 20, 20, RED);
                }
                
                DrawText("PRESIONA [ENTER] PARA VOLVER AL MENU", screenWidth/2 - MeasureText("PRESIONA [ENTER] PARA VOLVER AL MENU", 20)/2, screenHeight/2 + 80, 20, YELLOW);
                DrawText("PRESIONA [ESC] PARA SALIR", screenWidth/2 - MeasureText("PRESIONA [ESC] PARA SALIR", 20)/2, screenHeight/2 + 120, 20, GRAY);
                
                // MODIFICADO: usar volverAlMenu en lugar de break
                if (IsKeyPressed(KEY_ENTER)) {
                    volverAlMenu = true;  // Activar bandera para salir
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    CloseWindow();
                    exit(0);
                }
            }
            
        EndDrawing();
    }
    
    // Limpiar recursos
    disparos.clear();
    invasores.clear();
    
    // Restaurar el título de la ventana para el menú
    SetWindowTitle("SPACE INVADERS - MENU CLASSIC");
    ClearBackground(BLACK);
}