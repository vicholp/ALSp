#ifndef ALSP
#define ALSP

#include <bits/stdc++.h>

using namespace std;

class Instance
{
private:
    int n_planes;

    int *E;
    int *T;
    int *L;
    double *h;
    double *g;

    int **S;

    int max_m = 0;

public:
    Instance(int n_planes, int *E, int *T, int *L, double *h, double *g, int **S)
    {
        this->n_planes = n_planes;

        this->E = (int *)malloc(n_planes * sizeof(int));
        memcpy(this->E, E, n_planes * sizeof(int));

        this->T = (int *)malloc(n_planes * sizeof(int));
        memcpy(this->T, T, n_planes * sizeof(int));

        this->L = (int *)malloc(n_planes * sizeof(int));
        memcpy(this->L, L, n_planes * sizeof(int));

        this->h = (double *)malloc(n_planes * sizeof(double));
        memcpy(this->h, h, n_planes * sizeof(double));

        this->g = (double *)malloc(n_planes * sizeof(double));
        memcpy(this->g, g, n_planes * sizeof(double));

        this->S = (int **)malloc(n_planes * sizeof(int *));

        for (int i = 0; i < n_planes; i++)
        {
            this->S[i] = (int *)malloc(n_planes * sizeof(int));
            memcpy(this->S[i], S[i], n_planes * sizeof(int));
        }

        for (int i = 0; i < n_planes; i++)
        {
            int a = (T[i] - E[i]) * g[i];
            int b = (L[i] - T[i]) * h[i];

            max_m += max(a, b);
        }
    }

    ~Instance()
    {
        // free(E);
        // free(T);
        // free(L);
        // free(h);
        // free(g);

        for (int i = 0; i < n_planes; i++)
        {
            // free(S[i]);
        }

        // free(S);
    }

    int getE(int i)
    {
        return E[i];
    }

    int getT(int i)
    {
        return T[i];
    }

    int getL(int i)
    {
        return L[i];
    }

    double getH(int i)
    {
        return h[i];
    }

    double getG(int i)
    {
        return g[i];
    }

    int getS(int i, int j)
    {
        return S[i][j];
    }

    int getNPlanes()
    {
        return n_planes;
    }

    bool is_valid()
    {
        return true;
    }

    int getMaxM()
    {
        return max_m;
    }

    int getMaxN()
    {
        return max_m / n_planes;
    }

    void print()
    {
        cout << "n_planes: " << n_planes << endl;

        cout << "E: ";
        for (int i = 0; i < n_planes; i++)
        {
            cout << E[i] << " ";
        }
        cout << endl;
    }
};

#endif