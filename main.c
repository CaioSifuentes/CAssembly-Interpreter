#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

void printBinario(int num) { 
    printf("0b");
    for(int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}

void showDashboard() {
    printf("===============================================================\n");
    printf("Valor de mbr: %u\n", mbr);
    printf("Valor de ir: %d\n", ir);
    printf("Valor de mar: %u\n", mar);
    printf("Valor de ro0: %d\n", ro0);
    printf("Valor de ro1: %d\n", ro1);
    printf("Valor de ro2: %d\n", ro2);
    
    printf("Valores dos registradores:\n");
    for (int i = 0; i < 16; i++) {
        printf("reg[%d]: %u | ", i, reg[i]);
    }

    printf("\nValor de imm: %u\n", imm);
    printf("Valor de pc: %u\n", pc);
    printf("Valor de e: %d\n", e);
    printf("Valor de l: %d\n", l);
    printf("Valor de g: %d\n", g);
}

void Busca(void) 
{
    mbr = 0; mar = pc;
    for (int i = 0; i < 4; i++) mbr = (mbr << 8) | memoria[mar + i];
}

void Decodifica(void) 
{
    ir = mbr >> 27;
    if (ir == 2) {
        ro0 = mbr >> 23;
        ro0 = ro0 & 0b1111;
    }
    else if (ir >= 3 && ir <= 4){
        ro0 = mbr >> 23;
        ro0 = ro0 & 0b1111;

        ro1 = mbr >> 19;
        ro1 = ro1 & 0b1111;
    }
    else if (ir >= 5 && ir <= 6){
        ro0 = mbr >> 23;
        ro0 = ro0 & 0b1111;

        ro1 = mbr >> 19;
        ro1 = ro1 & 0b1111;

        mar = mar & 0b1111111111111111111;
    }
    else if (ir >= 7 && ir <= 13){
        ro0 = mbr >> 23;
        ro0 = ro0 & 0b1111;

        ro1 = mbr >> 19;
        ro1 = ro1 & 0b1111;

        ro1 = mbr >> 15;
        ro1 = ro1 & 0b1111;
    }
    else if (ir >= 14 && ir <= 15){
        ro0 = mbr >> 23;
        ro0 = ro0 & 0b1111;

        mar = mar & 0b11111111111111111111111;
    }
    else if (ir >= 16 && ir <= 23){
        ro0 = mbr >> 23;
        ro0 = ro0 & 0b1111;

        imm = imm & 0b11111111111111111111111;
    }
    else if (ir >= 24 && ir <= 30){
        mar = mar & 0b11111111111111111111111;
    }
    else {}
}

void Executa (void) 
{
    if (ir == 0) // HALT
    {
    }
    else if (ir == 1) // NO OPERATION
    {
        pc = pc + 4;
    }
    else if (ir == 2) // LOGICAL-NOT ON REGISTER
    {
        if (reg[ro0] >= 1){
            reg[ro0] = 0;
        }
        else {
            reg[ro0] = 1;
        }
    }
}

int main(void)
{ 
    lerArquivo("./operação1.txt");
    //lerArquivo("./operação2.txt");
    while (1){
        showDashboard();
        Busca();
        Decodifica();
        Executa();

        printf("Pressione Enter para continuar...");
        fflush(stdout);
        while (getchar() != '\n'); 
    }
    return 0;
}