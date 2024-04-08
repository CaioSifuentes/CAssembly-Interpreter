#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"

int main(void)
{
    //lerArquivo("./operação1.txt");
    //lerArquivo("./operação2.txt");
    reg[0] = 3;
    reg[1] = 8;
    reg[2] = 2;
    int memoriaInteiro = 0;

    while (1){
        for (int i = 0; i < 4; i++) memoriaInteiro = (memoriaInteiro << 8) | memoria[4 + i];
        // printf("%d", memoriaInteiro);
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

void lerArquivo(const char *Arquivo){
    char i = 0;
    FILE *arquivo;
    arquivo = fopen(Arquivo, "r");
    if(arquivo == NULL){
        perror("Nao foi possivel abrir o arquivo");
        return;
    }
    char buffer[256];
    while(fgets(buffer,sizeof(buffer), arquivo)){
        buffer[strcspn(buffer, "\n")] = 0;
        //insere(buffer);
    }
    fclose(arquivo);
}

void printBinario(int num) {
    printf("0b");
    for(int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
}

void showDashboard() {
    system("cls");
    printf("======================================================================\n");
    printf("| MBR: "); printBinario(mbr); printf("|\n");
    printf("| IR: %d | MAR: %u |\n", ir, mar);
    printf("| Ro0: %d | Ro1: %d | Ro2: %d | IMM: %d |\n", ro0, ro1, ro2, imm);
    printf("-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-\n");
    printf("| Registradores:\n");
    for (int i = 0; i < 16; i++) {
        printf("| Reg[%d]: %u | ", i, reg[i]);
        if (i % 2 == 1) printf("\n");
    }
    printf("-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-\n");
    printf("| E: %d | L: %d | G: %d |\n", e, l, g);
    printf("| PC: %u |\n", pc);
    printf("======================================================================\n");
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

        mar = mbr & 0b1111111111111111111;
    }
    else if (ir >= 7 && ir <= 13){
        ro0 = mbr >> 23;
        ro0 = ro0 & 0b1111;

        ro1 = mbr >> 19;
        ro1 = ro1 & 0b1111;

        ro2 = mbr >> 15;
        ro2 = ro2 & 0b1111;
    }
    else if (ir >= 14 && ir <= 15){
        ro0 = mbr >> 23;
        ro0 = ro0 & 0b1111;

        mar = mbr & 0b11111111111111111111111;
    }
    else if (ir >= 16 && ir <= 23){
        ro0 = mbr >> 23;
        ro0 = ro0 & 0b1111;

        imm = mbr & 0b11111111111111111111111;
    }
    else if (ir >= 24 && ir <= 30){
        mar = mbr & 0b11111111111111111111111;
    }
    else {}
}

void Executa (void)
{
    if (ir == 1) // NO OPERATION
    {}
    else if (ir == 2) // LOGICAL-NOT ON REGISTER
    {
        reg[ro0] = !(reg[ro0]);
    }
    else if(ir == 3){ // MOVE REGISTER
        reg[ro0] = reg[ro1];
    }
    else if (ir == 4) // COMPARE REGISTER
    {
        if (reg[ro0] == reg[ro1]) e = 1;
        else e = 0;

        if (reg[ro0] < reg[ro1]) l = 1;
        else l = 0;

        if (reg[ro0] > reg[ro1]) g = 1;
        else g = 0;
    }
    else if(ir == 5) // LOAD VIA BASE+OFFSET
     { 
        reg[ro0] = memoria[mar + reg[ro1]]; // Não sei se é assim, esse tava dificil kkkk------------------------------------------------
    }
    else if (ir == 6) // STORE VIA BASE+OFFSET
    {
        // Não sei fazer. ------------------------------------------------
    }
     else if(ir == 7){ // ADD REGISTER
        reg[ro0] = reg[ro1] + reg[ro2];
    }
    else if (ir == 8) // SUBTRACT REGISTER
    {
        reg[ro0] = reg[ro1] - reg[ro2];
    }
     else if(ir == 9){ // MULTIPLY REGISTER
        reg[ro0] = reg[ro1] * reg[ro2];
    }
    else if (ir == 10) // DIVIDE REGISTER
    {
        reg[ro0] = reg[ro1] / reg[ro2];
    }
    else if(ir == 11){ // LOGICAL-AND ON REGISTER
        reg[ro0] = reg[ro1] & reg[ro2];
    }
    else if (ir == 12) // LOGICAL-OR ON REGISTER
    {
        reg[ro0] = reg[ro1] | reg[ro2];
    }
    else if(ir == 13) // LOGICAL-XOR ON REGISTER
    { 
        reg[ro0] = reg[ro1] ^ reg[ro2];
    }
    else if(ir == 14) // LOAD
    {
        for (int i = 0; i < 4; i++) 
            reg[ro0] = (reg[ro0] << 8) | memoria[mar + i];
    }
    else if(ir == 15) // STORE
    {
        for (int i = 0; i < 4; i++) 
            memoria[mar + i] = (reg[ro0] << (24 - (8*i))) & 0b11111111; //------------------------------------------------------------//
    }
    else if(ir == 16) // MOVE IMMEDIATE TO THE LOWER HALF OF THE REGISTER
    {
        reg[ro0] = (imm & 0b1111111111111111);
    }
    else if(ir == 17) // MOVE IMMEDIATE TO THE HIGHER HALF OF THE REGISTER
    {
        reg[ro0] = ((imm & 0b1111111111111111) << 16) | (reg[ro0] & 0b1111111111111111); //------------------------------------------------------------//
    }
    else if(ir == 18) // ADD IMMEDIATE
    {
        reg[ro0] = reg[ro0] + imm;
    }
    else if(ir == 19) // SUBTRACT IMMEDIATE
    { 
        reg[ro0] = reg[ro0] - imm;
    }
    else if(ir == 20) // MULTIPLY IMMEDIATE
    {
        reg[ro0] = reg[ro0] * imm;
    }
    else if(ir == 21) // DIVIDE IMMEDIATE
    { 
        reg[ro0] = reg[ro0] / imm;
    }
    else if(ir == 22) // LEFT SHIFT
    {
        reg[ro0] = reg[ro0] << imm;
    }
    else if(ir == 23) // RIGHT SHIFT
    {
        reg[ro0] = reg[ro0] >> imm;
    }
    else if(ir == 24) // JUMP IF EQUAL TO
    {
        if(e == 1) pc = mar; //----------------------------------------------------------------------------//
    }
    else if(ir == 25) // JUMP IF NOT EQUAL TO
    {
        if(e == 0) pc = mar;
    }
    else if(ir == 26) // JUMP IF LOWER THA
    {
        if(l == 1) pc = mar;
    }
    else if(ir == 27) // JUMP IF LOWER THAN OR EQUAL TO
    {
        if(e == 1 || l == 1) pc = mar;
    }
    else if(ir == 28) // JUMP IF GREATER THAN
    {
        if(g == 1) pc = mar;
    }
    else if(ir == 29) // JUMP IF GREATER THAN OR EQUAL TO
    {
        if(e == 1 || g == 1) pc = mar; //----------------------------------------------------------------------------//
    }
    else if(ir == 30) // JUMP
    {
        pc = mar;
    }

    if (ir != 0) // Se a instrução for diferente de HALT.
        pc = pc + 4;
}