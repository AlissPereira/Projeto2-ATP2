#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

//-------structs--------------

typedef struct{
	int id;
	char nome[50];
} ProdutoC;

typedef struct{
	int id;
	float vCom,vVen;
	int quantE,quantV;
	bool del;
} ProdutoI;

//------funções principais-----

int vende();//opção 1
int cadastra();//opção 2
int remover();//opção 3
int consultaV();//opção 4
int consultaL();//opção 5
int repor();//opção 6
void sair();//opção 0

//-----funções auxiliares-----

FILE *abreChave(FILE *chave);
FILE *abreInfs(FILE *infs);
ProdutoC *carregaChave(FILE *chave,ProdutoC *produtosC,int *c);
ProdutoI *carregaInfs(FILE *infs,ProdutoI *produtosI,int *c);
void catalogo(ProdutoC *produtosC,ProdutoI *produtosI,int c);
int procuraDel();
int registraDel(ProdutoI *produtosI,ProdutoC *produtosC,int c);

//----------Busca-------------

int busca(ProdutoC *produtosC,char item[50], int c);

//-----funções ordenação------

void partition(int esq, int dir, int *i, int *j,ProdutoC *produtosC);
void sort(int esq, int dir,ProdutoC *produtosC);
void quicksort(ProdutoC *produtosC, int c);

//-----------Main-------------

int main (int argc, char *argv[]){
	
	setlocale (LC_ALL,"");	
	
	int op,ret;
	
	do{
		system("cls");
		
		printf("Entre com a opção desejada\n1 - Vender\n2 - Cadastrar novo produto\n3 - Remover produto do estoque\n");
		printf("4 - Consultar estoque\n5 - Consultar lucro\n6 - Repor estoque\n0 - Sair\n");
		scanf("%d",&op);
		
		if(op<0||op>6){
			system("cls");
			fflush(stdin);
			printf("Opção invalida - aperte enter para prosseguir\n");			
			getchar();
			system("cls");
		}
		
		if(op==1){//1 - Vender
			system("cls");
			ret = vende();
			if(ret==0){
				system("cls");
				printf("Venda realizada com sucesso - Aperte enter para continuar...\n");
				getchar();
			}
			else if(ret==1){
				system("cls");
				printf("Não há estoque disponível para atender a venda - Aperte enter para continuar...\n");
				getchar();
			}
			else if(ret==2){
				system("cls");
				printf("Produto não existe no estoque - Aperte enter para continuar...\n");
				getchar();				
			}
			else if(ret==3){
				system("cls");
				printf("Não existem produtos cadastrados no estoque - Aperte enter para continuar...\n");
				getchar();				
			}
		}
		
		else if(op==2){//2 - Cadastrar novo produto
			system("cls");
			ret = cadastra();
			if(ret==0){
				system("cls");
				printf("Cadastro realizado com sucesso - Aperte enter para continuar...\n");
				getchar();
			}
			if(ret==2){
				system("cls");
				printf("A quantidade vendida não pode ser maior que a quantidade em estoque - Aperte enter para continuar...\n");
				getchar();		
			}
		}
		
		else if(op==3){//3 - Remover produto do estoque
			system("cls");
			ret = remover();
			if(ret==0){
				system("cls");
				printf("Produto removido com sucesso - Aperte enter para continuar...\n");
				getchar();
			}
			else if(ret==2){
				system("cls");
				printf("Produto não existe no catalogo - Aperte enter para continuar...\n");
				getchar();
			}
			else if(ret==3){
				system("cls");
				printf("Não existe nenhum produto cadastrado - Aperte enter para continuar...\n");
				getchar();
			}
		}

		else if(op==4){//4 - Consultar estoque
			system("cls");
			consultaV();
			printf("\n\nAperte enter para continuar... ");
			getchar();
		}
		
		else if(op==5){//5 - Consultar lucro
			system("cls");
			consultaL();
			printf("\nAperte enter para continuar... ");
			getchar();
		}
		
		else if(op==6){//6 - Repor estoque
			system("cls");
			ret = repor();
			if(ret==0){
				printf("Estoque reposto com sucesso - Aperte enter para continuar... ");
			}
			else if(ret==1){
				printf("Não é possivel repor uma quantidade inferior a zero - Aperte enter para continuar... ");
			}
			else if(ret==2){
				printf("Produto não existe no catalogo - Aperte enter para continuar... ");
			}
			printf("\n");
			getchar();	
		}	
		
		else if(op==0){//0 - sair
			system("cls");
			sair();
		}		
		
	}while(op!=0);
	
	return 0;
}

