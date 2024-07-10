#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using std::cerr;
using std::string;
using std::to_string;

const int SCREEN_WIDTH = 478;
const int SCREEN_HEIGHT = 670;
const int BLOCK_SIZE = 26;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int FALL_SPEED = 500;

const int FRAME_OFFSET_X = 45;
const int FRAME_OFFSET_Y = 25;

// showing different shapes of the tetris blocks or tetrominoes
enum TetrominoType {
    I, O, T, S, Z, J, L, TetrominoTypeCount
};

class Tetromino {
public:
    //initializes tetris blocks in the center 
    Tetromino(TetrominoType type = static_cast<TetrominoType>(rand() % TetrominoTypeCount))
        : type(type), x(4), y(0), rotation(0) {}

    void moveLeft() { x--; }
    void moveRight() { x++; }
    void moveDown() { y++; }
    void moveUp() { y--; }

    // this function choosing one of the side to rotate 
    void rotate() {
        rotation = (rotation + 1) % 4;
    }

    // this function is the one used in the colision check methods
    bool getShapeCell(int i, int j) const {
        return shapes[type][rotation][i][j];
    }

    // the functions is responsible for drawing the tetris blocks by using its matrix
    void render(SDL_Renderer* renderer, SDL_Texture* texture) const {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (shapes[type][rotation][i][j]) {
                    SDL_Rect blockRect = { FRAME_OFFSET_X + (x + j) * BLOCK_SIZE, FRAME_OFFSET_Y + (y + i) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
                    SDL_RenderCopy(renderer, texture, NULL, &blockRect);
                }
            }
        }
    }

    TetrominoType type;
    int x, y, rotation;

private:
    static const int shapes[TetrominoTypeCount][4][4][4];
};

