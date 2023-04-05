#include <stdio.h>
#include "func.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <utility>
#include <limits>


using namespace std;

#define MIN_PHEROMONE 0.01

void parse_full_combinations()
{
    vector<vector<double>> matrix = read_matrix();
 
    pair<double, vector<int>> result = full_combinations(matrix);
    cout << "Минимальная длина пути: " << result.first << endl;
    cout << "Самый короткий путь: ";
    for (auto s : result.second)
        cout << s << ' ';
}


pair<double, vector<int>> full_combinations(vector<vector<double>> &matrix)
{
    vector<int> places(matrix.size());
    iota(begin(places), end(places), 0);
    vector<vector<int>> places_combinations;
    do {
        places_combinations.push_back(places);
    } while (next_permutation(places.begin(), places.end()));
    vector<int> best_way;
    double min_dist = numeric_limits<double>::max();
    for (auto comb : places_combinations)
    {
        //comb.push_back(comb[0]);
        double cur_dist = 0;
        for (int j = 0; j < matrix.size() - 1; ++j)
        {
            int start_city = comb[j];
            int end_city = comb[j + 1];
            cur_dist += matrix[start_city][end_city];
        }
        if (cur_dist < min_dist)
        {
            min_dist = cur_dist;
            best_way = comb;
        }
    }
    return pair<double, vector<int>>(min_dist, best_way);
}

vector<vector<double>> read_matrix()
{
    printf("Введите название файла с матрицей: ");
    string filename;
    cin >> filename;
    return read_matrix(filename);
}

vector<vector<double>> read_matrix(string filename)
{
    vector<vector<double>> matrix;
    string s;
    ifstream file(filename);
    int count = 0;
    file >> count;
    if (count == 0)
    {
        cout << "В начале файла с матрицей должен быть указан размер матрицы, принимающий положительные значения!"<<endl;
        exit(1);
    }
    double t;
    for (int i = 0; i < count; i++)
    {
        vector<double> v;
        for (int j = 0; j < count; j++)
        {
            file >> t;
            v.push_back(t);
        }
        matrix.push_back(v);
    }
    file.close();
    return matrix;
}

void parse_ant_alg()
{
    vector<vector<double>> matrix = read_matrix();

    double alpha, beta, ro;
    int days;
    read_coefs(alpha, beta, ro, days);
    pair<double, vector<int>> result = ant_alg(matrix, alpha, beta, ro, days);
    cout << "Минимальная длина пути: " << result.first << endl;
    cout << "Самый короткий путь: ";
    for (auto s : result.second)
        cout << s << ' ';
}


void read_coefs(double& alpha, double& beta, double& ro, int& days)
{
    cout << "Введите коэффициент alpha: " << endl;
    cin >> alpha;
    beta = 1 - alpha;
     cout << "Введите коэффициент ro: " << endl;
     cin >> ro;
    cout << "Введите количество дней: " << endl;
    cin >> days;
}

pair<double, vector<int>> ant_alg(vector<vector<double>> &matrix, double alpha, double beta, double ro, int days)
{
    int places = matrix.size();
    double q = calc_q(matrix, places);
    vector<int> best_way;
    double min_dist = numeric_limits<double>::max();
    vector<vector<double>> pheromones = calc_pheromones(places);
    vector<vector<double>> visibility = calc_visibility(matrix, places);
    int ants = places;
    for (int day = 0; day < days; ++day)
    {
        vector<int> route(places);
        iota(begin(route), end(route), 0);
        vector<vector<int>> visited = calc_visited_places(route, ants);
        for (int ant = 0; ant < ants; ++ant)
        {
            while (visited[ant].size() != ants)
            {
                vector<double> pk = find_ways(pheromones, visibility, visited, places, ant, alpha, beta);
                int chosen_place = choose_next_place_by_possibility(pk);
                visited[ant].push_back(chosen_place - 1);
            }
            //visited[ant].push_back(visited[ant][0]);
            double cur_length = calc_length(matrix, visited[ant]);

            if (cur_length < min_dist)
            {
                min_dist = cur_length;
                best_way = visited[ant];
            }
        }
        pheromones = update_pheromones(matrix, places, visited, pheromones, q, ro);
    }
    return pair<double, vector<int>>(min_dist, best_way);
}

vector<vector<double>> update_pheromones(vector<vector<double>>& matrix, int places, vector<vector<int>>& visited, vector<vector<double>> pheromones, double q, double ro)
{
    int ants = places;
    for (int i = 0; i < places; ++i)
        for (int j = 0; j < places; ++j)
        {
            double delta = 0;
            for (int ant = 0; ant < ants; ++ant)
            {
                double len = calc_length(matrix, visited[ant]);
                delta += q / len;
            }
            pheromones[i][j] *= (1 - ro);
            pheromones[i][j] += delta;
            if (pheromones[i][j] < MIN_PHEROMONE)
                pheromones[i][j] = MIN_PHEROMONE;
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

int choose_next_place_by_possibility(vector<double> &pk)
{
    double possibility = (((double)rand() + 1) / (RAND_MAX));
    double choice = 1e-6;
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

double calc_q(vector<vector<double>> &matrix, int size)
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

vector<vector<double>> calc_visibility(vector<vector<double>> &matrix, int size)
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


vector<vector<int>> calc_visited_places(vector<int> &route, int ants)
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

double summ(vector<double> &arr)
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