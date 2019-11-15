#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h> 
#include<string.h>
#include<errno.h> 
#include<time.h>
#include<stdio.h>
#include"tela.h"
#include <locale.h>

int fd = 0;
int a;
char psq[100]; // variavel para pesquisas

struct data{
int dia, mes, ano;
};

struct cliente{
int id;
char nome[100];
int rg;
struct data nascimento;
char cidade[100];};

struct filme{
int id;
char titulo[100];
char categoria[100];
int classif_indicada;
float valor_diario;
int status; // se = 1 está locado!!
};


struct locacao{
int id;
int id_filme;
int id_cliente;
struct tm dt_saida;
struct tm dt_entrada;
int d_locados;
float valor_t;
};

/*  struct globais  */
struct cliente client[100];
struct filme film[100];
struct locacao loc[100];
struct tm d_atual;     
time_t segundos;

extern int errno; 

/* funções gerais  */

void hora(){
time(&segundos);  
d_atual = *localtime(&segundos);
}

void imprimeData(){
hora();
printf("        ** ** Data: %d/%d/%d || ", d_atual.tm_mday, d_atual.tm_mon+1, d_atual.tm_year+1900); 
printf("Hora: %d:%d:%d  ** ** \n\n", d_atual.tm_hour, d_atual.tm_min, d_atual.tm_sec);//hora
}

void verifica(int erro){
if (erro ==-1){ 
printf("Error Number %d \n", errno); 	
perror("Program");}}

void carArquivos(){
fd = open("filmes.txt", O_RDWR | O_CREAT, 0666); 
verifica(fd);
read(fd, film, sizeof (film)*100);
close(fd);

fd = open("clientes.txt", O_RDWR | O_CREAT, 0666); 
verifica(fd);
read(fd, client, sizeof (client)*100);
close(fd);

fd = open("locacoes.txt", O_RDWR | O_CREAT, 0666); 
verifica(fd);
read(fd, loc, sizeof (loc)*100);
close(fd);

}

void arqLocacoes(){
fd = open("locacoes.txt", O_RDWR | O_CREAT, 0666);
verifica(fd);
write(fd, &loc, sizeof(loc)*100);
close(fd);}

void arqClientes(){
fd = open("clientes.txt", O_RDWR | O_CREAT, 0666);
verifica(fd);
write(fd, &client, sizeof(client)*100);
close(fd);}

void arqFilmes(){
fd = open("filmes.txt", O_RDWR | O_CREAT, 0666);
verifica(fd);
write(fd, &film, sizeof(film)*100);
close(fd);}

void calcValor(int c){ //calcula valor locação

printf("\n Data locação: %d/%d/%d ", loc[c].dt_saida.tm_mday, loc[c].dt_saida.tm_mon+1, loc[c].dt_saida.tm_year+1900);
printf("\n Data devolução: %d/%d/%d ", loc[c].dt_entrada.tm_mday, loc[c].dt_entrada.tm_mon+1, loc[c].dt_entrada.tm_year+1900);

loc[c].d_locados = (difftime(mktime(&loc[c].dt_entrada), mktime(&loc[c].dt_saida)))/(60*60*24);
loc[c].valor_t = loc[c].d_locados * film[loc[c].id_filme-1].valor_diario;

printf("\n Valor total da locação foi: %.2f em função de %d dias locados!\n", loc[c].valor_t, loc[c].d_locados);
}

/*   Navegação Menu   */

int menu1(){ // menu inicial
int opcao=-1;
rodape();
imprimeData();
printf("1.Filmes || 2.Relatórios || 3.Cadastro de Cliente || 4.Locações || 0.Sair || ->_");
while(opcao>4 || opcao<0){
	scanf("%d", &opcao);
	if(opcao>4 || opcao<0){
		printf("\n ERRO: Digite uma opção válida!\n\n");
	}
}
if (opcao == 0){opcao=-1;}
return opcao;
}

int menu11(){ // menu/filmes
int opcao=-1;
rodape();
printf("Menu/Filmes   \n 1.Novo || 2.Pesquisa || 3.Editar || 0.Voltar || ->_");
while(opcao>3 || opcao<0){
	scanf("%d", &opcao);
	if(opcao>3 || opcao<0){
		printf("\n ERRO: Digite uma opção válida!\n\n");
	}
}
return opcao;
}

