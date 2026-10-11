#include "raylib.h"

int main(void) {
    InitWindow(1500, 600, "My Snake Game"); // Window Size + Title

// Snake Head (Position + Size) 
int snakeSize = 20; // grid size

//Define the grid size
int gridWidth = 1500 / snakeSize;
int gridHeight = 600 / snakeSize;

// Vector2 snakeHead = {740, 300}; // position of the snake
Vector2 snakeHead = {snakeSize * 37, snakeSize * 15};
Vector2 direction =  {snakeSize, 0}; // Initial direction (moving right)

// Snake body 
#define MAX_SNAKE_LENGTH 200

Vector2 snakeBody[MAX_SNAKE_LENGTH];
int snakeLength = 1; // tracks how many segments the snake currentlly has.
snakeBody[0] = snakeHead; 


// Food
Vector2 food = {200, 200};

// Score system (Demo 2)
int score = 0;
int level = 1; // Start at level 1
int gameFPS = 10; 

SetTargetFPS(gameFPS);  // Set initial speed 

// UI Height (Demo 2)
int uiHeight = 60; 

// gameOver flag
bool gameOver = false;

// Victory message flag
bool winGame = false;


    while (!WindowShouldClose()) {
        // Restart game if the player presses enter
        if ((gameOver || winGame) && IsKeyPressed(KEY_ENTER))
        {
            // Reset snake
            snakeHead = (Vector2){snakeSize * 37, snakeSize * 15};
            direction = (Vector2){snakeSize, 0};

            snakeLength = 1;
            snakeBody[0] = snakeHead;

            // Reset food
            food = (Vector2){200, 200};

            // Reset score and level
            score = 0;
            level = 1;
            gameFPS = 10;
            SetTargetFPS(gameFPS);

            //Reset game state
            gameOver = false;
            winGame = false;
        } 

        // Stop game if players wins or loses
        if (!gameOver && !winGame) {
            if (IsKeyPressed(KEY_RIGHT) && direction.x == 0) // can't go right if you already moving left
            {
                direction.x = snakeSize;
                direction.y = 0;
            }
        
            if (IsKeyPressed(KEY_LEFT) && direction.x == 0)
            {
                direction.x = -snakeSize;
                direction.y = 0;
            }
            
            if (IsKeyPressed(KEY_UP) && direction.y == 0)
            {
                direction.x = 0;
                direction.y = -snakeSize;
            }

            if (IsKeyPressed(KEY_DOWN) && direction.y == 0)
            {
                direction.x = 0;
                direction.y = snakeSize;
            }
            
            // Predict next position  
            Vector2 nextHead = {
                snakeHead.x + direction.x,
                snakeHead.y + direction.y
            };
        
            
            // Wall collision
            if (nextHead.x < 0 || nextHead.x >= 1500 ||
                nextHead.y < uiHeight || nextHead.y >= 600) 
            {
                gameOver = true; // Exit the while loop = game over
            }

            // Self-collision (head hits body)
            for (int i = 1; i < snakeLength; i++)
            {
                if (nextHead.x == snakeBody[i].x &&
                    nextHead.y == snakeBody[i].y)
                {
                    gameOver = true;
                }
            }

            // If no collision, 
            if (!gameOver)
            {
                // Move body from tail to head
                for (int i = snakeLength - 1; i > 0; i--)
                {
                    snakeBody[i] = snakeBody[i - 1];
                }

                // Update head
                snakeHead = nextHead;
                snakeBody[0] = snakeHead;

                // Food collision
                if (nextHead.x == food.x && nextHead.y == food.y)
                {
                    score++; // Increase score when food is eaten (Demo 2)
                    level = score / 5 + 1; // Every 5 points = new level (Demo 2)
                    gameFPS = 10 + (level - 1) * 2; // increase speed (Demo 2)
                    SetTargetFPS(gameFPS);

                    // Grow snake by 1 segment
                    if (snakeLength < MAX_SNAKE_LENGTH)
                    {
                        // Initialize new segment at the current tail position
                        snakeBody[snakeLength] = snakeBody[snakeLength - 1]; // This shows tail growth
                        snakeLength++;
                    }

                    // Check if snake reached the maximum length (Demo 2)
                    if (snakeLength == MAX_SNAKE_LENGTH)
                    {
                        winGame = true;
                    }
                
                    bool validPosition;

                    do {
                        validPosition = true;

                        food.x = GetRandomValue(0, gridWidth - 1) * snakeSize;
                        food.y = GetRandomValue(uiHeight / snakeSize, gridHeight - 1) * snakeSize;

                        // Check against entire snake body
                        for (int i = 0; i < snakeLength; i++)
                        {
                            if (food.x == snakeBody[i].x &&
                                food.y == snakeBody[i].y)
                            {
                                validPosition = false;
                                break;
                            }
                        }

                    } while (!validPosition);
                }
            }
        }

        // DRAWING
        BeginDrawing();
        ClearBackground(WHITE);

        // draw UI bar
        DrawRectangle(0, 0, 1500, uiHeight, LIGHTGRAY); // (Demo 2)
        DrawLine(0, uiHeight, 1500, uiHeight, BLACK); // (Demo 2)

        // UI message when player wins
        if (!winGame)
        {
            DrawText(TextFormat("Score: %d", score), 200, 20, 20, BLACK); // Display score on screen (Demo 2)

            DrawText(TextFormat("Level: %d", level), 700, 20, 20, BLACK); // Display level on screen (Demo 2)

            DrawText(TextFormat("Speed: %d FPS", gameFPS), 1150, 20, 20, BLACK); // Display FPS in the bar (Demo 2)
        }
        else
        {
            DrawText("CONGRATULATIONS! YOU REACHED MAX SNAKE LENGTH!", 350, 20, 20, DARKGREEN);
        }
        
        // Draw snake and food only if player has NOT won 
        if (!gameOver && !winGame)
        {
            for (int i = 0; i <  snakeLength; i++)
            {
                Color color = (i == 0) ? DARKGREEN : GREEN;
                DrawRectangle(
                    snakeBody[i].x,
                    snakeBody[i].y,
                    snakeSize,
                    snakeSize,
                    color
                );
            }

            DrawRectangle(food.x, food.y, snakeSize, snakeSize, RED); // Draw food
        }
        
        if (gameOver)
        {                                                                                                                                                                                                                                                                                                                                                                                                                                           
            DrawText("GAME OVER", 650, 250, 40, RED);
        }

        if (gameOver || winGame)
        {
            DrawText("Press ENTER to Restart", 640, 310, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Make my project better with the design 
// put the score that we get 
// if possible, will increase the maximum snake length 
// Add level (level 1 10fps slower, increase fps of the snake level 2)
