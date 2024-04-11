#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "cpu.h"

/*
A função 'showDashboard' tem dois parâmetros de configuração:

decOuHex: Você pode alterar entre 'h' ou 'd' para mudar a impressão dos valores para hexadecimal ou decimal, respectivamente.
visibleRAM: Você pode alterar entre 0 ou 1 para definir se o programa irá imprimir a memória ou não; 0 => Falso, 1 => Verdadeiro.
 */
#define DEC_OU_HEXA 'h'
#define VISIBLE_RAM 0

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);

    // NOTA: O arquivo 'operacao0.txt' não roda nada.
    printf("Digite o nome do arquivo que você deseja executar (Exemplo: operacao1.txt):");
    int retorno = 0;
        while (retorno == 0){
        char nomeArquivo[100];
        printf("\n./codeFiles> ");
        fgets(nomeArquivo, sizeof(nomeArquivo), stdin);

        size_t len = strlen(nomeArquivo);
        if (len > 0 && nomeArquivo[len - 1] == '\n')
            nomeArquivo[len - 1] = '\0';

        char caminhoCompleto[100];
        strcpy(caminhoCompleto, "./codeFiles/");
        strcat(caminhoCompleto, nomeArquivo);

        retorno = lerArquivo(caminhoCompleto);
    }


    while (1){
        showDashboard(DEC_OU_HEXA, VISIBLE_RAM);
        Busca();
        Decodifica();
        Executa();

        printf("Pressione qualquer tecla para iniciar o próximo ciclo de máquina ou aperte CTRL+C para finalizar a execução do trabalho.\n");
        fflush(stdout);
        while (getchar() != '\n');
    }


    return 0;
}

int instructionToBinary(char instrucao[]){
char instrucoes[31][10] = {"hlt", "nop", "not", "movr", "cmp", "ldbo", "stbo", "add", "sub", "mul", "div", "and", "or", "xor", "ld", "st", "movil", "movih", "addi", "subi", "muli", "divi", "lsh", "rsh", "je", "jne", "jl", "jle", "jg", "jge", "jmp"};
    for (int i = 0; i < 31; i++)
    {
        if (strcmp(instrucoes[i], instrucao) == 0) return i;
    }
    return -1;
}

