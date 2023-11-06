#ifndef ALSP_SOLUTION
#define ALSP_SOLUTION

#include <bits/stdc++.h>
#include "instance.cpp"
#include "penalization.cpp"

class Solution
{

private:
    int **X;
    Instance *instance;
    bool *landed_planes;
    Penalization *penalization;
    int n_planes;

    bool evaluated = false;

public:
    Solution(Instance *instance)
    {
        n_planes = instance->getNPlanes();

        this->instance = (Instance *)malloc(sizeof(Instance));
        memcpy(this->instance, instance, sizeof(Instance));

        this->landed_planes = (bool *)malloc(n_planes * sizeof(bool));

        for (int i = 0; i < n_planes; i++)
        {
            this->landed_planes[i] = false;
        }

        this->X = (int **)malloc(n_planes * sizeof(int *));

        for (int i = 0; i < n_planes; i++)
        {
            this->X[i] = (int *)malloc(2 * sizeof(int));

            this->X[i][0] = -1;
            this->X[i][1] = 0;
        }

        this->penalization = (Penalization *)malloc(sizeof(Penalization));
        *penalization = Penalization(instance);
    }

    Solution(Instance *instance, int **X, bool *landed_planes)
    {
        this->instance = (Instance *)malloc(sizeof(Instance));
        memcpy(this->instance, instance, sizeof(Instance));

        n_planes = instance->getNPlanes();

        this->landed_planes = (bool *)malloc(n_planes * sizeof(bool));
        memcpy(this->landed_planes, landed_planes, n_planes * sizeof(bool));

        this->X = (int **)malloc(n_planes * sizeof(int *));
        for (int i = 0; i < n_planes; i++)
        {
            this->X[i] = (int *)malloc(2 * sizeof(int));
            memcpy(this->X[i], X[i], 2 * sizeof(int));
        }

        this->penalization = (Penalization *)malloc(sizeof(Penalization));
        *penalization = Penalization(instance);
    }

    Solution(Solution *solution)
    {
        this->instance = (Instance *)malloc(sizeof(Instance));
        memcpy(this->instance, solution->getInstance(), sizeof(Instance));

        n_planes = instance->getNPlanes();

        this->landed_planes = (bool *)malloc(n_planes * sizeof(bool));
        memcpy(this->landed_planes, solution->getLandedPlanes(), n_planes * sizeof(bool));

        this->X = (int **)malloc(n_planes * sizeof(int *));
        for (int i = 0; i < n_planes; i++)
        {
            this->X[i] = (int *)malloc(2 * sizeof(int));
            memcpy(this->X[i], solution->getX()[i], 2 * sizeof(int));
        }

        this->penalization = (Penalization *)malloc(sizeof(Penalization));
        *penalization = Penalization(instance);
    }

    ~Solution()
    {
        // free(this->landed_planes);

        for (int i = 0; i < n_planes; i++)
        {
            // free(this->X[i]);
        }
        // free(this->X);

        // penalization->~Penalization();
        // free(this->penalization);
    }

    Instance *getInstance()
    {
        return instance;
    }

    bool *getLandedPlanes()
    {
        return landed_planes;
    }

    int **getX()
    {
        return X;
    }

    double evaluation()
    {
        // penalization by early or late arrival
        for (int i = 0; i < n_planes; i++)
        {
            int plane = X[i][0];
            int time = X[i][1];

            int dif = time - instance->getT(plane);

            if (dif == 0)
            {
                continue;
            }

            if (dif > 0)
            {
                penalization->setDiffFromTarget(plane, dif * instance->getH(plane));
            }
            else
            {
                penalization->setDiffFromTarget(plane, -dif * instance->getG(plane));
            }
        }

        // penalization by invalid solution
        bool is_invalid = false;

        // penalization by landing out of limit
        for (int i = 0; i < n_planes; i++)
        {
            int plane = X[i][0];
            int time = X[i][1];

            int dif = time - instance->getL(plane);

            if (dif > 0)
            {
                is_invalid = true;

                penalization->setOutOfLimit(plane, instance->getMaxN() * dif * instance->getH(plane));
            }

            dif = instance->getE(plane) - time;

            if (dif > 0)
            {
                is_invalid = true;

                penalization->setOutOfLimit(plane, instance->getMaxN() * dif * instance->getG(plane));
            }
        }

        // penalization by separation

        for (int i = 1; i < n_planes; i++)
        {
            int plane = X[i][0];
            int time = X[i][1];

            int plane_before = X[i - 1][0];
            int time_before = X[i - 1][1];

            int min_separation = instance->getS(plane_before, plane);

            if (min_separation > time - time_before)
            {
                is_invalid = true;

                penalization->setNoSeparation(plane, instance->getMaxN() * (min_separation - (time - time_before)));
            }
        }

        evaluated = true;

        return penalization->getTotal();
    }
    float getEvaluation()
    {
        if (!evaluated)
        {
            evaluation();
        }

        return penalization->getTotal();
    }

    bool isValid()
    {
        if (!evaluated)
        {
            evaluation();
        }

        return !penalization->isInvalid();
    }

    void printX()
    {
        for (int i = 0; i < this->n_planes; i++)
        {
            if (X[i][0] == -1)
            {
                cout << i << " - x x" << endl;
                continue;
            }

            cout << i << " - " << X[i][0] + 1 << " " << X[i][1] << endl;
        }
    }

    void printXTable()
    {
        if (!evaluated)
        {
            evaluation();
        }

        printf("\n      i\t  plane\t   time\t  early\t target\t   late\t     p1\t     p2\t     p3\n");

        for (int i = 0; i < this->n_planes; i++)
        {
            int plane = X[i][0];
            int time = X[i][1];

            printf("%7d\t%7d\t%7d\t%7d\t%7d\t%7d\t%7.0f\t%7.0f\t%7.0f\n",
                   i + 1,
                   plane + 1,
                   time,
                   instance->getE(plane),
                   instance->getT(plane),
                   instance->getL(plane),
                   penalization->getDiffFromTarget(plane),
                   penalization->getOutOfLimit(plane),
                   penalization->getNoSeparation(plane));
        }

        printf("  TOTAL\t-\t-\t-\t-\t-\t%7.0f\n\n", penalization->getTotal());
    }

    void printC()
    {
        cout << fixed << "c:\t" << evaluation() << endl;
        cout << "max_m:\t" << instance->getMaxM() << endl;
    }

    /**
     * true if all the planes landed or not
     */
    bool isComplete()
    {
        for (int i = 0; i < n_planes; i++)
        {
            if (landed_planes[i] == false)
            {
                return false;
            }
        }

        return true;
    }
};

#endif