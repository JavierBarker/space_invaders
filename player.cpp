#include "./headers/player.h"

Player::Player() {

    // Tamaño del jugador
    player.width = 80;
    player.height = 40;

    // Posición inicial centrada abajo
    player.x = GetScreenWidth() / 2 - player.width / 2;
    player.y = GetScreenHeight() - 120;

    // Velocidades
    normalSpeed = 5.0f;
    slowSpeed = 2.0f;

    slowMode = false;
}

void Player::Update(float time) {

    int cycle = (int)time % 23;

    // Cambia a modo lento después de cierto tiempo
    if (cycle >= 15) {
        slowMode = true;
    }
    else {
        slowMode = false;
    }

    float currentSpeed;

    if (slowMode) {
        currentSpeed = slowSpeed;
    }
    else {
        currentSpeed = normalSpeed;
    }

    // Movimiento izquierda
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.x -= currentSpeed;
    }

    // Movimiento derecha
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.x += currentSpeed;
    }

    // ===== LIMITES PERSONALIZADOS =====

    int margenIzquierdo = 450;
    int margenDerecho = 450;

    // Limite izquierdo
    if (player.x < margenIzquierdo)
        player.x = margenIzquierdo;

    // Limite derecho
    if (player.x + player.width > GetScreenWidth() - margenDerecho)
        player.x = GetScreenWidth() - margenDerecho - player.width;
}

void Player::Draw(float time) {

int margenIzquierdo = 450;
    int margenDerecho = 450;

    DrawLine(
    margenIzquierdo,
    0,
    margenIzquierdo,
    GetScreenHeight(),
    ORANGE
);

DrawLine(
    GetScreenWidth() - margenDerecho,
    0,
    GetScreenWidth() - margenDerecho,
    GetScreenHeight(),
    ORANGE
);

    // Dibujar jugador
    DrawRectangleRec(player, BLUE);

    int cycle = (int)time % 23;

    int remaining;

    if (slowMode) {
        remaining = 23 - cycle;
    }
    else {
        remaining = 15 - cycle;
    }

    // ===== TEXTO DERECHA =====

    int xTexto = GetScreenWidth() - 180;

    DrawText(
        TextFormat("Tiempo: %.0f s", time),
        xTexto,
        40,
        20,
        PURPLE
    );

    // ===== MODO LENTO =====

    if (slowMode) {

        DrawText(
            TextFormat("Lento: %d s", remaining),
            xTexto,
            65,
            20,
            RED
        );

        DrawText(
            "MODO LENTO",
            10,
            40,
            30,
            RED
        );
    }

    // ===== VELOCIDAD NORMAL =====

    else {

        DrawText(
            TextFormat("Normal: %d s", remaining),
            xTexto,
            65,
            20,
            DARKGREEN
        );

        DrawText(
            "VELOCIDAD NORMAL",
            10,
            40,
            30,
            DARKGREEN
        );
    }
}