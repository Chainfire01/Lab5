#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define INFINITY 9999
#define MAX 410

void delay(int sekundy) {
    /*int milli_sekundy = 1000 * sekundy;
    clock_t start_time = clock; 
    while (clock() < start_time + milli_sekundy);*/
    unsigned int retTime = time(0) + sekundy;   // Get finishing time.
    while (time(0) < retTime);
}

int conv(int a, int b, int y, int x) {
    if (a == 0 || b == 0)return 0;
    int wyn = ((a-1)* x) + b;
        return wyn;
}

int survivals(char** map, int x, int y) {
    int wyn = 0;
    for (int i = 1; i <= y; i++) {
        for (int j = 1; j <= x; j++) {
            if (map[i][j] == '*')wyn++;
        }
    }
    return wyn;
}

void show(char** map, int x, int y) {
   
    for (int i = 0; i <= y + 1; i++) {
        for (int j = 0; j <= x + 1; j++) {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    } 
    return;
}

void step(char** map, int x, int y) {
    for (int i = 0; i <= y + 1; i++) {
        for (int j = 0; j <= x + 1; j++) {
            if (map[i][j] == '*') {
                int randx = rand() % 3 - 1 , randy = rand() % 3 - 1;
                while (map[i + randy][j + randx] != '.' || (randx==0 && randy==0)) {
                    randy = rand() % 3 - 1;
                    randx = rand() % 3 - 1;
                }
                map[i][j] = '.';
                map[i + randy][j + randx] = '*';
            }
        }
    }
    return;
}

void step_alert(char** map, int x, int y, int** alert_trace,int meta) {
    for (int i = 1; i <= y; i++) {
        for (int j = 1; j <= x; j++) {
            if (map[i][j] == '*') {
                int nod = conv(i, j, y, x);
                int next_hop = alert_trace[nod][1];
                int a = (next_hop - 1) / x;
                int b = next_hop - (a) * x;
                a++;
                //printf("next_hop =%di=%d, j=%d\n",next_hop,a,b);
                if (map[a][b] == '.') {
                    map[a][b] = '?';
                    map[i][j] = '.';
                }
                if (map[a][b] == '$') {
                    map[i][j] = '.';
                }
            }
        }
    }
    for (int i = 1; i <= y; i++) {
        for (int j = 1; j <= x; j++) {
            if (map[i][j] == '?')map[i][j] = '*';
        }
    }
    return;
}

void dijkstra(int** G, int n, int startnode,int** alert_trace, int meta){
    int cost[MAX][MAX], distance[MAX], pred[MAX];
    int visited[MAX], count, mindistance, nextnode, i, j;
    nextnode = 0;
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    for (i = 1; i <= n; i++)
        for (j = 1; j <= n; j++)
            if (G[i][j] == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = G[i][j];

    //initialize pred[],distance[] and visited[]
    for (i = 1; i <= n; i++)
    {
        distance[i] = cost[startnode][i];
        pred[i] = startnode;
        visited[i] = 0;
    }

    distance[startnode] = 0;
    visited[startnode] = 1;
    count = 1;
    while (count < n -1)
    {
        mindistance = INFINITY;

        //nextnode gives the node at minimum distance
        for (i = 1; i <= n; i++)
            if (distance[i] < mindistance && !visited[i])
            {
                mindistance = distance[i];
                nextnode = i;
            }

        //check if a better path exists through nextnode            
        visited[nextnode] = 1;
        for (i = 1; i <= n; i++)
            if (!visited[i])
                if (mindistance + cost[nextnode][i] < distance[i])
                {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
        count++;
    }
//print the path and distance of each node
/*for (i = 1; i <= n; i++){
    if (i != startnode)
    {
        printf("\nDistance of node%d=%d", i, distance[i]);
        printf("\nPath=%d", i);

        j = i;
        do
        {
            j = pred[j];
            printf("<-%d", j);
        } while (j != printf("\n");
} 
    }*/
    i = meta;
    j = distance[meta];
    alert_trace[startnode][j] = meta;
    do
    {
        j--;
        i = pred[i];
        //printf("<-%d", i, j);
        alert_trace[startnode][j] = i;
    } while (i != startnode);
}

int main() {
    int x, y, zarodek, meta;
    char **map;
    int **incydencja, **alert_trace;
    time_t tt;
    zarodek = time(&tt);
    srand(zarodek);
    printf("Wprowadz wymiar planszy x i y:   ");
    scanf_s("%d%d", &x, &y);
    
    map = (char**)malloc((y + 1) * sizeof(char*));
    for (int i = 0; i <= y + 1; i++) {
        map[i] = (char*)malloc((x + 1) * sizeof(char));
    }
    incydencja = (int**)malloc((y + 1)*(x + 1) * sizeof(int*));
    for (int i = 0; i <= (y + 1)*(x + 1); i++) {
        incydencja[i] = (int*)malloc((x + 1)*(y + 1) * sizeof(int));
    }
    alert_trace = (int**)malloc((y + 1) * (x + 1) * sizeof(int*));
    for (int i = 0; i <= (y + 1) * (x + 1); i++) {
        alert_trace[i] = (int*)malloc((x + 1) * (y + 1) * sizeof(int));
    }
    for (int i = 0; i <= (y+1)*(x+1); i++) {
        for (int j = 0; j <= (x+1)*(y+1); j++) {
            incydencja[i][j] = 0;
            alert_trace[i][j] = 0;
        }
    }
    printf("Wprowadz plansze, #sciana *czlowiek .przestrzen $wyjscie:\n");
    for (int i = 1; i <= y; i++) {
        for (int j = 1; j <= x; j++) {
            scanf_s(" %c", &map[i][j]);
            if (map[i][j] == '$') {
                meta = conv(i, j, y, x);
            }
        }
    }
    for (int i = 0; i <= x + 1; i++) {
        map[0][i] = '#';
        map[y + 1][i] = '#';
    }
    for (int i = 0; i <= y + 1; i++) {
        map[i][0] = '#';
        map[i][x+1] = '#';
    }
    system("CLS");
    show(map,x,y);
    system("PAUSE");
    system("CLS");
   for (int i = 1; i <= y;i++) {
        for (int j = 1; j <= x;j++) {
            if (map[i][j] != '#') {
                if (map[i + 1][j] != '#') {
                    incydencja[conv(i + 1, j, y, x)][conv(i, j, y, x)] = 1;
                    incydencja[conv(i , j, y, x)][conv(i+1, j, y, x)] = 1;
                }
                else {
                    incydencja[conv(i + 1, j, y, x)][conv(i, j, y, x)] = 0;
                    incydencja[conv(i, j, y, x)][conv(i+1, j, y, x)] = 0;
                }
                if (map[i][j + 1] != '#') {
                    incydencja[conv(i, j + 1, y, x)][conv(i, j, y, x)] = 1;
                    incydencja[conv(i, j, y, x)][conv(i, j + 1, y, x)] = 1;
                }
                else {
                    incydencja[conv(i, j + 1, y, x)][conv(i, j, y, x)] = 0;
                    incydencja[conv(i, j, y, x)][conv(i, j+1, y, x)] = 0;
                }
                if (map[i - 1][j] != '#') {
                    incydencja[conv(i - 1, j, y, x)][conv(i, j, y, x)] = 1;
                    incydencja[conv(i, j, y, x)][conv(i - 1, j, y, x)] = 1;
                }
                else {
                    incydencja[conv(i - 1, j, y, x)][conv(i, j, y, x)] = 0;
                    incydencja[conv(i, j, y, x)][conv(i - 1, j, y, x)] = 0;
                }
                if (map[i][j - 1] != '#') {
                    incydencja[conv(i, j - 1, y, x)][conv(i, j, y, x)] = 1;
                    incydencja[conv(i, j, y, x)][conv(i, j - 1, y, x)] = 1;
                }
                else {
                    incydencja[conv(i, j - 1, y, x)][conv(i, j, y, x)] = 0;
                    incydencja[conv(i, j, y, x)][conv(i, j - 1, y, x)] = 0;
                }
                incydencja[conv(i,j,y,x)][conv(i,j,y,x)] = 0;
            }
        }
    }
   for (int i = 1; i <= y; i++) {
       for (int j = 1; j <= x; j++) {
           if (map[i][j] != '#') {
               int nod = conv(i, j, y, x);
               dijkstra(incydencja, x * y, nod, alert_trace, meta);
           }
       }
   }
while (1) {
    step(map, x, y);
    show(map, x, y);
    delay(1);
    system("CLS");
    int danger = rand() % 10 + 1;
    if (danger < 2)break;
}
printf("NASTEPUJE ALARM!!!");
delay(3);
system("CLS");
while (survivals(map, x, y) > 0) {
    step_alert(map, x, y, alert_trace, meta);
    show(map, x, y);
    delay(1);
    system("CLS");
}
printf("WSZYSCY UCIEKLI :)");
	return 0;
}