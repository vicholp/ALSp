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
    Solution *best;
    int tabu_length = 0;
    int *tabu;

public:
    TabuSearch(Instance *instance, Solution *initial_solution, int tabu_length = 10)
    {
        this->instance = instance;

        this->initial_solution = new Solution(initial_solution);

        this->solution = new Solution(initial_solution);

        this->tabu_length = tabu_length;

        tabu = new int[tabu_length];

        for (int i = 0; i < tabu_length; i++)
        {
            tabu[i] = -1;
        }
    }

    ~TabuSearch()
    {
        delete solution;

        delete initial_solution;

        delete[] tabu;
    }

    bool isInTabu(int n)
    {
        if (n < 0 || n >= tabu_length)
            return false;

        for (int i = 0; i < tabu_length; i++)
        {
            if (tabu[i] == n)
                return true;
        }

        return false;
    }

    void appendToTabu(int n)
    {
        if (n < 0 || n >= tabu_length)
            return;

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

    Solution *compute(int steps = 10, bool progress = true, bool debug = false)
    {
        best = new Solution(initial_solution);

        if (progress)
        {
            printf("i\tc\tbest\tvalid\n");
            printf("0\t%.0f\t%.0f\t%d\n",
                   initial_solution->getEvaluation(),
                   initial_solution->getEvaluation(),
                   initial_solution->isValid());
        }

        for (int i = 0; i < steps; i++)
        {
            int v = step(debug);

            appendToTabu(v);

            delete initial_solution;
            initial_solution = new Solution(solution);

            if (progress)
            {
                printf("%d\t%.0f\t%.0f\t%d",
                       i + 1,
                       solution->getEvaluation(),
                       best->getEvaluation(),
                       solution->isValid());
            }

            if (solution->getEvaluation() < best->getEvaluation())
            {
                delete best;

                best = new Solution(solution);

                if (progress)
                    printf("\tBEST");
            }

            if (progress)
                printf("\n");
        }

        return best;
    }

    int step(bool debug = true)
    {
        int n_planes = instance->getNPlanes();

        int min_c = INT_MAX;
        int min_mov = -1;
        int min_dir = -1;
        int **min_X = new int *[n_planes];
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            min_X[i] = new int[2];
        }

        int ext_min_c = INT_MAX;
        int ext_min_mov = -1;
        int ext_min_dir = -1;
        int **ext_min_X = new int *[n_planes];
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            ext_min_X[i] = new int[2];
        }

        int **X = new int *[n_planes];
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            X[i] = new int[2];
        }

        Solution *temp_solution;

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

            temp_solution = new Solution(instance, X, initial_solution->getLandedPlanes());

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

            delete temp_solution;

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

            temp_solution = new Solution(instance, X, initial_solution->getLandedPlanes());

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

            delete temp_solution;
        }

        delete solution;
        solution = new Solution(instance, min_X, initial_solution->getLandedPlanes());

        if (debug)
            printf("MIN\t\t\t%d\n\n", min_c);

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            delete[] min_X[i];
            delete[] ext_min_X[i];
            delete[] X[i];
        }

        delete[] X;
        delete[] min_X;
        delete[] ext_min_X;

        return min_mov;
    }
};

#endif