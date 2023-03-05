#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t
{
    int value;
    struct node* next;
} node_t;

typedef struct graph_t{

    int number_of_nodes;
    float **adjacency_matrix;

} graph_t;

void init_graph (graph_t **graph, int nodes_number){

    *graph = (graph_t *) malloc(sizeof(graph_t));
    
    (*graph)->number_of_nodes = nodes_number;

    (*graph)->adjacency_matrix = (float **) malloc(sizeof(float *) *
    nodes_number);
    
    int index;
    for (index = 0; index < nodes_number; index++){
        (*graph)->adjacency_matrix[index] = (float *) calloc(nodes_number,
        sizeof(float));
    }
}

void insert_edge(graph_t *graph, int start_node, int arrival_node, float cost){
     
    graph->adjacency_matrix[start_node][arrival_node] = cost;
}

void free_graph(graph_t *graph){

    int i;
    for (i = 0; i < graph->number_of_nodes; i++){
        free(graph->adjacency_matrix[i]);
    }

    free(graph->adjacency_matrix);
    free(graph);
}

void dijkstra(graph_t *graph, int depo_ID, int market_ID, float *aux,
                        int *path_to_print, int *nr_nodes_to_print){

    int i, j;
    int *visited_IDs = (int *) calloc(graph->number_of_nodes, sizeof(int));
    float *dist_array = (float *) malloc(sizeof(float) *
        graph->number_of_nodes);

    int parent[graph->number_of_nodes];

    for (i = 0; i < graph->number_of_nodes; i++){
        dist_array[i] = 1000;
        visited_IDs[i] = 0;
    }

    dist_array[depo_ID] = 0;
    parent[depo_ID] = -1;

    for (i = 0; i < graph->number_of_nodes - 1; i++){
        int node;
        int min = 1000;
        for (j = 0; j < graph->number_of_nodes; j++){
            if (visited_IDs[j] == 0 && (min > dist_array[j])){
                min = dist_array[j];
                node = j;
            }
        }
        visited_IDs[node] = 1;

        if (dist_array[node] != 1000){
            for (j = 0; j < graph->number_of_nodes; j++){
                if (visited_IDs[j] == 0 && graph->adjacency_matrix[node][j] &&
                dist_array[node] + graph->adjacency_matrix[node][j] <
                dist_array[j]) {
                    dist_array[j] = dist_array[node] +
                    graph->adjacency_matrix[node][j];
                    parent[j]  = node;
                }
            }
        }
    }
    
    int parent_node = market_ID;
    *nr_nodes_to_print = 0;
    for (i = 0; i < graph->number_of_nodes; i++) {
        path_to_print[i] = parent_node;
        parent_node = parent[parent_node];
        (*nr_nodes_to_print)++;
        if (parent_node == depo_ID) {
            break;
        }
    }
    
    path_to_print[*nr_nodes_to_print] = depo_ID;
    (*nr_nodes_to_print)++;
   
    for (i = 0; i < (*nr_nodes_to_print) / 2; i++) {
        int aux = path_to_print[i];
        
        path_to_print[i] = path_to_print[(*nr_nodes_to_print) - i - 1];
        path_to_print[(*nr_nodes_to_print) - i - 1] = aux;
    }
    *aux = dist_array[market_ID];

    free(visited_IDs);
    free(dist_array);
}

