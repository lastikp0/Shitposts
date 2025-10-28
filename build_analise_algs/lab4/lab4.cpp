#include <iostream>
#include <string>
#include <vector>

#define DEBUG true

/*
Struct for debugging purposes
- debug -- is debugging enabled

Methods:
- logMessage -- prints message on screen (with or without additional value)
- logPrefixFunction -- prints the state of prefix-function on screen
- logSearch -- prints the state of search on screen
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

    void logPrefixFunction(std::string& pattern, int i, int j, std::vector<int>& lps)
    {
        if (debug)
        {
            for (int k = 0; k < pattern.size(); k++)
            {
                std::cout << "   " << k;
            }
            std::cout << "\n";

            for (int k = 0; k < pattern.size(); k++)
            {
                std::cout << "   " << pattern[k];
            }
            std::cout << "\n";

            std::cout << "   ";
            for (int k = 0; k < j; k++)
            {
                std::cout << "    ";
            }
            std::cout << "j" << "   ";
            for (int k = j + 1; k < i; k++)
            {
                std::cout << "    ";
            }
            std::cout << "i" << "\n";

            for (int k = 0; k < i; k++)
            {
                std::cout << "   " << lps[k];
            }
            std::cout << "\n";

            std::cout << "\n";
        }
    }

    void logSearch(std::string& pattern, std::string& text, int patternIndex, int textIndex, std::vector<int>& lps)
    {
        if (debug)
        {
            std::cout << "   ";
            for (int k = 0; k < textIndex; k++)
            {
                std::cout << "    ";
            }
            std::cout << "t" << "\n";

            for (int k = 0; k < text.size(); k++)
            {
                std::cout << "   " << text[k];
            }
            std::cout << "\n";

            for (int k = patternIndex; k < textIndex; k++)
            {
                std::cout << "    ";
            }
            for (int k = 0; k < pattern.size(); k++)
            {
                std::cout << "   " << pattern[k];
            }
            std::cout << "\n";

            std::cout << "   ";
            for (int k = patternIndex; k < textIndex; k++)
            {
                std::cout << "    ";
            }
            for (int k = 0; k < patternIndex; k++)
            {
                std::cout << "    ";
            }
            std::cout << "p" << "\n";

            std::cout << "\n";

            std::cout << "lps:" << "\n";
            for (int k = 0; k < pattern.size(); k++)
            {
                std::cout << "   " << pattern[k];
            }
            std::cout << "\n";
            for (int k = 0; k < lps.size(); k++)
            {
                std::cout << "   " << lps[k];
            }
            std::cout << "\n";

            std::cout << "\n";
        }
    }
};

/*
Struct for searching pattern in text using the KMP algorithm
- lps -- (Largest Prefix Suffix) integer array built by prefix-function from pattern.
lps[i] -- contains maximum length of prefix that equals suffix of substring pattern[0...i]
- result -- indexes of the beginning of occurrences of pattern in text
- debugger -- debugger for logging information

Methods:
- createLps -- prefix-function that creates lps array
- search -- searches for pattern in text using KMP algorithm
- printResult -- prints result of a search on screen
*/
struct Solver
{
    std::vector<int> lps;
    std::vector<int> result;
    Debugger debugger;

