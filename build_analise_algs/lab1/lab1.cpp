#include <iostream>
#include <vector>
#include <cmath>
#include <stack>

#define DEBUG true

#define SQUARE_PLACEMENT_MESSAGE "Placing square"
#define SQUARE_REMOVAL_MESSAGE "Removing square"
#define TABLETOP_STATE_MESSAGE "Current tabletop state"
#define BACKTRACKING_MESSAGE "Backtracking to previous step"

/*
Struct representing a square
- x and y -- top left corner
- size -- size of the side of the square
*/
struct Square
{
    int x;
    int y;
    int size;
};

/*
Struct representing a step in the partial solution
- firstFreeRow -- the row where the first free cell was found
- computed -- flag that shows if the square placement for current step was computed
- topLeftFree -- coordinates of the first free cell (top left)
- candidate size -- size of the current square for placement
*/
struct SolutionStep
{
    int firstFreeRow; 
    bool computed;

    std::pair<int, int> topLeftFree;
    int candidateSize;
};

/*
Struct representing a tabletop
- grid -- a matrix of cells of the tabletop
- squares -- all squares placed on the tabletop
- size -- size of the side of the tabletop

Methods:
- setState -- sets the value of the cells representing a square to the input value
- addSquare -- adds square to the tabletop
- removeSquare -- removes square from the tabletop
- findFirstFreeCell -- returns the first free cell coordinates on the tabletop or (-1, -1) if none were found
- findMaxSquareSize -- returns the maximum size of the square that can be placed in specified corner
- getLastSquare -- returns last square added to the tabletop
- printGrid -- prints the tabletop on screen
*/
struct Tabletop
{
    std::vector<std::vector<int>> grid;
    std::vector<Square> squares;
    int size;

    // tabletop initialization
    Tabletop(int size) : size(size), grid(size, std::vector<int>(size, 0)) {}

    // setting value of cells representing a square to the input value
    void setState(Square& square, int squareNumber)
    {
        // iterating over rows
        for (int i = square.y; i < square.y + square.size; i++)
        {
            // iterating over cells in row
            for (int j = square.x; j < square.x + square.size; j++)
            {
                grid[i][j] = squareNumber;
            }
        }
    }

    // adding square to the vector and updating grid state
    void addSquare(Square& square)
    {
        squares.push_back(square);

        // setting cells representing a square to it's number
        setState(square, squares.size());
    }

    // removing square from the vector and updating grid state
    void removeSquare()
    {
        Square square = squares.back();
        squares.pop_back();

        // setting cells that represented a square to 0
        setState(square, 0);
    }

    // searching for the first free cell
    std::pair<int, int> findFirstFreeCell(int firstFreeRow)
    {
        // iterating over rows starting at firstFreeRow
        for (int i = firstFreeRow; i < size; i++)
        {
            // iterating over cells in row
            for (int j = 0; j < size; j++)
            {
                // free cell found
                if (!grid[i][j])
                {
                    return {j, i};
                }
            }
        }

        // no free cell found
        return {-1, -1};
    }

    // calculating the maximum size of a square that can fit
    int findMaxSquareSize(std::pair<int, int> cellCoordinates)
    {
        int maxSize = 0;
        int x = cellCoordinates.first; 
        int y = cellCoordinates.second;

        while (true)
        {
            // out of grid boundaries
            if (x + maxSize >= size || y + maxSize >= size)
            {
                break;
            }

            // checking bottom row
            for (int i = x; i <= x + maxSize; i++)
            {
                // collision found -- returning current value
                if(grid[y + maxSize][i])
                {
                    return maxSize;
                }
            }
   
            // checking right column
            for (int i = y; i <= y + maxSize; i++)
            {
                // collision found -- returning current value
                if (grid[i][x + maxSize])
                {
                    return maxSize;
                }
            }

            // no collision found -- increasing size
            maxSize++;
        }

        return maxSize;
    }

    // returning last square added to the tabletop
    Square& getLastSquare()
    {
        return squares.back();
    }

