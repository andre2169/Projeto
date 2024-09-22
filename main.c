#include <stdio.h>
#include <stdlib.h> // Necessário para usar system( )
#include <string.h>
#include <unistd.h> // Para o uso de sleep()

#define MAX_LIVROS 10  // Número máximo de livros na biblioteca
#define DISPONIVEL 0   // Status de livro disponível
#define EMPRESTADO 1   // Status de livro emprestado

// Estrutura para armazenar informações de um livro
typedef struct {
    char titulo[100];  // Título do livro
    char autor[100];   // Autor do livro
    int ano;           // Ano de publicação
    char ISBN[14];     // Código ISBN do livro
    int id;            // ID do livro
    int emprestado;    // 0 = disponível, 1 = emprestado
} Livro;

Livro biblioteca[MAX_LIVROS];  // Array para armazenar os livros
int quantidade_livros = 0;      // Contador de livros cadastrados
int contador_ISBN = 1000;       // Contador para gerar ISBN único

// Função para exibir a tela de boas-vindas
void telaDeBoasVindas() {
    printf("========================================\n");
    printf("!! BEM-VINDO AO SISTEMA DE BIBLIOTECA !! \n");
    printf("========================================\n");
    printf("    Desenvolvido Para uso Academico\n");
    printf("V. 1.0\n");
    printf("========================================\n\n");
}

// Função de carregamento do sistema
void carregarSistema() {
    printf("Carregando o sistema");
    for (int i = 0; i < 3; i++) {
        sleep(1);  // Aguarda 1 segundo
        printf(".");
        fflush(stdout);  // Garante que os pontos sejam exibidos imediatamente
    }
    printf("\nCarregado com Sucesso!\n");
}

// Função para gerar um ISBN único
void gerarISBN(char* isbn) {
    sprintf(isbn, "%04d", contador_ISBN++);  // Gera um ISBN sequencial
}

// Função para verificar duplicidade de títulos
int verificarDuplicidade(char titulo[100]) {
    for (int i = 0; i < quantidade_livros; i++) {
        if (strcmp(biblioteca[i].titulo, titulo) == 0) {
            return 1;  // Título já existe
        }
    }
    return 0;  // Título não existe
}

// Função para adicionar um novo livro à biblioteca
void adicionarLivro() {
    // Verifica se a biblioteca está cheia
    if (quantidade_livros >= MAX_LIVROS) {
        printf("A biblioteca esta cheia. Nao is possivel adicionar mais livros.\n");
        return;
    }

    Livro novoLivro;  // Cria uma nova instância de livro

    // Entrada do título
    printf("Digite o titulo: ");
    fgets(novoLivro.titulo, sizeof(novoLivro.titulo), stdin);
    novoLivro.titulo[strcspn(novoLivro.titulo, "\n")] = '\0';  // Remove o '\n'

    // Verifica se o título já existe
    if (verificarDuplicidade(novoLivro.titulo)) {
        printf("Erro: Ja existe um livro com esse titulo!\n");
        return;
    }

    // Entrada do autor
    printf("Digite o autor: ");
    fgets(novoLivro.autor, sizeof(novoLivro.autor), stdin);
    novoLivro.autor[strcspn(novoLivro.autor, "\n")] = '\0';  // Remove o '\n'

    // Entrada do ano de publicação
    printf("Digite o ano de publicacao: ");
    while (scanf("%d", &novoLivro.ano) != 1 || novoLivro.ano < 0) {
        printf("Ano invalido. Tente novamente: ");
        while (getchar() != '\n');  // Limpa o buffer
    }
    getchar();  // Consome o '\n' deixado pelo scanf

    // Geração automática do ISBN
    gerarISBN(novoLivro.ISBN);
    novoLivro.id = quantidade_livros + 1;  // O ID será o próximo na lista
    novoLivro.emprestado = DISPONIVEL;  // Inicialmente, o livro está disponível

    // Adiciona o novo livro ao array
    biblioteca[quantidade_livros] = novoLivro;
    quantidade_livros++;  // Incrementa o contador de livros

    printf("Livro adicionado com sucesso! ID: %d, ISBN: %s\n", novoLivro.id, novoLivro.ISBN);
}

// Função que lista todos os livros da biblioteca
void listarLivros() {
    // Verifica se há livros cadastrados
    if (quantidade_livros == 0) {
        printf("Nenhum livro cadastrado.\n");
    } else {
        printf("\nListando todos os livros:\n");
        for (int i = 0; i < quantidade_livros; i++) {
            printf("\nLivro encontrado:\n");
            printf("Titulo: %s\n", biblioteca[i].titulo);
            printf("Autor: %s\n", biblioteca[i].autor);
            printf("Ano: %d\n", biblioteca[i].ano);
            printf("ISBN: %s\n", biblioteca[i].ISBN);
            printf("Status: %s\n", biblioteca[i].emprestado ? "EMPRESTADO" : "DISPONIVEL");
        }
    }
}

// Função para buscar um livro pelo título
void buscarLivro() {
    char titulo_busca[100];
    printf("Digite o titulo do livro para buscar: ");
    fgets(titulo_busca, sizeof(titulo_busca), stdin);
    titulo_busca[strcspn(titulo_busca, "\n")] = '\0';

    // Percorre a biblioteca para encontrar o livro
    for (int i = 0; i < quantidade_livros; i++) {
        if (strcmp(biblioteca[i].titulo, titulo_busca) == 0) {
            printf("\nLivro encontrado:\n");
            printf("Titulo: %s\n", biblioteca[i].titulo);
            printf("Autor: %s\n", biblioteca[i].autor);
            printf("Ano: %d\n", biblioteca[i].ano);
            printf("ISBN: %s\n", biblioteca[i].ISBN);
            printf("Status: %s\n", biblioteca[i].emprestado ? "EMPRESTADO" : "DISPONIVEL");
            return;  // Encerra a busca após encontrar o livro
        }
    }
    printf("Livro nao encontrado.\n");
}

