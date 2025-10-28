#include <iostream>
#include <vector>
#include <algorithm>

#define INF 100000000

/*
Class used for debugging logs, prints crucial info
*/
class Debugger 
{ 
private:
    bool debug = true;
    int size;
public:
    Debugger(int size) :
        size(size)
    {}

    void logMsg(const std::string& msg) 
    {
        if (!debug) 
        {
            return;
        }

        std::cout << msg << "\n";
    }

    void logCost(const std::string& msg, const int cost) 
    {
        if (!debug)
        {
            return;
        }

        if (cost < INF) 
        {
            std::cout << msg << cost << "\n";
        }
        else 
        {
            std::cout << msg << "INF" << "\n";
        }
    }

    void logCity(const std::string& startMsg, int city, const std::string& endMsg)
    {
        if (!debug) 
        {
            return;
        }    
        std::cout << startMsg << city << endMsg;
    }

    void logMask(const std::string& startMsg, const int mask, const std::string& endMsg)
    {
        if (!debug)
        {
            return;
        }

        std::cout << startMsg;

        std::vector<int> bitPositions;
        for (int i = 0; i < sizeof(int) * 8; i++) 
        {
            if (mask & (1 << i)) 
            {
                bitPositions.push_back(i);
            }
        }

        std::reverse(bitPositions.begin(), bitPositions.end());

        for(int i = 0; i < size; i++)
        {
            if (i == bitPositions[bitPositions.size() - 1])
            {
                bitPositions.pop_back();
                continue;
            }

            std::cout << i << " ";
        }

        std::cout << endMsg;
    }
};

class Solver
{    
private:
    int numberOfCities;
    std::vector<std::vector<int>> nextCities;       // stores the next city in the optimal path for a given state
    std::vector<std::vector<int>> foundWays;        // stroes the minimal cost of a trip for a given state
    std::vector<std::vector<int>> costMatrix;       // given data about costs of trips

    Debugger dbg;
public:
    Solver(std::vector<std::vector<int>>& costMatrix) :
        costMatrix(costMatrix),
        numberOfCities(costMatrix.size()),
        dbg(numberOfCities)
    {
        // use masks where every bit represents if we visited (1) a city or not (0)
        int mask = (1 << (numberOfCities - 1)) - 1;     // all 1-s meaning all cities

        // prepare the needed vectors
        nextCities.resize(mask, std::vector<int>{});
        for (auto & element : nextCities)
        {
            element.resize(numberOfCities - 1, 0);
        }
    
        foundWays.resize(mask, std::vector<int>{});
        for (auto& element : foundWays)
        {
            element.resize(numberOfCities - 1, -1);
        }
    }