    // printing the tabletop on screen
    void printGrid()
    {
        // iterating over rows
        for (int i = 0; i < size; i++)
        {
            // iterating over cells in row
            for (int j = 0; j < size; j++)
            {
                std::cout << "[" << grid[i][j] << "]";
            }

            std::cout << "\n";
        }

        std::cout << "\n";
    }
};

/*
Struct for debugging purposes
- debug -- is debugging enabled

Methods:
- log -- depending on input parametrs prints message on the screen
*/
struct Debugger
{
    bool debug = DEBUG;

    void log(const std::string& message)
    {
        if (debug)
        {
            std::cout << message << "\n";
        }
    }

    void log(const std::string& message, int value)
    {
        if (debug)
        {
            std::cout << message << ": " << value << "\n";
        }
    }

    void log(const std::string& message, int firstValue, int secondValue)
    {
        if (debug)
        {
            std::cout << message << ": (" << firstValue << ", " << secondValue << ")\n";
        }
    }

    void log(const std::string& message, Square& square)
    {
        if (debug)
        {
            std::cout << message << " (" << square.x + 1 << ", " << square.y + 1 << ") with size " << square.size << "\n";
        }
    }

    void log(Tabletop& tabletop)
    {
        if (debug)
        {
            tabletop.printGrid();
        }
    }
};


/*
Struct for calculating and printing solution
- solutionsStack -- stack with steps of a partial solution
- bestSolution -- current best found solution
- debugger -- debugger for logging information
- multiplier -- solutions for composite numbers are proportional to solutions for their smallest prime divisors

Methods:
- smallestPrime -- returns smallest prime divisor of the number or 0 if the number is prime
- getTabletop -- returns a created tabletop of given size with first squares for optimization
- solve -- returns optimal solution for given tabletop size
- outputSolution -- prints found optimal solution on screen
*/
struct Solver
{
    std::stack<SolutionStep> solutionsStack {};
    std::vector<Square> bestSolution {};
    Debugger debugger;
    int multiplier = 1;

    // searching for smallest prime divisor of the number
    int smallestPrime(int num)
    {
        // optimal search starts at 2 and ends at square root of the number
        for (int i = 2; i <= int(std::sqrt(num)); i++)
        {
            // prime divisor found
            if (num % i == 0)
            {
                return i;
            }
        }

        // number is prime
        return 0;
    }

    // creates a tabletop of given size and puts first squares for optimization
    Tabletop& getTabletop(int tabletopSize)
    {
        // initializing tabletop
        Tabletop& tabletop = *new Tabletop(tabletopSize);

        // if the size is odd, make some initial placements
        if (tabletopSize % 2 == 1)
        {
            // placing squares larger than this will be ineffective
            int threshold = tabletopSize / 2 + 1;
            int middle = tabletopSize / 2;
        
            Square sq1 = {middle, middle, threshold};
            Square sq2 = {0, middle + 1, threshold - 1};
            Square sq3 = {middle + 1, 0, threshold - 1};

            tabletop.addSquare(sq1);
            tabletop.addSquare(sq2);
            tabletop.addSquare(sq3);
            // from now on roughly 1/4 is unfilled

            debugger.log(SQUARE_PLACEMENT_MESSAGE, sq1);
            debugger.log(SQUARE_PLACEMENT_MESSAGE, sq2);
            debugger.log(SQUARE_PLACEMENT_MESSAGE, sq3);
            debugger.log(TABLETOP_STATE_MESSAGE);
            debugger.log(tabletop);
        }

        return tabletop;
    }

