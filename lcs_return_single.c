#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_single_string.h"
#include "file_reader.h"

struct lcs_collections {
    char *main_string;
    char *lcs;
    char *lcs_main_string;
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

void build_lcs(char *string1, char *string2, int m, int n,
               int **dp_matrix, struct dynamic_single_string *string_lcs) {
    if(m == 0 || n == 0) return;
    if(string1[m-1] == string2[n-1]) {
        build_lcs(string1, string2, m-1, n-1, dp_matrix, string_lcs);
        add_char(string_lcs, string1[m-1]);
    } else {
        if(dp_matrix[m-1][n] > dp_matrix[m][n-1])
            build_lcs(string1, string2, m-1, n, dp_matrix, string_lcs);
        else
            build_lcs(string1, string2, m, n-1, dp_matrix, string_lcs);
    }
}

int compute_lcs_length(char *string1, char *string2, int m, int n, int **dp_matrix) {
    for(int i = 0; i <= m; i++) {
        for(int j = 0; j <= n; j++) {
            if(i == 0 || j == 0) dp_matrix[i][j] = 0;
            else if(string1[i-1] == string2[j-1])
                dp_matrix[i][j] = dp_matrix[i-1][j-1] + 1;
            else
                dp_matrix[i][j] = max(dp_matrix[i-1][j], dp_matrix[i][j-1]);
        }
    }
    return dp_matrix[m][n];
}

void func(char *string1, char *string2, struct lcs_collections collections_with_lcs[],
          int i,char *largest_lcs) {
    int m = strlen(string1);
    int n = strlen(string2);
    
    struct dynamic_single_string string_lcs;
    initialization_dynamic_string(&string_lcs);
    
    // Allocate Matrix
    int **dp_matrix = (int**)malloc(sizeof(int*) * (m + 1));
    for(int i = 0; i <= m; i++)
        dp_matrix[i] = (int*)malloc(sizeof(int) * (n + 1));
    
    // Run LCS logic
    int lcs_length = compute_lcs_length(string1, string2, m, n, dp_matrix);
    build_lcs(string1, string2, m, n, dp_matrix, &string_lcs);
    // FIXED: Allocate memory before copying
//    collections_with_lcs[i].lcs =strdup(string_lcs.string);
	largest_lcs=strdup(string_lcs.string);
    
    // Cleanup
    for(int i = 0; i <= m; i++) free(dp_matrix[i]);
    free(dp_matrix);
}

int main() {
    // 1. Get the strings from first file
    char *collection[MAX_STRINGS];
    int count = read_thrust_strings("thrust_data.txt", collection);
    
    // 2. Get the strings from second file
    char *collection_2[MAX_STRINGS];
    int count_2 = read_thrust_strings("thrust_data_2.txt", collection_2);
    
    if (count < 1 || count_2 < 1) {
        printf("Need at least 1 string from each file.\n");
        printf("Found in file 1: %d, Found in file 2: %d\n", count, count_2);
        for(int i = 0; i < count; i++) free(collection[i]);
        for(int i = 0; i < count_2; i++) free(collection_2[i]);
        return 1;
    }
    
    // FIXED: Initialize the collections structure
    struct lcs_collections collections_with_lcs[MAX_STRINGS];
    
    // Initialize all pointers to NULL first
    for(int i = 0; i < MAX_STRINGS; i++) {
        collections_with_lcs[i].main_string = NULL;
	collections_with_lcs[i].lcs_main_string= NULL;
	collections_with_lcs[i].lcs=NULL;
        }
   
    
    // Process all combinations
    char *string1;
    char *string2;
    char *largest_lcs=NULL;
    char *actual_string_of_largest_lcs=NULL;
    char *temp_lcs=NULL;
    int largest_lcs_length=0;

      for(int i = 0; i < count; i++) {
          string1 = collection[i];

          // FIXED: Allocate and copy main_string
          collections_with_lcs[i].main_string = strdup(string1);

          for(int j = 0; j < count_2; j++) {
              string2 = collection_2[j];
		if(strcmp(string1,string2)!=0)
		{
	//	collections_with_lcs[i].main_string=strdup(string1);
		func(string1, string2, collections_with_lcs,i,temp_lcs);
		if(largest_lcs==NULL && largest_lcs_length==0)
		{
			largest_lcs=strdup(temp_lcs);
			actual_string_of_largest_lcs=strdup(string2);
			largest_lcs_length=strlen(temp_lcs);
			free(temp_lcs);
		}
		else
		{
			if(strlen(largest_lcs)<strlen(temp_lcs))
			{
				free(largest_lcs);
				free(actual_string_of_largest_lcs);
				largest_lcs=strdup(temp_lcs);
				largest_lcs_length=strlen(temp_lcs);
				actual_string_of_largest_lcs=strdup(string2);

			}
		
				}
          		}
	
	  	}
		collections_with_lcs[i].lcs=strdup(largest_lcs);
collections_with_lcs[i].lcs_main_string=strdup(actual_string_of_largest_lcs);	
      }
printf("collections_with_lcs[0].main_string=%s\n",collections_with_lcs[0].main_string);

    // Cleanup collection arrays
    for(int i = 0; i < count; i++) free(collection[i]);
    for(int i = 0; i < count_2; i++) free(collection_2[i]);
    
    // Cleanup collections_with_lcs
    for(int i = 0; i < count; i++) {
            free(collections_with_lcs[i].main_string);
            free(collections_with_lcs[i].lcs);
	    free(collections_with_lcs[i].lcs_main_string);
    }
    
    return 0;
}
