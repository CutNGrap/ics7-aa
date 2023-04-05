#include <stdio.h>
#include "func.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <utility>
#include <limits>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

#define MIN_PHEROMONE 0.01




vector<vector<double>> update_pheromones(vector<vector<double>> *matrix, int places, vector<vector<int>> *visited, vector<vector<double>> *pheromones, double q, double ro)
{
    int ants = places;
    for (int i = 0; i < places; ++i)
        for (int j = 0; j < places; ++j)
        {
            *(pheromones)[i][j] *= (1 - ro);
            if (*(pheromones)[i][j] < MIN_PHEROMONE)
                *(pheromones)[i][j] = MIN_PHEROMONE;
        }
    for (int ant = 0; ant < ants; ++ant)
    {
        double delta = 0;
        double len = 0;
        for (int way_ln = 1; way_ln < visited[ant].size(); ++way_ln)
            len += matrix[visited[ant][way_ln - 1]][visited[ant][way_ln]];
        delta = q / len;

        for (int way_ln = 1; way_ln < visited[ant].size(); ++way_ln)
            pheromones[visited[ant][way_ln - 1]][visited[ant][way_ln]] += delta;
    }
    return pheromones;
}

double calc_length(vector<vector<double>>& matrix, vector<int>& route)
{
    double ln = 0;
    for (int way_ln = 1; way_ln < route.size(); ++way_ln)
        ln += matrix[route[way_ln - 1]][route[way_ln]];
    return ln;
}

int choose_next_place_by_possibility(vector<double>& pk)
{
    double possibility = (((double)rand() + 1) / RAND_MAX);
    double choice = 0;
    int chosen_place = 0;

    while (choice < possibility && chosen_place < pk.size())
    {
        choice += pk[chosen_place];
        ++chosen_place;
    }
    if (chosen_place == 0)
        cout << 1;
    return chosen_place;
}

double calc_q(vector<vector<double>>& matrix, int size)
{
    double q = 0;
    int count = 0;
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            if (i != j)
            {
                q += matrix[i][j];
                ++count;
            }
    return q / count;
}

vector<vector<double>> calc_pheromones(int size)
{
    int min_phero = 1;
    vector<vector<double>> v(size, vector<double>(size, min_phero));
    return v;
}

vector<vector<double>> calc_visibility(vector<vector<double>>& matrix, int size)
{
    vector<vector<double>> v(size, vector<double>(size, 1));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
        {
            if (i == j)
                v[i][j] = 0;
            else
                v[i][j] = 1.0 / matrix[i][j];
        }
    return v;
}


vector<vector<int>> calc_visited_places(vector<int>& route, int ants)
{
    vector<vector<int>> visited(ants, vector<int>());
    for (int ant = 0; ant < ants; ++ant)
        visited[ant].push_back(route[ant]);
    return visited;
}

vector<double> find_ways(vector<vector<double>>& pheromones, vector<vector<double>>& visibility, vector<vector<int>>& visited, int places, int ant, double alpha, double beta)
{
    vector<double> pk(places, 0);
    for (int place = 0; place < places; ++place)
    {
        if (find(visited[ant].begin(), visited[ant].end(), place) == visited[ant].end())
        {
            int ant_place = visited[ant][visited[ant].size() - 1];
            pk[place] = pow(pheromones[ant_place][place], alpha) * \
                pow(visibility[ant_place][place], beta);
        }
        else
            pk[place] = 0;
    }
    double sum_pk = summ(pk);
    for (int place = 0; place < places; ++place)
        pk[place] /= sum_pk;
    return pk;
}

vector<double> find_ways(vector<vector<double>>& pheromones, vector<vector<double>>& visibility, vector<int>& visited, int places, int ant, double alpha, double beta)
{
    vector<double> pk(places, 0);
    for (int place = 0; place < places; ++place)
    {
        if (find(visited.begin(), visited.end(), place) == visited.end())
        {
            int ant_place = visited[visited.size() - 1];
            pk[place] = pow(pheromones[ant_place][place], alpha) * \
                pow(visibility[ant_place][place], beta);
        }
        else
            pk[place] = 0;
    }
    double sum_pk = summ(pk);
    for (int place = 0; place < places; ++place)
        pk[place] /= sum_pk;
    return pk;
}

double summ(vector<double>& arr)
{
    if (arr.size() == 0) { //this is the edge case
        return 0;
    }

    double s = 0;
    for (auto el : arr)
        s += el;
    return s;
}


void parse_all()
{
    vector<vector<double>> matrix = read_matrix();

    double alpha, beta, ro;
    int days;
    read_coefs(alpha, beta, ro, days);
    pair<double, vector<int>> result = full_combinations(matrix);
    cout << "Алгоритм полного перебора -- Минимальная длина пути: " << result.first << endl;
    cout << "Алгоритм полного перебора -- Самый короткий путь: ";
    for (auto s : result.second)
        cout << s << ' ';
    cout << endl;
    result = ant_alg(matrix, alpha, beta, ro, days);
    cout << "Муравьиный алгоритм -- Минимальная длина пути: " << result.first << endl;
    cout << "Муравьиный алгоритм -- Самый короткий путь: ";
    for (auto s : result.second)
        cout << s << ' ';
}