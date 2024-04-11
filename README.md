# CAssembly-Interpreter (Simulador de CPU)

Este é um simulador de uma Unidade de Processamento Central (CPU) básica desenvolvida em linguagem C. A CPU simulada é capaz de executar uma série de instruções básicas e manipular uma memória simulada.

## Utilização

1. Crie um arquivo de texto na pasta 'codeFiles' com o conjunto de instruções que você deseja simular.
**Cada instrução deve estar no formato:**
```endereço;instrução/dado;palavra de instrução/dado```

**Exemplo:**  
.\codeFiles\operacao1.txt
```
0;i;ld r0, 86
4;i;ld r1, 8a
8;i;ld r2, 8e
c;i;ld r3, 92
10;i;ld r4, 96
14;i;sub r5, r3, r4
18;i;div r6, r2, r5
1c;i;mul r7, r6, r1
20;i;add r8, r0, r7
24;i;st r8, 82
28;i;hlt
86;d;20
8a;d;3
8e;d;4
92;d;5
96;d;3
```

_NOTA: Todo programa deve começar na posição de memoria 0 e seguir sequêncialmente. As instruções devem vir primeiro que os dados. As posições de memória devem ser colocadas em Hexadecimal. Os valores dos registradores e os valores de dados devem ser colocados em decimal._

2. Compile e execute o código em 'main.c'.

3. Digite o nome do arquivo criado no terminal onde o código está sendo executado.  
**Exemplo:**  
```
operacao1.txt
```

### Conjunto de Instruções possiveis

|      Mneônico     |                     Instrução                     | Descrição                                                                                                                         |
|-------------------|---------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| hlt               | Halt                                              | Não faz nada. O registrador PC também não é incrementado. Deve ser colocado no fim de todo programa.                              |
| nop               | No Operation                                      | Não faz nada. O registrador PC ainda é incrementado.                                                                              |
| not rX            | Logical-Not On Register                           | rX = !rX                                                                                                                          |
| movr rX, rY       | Move Register                                     | rX = rY                                                                                                                           |
| cmp rX, rY        | Compare Register                                  | 1. Se rX = rY, então E = 1; senão E = 0 <br> 2. Se rX < rY, então L = 1; senão L = 0 <br> 3. Se rX > rY, então G = 1; senão G = 0 |
| ldbo rX, rY, M[Z] | Load Via Base + Offset                            | rX = M[Z + rY]                                                                                                                    |
| stbo rX, rY, M[Z] | Store Via Base + Offset                           | M[Z + rY] = rX                                                                                                                    |        
| add rX, rY, rZ    | Add Register                                      | rX = rY + rZ                                                                                                                      |
| sub rX, rY, rZ    | Substract Register                                | rX = rY - rZ                                                                                                                      |
| mul rX, rY, rZ    | Multiply Register                                 | rX = rY * rZ                                                                                                                      |
| div rX, rY, rZ    | Divide Register                                   | rX = rY / rZ                                                                                                                      |
| and rX, rY, rZ    | Logical-And On Register                           | rX = rY & rZ                                                                                                                      |
| or rX, rY, rZ     | Logical-Or On Register                            | rX = rY | rZ                                                                                                                      |
| xor rX, rY, rZ    | Logical-Xor On Register                           | rX = rY ^ rZ                                                                                                                      |
| ld rX, M[Y]       | Load                                              | rX = M[Y]                                                                                                                         |
| st rX, M[Y]       | Store                                             | M[Y] = rX                                                                                                                         |
| movil rX, imm     | Move Immediate To The Lower Half Of The Register  | Zera rX e move os 16 Bits da direita do Imediato para os 16 Bits da direita do rX.                                                |
| movih rX, imm     | Move Immediate To The Higher Half Of The Register | Move os 16 Bits da direita do Imediato para os 16 Bits da esquerda do rX, mantendo os 16 Bits da direita de rX.                   |
| addi rX, imm      | Add Immediate                                     | rX = rX + imm                                                                                                                     |
| subi rX, imm      | Substract Immediate                               | rX = rX - imm                                                                                                                     |
| muli rX, imm      | Multiply Immediate                                | rX = rX * imm                                                                                                                     |
| divi rX, imm      | Divide Immediate                                  | rX = rX / imm                                                                                                                     |
| lsh rX, imm       | Left Shift                                        | rX = rX << imm                                                                                                                    |
| rsh rX, imm       | Right Shift                                       | rX = rX >> imm                                                                                                                    |
| je M[X]           | Jump If Equal To                                  | Se E = 1, então PC = X                                                                                                            |
| jne M[X]          | Jump If Not Equal To                              | Se E != 1, então PC = X                                                                                                           |
| jl M[X]           | Jump If Lower Than                                | Se L = 1, então PC = X                                                                                                            |
| jle M[X]          | Jump If Lower Than Or Equal To                    | Se L = 1 && E = 1, então PC = X                                                                                                   |
| jg M[X]           | Jump If Greater Than                              | Se G = 1, então PC = X                                                                                                            |
| jge M[X]          | Jump If Greater Than Or Equal To                  | Se G = 1 && E = 1, então PC = X                                                                                                   |
| jmp M[X]          | Jump                                              | PC = X                                                                                                                            |

### Alterando o Código
A função *'showDashboard'* tem dois parâmetros de configuração:

**decOuHex:** Você pode alterar entre 'h' ou 'd' para mudar a impressão dos valores para hexadecimal ou decimal, respectivamente.
**visibleRAM:** Você pode alterar entre 0 ou 1 para definir se o programa irá imprimir a memória ou não; 0 => Falso, 1 => Verdadeiro.

Você pode alterar estes parâmetros das constantes definidas no início do código em 'main.c'.

## Autores

### Docentes:
Este projeto é um trabalho de implementação da diciplina de Arquitetura e Organização de Computadores I para a turma de 2024/1.  
Instituto Federal de Educação, Ciência e Tecnologia de Goiás - Campus Anápolis

Plano de projeto concebido pelo docente [Hugo Vinícius Leão e Silva](https://www.escavador.com/sobre/5606654/hugo-vinicius-leao-e-silva).

### Dicentes:
- [Caio César Sifuentes Barcelos](https://github.com/CaioSifuentes)
- [Lucas Henrique Campos Carvalho](https://github.com/LucasPastel2004)


