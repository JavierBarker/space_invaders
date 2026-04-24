#include "raylib.h"
#include <vector>

// Estructura para los invasores
struct Invasor {
    Vector2 posicion;
    bool activo;
    int tipo;
};

// Función principal del juego Space Invaders
void GameSpaceInvaders() {
    // Inicializar ventana
    const int ANCHO = 800;
    const int ALTO = 600;
    InitWindow(ANCHO, ALTO, "SPACE INVADERS - GAME");
    SetTargetFPS(60);
    
    // Jugador
    Vector2 jugadorPos = { (float)ANCHO / 2.0f, (float)ALTO - 50 };
    float jugadorVelocidad = 5.0f;
    float jugadorAncho = 40;
    // float jugadorAlto = 20;  // Comentado porque no se usa
    
    // Disparos
    struct Disparo {
        Vector2 posicion;
        bool activo;
        bool esEnemigo;
    };
    std::vector<Disparo> disparos;
    float tiempoEntreDisparos = 0.3f;
    float tiempoUltimoDisparo = 0;
    
    // Invasores
    std::vector<Invasor> invasores;
    int filas = 5;
    int columnas = 11;
    float invasorVelocidad = 50.0f;
    float direccionX = 1.0f;
    float tiempoMovimiento = 0;
    float tiempoEntreMovimientos = 0.5f;
    
    // Inicializar invasores (corregido: conversión explícita a float)
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            invasores.push_back({
                { (float)(50 + j * 60), (float)(50 + i * 50) },
                true,
                i % 3
            });
        }
    }
    
    // Puntuación
    int puntuacion = 0;
    bool juegoTerminado = false;
    bool victoria = false;
    
    // Bucle del juego
    while (!WindowShouldClose() && !juegoTerminado) {
        float deltaTime = GetFrameTime();
        
        // --- Actualización ---
        if (!victoria) {
            // Movimiento del jugador
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                jugadorPos.x -= jugadorVelocidad;
            }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                jugadorPos.x += jugadorVelocidad;
            }
            
            // Limitar movimiento del jugador
            if (jugadorPos.x < jugadorAncho/2) jugadorPos.x = jugadorAncho/2;
            if (jugadorPos.x > ANCHO - jugadorAncho/2) jugadorPos.x = ANCHO - jugadorAncho/2;
            
            // Disparo del jugador
            tiempoUltimoDisparo += deltaTime;
            if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && tiempoUltimoDisparo >= tiempoEntreDisparos) {
                disparos.push_back({ { jugadorPos.x, jugadorPos.y - 15 }, true, false });
                tiempoUltimoDisparo = 0;
            }
            
            // Movimiento de invasores
            tiempoMovimiento += deltaTime;
            if (tiempoMovimiento >= tiempoEntreMovimientos) {
                tiempoMovimiento = 0;
                
                // Mover todos los invasores
                bool tocarBorde = false;
                for (auto& invasor : invasores) {
                    if (invasor.activo) {
                        invasor.posicion.x += invasorVelocidad * direccionX * deltaTime * 2;
                        if (invasor.posicion.x <= 20 || invasor.posicion.x >= ANCHO - 20) {
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
            
            // Disparos enemigos (aleatorio)
            if (GetRandomValue(0, 100) < 3) {
                for (auto& invasor : invasores) {
                    if (invasor.activo && GetRandomValue(0, (int)invasores.size()) < 2) {
                        disparos.push_back({ { invasor.posicion.x, invasor.posicion.y + 20 }, true, true });
                        break;
                    }
                }
            }
            
            // Actualizar disparos
            for (int i = 0; i < (int)disparos.size(); i++) {
                if (disparos[i].esEnemigo) {
                    disparos[i].posicion.y += 5;
                } else {
                    disparos[i].posicion.y -= 8;
                }
                
                // Eliminar disparos fuera de pantalla
                if (disparos[i].posicion.y < 0 || disparos[i].posicion.y > ALTO) {
                    disparos.erase(disparos.begin() + i);
                    i--;
                    continue;
                }
                
                // Colisión con invasores (solo disparos del jugador)
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
                
                // Colisión con el jugador (disparos enemigos)
                if (i >= 0 && i < (int)disparos.size() && disparos[i].esEnemigo && 
                    CheckCollisionCircles(disparos[i].posicion, 5,
                                          { jugadorPos.x, jugadorPos.y }, 25)) {
                    juegoTerminado = true;
                }
            }
            
            // Verificar si todos los invasores están muertos
            bool todosMuertos = true;
            for (auto& invasor : invasores) {
                if (invasor.activo) {
                    todosMuertos = false;
                    break;
                }
            }
            if (todosMuertos) {
                victoria = true;
            }
            
            // Verificar si algún invasor llegó al fondo
            for (auto& invasor : invasores) {
                if (invasor.activo && invasor.posicion.y >= ALTO - 80) {
                    juegoTerminado = true;
                }
            }
        }
        
        // --- Dibujado ---
        BeginDrawing();
            ClearBackground(BLACK);
            
            if (!victoria && !juegoTerminado) {
                // Dibujar jugador (nave triangular)
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
                
                // Mostrar puntuación
                DrawText(TextFormat("SCORE: %06d", puntuacion), 10, 10, 20, WHITE);
            } else if (victoria) {
                DrawText("VICTORIA!", ANCHO/2 - 80, ALTO/2 - 30, 40, GREEN);
                DrawText("Press ESC to exit", ANCHO/2 - 100, ALTO/2 + 20, 20, GRAY);
                
                if (IsKeyPressed(KEY_ESCAPE)) {
                    break;
                }
            } else if (juegoTerminado) {
                DrawText("GAME OVER", ANCHO/2 - 100, ALTO/2 - 30, 40, RED);
                DrawText("Press ESC to exit", ANCHO/2 - 100, ALTO/2 + 20, 20, GRAY);
                
                if (IsKeyPressed(KEY_ESCAPE)) {
                    break;
                }
            }
            
        EndDrawing();
    }
    
    CloseWindow();
}