void function_for_task1(graph_t *graph){
                     
    int supply_deposit;
    scanf("%d", &supply_deposit);
            
    int nr_supplied_markets;
    scanf("%d", &nr_supplied_markets);
            
    int *supplied_markets_IDs;
    supplied_markets_IDs = malloc(sizeof(int) * nr_supplied_markets);

    int j;
    int supplied_market;
    for (j = 0; j < nr_supplied_markets; j++){
        scanf("%d", &supplied_market);
        supplied_markets_IDs[j] = supplied_market;
    }

    float *going_distances = malloc (sizeof(float) * nr_supplied_markets);
    float *return_distances = malloc (sizeof(float) * nr_supplied_markets);
    float distances_aux;

    for (j = 0; j < nr_supplied_markets; j++){
        int nr_to_print1, nr_to_print2;
        
        printf("%d\n", supplied_markets_IDs[j]);
        int *path1 = calloc (sizeof(int), graph->number_of_nodes);
        dijkstra(graph, supply_deposit, supplied_markets_IDs[j],
            &distances_aux, path1, &nr_to_print1);
        going_distances[j] = distances_aux;

        int *path2 = calloc (sizeof(int), graph->number_of_nodes);
        dijkstra(graph, supplied_markets_IDs[j], supply_deposit,
            &distances_aux, path2, &nr_to_print2);
        return_distances[j] = distances_aux;
    
        printf("%0.1f %0.1f\n", going_distances[j], return_distances[j]);
        
        int i;
        for (i = 0; i < nr_to_print2 - 1; i++) {
            path1[nr_to_print1 + i] = path2[i + 1];
        }
        
        for (i = 0; i < nr_to_print1 + nr_to_print2 - 1; i++) {
            printf("%d", path1[i]);
            if (i < nr_to_print1 + nr_to_print2 - 2){
                printf(" ");
            }
            else{
                printf("\n");
            }
        }

        free(path1);
        free(path2);
    }

    float sum = 0;
    int i;

    for (i = 0; i < nr_supplied_markets; i++){
        sum += going_distances[i] + return_distances[i];
    }

    printf("%0.1f", sum);

    free(supplied_markets_IDs);
    free(going_distances);
    free(return_distances);
}

void remove_road(graph_t *graph, int *deposints_id, int nr_deposits){

    int i, j, k;
    for (k = 0; k < nr_deposits; k++){
        for (i = 0; i < graph->number_of_nodes; i++){
            for (j = 0; j < graph->number_of_nodes; j++){
                if (graph->adjacency_matrix[deposints_id[k]][j] != 0){
                    graph->adjacency_matrix[deposints_id[k]][j] = 0;
                }
                if (graph->adjacency_matrix[j][deposints_id[k]] != 0){
                    graph->adjacency_matrix[j][deposints_id[k]] = 0;
                }
            }
        }
    }
}

int is_depo(int *depo_IDs, int depo_nr, int nr){

    int i;
    for (i = 0; i < depo_nr; i++){
        if (nr == depo_IDs[i]){
            nr = -1;
        }
    }

    return nr;
}

void find_isolate(graph_t *graph, int *depo_IDs, int depo_nr,
        int *isolated_points, int *isolated_points_nr){

    int i;
    int k,l;
    
    float sum = 0;
    float line_sum = 0;
    float column_sum = 0;
    int flag;
    *isolated_points_nr = 0;
    
    for (i = 0; i < graph->number_of_nodes; i++){
        sum = 0;
        line_sum = 0;
        column_sum = 0;
        flag = is_depo(depo_IDs, depo_nr, i);
        if (graph->adjacency_matrix[i][i] == 0.0 && flag != -1){
            for (k = 0; k < graph->number_of_nodes; k++){
                for (l = 0; l < graph->number_of_nodes; l++){
                    /* Initializari sume pentru linii si coloane */
                    sum += graph->adjacency_matrix[k][i] +
                    graph->adjacency_matrix[i][l];
                    line_sum = line_sum + graph->adjacency_matrix[k][i];
                    column_sum = column_sum + graph->adjacency_matrix[i][l];
                }
            }
      
            if (sum == 0){
                *(isolated_points_nr) += 1;
                isolated_points[i] = i;
            }
            if (line_sum == 0 && column_sum != 0){
                *(isolated_points_nr) += 1;
                isolated_points[i] = i;
            }
            if (column_sum == 0 && line_sum != 0){
                *(isolated_points_nr) += 1;
                isolated_points[i] = i;
            }
        }
    }
}

void dfs(graph_t *graph, int source, int *visited, int *dfs_output,
        int *index){
    
    visited[source] = 1;
    dfs_output[*index] = source;
    (*index) += 1;

    int i;
    for (i = 0; i < graph->number_of_nodes; i++){
        if ((graph->adjacency_matrix[i][source] != 0) && (visited[i] == 0)){
            dfs(graph, i, visited, dfs_output, index);
        }
    }
}

void sort_array(int *v, int nr_elem){
    
    int i, j;
    int aux_swap;

    for (i = 0; i < nr_elem - 1; i++){
        for (j = 1; j < nr_elem - i - 1; j++){
            if (v[j] > v[j + 1]){
                aux_swap = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux_swap;
            }
        }
    }
}

