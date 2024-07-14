#include <math.h>  
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

double euclidian_distance(const char *point1[], const char *point2[], int dim) {
    double sum = 0.0;
    
    for (int i = 0; i < dim; ++i) {
        // Convert the strings to floats
        float p1 = atof(point1[i]);
        float p2 = atof(point2[i]);
        
        // Calculate the difference, square it, and add to the sum
        double diff = p1 - p2;
        sum += diff * diff;
    }
    
    // Return the square root of the sum
    return sqrt(sum);
}

double *update(double *center_sum, int center_size, int dim){
    for (int i=0; i < dim; i++){
        center_sum[i] = center_sum[i] / center_size;
    }
    return center_sum;
}

void add_vec(double *vec1, double *vec2, int dim){
    for (int i = 0; i < dim; i++){
        vec1[i] += vec2[i];
    }
}

void cast_to_float(char *point[], int dim, double *temp_vec) {
    for (int i = 0; i < dim; i++) {
        char *endptr;
        temp_vec[i] = strtod(point[i], &endptr);
        if (*endptr != '\0') { // Check if conversion failed
            fprintf(stderr, "An error has occurred\n");
            exit(1);
        }
    }
}

double **round_vec(double *vecs[], int dim, int k){

    for (int i=0; i < k; i++){
        for (int j=0; j < dim; j++){
            vecs[i][j] = round(vecs[i][j] * 10000.0) / 10000.0; // increasing by 10^4 and than rounding to int and dividing 
        }
    }
    return vecs;
}

int find_dim(FILE *input_data){
    int num_points = 0;   // Number of points (elements) in the line
    int comma_count = 0;  // Count of commas in the line
    int ch;               // Character read from the file
    long file_pos;        // File position indicator
    file_pos = ftell(input_data);

    // Read one line from the file and count commas
    while ((ch = fgetc(input_data)) != '\n' && ch != EOF) {
        if (ch == ',') {
            comma_count++;
        }
    }

    // Number of points is one more than the number of commas
    num_points = comma_count + 1;
    fseek(input_data, 0, SEEK_SET);
    return num_points;
}

void split_line(char *line, char *point_str[]){
    char *token;
    token = strtok(line, ",");
    int count = 0;
        // Dynamic allocation based on previously found dimension
    while (token != NULL) {
        point_str[count++] = token;
        token = strtok(NULL, ",");
    }

}



void k_means(int k, int iter, FILE *file){
    int dim = find_dim(file);
    //initialising memmory for reading each line
    double *temp_vec = malloc(dim * sizeof(double));
    if (temp_vec == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    //initialising memmory for centeroids and temps
    double **centeroids = (double **)malloc(k * sizeof(double *));
    double **centeroids_sums = (double **)malloc(k * sizeof(double *));
    double *centeroids_size = (double *)malloc(k * sizeof(double *));
    if (centeroids == NULL || centeroids_sums == NULL || centeroids_size == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    for (int i = 0; i < k; i++) {
        centeroids[i] = (double *)malloc(dim * sizeof(double));
        centeroids_sums[i] = (double *)malloc(dim * sizeof(double));
        if (centeroids[i] == NULL || centeroids_sums[i] == NULL) {
            perror("Memory allocation failed");
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(centeroids[j]);
            }
            free(centeroids);
            return NULL;
        }
    }
    // temp for reading from the file
    char *point_str[dim];

    //initializing centroids
    for (int i = 0; i < k; i++){
        char *line = NULL;
        size_t len = 0;
        if (getline(line, &len, file) != -1) {
           split_line(line, point_str);
        }
         /*if (count != dim) {
                fprintf(stderr, "Error: Dimension mismatch in input file.\n");
                // Free memory
                free(line);
                for (int j = 0; j < k; j++) {
                    free(centroids[j]);
                }
                free(centroids);
                free(temp_vec);
                exit(1);
            }*/
        cast_to_float(point_str, dim, temp_vec);
        centeroids[i] = temp_vec;
        /*} else {
            fprintf(stderr, "Error reading file.\n");
            // Free memory
            for (int j = 0; j < k; j++) {
                free(centroids[j]);
            }
            
    }*/
    
    }
    
    for (int t = 0; t < iter; t++){
        fseek(file, 0, SEEK_SET); //goes to the beginning of the file
        char *line = NULL;
        size_t len = 0;
        while (getline(line, &len, file) != -1) {
           split_line(line, point_str);
           cast_to_float(point_str, dim, temp_vec);
           double min_dis = -1;
           int min_index = -1;
           for (int j = 0; j < k; j++){
                double dis = euclidian_distance(centeroids[j], temp_vec, dim);
                if ( min_dis== -1){
                    min_dis = dis;
                    min_index = j;
                }
                else if (dis <min_dis)
                {
                    min_dis = dis;
                    min_index = j;
                }
            add_vec(centeroids_sums[min_index],temp_vec, dim);
            centeroids_size[min_index]+= 1;
           }
        }
        int true = 1;
        for (int i = 0; i<k ; i++){
            centeroids_sums[i] = update(centeroids_sums[i], centeroids_size[i], dim);
            if (euclidian_distance(centeroids_sums[i], centeroids[i], dim) > 0.001){
                true = 0;
            }
            memcpy(centeroids[i], centeroids_sums[i], dim);
        }
        if (true == 1){
            printf(round_vec(centeroids, dim, k));
            break;
        }

    }
    for (int i = 0; i < k; i++) {
        free(centeroids[i]);
        free(centeroids_sums[i]);
    }
    free(centeroids);
    free(temp_vec);
    free(point_str);
    free(centeroids_size);
}





int main(){
    FILE *file = fopen("input_1.txt","r");
    k_means(3,100, file);
    fclose(file);

}