    // prefix-function that creates lps array for given pattern
    void createLps(std::string& pattern)
    {
        debugger.logMessage("Creating lps array");
        debugger.logMessage("lps[i] - contains maximum length of prefix that equals suffix of substring pattern[0...i]");

        // lps array size is equal to the size of given pattern
        lps.resize(pattern.size());

        // i - current element index, j - current prefix index
        int i = 1, j = 0;

        debugger.logMessage("j - current prefix index, i - current element index");
        debugger.logPrefixFunction(pattern, i, j, lps);

        // while pattern isn't over
        while (i < pattern.size())
        {
            // if symbols match -- prefix and suffix match
            if (pattern[i] == pattern[j])
            {
                // length of prefix is up by 1 from j
                lps[i] = j + 1;

                // both indexes are incremented to check matching of bigger prefix and suffix on next iteration
                i++;
                j++;

                debugger.logMessage("Symblos match - prefix and suffix match");
                debugger.logMessage("lps[i] = j + 1");
                debugger.logMessage("Both indexes are incremented to check matching of bigger prefix and suffix on next iteration");
                debugger.logPrefixFunction(pattern, i, j, lps);
            }
            // if symbols don't match
            else
            {
                debugger.logMessage("Symblos don't match");

                // there are no smaller prefixes
                if (j == 0)
                {
                    // no prefixes match suffixes
                    lps[i] = 0;

                    // move next
                    i++;

                    debugger.logMessage("No smaller prefixes");
                    debugger.logMessage("lps[i] = 0");
                    debugger.logMessage("Move next");
                    debugger.logPrefixFunction(pattern, i, j, lps);
                }
                // prefix and suffix partially match
                else
                {
                    // using information from previous step for optimizing search
                    j = lps[j - 1];

                    debugger.logMessage("Using information from previous step for optimizing search");
                    debugger.logMessage("j = lps[j - 1]");
                    debugger.logPrefixFunction(pattern, i, j, lps);
                }
            }
        }
    }

    // searching for pattern in text
    void search(std::string& pattern, std::string& text)
    {
        // creating lps array
        createLps(pattern);

        debugger.logMessage("Searching");

        // initializing indexes for pattern and text
        int patternIndex = 0, textIndex = 0;

        debugger.logMessage("t - text index, p - pattern index");
        debugger.logSearch(pattern, text, patternIndex, textIndex, lps);

        // while text isn't over
        while (textIndex < text.size())
        {
            // if symbols match
            if (text[textIndex] == pattern[patternIndex])
            {
                // incrementing both indexes to check next symbols
                patternIndex++;
                textIndex++;

                debugger.logMessage("Symbols match");
                debugger.logMessage("Incrementing both indexes to check next symbols");
                debugger.logSearch(pattern, text, patternIndex, textIndex, lps);

                // if pattern is over -- pattern found
                if (patternIndex == pattern.size())
                {
                    debugger.logMessage("Pattern found");

                    debugger.logMessage("Entry index", textIndex - patternIndex);
                    // save index of the beginning of occurrence of pattern in text
                    result.push_back(textIndex - patternIndex);

                    // using information from lps array for optimizing search
                    patternIndex = lps[patternIndex - 1];

                    debugger.logMessage("Using information from lps array for optimizing search");
                    debugger.logMessage("p = lps[p - 1]");
                    debugger.logSearch(pattern, text, patternIndex, textIndex, lps);
                }
            }
            // if symbols don't match
            else
            {
                debugger.logMessage("Symbols don't match");

                // start of pattern -- search further
                if (patternIndex == 0)
                {
                    textIndex++;

                    debugger.logMessage("Start of pattern doesn't match");
                    debugger.logMessage("Move next");
                    debugger.logSearch(pattern, text, patternIndex, textIndex, lps);
                }
                else
                {
                    // using information from lps array for optimizing search
                    patternIndex = lps[patternIndex - 1];

                    debugger.logMessage("Using information from lps array for optimizing search");
                    debugger.logMessage("p = lps[p - 1]");
                    debugger.logSearch(pattern, text, patternIndex, textIndex, lps);
                }
            }
        }

        debugger.logMessage("End of search");
    }
    
    // printing result on screen
    void printResult()
    {
        // if pattern wasn't found
        if (result.size() == 0)
        {
            std::cout << -1;
        }
        // pattern found
        else
        {
            for (int i = 0; i < result.size() - 1; i++)
            {
                std::cout << result[i] << ",";
            }
            std::cout << result[result.size() - 1];
        }
    }
};

int main()
{
    std::string pattern;
    std::string text;

    std::cin >> pattern;
    std::cin >> text;

    Solver solver;
    solver.search(pattern, text);
    solver.printResult();

    return 0;
}