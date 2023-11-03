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

class Solution
{

private:
    int **X;
    Instance *instance;
    bool *landed_planes;

public:
    Solution(Instance *instance)
    {
        this->instance = (Instance *)malloc(sizeof(Instance));
        memcpy(this->instance, instance, sizeof(Instance));

        this->landed_planes = (bool *)malloc(instance->getNPlanes() * sizeof(bool));

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            this->landed_planes[i] = false;
        }

        this->X = (int **)malloc(instance->getNPlanes() * sizeof(int *));

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            this->X[i] = (int *)malloc(2 * sizeof(int));

            this->X[i][0] = -1;
            this->X[i][1] = 0;
        }
    }

    Solution(Instance *instance, int **X, bool *landed_planes)
    {
        this->instance = (Instance *)malloc(sizeof(Instance));
        memcpy(this->instance, instance, sizeof(Instance));

        this->landed_planes = (bool *)malloc(instance->getNPlanes() * sizeof(bool));
        memcpy(this->landed_planes, landed_planes, instance->getNPlanes() * sizeof(bool));

        this->X = (int **)malloc(instance->getNPlanes() * sizeof(int *));
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            this->X[i] = (int *)malloc(2 * sizeof(int));
            memcpy(this->X[i], X[i], 2 * sizeof(int));
        }
    }

    Solution(Solution *solution)
    {
        this->instance = (Instance *)malloc(sizeof(Instance));
        memcpy(this->instance, solution->getInstance(), sizeof(Instance));

        this->landed_planes = (bool *)malloc(instance->getNPlanes() * sizeof(bool));
        memcpy(this->landed_planes, solution->getLandedPlanes(), instance->getNPlanes() * sizeof(bool));

        this->X = (int **)malloc(instance->getNPlanes() * sizeof(int *));
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            this->X[i] = (int *)malloc(2 * sizeof(int));
            memcpy(this->X[i], solution->getX()[i], 2 * sizeof(int));
        }
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
        int c = 0;

        // penalization by early or late arrival
        for (int i = 0; i < instance->getNPlanes(); i++)
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
                c += dif * instance->getH(plane);
            }
            else
            {
                c += -dif * instance->getG(plane);
            }
        }

        // penalization by landing out of limit

        bool is_invalid = false;

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            int plane = X[i][0];
            int time = X[i][1];

            int dif = time - instance->getL(plane);

            if (dif > 0)
            {
                is_invalid = true;

                c += instance->getMaxN() * dif;
            }

            dif = instance->getE(plane) - time;

            if (dif > 0)
            {
                is_invalid = true;

                c += instance->getMaxN() * dif;
            }
        }

        // penalization by restriction

        for (int i = 1; i < instance->getNPlanes(); i++)
        {
            int plane = X[i][0];
            int time = X[i][1];

            int plane_before = X[i - 1][0];
            int time_before = X[i - 1][1];

            int min_separation = instance->getS(plane_before, plane);

            if (min_separation > time - time_before)
            {
                is_invalid = true;

                c += instance->getMaxN() * (time - time_before);
            }
        }

        if (is_invalid)
        {
            c += instance->getMaxM();
        }

        return c;
    }

    void printX()
    {
        for (int i = 0; i < this->instance->getNPlanes(); i++)
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
        cout << endl
             << "pos\tplane\ttime\tmin\ttarget\tlate\tp1\tp2\tp3" << endl;

        for (int i = 0; i < this->instance->getNPlanes(); i++)
        {
            int plane = X[i][0];
            int time = X[i][1];

            if (plane == -1)
            {
                cout << i << " - x x" << endl;
                continue;
            }

            bool out_of_time = X[i][1] > instance->getL(plane);

            float pen1 = 0;
            float pen2 = 0;
            float pen3 = 0;

            int dif = time - instance->getT(plane);

            if (dif > 0)
            {
                pen1 = dif * instance->getH(plane);
            }
            else
            {
                pen1 = -dif * instance->getG(plane);
            }

            dif = time - instance->getL(plane);

            if (dif > 0)
            {

                pen2 = instance->getMaxN() * dif;
            }

            dif = instance->getE(plane) - time;

            if (dif > 0)
            {
                pen2 = instance->getMaxN() * dif;
            }

            if (i > 0)
            {

                int plane_before = X[i - 1][0];
                int time_before = X[i - 1][1];

                int min_separation = instance->getS(plane_before, plane);

                if (min_separation > time - time_before)
                {
                    pen3 = instance->getMaxN() * (time - time_before);
                }
            }

            int p1 = (int)pen1;
            int p2 = (int)pen2;
            int p3 = (int)pen3;

            cout << i << "\t" << plane + 1 << "\t" << time << "\t" << instance->getE(plane) << "\t" << instance->getT(plane) << "\t" << instance->getL(plane) << "\t" << p1 << "\t" << p2 << "\t" << p3 << endl;
        }

        cout << "t"
             << "\t"
             << "-"
             << "\t"
             << "-"
             << "\t"
             << "-"
             << "\t"
             << "-"
             << "\t"
             << "-"
             << "\t" << evaluation() << "\t"
             << "-" << endl
             << endl;
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
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            if (landed_planes[i] = false)
            {
                return false;
            }
        }

        return true;
    }
};

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
        this->instance = (Instance *)malloc(sizeof(Instance));
        memcpy(this->instance, instance, sizeof(Instance));

        this->landed_planes = (bool *)malloc(instance->getNPlanes() * sizeof(bool));

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            this->landed_planes[i] = false;
        }

        this->X = (int **)malloc(instance->getNPlanes() * sizeof(int *));

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            this->X[i] = (int *)malloc(2 * sizeof(int));

            this->X[i][0] = -1;
            this->X[i][1] = 0;
        }
    }

    void compute(bool debug = false)
    {
        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            step(debug);
        }
    }

    Solution *getSolution()
    {
        return new Solution(instance, X, landed_planes);
    }

