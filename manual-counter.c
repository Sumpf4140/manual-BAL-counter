#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

//Globals
int DEFAULT = 600;
int M_KEY = 1;
int L_KEY = 2;
int N_KEY = 3;
int E_KEY = 4;
int EXIT_KEY = 0;

// how many different cell types to differentiate
#define MAX 4

//Array of possible cells, they have name, and count
typedef struct
{
    char *name;
    int count;
}
cell_types;

//sets max for array
cell_types cells[MAX];

//prototypes of functions
double calc_perc(double a, double b);
double counter(int totalcount, int argc);
void print_results(long time1, int sum_counted, int argc);


int main(int argc, char **argv)
{
    // checks if a predifined amount of command lines is given
    if (argc > 3)
    {
        printf("Usage: ./counter  Number_of_cells_wanting_to_count  nosave_.csv\n");
        return 1;
    }

    // if no comamnd line imput is given 600 is the default value
    int totalcount = DEFAULT;
    if (argc == 1)
    {
        printf("\nCells to count set to default %i\n", totalcount);
    }

    // takes command line imput and checks it, if it is valid updates totalcount
    if (argc > 2)
    {
        char *t;
        long temp = strtol(argv[1], &t, 10);
        if (temp > INT_MAX)
        {
            printf("Do you really want to count %ld cells?\nI hope not.\n", temp);
            return 1;
        }
        if (temp == 1)
        {
            printf("Do you really need this program to count %ld cell?\n", temp);
            return 1;
        }
        if (temp < 1)
        {
            printf("Value %ld not allowed\n", temp);
            return 1;
        }
        totalcount = temp;
        printf("\nCells to count set to %i\n", totalcount);
    }

    if (argc == 3)
    {
        printf("Results will be saved as .csv into the same folder as counter.exe\n");
    }

    // populates the empty array
    cells[0].name = "Makrophagen";
    cells[1].name = "Lymphozyten";
    cells[2].name = "neutrophile Granulozyten";
    cells[3].name = "eosinophile Granulozyten";

    // counts the cells that were nooded, calculates time that was needed

    counter(totalcount, argc);

}




double counter(int totalcount, int argc)
{
    long time1 = time(NULL);

    printf("What keys to press:\n%i = Makrophage\n%i = Lymphozyt\n%i = Neutrophiler Granulozyt\n%i = Eosinophiler Granulozyt\n",
    M_KEY, L_KEY, N_KEY, E_KEY);

    // sets all counts to 0
    for (int i = 0; i < MAX; i++)
    {
        cells[i].count = 0;
    }

    int sum_counted = 0;

    while (sum_counted < totalcount)
    {
        int counted = get_int("%03i/%03i: ", sum_counted + 1, totalcount);

        if (counted == M_KEY)
        {
            cells[0].count++;
            sum_counted++;
        }
        else if (counted == L_KEY)
        {
            cells[1].count++;
            sum_counted++;
        }
        else if (counted == N_KEY)
        {
            cells[2].count++;
            sum_counted++;
        }
        else if (counted == E_KEY)
        {
            cells[3].count++;
            sum_counted++;
        }
        else if (counted == EXIT_KEY)
        {
            print_results(time1, sum_counted, argc);

            int check = get_int("to EXIT press %i again, to RESUME counting any other number\n", EXIT_KEY);

            if(check == EXIT_KEY)
            {
                break;
            }
        }
        else
        {
            printf("Input %i not recognized\nWhat keys to press\n%i = Makrophage\n%i = Lymphozyt\n%i = Neutrophiler Granulozyt\n%i = Eosinophiler Granulozyt\n%i = Exit\n",
            counted, M_KEY, L_KEY, N_KEY, E_KEY, EXIT_KEY);
        }
    }

    // if no cells were noded, break
    if (sum_counted == 0)
    {
        printf("\nERROR: no inputs detected\n\n");
        return 1;
    }
    else if (sum_counted == totalcount)
    {
        print_results(time1, sum_counted, argc);
    }

    return 0;
}


//calculates the percentage of a single popluation in relation to all cells counted
double calc_perc(double a, double b)
{
    double perc = ((a / b) * 100);
    return perc;
}

void print_results(long time1, int sum_counted, int argc)
{
    long time2 = time(NULL);
    long timepassed = 0;
    timepassed = time2 - time1;

    printf("\nYou counted %i cells in %ld seconds\n", sum_counted, timepassed);
    //calculate the percentage of the cells
    for (int i = 0; i < MAX; i++)
    {
        double perc = calc_perc(cells[i].count, sum_counted);
        printf("%.2f%% %s (%i)\n", perc, cells[i].name, cells[i].count);
    }
    printf("\n");

    //prints the results to .csv into the same dir
    if (argc != 3)
    {
        FILE *fpt;
        fpt = fopen("results.csv", "w+");
        fprintf(fpt,"perc, type, count\n");
        for (int i = 0; i < MAX; i++)
        {
            double perc = calc_perc(cells[i].count, sum_counted);
            fprintf(fpt, "%.2f%% %s %i\n", perc, cells[i].name, cells[i].count);
        }
        fclose(fpt);
    }
}
