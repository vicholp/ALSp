#include <bits/stdc++.h>

#include "instance.cpp"
#include "penalization.cpp"
#include "solution.cpp"
#include "greedy.cpp"
#include "tabu_search.cpp"

using namespace std;

#define MAX_ITERATIONS 100000
#define MAX_TABU_SIZE 0.2

int main()
{
    int n_planes;

    cin >> n_planes;

    int *E = new int[n_planes];
    int *T = new int[n_planes];
    int *L = new int[n_planes];
    double *h = new double[n_planes];
    double *g = new double[n_planes];

    int **S = new int *[n_planes];
    for (int i = 0; i < n_planes; i++)
    {
        S[i] = new int[n_planes];
    }

    for (int i = 0; i < n_planes; i++)
    {
        cin >> E[i] >> T[i] >> L[i] >> g[i] >> h[i];

        for (int j = 0; j < n_planes; j++)
        {
            cin >> S[i][j];
        }
    }

    Instance instance = Instance(n_planes, E, T, L, h, g, S);

    Greedy greedy = Greedy(&instance);
    Solution *greedy_solution = greedy.compute(false);

    TabuSearch tabu = TabuSearch(&instance, greedy_solution,
                                 instance.getNPlanes() * MAX_TABU_SIZE);

    Solution *solution = tabu.compute(MAX_ITERATIONS, true, false);

    printf("\n");
    printf("initial solution: %f\n", greedy_solution->getEvaluation());
    printf("best solution: %f\n", solution->getEvaluation());

    printf("\n");
    if (greedy_solution->isValid())
    {
        printf("valid solution ->");
    }
    else
    {
        printf("invalid solution -> ");
    }
    if (solution->isValid())
    {
        printf(" valid solution\n");
    }
    else
    {
        printf(" invalid solution\n");
    }
    delete greedy_solution;
    delete solution;

    printf("\n");

    delete[] E;
    delete[] T;
    delete[] L;
    delete[] h;
    delete[] g;

    for (int i = 0; i < n_planes; i++)
    {
        delete[] S[i];
    }

    delete[] S;

    cout << "END" << endl;
}