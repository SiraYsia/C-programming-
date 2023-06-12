/* Yeabsira Tamiru
    117857740 */

#include <math.h>

#include <stdio.h>

#define MAX 50

/*This function takes the number of assignment and an array of the data
 * provided.*/
void assignment_info(int num, int data[]) {
  int i;

  /* a maximum of 4 columns holding the assignmnet number, score, weight and
     days late. num * 4 = total data provided. Increment by 4 after each row
     execution */
  for (i = 0; i < (num * 4); i += 4) {
    scanf("%d, %d, %d, %d", &data[i], &data[i + 1], &data[i + 2], &data[i + 3]);
  }
}

/* This function calculates the mean. penality stands for points taken off
   per days late */
static double mean2(int assignments[], int num, int penality) {
  int i;
  double average;

  for (i = 0; i < (4 * num); i += 4) {
    /*The current average is the first score after the deduction of points for
     * late assugnment.*/
    average += assignments[i + 1] - (penality * assignments[i + 3]);
  }
  /*after the scores are all added up together divde by the total number of
   * assignmnets to get the actual average */
  average /= num;
  return average;
}

/* This functions drops an assignment based on the number of assignments to drop
provided by the user. It then sets the weight to 0 so its skipped in calculation
*/
void drop_assignment(int assignments[], int num, int drop_num) {
  int i, j, temp_lowest, lowest, index_to_drop = 0;

  /*first assignment is assigned as the lowest */
  /*this loops until the number of assignment to be droped is reached */
  for (j = 0; j < drop_num; j++) {
    lowest = assignments[1] * assignments[2];
    /*loops the entire data to find the lowest score and assigns it to
     * temp_lowest*/
    for (i = 0; i < (num * 4); i += 4) {
      temp_lowest = assignments[i + 1] * assignments[i + 2];
      /*if temp_lowest is not 0 and the lowest score is found identify its
       * index*/
      if (temp_lowest != 0) {
        if (temp_lowest < lowest) {
          lowest = temp_lowest;
          index_to_drop = i;
        }
      }
    }
    /*the weight of the lowest score is now 0. */
    assignments[index_to_drop + 2] = 0;
  }
}

/*This function calculate the numeric score. It takes the number of assignments,
points taken off per days late, and the number of assignments to be droped*/
static double numeric_score2(int assignments[], int num, int penality,
                             int drop_num) {
  /* if there is more than one assignment to be droped drop the assignment
     per last function */
  int i, curr;
  double total_weight = 0;
  double final_score = 0;

  if (drop_num > 0) {
    drop_assignment(assignments, num, drop_num);
  }
  for (i = 0; i < (num * 4); i += 4) {
    total_weight += assignments[i + 2];
  }
  /*To make sure the weight does not excede 100 percent */
  if (total_weight > 100.0) {
    printf("ERROR: Invalid values provided");
    return 0;

  } else {
    for (i = 0; i < (num * 4); i += 4) {
      /*curr holds the value of an assignment after late penality points are
       * deducted*/
      curr = assignments[i + 1] - (penality * assignments[i + 3]);
      /*calculates the numeric score */
      final_score += (curr * (assignments[i + 2]) / total_weight);
    }
  }
  return final_score;
}

/*This function returns the standard deviation of a data*/
static double Standard_deviation(int assignments[], int num, int penality) {
  int i;
  int score;
  double mean, sta_dev, variance;

  mean = mean2(assignments, num, penality);

  for (i = 0; i < (num * 4); i += 4) {
    /* score calculated based on points deducted for late submission and
     * penality */
    score = assignments[i + 1] - (assignments[i + 3] * penality);
    /*sum all the powers after calucalting deviation from the mean for each
    data*/
    variance += pow((score - mean), 2);
  }
  /*divide the variance by total number of assignments and return the sqrt as
   * the standard deviation */
  sta_dev = sqrt(variance / (float)num);
  return sta_dev;
}

/*This function prints all the data associated with an
assignment in a chronological order of the assignment number*/
void order(int num, int assignments[]) {
  int j = 1, i;

  while (j <= num) {
    for (i = 0; i < (num * 4); i += 4) {
      /*if at each new begining index  matches j which is incremented by one
      every execution print the rest of the data*/
      if (assignments[i] == j) {
        printf("%d, %d, %d, %d\n", assignments[i], assignments[i + 1],
               assignments[i + 2], assignments[i + 3]);
      }
    }
    j++;
  }
}
/*This function prints the output with all the information*/
void output(int assignments[], int num, int penality, int drop_num, char stat) {
  int i, temp_assignments[MAX * 4];
  double mean, numeric_score, standard_deviation;

  /*makes a copy of the assignments array */
  for (i = 0; i < (num * 4); i++) {
    temp_assignments[i] = assignments[i];
  }
  /*if the number of assignments to be droped is less than 0 and it exceeds the
  total number of assignment the numeric score is set 0 else calculate the
  numeric score normally.*/
  if (drop_num <= 0 && num < drop_num) {
    numeric_score = 0;
  } else {
    numeric_score = numeric_score2(temp_assignments, num, penality, drop_num);
  }
  printf("Numeric Score: %5.4f\n", numeric_score);
  printf("Points Penalty Per Day Late: %d\n", penality);
  printf("Number of Assignments Dropped: %d\n", drop_num);

  printf("Values Provided:\n");
  printf("Assignment, Score, Weight, Days Late\n");
  order(num, assignments);

  if (stat == 'Y' || stat == 'y') {
    mean = mean2(assignments, num, penality);
    standard_deviation = Standard_deviation(assignments, num, penality);
    printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean,
           standard_deviation);
  }
}

int main() {
  int num, penality, drop_num, assignments[MAX * 4];
  char stat;

  /*points per day late, number of assignments to drop, stats */
  scanf("%d %d %c", &penality, &drop_num, &stat);
  /* Number of assignments */
  scanf("%d", &num);
  /*scans the data */
  assignment_info(num, assignments);

  /*if total number of assignments, penality per day late and assignments to be
  droped is all less than 0 return 0, other wise print the output*/
  if (!(num < 0) && !(penality < 0) && !(drop_num < 0)) {
    output(assignments, num, penality, drop_num, stat);
  }
  return 0;
}