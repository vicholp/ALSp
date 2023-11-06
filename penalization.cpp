#ifndef ALSP_PENALIZATION
#define ALSP_PENALIZATION

#include <bits/stdc++.h>
#include "instance.cpp"

using namespace std;

class Penalization
{
private:
    Instance *instance;
    float **p;
    float total;
    int n_planes;

    bool is_invalid = false;

public:
    Penalization(Instance *instance)
    {
        n_planes = instance->getNPlanes();
        this->instance = instance;
        p = (float **)malloc(n_planes * sizeof(float *));

        for (int i = 0; i < n_planes; i++)
        {
            p[i] = (float *)malloc(3 * sizeof(float));

            for (int j = 0; j < 3; j++)
            {
                p[i][j] = 0;
            }
        }
        total = 0;
    }

    ~Penalization()
    {
        for (int i = 0; i < n_planes; i++)
        {
            // free(p[i]);
        }

        // free(p);
    }

    void setDiffFromTarget(int plane, float penalization)
    {
        float dif = penalization - p[plane][0];
        p[plane][0] += penalization;
        total += dif;
    }

    void setOutOfLimit(int plane, float penalization)
    {
        float dif = penalization - p[plane][1];
        p[plane][1] += penalization;

        total += dif;

        is_invalid = true;
    }

    void setNoSeparation(int plane, float penalization)
    {
        float dif = penalization - p[plane][2];
        p[plane][2] += penalization;

        total += dif;

        is_invalid = true;
    }

    float getTotal()
    {
        if (is_invalid)
            return total + instance->getMaxM();

        return total;
    }

    bool isInvalid()
    {
        return is_invalid;
    }

    float getDiffFromTarget(int plane)
    {
        return p[plane][0];
    }

    float getOutOfLimit(int plane)
    {
        return p[plane][1];
    }

    float getNoSeparation(int plane)
    {
        return p[plane][2];
    }
};

#endif