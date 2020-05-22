#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	errno_t err;
	FILE* f;
    char str[200];
    int matrix[200][200], x = 0, y = 0, x_flag = 0, zero_d=0, zero_g=0;

	printf("Zaraz pobiore twoj plik\n");
    err = fopen_s(&f, "tescik.csv", "r");
    if (err == 0) {
        printf("Otworzylismy twoj plik :D\n");
    }
    else {
        printf("Nie udalo sie otworzyc twojego pliku :(\n");
        return 0;
    }
    fseek(f, 0, SEEK_SET); 
    while (!feof(f)) {
        fscanf_s(f, "%s", str, _countof(str));
        int dl = strlen(str);
        //printf("%s\n", str);
        for (int it = dl - 1;it >= 0; it--){
            int pot = 1, num = 0;
            while (it >= 0 && str[it] != ';') {
                //printf("Whilek\n");
                num += (str[it] - 48) * pot;
                pot *= 10;
                it--;
           }
            //printf("%d\n", num);
            matrix[x][y] = num;
            x++;
        }
        if (x_flag == 0)x_flag = x;
        if (x != x_flag) {
            printf("STOP! To nie jest macierz!!!\n");
            return 0;
        }
        x = 0;
        y++;
    }
    x = x_flag;
    y--;
    //printf("%d %d", x, y);
    if (x != y) {
        printf("STOP! To nie jest macierz kwadratowa!!!\n");
        return 0;
    }
    for (int i = 0; i < y; i++) {
        for (int j = x-1; j >= 0; j--) {
            printf("%d ", matrix[j][i]);
        }
        printf("\n");
    }
    for (int i = 0; i < x; i++) {
        int j = 0;
        while (j != x-i-1) {
            if (matrix[i][j] == 0) {
                zero_g++;
            }
            j++;
        }
        j++;
        while (j < y) {
            if (matrix[i][j] == 0) {
                zero_d++;
            }
            j++;
        }
        //printf("Zera_g %d \n", zero_g);
        //printf("Zera_d %d \n", zero_d);
    }
    //printf("\nZera_g %d \n", zero_g);
    //printf("Zera_d %d \n", zero_d);
    if (zero_g == x * (x - 1) / 2)printf("\nTo macierz trojkatna gorna!\n");
    if (zero_d == x * (x - 1) / 2)printf("\nTo macierz trojkatna dolna!\n");
    if (zero_g == x * (x - 1) / 2  &&  zero_g == zero_d)printf("\nTo macierz trojkatna gorna i dolna!\n");
    if (zero_g != x * (x - 1) / 2 && zero_d != x * (x - 1) / 2)printf("\nTa macierz nie jest trojkatna!\n");
    err = fclose(f);
    system("PAUSE");
	return 0;
}