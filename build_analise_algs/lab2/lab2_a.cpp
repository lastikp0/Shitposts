#include <iostream>
#include <vector>

#define INF 10000000

/*
Class used for debugging logs, prints crucial info
*/
class Debugger
{
private:
    bool debug = true;

public:
    void logMsg(const std::string& msg) 
    {
        if (!debug) 
        {
            return;
        }

        std::cout << msg << "\n";
    }

    void log(const std::string& startMsg, const float cost, const std::string& endMsg) 
    {
        if (!debug)
        {
            return;
        }

        if (cost < INF) 
        {
            std::cout << startMsg << cost << endMsg;
        }
        else 
        {
            std::cout << startMsg << "INF" << endMsg;
        }
    }

    void logPath(std::vector<int> path)
    {
        if (!debug)
        {
            return;
        }

        for(auto& element : path)
        {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }
};

class Solver
{
private:
    int numberOfCities;             // number of all cities
    int startCity;                  // index of statring city (default 0)
    double totalCost = 0;           // total cost of tour

    std::vector<int> path {};                           // tour path
    std::vector<int> citiesToVisit {};                  // array of unvisited cities (candidates)
    std::vector<std::vector<double>> costMatrix{};      // given data about costs of trips

    Debugger dbg;
public:
    Solver(std::vector<std::vector<double>>& costMatrix, int startCity = 0) :
        costMatrix(costMatrix),
        numberOfCities(costMatrix.size()),
        startCity(startCity)
    {
        // adding starting city to path
        path.push_back(startCity);

        // add all cities except for the starting one
        for (int i = 0; i < numberOfCities; i++)
        {
            if (i == startCity)
            {
                continue;
            }

            // cities that are yet to be visited
            citiesToVisit.push_back(i);
        }
    }

    // method searching the best cost for visiting all the cities and coresponding path
    void findSolution()
    {
        int currentCity = startCity;

        // while we haven't visited everything except last one
        while (path.size() != numberOfCities - 1)
        {
            dbg.logMsg("\nCurrent path");
            dbg.logPath(path);

            // cheapest arcs going in and out of path (which don't create a loop prematurely)
            std::pair<double, double> pathCheapestInOut; 

            // info about best candidate
            int bestCity, index;

            // best lower estimation of the rest of the trip
            double bestCandidateCost = INF;

            dbg.log("Current city ", currentCity, "\n");
            dbg.log("Current cost: ", totalCost, "\n");

            dbg.logMsg("Avaliable cities:");
            dbg.logPath(citiesToVisit);

            dbg.logMsg("\nReviewing all candidates:");

            // examing unreviewed cities
            for (int i = 0; i < citiesToVisit.size(); i++)
            {
                dbg.log("\nBest candidate cost: ", bestCandidateCost, "\n");

                // current reviewed city
                int city = citiesToVisit[i];
                dbg.log("Reviewing city ", city, "\n");

                // adding candidate to path to calculate it's lower bound
                path.push_back(city);

                // lower estimation of the rest of the trip for current candidate
                // initialized buy arcs for current path
                double lowerBound = findCheapestWays(startCity, true).first + findCheapestWays(city, true).second;

                // calculating all lowest arcs for free vertecies except current candidate
                std::pair<double, double> cityCheapestInOut;
                for (int j = 0; j < citiesToVisit.size(); j++)
                {
                    if (j != i)
                    {
                        cityCheapestInOut = findCheapestWays(citiesToVisit[j], false);

                        // adding them to sum
                        lowerBound += cityCheapestInOut.first + cityCheapestInOut.second;
                    }
                }

                // lower bound is half of the sum of arcs
                lowerBound /= 2;

                dbg.log("Lower bound L: ", lowerBound, "\n");
                dbg.log("Arc cost S: ", costMatrix[currentCity][city], "\n");
                dbg.log("S + L: ", costMatrix[currentCity][city] + lowerBound, "");

                // pick an arc if its (weight + lower estimation) is better than current candidate
                if (costMatrix[currentCity][city] + lowerBound < bestCandidateCost)
                {
                    dbg.log(", which is better than: ", bestCandidateCost, "\n");

                    // update the best cost of arc
                    bestCandidateCost = costMatrix[currentCity][city] + lowerBound;
                    bestCity = city; index = i;     

                    dbg.log("Update best candidate cost: ", bestCandidateCost, "\n");
                    dbg.log("Update the best candidate city ", bestCity, "\n");
                }
                else
                {
                    dbg.logMsg("");
                }

                // removing candidate from path to review others
                path.pop_back();
            }

            dbg.log("\nThe best candidate city ", bestCity, "\n");
            dbg.log("Travelling there costs: ", costMatrix[currentCity][bestCity], "\n");
            dbg.log("The total cost was: ", totalCost, "\n");

            // after finding the best arc, add the cost of travelling
            totalCost += costMatrix[currentCity][bestCity];

            // and add the new visited city to our path
            path.push_back(bestCity);
            // the city is no longer unvisited
            citiesToVisit.erase(citiesToVisit.begin() + index);

            // move to the new city
            currentCity = bestCity;

            dbg.log("Update the total cost: ", totalCost, "\n");
            dbg.logMsg("Update the path:");
            dbg.logPath(path);
            dbg.logMsg("Update avaliable cities:");
            dbg.logPath(citiesToVisit);
            dbg.log("Change city to ", currentCity, "\n");
        }

        // add the cost of the last city
        totalCost += costMatrix[currentCity][citiesToVisit[0]];

        // add last city to path
        path.push_back(citiesToVisit[0]);

        dbg.log("\nTravelling to last city costs: ", costMatrix[currentCity][citiesToVisit[0]], "\n");
        dbg.log("Update the total cost: ", totalCost, "\n");

        dbg.logMsg("Update the path:");
        dbg.logPath(path);

        // add the cost of the return (completing the cycle)
        totalCost += costMatrix[citiesToVisit[0]][startCity];

        // add start city to path (completing the cycle)
        path.push_back(startCity);

        dbg.log("\nTravelling to the start costs ", costMatrix[citiesToVisit[0]][startCity], "\n");
        dbg.log("Update the total cost ", totalCost, "\n");

        dbg.logMsg("Update the path:");
        dbg.logPath(path);
    }

