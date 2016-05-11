
#include "intelligence.h"
extern color_t cube[3][3][3];

color_t three_in_a_row_check() {
    color_t result;
    byte z, y, x;
    for (z = 0; z < 3; z++) {
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (cube[z][y][x] != 0) {
                    if (cube[z][y][0] == cube[z][y][1] && cube[z][y][0] == cube[z][y][2])
                    {
                        result = cube[z][y][0];
                        return result;
                    }
                }
            }
        }
    }
    for (z = 0; z < 3; z++) {
        for (x = 0; x < 3; x++) {
            for (y = 0; y < 3; y++) {
                if (cube[z][y][x] != 0) {
                    if (cube[z][0][x] == cube[z][1][x] && cube[z][0][x] == cube[z][2][x])
                    {
                        result = cube[z][0][x];
                        return result;
                    }
                    if (cube[0][y][x] == cube[1][y][x] && cube[0][y][x] == cube[2][y][x])
                    {
                        result = cube[0][y][x];
                        return result;
                    }
                }
                
            }

        }
    }

    for (z = 0; z < 3; z++) {
        if (cube[z][0][0] == cube[z][1][1] && cube[z][0][0] == cube[z][2][2])
        {
            result = cube[z][0][0];
        }

    }
    for (y = 0; y < 3; y++) {
        if (cube[0][y][0] == cube[1][y][1] && cube[0][y][0]== cube[2][y][2])
        {
            result = cube[0][y][0];
        }

    }
    for (x = 0; x < 3; x++) {
        if (cube[0][0][x] == cube[1][1][x] && cube[0][0][x] == cube[2][2][x])
        {
            result = cube[0][x][0];
        }

    }
    if (cube[1][1][1] != 0) {
        color_t cen = cube[1][1][1];
        if (cube[0][0][0] == cen && cube[2][2][2] == cen) {
            result = cen;
        }
        if (cube[0][2][0] == cen && cube[2][0][2] == cen) {
            result = cen;
        }
        if (cube[2][0][0] == cen && cube[0][2][2] == cen) {
            result = cen;
        }
        if (cube[2][2][0] == cen && cube[0][0][2] == cen) {
            result = cen;
        }
    }
    return result;
}