//this array contains all tetromino types in four possible rotations
const int Tetromino::shapes[TetrominoTypeCount][4][4][4] = {
    // I
    {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}},
     // O
     {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
      // T
      {{{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
       {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
       {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
       {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}},
       // S
       {{{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}},
        // Z
        {{{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
         {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
         {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
         {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}},
         // J
         {{{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
          {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
          {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
          {{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}},
          // L
          {{{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
           {{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
           {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
           {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}}
};

class Board {
public:
    Board() {
        clear();
    }

    // converts all values of the playfield matrix to zeros, plus clearing it from figures (at the end of the game)
    void clear() {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board[y][x] = 0;
            }
        }
    }

    //checks if a cell on the playing field is occupied
    bool isOccupied(int x, int y) const {
        return board[y][x] != 0;
    }

    //makes a piece part of the playing field after it has reached the bottom border 
    void placeTetromino(const Tetromino& tetromino) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetromino.getShapeCell(i, j)) {
                    board[tetromino.y + i][tetromino.x + j] = 1;
                }
            }
        }
    }

    //each frame draws tetromines in place of occupied cells
    void render(SDL_Renderer* renderer, SDL_Texture* texture) const {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                if (board[y][x] != 0) {
                    SDL_Rect blockRect = { FRAME_OFFSET_X + x * BLOCK_SIZE, FRAME_OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
                    SDL_RenderCopy(renderer, texture, NULL, &blockRect);
                }
            }
        }
    }

    //methods for checking the status and modification of cells of the playfield
    void setCell(int x, int y, int value) {
        board[y][x] = value;
    }

    int getCell(int x, int y) const {
        return board[y][x];
    }

private:
    int board[BOARD_HEIGHT][BOARD_WIDTH];
};


class TetrisGame {
public:
    //it initializes class members when a new TetrisGame object is created.
    TetrisGame()
        : window(nullptr), renderer(nullptr), blockTexture(nullptr), backgroundTexture(nullptr), frameTexture(nullptr), tetromino(), board(), gameOver(false), lastFallTime(0) {}

    //checks if the sound works
    bool init() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << '\n';
            return false;
        }

        // load background music
        backgroundMusic = Mix_LoadMUS("music/Tetris_Theme.mp3");
        if (!backgroundMusic) {
            cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << '\n';
            return false;
        }

        // load game over sound effect
        gameOverSound = Mix_LoadWAV("music/Game_Over.wav");
        if (!gameOverSound) {
            cerr << "Failed to load game over sound effect! SDL_mixer Error: " << Mix_GetError() << '\n';
            return false;
        }

        // play background music
        Mix_PlayMusic(backgroundMusic, -1);

        //checks if the game can be finished
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
            return false;
        }

        //making a window for game
        window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
            return false;
        }

        //checks if the render works just fine
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << '\n';
            return false;
        }

        //checks if all the images can work
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << '\n';
            return false;
        }

        //making textures for the game
        blockTexture = loadTexture("images/tiles.png");
        if (!blockTexture) {
            cerr << "Failed to load block texture!" << '\n';
            return false;
        }

        frameTexture = loadTexture("images/frame.png");
        if (!frameTexture) {
            cerr << "Failed to load frame texture!" << '\n';
            return false;
        }

        backgroundTexture = loadTexture("images/background.png");
        if (!backgroundTexture) {
            cerr << "Failed to load background texture!" << '\n';
            return false;
        }

        //checking if the score can work and count
        if (TTF_Init() == -1) {
            cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << '\n';
            return false;
        }

        // checks if the text can work
        font = TTF_OpenFont("fonts/arial.ttf", 24);
        if (!font) {
            cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << '\n';
            return false;
        }

        score = 0;

        return true;
    }

    //this function loads an image from a file at the specified path and using it as the texture
    SDL_Texture* loadTexture(const string& path) {
        SDL_Texture* newTexture = nullptr;
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (!loadedSurface) {
            cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << '\n';
        }
        else {
            newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            if (!newTexture) {
                cerr << "Unable to create texture from " << path << "! SDL_Error: " << SDL_GetError() << '\n';
            }
            SDL_FreeSurface(loadedSurface);
        }
        return newTexture;
    }

    //stops the work of all the sdl libraries 
    void close() {
        Mix_FreeMusic(backgroundMusic);
        Mix_FreeChunk(gameOverSound);

        // Close SDL_mixer
        Mix_CloseAudio();

        SDL_DestroyTexture(blockTexture);
        blockTexture = nullptr;
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
        SDL_DestroyTexture(frameTexture);
        frameTexture = nullptr;

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        window = nullptr;
        renderer = nullptr;

        TTF_CloseFont(font);
        TTF_Quit();

        IMG_Quit();
        SDL_Quit();
    }

    //records all keystrokes and game exits
    void run() {
        bool quit = false;
        bool gameOverSoundPlayed = false; // flag to track if the game over sound has been played
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        tetromino.moveLeft();
                        if (isColliding()) tetromino.moveRight();
                        break;
                    case SDLK_RIGHT:
                        tetromino.moveRight();
                        if (isColliding()) tetromino.moveLeft();
                        break;
                    case SDLK_DOWN:
                        tetromino.moveDown();
                        if (isColliding()) tetromino.moveUp();
                        break;
                    case SDLK_UP:
                        tetromino.rotate();
                        if (isColliding()) tetromino.rotate();
                        break;
                    }
                }
            }

            // is responsible for dropping a piece before it collides with
            // obstacles and is also responsible for ending the game by checking
            // if the new piece does not collide with something immediately after appearing 
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastFallTime >= FALL_SPEED) {
                tetromino.moveDown();
                if (isColliding()) {
                    tetromino.moveUp();
                    board.placeTetromino(tetromino);
                    clearLines();
                    tetromino = Tetromino();
                    if (isColliding() && !gameOverSoundPlayed) { // Check if game over sound has not been played
                        gameOver = true;
                        // Play game over sound effect
                        Mix_PlayChannel(-1, gameOverSound, 0);
                        // Stop background music
                        Mix_HaltMusic();
                        gameOverSoundPlayed = true; // Set the flag to true after playing the sound
                    }
                }
                lastFallTime = currentTime;
            }

            render();
        }

        // Free resources
        Mix_FreeMusic(backgroundMusic);
        Mix_FreeChunk(gameOverSound);
    }



private:
       SDL_Window* window;              // Pointer to the SDL window used for displaying the game.
       SDL_Renderer* renderer;          // Pointer to the SDL renderer used for drawing graphics in the window.
       SDL_Texture* blockTexture;       // Texture for displaying tetromino blocks.
       SDL_Texture* backgroundTexture;  // Texture for displaying the background of the game board.
       SDL_Texture* frameTexture;       // Texture for displaying the frame around the game board.
       TTF_Font* font;                  // Font used for rendering text.
       Tetromino tetromino;             // Object representing the current tetromino on the game board.
       Board board;                     // Object representing the game board.
       Mix_Music* backgroundMusic;      // Pointer to the background music of the game.
       Mix_Chunk* gameOverSound;        // Pointer to the sound played when the game is over.
       bool gameOver;                   // Flag indicating whether the game is over.
       Uint32 lastFallTime;             // Time in milliseconds when the tetromino last moved down.
       int score;                       // Player's score.

    //checking for the collision
    bool isColliding() const {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetromino.getShapeCell(i, j)) {
                    int x = tetromino.x + j;
                    int y = tetromino.y + i;
                    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT || board.isOccupied(x, y)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    //checking about clearing lines
    void clearLines() {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            bool fullLine = true;
            for (int x = 0; x < BOARD_WIDTH; x++) {
                if (!board.isOccupied(x, y)) {
                    fullLine = false;
                    break;
                }
            }
            if (fullLine) {
                for (int yy = y; yy > 0; yy--) {
                    for (int xx = 0; xx < BOARD_WIDTH; xx++) {
                        board.setCell(xx, yy, board.getCell(xx, yy - 1));
                    }
                }
                for (int xx = 0; xx < BOARD_WIDTH; xx++) {
                    board.setCell(xx, 0, 0);
                }
                score += 100;
            }
        }
    }

    void render() {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        board.render(renderer, blockTexture);
        tetromino.render(renderer, blockTexture);

        SDL_RenderCopy(renderer, frameTexture, NULL, NULL);

        if (gameOver) {
            renderText("Game Over", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        }
        else {
            renderText("Score: " + to_string(score), FRAME_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 55, FRAME_OFFSET_Y);
        }

        SDL_RenderPresent(renderer);
    }

    void renderText(const string& text, int x, int y) {
        SDL_Color textColor = { 0, 0, 0, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
                SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
        else {
            cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
        }
    }
};

int main(int argc, char* args[]) {
    TetrisGame game;

    if (!game.init()) {
        cerr << "Failed to initialize!" << '\n';
    }
    else {
        game.run();
    }

    game.close();

    return 0;
}