// Função para excluir um livro pelo título ou ISBN
void excluirLivro() {
    int opcao;
    char entrada[100];

    // Pergunta ao usuário se ele deseja exclusão por título ou ISBN
    printf("Escolha o metodo de exclusao:\n");
    printf("1. Excluir pelo Título\n");
    printf("2. Excluir pelo ISBN\n");
    printf("Opção: ");
    scanf("%d", &opcao);
    getchar();  // Consome o '\n' deixado pelo scanf

    // Se o usuário escolher excluir pelo título
    if (opcao == 1) {
        printf("Digite o titulo do livro para excluir: ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';  // Remove o '\n'

        for (int i = 0; i < quantidade_livros; i++) {
            if (strcmp(biblioteca[i].titulo, entrada) == 0) {
                // Move os livros subsequentes para a posição anterior
                for (int j = i; j < quantidade_livros - 1; j++) {
                    biblioteca[j] = biblioteca[j + 1];
                }
                quantidade_livros--;  // Decrementa a contagem de livros
                printf("Livro com titulo '%s' excluido com sucesso!\n", entrada);
                return;
            }
        }
        printf("Livro com titulo '%s' nao encontrado.\n", entrada);
    }
    // Se o usuário escolher excluir pelo ISBN
    else if (opcao == 2) {
        printf("Digite o ISBN do livro para excluir: ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';  // Remove o '\n'

        for (int i = 0; i < quantidade_livros; i++) {
            if (strcmp(biblioteca[i].ISBN, entrada) == 0) {
                // Move os livros subsequentes para a posição anterior
                for (int j = i; j < quantidade_livros - 1; j++) {
                    biblioteca[j] = biblioteca[j + 1];
                }
                quantidade_livros--;  // Decrementa a contagem de livros
                printf("Livro com ISBN '%s' excluido com sucesso!\n", entrada);
                return;
            }
        }
        printf("Livro com ISBN '%s' nao encontrado.\n", entrada);
    }
    // Se o usuário inserir uma opção inválida
    else {
        printf("Opcao invalida. Tente novamente.\n");
    }
}

// Função para emprestar um livro
void emprestarLivro() {
    char titulo_emprestimo[100];
    printf("Digite o titulo do livro para emprestar: ");
    fgets(titulo_emprestimo, sizeof(titulo_emprestimo), stdin);
    titulo_emprestimo[strcspn(titulo_emprestimo, "\n")] = '\0';

    // Percorre a biblioteca para encontrar o livro
    for (int i = 0; i < quantidade_livros; i++) {
        if (strcmp(biblioteca[i].titulo, titulo_emprestimo) == 0) {
            if (biblioteca[i].emprestado == DISPONIVEL) {
                biblioteca[i].emprestado = EMPRESTADO;  // Altera o status para emprestado
                printf("Livro '%s' emprestado com sucesso!\n", biblioteca[i].titulo);
            } else {
                printf("Livro '%s' ja esta emprestado.\n", biblioteca[i].titulo);
            }
            return;
        }
    }
    printf("Livro nao encontrado.\n");
}

// Função para devolver um livro
void devolverLivro() {
    char titulo_devolucao[100];
    printf("Digite o titulo do livro para devolver: ");
    fgets(titulo_devolucao, sizeof(titulo_devolucao), stdin);
    titulo_devolucao[strcspn(titulo_devolucao, "\n")] = '\0';

    // Percorre a biblioteca para encontrar o livro
    for (int i = 0; i < quantidade_livros; i++) {
        if (strcmp(biblioteca[i].titulo, titulo_devolucao) == 0) {
            if (biblioteca[i].emprestado == EMPRESTADO) {
                biblioteca[i].emprestado = DISPONIVEL;  // Altera o status para disponível
                printf("Livro '%s' devolvido com sucesso!\n", biblioteca[i].titulo);
            } else {
                printf("Livro '%s' ja esta disponivel.\n", biblioteca[i].titulo);
            }
            return;
        }
    }
    printf("Livro nao encontrado.\n");
}

// Função principal que executa o sistema
int main() {
    telaDeBoasVindas();  // Exibe a tela de boas-vindas
    carregarSistema();   // Simula o carregamento do sistema

    int opcao;
    do {
        printf("\n------  MENU PRINCIPAL ----\n");
        printf("|(1)  < Adicionar livro > |\n");
        printf("|(2)  < Listar livros   > |\n");
        printf("|(3)  < Buscar livro    > |\n");
        printf("|(4)  < Excluir livro   > |\n");
        printf("|(5)  < Emprestar livro > |\n");
        printf("|(6)  < Devolver livro  > |\n");
        printf("|(0)       < Sair >       |\n");
        printf("   Escolha uma opcao: \n");
        scanf("%d", &opcao);
        getchar();  // Consome o '\n' deixado pelo scanf

        // Executa a opção escolhida
        switch (opcao) {
            case 1:
                adicionarLivro();
                break;
            case 2:
                listarLivros();
                break;
            case 3:
                buscarLivro();
                break;
            case 4:
                excluirLivro();
                break;
            case 5:
                emprestarLivro();
                break;
            case 6:
                devolverLivro();
                break;
            case 0:
                printf("SAINDO DO SISTEMA...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);  // Continua até que o usuário escolha sair

    return 0;  // Finaliza o programa
}

printf("OI...\n");