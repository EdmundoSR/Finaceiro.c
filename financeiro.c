#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define ARQUIVO "lancamentos.txt"

typedef struct {
    char tipo; // R = Receita | D = Despesa
    char descricao[50];
    float valor;
} Lancamento;

Lancamento lancamentos[MAX];
int totalLanc = 0;
float saldo = 0.0;

// Lê os dados do arquivo ao iniciar
void carregarLancamentos() {
    FILE *fp = fopen(ARQUIVO, "r");
    if (fp == NULL) return; // arquivo ainda não existe

    while (fscanf(fp, "%c;%49[^;];%f\n", &lancamentos[totalLanc].tipo, lancamentos[totalLanc].descricao, &lancamentos[totalLanc].valor) == 3) {
        if (lancamentos[totalLanc].tipo == 'R') {
            saldo += lancamentos[totalLanc].valor;
        } else {
            saldo -= lancamentos[totalLanc].valor;
        }
        totalLanc++;
    }
    fclose(fp);
}

// Grava os dados no arquivo
void salvarLancamento(Lancamento l) {
    FILE *fp = fopen(ARQUIVO, "a");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    fprintf(fp, "%c;%s;%.2f\n", l.tipo, l.descricao, l.valor);
    fclose(fp);
}

int main() {
    int opcao;

    carregarLancamentos();

    do {
        printf("\n--- MENU FINANCEIRO ---\n");
        printf("1. Adicionar receita\n");
        printf("2. Adicionar despesa\n");
        printf("3. Ver saldo atual\n");
        printf("4. Mostrar todos os lançamentos\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpa o ENTER do buffer

        Lancamento l;

        switch (opcao) {
            case 1:
            case 2:
                if (totalLanc >= MAX) {
                    printf("Limite de lançamentos atingido!\n");
                    break;
                }
                l.tipo = (opcao == 1) ? 'R' : 'D';
                printf("Descrição: ");
                fgets(l.descricao, 50, stdin);
                strtok(l.descricao, "\n");
                printf("Valor: R$ ");
                scanf("%f", &l.valor);
                getchar(); // limpa o ENTER

                lancamentos[totalLanc++] = l;
                saldo += (l.tipo == 'R') ? l.valor : -l.valor;
                salvarLancamento(l);
                printf("%s adicionada com sucesso!\n", (l.tipo == 'R') ? "Receita" : "Despesa");
                break;

            case 3:
                printf("Saldo atual: R$ %.2f\n", saldo);
                printf("Situação: %s\n", (saldo >= 0) ? "POSITIVO" : "NEGATIVO");
                break;

            case 4:
                printf("\n--- Lançamentos ---\n");
                for (int i = 0; i < totalLanc; i++) {
                    printf("%d. [%s] R$ %.2f - %s\n", i + 1,
                        (lancamentos[i].tipo == 'R') ? "RECEITA" : "DESPESA",
                        lancamentos[i].valor,
                        lancamentos[i].descricao);
                }
                break;

            case 5:
                printf("Saindo do sistema. Até logo!\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 5);

    return 0;
}
