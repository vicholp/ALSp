#include <bits/stdc++.h>

#include "instance.cpp"
#include "penalization.cpp"
#include "solution.cpp"
#include "greedy.cpp"
#include "tabu_search.cpp"

using namespace std;

int main()
{
    int n_planes;

    cin >> n_planes;

    int *E = (int *)malloc(n_planes * sizeof(int));
    int *T = (int *)malloc(n_planes * sizeof(int));
    int *L = (int *)malloc(n_planes * sizeof(int));
    double *h = (double *)malloc(n_planes * sizeof(double));
    double *g = (double *)malloc(n_planes * sizeof(double));

    int **S = (int **)malloc(n_planes * sizeof(int *));

    for (int i = 0; i < n_planes; i++)
    {
        S[i] = (int *)malloc(n_planes * sizeof(int));
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

    greedy_solution->printXTable();

    // Luego de esto se usa tabu search

    TabuSearch tabu = TabuSearch(&instance, greedy_solution, instance.getNPlanes() * 0.7);

    // Para desactivar puedes usar tabu.compute(0, false)
    // para que realice 0 iteraciones
    Solution *solution = tabu.compute(1000, false);
    solution->printXTable();

    printf("\n");
    if (greedy_solution->isValid())
    {
        printf("valid solution\n");
    }
    else
    {
        printf("invalid solution\n");
    }

    printf("initial solution: %f\n", greedy_solution->getEvaluation());
    printf("final solution: %f\n", solution->getEvaluation());

    if (solution->isValid())
    {
        printf("valid solution\n");
    }
    else
    {
        printf("invalid solution\n");
    }

    printf("\n");

    free(E);
    free(T);
    free(L);
    free(h);
    free(g);

    for (int i = 0; i < n_planes; i++)
    {
        free(S[i]);
    }

    free(S);

    cout << "END" << endl;
}