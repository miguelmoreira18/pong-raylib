#include <iostream>
#include <raylib.h>

using namespace std;

class Ball {
public:
    float x, y;
    float speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }
    void Update() {
        x += speed_x;
        y += speed_y;

        // checking collision with horizontal borders
        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
    }
};

class Paddle {
public:
    float x, y;
    int speed_y;
    float width, height;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }
    void Update() {
        if (y >= 0 && IsKeyDown(KEY_UP)) {
            y -= speed_y;
        }
        else if (y + height <= GetScreenHeight() && IsKeyDown(KEY_DOWN)) {
            y += speed_y;
        }
    }
};

class ComputerPaddle: public Paddle {
public:
    void Update(float ball_y) {
        y = ball_y - height/2;
    }
};

Ball ball;
Paddle player;
ComputerPaddle computer;

int main () {

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const char* pause_message = "Press Space to unpause";

    ball.radius = 15;
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.speed_x = 5;
    ball.speed_y = 5;

    player.width = 20;
    player.height = 100;
    player.x = 40;
    player.y = SCREEN_HEIGHT/2 - player.height/2;
    player.speed_y = 7;

    computer.width = 20;
    computer.height = 100;
    computer.x = SCREEN_WIDTH - 40;
    computer.y = SCREEN_HEIGHT/2 - computer.height/2;

    int player_points = 0;
    int computer_points = 0;

    bool game_paused = true;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);

    while (WindowShouldClose() == false){

        // check ball out of screen
        if (ball.x > SCREEN_WIDTH) {
            game_paused = true;
            player_points++;
            ball.x = SCREEN_WIDTH/2;
            ball.y = SCREEN_HEIGHT/2;
            ball.speed_x = 5;
            ball.speed_y = 5;
            computer.y = SCREEN_HEIGHT/2 - computer.height/2;
            player.y = SCREEN_HEIGHT/2 - player.height/2;
        } else if (ball.x < 0) {
            game_paused = true;
            computer_points++;
            ball.x = SCREEN_WIDTH/2;
            ball.y = SCREEN_HEIGHT/2;
            ball.speed_x = 5;
            ball.speed_y = 5;
            computer.y = SCREEN_HEIGHT/2 - computer.height/2;
            player.y = SCREEN_HEIGHT/2 - player.height/2;
        }

        // check collision with paddle
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
            ball.speed_x > 0 ? ball.speed_x += .1 : ball.speed_x -= .1;
            ball.speed_y > 0 ? ball.speed_y += .1 : ball.speed_y -= .1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{computer.x, computer.y, computer.width, computer.height})) {
            ball.speed_x *= -1;
            ball.speed_x > 0 ? ball.speed_x += .1 : ball.speed_x -= .1;
            ball.speed_y > 0 ? ball.speed_y += .1 : ball.speed_y -= .1;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            game_paused = !game_paused;
        }
        
        BeginDrawing();
            // Updating
            if (game_paused == false) {
                ball.Update();
                player.Update();
                computer.Update(ball.y);
            }

            // Drawing
            ClearBackground(BLACK);
            DrawLine(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, WHITE);
            DrawText(TextFormat("%i", player_points), 20, 20, 30, WHITE);
            DrawText(TextFormat("%i", computer_points), SCREEN_WIDTH - 30, 20, 30, WHITE);
            if (game_paused) {
                DrawText(pause_message, SCREEN_WIDTH/2 - MeasureText(pause_message, 30)/2, SCREEN_HEIGHT/2, 30, WHITE);
            }
            ball.Draw();
            player.Draw();
            computer.Draw();
        EndDrawing();
    }

    CloseWindow();
}