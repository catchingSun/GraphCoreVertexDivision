#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "genetic_algorithm.h"

extern int node_count;
int genetic_len;
int cross_count; 
int variation_count;
WGraph *wgraph;
int cross_len;
int init_group_count; 


int splice_array(int (*init_sequence)[node_count], int (*new_cross_sequence)[node_count], int (*new_variation_sequence)[node_count], int (*genetic_sequence)[node_count]){
	int i = 0, j;
	int temp;

	for(i = 0; i < init_group_count; i++){
		for(j = 0; j < node_count; j++){
			genetic_sequence[i][j] =  init_sequence[i][j];

		}
	}

	temp = init_group_count;
	for(i = temp; i < cross_len + temp; i++){
		for(j = 0; j < node_count; j++){
			genetic_sequence[i][j] =  new_cross_sequence[i - temp][j];
		}
	}
	temp = init_group_count + cross_len;

	for(i = temp; i < variation_count + temp; i++){
		for(j = 0; j < node_count; j++){
			genetic_sequence[i][j] =  new_variation_sequence[i - temp][j];

		}
	}
}

int init_wgraph(){
	FILE *wgraph_fp;
	wgraph_fp = fopen(WGRAPH_FILE_PATH, "r");
	fscanf(wgraph_fp, "%d", &node_count);
	
	
	wgraph = (WGraph *)malloc(node_count * sizeof(WGraph));
	int i;
	for(i = 0; i < node_count; i++){
		wgraph[i] = (WGraph)malloc(sizeof(struct WeightGraph));
		init_nextnodelist(&(wgraph[i]->nside_weight));
	}
	init_wgraph_array(wgraph, wgraph_fp, node_count);
	return 0;
}

int main()
{
	char *wgraph_path = WGRAPH_FILE_PATH;
	int division_count;
	double alpha = ALPHA;
	double theta = THETA;
	int record_value;
	int genetic_count = 0;
	OptimalSequence optimal_sequence_old, optimal_sequence_new;
	optimal_sequence_old = (OptimalSequence)malloc(sizeof(OSequence));
	optimal_sequence_new = (OptimalSequence)malloc(sizeof(OSequence));
	get_node_count();

	optimal_sequence_old->nodes_sequence = malloc( node_count * sizeof(int));
	memset(optimal_sequence_old->nodes_sequence, -1, node_count * sizeof(int));
	optimal_sequence_new->nodes_sequence = malloc( node_count * sizeof(int));
	memset(optimal_sequence_new->nodes_sequence, -1, node_count * sizeof(int));
	optimal_sequence_old->fitness = -1;

	printf("请问需要将图划分成多少部分？："); 
	scanf("%d", &division_count);
	printf("请输入初始随机序列组数 ：");
	scanf("%d", &init_group_count);
	
	init_wgraph();

	int i;
	
	int sub_population[SUB_POPULATION_COUNT][node_count];
	int init_sequence[init_group_count][node_count];

	init_random_sequence(init_sequence, init_group_count, node_count, division_count);
	while(genetic_count < GENETIC_TIMES){
		if(genetic_count == 0){

			cross_count = CROSSOVER_P * init_group_count;
			cross_len = cross_count * (cross_count - 1);
			int new_cross_sequence[cross_len][node_count];
			memset(new_cross_sequence, -1, cross_count * node_count * sizeof(int));
			cross_process(init_sequence, new_cross_sequence, cross_len, cross_count, node_count, init_group_count);
			
			variation_count = init_group_count * MUTATION_P;
			int new_variation_sequence[variation_count][node_count];

			variation_process(init_sequence, new_variation_sequence, variation_count, node_count, division_count, init_group_count);
			
			genetic_len = init_group_count + cross_len + variation_count;
			int genetic_sequence[genetic_len][node_count];
			memset(genetic_sequence, -1, genetic_len * node_count * sizeof(int));
			splice_array(init_sequence, new_cross_sequence, new_variation_sequence, genetic_sequence);
			
			fitness_function(wgraph, &optimal_sequence_new, genetic_sequence, sub_population, division_count, node_count, genetic_len, alpha, theta);
			printf("\noptimal_sequence_new\n");
			for(i = 0; i < node_count; i++){
				printf("%d ", optimal_sequence_new->nodes_sequence[i]);
			}
		}else{

			if(genetic_len <= SUB_POPULATION_COUNT){
				init_group_count = genetic_len;
			}else{
				init_group_count = SUB_POPULATION_COUNT;
			}
			cross_count = CROSSOVER_P * init_group_count;
			cross_len = cross_count * (cross_count - 1);
			int new_cross_sequence[cross_len][node_count];
			cross_process(sub_population, new_cross_sequence, cross_len, cross_count, node_count, init_group_count);

			variation_count = init_group_count * MUTATION_P;
			
			int new_variation_sequence[variation_count][node_count];
			variation_process(sub_population, new_variation_sequence, variation_count, node_count, division_count, init_group_count);
	
			genetic_len = init_group_count + cross_len + variation_count;
			int genetic_sequence[genetic_len][node_count];
			memset(genetic_sequence, -1, genetic_len * node_count * sizeof(int));
			splice_array(sub_population, new_cross_sequence, new_variation_sequence, genetic_sequence);
			
			if(optimal_sequence_new->fitness > optimal_sequence_old->fitness ){
				optimal_sequence_old->fitness = optimal_sequence_new->fitness;
				for(i = 0; i < node_count; i++){
					optimal_sequence_old->nodes_sequence[i] = optimal_sequence_new->nodes_sequence[i];
				}
			}
			printf("optimal_sequence_old\n");
			for(i = 0; i < node_count; i++){
				printf("%d ", optimal_sequence_old->nodes_sequence[i]);
			}
			printf("\n");
			fitness_function(wgraph, &optimal_sequence_new, genetic_sequence, sub_population, division_count, node_count, genetic_len, alpha, theta);
			printf("\nold_fitness: %f new_fitness: %f\n", optimal_sequence_old->fitness, optimal_sequence_new->fitness);
		}

		genetic_count ++;
	}
	
	if(optimal_sequence_new->fitness > optimal_sequence_old->fitness ){
		optimal_sequence_old->fitness = optimal_sequence_new->fitness;
		for(i = 0; i < node_count; i++){
			optimal_sequence_old->nodes_sequence[i] = optimal_sequence_new->nodes_sequence[i];
		}
	}
	printf("optimal_sequence_old\n");
	for(i = 0; i < node_count; i++){
		printf("%d ", optimal_sequence_old->nodes_sequence[i]);
	}
	printf("\n");
	record(wgraph, optimal_sequence_old, node_count);

	return(0);
}
