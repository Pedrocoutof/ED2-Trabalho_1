#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <conio.h>

using namespace std;

#define NUM_MAX_DE_REGISTRO 400

typedef struct{

    char review_id [90];        //  string identificadora única
    char review_text[550];      //  o review postado pelo usuário
    char upvotes[5];            //  número de votos favoráveis ao review
    char app_version[10];       //  string indicando a versão do aplicativo a que o review se refere
    char posted_date[20];       //  data e hora de postagem

}TiktokReviews;

int valorAleatorio(){
    return rand() % NUM_MAX_DE_REGISTRO;
}

void printaArquivoTxt(FILE * arq){
    for(char c = fgetc(arq); !feof(arq); c = fgetc(arq)){
        printf("%c", c);
        if(c == '\n'){
            cout << endl;
        }
    }
    rewind(arq);
}

void printaConteudoTiktokReviews(TiktokReviews tiktokReviews){

    cout <<     "Review ID : "       <<      tiktokReviews.review_id      << endl;
    cout <<     "Review Text : "     <<      tiktokReviews.review_text    << endl;
    cout <<     "Up Votes : "        <<      tiktokReviews.upvotes        << endl;
    cout <<     "App Version : "     <<      tiktokReviews.app_version    << endl;
    cout <<     "Posted Date : "     <<      tiktokReviews.posted_date    << endl << endl;

}

void gravaTiktokReviewEmBinario(FILE * arq_csv, FILE * arq_bin){

    rewind(arq_csv);
    rewind(arq_bin);
    char c;

    // PULA A PRIMEIRA LINHA DO "arq_csv" //
    for(c = fgetc(arq_csv); c != '\n'; c = fgetc(arq_csv)){}

    while(!feof(arq_csv)){

        TiktokReviews tiktokReviews;

        fscanf(arq_csv, "%[^,]", tiktokReviews.review_id);
        fgetc(arq_csv);

        c = fgetc(arq_csv);
        if(c == '"'){
                int i = 0;
                c = fgetc(arq_csv);
            while(c!='"'){
                tiktokReviews.review_text[i] = c;
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
                while(c != ','){
                    tiktokReviews.review_text[i] = c;
                    c = fgetc(arq_csv);
                    i++;
                }
            tiktokReviews.review_text[i] = '\0';
        }

        fscanf(arq_csv, "%[^,]", tiktokReviews.upvotes);
        fgetc(arq_csv);

        fscanf(arq_csv, "%[^,]", tiktokReviews.app_version);
        fgetc(arq_csv);

        int i = 0;
        for(c = fgetc(arq_csv); c != '\n' && c!=EOF; c = fgetc(arq_csv)){
            tiktokReviews.posted_date[i] = c;
            i++;
        }
        tiktokReviews.posted_date[i] = '\0';

        //printaConteudoTiktokReviews(tiktokReviews);

        fwrite(tiktokReviews.review_id,     sizeof(tiktokReviews.review_id),    1, arq_bin);
        fwrite(tiktokReviews.review_text,   sizeof(tiktokReviews.review_text),  1, arq_bin);
        fwrite(&tiktokReviews.upvotes,      sizeof(tiktokReviews.upvotes),      1, arq_bin);
        fwrite(tiktokReviews.app_version,   sizeof(tiktokReviews.app_version),  1, arq_bin);
        fwrite(tiktokReviews.posted_date,   sizeof(tiktokReviews.posted_date),  1, arq_bin);
    }
}

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

void gravaLinhaEmBinario(FILE * arq_txt, FILE * arq_bin){

    char linha[500];
    char c;
    int i = 0;
    while(!feof(arq_txt)){
        c = fgetc(arq_txt);
        if(c != '\n'){
            linha[i] = c;
            i++;
            linha[i+1] = '\0';
        }
        else{
            fwrite(linha, sizeof(linha), 1, arq_bin);
            i = 0;
            linha[0] = '\0';
            }
        }

}

void leLinhaEmBinario(int i){
    FILE * arq_bin = fopen("linha.bin", "rb");
    char linha[500];

    fseek(arq_bin, i * sizeof(linha), 1);
    fread(&linha, sizeof(linha), 1, arq_bin);
    cout << linha << endl;

}

int menuTesteImportacao(){

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

    int resp;
    scanf("%d", &resp);
    return resp;

}

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
    //char diretorio_csv[] = "H:\\UFJF\\2021.3\\ED2\\Trabalho 1\\tiktok_app_reviews_reduzido.csv";
    //FILE * arq_csv = fopen(diretorio_csv, "r");
    //H:\UFJF\2021.3\ED2\Trabalho 1\tiktok_app_reviews_reduzido.csv

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
        gravaTiktokReviewEmBinario(arq_csv, arq_bin);
        fclose(arq_bin);
    }

    int resp = menuAcessaRegistro();

    acessaRegistro(resp);

    resp = menuTesteImportacao();
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

    cout << endl << endl << endl << "Pressione qualquer tecla para sair...";
    getch();
    return 0;
}
