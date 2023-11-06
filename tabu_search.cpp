#ifndef ALSP_TABU_SEARCH
#define ALSP_TABU_SEARCH

#include <bits/stdc++.h>
#include "instance.cpp"
#include "solution.cpp"

using namespace std;

class TabuSearch
{
private:
    Instance *instance;
    Solution *initial_solution;
    Solution *solution;
    int tabu_length = 0;
    int *tabu;

public:
    TabuSearch(Instance *instance, Solution *initial_solution, int tabu_length = 10)
    {
        this->instance = (Instance *)malloc(sizeof(Instance));
        memcpy(this->instance, instance, sizeof(Instance));

        // this->initial_solution = new Solution(initial_solution);
        this->initial_solution = (Solution *)malloc(sizeof(Solution));
        *this->initial_solution = Solution(initial_solution);

        this->solution = (Solution *)malloc(sizeof(Solution));
        *this->solution = Solution(initial_solution);

        this->tabu_length = tabu_length;

        tabu = (int *)malloc(tabu_length * sizeof(int));

        for (int i = 0; i < tabu_length; i++)
        {
            tabu[i] = -1;
        }
    }

    ~TabuSearch()
    {
        // solution->~Solution();
        // free(solution);

        // free(tabu);
    }

    bool isInTabu(int n)
    {
        for (int i = 0; i < tabu_length; i++)
        {
            if (tabu[i] == n)
                return true;
        }

        return false;
    }

    void appendToTabu(int n)
    {
        if (isInTabu(n))
            return;

        for (int i = tabu_length - 1; i > 0; i--)
        {
            tabu[i] = tabu[i - 1];
        }

        tabu[0] = n;
    }

    void printTabu()
    {
        for (int i = 0; i < tabu_length; i++)
        {
            printf("%d ", tabu[i]);
        }

        printf("\n");
    }

    void actualValue()
    {
        printf("c:\t%0.f\n", solution->getEvaluation());
    }

    Solution *compute(int steps = 10, bool debug = true)
    {
        Solution *best = (Solution *)malloc(sizeof(solution));
        *best = Solution(initial_solution);

        printf("i\tc\tbest\tvalid\n");
        printf("0\t%.0f\t%.0f\t%d\n",
               initial_solution->getEvaluation(),
               initial_solution->getEvaluation(),
               initial_solution->isValid());

        for (int i = 0; i < steps; i++)
        {
            int v = step(debug);

            appendToTabu(v);

            initial_solution = solution;

            printf("%d\t%.0f\t%.0f\t%d",
                   i,
                   solution->getEvaluation(),
                   best->getEvaluation(),
                   solution->isValid());

            if (solution->getEvaluation() < best->getEvaluation())
            {
                *best = *solution;
                printf("\tBEST");
            }

            printf("\n");

            // solution->printXTable();
        }

        return best;
    }

    int step(bool debug = true)
    {
        int min_c = INT_MAX;
        int min_mov = -1;
        int min_dir = -1;
        int **min_X = (int **)malloc(instance->getNPlanes() * sizeof(int *));
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            min_X[i] = (int *)malloc(2 * sizeof(int));
        }

        int ext_min_c = INT_MAX;
        int ext_min_mov = -1;
        int ext_min_dir = -1;
        int **ext_min_X = (int **)malloc(instance->getNPlanes() * sizeof(int *));
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            ext_min_X[i] = (int *)malloc(2 * sizeof(int));
        }

        int **X = (int **)malloc(instance->getNPlanes() * sizeof(int *));
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            X[i] = (int *)malloc(2 * sizeof(int));
        }

        Solution *temp_solution = (Solution *)malloc(sizeof(Solution));

        if (debug)
            printf("plane\torig\tfinal\tc\n");

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            if (isInTabu(i))
                continue;

            for (int j = 0; j < instance->getNPlanes(); j++)
            {
                memcpy(X[j], initial_solution->getX()[j], 2 * sizeof(int));
            }

            int plane = X[i][0];
            int time = X[i][1];

            if (debug)
                printf("%d\t%d", plane, time);

            if (time > 0)
            {
                if (i > 0)
                {
                    if (X[i - 1][1] == X[i][1] - 1)
                    {
                        int t_plane = X[i - 1][0];

                        X[i - 1][0] = X[i][0];

                        X[i][0] = t_plane;
                    }
                    else
                    {
                        X[i][1] -= 1;
                    }
                }
                else
                {
                    X[i][1] -= 1;
                }
            }

            if (debug)
                printf("\t%d\t", X[i][1]);

            *temp_solution = Solution(instance, X, initial_solution->getLandedPlanes());

            int c = temp_solution->evaluation();

            if (c < min_c)
            {
                min_c = c;
                min_mov = i;
                min_dir = 0;

                for (int j = 0; j < instance->getNPlanes(); j++)
                {
                    memcpy(min_X[j], X[j], 2 * sizeof(int));
                }
            }

            if (debug)
                printf("%d\n", c);

            if (debug)
                printf("%d\t%d", plane, time);

            for (int j = 0; j < instance->getNPlanes(); j++)
            {
                memcpy(X[j], initial_solution->getX()[j], 2 * sizeof(int));
            }

            if (i < instance->getNPlanes() - 1)
            {
                if (X[i + 1][1] == X[i][1] + 1)
                {
                    int t_plane = X[i + 1][0];

                    X[i + 1][0] = X[i][0];

                    X[i][0] = t_plane;
                }
                else
                {
                    X[i][1] += 1;
                }
            }
            else
            {
                X[i][1] += 1;
            }

            if (debug)
                printf("\t%d\t", X[i][1]);

            *temp_solution = Solution(instance, X, initial_solution->getLandedPlanes());

            c = temp_solution->evaluation();

            if (c < min_c)
            {
                min_c = c;
                min_mov = i;
                min_dir = 0;

                for (int j = 0; j < instance->getNPlanes(); j++)
                {
                    memcpy(min_X[j], X[j], 2 * sizeof(int));
                }
            }

            if (debug)
                printf("%d\n", c);
        }

        *solution = Solution(instance, min_X, initial_solution->getLandedPlanes());

        if (debug)
            printf("MIN\t\t\t%d\n\n", min_c);

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            // free(min_X[i]);
            // free(ext_min_X[i]);
            // free(X[i]);
        }

        // free(X);
        // free(min_X);
        // free(ext_min_X);

        // temp_solution->~Solution();
        // free(temp_solution);

        return min_mov;
    }
};

#endif