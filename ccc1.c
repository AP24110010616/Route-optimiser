#include <stdio.h>
#include <limits.h>
#include <string.h>

#define MAX 10

int n;
int dist[MAX][MAX];
char cities[MAX][50];

int memo[1 << MAX][MAX];
int parent[1 << MAX][MAX];

// -----------------------------
// GREEDY APPROACH
// -----------------------------
void greedy_tsp() {
    int visited[MAX] = {0};
    int route[MAX + 1];
    int total_cost = 0;

    int current = 0;
    route[0] = current;
    visited[current] = 1;

    for (int i = 1; i < n; i++) {
        int next_city = -1;
        int min_dist = INT_MAX;

        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[current][j] < min_dist) {
                min_dist = dist[current][j];
                next_city = j;
            }
        }

        route[i] = next_city;
        visited[next_city] = 1;
        total_cost += min_dist;
        current = next_city;
    }

    total_cost += dist[current][0];
    route[n] = 0;

    printf("\n🔹 Greedy Route:\n");
    for (int i = 0; i <= n; i++) {
        printf("%s", cities[route[i]]);
        if (i < n) printf(" -> ");
    }
    printf("\nCost: %d\n", total_cost);
}

// -----------------------------
// DP (TSP)
// -----------------------------
int min(int a, int b) {
    return a < b ? a : b;
}

int tsp(int mask, int pos) {
    if (mask == (1 << n) - 1)
        return dist[pos][0];

    if (memo[mask][pos] != -1)
        return memo[mask][pos];

    int ans = INT_MAX;

    for (int city = 0; city < n; city++) {
        if ((mask & (1 << city)) == 0) {
            int newAns = dist[pos][city] +
                         tsp(mask | (1 << city), city);

            if (newAns < ans) {
                ans = newAns;
                parent[mask][pos] = city; // store path
            }
        }
    }

    return memo[mask][pos] = ans;
}

// -----------------------------
// PRINT DP PATH
// -----------------------------
void print_dp_path() {
    int mask = 1;
    int pos = 0;

    printf("\n🔹 Optimal Route (DP):\n");
    printf("%s", cities[0]);

    while (mask != (1 << n) - 1) {
        int next = parent[mask][pos];
        printf(" -> %s", cities[next]);

        mask |= (1 << next);
        pos = next;
    }

    printf(" -> %s", cities[0]); // return to start
}

// -----------------------------
// MAIN
// -----------------------------
int main() {
    printf("🚗 Smart Route Optimizer (C Version)\n\n");

    printf("Enter number of cities: ");
    scanf("%d", &n);

    // Input city names
    for (int i = 0; i < n; i++) {
        printf("Enter city %d name: ", i);
        scanf("%s", cities[i]);
    }

    // Input distance matrix
    printf("\nEnter distance matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("%s: ", cities[i]);
        for (int j = 0; j < n; j++) {
            scanf("%d", &dist[i][j]);
        }
    }

    // Initialize memo & parent
    for (int i = 0; i < (1 << n); i++) {
        for (int j = 0; j < n; j++) {
            memo[i][j] = -1;
            parent[i][j] = -1;
        }
    }

    // Run Greedy
    greedy_tsp();

    // Run DP
    int optimal_cost = tsp(1, 0);

    // Print DP path
    print_dp_path();

    printf("\nOptimal Cost (DP): %d\n", optimal_cost);

    return 0;
}