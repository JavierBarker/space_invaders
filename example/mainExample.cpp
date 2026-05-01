#include "raylib.h"           // Librería para crear ventanas, dibujar y capturar input
#include "SpaceInvaders.h"    // Header propio: declara la función GameSpaceInvaders()
#include <string>             // Permite usar std::string para manejar texto
#include <iostream>           // Permite usar cout/cin (no se usa directamente aquí)

// =============================================================
//  FUNCIÓN: ToggleFullscreenWindow
//  Alterna entre pantalla completa y ventana normal.
//  Recibe el ancho y alto original para restaurar al salir
//  del modo fullscreen.
// =============================================================
void ToggleFullscreenWindow(int windowWidth, int windowHeight)
{
    static bool isFullscreen = false;
    // "static" significa que esta variable NO se reinicia cada vez
    // que se llama la función. Recuerda su valor entre llamadas.
    // Empieza en false (ventana normal).

    if (!isFullscreen)
    // Si actualmente está en ventana normal → pasar a fullscreen
    {
        int monitor = GetCurrentMonitor();
        // Obtiene el índice del monitor donde está la ventana.
        // Útil si el usuario tiene múltiples monitores.

        int monitorWidth  = GetMonitorWidth(monitor);
        int monitorHeight = GetMonitorHeight(monitor);
        // Obtiene la resolución real del monitor actual.
        // Ejemplo: 1920x1080

        SetWindowSize(monitorWidth, monitorHeight);
        // Redimensiona la ventana al tamaño exacto del monitor
        // ANTES de activar fullscreen para evitar glitches visuales.

        ToggleFullscreen();
        // Función de Raylib que activa el modo fullscreen.

        isFullscreen = true;
        // Marca que ahora estamos en fullscreen.
    }
    else
    // Si actualmente está en fullscreen → volver a ventana normal
    {
        ToggleFullscreen();
        // Sale del modo fullscreen.

        WaitTime(0.1);
        // Espera 0.1 segundos antes de redimensionar.
        // Sin esta pausa, el cambio de tamaño puede fallar porque
        // el sistema operativo no terminó de salir del fullscreen.

        SetWindowSize(windowWidth, windowHeight);
        // Restaura la ventana al tamaño original (800x600).

        isFullscreen = false;
        // Marca que ahora estamos en ventana normal.
    }
}