//------funções---------------

int vende(){//opção 1
	FILE *chave, *infs;
	
	ProdutoI *produtosI;	
	ProdutoC *produtosC;
	
	int c,i,aux;
	char compra[50];
	
	chave = abreChave(chave);
	infs = abreInfs(infs);
	
	produtosI = carregaInfs(infs,produtosI,&c);
	produtosC = carregaChave(chave,produtosC,&c);

	c=c-1;
	
	if(c==0){//não existem produto cadastrado
		fflush(stdin);
		return 3;
	}

	catalogo(produtosC,produtosI,c);//mostra catalogo de produtos
		
	printf("Digite qual produto deseja comprar: ");
	fflush(stdin);
	fgets(compra,50,stdin);
	
	for(i=0;i<c;i++){
		if(strcmp(compra,produtosC[i].nome)==0){
			aux=-1;
			break;
		}
	}
	
	if(aux!=-1){
		fflush(stdin);
		return 2;//produto não existem no estoque
	}
	
	printf("O valor do produto é %.2f, entre com a quantidade que o cliente quer comprar desse produto: ",produtosI[produtosC[i].id-1].vVen);
	fflush(stdin);
	scanf("%d",&aux);
	
	if(aux<=produtosI[produtosC[i].id-1].quantE){
		produtosI[produtosC[i].id-1].quantE = produtosI[produtosC[i].id-1].quantE-aux;//diminui estoque
		produtosI[produtosC[i].id-1].quantV = produtosI[produtosC[i].id-1].quantV+aux;//aumenta a quantidade de produtos vendidos
		
		fclose(infs);
		infs = fopen("infs.bin","w+b");
		
		for(i=0;i<c;i++){
			fwrite(&produtosI[i],sizeof(ProdutoI),1,infs);
		}
		
		fflush(stdin);
		fclose(infs);
		fclose(chave);
		return 0;
	}
	else{
		fflush(stdin);
		fclose(infs);
		fclose(chave);
		return 1;
	}
}

int cadastra(){//opção 2
	
	FILE *chave, *infs;
	
	ProdutoI *produtosI;
	
	ProdutoC *produtosC;
	
	int c,i,ret,aux=0,op;
	
	chave = abreChave(chave);
	infs = abreInfs(infs);
	
	produtosI = carregaInfs(infs,produtosI,&c);
	produtosC = carregaChave(chave,produtosC,&c);
	
	ret = procuraDel();
	
	//If garante que quando algum registro for excluido e chave tiver menos registros que info, o cadastro de produtos ira funcionar normalmente
	if(ret==0){//não tem registro excluido
		c=c-1;
		
		printf("Digite o nome do produto: ");
		fflush(stdin);
		fgets(produtosC[c].nome,50,stdin);
		produtosI[c].id = c+1;		
	}
	else{//tem registro excluido
		printf("Digite o nome do produto: ");
		fflush(stdin);
		fgets(produtosC[c-1].nome,50,stdin);
		produtosI[c].id = c;
	}
	
	//adiciona infos de ProdutosI
	
	printf("Entre com o valor da compra: ");
	scanf("%f",&produtosI[c].vCom);
	
	printf("Entre com o valor da venda: ");
	scanf("%f",&produtosI[c].vVen);
	
	printf("Entre com a quantidade em estoque: ");
	scanf("%d",&produtosI[c].quantE);
	
	printf("Entre com a quantidade vendida: ");
	scanf("%d",&produtosI[c].quantV);
	
	if(produtosI[c].quantV>produtosI[c].quantE){
		fflush(stdin);
		return 2;
	}
	if(produtosI[c].quantV>0){
		do{
			printf("\nDeseja que essa quantidade vendida seja reduzida do estoque?\n0 - sim\n1 - não\n");
			scanf("%d",&op);
			
			if(aux<0||aux>1){
				printf("Opção invalida - digite um valor valido");
				fflush(stdin);
				getchar();
				system("cls");
			}
			else if(op==0){
				produtosI[c].quantE = produtosI[c].quantE-produtosI[c].quantV;
				aux=1;
			}
			else{
				aux=1;
			}
			
		}while(aux!=1);
	}
	
	produtosI[c].del = 0;
	
	fflush(stdin);
	
	if(ret==1){
		ret = registraDel(produtosI,produtosC,c);//sobrescreve registros deletados
		
		quicksort(produtosC,c);//ordena chave
		
		fclose(chave);
		fopen("chave.bin","w+b");
		
		for(i=0;i<c;i++){
			fwrite(&produtosC[i],sizeof(ProdutoC),1,chave);
		}
		
	}
	else{//não encontrou registro deletado, então cadastra registro normalmente
		fwrite(&produtosI[c],sizeof(ProdutoI),1,infs);

		produtosC[c].id = c+1;

		quicksort(produtosC,c+1);//ordena chave
	
		fclose(chave);
		fopen("chave.bin","w+b");
	
		for(i=0;i<c+1;i++){
			fwrite(&produtosC[i],sizeof(ProdutoC),1,chave);
		}

	}
	fclose(chave);
	fclose(infs);
	fflush(stdin);
	return 0;
}