int menu111(){ // menu/filmes/novo
int b = 0;
rodape();
printf("Menu/Filmes/Novo \nComplete com os dados do Novo Filme:\n\n");

for(a=0; a<100; a++){
	if (film[a].id == 0){
		b = a;
		film[a].id = a+1;
	break;
	}
}
printf("ID: %d\n", film[a].id);
limparb();
printf("Nome do filme: ");
gets(film[b].titulo);
printf("Categoria(Ex. ação): ");
gets(film[b].categoria);
printf("Classificação indicada (idade em anos): ");
scanf(" %d", &film[b].classif_indicada);
printf("Valor aluguel diário (EX. 2.50): R$ ");
scanf(" %f", &film[b].valor_diario);
arqFilmes();
printf("\nFilme cadastrado com sucesso!!!\n");
limparb();
continuar();
return 10;
}

int menu112(){ // menu/filmes/pesquisa
int flag =0;
int t;
printf("Digite o nome de filme ou ID para pesquisa: ");
limparb();
gets(psq);
printf("\n\n Resultado: ");
for (a=0; (a<100 && film[a].id != 0); a++){
	t = atoi(psq); // conversor para int
	if(strstr(film[a].titulo, psq) || (film[a].id == t)){
		printf("\n\nID: %d \n Titulo: %s\n Categoria: %s\n Classificação: %d anos\n Diária: R$ %.2f\n Status: ", film[a].id, film[a].titulo, film[a].categoria, film[a].classif_indicada, film[a].valor_diario);
		if (film[a].status == 0){
		printf("Disponível");
		}
		else{
		printf("Locado");
		}
		flag ++;
	}
}
if (flag==0){
	printf(" Nenhum filme localizado!");
}
continuar();
return 10;
}

int menu113(){ //menu/filmes/editar
int t=0;
printf("\nDigite a ID que do filme que deseja editar ou '0' para voltar:");
scanf("%d", &a);
a=a-1;
if (film[a].id>0){
	printf("\n ID: %d \n(1) Titulo: %s \n(2) Categoria: %s", film[a].id, film[a].titulo, film[a].categoria);
	printf("\n(3) Classificação: %d anos \n(4) Diária: R$ %.2f \n ", film[a].classif_indicada, film[a].valor_diario);
	printf("\nDigite o campo que deseja alterar ou '0' para finalizar: ");
	scanf("%d", &t);
}
else{
	printf("Nenhum filme localizado!");
	t=-1;
}

while (t>0){
	switch (t){
		case 1:
			printf("\nNome do filme: ");
			limparb();
			gets(film[a].titulo);
			break;
		case 2:
			printf("\nCategoria(Ex. ação): ");
			limparb();
			gets(film[a].categoria);
			break;
		case 3:
			printf("\nClassificação indicada (idade em anos): ");
			scanf(" %d", &film[a].classif_indicada);
			break;
		case 4:
			printf("\nValor aluguel diário (EX. 2.50): R$ ");
			scanf(" %f", &film[a].valor_diario);
			break;
		case 5:
			printf("\nStatus 0- Disponível 1- Locado: ");
			scanf(" %d", &film[a].status);
			break;
		default:
			break;
	}
printf("\nDeseja alterar mais algum campo? (opção '0' para finalizar) ");
scanf("%d", &t);
}
arqFilmes();
printf("\nEncerrado!!\n");
sleep(1);return 10;
}

int menu12(){ //menu/relatório
int opcao=-1;
rodape();
printf("Menu/Relatórios\n1.Todos os filmes || 2.Todos os clientes ||");
printf("3.Filmes disponíveis || 4.Filmes locados || 0.Voltar ->");
while(opcao>4 || opcao<0){
	scanf("%d", &opcao);
	if(opcao>4 || opcao<0){
		printf("\n ERRO: Digite uma opção válida!\n\n");
	}
}
return opcao;
}

int menu121(){ // relatorio todos filmes cadastrados
int w;
rodape();
printf("Menu/Relatorios/FilmesCadastrados:");

for (a=0; (a<100 && film[a].id != 0); a++){
	printf("\n\nID: %d\n Titulo: %s\n Categoria: %s\n Classificação: %d anos\n Diária: R$ %.2f\n Status: ", film[a].id, film[a].titulo, film[a].categoria, film[a].classif_indicada, film[a].valor_diario);
	if (film[a].status == 0){
		printf("Disponível");
	}
	else{
		printf("Locado");
	}
}
printf("\n\n   ****  Gerar PDF? (1.Sim 2.Não): ");
scanf("%d", &w);

if (w==1){ //Relatório PDF
	FILE *fp;
	fp = fopen("relatorio.txt", "w"); 
	fprintf(fp, "\n*****  Relatorio de filmes cadastrados no Sistema: *****\n\n");
	for (a=0; (a<100 && film[a].id != 0); a++){
		fprintf(fp, "\n\nID: %d\n Titulo: %s\n Categoria: %s\n Classificacao: %d anos\n Diaria: R$ %.2f\n Status: ", film[a].id, film[a].titulo, film[a].categoria, film[a].classif_indicada, film[a].valor_diario);
		if (film[a].status == 0){
			fprintf(fp, "Disponivel");
		}
		else{fprintf(fp, "Locado");
		}
	}
fflush(fp); // 
fclose(fp); 
system("enscript relatorio.txt -o - | ps2pdf - relatorio_filmes_cadastrados.pdf"); // Fecha Relatório

printf("\nGerado com sucesso!!");
}
limparb();
continuar();
return 10;}

