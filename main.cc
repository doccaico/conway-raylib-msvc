#include <random>

#include "raylib.h"


constexpr int cellSize = 2;
constexpr int initialCellCount = 55;
constexpr int fps = 30;
# if !defined(NDEBUG)
    constexpr char const* screenTitle = "Conway Cpp (Debug)";
    constexpr int screenWidth = 480 + (cellSize * 2);
    constexpr int screenHeight = 640 + (cellSize * 2);
    constexpr int colSize = screenWidth / cellSize;
    constexpr int rowSize = screenHeight / cellSize;
# else
    constexpr char const* screenTitle = "Conway Cpp";
    constexpr int screenWidth = 480;
    constexpr int screenHeight = 640;
    constexpr int colSize = screenWidth / cellSize + 2;
    constexpr int rowSize = screenHeight / cellSize + 2;
# endif


enum class State : bool {
    Empty = false,
    Exist = true,
};


State grid[rowSize][colSize];
std::random_device seed_gen;
std::mt19937_64 engine(seed_gen());


void initializeGrid() {
    for (int i = 0; i <= rowSize; ++i) {
        grid[0][i] = State::Empty;
    }
    for (int i = 0; i <= rowSize; ++i) {
        grid[rowSize - 1][i] = State::Empty;
    }
    for (int i = 0; i <= rowSize; ++i) {
        grid[i][0] = State::Empty;
    }
    for (int i = 0; i <= rowSize; ++i) {
        grid[i][colSize - 1] = State::Empty;
    }

    for (int i = 1; i < rowSize - 1; ++i) {
        for (int j = 1; j < colSize - 1; ++j) {
            grid[i][j] = (1 <= j && j <= initialCellCount) ? State::Exist : State::Empty;
        }
    }
}

void randomize() {
    for (int i = 1; i < rowSize - 1; ++i) {
        std::shuffle(&grid[i][1], &grid[i][colSize - 1], engine);
    }
}

void draw() {
# if !defined(NDEBUG)
    for (int i = 0; i <= rowSize; ++i) {
        for (int j = 0; j <= colSize; ++j) {
            if (grid[i][j] == State::Exist)
                DrawRectangle(cellSize * j, cellSize * i, cellSize, cellSize, BLACK);
            else if (grid[i][j] == State::Empty && (i == 0 || i == rowSize - 1 || j == 0 || j == colSize - 1))
                DrawRectangle(cellSize * j, cellSize * i, cellSize, cellSize, GREEN);
        }
    }
# else
    for (int i = 1; i <= rowSize - 2; ++i) {
        for (int j = 1; j <= colSize - 2; ++j) {
            if (grid[i][j] == State::Exist)
                DrawRectangle(cellSize * (j - 1), cellSize * (i - 1), cellSize, cellSize, BLACK);
        }
    }
# endif
}

void nextGeneration() {
  unsigned char neighbors[rowSize][colSize];

  for (int i = 1; i <= rowSize - 2; ++i) {
    for (int j = 1; j <= colSize - 2; ++j) {
      neighbors[i][j] =
        // top-left
        static_cast<unsigned char>(grid[i - 1][j - 1]) +
        // top-middle
        static_cast<unsigned char>(grid[i - 1][j]) +
        // top-right
        static_cast<unsigned char>(grid[i - 1][j + 1]) +
        // left
        static_cast<unsigned char>(grid[i][j - 1]) +
        // right
        static_cast<unsigned char>(grid[i][j + 1]) +
        // bottom-left
        static_cast<unsigned char>(grid[i + 1][j - 1]) +
        // bottom-middle
        static_cast<unsigned char>(grid[i + 1][j]) +
        // bottom-right
        static_cast<unsigned char>(grid[i + 1][j + 1]);
    }
  }

  for (int i = 1; i <= rowSize - 2; ++i) {
    for (int j = 1; j <= colSize - 2; ++j) {
      switch(neighbors[i][j]) {
        case 2:
          // Do nothing
          break;
        case 3:
          grid[i][j] = State::Exist;
          break;
        default:
          grid[i][j] = State::Empty;
          break;
      }
    }
  }
}

int main() {
    initializeGrid();

    randomize();

    InitWindow(screenWidth, screenHeight, screenTitle);
    SetTargetFPS(fps);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            initializeGrid();
            randomize();
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            draw();
            nextGeneration();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