int lerArquivo(const char *Arquivo) {
    FILE *arquivo = fopen(Arquivo, "r");
    if (arquivo == NULL) {
        perror("Não foi possível abrir o arquivo");
        return 0;
    }

    char buffer[256];
    int linhaAtual = 1;
    while (fgets(buffer, sizeof(buffer), arquivo)) {
        unsigned int posMemoria; char tipoOperacao; char parsedBuffer[50];
        unsigned char pag[4]; unsigned int palavraCompleta;
        
        /*
        Remove o '\n' do buffer e muda para um '\0'.
        */
        int posicao_nova_linha = strcspn(buffer, "\n");
        if (buffer[posicao_nova_linha] == '\n') buffer[posicao_nova_linha] = '\0';

        /*
        Separa uma linha do arquivo da seguinte maneira:
        Linha de Exemplo: '0;i;ld r0, 86'

        posMemoria => Posição na memória. (Exemplo: 0)
        tipoOperacao => Tipo de Operação a ser feita entre i e d. (Exemplo: i)
        parsedBuffer => A parte restante da linha. (Exemplo: ld r0, 86)
        */
        sscanf(buffer, "%x;%c;%256[^\0]", &posMemoria, &tipoOperacao, parsedBuffer);

        if (tipoOperacao == 'i') // Se estivermos lidando com uma instrução...
        {
            /*
            Separa a instrução dos parametros passados por ela. Baseado no parsedBuffer recebido anteriormente
            Linha de Exemplo: ld r0, 86

            instrucao => String da instrução. (Exemplo: ld)
            parametros => String com todos os parametros da instrução. (Exemplo: r0, 86)
            */
            char instrucao[5], parametros[45]; unsigned int instrucaoBinario;
            sscanf(parsedBuffer, "%s %20[^\0]", instrucao, parametros);
            instrucaoBinario = instructionToBinary(instrucao);

            /*
            Separa os parametros de acordo com o formato da instrução.
            Linha de Exemplo: ld r0 86

            r0 => Valor do registrador citado na instrução. (Exemplo: 0)
            mar => Valor da posição da memória citada na instrução. (Exemplo: 86)

            Outros valores:
            r1, r2 => Valores dos registrados citados na instrução.
            imm => Valor do imediato citado na instrução.

            Por fim, acumula tudo em uma única palavra de 32 bits na variavel 'palavraCompleta', conforme o formato da instrução.
            Exemplo: 0b01110000 00000000 00000000 10000110
            */
            unsigned int r0, r1, r2, mar, imm;
            if (instrucaoBinario == 0 || instrucaoBinario == 1);
            else if (instrucaoBinario >= 2 && instrucaoBinario <= 2)
            {
                sscanf(parametros, "r%d", &r0);
                palavraCompleta = (instrucaoBinario << 4) | r0;
                palavraCompleta = palavraCompleta << 23;
            }
            else if (instrucaoBinario >= 3 && instrucaoBinario <= 4)
            {
                sscanf(parametros, "r%d, r%d", &r0, &r1);
                palavraCompleta = (instrucaoBinario << 4) | r0;
                palavraCompleta = (palavraCompleta << 4) | r1;
                palavraCompleta = palavraCompleta << 19;
            }
            else if (instrucaoBinario >= 5 && instrucaoBinario <= 6)
            {
                sscanf(parametros, "r%d, r%d, %x", &r0, &r1, &mar);
                palavraCompleta = (instrucaoBinario << 4) | r0;
                palavraCompleta = (palavraCompleta << 4) | r1;
                palavraCompleta = (palavraCompleta << 19) | mar;
            }
            else if (instrucaoBinario >= 7 && instrucaoBinario <= 13)
            {
                sscanf(parametros, "r%d, r%d, r%d", &r0, &r1, &r2);
                palavraCompleta = (instrucaoBinario << 4) | r0;
                palavraCompleta = (palavraCompleta << 4) | r1;
                palavraCompleta = (palavraCompleta << 4) | r2;
                palavraCompleta = (palavraCompleta << 15);
            }
            else if (instrucaoBinario >= 14 && instrucaoBinario <= 15)
            {
                sscanf(parametros, "r%d, %x", &r0, &mar);
                palavraCompleta = (instrucaoBinario << 4) | r0;
                palavraCompleta = (palavraCompleta << 23) | mar;
            }
            else if (instrucaoBinario >= 16 && instrucaoBinario <= 23)
            {
                sscanf(parametros, "r%d, %x", &r0, &imm);
                palavraCompleta = (instrucaoBinario << 4) | r0;
                palavraCompleta = (palavraCompleta << 23) | imm;
            }
            else if (instrucaoBinario >= 24 && instrucaoBinario <= 30)
            {
                sscanf(parametros, "%x", &mar);
                palavraCompleta = (instrucaoBinario << 4);
                palavraCompleta = (palavraCompleta << 23) | mar;
            }
            else
            {
                printf("FALHA NA LEITURA DO CÓDIGO: INSTRUÇÃO NÃO ENCONTRADA. PRÓXIMO DA LINHA %d.", linhaAtual);
                exit(1);
            }
        }
        else if (tipoOperacao == 'd') // Se estivermos lidando com um dado...
        {
            sscanf(parsedBuffer, "%x", &palavraCompleta);
        }
        else // Se o valor for diferente do esperado...
        {
            printf("FALHA NA LEITURA DO CÓDIGO: TIPO DE OPERAÇÃO NÃO ENCONTRADO. PRÓXIMO DA LINHA %d.", linhaAtual);
            exit(1);
        }

        /*
        Armazena o valor em binário da palavra de 32 bits armazenada na variavel 'palavraCompleta' em "páginas" de 8 bits (char).
            
        Exemplo: 0b00101101 01010111 11111111 11111111
        pag[0] => 0b00101101
        pag[1] => 0b01010111
        pag[2] => 0b11111111
        pag[3] => 0b11111111
        */
        for (int i = 0; i < 4; i++)
            pag[i] = (palavraCompleta >> (24 - (8*i))) & 0b11111111;

        /*
        Adiciona as páginas na memória conforme o valor indicado na linha.
        */
        for (int i = 0; i < 4; i++) memoria[posMemoria + i] = pag[i];

        linhaAtual++;
    }
    fclose(arquivo);
    return 1;
}

