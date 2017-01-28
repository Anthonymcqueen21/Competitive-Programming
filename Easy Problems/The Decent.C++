#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{

    // game loop
    while (1) {
        int max = 0;
        int imax = 0;
        for (int i = 0; i < 8; i++) {
            int mountainH; // represents the height of one mountain, from 9 to 0. Mountain heights are provided from left to right.
            scanf("%d", &mountainH);
            
            if (mountainH > max) {
                max = mountainH;
                imax = i;
            }
        }

        printf("%d\n", imax);
    }

    return 0;
}
