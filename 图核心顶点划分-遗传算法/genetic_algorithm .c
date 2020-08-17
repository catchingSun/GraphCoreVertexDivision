#include <stdio.h>  
#include <math.h>  
#include <stdlib.h>  
#include <time.h> 
#include <windows.h>
#include <string.h>
#include "genetic_algorithm.h"

int node_count;

int get_node_count(){
	FILE *wgraph_fp;
	wgraph_fp = fopen(WGRAPH_FILE_PATH, "r");
	fscanf(wgraph_fp, "%d", &node_count);
	close(wgraph_fp);
	return node_count;
}

int init_wgraph_array(WGraph *raw_wgraph, FILE *raw_wgraph_fp, const int node_count){
	int side_count;
	int i, j;
	
	fscanf(raw_wgraph_fp, "%d" , &side_count);
	if(raw_wgraph == NULL){
		
		printf("Init raw_wgraph error!\n");
		exit(1);
	}
	
	for(i = 0; i < node_count; i++){
		fscanf(raw_wgraph_fp, "%d %d", &(raw_wgraph[i]->node_num), &(raw_wgraph[i]->node_weight));
		
	}

	for(i = 0; i < side_count; i++){
		int temp1;
		int temp2;
		
		fscanf(raw_wgraph_fp, "%d%d", &temp1, &temp2);
		for(j = 0; j < node_count; j++){
			
			if((temp1 == raw_wgraph[j]->node_num)){
				int side_weight;
				
				fscanf(raw_wgraph_fp, "%d", &side_weight);		
				creat_adjacency_node(raw_wgraph[j]->nside_weight, temp2, side_weight);
							
			}
		}
	}
	
	fclose(raw_wgraph_fp);
	return 0;
}

int scount = -1;

int init_random_sequence(int (*init_random_sequence)[node_count], const int random_sequence_count, const int node_count,const int division_count){
	
	int i, j;
    srand((unsigned)time(NULL));
	printf("\ninit_sequence : \n");
	for(i = 0; i < random_sequence_count; i++){
		for(j = 0; j < node_count; j++){
			int temp = random_num(division_count);
			init_random_sequence[i][j] = temp;
			printf("%d ", init_random_sequence[i][j]);
		}
		printf("\n");
	}
	

	return 0;
}

//cross_count = CROSSOVER_P * RANDOM_SEQUENCE_COUNT 
int cross_process(int (*cross_sequence)[node_count], int (*new_cross_sequence)[node_count], const int cross_len, const int cross_count, const int node_count, const int random_sequence_count){
	
	int i = 0, j = 0, k = 0;
	int cross_array[cross_count];
	int control_isrepeat[random_sequence_count];
	int cross_site = 0;

	memset(cross_array, 0, cross_count * sizeof(int));
	memset(new_cross_sequence, 0, cross_count * node_count * sizeof(int));
	memset(control_isrepeat, 0, random_sequence_count * sizeof(int));

	for(i = 0; i < cross_count; i++){

		int temp = random_cross_variation_number(random_sequence_count);
		
		while(control_isrepeat[temp] == 1){
			
			temp = random_cross_variation_number(random_sequence_count);
		}
		
		cross_array[i] = temp;

		control_isrepeat[temp] = 1;

	}
/*	printf("\ninit_cross_sequence :\n");
	for(i = 0; i < cross_count; i++){
		for(j = 0; j < node_count; j ++){
			printf("%d ", cross_sequence[cross_array[i]][j]);
		}
		printf("\n");
	}

*/
	int cross = 0;
	for(i = 0; i < cross_count - 1; i++){
		for(j = i + 1; j < cross_count; j++){
			cross_site = random_cross_variation_site(node_count);

			for(k = 0; k < node_count; k++){

				if(k >= cross_site){
			
					new_cross_sequence[cross][k] = cross_sequence[cross_array[j]][k];
					new_cross_sequence[cross + 1][k] = cross_sequence[cross_array[i]][k];
				}else{
					new_cross_sequence[cross][k] = cross_sequence[cross_array[i]][k];
					new_cross_sequence[cross + 1][k] = cross_sequence[cross_array[j]][k];
				}
			}
			cross = cross + 2;

		}
		
	}
	 
/*
	printf("\ncross_process\n");
	for(i = 0; i < cross_len; i++){
		for(j = 0; j < node_count; j++){
			printf("%d", new_cross_sequence[i][j]);
		}
		printf("\n");
	}
	printf("\n");
*/
	return ;
}