    // finding optimal solution for given tabletop size
    void solve(int tabletopSize)
    {
        // searching for smallest prime divisor
        int prime = smallestPrime(tabletopSize);
        multiplier = prime ? tabletopSize / prime : 1;

        // reduce the size of a tabletop and solve the problem for a smaller size (if tabletopSize is a composite number)
        if (multiplier != 1)
        {
            debugger.log("Optimizing tabletop size from", tabletopSize);
        }
        tabletopSize /= multiplier;

        debugger.log("Solving for", tabletopSize);

        // creating tabletop
        Tabletop& tabletop = getTabletop(tabletopSize);

        // adding the inital step of solutions
        SolutionStep initialStep = {0, false, {0, 0}, 0};
        solutionsStack.push(initialStep);

        debugger.log("START");

        // until we run out of steps in solutions
        while (!solutionsStack.empty())
        {
            // get step from the top of the stack
            SolutionStep& topStep = solutionsStack.top();

            debugger.log("Checking step");
            debugger.log(TABLETOP_STATE_MESSAGE);
            debugger.log(tabletop);

            // prune the step if partial solution already worse than current best
            if (!bestSolution.empty() && tabletop.squares.size() >= bestSolution.size())
            {
                debugger.log("Prunning step of sub-optimal partial solution");

                // going to previous step
                solutionsStack.pop();

                // removing square that was placed at this step
                if (!solutionsStack.empty())
                {
                    debugger.log(SQUARE_REMOVAL_MESSAGE, tabletop.getLastSquare());

                    tabletop.removeSquare();

                    debugger.log(BACKTRACKING_MESSAGE);
                    debugger.log(tabletop);
                }

                continue;
            }

            // if placement of the square on this step hasn't been calculated
            if (!topStep.computed)
            {
                // searching for first free cell
                std::pair<int, int> firstFreeCell = tabletop.findFirstFreeCell(topStep.firstFreeRow);

                // if no free cells found -- tabletop is covered
                if (firstFreeCell.first == -1)
                {
                    debugger.log("Tabletop fully covered");

                    // if current solution is better then current best
                    if (bestSolution.empty() || tabletop.squares.size() < bestSolution.size())
                    {
                        debugger.log("New best solution");

                        bestSolution = tabletop.squares;
                    }

                    // going to previous step
                    solutionsStack.pop();

                    // removing square that was placed at this step
                    if (!solutionsStack.empty())
                    {
                        debugger.log(SQUARE_REMOVAL_MESSAGE, tabletop.getLastSquare());

                        tabletop.removeSquare();

                        debugger.log(BACKTRACKING_MESSAGE);
                        debugger.log(tabletop);
                    }

                    continue;
                }

                // calculating maximum size of a fitting square
                topStep.topLeftFree = firstFreeCell;
                topStep.candidateSize = std::min(tabletop.findMaxSquareSize(firstFreeCell), tabletopSize - 1);
                topStep.computed = true;

                debugger.log("Free cell", topStep.topLeftFree.first + 1, topStep.topLeftFree.second + 1);
            }

            // placing squares of all avaliable sizes from the biggest one
            if (topStep.candidateSize >= 1)
            {
                int currentSize = topStep.candidateSize;
                topStep.candidateSize--;
                Square newSquare = {topStep.topLeftFree.first, topStep.topLeftFree.second, currentSize};
                tabletop.addSquare(newSquare);

                debugger.log(SQUARE_PLACEMENT_MESSAGE, newSquare);
                debugger.log(TABLETOP_STATE_MESSAGE);
                debugger.log(tabletop);
                
                // adding a step of a new partial solution
                SolutionStep newStep;
                newStep.firstFreeRow = topStep.topLeftFree.second;
                newStep.computed = false;
                solutionsStack.push(newStep);

                continue;
            }
            // all posible sizes were tried
            else
            {
                // going to previous step
                solutionsStack.pop();

                // removing square that was placed at this step
                if (!solutionsStack.empty())
                {
                    debugger.log(SQUARE_REMOVAL_MESSAGE, tabletop.getLastSquare());

                    tabletop.removeSquare();

                    debugger.log(BACKTRACKING_MESSAGE);
                    debugger.log(tabletop);
                }
            }
        }

        debugger.log("END");
    }

    // printing found optimal solution on screen
    void outputSolution()
    {
        std::cout << bestSolution.size();

        for (auto& square : bestSolution)
        {
            std::cout << "\n" << square.x * multiplier + 1 << " " << square.y * multiplier + 1 << " " << square.size * multiplier;
        }

        std::cout << "\n";
    }
};

int main()
{
    int N;
    std::cin >> N;
    Solver solver;
    solver.solve(N);
    solver.outputSolution();
}
