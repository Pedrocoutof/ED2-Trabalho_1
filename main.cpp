#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <conio.h>

using namespace std;

#define NUM_MAX_DE_REGISTRO 400 // TAMANHO DE LINHAS DO ARQUIVO '.csv' QUE USAMOS PARA TESTES //

// ESTRUTURA USADA PARA ARMAZENAR DE FORMA SEPARADA CADA INFORMAÇÃO DO ARQUIVO '.csv' //
typedef struct{

    char review_id [90];        //  string identificadora única
    char review_text[550];      //  o review postado pelo usuário
    char upvotes[5];            //  número de votos favoráveis ao review
    char app_version[10];       //  string indicando a versão do aplicativo a que o review se refere
    char posted_date[20];       //  data e hora de postagem

}TiktokReviews;

// FUNCAO USADA PARA GERAR E RETORNAR VALOR ALEATORIO DE FORMA MAIS PRÁTICA E LEGÍVEL //
int valorAleatorio(){
    return rand() % NUM_MAX_DE_REGISTRO;
}

// FUNCAO USADA PARA IMPRIMIR NO CONSOLE CONTEÚDO DE UM REGISTRO GUARDADO NA ESTRUTURA 'TiktokReviews' //
void printaConteudoTiktokReviews(TiktokReviews tiktokReviews){

    cout <<     "Review ID : "       <<      tiktokReviews.review_id      << endl;
    cout <<     "Review Text : "     <<      tiktokReviews.review_text    << endl;
    cout <<     "Up Votes : "        <<      tiktokReviews.upvotes        << endl;
    cout <<     "App Version : "     <<      tiktokReviews.app_version    << endl;
    cout <<     "Posted Date : "     <<      tiktokReviews.posted_date    << endl << endl;

}

// FUNÇÃO USADA PARA LER TODO ARQUIVO '.csv' E GERAR UM ARQUIVO '.bin' //
void gravaCsvEmBinario(FILE * arq_csv, FILE * arq_bin){

    rewind(arq_csv);    // REINICIA O PONTEIRO DO '.csv' //
    rewind(arq_bin);    // REINICIA O PONTEIRO DO '.bin' //
    char c;

    for(c = fgetc(arq_csv); c != '\n'; c = fgetc(arq_csv)){}    // PULA A PRIMEIRA LINHA DO '.csv' JÁ QUE A PRIMEIRA LINHA TEM INFORMAÇÕES DE COMO O ARQUIVO É SEPARADO //
    while(!feof(arq_csv)){

        TiktokReviews tiktokReviews;

        fscanf(arq_csv, "%[^,]", tiktokReviews.review_id);  // FAZ A LEITURA ATÉ A VIRGULA E GUARDA NA VARIAVEL 'tiktokReviews.review_id' //
        fgetc(arq_csv);

        c = fgetc(arq_csv);
        if(c == '"'){       // VERIFICA SE O REVIEW_TEXT DO ARQUIVO '.csv' É SEPARADO POR ' " ' //
                int i = 0;
                c = fgetc(arq_csv);
            while(c!='"'){
                tiktokReviews.review_text[i] = c;   // SE FOR ELE IRÁ GUARDAR ATÉ A PRÓXIMA ' " ' //
                c = fgetc(arq_csv);
                i++;
            }
            tiktokReviews.review_text[i] = '\0';
            c = fgetc(arq_csv);
        }
        else{
                fseek(arq_csv, -1, SEEK_CUR);
                int i = 0;
                c = fgetc(arq_csv);
                while(c != ','){    // CASO CONTRÁRIO ELE GUARDA ATÉ A PRÓXIMA ',' //
                    tiktokReviews.review_text[i] = c;
                    c = fgetc(arq_csv);
                    i++;
                }
            tiktokReviews.review_text[i] = '\0';
        }

        fscanf(arq_csv, "%[^,]", tiktokReviews.upvotes);    // GUARDA UPVOTES EM 'tiktokReviews.upvotes' ATÉ A PRÓXIMA VIRGULA //
        fgetc(arq_csv);

        fscanf(arq_csv, "%[^,]", tiktokReviews.app_version);    // GUARDA APP_VERSION NA VARIAVEL 'tiktokReviews.app_version' ATE A PRÓXIMA VIRGULA //
        fgetc(arq_csv);

        int i = 0;
        for(c = fgetc(arq_csv); c != '\n' && c!=EOF; c = fgetc(arq_csv)){   // GUARDA O RESTO DO CONTEUDO VERIFICANDO SE CHEGOU NO FINAL DA LINHA '\n' OU SE CHEGOU NO FINAL DO ARQUIVO 'EOF' E GUARDA NA VARIAVEL 'tiktokReviews.posted_date' //
            tiktokReviews.posted_date[i] = c;
            i++;
        }
        tiktokReviews.posted_date[i] = '\0';

        //printaConteudoTiktokReviews(tiktokReviews);

        fwrite(tiktokReviews.review_id,     sizeof(tiktokReviews.review_id),    1, arq_bin);    // GRAVA EM BINARIO O 'review_id'   // 
        fwrite(tiktokReviews.review_text,   sizeof(tiktokReviews.review_text),  1, arq_bin);    // GRAVA EM BINARIO O 'review_text' //
        fwrite(&tiktokReviews.upvotes,      sizeof(tiktokReviews.upvotes),      1, arq_bin);    // GRAVA EM BINARIO O 'upvotes'     //
        fwrite(tiktokReviews.app_version,   sizeof(tiktokReviews.app_version),  1, arq_bin);    // GRAVA EM BINÁRIO O 'app_version' //
        fwrite(tiktokReviews.posted_date,   sizeof(tiktokReviews.posted_date),  1, arq_bin);    // GRAVA EM BINÁRIO O 'posted_date' //
    }
}