    // method initiating the search of a solution
    void solve()
    {
        dbg.logMsg("START");

        // find the solution
        findSolution();

        dbg.logMsg("\nEND");

        // path wasn't found (no hamiltonian cycle)
        if (totalCost >= INF)
        {
            std::cout << "no path";
            return;
        }

        // print results
        dbg.logMsg("\nANSWER:\n");

        std::cout << totalCost << "\n";
        for (auto& element : path)
        {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }

    // method for searching cheapest IN and OUT arcs
    std::pair<double, double> findCheapestWays(int city, bool noLoop)
    {
        // the cheapest arcs going IN to the vertice and OUT of it
        std::pair<double, double> result {INF, INF};

        // reviewing all cities that we may travel to
        for (auto& currentCity : citiesToVisit)
        {
            // update an arc if it's cheaper than the one that was found previously
            if (costMatrix[currentCity][city] < result.first)     
            {
                result.first = costMatrix[currentCity][city];
            }

            if (costMatrix[city][currentCity] < result.second)
            {
                result.second = costMatrix[city][currentCity];
            }
        }

        // we shouldn't consider the arcs that go back to the same chunk
        // it's okay, when the chunk is a single city (as cost of trip to itself is set to INF)
        // but when we examine the already constructed path, we should exclude the arcs leading back to this chunk
        if (!noLoop)   
        {
            // so here we also check the arcs to start and end of our path
            // if we examine the path itself, this block is skipped
            if (costMatrix[path[path.size() - 1]][city] < result.first)
            {
                result.first = costMatrix[path[path.size() - 1]][city];
            }

            if(costMatrix[city][path[0]] < result.second)
            {
                result.second = costMatrix[city][path[0]];
            }
        }

        return result;
    }
};

int main() {
    // size of cost matrix
    int n;
    std::cin >> n;

    std::vector<std::vector<double>> costMatrix;
    costMatrix.resize(n, std::vector<double>{});

    // buffer for reading data
    double buffer;
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

    Solver s(costMatrix, 0);
    s.solve();
       
    return 0;
}