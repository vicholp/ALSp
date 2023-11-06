#ifndef ALSP_GREEDY
#define ALSP_GREEDY

#include <bits/stdc++.h>
#include "instance.cpp"
#include "solution.cpp"

using namespace std;

class Greedy
{
private:
    int **X;
    Instance *instance;
    bool *landed_planes;
    int pos = 0;

public:
    Greedy(Instance *instance)
    {
        this->instance = instance;

        landed_planes = new bool[instance->getNPlanes()];

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            landed_planes[i] = false;
        }

        X = new int *[instance->getNPlanes()];

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            X[i] = new int[2];

            X[i][0] = -1;
            X[i][1] = 0;
        }
    }

    ~Greedy()
    {
        delete[] landed_planes;

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            delete[] X[i];
        }
        delete[] X;
    }

    Solution *compute(bool debug = false)
    {
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            step(debug);
        }

        return new Solution(instance, X, landed_planes);
    }

private:
    void step(bool debug = false)
    {
        if (debug)
            printf("I: %d\n", pos);

        int last_plane = -1;
        int last_land = 0;

        if (pos > 0)
        {
            last_plane = X[pos - 1][0];
            last_land = X[pos - 1][1];
        }

        if (debug)
        {
            printf("\tlast plane: \t%d\n", last_plane);
            printf("\tlast land: \t%d\n\n", last_land);
        }

        int earlier_time = INT_MAX;
        int earlier_plane = -1;
        int min_penalization = INT_MAX;
        int min_out_of_time = INT_MAX;

        if (debug)
            printf("\tplane\tearly\ttarget\tlate\tm_land\t----\n");

        for (int plane = 0; plane < instance->getNPlanes(); plane++)
        {
            if (this->landed_planes[plane])
                continue;

            if (debug)
                printf("\t%d", plane);

            int early = instance->getE(plane);
            int target = this->instance->getT(plane);
            int late = instance->getL(plane);

            if (debug)
                printf("\t%d\t%d\t%d", early, target, late);

            int min_valid_land = 0;
            if (last_plane != -1)
            {
                min_valid_land = last_land + instance->getS(last_plane, plane);
            }
            if (debug)
                printf("\t%d", min_valid_land);

            if (target >= min_valid_land)
            {
                if (target < earlier_time)
                {
                    earlier_time = target;
                    earlier_plane = plane;
                    min_penalization = 0;
                    min_out_of_time = 0;

                    if (debug)
                        printf("\tbest");
                }
                if (debug)
                    printf("\n");
                continue;
            }

            int penalization = (min_valid_land - target) * instance->getH(plane);
            int out_of_time = 0;

            if (min_valid_land > late)
            {
                out_of_time = min_valid_land - late;
            }

            if (out_of_time < min_out_of_time || penalization < min_penalization)
            {
                earlier_time = min_valid_land;
                earlier_plane = plane;
                min_penalization = penalization;
                min_out_of_time = out_of_time;
                if (debug)
                    printf("\tbest");
            }
            if (debug)
                printf("\n");
        }

        if (debug)
        {
            printf("\n");
            printf("   earlier_plane: %d\n", earlier_plane);
            printf("   earlier time: %d\n", earlier_time);
            printf("   penalization: %d\n", min_penalization);
            printf("   out_of_time: %d\n", min_out_of_time);
        }

        if (earlier_plane == -1)
        {
            return;
        }

        X[pos][0] = earlier_plane;
        X[pos][1] = earlier_time;
        landed_planes[earlier_plane] = true;

        pos++;

        if (debug)
            cout << endl;
    }
};

#endif