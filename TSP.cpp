#include <bits/stdc++.h>
#include <limits.h>
#include <ctime>
using namespace std;

// Number of cities in TSP
#define V 16 
  
// Starting Node Value
#define START 0
  
// Initial population size for the algorithm
#define POP_SIZE 10

// implementation of Dynamic Programming
int TSPDynamicProgramming(int graph[][V], int s)
{
    // store all vertex apart from source vertex
    vector<int> vertex;
    for (int i = 0; i < V; i++)
        if (i != s)
            vertex.push_back(i);
 
    // store minimum weight Hamiltonian Cycle.
    int min_path = INT_MAX;
    do {
 
        // store current Path weight(cost)
        int current_pathweight = 0;
 
        // compute current path weight
        int k = s;
        for (int i = 0; i < vertex.size(); i++) {
            current_pathweight += graph[k][vertex[i]];
            k = vertex[i];
        }
        current_pathweight += graph[k][s];
 
        // update minimum
        min_path = min(min_path, current_pathweight);
 
    } while (
        next_permutation(vertex.begin(), vertex.end()));
 
    return min_path;
}

struct individual {
    string gnome;
    int fitness;
};
  
// Function to return a random number
// from start and end
int rand_num(int start, int end)
{
    int r = end - start;
    int rnum = start + rand() % r;
    return rnum;
}
  
// Function to check if the character
// has already occurred in the string
bool repeat(string s, char ch)
{
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == ch)
            return true;
    }
    return false;
}
  
// Function to return a mutated GNOME
// Mutated GNOME is a string
// with a random interchange
// of two genes to create variation in species
string mutatedGene(string gnome)
{
    while (true) {
        int r = rand_num(1, V);
        int r1 = rand_num(1, V);
        if (r1 != r) {
            char temp = gnome[r];
            gnome[r] = gnome[r1];
            gnome[r1] = temp;
            break;
        }
    }
    return gnome;
}
  
// Function to return a valid GNOME string
// required to create the population
string create_gnome()
{
    string gnome = "0";
    while (true) {
        if (gnome.size() == V) {
            gnome += gnome[0];
            break;
        }
        int temp = rand_num(1, V);
        if (!repeat(gnome, (char)(temp + 48)))
            gnome += (char)(temp + 48);
    }
    return gnome;
}
  
// Function to return the fitness value of a gnome.
// The fitness value is the path length
// of the path represented by the GNOME.
int cal_fitness(string gnome,int map[V][V])
{
    int f = 0;
    for (int i = 0; i < gnome.size() - 1; i++) {
        if (map[gnome[i] - 48][gnome[i + 1] - 48] == INT_MAX)
            return INT_MAX;
        f += map[gnome[i] - 48][gnome[i + 1] - 48];
    }
    return f;
}
  
// Function to return the updated value
// of the cooling element.
int cooldown(int temp)
{
    return (90 * temp) / 100;
}
  
// Comparator for GNOME struct.
bool lessthan(struct individual t1,
              struct individual t2)
{
    return t1.fitness < t2.fitness;
}
  
// Utility function for TSP problem.
int TSPGeneticAlgo(int map[V][V])
{
    // Generation Number
    int gen = 1;
    // Number of Gene Iterations
    int gen_thres = 5;
    int Gmin_path = INT_MAX;
  
    vector<struct individual> population;
    struct individual temp;
  
    // Populating the GNOME pool.
    for (int i = 0; i < POP_SIZE; i++) {
        temp.gnome = create_gnome();
        temp.fitness = cal_fitness(temp.gnome,map);
        population.push_back(temp);
    }
  
    /*cout << "\nInitial population: " << endl
         << "GNOME     FITNESS VALUE\n";
    for (int i = 0; i < POP_SIZE; i++)
        cout << population[i].gnome << "   "
             << population[i].fitness << endl;
    cout << "\n";*/
  
    bool found = false;
    int temperature = 10000;
  
    while (temperature > 1000 && gen <= gen_thres) {
        sort(population.begin(), population.end(), lessthan);
        //cout << "\nCurrent temp: " << temperature << "\n";
        vector<struct individual> new_population;
  
        for (int i = 0; i < POP_SIZE; i++) {
            struct individual p1 = population[i];
  
            while (true) {
                string new_g = mutatedGene(p1.gnome);
                struct individual new_gnome;
                new_gnome.gnome = new_g;
                new_gnome.fitness = cal_fitness(new_gnome.gnome,map);
  
                if (new_gnome.fitness <= population[i].fitness) {
                    new_population.push_back(new_gnome);
                    break;
                }
                else {
  
                    // Accepting the rejected children at
                    // a possible probablity above threshold.
                    float prob = pow(2.7,
                                     -1 * ((float)(new_gnome.fitness
                                                   - population[i].fitness)
                                           / temperature));
                    if (prob > 0.5) {
                        new_population.push_back(new_gnome);
                        break;
                    }
                }
            }
        }
  
        temperature = cooldown(temperature);
        population = new_population;
        //cout << "Generation " << gen << " \n";
        //cout << "GNOME     FITNESS VALUE\n";
  
        for (int i = 0; i < POP_SIZE; i++){
            /*cout << population[i].gnome << "   "
                 << population[i].fitness << endl;*/
            Gmin_path = min(Gmin_path , population[i].fitness);
    	}
        gen++;
    }
    return Gmin_path;
}

void test(){
	clock_t s1,e1,s2,e2;
	int  graph[V][V];
	for (int i=0;i<V;i++){
		for(int j=0;j<V;j++){
			if (i == j)
				graph[i][j] = 0;
			else
				graph[i][j] = graph[j][i] = rand()%30+1;
		}
	}
	s1 = clock();
    int genetic = TSPGeneticAlgo(graph);
    e1 = clock();
    s2 = clock();
    int optimal = TSPDynamicProgramming(graph, START);
    e2 = clock();
    double Gene_used_time = (double)(e1-s1)/CLOCKS_PER_SEC;
    double DP_used_time = (double)(e2-s2)/CLOCKS_PER_SEC;
    cout <<"Genetic best = "<< genetic << endl;
    cout <<"time = "<< Gene_used_time << endl;
    cout <<"DP = "<< optimal << endl;
    cout <<"time = "<< DP_used_time << endl;
    double error = (double)(genetic - optimal)/optimal;
    cout <<"»~®t: "<< error << endl;
}

int main()
{
	for (int i=0;i<5;i++){
		test();
		cout<<endl;
	}
    return 0;
}
