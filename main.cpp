#include "raylib.h"
#include "SpaceInvaders.h"
#include <string>
#include <iostream>

void ToggleFullscreenWindow(int windowWidth, int windowHeight)
{
    static bool isFullscreen = false;
    
    if (!isFullscreen)
    {
        int monitor = GetCurrentMonitor();
        int monitorWidth = GetMonitorWidth(monitor);
        int monitorHeight = GetMonitorHeight(monitor);
        
        SetWindowSize(monitorWidth, monitorHeight);
        ToggleFullscreen();
        isFullscreen = true;
    }
    else
    {
        ToggleFullscreen();
        WaitTime(0.1);
        SetWindowSize(windowWidth, windowHeight);
        isFullscreen = false;
    }
}

int main()
{
    const int ANCHO = 800;
    const int ALTO = 600;
    
    bool programaEjecutandose = true;
    
    while (programaEjecutandose)
    {
        // === LOGIN ===
        InitWindow(ANCHO, ALTO, "SPACE INVADERS - LOGIN");
        SetTargetFPS(60);
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        MaximizeWindow();
        
        std::string usuario = "";
        std::string contrasena = "";
        bool ingresandoUsuario = true;
        bool loginExitoso = false;
        bool mostrarError = false;
        float tiempoError = 0;
        bool salirDelLogin = false;
        
        while (!WindowShouldClose() && !loginExitoso && !salirDelLogin)
        {
            float deltaTime = GetFrameTime();
            
            // SOLO desde el login se puede cerrar el programa con ESC
            if (IsKeyPressed(KEY_ESCAPE))
            {
                salirDelLogin = true;
                programaEjecutandose = false; // Esto cierra el programa completamente
                break;
            }
            
            if (IsKeyPressed(KEY_F11))
            {
                ToggleFullscreenWindow(ANCHO, ALTO);
            }
            
            if (mostrarError)
            {
                tiempoError += deltaTime;
                if (tiempoError >= 2.0f)
                {
                    mostrarError = false;
                    tiempoError = 0;
                }
            }
            
            int key = GetCharPressed();
            while (key > 0)
            {
                if (ingresandoUsuario)
                {
                    if (usuario.length() < 20 && key >= 32 && key <= 125)
                        usuario += (char)key;
                }
                else
                {
                    if (contrasena.length() < 20 && key >= 32 && key <= 125)
                        contrasena += (char)key;
                }
                key = GetCharPressed();
            }
            
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                if (ingresandoUsuario && !usuario.empty())
                    usuario.pop_back();
                else if (!ingresandoUsuario && !contrasena.empty())
                    contrasena.pop_back();
            }
            
            if (IsKeyPressed(KEY_TAB))
            {
                ingresandoUsuario = !ingresandoUsuario;
            }
            
            if (IsKeyPressed(KEY_ENTER))
            {
                if (ingresandoUsuario && !usuario.empty())
                {
                    ingresandoUsuario = false;
                }
                else if (!ingresandoUsuario && !contrasena.empty())
                {
                    if (usuario == "admin" && contrasena == "1234")
                    {
                        loginExitoso = true;
                    }
                    else
                    {
                        mostrarError = true;
                        usuario = "";
                        contrasena = "";
                        ingresandoUsuario = true;
                    }
                }
                else
                {
                    mostrarError = true;
                }
            }
            
            BeginDrawing();
            ClearBackground(BLACK);
            
            int currentWidth = GetScreenWidth();
            
            int fontSizeTitulo = 50;
            int anchoTitulo = MeasureText("SPACE INVADERS", fontSizeTitulo);
            DrawText("SPACE INVADERS", currentWidth/2 - anchoTitulo/2, 80, fontSizeTitulo, YELLOW);
            
            int fontSizeSub = 30;
            int anchoSub = MeasureText("LOGIN SYSTEM", fontSizeSub);
            DrawText("LOGIN SYSTEM", currentWidth/2 - anchoSub/2, 150, fontSizeSub, LIGHTGRAY);
            
            int marcoAncho = 500;
            int marcoAlto = 180;
            DrawRectangleLines(currentWidth/2 - marcoAncho/2, 220, marcoAncho, marcoAlto, YELLOW);
            
            DrawText("USUARIO:", currentWidth/2 - 220, 260, 25, WHITE);
            DrawRectangle(currentWidth/2 - 100, 255, 300, 35, DARKGRAY);
            DrawRectangleLines(currentWidth/2 - 100, 255, 300, 35, ingresandoUsuario ? YELLOW : GRAY);
            DrawText(usuario.c_str(), currentWidth/2 - 90, 260, 25, WHITE);
            if (ingresandoUsuario && ((int)(GetTime() * 2) % 2 == 0))
                DrawText("_", currentWidth/2 - 90 + MeasureText(usuario.c_str(), 25), 260, 25, YELLOW);
            
            DrawText("CONTRASENA:", currentWidth/2 - 220, 320, 25, WHITE);
            DrawRectangle(currentWidth/2 - 100, 315, 300, 35, DARKGRAY);
            DrawRectangleLines(currentWidth/2 - 100, 315, 300, 35, !ingresandoUsuario ? YELLOW : GRAY);
            std::string passOculta;
            passOculta.assign(contrasena.length(), '*'); 
            DrawText(passOculta.c_str(), currentWidth/2 - 90, 320, 25, WHITE);
            if (!ingresandoUsuario && ((int)(GetTime() * 2) % 2 == 0))
                DrawText("_", currentWidth/2 - 90 + MeasureText(passOculta.c_str(), 25), 320, 25, YELLOW);
            
            DrawText("TAB - Cambiar campo", currentWidth/2 - 180, 420, 15, GRAY);
            DrawText("ENTER - Siguiente/Login", currentWidth/2 - 170, 440, 15, GRAY);
            DrawText("F11 - Pantalla completa", currentWidth/2 - 160, 460, 15, GRAY);
            DrawText("ESC - Cerrar programa", currentWidth/2 - 150, 480, 15, RED); // CAMBIADO: ahora dice "Cerrar programa"
            
            if (mostrarError)
            {
                DrawText("! CREDENCIALES INCORRECTAS !", currentWidth/2 - 170, 520, 20, RED);
            }
            
            EndDrawing();
        }
        
        // Si el login fue exitoso y no se cerró el programa
        if (loginExitoso && programaEjecutandose)
        {
            int monedas = 100;
            bool enMenu = true;
            bool tokensAgotados = false;
            bool salirAlLogin = false;
            
            while (enMenu && !WindowShouldClose() && !salirAlLogin)
            {
                int opcionSeleccionada = 0;
                bool enJuego = false;
                bool mostrarMensajeError = false;
                float tiempoErrorMenu = 0;
                
                Color COLOR_RETRO_AMARILLO = {255, 255, 100, 255};
                Color COLOR_RETRO_VERDE = {100, 255, 100, 255};
                Color COLOR_RETRO_ROJO = {255, 80, 80, 255};
                
                // Cambiar título de la ventana existente
                SetWindowTitle("SPACE INVADERS - MENU CLASSIC");
                MaximizeWindow();
                
                while (!WindowShouldClose() && !enJuego && !salirAlLogin)
                {
                    float deltaTime = GetFrameTime();
                    
                    if (IsKeyPressed(KEY_F11)) 
                    {
                        ToggleFullscreenWindow(ANCHO, ALTO);
                    }
                    
                    // En el menú principal, ESC ahora vuelve al LOGIN (NO cierra el programa)
                    if (IsKeyPressed(KEY_ESCAPE))
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
                            tiempoErrorMenu = 0;
                        }
                    }
                    
                    // Si los tokens se agotaron, solo permitir seleccionar SALIR
                    if (monedas <= 0)
                    {
                        tokensAgotados = true;
                        opcionSeleccionada = 1;
                    }
                    
                    if (!tokensAgotados)
                    {
                        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) 
                            opcionSeleccionada = (opcionSeleccionada - 1 + 2) % 2;
                        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) 
                            opcionSeleccionada = (opcionSeleccionada + 1) % 2;
                    }
                    
                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                    {
                        if (opcionSeleccionada == 0 && !tokensAgotados)
                        {
                            if (monedas >= 15)
                            {
                                monedas -= 15;
                                enJuego = true;
                            }
                            else
                            {
                                mostrarMensajeError = true;
                            }
                        }
                        else if (opcionSeleccionada == 1)
                        {
                            salirAlLogin = true;  // Esto vuelve al login (NO cierra el programa)
                            break;
                        }
                    }
                    
                    BeginDrawing();
                    ClearBackground(BLACK);
                    
                    int currentWidth = GetScreenWidth();
                    int currentHeight = GetScreenHeight();
                    
                    DrawRectangleLines(10, 10, currentWidth - 20, currentHeight - 20, COLOR_RETRO_AMARILLO);
                    DrawRectangleLines(12, 12, currentWidth - 24, currentHeight - 24, DARKGRAY);
                    
                    const char *titulo = "SPACE INVADERS";
                    int fontSizeTitulo = 60;
                    int anchoTitulo = MeasureText(titulo, fontSizeTitulo);
                    DrawText(titulo, currentWidth / 2 - anchoTitulo / 2, 80, fontSizeTitulo, COLOR_RETRO_VERDE);
                    DrawText(titulo, currentWidth / 2 - anchoTitulo / 2 + 2, 82, fontSizeTitulo, COLOR_RETRO_AMARILLO);
                    DrawText(titulo, currentWidth / 2 - anchoTitulo / 2, 80, fontSizeTitulo, COLOR_RETRO_VERDE);
                    
                    DrawLine(currentWidth / 2 - 200, 150, currentWidth / 2 + 200, 150, COLOR_RETRO_AMARILLO);
                    
                    const char *opciones[2];
                    if (tokensAgotados)
                    {
                        opciones[0] = "JUGAR (BLOQUEADO)";
                        opciones[1] = "SALIR AL LOGIN";
                    }
                    else
                    {
                        opciones[0] = "> JUGAR <";
                        opciones[1] = "SALIR AL LOGIN";
                    }
                    
                    for (int i = 0; i < 2; i++)
                    {
                        Color colorOpcion;
                        if (tokensAgotados && i == 0)
                        {
                            colorOpcion = DARKGRAY;
                        }
                        else
                        {
                            colorOpcion = (i == opcionSeleccionada) ? COLOR_RETRO_AMARILLO : GRAY;
                        }
                        
                        int sizeTexto = (i == opcionSeleccionada && !(tokensAgotados && i == 0)) ? 35 : 30;
                        int anchoTexto = MeasureText(opciones[i], sizeTexto);
                        
                        if (i == opcionSeleccionada && !(tokensAgotados && i == 0) && (int)(GetTime() * 5) % 2 == 0)
                        {
                            DrawText(opciones[i], currentWidth / 2 - anchoTexto / 2, 250 + i * 70, sizeTexto, COLOR_RETRO_ROJO);
                        }
                        else
                        {
                            DrawText(opciones[i], currentWidth / 2 - anchoTexto / 2, 250 + i * 70, sizeTexto, colorOpcion);
                        }
                    }
                    
                    std::string textoMonedas;
                    if (tokensAgotados)
                    {
                        textoMonedas = "TOKENS: 0/100 - GAME OVER";
                    }
                    else
                    {
                        textoMonedas = "TOKENS: " + std::to_string(monedas) + "/100";
                    }
                    
                    int fontSizeMonedas = 25;
                    int anchoMonedas = MeasureText(textoMonedas.c_str(), fontSizeMonedas);
                    DrawRectangle(currentWidth - anchoMonedas - 25, 25, anchoMonedas + 15, 35, DARKGRAY);
                    DrawRectangleLines(currentWidth - anchoMonedas - 25, 25, anchoMonedas + 15, 35, COLOR_RETRO_AMARILLO);
                    DrawText(textoMonedas.c_str(), currentWidth - anchoMonedas - 20, 30, fontSizeMonedas, tokensAgotados ? RED : COLOR_RETRO_VERDE);
                    
                    DrawCircle(currentWidth - anchoMonedas - 35, 42, 8, COLOR_RETRO_AMARILLO);
                    DrawCircle(currentWidth - anchoMonedas - 35, 42, 4, GOLD);
                    
                    std::string textoCosto = "COSTO: 15 TOKENS";
                    int fontSizeCosto = 18;
                    int anchoCosto = MeasureText(textoCosto.c_str(), fontSizeCosto);
                    DrawText(textoCosto.c_str(), currentWidth - anchoCosto - 25, 70, fontSizeCosto, GRAY);
                    
                    if (mostrarMensajeError)
                    {
                        const char *errorMsg = "!TOKENS INSUFICIENTES! Necesitas 15 tokens";
                        int fontSizeError = 25;
                        int anchoError = MeasureText(errorMsg, fontSizeError);
                        DrawRectangle(currentWidth / 2 - anchoError / 2 - 10, currentHeight - 80, anchoError + 20, 40, RED);
                        DrawText(errorMsg, currentWidth / 2 - anchoError / 2, currentHeight - 70, fontSizeError, WHITE);
                    }
                    
                    if (tokensAgotados)
                    {
                        const char *gameOverMsg = "!!! GAME OVER - TOKENS AGOTADOS !!!";
                        int fontSizeGO = 30;
                        int anchoGO = MeasureText(gameOverMsg, fontSizeGO);
                        DrawRectangle(currentWidth / 2 - anchoGO / 2 - 10, currentHeight / 2 - 50, anchoGO + 20, 50, RED);
                        DrawText(gameOverMsg, currentWidth / 2 - anchoGO / 2, currentHeight / 2 - 30, fontSizeGO, BLACK);
                        
                        const char *thanksMsg = "GRACIAS POR JUGAR";
                        int fontSizeThanks = 20;
                        int anchoThanks = MeasureText(thanksMsg, fontSizeThanks);
                        DrawText(thanksMsg, currentWidth / 2 - anchoThanks / 2, currentHeight / 2 + 20, fontSizeThanks, YELLOW);
                    }
                    
                    if (monedas < 15 && monedas > 0 && !tokensAgotados)
                    {
                        const char *warnMsg = "!POCOS TOKENS! Gana partidas para recargar (necesitas 15 para jugar)";
                        int fontSizeWarn = 15;
                        int anchoWarn = MeasureText(warnMsg, fontSizeWarn);
                        DrawText(warnMsg, currentWidth / 2 - anchoWarn / 2, currentHeight - 100, fontSizeWarn, ORANGE);
                    }
                    
                    int fontSizeInst = 15;
                    if (tokensAgotados)
                    {
                        DrawText("PRESIONA [ENTER] para SALIR AL LOGIN",
                                 currentWidth / 2 - 250, currentHeight - 40, fontSizeInst, DARKGRAY);
                    }
                    else
                    {
                        DrawText("USE [W/S] o [UP/DOWN]   [ENTER] para SELECCIONAR   [F11] FULLSCREEN   [ESC] LOGOUT",
                                 currentWidth / 2 - 450, currentHeight - 40, fontSizeInst, DARKGRAY);
                    }
                    
                    EndDrawing();
                }
                
                if (enJuego)
                {
                    GameSpaceInvaders(monedas);
                    
                    if (monedas <= 0)
                    {
                        tokensAgotados = true;
                        // Mostrar mensaje de tokens agotados
                        float tiempoMensaje = 0;
                        while (!WindowShouldClose() && tiempoMensaje < 3.0f)
                        {
                            tiempoMensaje += GetFrameTime();
                            BeginDrawing();
                            ClearBackground(BLACK);
                            int w = GetScreenWidth();
                            int h = GetScreenHeight();
                            const char *msg = "SE HAN AGOTADO LOS TOKENS";
                            int fontSize = 35;
                            int ancho = MeasureText(msg, fontSize);
                            DrawText(msg, w/2 - ancho/2, h/2 - 40, fontSize, RED);
                            DrawText("Presiona ESC para volver al menu", w/2 - MeasureText("Presiona ESC para volver al menu", 20)/2, h/2 + 20, 20, YELLOW);
                            EndDrawing();
                            
                            if (IsKeyPressed(KEY_ESCAPE))
                            {
                                break;
                            }
                        }
                    }
                }
            }
            
            // Cerrar la ventana actual antes de volver a mostrar el login
            if (salirAlLogin)
            {
                CloseWindow();
            }
        }
        else if (!loginExitoso && !salirDelLogin)
        {
            // La ventana se cerró manualmente
            break;
        }
    }
    
    // Asegurar que la ventana esté cerrada
    if (IsWindowReady())
        CloseWindow();
    
    return 0;
}