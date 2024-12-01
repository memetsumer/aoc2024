#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_LINES 1000
#define DELIMITER "   "

int compare(const void *a, const void *b) { return (*(int *)b - *(int *)a); }

double elapsed(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {

  const char *file_name = "day1_input.txt";
  FILE *fptr;
  fptr = fopen(file_name, "r");

  if (fptr == NULL) {
    printf("Error opening file: %s", file_name);
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  int first_row[TOTAL_LINES];
  int first_smallest_indices[TOTAL_LINES];
  int second_row[TOTAL_LINES];
  int second_smallest_indices[TOTAL_LINES];

  int counter = 0;
  while ((read = getline(&line, &len, fptr)) != -1) {
    char *tok = strtok(line, DELIMITER);
    int word_counter = 0;
    while (tok != NULL) {
      int num = atoi(tok);
      if (word_counter % 2 == 0) {
        first_row[counter] = num;
      } else {
        second_row[counter] = num;
      }
      word_counter++;
      tok = strtok(NULL, DELIMITER);
    }
    counter++;
  }

  qsort(first_row, TOTAL_LINES, sizeof(int), compare);
  qsort(second_row, TOTAL_LINES, sizeof(int), compare);

  // First part
  int total = 0;
  for (int i = 0; i < TOTAL_LINES; i++) {
    total += abs(second_row[i] - first_row[i]);
  }

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  int total_similarity = 0;
  for (int i = 0; i < TOTAL_LINES; i++) {
    int times_appeared = 0;
    for (int j = 0; j < TOTAL_LINES; j++) {
      if (first_row[i] == second_row[j]) {
        times_appeared++;
      }
    }
    total_similarity += first_row[i] * times_appeared;
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed_time_nested = elapsed(start, end);
  printf("Time taken for total_similarity: %.9f seconds\n",
         elapsed_time_nested);

  clock_gettime(CLOCK_MONOTONIC, &start);
  total_similarity = 0;
  int *frequency = (int *)malloc(second_row[0] * sizeof(int));
  for (int i = 0; i < TOTAL_LINES; i++) {
    frequency[second_row[i]]++;
  }
  for (int i = 0; i < TOTAL_LINES; i++) {
    total_similarity += first_row[i] * frequency[first_row[i]];
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed_time_hash = elapsed(start, end);
  printf("Time taken for total_similarity: %.9f seconds\n", elapsed_time_hash);
  if (elapsed_time_hash < elapsed_time_nested && elapsed_time_hash != 0) {
    printf("Second solution is %.2f times faster than the first one\n",
           elapsed_time_nested / elapsed_time_hash);
  }

  printf("Total: %d\n", total);
  printf("Total Similarity: %d\n", total_similarity);

  fclose(fptr);
  if (line) {
    free(line);
  }
  free(frequency);
  return 0;
}
