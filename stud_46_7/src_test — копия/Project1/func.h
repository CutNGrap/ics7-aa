#ifndef FUNC
#define FUNC

#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <queue>

#define OK 0


using namespace std;

void parse_full_combinations();
void parse_ant_alg();

pair<double, vector<int>> full_combinations(vector<vector<double>> &matrix);
void read_coefs(double& alpha, double& beta, double& ro, int& days);

pair<double, vector<int>> ant_alg(vector<vector<double>>& matrix, double alpha, double beta, double ro, int days);

double calc_q(vector<vector<double>> &matrix, int size);
vector<vector<double>> calc_pheromones(int size);
vector<vector<double>> calc_visibility(vector<vector<double>> &matrix, int size);
vector<vector<int>> calc_visited_places(vector<int> &route, int ants);
vector<double> find_ways(vector<vector<double>>& pheromones, vector<vector<double>>& visibility, vector<vector<int>>& visited, int places, int ant, double alpha, double beta);
int choose_next_place_by_possibility(vector<double>& pk);
double calc_length(vector<vector<double>>& matrix, vector<int>& route);
vector<vector<double>> update_pheromones(vector<vector<double>>& matrix, int places, vector<vector<int>>& visited, vector<vector<double>> pheromones, double q, double ro);

void parse_all();
vector<vector<double>> read_matrix(string filename);


vector<vector<double>> read_matrix();
double summ(vector<double>& arr);
void parametrization();

void my_time();


#endif