private:
    void step(bool debug = false)
    {
        if (debug)
            cout << "I: " << pos << endl;

        int last_plane = -1;
        int last_land = 0;

        if (pos > 0)
        {
            last_plane = X[pos - 1][0];
            last_land = X[pos - 1][1];
        }

        if (debug)
            cout << "   last plane: " << last_plane << endl;
        if (debug)
            cout << "   last land : " << last_land << endl;

        int earlier_time = INT_MAX;
        int earlier_plane = -1;
        int min_penalization = INT_MAX;
        int min_out_of_time = INT_MAX;

        for (int plane = 0; plane < instance->getNPlanes(); plane++)
        {
            if (this->landed_planes[plane])
                continue;

            if (debug)
                cout << "    plane:" << plane << endl;

            int early = instance->getE(plane);
            int target = this->instance->getT(plane);
            int late = instance->getL(plane);

            if (debug)
                cout << "       " << early << " " << target << " " << late << endl;

            int min_valid_land = 0;
            if (last_plane != -1)
            {
                min_valid_land = last_land + instance->getS(last_plane, plane);
                if (debug)
                    cout << "       min_valid_land " << min_valid_land << endl;
            }

            if (target >= min_valid_land)
            {
                // if it better than previous
                if (target < earlier_time)
                {
                    earlier_time = target;
                    earlier_plane = plane;
                    min_penalization = 0;
                    min_out_of_time = 0;
                }

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
            }
        }

        if (debug)
        {

            cout << "   earlier_plane: " << earlier_plane << endl;
            cout << "   earlier time: " << earlier_time << endl;
            cout << "   penalization: " << min_penalization << endl;
            cout << "   out_of_time: " << min_out_of_time << endl;
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

class TabuSearch
{
private:
    Instance *instance;
    Solution *solution;

public:
    TabuSearch(Instance *instance, Solution *initial_solution)
    {
        this->instance = (Instance *)malloc(sizeof(Instance));
        memcpy(this->instance, instance, sizeof(Instance));

        this->solution = new Solution(initial_solution);
    }

    void actualValue()
    {
        cout << fixed << "c: \t" << solution->evaluation() << endl;
    }

    Solution *getNeib()
    {
        int min_c = INT_MAX;
        int min_mov = -1;
        int min_dir = -1;

        int ****X = (int ****)malloc(instance->getNPlanes() * sizeof(int ***));

        for (int i = 0; i < instance->getNPlanes(); i++)
        {
            // cout << "i:" << i << endl;
            X[i] = (int ***)malloc(2 * sizeof(int **));

            X[i][0] = (int **)malloc(instance->getNPlanes() * sizeof(int *));
            for (int j = 0; j < instance->getNPlanes(); j++)
            {
                X[i][0][j] = (int *)malloc(2 * sizeof(int));
                memcpy(X[i][0][j], solution->getX()[j], 2 * sizeof(int));
            }

            int time = X[i][0][i][1];
            int plane = X[i][0][i][0];

            if (time > 0)
            {
                if (i > 0)
                {
                    if (X[i][0][i - 1][1] == X[i][0][i][1] - 1)
                    {
                        int t_plane = X[i][0][i - 1][0];

                        X[i][0][i - 1][0] = X[i][0][i][0];

                        X[i][0][i][0] = t_plane;
                    }
                    else
                    {
                        X[i][0][i][1] -= 1;
                    }
                }
                else
                {
                    X[i][0][i][1] -= 1;
                }
            }

            Solution *sa = new Solution(instance, X[i][0], solution->getLandedPlanes());

            int c = sa->evaluation();
            // cout << "a:" << c << endl;
            // sa->printXTable();

            if (c < min_c)
            {
                min_c = c;
                min_mov = i;
                min_dir = 0;
            }

            X[i][1] = (int **)malloc(instance->getNPlanes() * sizeof(int *));
            for (int j = 0; j < instance->getNPlanes(); j++)
            {
                X[i][1][j] = (int *)malloc(2 * sizeof(int));
                memcpy(X[i][1][j], solution->getX()[j], 2 * sizeof(int));
            }

            if (i < instance->getNPlanes() - 1)
            {
                if (X[i][1][i + 1][1] == X[i][1][i][1] + 1)
                {
                    int t_plane = X[i][1][i + 1][0];

                    X[i][1][i + 1][0] = X[i][1][i][0];

                    X[i][1][i][0] = t_plane;
                }
                else
                {
                    X[i][1][i][1] += 1;
                }
            }
            else
            {
                X[i][1][i][1] += 1;
            }

            Solution *sb = new Solution(instance, X[i][1], solution->getLandedPlanes());

            c = sb->evaluation();
            // sb->printXTable();
            // cout << "b:" << c << endl;

            if (c < min_c)
            {
                min_c = c;
                min_mov = i;
                min_dir = 1;
            }
        }

        // cout << "min_c " << min_c << endl;

        return new Solution(instance, X[min_mov][min_dir], solution->getLandedPlanes());
    }
};
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

    Instance *instance = new Instance(n_planes, E, T, L, h, g, S);

    Greedy *greedy = new Greedy(instance);
    greedy->compute(false);
    (greedy->getSolution())->printXTable();

    TabuSearch *tabu = new TabuSearch(instance, greedy->getSolution());

    Solution *solution;
    for (int i = 0; i < 1000; i++)
    {
        // tabu->actualValue();
        solution = tabu->getNeib();
        tabu = new TabuSearch(instance, solution);
    }

    solution->printXTable();

    cout
        << "END" << endl;
}