int remover(){//opção 3
	FILE *chave, *infs;
	
	ProdutoI *produtosI;
	
	ProdutoC *produtosC;
	ProdutoC *aux;
	
	int c,i,ret;
	char item[50];
	
	chave = abreChave(chave);
	infs = abreInfs(infs);
	
	produtosI = carregaInfs(infs,produtosI,&c);
	produtosC = carregaChave(chave,produtosC,&c);
	aux = (ProdutoC*) malloc(c*sizeof(ProdutoC));
	c=c-1;
	
	if(c==0){//não existe produto cadastrado
		fflush(stdin);
		return 3;
	}

	catalogo(produtosC,produtosI,c);//mostra catalogo de produtos
		
	printf("Digite qual produto deseja remover: ");
	fflush(stdin);
	fgets(item,50,stdin);
	
	for(i=0;i<c;i++){
		if(strcmp(item,produtosC[i].nome)==0){
			ret=-1;
			break;
		}
	}
	
	if(ret!=-1){
		fflush(stdin);
		return 2;//produto não existe no estoque
	}
	
	for(i=0;i<c;i++){//acha o produto que tem que excluir
		if(strcmp(item,produtosC[i].nome)==0){
			break;
		}
	}
	
	produtosI[produtosC[i].id-1].del = 1;
	produtosI[produtosC[i].id-1].id = -1;
	
	produtosC[i].id= -1;
	strcpy(produtosC[i].nome,"");
	
	for(i=0;i<c;i++){
		if(produtosC[i].id!=-1){
			aux[i] = produtosC[i];
		}
	}
	
	quicksort(aux,c);//ordena o novo vetor
	
	for(i=0;i<c-1;i++){
		aux[i]=aux[i+1];//remove espaço no começo do vetor
	}
	
	fclose(chave);
	fclose(infs);
	chave = fopen("chave.bin","w+b");
	infs = fopen("infs.bin","w+b");
	
	for(i=0;i<(c-1);i++){//rescreve o primeiro arquivo
		fwrite(&aux[i],sizeof(ProdutoC),1,chave);
	}
	for(i=0;i<c;i++){//rescreve o segundo arquivo
		fwrite(&produtosI[i],sizeof(ProdutoI),1,infs);
	}
	
	getchar();
	fflush(stdin);
	fclose(chave);
	fclose(infs);
	
	return 0;
}

int consultaV(){//opção 4
	FILE *chave, *infs;
		
	chave = abreChave(chave);
	infs = abreInfs(infs);
	
	ProdutoI *produtosI;	
	ProdutoC *produtosC;
	
	int c,i;
	float soma=0;
	
	produtosI = carregaInfs(infs,produtosI,&c);
	produtosC = carregaChave(chave,produtosC,&c);
	
	c=c-1;
	
	if(c==0){
		system("cls");
		printf("Não existem produtos cadastrados no estoque - Aperte enter para continuar...\n");
		getchar();	
		return 0;
	}
	
	printf("=====Produtos=====\n\n");
	
	for(i=0;i<c;i++){
		printf("Produto: %s",produtosC[i].nome);
		printf("Quantidade em estoque: %d\n",produtosI[produtosC[i].id-1].quantE);
		printf("Valor do estoque do produto: R$ %.2f\n\n",(float)produtosI[produtosC[i].id-1].quantE*produtosI[produtosC[i].id-1].vVen);
		
		soma = ((float)produtosI[produtosC[i].id-1].quantE*produtosI[produtosC[i].id-1].vVen) + soma;
	}
	
	printf("O valor total do estoque é R$ %.2f",soma);
	
	getchar();
	fflush(stdin);
	return 0;
}

