#include <stdio.h>
#define MAX 100
int adj[MAX][MAX];
int par[MAX];
int rnk[MAX];
int eu[MAX * MAX];
int ev[MAX * MAX];
int ew[MAX * MAX];
int ecnt;
int mu[MAX];
int mv[MAX];
int mw[MAX];
int mcnt;
int find(int x)// поиск корень компоненты
{
    if (par[x] != x)
        par[x] = find(par[x]);
    return par[x];
}
void unite(int a, int b) // объединяем две компонт 
{
    a = find(a);
    b = find(b);
    if (a == b)
        return;
    if (rnk[a] < rnk[b])
    {
        int t = a;
        a = b;
        b = t;
    }
    par[b] = a;
    if (rnk[a] == rnk[b])
        rnk[a]++;
}
void sorted() // сортируем  рёбра по весу пузырьком 
{
    for (int i = 0; i < ecnt - 1; i++)
        for (int j = 0; j < ecnt - i - 1; j++)
            if (ew[j] > ew[j + 1])
            {
                int t;
                t = ew[j];
                ew[j] = ew[j + 1];
                ew[j + 1] = t;
                t = eu[j];
                eu[j] = eu[j + 1];
                eu[j + 1] = t;
                t = ev[j];
                ev[j] = ev[j + 1];
                ev[j + 1] = t;
            }
}
int main()
{
    int n;
    printf("enter vertices count: ");
    scanf("%d", &n);
    printf("enter adjacency matrix (%dx%d):\n", n, n);
    for (int i = 0; i < n; i++) // читение графа 
        for (int j = 0; j < n; j++)
            scanf("%d", &adj[i][j]);
    ecnt = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (adj[i][j] != 0)
            {
                eu[ecnt] = i;
                ev[ecnt] = j;
                ew[ecnt] = adj[i][j];
                ecnt++;
            }
    sorted();
    for (int i = 0; i < n; i++)
    {
        par[i] = i;
        rnk[i] = 0;
    }
    mcnt = 0;
    int tw = 0;
    for (int i = 0; i < ecnt; i++)
    {
        if (find(eu[i]) != find(ev[i]))
        {
            unite(eu[i], ev[i]);
            mu[mcnt] = eu[i];
            mv[mcnt] = ev[i];
            mw[mcnt] = ew[i];
            mcnt++;
            tw += ew[i];
        }
    }
    printf("\nminimum spanning tree:\n");
    for (int i = 0; i < mcnt; i++)
        printf("%d -> %d  weight: %d\n", mu[i] + 1, mv[i] + 1, mw[i]);
    printf("total weight: %d\n", tw);
    return 0;
}