void strongly_connected_components(graph_t *graph, int *number_of_components,
                                  int *isolated_points, int nr_isolated_points,
                                  int *deposits_IDs, int deposits_number){
    
    int *visited = calloc(sizeof(int), graph->number_of_nodes);
    int *dfs_nodes = malloc(graph->number_of_nodes * sizeof(int));
    int nr_dfs_nodes = 0;

    int i, j;
    for (i = 0; i < graph->number_of_nodes; i++){
        if (visited[i] == 0 && is_depo(deposits_IDs, deposits_number, i) != -1
            && isolated_points[i] == 0){
            
            *(number_of_components) += 1;
            dfs(graph, i, visited, dfs_nodes, &nr_dfs_nodes);

        }
    }

    int sum = *number_of_components + nr_isolated_points;
    printf("%d", sum);

    memset(visited, 0, sizeof(int) * graph->number_of_nodes);
    memset(dfs_nodes, 0, graph->number_of_nodes * sizeof(int));
    nr_dfs_nodes = 0;

    int conex_elem = -1;
    for (i = 0; i < graph->number_of_nodes; i++){
        if (isolated_points[i] != 0){
            conex_elem = isolated_points[i];
        }
    }

    for (i = 0; i < graph->number_of_nodes; i++){
        if (visited[i] == 0 && is_depo(deposits_IDs, deposits_number, i) != -1
                && isolated_points[i] == 0){
            
            dfs(graph, i, visited, dfs_nodes, &nr_dfs_nodes);
        
            printf("\n");

            sort_array(dfs_nodes, nr_dfs_nodes);
            for (j = 0; j < nr_dfs_nodes; j++) {
                if ((dfs_nodes[j] > conex_elem) && (conex_elem != -1)){
                    printf("%d\n", conex_elem);
                    conex_elem = -1;
                    printf("%d ", dfs_nodes[j]);
                }
                else{
                    if (j == nr_dfs_nodes - 1) {
                        printf("%d", dfs_nodes[j]);
                    } else {
                        printf("%d ", dfs_nodes[j]);
                    }
                }
            }
            memset(dfs_nodes, 0, graph->number_of_nodes * sizeof(int));
            nr_dfs_nodes = 0;
            }
    }
    
    if (nr_isolated_points != 0){
        if (conex_elem != -1){
            printf("\n%d", conex_elem);
        }
    }
    
    free(visited);
    free(dfs_nodes);
}

void function_for_task2(graph_t *graph, int *deposits_IDs,
        int deposits_number){
      
    remove_road(graph, deposits_IDs, deposits_number);
    
    int *isolated_points = calloc(sizeof(int), graph->number_of_nodes);
    int nr_of_isolated_points;

    find_isolate(graph, deposits_IDs, deposits_number, isolated_points,
    &nr_of_isolated_points);
    
    int nr_of_scc = 0;
    strongly_connected_components(graph, &nr_of_scc, isolated_points,
    nr_of_isolated_points, deposits_IDs, deposits_number);

    free(isolated_points);
}

int main(){

    int nodes_number, edges_number, deposits_number;
    scanf("%d %d %d", &nodes_number, &edges_number, &deposits_number);

    int start_node, arrival_node;
    float cost_value;
    
    graph_t *graph;
    init_graph(&graph, nodes_number);

    int i;
    for (i = 0; i < edges_number; i++){
        scanf("%d %d %f", &start_node, &arrival_node, &cost_value);
        insert_edge(graph, start_node, arrival_node, cost_value);
    }

    int *deposits_IDs = malloc (sizeof(int) * deposits_number);
    int deposit;

    for (i = 0; i < deposits_number; i++){
        scanf("%d", &deposit);
        deposits_IDs[i] = deposit;
    }

    int nr_tasks;
    scanf("%d", &nr_tasks);
    
    char task1[3] = "e1";
    char task2[3] = "e2";
    char task3[3] = "e3";
    
    char task[3];

    for (i = 0; i < nr_tasks; i++){
        scanf("%s", task);
        if (!strcmp(task, task1)){
            function_for_task1(graph);
        }
        if (!strcmp(task, task2)){
            function_for_task2(graph, deposits_IDs, deposits_number);
        }
        if (!strcmp(task, task3)){
            // function_for_task3();
        }
        if (i < nr_tasks - 1){
            printf("\n");
        }
    }

    free_graph(graph);
    free(deposits_IDs);

    return 0;
}