void printBinario(int num) {
    printf("0b");
    for(int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
}

void showDashboard(char decOuHex, int visibleRAM) {
    system("cls");
    printf("======================================================================\n");
    printf("| MBR: "); printBinario(mbr); printf("|\n");
    if (decOuHex == 'd') // Parametro 'd' imrprime os valores em decimal.
    {
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
        if (visibleRAM){
            printf("-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-\n");
            printf("| Memória:\n");
            for (int i = 1; i < 155; i++) {
                printf("[%d]: %u ", i - 1, memoria[i-1]);
                if (i % 20 == 0) printf("\n");
            }
            printf("\n-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-\n");
        }
    }
    else if (decOuHex == 'h') // Parametro 'h' imrprime os valores em hexadecimal.
    {
        printf("| IR: %x | MAR: %x |\n", ir, mar);
        printf("| Ro0: %x | Ro1: %x | Ro2: %x | IMM: %x |\n", ro0, ro1, ro2, imm);
        printf("-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-\n");
        printf("| Registradores:\n");
        for (int i = 0; i < 16; i++) {
            printf("| Reg[%d]: %x | ", i, reg[i]);
            if (i % 2 == 1) printf("\n");
        }
        printf("-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-\n");
        printf("| E: %x | L: %x | G: %x |\n", e, l, g);
        printf("| PC: %x |\n", pc);
        if (visibleRAM){
            printf("-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-\n");
            printf("| Memória:\n");
            for (int i = 1; i < 155; i++) {
                printf("[%x]: %x ", i - 1, memoria[i-1]);
                if (i % 20 == 0) printf("\n");
            }
            printf("\n-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-\n");
        }
    }
    else {
        system("cls");
        perror("ERRO: Parametro inválido na função 'showDashboard'.");
        exit(1);
    }
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
    if (ir == 0); // HALT
    else if (ir == 1) // NO OPERATION
    {
        pc += 4;
    }
    else if (ir == 2) // LOGICAL-NOT ON REGISTER
    {
        reg[ro0] = !(reg[ro0]);
        pc += 4;
    }
    else if(ir == 3){ // MOVE REGISTER
        reg[ro0] = reg[ro1];
        pc += 4;
    }
    else if (ir == 4) // COMPARE REGISTER
    {
        if (reg[ro0] == reg[ro1]) e = 1;
        else e = 0;

        if (reg[ro0] < reg[ro1]) l = 1;
        else l = 0;

        if (reg[ro0] > reg[ro1]) g = 1;
        else g = 0;

        pc += 4;
    }
    else if(ir == 5) // LOAD VIA BASE+OFFSET
    {
        reg[ro0] = 0;
        for (int i = 0; i < 4; i++)
            reg[ro0] |= (memoria[mar + ro1 + i] << (i * 8));
        pc += 4;
    }
    else if (ir == 6) // STORE VIA BASE+OFFSET
    {
        for (int i = 0; i < 4; i++)
            memoria[mar + ro1 + i] = (reg[ro0] >> (i * 8)) & 0b11111111;
        pc += 4;
    }
     else if(ir == 7){ // ADD REGISTER
        reg[ro0] = reg[ro1] + reg[ro2];
        pc += 4;
    }
    else if (ir == 8) // SUBTRACT REGISTER
    {
        reg[ro0] = reg[ro1] - reg[ro2];
        pc += 4;
    }
     else if(ir == 9){ // MULTIPLY REGISTER
        reg[ro0] = reg[ro1] * reg[ro2];
        pc += 4;
    }
    else if (ir == 10) // DIVIDE REGISTER
    {
        reg[ro0] = reg[ro1] / reg[ro2];
        pc += 4;
    }
    else if(ir == 11){ // LOGICAL-AND ON REGISTER
        reg[ro0] = reg[ro1] & reg[ro2];
        pc += 4;
    }
    else if (ir == 12) // LOGICAL-OR ON REGISTER
    {
        reg[ro0] = reg[ro1] | reg[ro2];
        pc += 4;
    }
    else if(ir == 13) // LOGICAL-XOR ON REGISTER
    {
        reg[ro0] = reg[ro1] ^ reg[ro2];
        pc += 4;
    }
    else if(ir == 14) // LOAD
    {
        for (int i = 0; i < 4; i++)
            reg[ro0] = (reg[ro0] << 8) | memoria[mar + i];
        pc += 4;
    }
    else if(ir == 15) // STORE
    {
        for (int i = 0; i < 4; i++)
            memoria[mar + i] = (reg[ro0] << (24 - (8*i))) & 0b11111111;
        pc += 4;
    }
    else if(ir == 16) // MOVE IMMEDIATE TO THE LOWER HALF OF THE REGISTER
    {
        reg[ro0] = (imm & 0b1111111111111111);
    }
    else if(ir == 17) // MOVE IMMEDIATE TO THE HIGHER HALF OF THE REGISTER
    {
        reg[ro0] = ((imm & 0b1111111111111111) << 16) | (reg[ro0] & 0b1111111111111111);
        pc += 4;
    }
    else if(ir == 18) // ADD IMMEDIATE
    {
        reg[ro0] = reg[ro0] + imm;
        pc += 4;
    }
    else if(ir == 19) // SUBTRACT IMMEDIATE
    {
        reg[ro0] = reg[ro0] - imm;
        pc += 4;
    }
    else if(ir == 20) // MULTIPLY IMMEDIATE
    {
        reg[ro0] = reg[ro0] * imm;
        pc += 4;
    }
    else if(ir == 21) // DIVIDE IMMEDIATE
    {
        reg[ro0] = reg[ro0] / imm;
        pc += 4;
    }
    else if(ir == 22) // LEFT SHIFT
    {
        reg[ro0] = reg[ro0] << imm;
        pc += 4;
    }
    else if(ir == 23) // RIGHT SHIFT
    {
        reg[ro0] = reg[ro0] >> imm;
        pc += 4;
    }
    else if(ir == 24) // JUMP IF EQUAL TO
    {
        if(e == 1)
            pc = mar;
        else
            pc += 4;
    }
    else if(ir == 25) // JUMP IF NOT EQUAL TO
    {
        if(e == 0)
            pc = mar;
         else
            pc += 4;
    }
    else if(ir == 26) // JUMP IF LOWER THA
    {
        if(l == 1)
            pc = mar;
         else
            pc += 4;
    }
    else if(ir == 27) // JUMP IF LOWER THAN OR EQUAL TO
    {
        if(e == 1 || l == 1)
            pc = mar;
         else
            pc += 4;
    }
    else if(ir == 28) // JUMP IF GREATER THAN
    {
        if(g == 1)
            pc = mar;
         else
            pc += 4;
    }
    else if(ir == 29) // JUMP IF GREATER THAN OR EQUAL TO
    {
        if(e == 1 || g == 1)
            pc = mar;
         else
            pc += 4;
    }
    else if(ir == 30) // JUMP
    {
        pc = mar;
    }
}
