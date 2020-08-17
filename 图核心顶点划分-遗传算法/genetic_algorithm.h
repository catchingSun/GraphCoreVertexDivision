#ifndef __GENETIC_ALGORITHM_

#define WGRAPH_FILE_PATH "1x.txt"
#define CROSSOVER_P 0.7
#define MUTATION_P 0.1
#define SUB_POPULATION_COUNT 20
#define GENETIC_TIMES 30 
#define ERROR_FITNESS 0.000001
#define ALPHA 0.5
#define THETA 0.5
 
typedef struct NextNum{
	int access_node_num;
	int side_weight;
	struct NextNum *next;
}NNode, *NextNodeList;


typedef struct WeightGraph{
	int node_num;
	int node_weight;
	NextNodeList nside_weight;
}*WGraph, RAWGraph;

typedef struct FitnessArray{
	int side;
	double fitness;
	double select_p;
}*FitnessArray;

typedef struct OptimalSequence{
	int *nodes_sequence;
	double fitness;
}OSequence, *OptimalSequence;

int get_node_count();
int init_wgraph_array(WGraph *, FILE *, const int);
int init_random_sequence(int (*)[], const int, const int, const int);
int cross_process(int (*)[], int (*)[], const int, const int, const int, const int);
int variation_process(int (*)[], int (*)[], const int, const int, const int, const int);
int fitness_function(WGraph *, OptimalSequence *, int (*)[], int (*)[], const int, const int, const int, const double, const double);
int record(WGraph *, const OptimalSequence, const int);

double random_p();
int calculation_select_p(FitnessArray *, const int);
int output_division_graph(WGraph *, const OptimalSequence, const int);
int selection_seed_sequence(int (*)[], FitnessArray *, int (*)[], const int, const int);
int calculation_fitness(WGraph *, int (*)[], FitnessArray *,const int, const int, int, const double, const double);
int calculate_variance(int *, const int , double *);
void swap(FitnessArray *, FitnessArray *);
void selectsort(FitnessArray *, const int);
int init_nextnodelist(NextNodeList *);
NextNodeList creat_adjacency_node(NextNodeList, const int, const int);
int random_cross_variation_number(const int);
int random_cross_variation_site(const int);
int random_num(const int);

#endif
