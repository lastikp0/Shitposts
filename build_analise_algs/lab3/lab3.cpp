#include <bits/stdc++.h>

#define DEBUG true

/*
Struct for debugging purposes
- debug -- is debugging enabled

Methods:
- logMessage -- prints message on screen (with or without additional value/values)
- logStep -- prints information about step (char) on screen
- logStart -- initial state of distance matrix
- logDelete -- prints process of filling the first row of distance matrix
- logInsert -- prints process of filling the first column of distance matrix
- logDistance -- prints process of filling of distance matrix
- logNeighbours -- prints cell neighbours for restoring edit sequence
*/
struct Debugger
{
    bool debug = DEBUG;

    void logMessage(const std::string& message)
    {
        if (debug)
        {
            std::cout << message << "\n";
            std::cout << "\n";
        }
    }

    void logMessage(const std::string& message, int value)
    {
        if (debug)
        {
            std::cout << message << ": " << value << "\n";
            std::cout << "\n";
        }
    }

    void logMessage(const std::string& message, int i, int j)
    {
        if (debug)
        {
            std::cout << message << ": " << i << " " << j << "\n";
            std::cout << "\n";
        }
    }

    void logStep(char step)
    {
        if (debug)
        {
            if (step == 'M')
            {
                std::cout << "M: symbols match" << "\n";
            }
            else if (step == 'R')
            {
                std::cout << "R: replace symbol" << "\n";
            }
            else if (step == 'I')
            {
                std::cout << "I: insert symbol" << "\n";
            }
            else if (step == 'D')
            {
                std::cout << "D: delete symbol" << "\n";
            }
            std::cout << "\n";
        }
    }

    void logStart(std::string& a, std::string& b)
    {
        if (debug)
        {
            std::cout << "\t\t";
            for (int i = 0; i < a.size(); i++)
            {
                std::cout << a[i] << "\t";
            }
            std::cout << "\n";

            std::cout << "\t";
            std::cout << "0" << "\t";
            for (int i = 1; i < a.size() + 1; i++)
            {
                std::cout << " " << "\t";
            }
            std::cout << "\n";

            for (int j = 1; j < b.size() + 1; j++)
            {
                std::cout << b[j - 1] << "\t";

                for (int i = 0; i < a.size() + 1; i++)
                {
                    std::cout << " " << "\t";
                }

                std::cout << "\n";
            }

            std::cout << "\n";
        }
    }

    void logDelete(std::vector<std::vector<int>>& distance, std::string& a, std::string& b, int aIndex)
    {
        if (debug)
        {
            std::cout << "\t\t";
            for (int i = 0; i < a.size(); i++)
            {
                std::cout << a[i] << "\t";
            }
            std::cout << "\n";

            std::cout << "\t";
            for (int i = 0; i < a.size() + 1; i++)
            {
                if (i <= aIndex)
                {
                    std::cout << distance[i][0] << "\t";
                }
                else
                {
                    std::cout << " " << "\t";
                }
            }
            std::cout << "\n";

            for (int j = 1; j < b.size() + 1; j++)
            {
                std::cout << b[j - 1] << "\t";

                for (int i = 0; i < a.size() + 1; i++)
                {
                    std::cout << " " << "\t";
                }

                std::cout << "\n";
            }

            std::cout << "\n";
        }
    }

    void logInsert(std::vector<std::vector<int>>& distance, std::string& a, std::string& b, int bIndex)
    {
        if (debug)
        {
            std::cout << "\t\t";
            for (int i = 0; i < a.size(); i++)
            {
                std::cout << a[i] << "\t";
            }
            std::cout << "\n";

            std::cout << "\t";
            for (int i = 0; i < a.size() + 1; i++)
            {
                std::cout << distance[i][0] << "\t";
            }
            std::cout << "\n";

            for (int j = 1; j < b.size() + 1; j++)
            {
                std::cout << b[j - 1] << "\t";

                for (int i = 0; i < a.size() + 1; i++)
                {
                    if (j <= bIndex && i == 0)
                    {
                        std::cout << distance[i][j] << "\t";
                    }
                    else
                    {
                        std::cout << " " << "\t";
                    }
                }

                std::cout << "\n";
            }

            std::cout << "\n";
        }
    }

