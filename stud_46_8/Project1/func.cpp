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
#define THREADS 2

void parse_full_combinations()
{
    vector<vector<double>> matrix = read_matrix();
 
    pair<double, vector<int>> result = full_combinations(matrix);
    cout << "Минимальная длина пути: " << result.first << endl;
    cout << "Самый короткий путь: ";
    for (auto s : result.second)
        cout << s << ' ';
}

void parse_ant_alg_thr()
{
    vector<vector<double>> matrix = read_matrix();

    double alpha, beta, ro;
    int days;
    read_coefs(alpha, beta, ro, days);
    pair<double, vector<int>> result = ant_alg_thr1(matrix, alpha, beta, ro, days, 8);
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
        for (int j = 0; j < matrix.size(); ++j)
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

pair<double, vector<int>> ant_alg_thr(vector<vector<double>> &matrix, double alpha, double beta, double ro, int days)
{
    mutex m;
    int places = matrix.size();
    parameters arg;
    arg.places = places;
    arg.alpha = alpha;
    arg.mute = &m;
    arg.matrix = matrix;
    arg.q = calc_q(arg.matrix, places);
    arg.min_dist = numeric_limits<double>::max();
    arg.pheromones = calc_pheromones(places);
    arg.visibility = calc_visibility(arg.matrix, places);
    int ants = places;
    for (int day = 0; day < days; ++day)
    {
        vector<int> route(places);
        iota(begin(route), end(route), 0);
        arg.visited = calc_visited_places(route, ants);
        arg.sync = 0;

        auto start1 = chrono::high_resolution_clock::now();
        vector<thread> threads;
        for (int thr_count = 0; thr_count < THREADS; ++thr_count)
            threads.push_back(thread(thread_work, std::ref(arg)));
        auto start2 = chrono::high_resolution_clock::now();
        for (int thr_count = 0; thr_count < THREADS; ++thr_count)
            threads[thr_count].join();
        auto start3 = chrono::high_resolution_clock::now();
        auto i = chrono::duration_cast<std::chrono::microseconds>(start2 - start1).count();
        auto j = chrono::duration_cast<std::chrono::microseconds>(start3 - start2).count();
        arg.pheromones = update_pheromones(arg.matrix, places, arg.visited, arg.pheromones, arg.q, ro);
    }
    return pair<double, vector<int>>(arg.min_dist, arg.best_way);
}

pair<double, vector<int>> ant_alg_thr1(vector<vector<double>>& matrix, double alpha, double beta, double ro, int days, int thrs)
{
    mutex m, m1;
    int places = matrix.size();
    int ants = places;
    vector<thread> threads;
    vector<parameters> arg(thrs);
    int delta = days / thrs;
    double min_dist = numeric_limits<double>::max();
    vector<int> best_way;
    auto phero = calc_pheromones(places);
    auto vis = calc_visibility(matrix, places);
    for (int th = 0; th < thrs; ++th)
    {
        arg[th].places = places;
        arg[th].alpha = alpha;
        arg[th].matrix = matrix;
        arg[th].q = calc_q(arg[th].matrix, places);
        arg[th].min_dist = numeric_limits<double>::max();
        arg[th].pheromones = phero;
        arg[th].visibility = vis;
        arg[th].ro = ro;
        arg[th].mute1 = &m1;
        arg[th].mute = &m;
        arg[th].min_dist = min_dist;
        arg[th].best_way = best_way;
        if (th < thrs - 1)
            arg[th].to_do = delta;
        else
            arg[th].to_do = delta + days % thrs;
        threads.push_back(thread(thread_work1, ref(arg[th])));
    }
    for (int th = 0; th < thrs; ++th)
        threads[th].join();
    return pair<double, vector<int>>(min_dist, best_way);
}

void thread_work1(parameters& arg)
{
    srand(time(nullptr));
    int ants = arg.places;
    double beta = 1 - arg.alpha;
    for (int i = 0; i < arg.to_do; ++i)
    {
        vector<int> route(arg.places);
        iota(begin(route), end(route), 0);
        arg.visited = calc_visited_places(route, ants);
        for (int ant = 0; ant < ants; ++ant)
        {
            while (arg.visited[ant].size() != ants)
            {
                vector<double> pk = find_ways(arg.pheromones, arg.visibility, arg.visited, arg.places, ant, arg.alpha, beta);
                int chosen_place = choose_next_place_by_possibility(pk);
                arg.visited[ant].push_back(chosen_place - 1);
            }
            double cur_length = calc_length(arg.matrix, arg.visited[ant]);
            if (cur_length < arg.min_dist)
            {
                arg.mute->lock();
                if (cur_length < arg.min_dist)
                {
                    arg.min_dist = cur_length;
                    arg.best_way = arg.visited[ant];
                }
                arg.mute->unlock();
            }
        }
        auto p = update_pheromones(arg.matrix, arg.places, arg.visited, arg.pheromones, arg.q, arg.ro);
        arg.mute1->lock();
        arg.pheromones = p;
        arg.mute1->unlock();
    }
}

void thread_work(parameters& arg)
{
    srand(time(nullptr));
    int ant;
        int counter = 1;
    while ((ant = arg.sync.fetch_add(1)) < arg.places) // 1: 1-N, 2: 2-N, 3: 3-N,..
    {
        int ants = arg.places;
        double beta = 1 - arg.alpha;
        while (arg.visited[ant].size() != ants)
        {
            vector<double> pk = find_ways(arg.pheromones, arg.visibility, arg.visited, arg.places, ant, arg.alpha, beta);
            int chosen_place = choose_next_place_by_possibility(pk);
            arg.visited[ant].push_back(chosen_place - 1);
        }
        double cur_length = calc_length(arg.matrix, arg.visited[ant]);

        if (cur_length < arg.min_dist)
        {
            arg.mute->lock();
            if (cur_length < arg.min_dist)
            {
                arg.min_dist = cur_length;
                arg.best_way = arg.visited[ant];
            }
            arg.mute->unlock();
        }
        ++counter;
    
    }
}

pair<double, vector<int>> ant_alg(vector<vector<double>>& matrix, double alpha, double beta, double ro, int days)
{
    int places = matrix.size();
    parameters arg;
    arg.places = places;
    arg.alpha = alpha;
    arg.matrix = matrix;
    arg.q = calc_q(arg.matrix, places);
    arg.min_dist = numeric_limits<double>::max();
    arg.pheromones = calc_pheromones(places);
    arg.visibility = calc_visibility(arg.matrix, places);
    int ants = places;
    for (int i = 0; i < days; ++i)
    {
        vector<int> route(arg.places);
        iota(begin(route), end(route), 0);
        arg.visited = calc_visited_places(route, ants);
        arg.sync = 0;
        for (int ant = 0; ant < ants; ++ant)
        {
            while (arg.visited[ant].size() != ants)
            {
                vector<double> pk = find_ways(arg.pheromones, arg.visibility, arg.visited, arg.places, ant, arg.alpha, beta);
                int chosen_place = choose_next_place_by_possibility(pk);
                arg.visited[ant].push_back(chosen_place - 1);
            }
            double cur_length = calc_length(arg.matrix, arg.visited[ant]);

            if (cur_length < arg.min_dist)
            {
                arg.min_dist = cur_length;
                arg.best_way = arg.visited[ant];
            }
        }
        arg.pheromones = update_pheromones(arg.matrix, arg.places, arg.visited, arg.pheromones, arg.q, arg.ro);
    }
    return pair<double, vector<int>>(arg.min_dist, arg.best_way);
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

vector<vector<double>> update_pheromones1(vector<vector<double>>& matrix, int places, vector<vector<int>>& visited, vector<vector<double>> pheromones, double q, double ro)
{
    int ants = places;
    for (int i = 0; i < places; ++i)
        for (int j = 0; j < places; ++j)
        {
            pheromones[i][j] *= (1 - ro);
            if (pheromones[i][j] < MIN_PHEROMONE)
                pheromones[i][j] = MIN_PHEROMONE;
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

int choose_next_place_by_possibility(vector<double> &pk)
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