int menu122(){
int w = 0;
rodape();
printf("Menu/Relatórios/ClientesCadastrados:\n");
for (a=0; (a<100 && client[a].id != 0); a++){
	printf("\nID: %d \n Nome: %s \n Cidade: %s\n", client[a].id, client[a].nome, client[a].cidade);
}

printf("\n\n   ****  Gerar PDF? (1.Sim 2.Não): ");
scanf("%d", &w);

if (w==1){ //Relatório PDF
	FILE *fp;
	fp = fopen("relatorio.txt", "w"); 
	fprintf(fp, "\n*****  Relatorio de clientes cadastrados no Sistema: *****\n\n");
	for (a=0; (a<100 && client[a].id != 0); a++){
		fprintf(fp, "\nID: %d \n Nome: %s \n Cidade: %s\n", client[a].id, client[a].nome, client[a].cidade);
	}
	fflush(fp); 
	fclose(fp); 
	system("enscript relatorio.txt -o - | ps2pdf - relatorio_clientes_cadastrados.pdf"); // Fecha Relatório
	printf("\nGerado com sucesso!!");
}

limparb();
continuar();
return 10;}

int menu123(){
int flag=0;
int w = 0;
rodape();
printf("Menu/Relatórios/FilmesDisponíveis:");
for (a=0; (a<100 && film[a].id != 0); a++){
	if (film[a].status == 0){
		flag=1;
		printf("\n\nID: %d\n Titulo: %s\n Categoria: %s\n Classificação: %d anos\n Diária: R$ %.2f", film[a].id, film[a].titulo, film[a].categoria, film[a].classif_indicada, film[a].valor_diario);
	}
}
if (flag==0){
	printf("\n\nNenhum Filme Disponível!!\n");
}

printf("\n\n   ****  Gerar PDF? (1.Sim 2.Não): ");
scanf("%d", &w);

if (w==1){ //Relatório PDF
	FILE *fp;
	fp = fopen("relatorio.txt", "w"); 
	fprintf(fp, "\n*****  Relatorio de filmes disponiveis sistema: *****\n\n");
	if (flag==1){
		for (a=0; (a<100 && film[a].id != 0); a++){
			if (film[a].status == 0){
				fprintf(fp, "\n\nID: %d\n Titulo: %s\n Categoria: %s\n Classificacao: %d anos\n Diaria: R$ %.2f", film[a].id, film[a].titulo, film[a].categoria, film[a].classif_indicada, film[a].valor_diario);
			}
		}
	}
	else{	
		fprintf(fp, "\n\nNenhum Filme Disponivel!!\n");
	}
	fflush(fp); 
	fclose(fp); 
	system("enscript relatorio.txt -o - | ps2pdf - relatorio_filmes_disponiveis.pdf"); // Fecha Relatório
	printf("\nGerado com sucesso!!");
}

limparb();
continuar();
return 10;}

int menu124(){
int flag=0;
int w = 0;
rodape();
printf("Menu/Relatórios/FilmesLocados:");
for (a=0; (a<100 && film[a].id != 0); a++){
	if (film[a].status == 1){
		flag=1;
		printf("\n\nID: %d\n Titulo: %s\n Categoria: %s\n Classificação: %d anos\n Diária: R$ %.2f", film[a].id, film[a].titulo, film[a].categoria, film[a].classif_indicada, film[a].valor_diario);
	}
}
if (flag==0){
	printf("\n\nNenhum Filme Locado!!\n");
}


printf("\n\n   ****  Gerar PDF? (1.Sim 2.Não): ");
scanf("%d", &w);

if (w==1){ //Relatório PDF
	FILE *fp;
	fp = fopen("relatorio.txt", "w"); 
	fprintf(fp, "\n*****  Relatorio de filmes locados sistema: *****\n\n");
	if (flag==1){
		for (a=0; (a<100 && film[a].id != 0); a++){
			if (film[a].status == 1){
				fprintf(fp, "\n\nID: %d\n Titulo: %s\n Categoria: %s\n Classificacao: %d anos\n Diaria: R$ %.2f", film[a].id, film[a].titulo, film[a].categoria, film[a].classif_indicada, film[a].valor_diario);
			}
		}
	}
	else{	
		fprintf(fp, "\n\nNenhum Filme Locado!!\n");
	}
	fflush(fp); 
	fclose(fp); 
	hora();
	system("enscript relatorio.txt -o - | ps2pdf - relatorio_filmes_locados.pdf"); // Fecha Relatório
	printf("\nGerado com sucesso!!");
}

limparb();
continuar();
return 10;}