    // method initiating the search of a solution
    void solve()
    {
        dbg.logMsg("START\n");

        // single city case
        if (costMatrix.size() == 1)
        {
            dbg.logMsg("FINISH\n");

            std::cout << "0\n0\n" ;
            return;
        }

        // find the best cost 
        int cost = findCost();
        dbg.logMsg("FINISH\n");
        
        // path wasn't found (no hamiltonian cycle)
        if (cost >= INF)
        {
            std::cout << "no path\n";
            return;
        }

        // get the order of visiting the cities
        std::vector<int> way = getWay();

        dbg.logMsg("ANSWER:");

        // print results
        std::cout << cost << "\n";
        for (auto& element : way)
        {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }

    // find the cheapest way to visit remaining cities (mask) starting from the given one (last)
    int tspDP(int mask, int last)
    {
        dbg.logMsg("\nCurrent state:");
        dbg.logMask("Unvisited cities: ", mask << 1, "\n");
        dbg.logCity("Last visited city ", last + 1, "\n");

        // if mask is full ones, this means we have visited all cities
        // except for home city 0, as it's not represented in the mask
        // so we return the cost of going from our given city last to home city 0
        if (mask == ((1 << (numberOfCities - 1)) - 1))
        {
            dbg.logMsg("\nAll cities visited");
            dbg.logCity("Returning from city ", last + 1, " to start city 0\n");
            dbg.logCost("Cost to return: ", costMatrix[last + 1][0]);

            return costMatrix[last + 1][0];
        }

        // return memorized result if available.
        if (foundWays[mask][last] != -1)
        {
            dbg.logMsg("\nCheapest way from this state is already calculated");
            dbg.logCost("Stored cost: ", foundWays[mask][last]);

            return foundWays[mask][last];
        }

        int bestCost = INF;
    
        // try all non-visited cities.
        for (int city = 0; city < numberOfCities - 1; city++) 
        {
            dbg.logCost("Current best cost: ", bestCost);

            dbg.logMsg("\nReviewing next move:");

            // if city is already visited, skip it.
            if (mask & (1 << city))
            {
                dbg.logCity("Skipping city ", city + 1, " (already visited)\n");
                continue;
            }
            
            dbg.logCity("Traveling from city ", last + 1, "");
            dbg.logCity(" to city ", city + 1, "\n");

            // new remaining cities
            int newMask = mask | (1 << city);
            dbg.logMask("Updated unvisited cities: ", newMask << 1, "\n");

            // compute cost of visiting the remaining cities after picking one 
            int cost = costMatrix[last + 1][city + 1] + tspDP(newMask, city);

            dbg.logCity("\nBacktracking: Finished exploring city ", city + 1, "");
            dbg.logCity(" from city ", last + 1, "\n");
            dbg.logMask("Visited cities: ", mask << 1, "\n");
            dbg.logCost("Path cost excluding last move: ", cost - costMatrix[last + 1][city + 1]);
            dbg.logCost("Return cost to city: ", costMatrix[last + 1][city + 1]);
            dbg.logCost("Total subtour cost: ", cost);

            // if subtour's cost is better than our current cost - we update it
            if (cost < bestCost)
            {
                dbg.logCost("New optimal path found! Updating best cost: ", cost);

                bestCost = cost;

                // and also update the city we choose at this state for the best cost
                nextCities[mask][last] = city + 1;
            }
        }

        foundWays[mask][last] = bestCost;

        return bestCost;
    }

    // method searching the best cost for visiting all the cities
    int findCost() 
    {
        // best cost for tour
        int bestTourCost = INF;

        for (int city = 0; city < numberOfCities - 1; city++)
        {
            dbg.logCost("Current best tour cost: ", bestTourCost);
            dbg.logCity("\nReviewing city ", city + 1, " as Start:\n");

            // mask representing one visited city
            int mask = 1 << city;
            dbg.logMask("Cities to visit: ", 1 << city + 1, "\n");

            dbg.logMsg("\nBEGIN");
            // compute the minimal cost of visiting all cities one time starting from city i
            int cost = costMatrix[0][city + 1] + tspDP(mask, city);     // complete the tour - return to city 0 from city i
            dbg.logMsg("END\n");

            dbg.logMask("Visited cities: ", 1 << city + 1, "\n");
            dbg.logCost("It costed: ", cost - costMatrix[0][city + 1]);
            dbg.logCost("Return to start costed: ", costMatrix[0][city + 1]);
            dbg.logCost("Total tour cost: ", cost);

            // if we found cost is better than the current one - update it
            if (cost < bestTourCost)
            {
                dbg.logCost("New optimal path found! Updating best tour cost: ", cost);
                bestTourCost = cost;

                // update the city we start the tour from
                nextCities[0][0] = city + 1;
            }
        }

        foundWays[0][0] = bestTourCost;

        return bestTourCost;
    }

    // reconstruct the tour from the computed nextCities pointers.
    std::vector<int> getWay() 
    {
        dbg.logMsg("\nTracing optimal path:");

        std::vector<int> result;

        // home city (city 0)
        result.push_back(0);
        dbg.logCity("Starting at home city ", 0, "\n");

        // the best starting city was stored in nextCities[0][0]
        int currentCity = nextCities[0][0];  
        result.push_back(currentCity);
        dbg.logCity("First move: traveling from city ", 0, "");
        dbg.logCity(" to city ", currentCity, "\n");

        // last visited city
        int last = currentCity - 1;

        // the mask representing already visited cities
        int mask = 1 << (last);

        // continue until all non-start cities are visited.
        while (mask != ((1 << (numberOfCities - 1)) - 1)) 
        {
            // get the next best city from the current state
            int next = nextCities[mask][last];
            dbg.logCity("Ttraveling from city ", currentCity, "");
            dbg.logCity(" to city ", next, "\n");

            result.push_back(next);
            last = next - 1;

            // mark this city as visited
            mask |= (1 << (last));

            // update currentCity for better readability in logs
            currentCity = next;
        }

        // finally, return to the starting city
        result.push_back(0);
        dbg.logCity("Final move: traveling from city ", currentCity, "");
        dbg.logCity(" to home city ", 0, "\n");

        return result;
    }
};

int main() 
{
    // size of cost matrix
    int n;
    std::cin >> n;

    std::vector<std::vector<int>> costMatrix;
    costMatrix.resize(n, std::vector<int>{});

    // buffer for reading data
    int buffer;
    for (int i = 0; i < n; i++)
    {
        costMatrix[i].resize(n);

        for (int j = 0; j < n; j++)
        {
            std::cin >> buffer;

            // if buffer is not 0 -> there is a way
            if (buffer)
            {
                costMatrix[i][j] = buffer;
            }
            // buffer is 0 -> there is no way
            else
            {
                costMatrix[i][j] = INF;
            }
        }
    }

    Solver s(costMatrix);
    s.solve();
}