// FUNÇÃO USADA PARA ACESSAR REGISTRO E IMPRIMIR NO CONSOLE O VALOR DO REGISTRO PASSADO POR PARAMETRO //
void acessaRegistro(int i){

    FILE * arq_bin = fopen("tiktok_app_reviews.bin", "rb");

    if(arq_bin == NULL){
        cout << "Erro! Arquivo não encontrado no diretorio : " << arq_bin;
        exit(1);
    }

    TiktokReviews tiktokReviews;

    fseek(arq_bin, i * sizeof(TiktokReviews), 1);

    fread(&tiktokReviews.review_id, sizeof(tiktokReviews.review_id), 1, arq_bin);
    fread(&tiktokReviews.review_text, sizeof(tiktokReviews.review_text), 1, arq_bin);
    fread(&tiktokReviews.upvotes, sizeof(tiktokReviews.upvotes), 1, arq_bin);
    fread(&tiktokReviews.app_version, sizeof(tiktokReviews.app_version), 1, arq_bin);
    fread(&tiktokReviews.posted_date, sizeof(tiktokReviews.posted_date), 1, arq_bin);

    for(int i = 0; i < 14; i ++){
        printf("%c", 461);
    }
    cout << "  ACESSANDO REGISTRO : " << i << "  ";

    for(int i = 0; i < 14; i ++){
        printf("%c", 461);
    }
    cout << endl;
    printaConteudoTiktokReviews(tiktokReviews);
    fclose(arq_bin);
}

// FUNCAO SEMELHANTE A 'acessaRegistro' MAS QUE AO INVÉS DE PRINTAR NO CONSOLE ELA RETORANAR O VALOR DO REGISTRO PASSADO POR PARAMETRO //
TiktokReviews retornaRegistro(int i){

    FILE * arq_bin = fopen("tiktok_app_reviews.bin", "rb");

    if(arq_bin == NULL){
        cout << "Erro! Arquivo não encontrado no diretorio : " << arq_bin;
        exit(1);
    }

    TiktokReviews tiktokReviews;

    fseek(arq_bin, i * sizeof(TiktokReviews), 1);

    fread(&tiktokReviews.review_id, sizeof(tiktokReviews.review_id), 1, arq_bin);
    fread(&tiktokReviews.review_text, sizeof(tiktokReviews.review_text), 1, arq_bin);
    fread(&tiktokReviews.upvotes, sizeof(tiktokReviews.upvotes), 1, arq_bin);
    fread(&tiktokReviews.app_version, sizeof(tiktokReviews.app_version), 1, arq_bin);
    fread(&tiktokReviews.posted_date, sizeof(tiktokReviews.posted_date), 1, arq_bin);


    fclose(arq_bin);

    return tiktokReviews;
}