//variation_count = NODE_COUNT * MUTATION_P
int variation_process(int (*variation_sequence)[node_count], int (*new_variation_sequence)[node_count], const int variation_count, const int node_count, const int division_count, const int random_sequence_count){
	
	int i = 0, j = 0;
	int variation_array[variation_count];
	int control_isrepeat[random_sequence_count];
	int variation_site = 0;
	int variation_value = 0;

	memset(variation_array, 0, variation_count * sizeof(int));
	memset(control_isrepeat, 0, random_sequence_count * sizeof(int));
	memset(new_variation_sequence, 0, variation_count * node_count * sizeof(int));
	for(i = 0; i < variation_count; i++){

		int temp = random_cross_variation_number(random_sequence_count);

		while(control_isrepeat[temp] == 1){
			temp = random_cross_variation_number(random_sequence_count);
		}
		variation_array[i] = temp;
		control_isrepeat[temp] = 1;
	}
	
	for(i = 0; i < variation_count; i++){

		for(j = 0; j < node_count; j++){
			new_variation_sequence[i][j] = variation_sequence[variation_array[i]][j];
		}
		variation_site = random_cross_variation_site(node_count);
		variation_value = random_num(division_count);
		
		while(variation_value == new_variation_sequence[i][variation_site]){
			variation_value = random_num(division_count);
		}
		new_variation_sequence[i][variation_site] = variation_value;
	}
/*	printf("\nnew_variation_sequence : \n");
	for(i = 0; i < variation_count; i++){
		for(j = 0; j < node_count ; j++){
			printf("%d ", new_variation_sequence[i][j]);
		}
		printf("\n");
	}
*/
	return;
}
int  xxx= 1;
int fitness_function(WGraph *wgraph, OptimalSequence * optimal_sequence, int (*genetic_sequence)[node_count], int (*sub_population)[node_count],const int devision_count, const int node_count, const int genetic_len, const double alpha, const double theta){

	FitnessArray *fitness_array;
	int i, j;

	fitness_array = (FitnessArray *)malloc(genetic_len * sizeof(struct FitnessArray));
	
	for(i = 0; i < genetic_len; i++){
		fitness_array[i] = (FitnessArray)malloc(sizeof(struct FitnessArray));
		
	}

	calculation_fitness(wgraph, genetic_sequence, fitness_array, node_count, devision_count, genetic_len, alpha, theta);

	selectsort(fitness_array, genetic_len);
	if(xxx == 1){
		for(i = 0; i < genetic_len; i++){
			printf("%f :", fitness_array[i]->fitness);
			for(j = 0; j < node_count; j++){
			
				printf("%d ", genetic_sequence[fitness_array[i]->side][j]);
			}
			printf("\n");
		}
	}
	xxx = 0;
	
	fflush(stdout);
	
	(*optimal_sequence)->fitness = fitness_array[genetic_len - 1]->fitness;
	for(i = 0; i < node_count; i++){
		(*optimal_sequence)->nodes_sequence[i] = genetic_sequence[fitness_array[genetic_len - 1]->side][i];
		fflush(stdout);
	//	printf(" %d ", genetic_sequence[genetic_len - 1][i]);
	}

	selection_seed_sequence(genetic_sequence, fitness_array, sub_population, node_count, genetic_len);

	return;
}

int calculation_select_p(FitnessArray *fitness_array, const int genetic_len){
	double fitness_sum = 0;
	int i;
	
	for(i = 0; i < genetic_len; i++){
		fitness_sum += fitness_array[i]->fitness;
		
	}
	
	for(i = 0; i < genetic_len; i++){
		fitness_array[i]->select_p = fitness_array[i]->fitness / fitness_sum;
		//printf("\n=======%f======\n", fitness_array[i]->select_p);
	}
}

