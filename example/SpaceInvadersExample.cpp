// Inclusión de la librería principal para gráficos, controles y audio
#include "raylib.h"
// Inclusión de la cabecera específica del juego (probablemente contiene declaraciones compartidas)
#include "SpaceInvaders.h"
// Inclusión de la librería estándar para usar arreglos dinámicos (vectores)
#include <vector>
// Inclusión de la librería estándar para manejar cadenas de texto
#include <string>

// Estructura que define las propiedades de un enemigo (Invasor)
struct Invasor {
    Vector2 posicion; // Coordenadas (x, y) del invasor en la pantalla
    bool activo;      // Indica si el invasor sigue vivo (true) o fue destruido (false)
    int tipo;         // Identificador para cambiar el color o comportamiento del invasor
};

// Estructura que define las propiedades de un proyectil (Disparo)
struct Disparo {
    Vector2 posicion; // Coordenadas (x, y) del disparo en la pantalla
    bool activo;      // Indica si el disparo debe procesarse/dibujarse
    bool esEnemigo;   // true si lo disparó un invasor, false si lo disparó el jugador
};

// Función principal del juego, recibe una referencia a 'monedas' para poder modificarlas globalmente
void GameSpaceInvaders(int& monedas) {
    // Cambia el título de la ventana del juego
    SetWindowTitle("SPACE INVADERS - GAME");
    
    // --- Inicialización del Jugador ---
    // Coloca al jugador en el centro horizontal y cerca del borde inferior
    Vector2 jugadorPos = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() - 50 };
    float jugadorVelocidad = 5.0f; // Velocidad de movimiento lateral del jugador
    float jugadorAncho = 40;       // Anchura de la nave del jugador para calcular colisiones y bordes
    
    // --- Inicialización de Disparos ---
    std::vector<Disparo> disparos; // Lista dinámica que almacenará todos los disparos en pantalla
    float tiempoEntreDisparos = 0.3f; // Tiempo de enfriamiento (cooldown) del arma del jugador
    float tiempoUltimoDisparo = 0;    // Temporizador para saber cuándo se puede volver a disparar
    
    // --- Inicialización de Invasores ---
    std::vector<Invasor> invasores; // Lista dinámica de los enemigos
    int filas = 5;                  // Cantidad de filas de la formación enemiga
    int columnas = 11;              // Cantidad de columnas de la formación enemiga
    float invasorVelocidad = 80.0f; // Velocidad de movimiento de la horda
    float direccionX = 1.0f;        // Dirección actual: 1.0f (derecha), -1.0f (izquierda)
    float tiempoEntreMovimientos = 0.7f; // Cada cuánto tiempo (en segundos) avanza la horda
    
    // Guardamos las dimensiones actuales de la pantalla
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // --- Generación de la cuadrícula de invasores ---
    // Calcula dónde empezar a dibujar para que la horda quede centrada
    int startX = (screenWidth - (columnas * 60)) / 2;
    for (int i = 0; i < filas; i++) { // Bucle para las filas
        for (int j = 0; j < columnas; j++) { // Bucle para las columnas
            // Añade un nuevo invasor a la lista
            invasores.push_back({
                { (float)(startX + j * 60), (float)(50 + i * 50) }, // Calcula la posición X e Y según su fila/columna
                true, // Nace vivo
                i % 3 // Asigna un tipo (0, 1 o 2) dependiendo de la fila para darle variedad
            });
        }
    }
    
    // --- Variables de estado del juego ---
    int puntuacion = 0;             // Puntos acumulados en la partida
    bool juegoTerminado = false;    // Bandera de derrota
    bool victoria = false;          // Bandera de victoria
    bool mostrandoGameOver = false; // Indica si estamos en la pantalla de resultados
    float temporizadorPausa = 0;    // Tiempo que dura la pausa antes de mostrar la pantalla final
    bool enPausa = false;           // Congela el juego al ganar o perder
    int tokensExtraGanados = 0;     // Monedas obtenidas en esta partida
    bool volverAlMenu = false;      // Bandera para salir del bucle principal y volver al menú
    
    // --- Bucle principal del juego ---
    // Se ejecuta mientras no se cierre la ventana y no se haya pedido volver al menú
    while (!WindowShouldClose() && !volverAlMenu) { 
        // Actualiza las dimensiones por si la ventana cambió de tamaño
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        
        // Obtiene el tiempo transcurrido desde el último fotograma (para movimiento independiente de los FPS)
        float deltaTime = GetFrameTime();
        
        // ==========================================
        // --- FASE DE ACTUALIZACIÓN (LÓGICA) ---
        // ==========================================
        if (!victoria && !juegoTerminado && !enPausa) { // Solo actualiza si estamos jugando activamente
            
            // Movimiento del jugador hacia la izquierda (flecha o tecla A)
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                jugadorPos.x -= jugadorVelocidad;
            }
            // Movimiento del jugador hacia la derecha (flecha o tecla D)
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                jugadorPos.x += jugadorVelocidad;
            }
            
            // Limita el movimiento para que el jugador no salga por el lado izquierdo
            if (jugadorPos.x < jugadorAncho/2) jugadorPos.x = jugadorAncho/2;
            // Limita el movimiento para que el jugador no salga por el lado derecho
            if (jugadorPos.x > screenWidth - jugadorAncho/2) jugadorPos.x = screenWidth - jugadorAncho/2;
            
            // Incrementa el temporizador de disparo
            tiempoUltimoDisparo += deltaTime;
            // Si se presiona ESPACIO o ARRIBA y ya pasó el tiempo de enfriamiento...
            if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && tiempoUltimoDisparo >= tiempoEntreDisparos) {
                // ...crea un nuevo disparo en la punta de la nave del jugador
                disparos.push_back({ { jugadorPos.x, jugadorPos.y - 15 }, true, false });
                tiempoUltimoDisparo = 0; // Reinicia el temporizador de disparo
            }
            
            // --- Lógica de movimiento de la horda enemiga ---
            static float movimientoTimer = 0; // Variable estática que conserva su valor entre fotogramas
            movimientoTimer += deltaTime;
            if (movimientoTimer >= tiempoEntreMovimientos) { // Si es el momento de que la horda avance
                movimientoTimer = 0; // Reinicia el temporizador de la horda
                
                bool tocarBorde = false; // Bandera para saber si algún enemigo tocó el borde de la pantalla
                for (auto& invasor : invasores) { // Revisa cada enemigo
                    if (invasor.activo) {
                        // Lo mueve en el eje X según la velocidad y dirección
                        invasor.posicion.x += invasorVelocidad * direccionX * deltaTime;
                        // Si toca los márgenes laterales (30 px de margen)...
                        if (invasor.posicion.x <= 30 || invasor.posicion.x >= screenWidth - 30) {
                            tocarBorde = true; // ...avisa que la horda debe cambiar de dirección
                        }
                    }
                }
                
                // Si la horda tocó un borde...
                if (tocarBorde) {
                    direccionX *= -1; // Invierte la dirección (de derecha a izquierda o viceversa)
                    for (auto& invasor : invasores) { // Revisa a cada invasor
                        if (invasor.activo) {
                            invasor.posicion.y += 20; // Los hace descender 20 píxeles
                        }
                    }
                }
            }
            
            // --- Lógica de disparos enemigos ---
            // Hay un 2% de probabilidad por fotograma de que ocurra un ataque enemigo
            if (GetRandomValue(0, 100) < 2) {
                for (auto& invasor : invasores) {
                    // Si el invasor está vivo y tiene suerte (33% de probabilidad entre los vivos)...
                    if (invasor.activo && GetRandomValue(0, (int)invasores.size()) < 3) {
                        // ...crea un disparo enemigo desde su posición
                        disparos.push_back({ { invasor.posicion.x, invasor.posicion.y + 20 }, true, true });
                        break; // Solo dispara un enemigo por vez
                    }
                }
            }
            
            // --- Actualización de todos los disparos ---
            for (int i = 0; i < (int)disparos.size(); i++) { // Recorre la lista de disparos
                if (disparos[i].esEnemigo) {
                    disparos[i].posicion.y += 4; // Si es enemigo, baja a velocidad 4
                } else {
                    disparos[i].posicion.y -= 7; // Si es del jugador, sube a velocidad 7
                }
                
                // Si el disparo sale de la pantalla por arriba o abajo...
                if (disparos[i].posicion.y < 0 || disparos[i].posicion.y > screenHeight) {
                    disparos.erase(disparos.begin() + i); // ...lo elimina de la memoria
                    i--; // Ajusta el índice tras borrar un elemento para no saltarse el siguiente
                    continue; // Pasa al siguiente disparo
                }
                
                // --- Colisión: Disparo del jugador contra Invasores ---
                if (!disparos[i].esEnemigo) { // Solo revisa si es un disparo del jugador
                    for (int j = 0; j < (int)invasores.size(); j++) {
                        // Si el enemigo está vivo y hay intersección entre los radios del disparo y el enemigo...
                        if (invasores[j].activo &&
                            CheckCollisionCircles(disparos[i].posicion, 5, 
                                                  invasores[j].posicion, 20)) {
                            invasores[j].activo = false; // Muere el enemigo
                            disparos.erase(disparos.begin() + i); // Se destruye la bala
                            puntuacion += 100; // Suma 100 puntos
                            i--; // Ajusta el índice tras borrar la bala
                            break; // Deja de revisar este disparo
                        }
                    }
                }
                
                // --- Colisión: Disparo enemigo contra Jugador ---
                // Verifica que el índice sea válido, que sea bala enemiga y que colisione con el jugador
                if (i >= 0 && i < (int)disparos.size() && disparos[i].esEnemigo && 
                    CheckCollisionCircles(disparos[i].posicion, 5,
                                          { jugadorPos.x, jugadorPos.y }, 25)) {
                    juegoTerminado = true; // El jugador muere, fin del juego
                }
            }
            
            // --- Verificar condición de Victoria ---
            bool todosMuertos = true;
            for (auto& invasor : invasores) { // Revisa si queda algún enemigo vivo
                if (invasor.activo) {
                    todosMuertos = false;
                    break;
                }
            }
            if (todosMuertos) { // Si todos murieron...
                victoria = true; // ...activa bandera de victoria
                // Calcula el premio: base de 10 tokens + 3 extras por cada 100 puntos
                tokensExtraGanados = (puntuacion / 100) * 3 + 10;
                monedas += tokensExtraGanados; // Añade los tokens al total del jugador
            }
            
            // --- Verificar si los enemigos llegaron a la base (Derrota) ---
            for (auto& invasor : invasores) {
                if (invasor.activo && invasor.posicion.y >= screenHeight - 80) {
                    juegoTerminado = true; // Activa Game Over si cruzan la línea crítica
                }
            }
        } // Fin del bloque de actualización (solo activo si no está pausado/terminado)
        
        // --- Manejo de la transición tras terminar la partida ---
        if ((juegoTerminado || victoria) && !mostrandoGameOver) {
            mostrandoGameOver = true; // Evita entrar a este bloque múltiples veces
            temporizadorPausa = 0;    // Inicia cuenta regresiva
            enPausa = true;           // Activa la pausa dramática
        }
        
        // Cuenta el tiempo que la pantalla se queda congelada tras perder o ganar
        if (enPausa) {
            temporizadorPausa += deltaTime;
            if (temporizadorPausa >= 2.0f) { // Tras 2 segundos de pausa...
                enPausa = false; // ...quita la pausa para mostrar el menú de resultados
            }
        }
        
        // ==========================================
        // --- FASE DE DIBUJADO (RENDERIZADO) ---
        // ==========================================
        BeginDrawing(); // Prepara raylib para empezar a dibujar
            ClearBackground(BLACK); // Limpia la pantalla con color negro
            
            // --- Dibujado durante el juego ---
            if (!victoria && !juegoTerminado) {
                // Dibuja la nave del jugador como un triángulo verde
                DrawTriangle(
                    { jugadorPos.x, jugadorPos.y - 15 }, // Punta superior
                    { jugadorPos.x - 20, jugadorPos.y + 10 }, // Esquina inferior izquierda
                    { jugadorPos.x + 20, jugadorPos.y + 10 }, // Esquina inferior derecha
                    GREEN
                );
                
                // Dibuja la horda de invasores
                for (auto& invasor : invasores) {
                    if (invasor.activo) { // Solo dibuja los vivos
                        Color colorInvasor;
                        // Elige un color según su 'tipo' para diferenciarlos visualmente
                        switch (invasor.tipo) {
                            case 0: colorInvasor = RED; break;
                            case 1: colorInvasor = ORANGE; break;
                            default: colorInvasor = YELLOW; break;
                        }
                        // Dibuja el cuerpo (cuadrado) y la cabeza (rectángulo) del invasor
                        DrawRectangle(invasor.posicion.x - 15, invasor.posicion.y - 15, 30, 30, colorInvasor);
                        DrawRectangle(invasor.posicion.x - 10, invasor.posicion.y - 20, 20, 10, colorInvasor);
                    }
                }
                
                // Dibuja todos los disparos
                for (auto& disparo : disparos) {
                    // Dibuja círculos rojos si son del enemigo, o blancos si son del jugador
                    DrawCircle(disparo.posicion.x, disparo.posicion.y, 4, 
                              disparo.esEnemigo ? RED : WHITE);
                }
                
                // Dibuja la interfaz de usuario: Puntuación (esquina sup. izquierda) y Tokens (esquina sup. derecha)
                DrawText(TextFormat("SCORE: %06d", puntuacion), 10, 10, 20, WHITE);
                DrawText(TextFormat("TOKENS: %d", monedas), screenWidth - 150, 10, 20, YELLOW);
            } 
            // --- Dibujado durante la pausa dramática (justo al morir/ganar) ---
            else if (enPausa) {
                // Prepara el texto principal según el resultado
                const char *mensaje = juegoTerminado ? "GAME OVER" : "VICTORIA!";
                Color color = juegoTerminado ? RED : GREEN;
                int fontSize = 50;
                int ancho = MeasureText(mensaje, fontSize);
                // Dibuja "GAME OVER" o "VICTORIA" centrado
                DrawText(mensaje, screenWidth/2 - ancho/2, screenHeight/2 - 80, fontSize, color);
                
                // Dibuja la puntuación final debajo
                std::string scoreText = "PUNTUACION FINAL: " + std::to_string(puntuacion);
                int fontSizeScore = 25;
                int anchoScore = MeasureText(scoreText.c_str(), fontSizeScore);
                DrawText(scoreText.c_str(), screenWidth/2 - anchoScore/2, screenHeight/2 - 20, fontSizeScore, WHITE);
                
                // Muestra ganancia/pérdida de tokens provisional
                if (victoria) {
                    std::string tokensText = "HAS GANADO +" + std::to_string(tokensExtraGanados) + " TOKENS!";
                    int anchoTokens = MeasureText(tokensText.c_str(), 20);
                    DrawText(tokensText.c_str(), screenWidth/2 - anchoTokens/2, screenHeight/2 + 20, 20, YELLOW);
                } else {
                    std::string tokensText = "HAS PERDIDO 15 TOKENS"; // Nota: la lógica de perder 15 no está implementada arriba
                    int anchoTokens = MeasureText(tokensText.c_str(), 20);
                    DrawText(tokensText.c_str(), screenWidth/2 - anchoTokens/2, screenHeight/2 + 20, 20, RED);
                }
                
                // Muestra cuenta regresiva hacia el menú
                std::string tiempoText = "REGRESANDO AL MENU EN " + std::to_string((int)(3 - temporizadorPausa)) + "...";
                int anchoTiempo = MeasureText(tiempoText.c_str(), 20);
                DrawText(tiempoText.c_str(), screenWidth/2 - anchoTiempo/2, screenHeight/2 + 80, 20, YELLOW);
            }
            // --- Dibujado de la pantalla de resultados interactiva ---
            else if (mostrandoGameOver && !enPausa) {
                // Título principal
                const char *mensaje = juegoTerminado ? "GAME OVER" : "VICTORIA!";
                Color color = juegoTerminado ? RED : GREEN;
                int fontSize = 50;
                int ancho = MeasureText(mensaje, fontSize);
                DrawText(mensaje, screenWidth/2 - ancho/2, screenHeight/2 - 80, fontSize, color);
                
                // Puntuación
                std::string scoreText = "PUNTUACION FINAL: " + std::to_string(puntuacion);
                int fontSizeScore = 25;
                int anchoScore = MeasureText(scoreText.c_str(), fontSizeScore);
                DrawText(scoreText.c_str(), screenWidth/2 - anchoScore/2, screenHeight/2 - 20, fontSizeScore, WHITE);
                
                // Resumen detallado de la economía
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
                
                // Instrucciones para salir
                DrawText("PRESIONA [ENTER] PARA VOLVER AL MENU", screenWidth/2 - MeasureText("PRESIONA [ENTER] PARA VOLVER AL MENU", 20)/2, screenHeight/2 + 80, 20, YELLOW);
                DrawText("PRESIONA [ESC] PARA SALIR", screenWidth/2 - MeasureText("PRESIONA [ESC] PARA SALIR", 20)/2, screenHeight/2 + 120, 20, GRAY);
                
                // Detecta entrada del usuario en la pantalla de resultados
                if (IsKeyPressed(KEY_ENTER)) {
                    volverAlMenu = true;  // Rompe el bucle 'while' principal para volver
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    CloseWindow(); // Cierra raylib inmediatamente
                    exit(0);       // Cierra el programa entero
                }
            }
            
        EndDrawing(); // Manda a renderizar todo lo dibujado en este fotograma
    }
    
    // ==========================================
    // --- FASE DE LIMPIEZA Y SALIDA ---
    // ==========================================
    // Vacía los arreglos dinámicos para liberar memoria antes de salir de la función
    disparos.clear();
    invasores.clear();
    
    // Restaura el título original de la ventana antes de devolver el control al menú principal
    SetWindowTitle("SPACE INVADERS - MENU CLASSIC");
    // Pone el fondo en negro para que el menú no tenga un parpadeo visual del juego viejo
    ClearBackground(BLACK);
}