int menu13(){ // menu/cadastro clientes
int opcao=-1;
rodape();
printf("Menu/Cadastro de clientes   \n 1.Novo || 2.Pesquisa || 0.Voltar || ->_");
while(opcao>2 || opcao<0){
	scanf("%d", &opcao);
	if(opcao>2 || opcao<0){
		printf("\n ERRO: Digite uma opção válida!\n\n");
	}
}
return opcao;
}

int menu131(){ // menu/Cadastro Cliente/novo
int b=0;
rodape();
printf("Menu/Cadastro Clientes/Novo \nComplete com os dados do novo cliente:\n");
for(a=0; a<100; a++){
	if (client[a].id == 0){
		b = a;
		client[a].id = a+1;
		break;
	}
}
printf("ID: %d \n", client[a].id);
limparb();
printf("Nome: ");
gets(client[b].nome);
printf("Data de nascimento Dia: ");
scanf(" %d", &client[b].nascimento.dia);
printf("Data de nascimento Mes: ");
scanf(" %d", &client[b].nascimento.mes);
printf("Data de nascimento Ano(xxxx): ");
scanf(" %d", &client[b].nascimento.ano);
printf("Cidade: ");
limparb();
gets(client[b].cidade);
arqClientes();
printf("\n\nCliente cadastrado com sucesso!!!\n");
sleep(1);
return 10;
}

int menu132(){ // menu/cliente/pesquisa
printf("Digite o nome para pesquisa: ");
limparb();
gets(psq);
printf("\n\n Resultado: \n");
for (a=0; (a<100 && client[a].id != 0); a++){
	if(strstr(client[a].nome, psq)){
		printf("\nID: %d\n Nome: %s \n", client[a].id, client[a].nome);
	}
}
continuar();
return 10;
}

int menu14(){ // menu/Locações
int opcao=-1;
rodape();
printf("Menu/Locações   \n 1.Novo || 2.Pesquisa || 3.Finalizar || 0.Voltar || ->_");
while(opcao>3 || opcao<0){
	scanf("%d", &opcao);
	if(opcao>3 || opcao<0){
		printf("\n ERRO: Digite uma opção válida!\n\n");
	}
}
return opcao;
}

int menu141(){ // menu/Locacao/novo
int b = 0;
int c = 2;
rodape();
printf("Menu/Locação/Novo \nComplete os campos para locação:\n");

for(a=0; a<100; a++){
	if (loc[a].id == 0){
		b = a;
		loc[a].id = a+1;
		break;
 	}
}
while (c==2){
	printf("\nID Cliente: ");
	scanf(" %d", &loc[b].id_cliente);
	printf("Nome: %s !!\n", client[(loc[b].id_cliente-1)].nome);
	printf("Cliente correto? (1) Sim (2) Não -> ");
	scanf("%d", &c);
}
if (c==1){
	c=2;
}
while (c==2){
	printf("\nID Filme: ");
	scanf(" %d", &loc[b].id_filme);
	if(film[(loc[b].id_filme-1)].status == 0){
		printf("Titulo: %s !!\n", film[(loc[b].id_filme-1)].titulo);
		printf("Filme correto? (1) Sim (2) Não -> ");
		scanf("%d", &c);
	}
	else{
		printf("\nFilme já locado! Escolha outro!!\n");
	}
}

hora();
loc[b].dt_saida = d_atual; // carrega hora atual
film[(loc[b].id_filme-1)].status = 1; // flag filme locado

printf("\nRegistrado a saida do filme '%s' pra o cliente '%s' com valor diário de '%.2f' : ", film[(loc[b].id_filme-1)].titulo, client[loc[b].id_cliente-1].nome, film[loc[b].id_filme-1].valor_diario);
arqLocacoes();
arqFilmes();
printf("\n\nFinalizado!!\n\n");
printf("\n\n Tecle enter para voltar");
limparb();
gets(psq);
return 10;
}

