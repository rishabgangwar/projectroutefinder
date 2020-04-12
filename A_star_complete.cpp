#include <algorithm>  // for sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;
using std::abs;

// TODO: Add kStart and KFinish enumerators to the State enum.
enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}

//JUST read by 1.txt and enter it to the vector of vector
//INPUT:PATH String
//output: vector of vector
vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);//line by line enter it to the parse function it returns vector
      board.push_back(row);//it then enters the returned row to the (kempty in place of 0 and k0bstacle in place of 0);
    }
  }
  return board;// this is vector of vector kempty,kobstacle; control returns to main
}
//our grid is ready
//cursor is moved to main

/**
 * Compare the F values of two cells.
 */
bool Compare(const vector<int> a, const vector<int> b) {
  int f1 = a[2] + a[3]; // f1 = g1 + h1(
  int f2 = b[2] + b[3]; // f2 = g2 + h2
  return f1 > f2;//it return 1 for more cost fns;
}


/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);//it will compare on the bases of compare() and return existing paths in descending order;
}


// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}


/**
 * Check that a cell is valid: on the grid, not an obstacle, and clear.
 */
bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  bool on_grid_x = (x >= 0 && x < grid.size());
  bool on_grid_y = (y >= 0 && y < grid[0].size());
  if (on_grid_x && on_grid_y)
    return grid[x][y] == State::kEmpty;
  return false;
}


/**
 * Add a node to the open list and mark it as open.
 */
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid) {
  // Add node to open vector, and mark grid cell as closed.
  openlist.push_back(vector<int>{x, y, g, h});
  grid[x][y] = State::kClosed;
}


/**
 * Expand current nodes's neighbors and add them to the open list.
 */
void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid) {
  // Get current node's data.
  int x = current[0];
  int y = current[1];
  int g = current[2];

  // Loop through current node's potential neighbors.
  for (int i = 0; i < 4; i++) {
    int x2 = x + delta[i][0];
    int y2 = y + delta[i][1];

    // Check that the potential neighbor's x2 and y2 values are on the grid and not closed.
    if (CheckValidCell(x2, y2, grid)) {
      // Increment g value and add neighbor to open list.
      int g2 = g + 1;
      int h2 = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g2, h2, openlist, grid);
    }
  }
}


/**
 * Implementation of A* search algorithm
 */
 //INPUT:2d vector of satate
 //output: is also a 2d vector STATE(original grid containing obstacles only) and 2 int array;
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]) {
  // Create the vector of open nodes.
  vector<vector<int>> open {};//2d vector of INT

  // Initialize the starting node.
  int x = init[0];
  int y = init[1];
  int g = 0;
  int h = Heuristic(x, y, goal[0],goal[1]);
  AddToOpen(x, y, g, h, open, grid);

// problem1
  while (1) {
    // Get the next node
    CellSort(&open);
    auto current = open.back();
    open.pop_back();//pop back will spit out the function with least cost funs
    x = current[0];
    y = current[1];
    grid[x][y] = State::kPath;//this chosen coordinate is now a path:-)

    // Check if we're done.
    if (x == goal[0] && y == goal[1]) {
      // Set the init[0],init[1]=x,y (intial cordinates) grid cell to kStart, and
      // set the goal[0],goal[1]=x,y (final cordinates) grid cell to kFinish before returning the grid.
      grid[ init[0] ][ init[1] ] = State::kStart;
      grid[ goal[0] ][ goal[1] ] = State::kFinish;
      return grid;
    }
    // If we're not done, expand search to current node's neighbors.
    ExpandNeighbors(current, goal, open, grid);
  }

  // We've run out of new nodes to explore and haven't found a path.
  cout << "No path found!" << "\n";
  return std::vector<vector<State>>{};
}


string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰";
    case State::kPath: return "🚗";
    case State::kStart: return "🚦";
    case State::kFinish: return "🏁";
    default: return "   0";
  }
}


void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << "   "<<  CellString(board[i][j]);
    }
    cout <<"  "<< "\n";
  }
}



int main() {
  int init[2]{0, 0};// 1d vector initial points x,y;
  int goal[2]{4,5};// 1d vector having end point;
  auto board = ReadBoardFile("1.txt");//auto it self choses the data type;
  auto solution = Search(board, init, goal);
  PrintBoard(solution);

}