int consultaL(){//opção 5
	FILE *chave, *infs;
	
	ProdutoI *produtosI;	
	ProdutoC *produtosC;
	
	int c,i;
	float lucro;
	
	chave = abreChave(chave);
	infs = abreInfs(infs);
	
	produtosI = carregaInfs(infs,produtosI,&c);
	produtosC = carregaChave(chave,produtosC,&c);

	c=c-1;
	
	if(c==0){
		system("cls");
		printf("Não existem produtos cadastrados no estoque - Aperte enter para continuar...\n");
		getchar();
		return 0;
	}
	
	printf("=====Vendas de cada produto=====\n\n");
	for(i=0;i<c;i++){
		lucro = (produtosI[produtosC[i].id-1].vVen*(float)produtosI[produtosC[i].id-1].quantV)-(produtosI[produtosC[i].id-1].vCom*(float)produtosI[produtosC[i].id-1].quantV);
		
		printf("Foram vendidos R$ %.2f de %s\n",lucro,produtosC[i].nome);
	}
	getchar();
	fflush(stdin);
	return 0;
}

int repor(){//opção 6
	FILE *chave, *infs;
	
	ProdutoI *produtosI;	
	ProdutoC *produtosC;
	
	int c,i,aux,quant;
	char item[50];
	
	chave = abreChave(chave);
	infs = abreInfs(infs);
	
	produtosI = carregaInfs(infs,produtosI,&c);
	produtosC = carregaChave(chave,produtosC,&c);
	
	c=c-1;
	
	if(c==0){//não existe produto cadastrado
		fflush(stdin);
		return 3;
	}

	catalogo(produtosC,produtosI,c);
		
	printf("Digite qual produto repor o estoque: ");
	fflush(stdin);
	fgets(item,50,stdin);
	
	aux = busca(produtosC,item,c);
	
	if(aux==-1){
		fflush(stdin);
		return 2;//produto não existe no estoque
	}
	
	printf("Digite o quanto deseja repor no estoque: ");
	fflush(stdin);
	scanf("%d",&quant);
	
	if(quant>0){
		produtosI[produtosC[aux].id-1].quantE = produtosI[produtosC[aux].id-1].quantE+quant;//diminui estoque
		fclose(infs);
		infs = fopen("infs.bin","w+b");
		
		for(i=0;i<c;i++){
			fwrite(&produtosI[i],sizeof(ProdutoI),1,infs);
		}
		
		fflush(stdin);
		fclose(infs);
		fclose(chave);
		return 0;//sucesso
	}
	else{
		fflush(stdin);
		fclose(infs);
		fclose(chave);
		return 1;//quant = 0 ou menor
	}
}

void sair(){//opção 0
	FILE *chave, *infs;
	
	ProdutoI *produtosI;	
	ProdutoC *produtosC;
	
	int c,i;
	float soma=0,lucro=0;
	
	chave = abreChave(chave);
	infs = abreInfs(infs);
	
	produtosI = carregaInfs(infs,produtosI,&c);
	produtosC = carregaChave(chave,produtosC,&c);

	c=c-1;
	
	printf("=====Resumo do dia=====\n\n");
	for(i=0;i<c;i++){
		printf("Produto: %s",produtosC[i].nome);
		printf("Quantidade vendida: %d\n",produtosI[produtosC[i].id-1].quantV);
		printf("Quantidade em estoque: %d\n",produtosI[produtosC[i].id-1].quantE);
		printf("Valor do produto: R$ %.2f\n",produtosI[produtosC[i].id-1].vVen);
		printf("Valor do estoque do produto: R$ %.2f\n\n",(float)produtosI[produtosC[i].id-1].quantE*produtosI[produtosC[i].id-1].vVen);
	
		soma = ((float)produtosI[produtosC[i].id-1].quantE*produtosI[produtosC[i].id-1].vVen) + soma;
		lucro = ((produtosI[produtosC[i].id-1].vVen*(float)produtosI[produtosC[i].id-1].quantV)-(produtosI[produtosC[i].id-1].vCom*(float)produtosI[produtosC[i].id-1].quantV)) + lucro;
	}
	
	printf("O valor total do estoque é R$ %.2f\n",soma);
	printf("O lucro do dia é R$ %.2f\n",lucro);
	
	fflush(stdin);
	getchar();
	fclose(infs);
	fclose(chave);
	fflush(stdin);
}

//funções auxiliares

FILE *abreChave(FILE *chave){
	chave = fopen("chave.bin","a+b");
	
	if (chave == NULL) {
		printf("Nao foi possivel abrir chave.bin!");
		getchar();
		return NULL;
	}
	
	return chave;
}

