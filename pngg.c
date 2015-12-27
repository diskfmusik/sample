#include <stdio.h>

int main()
{
    int i;
    for(i = 0; i < 16; i++){ // MAGIC , IHDR: size, type
        getchar();
    }

    int w, h;

    unsigned char buf[4];
    fread(buf, sizeof(buf), 1, stdin);
    w = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | (buf[3] << 0);
    printf("w : %d\n", w);
    fread(buf, sizeof(buf), 1, stdin);
    h = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | (buf[3] << 0);
    printf("h : %d\n", h);

    return 0;
}