    void logDistance(std::vector<std::vector<int>>& distance, std::string& a, std::string& b, int aIndex, int bIndex)
    {
        if (debug)
        {
            std::cout << "\t\t";
            for (int i = 0; i < a.size(); i++)
            {
                std::cout << a[i] << "\t";
            }
            std::cout << "\n";

            std::cout << "\t";
            for (int i = 0; i < a.size() + 1; i++)
            {
                std::cout << distance[i][0] << "\t";
            }
            std::cout << "\n";

            for (int j = 1; j < b.size() + 1; j++)
            {
                std::cout << b[j - 1] << "\t";

                for (int i = 0; i < a.size() + 1; i++)
                {
                    if (i < aIndex)
                    {
                        std::cout << distance[i][j] << "\t";
                    }
                    else if (i == aIndex)
                    {
                        if (j <= bIndex)
                        {
                            std::cout << distance[i][j] << "\t";
                        }
                        else
                        {
                            std::cout << " " << "\t";
                        }
                    }
                    else
                    {
                        std::cout << " " << "\t";
                    }
                }

                std::cout << "\n";
            }

            std::cout << "\n";
        }
    }

    void logNeighbours(std::vector<std::vector<int>>& distance, int i, int j)
    {
        if (debug)
        {
            if (j - 1 >= 0)
            {
                if (i - 1 >= 0)
                {
                    std::cout << distance[i - 1][j - 1] << "\t";
                }

                std::cout << distance[i][j - 1] << "\n";
            }

            if (i - 1 >= 0)
            {
                std::cout << distance[i - 1][j] << "\t";
            }

            std::cout << distance[i][j] << "\n";

            std::cout << "\n";
        }
    }
};

int main() {
    Debugger debugger;

    // getting cost of operations
    int replaceCost, insertCost, deleteCost;
    std::cin >> replaceCost >> insertCost >> deleteCost;

    // getting string a to edit into string b
    std::string a, b;
    std::cin >> a >> b;

    // matrix of min cost of editing prefix i of string a into prefix j of string b
    std::vector<std::vector<int>> distance(a.size() + 1, std::vector<int> (b.size() + 1));

    // empty string into empty string -- always 0
    distance[0][0] = 0;

    debugger.logMessage("Start");
    debugger.logStart(a, b);

    // filling first row by delete cost
    debugger.logMessage("Filling first row by delete cost");
    for (int i = 1; i < a.size() + 1; i++)
    {
        distance[i][0] = distance[i - 1][0] + deleteCost;
        debugger.logDelete(distance, a, b, i);
    }

    // filling first column by insert cost
    debugger.logMessage("Filling first column by insert cost");
    for (int j = 1; j < b.size() + 1; j++)
    {
        distance[0][j] = distance[0][j - 1] + insertCost;
        debugger.logInsert(distance, a, b, j);
    }

    // main cycle
    debugger.logMessage("Starting main cycle");
    for (int i = 1; i < a.size() + 1; i++)
    {
        for (int j = 1; j < b.size() + 1; j++)
        {
            // calculating min cost from: Insert (insertCost), Delete (deleteCost), Replace (replaceCost), Match(0)
            distance[i][j] = std::min(distance[i - 1][j - 1] + replaceCost * (a[i - 1] != b[j - 1]),
                            std::min(distance[i - 1][j] + deleteCost,
                                    distance[i][j - 1] + insertCost));
            
            debugger.logMessage("Choosing cheapest path for", i, j);
            debugger.logDistance(distance, a, b, i, j);
        }
    }

    debugger.logMessage("End of main cycle");
    debugger.logMessage("End");

    // final value -- right-bottom cell
    std::cout << distance.back().back() << "\n";
}