FILE *abreInfs(FILE *infs){
	infs = fopen("infs.bin","a+b");
	
	if (infs == NULL) {
		printf("Nao foi possivel abrir infs.bin!");
		getchar();
		return NULL;
	}
	
	return infs;
}

ProdutoC *carregaChave(FILE *chave,ProdutoC *produtosC,int *c){
	produtosC = (ProdutoC*) malloc((*c)*sizeof(ProdutoC));
	
	for((*c)=0;!feof(chave);(*c)++){
		fread(&produtosC[*c],sizeof(ProdutoC),1,chave);
	}
	return produtosC;
}

ProdutoI *carregaInfs(FILE *infs,ProdutoI *produtosI,int *c){
	produtosI = (ProdutoI*) malloc(sizeof(ProdutoI));
	
	for((*c)=0;!feof(infs);(*c)++){
		produtosI = (ProdutoI*) realloc(produtosI,((*c)+1)*sizeof(ProdutoI));
		fread(&produtosI[*c], sizeof(ProdutoI), 1, infs);
	}
	
	return produtosI;
}

void catalogo(ProdutoC *produtosC,ProdutoI *produtosI, int c){
	int aux,i;
	
	do{
		printf("Deseja ver os produtos em estoque\n1 - Sim\n0 - Não\n");
		scanf("%d",&aux);
		system("cls");
		
		if(aux==1){
			system("cls");
			printf("=======Catalogo de produto========\n\n");
			
			for(i=0;i<c;i++){
				printf("Nome: %s",produtosC[i].nome);
				printf("estoque: %d\n\n",produtosI[produtosC[i].id-1].quantE);
			}
		}
		
		if(aux<0 || aux>1){
			printf("Opção invalida\n");
			system("cls");
		}
		
		printf("Aperte enter para continuar... ");
		fflush(stdin);
		getchar();
		
	}while(aux<0 || aux>1);
	system("cls");

}

int procuraDel(){
	int i,c=0,aux;
	
	FILE *infs;
	ProdutoI *produtosI;
	
	infs = abreInfs(infs);
	
	produtosI = carregaInfs(infs,produtosI,&c);
	
	for(i=0;i<c-1;i++){
		if(produtosI[i].del==1){
			aux=1;
			break;
		}
	}
	
	fclose(infs);
	free(produtosI);
	
	if(aux==1){
		return 1;//achou
	}
	else{
		return 0;//não achou
	}
}

int registraDel(ProdutoI *produtosI,ProdutoC *produtosC,int c){
	int i,aux;
	
	FILE *infs;
	
	infs = abreInfs(infs);
	
	for(i=0;i<c;i++){

		if(produtosI[i].del==1){
			produtosI[i]=produtosI[c];
			produtosI[i].id=i+1;			
			produtosC[c-1].id=i+1;
	
			break;
		}
	}

	fclose(infs);

	infs = fopen("infs.bin","w+b");
		
	for(i=0;i<c;i++){
		fwrite(&produtosI[i],sizeof(ProdutoI),1,infs);
	}
		
	fclose(infs);
	fflush(stdin);
		
	return 1;//encontrou um registro deletado
}

//busca binaria

int busca(ProdutoC *produtosC,char item[50], int c){
	int m,inf = 0, sup = c-1;
	
	while(inf<=sup){
		m = inf+sup/2;
	
		if(strcmp(item,produtosC[m].nome)>0){
			sup = m-1;
		}
		else if(strcmp(item,produtosC[m].nome)<0){
			inf=m+1;
		}
		else{
			return m;
		}
	}
	
	return -1;
}

//ordenação

void quicksort(ProdutoC *produtosC, int c){
	sort(0,c-1,produtosC);
}

void sort(int esq, int dir,ProdutoC *produtosC){
	
	int i,j;
	
	partition(esq,dir,&i,&j,produtosC);
	
	if(esq<j){
		sort(esq,j,produtosC);
	}
	if(i<dir){
		sort(i,dir,produtosC);
	}
}

void partition(int esq, int dir, int *i, int *j,ProdutoC *produtosC){
	
	ProdutoC x,w;
	
	*i = esq;
	*j = dir;
	
	x = produtosC[(*i+*j)/2];
	
	do{
		
		while(strcmp(x.nome,produtosC[*i].nome)>0){
			(*i)++;
		}
		
		while(strcmp(x.nome,produtosC[*j].nome)<0){
			(*j)--;
		}	
		
		if(*i<=*j){
			w = produtosC[*i];
			produtosC[*i] = produtosC[*j];
			produtosC[*j] = w;
			(*i)++;
			(*j)--;			
		}
		
	}while(*i<=*j);
}

