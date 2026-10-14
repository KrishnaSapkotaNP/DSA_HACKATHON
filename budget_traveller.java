import java.util.Scanner;

public class BudgetTravelerSimple {
    
    static int[][] graph = new int[105][105];
    static int N, M, K, C;
    static int minCost = -1;

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        N = sc.nextInt();
        M = sc.nextInt();

        K = sc.nextInt();
        C = sc.nextInt();

        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                graph[i][j] = -1;
            }
        }

        for (int i = 0; i < M; i++) {
            int u = sc.nextInt();
            int v = sc.nextInt();
            int p = sc.nextInt();
            graph[u][v] = p;
        }

        findPath(1, 0, 0, 0);

        System.out.println(minCost);
        sc.close();
    }

    public static void findPath(int currentCity, int currentCost, int stops, int couponsUsed) {
        if (currentCity == N) {
            if (minCost == -1 || currentCost < minCost) {
                minCost = currentCost;
            }
            return;
        }

        if (minCost != -1 && currentCost >= minCost) {
            return;
        }

        for (int nextCity = 1; nextCity <= N; nextCity++) {
            if (graph[currentCity][nextCity] != -1) {
                int price = graph[currentCity][nextCity];

                if (stops + 1 <= K) {
                    findPath(nextCity, currentCost + price, stops + 1, couponsUsed);

                    if (couponsUsed < C) {
                        int discountedPrice = price / 2;
                        findPath(nextCity, currentCost + discountedPrice, stops + 1, couponsUsed + 1);
                    }
                }
            }
        }
    }
}