int main()
{
    const int ANCHO = 800;
    const int ALTO  = 600;
    // Tamaño inicial de la ventana en píxeles.
    // "const" significa que no pueden cambiar después de definirse.

    bool programaEjecutandose = true;
    // Controla el loop más externo.
    // Solo se pone en false cuando el usuario presiona ESC en el login,
    // lo que cierra el programa completamente.

    while (programaEjecutandose)
    // LOOP EXTERNO: permite volver al login desde el menú.
    // Cada vuelta recrea la ventana limpia para el login.
    {
        InitWindow(ANCHO, ALTO, "SPACE INVADERS - LOGIN");
        // Crea la ventana de 800x600 con ese título.
        // A diferencia del otro archivo que usaba (0,0), este
        // empieza con tamaño definido y luego la maximiza.

        SetTargetFPS(60);
        // El juego correrá a 60 fotogramas por segundo.
        // Raylib agrega pausas automáticas para mantener ese ritmo.

        SetWindowState(FLAG_WINDOW_RESIZABLE);
        // Permite redimensionar la ventana arrastrando sus bordes.

        MaximizeWindow();
        // Maximiza la ventana para que ocupe toda la pantalla
        // sin entrar en fullscreen real (la barra de tareas sigue visible).

        // Variables del login
        std::string usuario        = "";    // Texto escrito en el campo "Usuario"
        std::string contrasena     = "";    // Texto escrito en el campo "Contraseña"
        bool ingresandoUsuario     = true;  // true = foco en usuario, false = foco en contraseña
        bool loginExitoso          = false; // true = credenciales correctas, salir del login
        bool mostrarError          = false; // true = mostrar mensaje rojo de error
        float tiempoError          = 0;     // Segundos que lleva visible el mensaje de error
        bool salirDelLogin         = false; // true = ESC presionado, cerrar programa

        while (!WindowShouldClose() && !loginExitoso && !salirDelLogin)
        // LOOP DEL LOGIN: se repite mientras la ventana esté abierta,
        // el login no haya sido exitoso, y no se haya presionado ESC.
        {
            float deltaTime = GetFrameTime();
            // Tiempo en segundos desde el último frame (~0.016 a 60fps).
            // Se usa para que los temporizadores no dependan de los FPS.

            if (IsKeyPressed(KEY_ESCAPE))
            // IsKeyPressed = true SOLO el frame en que se presiona la tecla.
            {
                salirDelLogin        = true;  // Señal para salir del loop del login
                programaEjecutandose = false; // Señal para salir del loop externo también
                break;                        // Sale inmediatamente del loop del login
            }

            if (IsKeyPressed(KEY_F11))
            {
                ToggleFullscreenWindow(ANCHO, ALTO);
                // Alterna fullscreen pasando el tamaño original
                // para poder restaurarlo al salir del fullscreen.
            }

            if (mostrarError)
            // Solo entra aquí si hay un error activo mostrándose.
            {
                tiempoError += deltaTime;
                // Acumula el tiempo transcurrido desde que apareció el error.

                if (tiempoError >= 2.0f)
                // Si ya pasaron 2 segundos...
                {
                    mostrarError = false; // Oculta el mensaje
                    tiempoError  = 0;     // Resetea el contador
                }
            }

            int key = GetCharPressed();
            // Retorna el código ASCII del siguiente caracter en la cola.
            // Retorna 0 si no hay teclas pendientes.

            while (key > 0)
            // Procesa todos los caracteres pendientes en la cola.
            {
                if (ingresandoUsuario)
                {
                    if (usuario.length() < 20 && key >= 32 && key <= 125)
                    // Máximo 20 caracteres. ASCII 32-125 = caracteres imprimibles.
                        usuario += (char)key;
                    // Convierte el código ASCII a caracter y lo agrega al string.
                }
                else
                {
                    if (contrasena.length() < 20 && key >= 32 && key <= 125)
                        contrasena += (char)key;
                }
                key = GetCharPressed();
                // Obtiene el siguiente caracter. El while continúa
                // hasta que no quede ninguno (key == 0).
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                if (ingresandoUsuario && !usuario.empty())
                    usuario.pop_back();
                // pop_back() elimina el último caracter del string.
                // !usuario.empty() evita crashear si ya está vacío.

                else if (!ingresandoUsuario && !contrasena.empty())
                    contrasena.pop_back();
            }

            if (IsKeyPressed(KEY_TAB))
            {
                ingresandoUsuario = !ingresandoUsuario;
                // Invierte el bool: alterna el foco entre los dos campos.
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                if (ingresandoUsuario && !usuario.empty())
                {
                    ingresandoUsuario = false;
                    // Mueve el foco al campo contraseña.
                }
                else if (!ingresandoUsuario && !contrasena.empty())
                {
                    if (usuario == "admin" && contrasena == "1234")
                    // Validación hardcodeada. En el proyecto real
                    // esto se reemplazará con una llamada a la API REST.
                    {
                        loginExitoso = true;
                        // Login correcto: sale del loop del login.
                    }
                    else
                    {
                        mostrarError      = true;  // Activa el mensaje de error
                        usuario           = "";    // Limpia el campo usuario
                        contrasena        = "";    // Limpia el campo contraseña
                        ingresandoUsuario = true;  // Regresa el foco a usuario
                    }
                }
                else
                {
                    mostrarError = true;
                    // ENTER con campos vacíos también muestra error.
                }
            }

            BeginDrawing();
            // Inicia el frame. Todo lo dibujado va a un buffer interno.

            ClearBackground(BLACK);
            // Limpia el buffer con negro. Sin esto los frames se acumularían.

            int currentWidth = GetScreenWidth();
            // Ancho actual de la pantalla. Se recalcula cada frame para
            // que el centrado funcione si la ventana se redimensiona.

            int fontSizeTitulo = 50;
            int anchoTitulo    = MeasureText("SPACE INVADERS", fontSizeTitulo);
            // MeasureText devuelve cuántos píxeles de ancho ocupa ese texto.
            // Se usa para calcular la posición X que lo centra.

            DrawText("SPACE INVADERS", currentWidth/2 - anchoTitulo/2, 80, fontSizeTitulo, YELLOW);
            // Parámetros: texto, posX, posY, tamaño, color
            // currentWidth/2 - anchoTitulo/2 centra el texto horizontalmente.

            int fontSizeSub = 30;
            int anchoSub    = MeasureText("LOGIN SYSTEM", fontSizeSub);
            DrawText("LOGIN SYSTEM", currentWidth/2 - anchoSub/2, 150, fontSizeSub, LIGHTGRAY);
            // Subtítulo centrado 70px debajo del título.

            int marcoAncho = 500;
            int marcoAlto  = 180;
            DrawRectangleLines(currentWidth/2 - marcoAncho/2, 220, marcoAncho, marcoAlto, YELLOW);
            // Dibuja SOLO el borde de un rectángulo (sin relleno).
            // Enmarca el formulario de login, centrado en pantalla.

            // ── Campo USUARIO ──
            DrawText("USUARIO:", currentWidth/2 - 220, 260, 25, WHITE);
            // Etiqueta del campo a la izquierda del input.

            DrawRectangle(currentWidth/2 - 100, 255, 300, 35, DARKGRAY);
            // Fondo gris oscuro del input (con relleno).

            DrawRectangleLines(currentWidth/2 - 100, 255, 300, 35, ingresandoUsuario ? YELLOW : GRAY);
            // Borde: amarillo si el campo tiene el foco, gris si no.
            // El operador ternario (? :) elige el color según el estado.

            DrawText(usuario.c_str(), currentWidth/2 - 90, 260, 25, WHITE);
            // Dibuja el texto escrito. .c_str() convierte string a const char*.

            if (ingresandoUsuario && ((int)(GetTime() * 2) % 2 == 0))
            // Cursor parpadeante: GetTime()*2 da un valor que alterna entre
            // par e impar cada 0.5 segundos. %2==0 lo convierte en true/false.
                DrawText("_", currentWidth/2 - 90 + MeasureText(usuario.c_str(), 25), 260, 25, YELLOW);
            // Dibuja el guión bajo justo después del último caracter.

            // ── Campo CONTRASEÑA ──
            DrawText("CONTRASENA:", currentWidth/2 - 220, 320, 25, WHITE);
            DrawRectangle(currentWidth/2 - 100, 315, 300, 35, DARKGRAY);
            DrawRectangleLines(currentWidth/2 - 100, 315, 300, 35, !ingresandoUsuario ? YELLOW : GRAY);
            // Mismo patrón pero con ! invertido: este campo se resalta
            // cuando ingresandoUsuario es FALSE.

            std::string passOculta;
            passOculta.assign(contrasena.length(), '*');
            // Crea un string con tantos '*' como caracteres tiene la contraseña.
            // Ejemplo: "abc" → "***". Nunca se muestra la contraseña real.

            DrawText(passOculta.c_str(), currentWidth/2 - 90, 320, 25, WHITE);

            if (!ingresandoUsuario && ((int)(GetTime() * 2) % 2 == 0))
                DrawText("_", currentWidth/2 - 90 + MeasureText(passOculta.c_str(), 25), 320, 25, YELLOW);
            // Cursor parpadeante en contraseña. Usa passOculta para medir
            // el ancho (no la contraseña real).

            DrawText("TAB - Cambiar campo",     currentWidth/2 - 180, 420, 15, GRAY);
            DrawText("ENTER - Siguiente/Login", currentWidth/2 - 170, 440, 15, GRAY);
            DrawText("F11 - Pantalla completa", currentWidth/2 - 160, 460, 15, GRAY);
            DrawText("ESC - Cerrar programa",   currentWidth/2 - 150, 480, 15, RED);
            // Recordatorios de controles al pie del formulario.

            if (mostrarError)
            {
                DrawText("! CREDENCIALES INCORRECTAS !", currentWidth/2 - 170, 520, 20, RED);
                // Solo visible cuando mostrarError == true.
                // Desaparece automáticamente después de 2 segundos.
            }

            EndDrawing();
            // Muestra el buffer en pantalla. El jugador ve el frame aquí.
        }
        // ── Fin del loop del login ───────────────────────────────────

        if (loginExitoso && programaEjecutandose)
        // Solo entra si las credenciales fueron correctas
        // Y el usuario no cerró el programa con ESC.
        {
            int monedas         = 100;  // Tokens del jugador al iniciar sesión
            bool enMenu         = true; // Controla el loop externo del menú
            bool tokensAgotados = false;// true cuando monedas llega a 0
            bool salirAlLogin   = false;// true cuando elige "Salir al login"

            while (enMenu && !WindowShouldClose() && !salirAlLogin)
            // LOOP MENÚ+JUEGO: se repite mientras el jugador quiera
            // seguir jugando o volviendo al menú.
            {
                int opcionSeleccionada   = 0;     // 0=JUGAR, 1=SALIR AL LOGIN
                bool enJuego             = false; // true = ir al juego
                bool mostrarMensajeError = false; // true = tokens insuficientes
                float tiempoErrorMenu    = 0;     // Temporizador del error del menú

                Color COLOR_RETRO_AMARILLO = {255, 255, 100, 255};
                Color COLOR_RETRO_VERDE    = {100, 255, 100, 255};
                Color COLOR_RETRO_ROJO     = {255,  80,  80, 255};
                // Colores definidos como {R, G, B, A}.
                // A=255 significa completamente opaco (sin transparencia).

                SetWindowTitle("SPACE INVADERS - MENU CLASSIC");
                // Cambia el texto de la barra de título de la ventana.

                MaximizeWindow();
                // Maximiza de nuevo por si el juego cambió el tamaño.

                while (!WindowShouldClose() && !enJuego && !salirAlLogin)
                // LOOP DEL MENÚ: se repite hasta que el jugador
                // seleccione jugar o salir al login.
                {
                    float deltaTime = GetFrameTime();

                    if (IsKeyPressed(KEY_F11))
                        ToggleFullscreenWindow(ANCHO, ALTO);
                    // F11 también funciona en el menú.

                    if (IsKeyPressed(KEY_ESCAPE))
                    // ESC en el menú vuelve al LOGIN (no cierra el programa).
                    {
                        salirAlLogin = true;
                        break;
                    }

                    if (mostrarMensajeError)
                    {
                        tiempoErrorMenu += deltaTime;
                        if (tiempoErrorMenu >= 2.0f)
                        {
                            mostrarMensajeError = false;
                            tiempoErrorMenu     = 0;
                        }
                    }

                    if (monedas <= 0)
                    // Si se quedó sin tokens...
                    {
                        tokensAgotados     = true; // Bloquea la opción JUGAR
                        opcionSeleccionada = 1;    // Fuerza "Salir al login" seleccionado
                    }

                    if (!tokensAgotados)
                    // Solo navega el menú si tiene tokens.
                    {
                        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
                            opcionSeleccionada = (opcionSeleccionada - 1 + 2) % 2;
                        // Sube en el menú. El +2 y %2 hacen la navegación circular:
                        // si está en 0 y sube → (0-1+2)%2 = 1 (va al final).

                        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
                            opcionSeleccionada = (opcionSeleccionada + 1) % 2;
                        // Baja en el menú. %2 hace que al llegar a 2 vuelva a 0.
                    }

                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                    {
                        if (opcionSeleccionada == 0 && !tokensAgotados)
                        {
                            if (monedas >= 15)
                            {
                                monedas -= 15; // Descuenta el costo de la partida
                                enJuego  = true;// Sale del menú para ir al juego
                            }
                            else
                                mostrarMensajeError = true;
                        }
                        else if (opcionSeleccionada == 1)
                        {
                            salirAlLogin = true;
                            break;
                        }
                    }

                    BeginDrawing();
                    ClearBackground(BLACK);

                    int currentWidth  = GetScreenWidth();
                    int currentHeight = GetScreenHeight();

                    DrawRectangleLines(10, 10, currentWidth - 20, currentHeight - 20, COLOR_RETRO_AMARILLO);
                    // Borde decorativo exterior que rodea toda la pantalla.

                    DrawRectangleLines(12, 12, currentWidth - 24, currentHeight - 24, DARKGRAY);
                    // Segundo borde interior gris: crea efecto de doble marco retro.

                    const char *titulo  = "SPACE INVADERS";
                    int fontSizeTitulo  = 60;
                    int anchoTitulo     = MeasureText(titulo, fontSizeTitulo);

                    DrawText(titulo, currentWidth/2 - anchoTitulo/2,     80, fontSizeTitulo, COLOR_RETRO_VERDE);
                    DrawText(titulo, currentWidth/2 - anchoTitulo/2 + 2, 82, fontSizeTitulo, COLOR_RETRO_AMARILLO);
                    DrawText(titulo, currentWidth/2 - anchoTitulo/2,     80, fontSizeTitulo, COLOR_RETRO_VERDE);
                    // Efecto de sombra en 3 pasos:
                    // 1. Verde en posición normal
                    // 2. Amarillo desplazado 2px derecha y 2px abajo (la sombra)
                    // 3. Verde de nuevo encima para que quede en primer plano.
                    // Resultado visual: texto verde con borde amarillo.

                    DrawLine(currentWidth/2 - 200, 150, currentWidth/2 + 200, 150, COLOR_RETRO_AMARILLO);
                    // Línea decorativa horizontal centrada debajo del título.

                    const char *opciones[2];
                    if (tokensAgotados)
                    { opciones[0] = "JUGAR (BLOQUEADO)"; opciones[1] = "SALIR AL LOGIN"; }
                    else
                    { opciones[0] = "> JUGAR <";         opciones[1] = "SALIR AL LOGIN"; }

                    for (int i = 0; i < 2; i++)
                    // Dibuja cada opción del menú.
                    {
                        Color colorOpcion;
                        if (tokensAgotados && i == 0)
                            colorOpcion = DARKGRAY;
                        // JUGAR bloqueado aparece en gris oscuro.
                        else
                            colorOpcion = (i == opcionSeleccionada) ? COLOR_RETRO_AMARILLO : GRAY;
                        // Seleccionada = amarillo, no seleccionada = gris.

                        int sizeTexto  = (i == opcionSeleccionada && !(tokensAgotados && i == 0)) ? 35 : 30;
                        // Opción seleccionada se dibuja más grande (35 vs 30).
                        int anchoTexto = MeasureText(opciones[i], sizeTexto);

                        if (i == opcionSeleccionada && !(tokensAgotados && i == 0) && (int)(GetTime() * 5) % 2 == 0)
                        // Parpadeo en rojo: GetTime()*5 y %2 crean 5 parpadeos por segundo.
                            DrawText(opciones[i], currentWidth/2 - anchoTexto/2, 250 + i*70, sizeTexto, COLOR_RETRO_ROJO);
                        else
                            DrawText(opciones[i], currentWidth/2 - anchoTexto/2, 250 + i*70, sizeTexto, colorOpcion);
                        // 250 + i*70: opción 0 en Y=250, opción 1 en Y=320.
                    }

                    // Contador de tokens (esquina superior derecha)
                    std::string textoMonedas = tokensAgotados
                        ? "TOKENS: 0/100 - GAME OVER"
                        : "TOKENS: " + std::to_string(monedas) + "/100";
                    // std::to_string convierte el int monedas a texto concatenable.

                    int fontSizeMonedas = 25;
                    int anchoMonedas    = MeasureText(textoMonedas.c_str(), fontSizeMonedas);
                    DrawRectangle(currentWidth - anchoMonedas - 25, 25, anchoMonedas + 15, 35, DARKGRAY);
                    // Fondo gris detrás del contador, alineado a la derecha.
                    DrawRectangleLines(currentWidth - anchoMonedas - 25, 25, anchoMonedas + 15, 35, COLOR_RETRO_AMARILLO);
                    // Borde amarillo alrededor.
                    DrawText(textoMonedas.c_str(), currentWidth - anchoMonedas - 20, 30,
                             fontSizeMonedas, tokensAgotados ? RED : COLOR_RETRO_VERDE);
                    // Rojo si agotados, verde si hay tokens.

                    DrawCircle(currentWidth - anchoMonedas - 35, 42, 8, COLOR_RETRO_AMARILLO);
                    DrawCircle(currentWidth - anchoMonedas - 35, 42, 4, GOLD);
                    // Dos círculos concéntricos = ícono de moneda.
                    // Radio 8 amarillo (exterior) + radio 4 dorado (interior).

                    std::string textoCosto = "COSTO: 15 TOKENS";
                    int anchoCosto = MeasureText(textoCosto.c_str(), 18);
                    DrawText(textoCosto.c_str(), currentWidth - anchoCosto - 25, 70, 18, GRAY);
                    // Texto pequeño debajo del contador: costo por partida.

                    if (mostrarMensajeError)
                    {
                        const char *errorMsg = "!TOKENS INSUFICIENTES! Necesitas 15 tokens";
                        int anchoError = MeasureText(errorMsg, 25);
                        DrawRectangle(currentWidth/2 - anchoError/2 - 10, currentHeight - 80, anchoError + 20, 40, RED);
                        DrawText(errorMsg, currentWidth/2 - anchoError/2, currentHeight - 70, 25, WHITE);
                        // Fondo rojo + texto blanco: máximo contraste para el aviso.
                    }

                    if (tokensAgotados)
                    {
                        const char *gameOverMsg = "!!! GAME OVER - TOKENS AGOTADOS !!!";
                        int anchoGO = MeasureText(gameOverMsg, 30);
                        DrawRectangle(currentWidth/2 - anchoGO/2 - 10, currentHeight/2 - 50, anchoGO + 20, 50, RED);
                        DrawText(gameOverMsg, currentWidth/2 - anchoGO/2, currentHeight/2 - 30, 30, BLACK);
                        // GAME OVER centrado en pantalla, texto negro sobre fondo rojo.

                        const char *thanksMsg = "GRACIAS POR JUGAR";
                        int anchoThanks = MeasureText(thanksMsg, 20);
                        DrawText(thanksMsg, currentWidth/2 - anchoThanks/2, currentHeight/2 + 20, 20, YELLOW);
                    }

                    if (monedas < 15 && monedas > 0 && !tokensAgotados)
                    // Tiene tokens pero no suficientes para jugar (necesita 15).
                    {
                        const char *warnMsg = "!POCOS TOKENS! Gana partidas para recargar (necesitas 15 para jugar)";
                        int anchoWarn = MeasureText(warnMsg, 15);
                        DrawText(warnMsg, currentWidth/2 - anchoWarn/2, currentHeight - 100, 15, ORANGE);
                        // Aviso naranja cerca del fondo.
                    }

                    if (tokensAgotados)
                        DrawText("PRESIONA [ENTER] para SALIR AL LOGIN",
                                 currentWidth/2 - 250, currentHeight - 40, 15, DARKGRAY);
                    else
                        DrawText("USE [W/S] o [UP/DOWN]   [ENTER] para SELECCIONAR   [F11] FULLSCREEN   [ESC] LOGOUT",
                                 currentWidth/2 - 450, currentHeight - 40, 15, DARKGRAY);
                    // Instrucciones al pie. Cambian según el estado de tokens.

                    EndDrawing();
                }
                // ── Fin del loop del menú ────────────────────────────────

                if (enJuego)
                {
                    GameSpaceInvaders(monedas);
                    // Llama al juego completo de SpaceInvaders.cpp.
                    // Le pasa monedas por referencia para que el juego
                    // pueda modificarlo: sumar al ganar, restar al perder.
                    // El programa se "detiene" aquí hasta que el juego termine.

                    if (monedas <= 0)
                    {
                        tokensAgotados = true;
                        float tiempoMensaje = 0;

                        while (!WindowShouldClose() && tiempoMensaje < 3.0f)
                        // Muestra el aviso de tokens agotados por 3 segundos máximo.
                        {
                            tiempoMensaje += GetFrameTime();

                            BeginDrawing();
                            ClearBackground(BLACK);

                            int w = GetScreenWidth();
                            int h = GetScreenHeight();
                            const char *msg = "SE HAN AGOTADO LOS TOKENS";
                            int ancho = MeasureText(msg, 35);
                            DrawText(msg, w/2 - ancho/2, h/2 - 40, 35, RED);
                            DrawText("Presiona ESC para volver al menu",
                                     w/2 - MeasureText("Presiona ESC para volver al menu", 20)/2,
                                     h/2 + 20, 20, YELLOW);
                            EndDrawing();

                            if (IsKeyPressed(KEY_ESCAPE))
                                break;
                            // El jugador puede saltar los 3 segundos con ESC.
                        }
                    }
                }
            }
            // ── Fin del loop menú/juego ──────────────────────────────────

            if (salirAlLogin)
            {
                CloseWindow();
                // Destruye la ventana. El loop externo while(programaEjecutandose)
                // volverá a llamar InitWindow() para crear una ventana limpia
                // para el login en la siguiente iteración.
            }
        }
        else if (!loginExitoso && !salirDelLogin)
        // La ventana se cerró con la X del sistema operativo.
        {
            break;
            // Sale del loop externo: el programa termina.
        }
    }
    // ── Fin del loop externo ─────────────────────────────────────────

    if (IsWindowReady())
        CloseWindow();
    // Verificación de seguridad: cierra la ventana si todavía está abierta.
    // IsWindowReady() evita llamar CloseWindow() dos veces, lo que crashearía.

    return 0;
    // El programa terminó sin errores.
    // return 0 le indica al sistema operativo que todo fue exitoso.
}