int selection_seed_sequence(int (*genetic_sequence)[node_count], FitnessArray *fitness_array, int (*sub_population)[node_count], const int node_count, const int genetic_len){
	int i, j, k;
	double random;
	int len = genetic_len + 1;
	double interval[len];
	int control_isrepreat[genetic_len];
	int sub_population_count = SUB_POPULATION_COUNT;
	double sum = 0;
	
	memset(control_isrepreat, 0, genetic_len * sizeof(int));
	memset(interval, 0, len * sizeof(int));
	fflush(stdout);

	if(genetic_len <= sub_population_count){
		memset(sub_population, 0, genetic_len * sizeof(int));
		for(i = 0; i < genetic_len; i++){
		
			for(j = 0; j < node_count; j++){
			

				sub_population[i][j] = genetic_sequence[i][j];

			}

		}
	}else{
		calculation_select_p(fitness_array, genetic_len);
		memset(sub_population, 0, sub_population_count * sizeof(int));
		for(i = 0; i < node_count; i++){
			sub_population[0][i] = genetic_sequence[fitness_array[genetic_len - 1]->side][i];
			control_isrepreat[fitness_array[genetic_len - 1]->side] = 1;
		}
		
		interval[0] = 0;
		for(i = 1; i < len; i++){
			sum += fitness_array[i - 1]->select_p;

			interval[i] = sum;
		}
		
		for(i = 1; i < sub_population_count; i++){
			int flag = 0;
			while(flag == 0){
				random = random_p();
				for(j = 0; j < genetic_len; j ++){
					if((random - interval[j]) >= 0.0 && (interval[j + 1] - random) >= 0){
						if(control_isrepreat[j] == 0){
							control_isrepreat[j] = 1;
							int temp = fitness_array[j]->side;
							for(k = 0; k < node_count; k++){
								sub_population[i][k] = genetic_sequence[temp][k];
							}
							flag = 1;
						}
					}
					if(flag == 1){
						break;
					}

				}
			}
			
		}

/*		printf("\nsub_population\n");
		for(i = 0; i < sub_population_count; i++){
			for(j = 0; j < node_count; j ++){
				printf("%d ", sub_population[i][j]);
			}
			printf("\n");
		}
*/
	}

	return ;
}

int calculation_fitness(WGraph *wgraph, int (*genetic_sequence)[node_count], FitnessArray *fitness_array, const int node_count, const int division_count, int genetic_len, const double alpha, const double theta){
	int nodes_weight[division_count];
	int sides_weight[node_count];
	double sum_nodes_weight;
	double sum_sides_weight;
	int control_isrepeat[node_count];
	int isnext = 0;
	int _isnext = 0;
	int group_count;
	int i, j, k;
	NNode *nnode;
		
	for(i = 0; i < genetic_len; i++){
		
		memset(nodes_weight, 0, sizeof(nodes_weight));
		memset(sides_weight, 0, sizeof(sides_weight));
		memset(control_isrepeat, 0, sizeof(control_isrepeat));
		
		group_count = -1;
		for(j = 0; j < node_count; j++){
			int count = 1;
			int temp = -1;
			if(control_isrepeat[j] == 0){
				temp = genetic_sequence[i][j];
				group_count++;
				nodes_weight[group_count] = wgraph[j]->node_weight;
			}
	
			for(k = j + 1; k < node_count; k++){
				if(control_isrepeat[k] == 0){
					if(genetic_sequence[i][k] == temp){				

						nodes_weight[group_count] +=  wgraph[k]->node_weight;
					
						control_isrepeat[k] = 1;
						count++;
					}
					
					if(count == node_count){
						break;
					}
					
				}	
			}
		}
		for(j = group_count + 1; j < division_count; j++){
			nodes_weight[j] = 0;
		}
		
		calculate_variance(nodes_weight, division_count, &sum_nodes_weight);
		
		sum_sides_weight = 0;
		for(j = 0; j < node_count; j++){
			nnode = NULL;
			nnode = wgraph[j]->nside_weight;
			nnode = nnode->next;
			while(nnode != NULL){
				for(k = j; k < node_count; k++){
					if(nnode->access_node_num  == wgraph[k]->node_num){
						if(genetic_sequence[i][j] != genetic_sequence[i][k]){
							
							sum_sides_weight += nnode->side_weight;
						}
					}
				}
				nnode = nnode->next;
			}
			
		}
		
		fflush(stdout);
		
		double fitness = alpha * sum_nodes_weight + theta * sum_sides_weight;
		if(fitness != 0){
			fitness = 1 / fitness;
		}
		fitness_array[i]->side = i;
		fitness_array[i]->fitness = fitness;

	}
	
	return 0;
}

