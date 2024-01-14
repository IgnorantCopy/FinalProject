//
// Created by Ignorant on 2024/1/14.
//


#include <stdio.h>

void add(int *num);
int main() {
    int n = 0;
    add(&n);
    printf("%d", n);
    return 0;
}

void add(int *num) {
    *num = *num + 1;
}