int menu142(){ //menu/locaçao/pesquisa
int c=0;
int flag = 0;
printf("Digite o nome do cliente: ");
limparb();
gets(psq);
printf("\n Resultado da pesquisa: \n");
for (a=0; (a<100 && loc[a].id != 0); a++){

	if(strstr(client[a].nome, psq)){
		for (c=0; (c<100 && loc[c].id != 0); c++){
			if (client[a].id == loc[c].id_cliente && film[(loc[c].id_filme-1)].status == 1 && loc[c].d_locados == 0){
				flag = 1;
				printf("\nID locação: %d\n ID cliente: %d\n Nome: %s\n Filme locado: %s\n", loc[c].id, client[a].id, client[a].nome, film[loc[c].id_filme-1].titulo);

				printf(" Data locação: %d/%d/%d \n", loc[c].dt_saida.tm_mday, loc[c].dt_saida.tm_mon+1, loc[c].dt_saida.tm_year+1900);

			}
		}
	}
}
if (flag == 0){
	printf("\nNenhum registro encontrado em aberto!!\n");
}
continuar();
return 10;}

int menu143(){
int c, d =0;
rodape();

/* Gera data antiga // lógica para testes de cálculo*/
for (a=0; loc[a].id_filme != 0; a++){
loc[a].dt_saida.tm_mday = a+1;
}


printf("Menu/Locação/Encerrar \nDigite o ID da locação:");
scanf("%d", &c);
c=c-1;
if (loc[c].id>0 && film[(loc[c].id_filme-1)].status==1 && loc[c].d_locados == 0){
	printf("\nID locação: %d\n Nome: %s (ID %d)\n Filme locado: %s \n Data locação: %d/%d/%d \n Valor diário: %.2f \n", loc[c].id, client[loc[c].id_cliente-1].nome, client[loc[c].id_cliente-1].id, film[loc[c].id_filme-1].titulo, loc[c].dt_saida.tm_mday, loc[c].dt_saida.tm_mon+1, loc[c].dt_saida.tm_year+1900, film[loc[c].id_filme-1].valor_diario);
	printf("\nEncerrar locação? (1)Sim (2) Não -> ");
	scanf("%d", &d);
	if (d==1){
		hora();
		loc[c].dt_entrada = d_atual; // carrega hora atual
		calcValor(c);
		film[(loc[c].id_filme-1)].status = 0; // flag filme locado
	}
	arqLocacoes();
	arqFilmes();
}
else{
	printf("\nLocação não localizada!");
}
limparb();
continuar();
return 10;
}

/*                     main           */

void main(){

int menu = 1; // menu selecionado
	setlocale(LC_ALL, "Portuguese");
	memset(client, 0, sizeof (client));
	memset(film,0,sizeof (film));
	memset(loc,0,sizeof (loc));
	carArquivos();
	inicio();
	menu = menu1();
	while (menu>-1) {
		switch (menu){
			case 1: // ****************************Filmes
				menu = 10;
				while(menu>0){
					switch (menu){ // Navegação Filmes	
						case 1: // Novo
							menu = menu111();
							break;
						case 2: // Pesquisa de filmes	
							menu = menu112();
							break;
						case 3:
							menu = menu113();
							break;
						case 10:
							menu = menu11(); // Menu filmes
							break;
     					}
    				}
				break; // **************************Filmes

  			case 2: //**************************Relatórios
   				menu=10;
  				while(menu>0){
					switch (menu){ // Navegação relatórios
						case 1: //Todos os filmes cadastrados
							menu = menu121();
							break;
						case 2:// Todos os clientes cadastrados
							menu = menu122();
							break;
						case 3:// Filmes disponíveis
							menu = menu123();
							break;
						case 4:// Filmes locados
							menu = menu124();
							break;
						case 10: // menu navegação relatórios
							menu = menu12();
							break;//
					}
				}
				break; //******************************Relatório

			case 3: //**************************cadastro de cliente
				menu=10;
				while(menu>0){
					switch (menu){ // Navegação clientes
						case 1: //Novo cliente
							menu = menu131();
							break;
						case 2:// Pesquisa cliente
							menu = menu132();
							break;
						case 10: // menu cadastro de cliente
							menu = menu13();
							break;
					}
				}
				break; //******************************cadastro de cliente

			case 4: //************************** Locação
				menu =10; 
				while(menu>0){
					switch (menu){ // Navegação clientes
						case 1: //Nova locacao
							menu = menu141();
							break;
						case 2://Pesquisa Locacao
							menu = menu142();
							break;
						case 3://encerra locacao
							menu = menu143();
							break;
						case 10: // menu Locações
							menu = menu14();
							break;
					}
				} //************************** Locação
			default:
				menu = menu1();
				break;	
		}
	}
}