int record(WGraph *wgraph, const OptimalSequence optimal_sequence, const int node_count){
	
	double fitness_error;

	output_division_graph(wgraph, optimal_sequence, node_count);

	return 0;
}

int output_division_graph(WGraph *raw_wgraph, const OptimalSequence optimal_sequence, const int node_count){
	FILE *output_fp;
	int i = 0, j = 0, k = 0;
	char output_file_path[30];
	char *file_name;
	char *file_format = ".txt";
	int group_count = 0;
	int control_isrepeat[node_count];
	
	memset(control_isrepeat, 0, sizeof(control_isrepeat));
	fflush(stdout);

	for(i = 0; i < node_count; i++){
	
		if(control_isrepeat[i] == 0){
			output_fp = NULL;
			itoa(group_count, output_file_path, 10);
		
			strcat(output_file_path, file_format);

			output_fp = fopen(output_file_path, "w");

			fprintf(output_fp, "%d\n", raw_wgraph[i]->node_num);
			control_isrepeat[i] = 1;

			group_count++;
		}
		
		for(j = i + 1; j < node_count; j++){
			if(optimal_sequence->nodes_sequence[i] == optimal_sequence->nodes_sequence[j] && control_isrepeat[j] == 0){
				control_isrepeat[j] = 1;
				fprintf(output_fp, "%d\n", raw_wgraph[j]->node_num);

			}
		}
	}
	
}

void selectsort(FitnessArray *fitness_array, const int genetic_len){
    int i, j, Maxdex;
        
    for (i = 0; i < genetic_len - 1; i++)
    {
        Maxdex = i;
        for (j = i + 1; j < genetic_len; j++)
        {	
            if ((fitness_array[Maxdex]->fitness - fitness_array[j]->fitness) >= 0.000001){
            	
            	Maxdex = j;
            }
                
        }
        swap(&fitness_array[i], &fitness_array[Maxdex]);
    }
}

void swap(FitnessArray *fitness_array1, FitnessArray *fitness_array2){

	FitnessArray temp;
	temp = (FitnessArray)malloc(sizeof(struct FitnessArray));
	
	temp->fitness = (*fitness_array1)->fitness;
	temp->side = (*fitness_array1)->side;
	
	(*fitness_array1)->fitness = (*fitness_array2)->fitness;
	(*fitness_array1)->side = (*fitness_array2)->side;
	
	(*fitness_array2)->fitness = temp->fitness;
	(*fitness_array2)->side = temp->side;

}

int calculate_variance(int *weight, const int group_count, double *sum_side_weight){
	int i = 0;
	double average = 0;
	double variance = 0;
	for(i = 0; i < group_count; i++){
		average += weight[i];
		
	}
	average = average / group_count;
	for(i = 0; i < group_count; i++){

		variance += (pow((weight[i] - average), 2) / group_count);
	}
	*sum_side_weight = variance;

	return ;
}

int init_nextnodelist(NextNodeList *NCL){
	*NCL = (NextNodeList)malloc(sizeof(struct WeightGraph));
	(*NCL)->next = NULL;
	return 0;
}

NextNodeList creat_adjacency_node(NextNodeList NCL, const int node_num, const int node_weight){
	NNode *n, *nn;
	nn = NCL;
	n = (NNode *)malloc(sizeof(struct NextNum));
	n->access_node_num = node_num;

	n->side_weight = node_weight;
	if(nn->next == NULL){
		
		nn->next = n;
		nn = n;
		nn->next = NULL;
	}else{
		while(nn->next != NULL){
			
			nn = nn->next;
		}
		nn->next = n;
		nn = n;
		nn->next = NULL;
	}
	
	return NCL;
	
}

double random_p(){
	double random;
	random = rand() / (double)(RAND_MAX);
	return random;
}

int random_cross_variation_number(const int random_sequence_count){
	int cv_num;

	cv_num = rand() % random_sequence_count;
	return cv_num;
}

int random_cross_variation_site(const int node_count){
	int cv_site;

	cv_site = rand() % node_count;
	return cv_site;
}

int random_num(const int division_count){
	int node_num;

	node_num = rand() % division_count;
	return node_num;
}
