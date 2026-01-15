#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_single_string.h"
#include "file_reader.h"

struct lcs_collections {
    char *main_string;
    char *lcs;
    char *lcs_main_string;
    int main_string_index;
    int lcs_main_string_index;
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

// FIXED: Return the LCS string instead of using output parameter
char* func(char *string1, char *string2) {
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

    // Create a copy to return
    char *result = strdup(string_lcs.string);

    // Cleanup
    for(int i = 0; i <= m; i++) free(dp_matrix[i]);
    free(dp_matrix);

    return result;
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

    // Initialize the collections structure
    struct lcs_collections collections_with_lcs[MAX_STRINGS];

    // Initialize all pointers to NULL first
    for(int i = 0; i < MAX_STRINGS; i++) {
        collections_with_lcs[i].main_string = NULL;
        collections_with_lcs[i].lcs_main_string = NULL;
        collections_with_lcs[i].lcs = NULL;
    }

    // Process all combinations
    char *string1;
    char *string2;

    for(int i = 0; i < count; i++) {
        string1 = collection[i];

        // Allocate and copy main_string
        collections_with_lcs[i].main_string = strdup(string1);
        collections_with_lcs[i].main_string_index=i;

        char *largest_lcs = NULL;
        char *actual_string_of_largest_lcs = NULL;
        int largest_lcs_length = 0;
        int  largest_main_lcs_string_index;
/*        printf("\n=== Processing string %d from file 1 ===\n", i);
        printf("Main string length: %d\n", (int)strlen(string1));*/

        for(int j = 0; j < count_2; j++) {
            string2 = collection_2[j];
//          largest_main_lcs_string_index=j;
            if(strcmp(string1, string2) != 0) {
                // FIXED: Get the returned LCS string
                char *temp_lcs = func(string1, string2);
//                largest_main_lcs_string_index=j;
/*                printf("  Comparing with string %d from file 2, LCS length: %d\n",
                       j, (int)strlen(temp_lcs));
*/
                if(largest_lcs == NULL) {
                    // First comparison
                    largest_lcs = strdup(temp_lcs);
                    actual_string_of_largest_lcs = strdup(string2);
                    largest_lcs_length = strlen(temp_lcs);
                    largest_main_lcs_string_index=j;
                } else {
                    // Check if current LCS is larger
                    if(strlen(temp_lcs) > largest_lcs_length) {
                        free(largest_lcs);
                        free(actual_string_of_largest_lcs);
                        largest_lcs = strdup(temp_lcs);
                        largest_lcs_length = strlen(temp_lcs);
                        largest_main_lcs_string_index=j;
                        actual_string_of_largest_lcs = strdup(string2);

                    }
                }

                // Free temp_lcs after use
                free(temp_lcs);
            }
        }

        // Store the largest LCS found for this main string
        if(largest_lcs != NULL) {
            collections_with_lcs[i].lcs = largest_lcs;  // Transfer ownership
            collections_with_lcs[i].lcs_main_string = actual_string_of_largest_lcs;  // Transfer ownership
collections_with_lcs[i].lcs_main_string_index=largest_main_lcs_string_index;

//            printf("Largest LCS length for string %d: %d\n", i, largest_lcs_length);
        } else {
            // No comparison was made (all strings were identical or no valid comparisons)
            collections_with_lcs[i].lcs = strdup("");
            collections_with_lcs[i].lcs_main_string = strdup("");
        }
    }

    // Display results
/*    printf("\n\n=== FINAL RESULTS ===\n");
    for(int i = 0; i < count; i++) {
        printf("\n[%d] Main String: %.100s...\n", i, collections_with_lcs[i].main_string);
        printf("    LCS Length: %d\n", (int)strlen(collections_with_lcs[i].lcs));
        printf("    Best Match: %.100s...\n", collections_with_lcs[i].lcs_main_string);
        printf("    LCS: %.100s...\n", collections_with_lcs[i].lcs);
    }
*/

    printf("\n==final_Result==\n");
  printf("string no. %d from the thrust_data.txt Main string is =%s\n\n",collections_with_lcs[3].main_string_index,collections_with_lcs[3].main_string);
    printf("The closes string index is =%d from the file thrust_data_2.txt and the string is =%s \n\n",collections_with_lcs[3].lcs_main_string_index,collections_with_lcs[3].lcs_main_string);
    printf("and the LCS among them is =%s\n",collections_with_lcs[3].lcs);

    // Cleanup collection arrays
    for(int i = 0; i < count; i++) free(collection[i]);
    for(int i = 0; i < count_2; i++) free(collection_2[i]);

    // Cleanup collections_with_lcs
    for(int i = 0; i < count; i++) {
        if(collections_with_lcs[i].main_string != NULL)
            free(collections_with_lcs[i].main_string);
        if(collections_with_lcs[i].lcs != NULL)
            free(collections_with_lcs[i].lcs);
        if(collections_with_lcs[i].lcs_main_string != NULL)
            free(collections_with_lcs[i].lcs_main_string);
    }

    return 0;
}