// FUNCAO UTILIZADA PARA GRAVAR EM UM ARQUIVO DE TEXTO, TODAS VARIAVEIS DO TIPO 'TiktokReviews' //
void gravaTiktokReviewEmTxt(FILE * arq_txt, TiktokReviews tiktokReviews){

            fprintf(arq_txt, "%s", tiktokReviews.review_id);
            fprintf(arq_txt, "%c", 44);
            fprintf(arq_txt, "%s", tiktokReviews.review_text);
            fprintf(arq_txt, "%c", 44);
            fprintf(arq_txt, "%s", tiktokReviews.upvotes);
            fprintf(arq_txt, "%c", 44);
            fprintf(arq_txt, "%s", tiktokReviews.app_version);
            fprintf(arq_txt, "%c", 44);
            fprintf(arq_txt, "%s", tiktokReviews.posted_date);
            fprintf(arq_txt, "%c", '\n');
}

// FUNCAO QUE GERA NO CONSOLE UM MENU PARA INTERAÇÃO COM O USUARIO PARA TESTAR A IMPORTAÇÃO //
void menuTesteImportacao(){
printf("%c", 201);
    for(int i = 0; i < 55; i ++){
        printf("%c", 461);
    }
    printf("%c\n", 443);
    printf("%c", 186);
    printf("\t\t\t\t\t\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t\tVoce Deseja:\t\t\t");
    printf("%c", 186);
    printf("\n%c", 186);
    printf("\t\t\t            \t\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t1 - Imprimir no Console\t\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t2 - Guardar no arquivo texto\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t\t\t\t\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t\t\t\t\t\t");
    printf("%c\n", 186);
    printf("%c", 200);

    for(int i = 0; i < 55; i ++){
        printf("%c", 461);
    }
    printf("%c\n", 188);

}

// FUNCAO PARA TESTAR A IMPORTACAO //
void testaImportacao(){

    int resp;
    scanf("%d", &resp);
    
        srand(time(nullptr));

    if(resp == 1){
        for(int i = 0; i < 10; i++){
                acessaRegistro(valorAleatorio());
        }
    }

    else if(resp == 2){
        FILE * arq_txt = fopen("registrosAleatorios.txt", "w");
        for(int i = 0; i < 100; i++){
            TiktokReviews tiktokReviews = retornaRegistro(valorAleatorio());
            gravaTiktokReviewEmTxt(arq_txt, tiktokReviews);
        }
    }


}

// FUNCAO QUE GERA NO CONSOLE UM MENU PARA INTERAÇÃO COM O USUARIO PARA TESTAR A FUNÇÃO 'acessaRegistro' //
int menuAcessaRegistro(){

    printf("%c", 201);
    for(int i = 0; i < 55; i ++){
        printf("%c", 461);
    }
    printf("%c\n", 443);
    printf("%c\t\t\t\t\t\t\t%c\n", 186, 186);
    printf("%c", 186);
    printf("    Digite o Valor do registro que deseja acessar:     ");
    printf("%c\n", 186);
    printf("%c\t\t\t\t\t\t\t%c\n", 186, 186);
    printf("%c", 200);

    for(int i = 0; i < 55; i ++){
        printf("%c", 461);
    }
    printf("%c\n", 188);

    int resp;
    scanf("%d", &resp);
    return resp;

}

int main(int argc, char * argv[])
{
    // char diretorio_csv[] = "H:\\UFJF\\2021.3\\ED2\\Trabalho 1\\tiktok_app_reviews_reduzido.csv";
    // FILE * arq_csv = fopen(diretorio_csv, "r");
    // H:\UFJF\2021.3\ED2\Trabalho 1\tiktok_app_reviews_reduzido.csv

    FILE * arq_csv = fopen(argv[1], "r");

    if(arq_csv == NULL){
        cout << "Erro! Arquivo <.csv> nao encontrado no diretorio";
        return 0;
    }

    char diretorio_bin[] = "tiktok_app_reviews.bin";
    FILE * arq_bin = fopen(diretorio_bin, "rb");

    if(arq_bin == NULL){
        cout << endl << "Arquivo <" << diretorio_bin << "> nao encontrado." << endl;
        cout << "Gerando <" << diretorio_bin << ">." << endl;
        FILE * arq_bin = fopen(diretorio_bin, "wb");
        gravaCsvEmBinario(arq_csv, arq_bin);
        fclose(arq_bin);
    }

    int resp = menuAcessaRegistro();

    acessaRegistro(resp);

    menuTesteImportacao();

    testaImportacao();

    cout << endl << endl << endl << "Pressione qualquer tecla para sair...";
    getch();
    return 0;
}