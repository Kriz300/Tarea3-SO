/*-carpeta:
	-next
	-child(archivos):

-archivo:
	-causa: string
	-archivado: int
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

//--------------STRUCTS--------------
typedef struct nodo{
	char* nombre;
	char* path;
	struct nodo* adyacente;
	struct nodo* hijo;			// NULL => si es archivo, dado que no puede poseer "hijos"
	unsigned int tipo;			// 0 => carpetas | 1 => archivos
} nodo_t;

typedef struct cabezera{
	nodo_t* raiz;
	nodo_t* curr;
} cabezera_t;

cabezera_t * cabeza;
char cwd[1024];

//-----------------------------------

int menu_principal(){
	int numero0;
	printf("------MENU PRINCIPAL------\n");
	printf("Opciones:\n");
	printf("0-Salir\n");	//LISTO
	printf("1-Ver directorio actual\n");	//LISTO
	printf("2-Moverse a un directorio\n");
	printf("3-Crear Archivo o Carpeta\n");	//LISTO
	printf("4-Ver causa\n");			//LISTO
	printf("5-Modificar nombre\n");		//LISTO
	printf("6-Eliminar\n");				//LISTO
	printf("7-Mover archivo o carpeta\n");
	printf("8-Archivar\n");
	printf("Ingrese su opcion: ");
	scanf("%d", &numero0);
	return numero0;
}

void incializar(){
	char* nombre = "root";
	nodo_t* aux;
	aux = (nodo_t *) malloc(sizeof(nodo_t));
	aux->nombre = nombre;
	aux->path = "~\\root";
	aux->adyacente = NULL;
	aux->hijo = NULL;
	aux->tipo = 0;
	cabeza->raiz = aux;
	cabeza->curr = aux;
	return;
}

void ver_causa(){
	char nombre1[15];
	int flag = 1; 
	printf("Ingrese el nombre de la causa a revisar:");
	scanf("%s", nombre1);
	while(flag){
		if(strcmp(cabeza->curr->nombre, nombre1) == 0){
			printf("Nombre: %s", cabeza->curr->nombre);
			printf("Causa: Mostrando causa...\n");
			flag = 0;
		}
		else if(cabeza->curr->adyacente == NULL){
			printf("No se encontro la causa :c\n");
			flag = 0;
		}
		cabeza->curr = cabeza->curr->adyacente;
	}
	printf("presione enter para continuar\n");
	while(getchar()!='\n');
	getchar();
}

void mostrar_directorio(){
	printf("%s\n",cabeza->curr->nombre);
	printf("%s\n",cabeza->curr->path);
	printf("presione enter para continuar\n");
	while(getchar()!='\n');
	getchar();
}

void modificar_nombre(){
	char nombre_despues[15];
	char nombre_anterior[15];
	int flag = 1;
	printf("Indique el nombre del archivo a cambiar el nombre: \n");
	scanf("%s", nombre_anterior);
	printf("Indique el nombre al cual quiere cambiar: \n");
	scanf("%s", nombre_despues);
	while(flag){
		if(cabeza->curr->nombre == nombre_anterior){
			cabeza->curr->nombre = nombre_despues;
			flag = 0;
		}
		else if(cabeza->curr->adyacente == NULL){
			printf("No se encontrÃ³ el nombre :c\n");
			flag = 0;
		}
		cabeza->curr = cabeza->curr->adyacente;
	}	
}

void moverse_a_directorio(){ //pendiente
	char directorio[15];
    printf("escriba el nombre del directorio\n");
    scanf("%s", directorio);
    printf("directorio: %s\n",directorio);
    char * ruta = strtok(directorio, "\\");
	nodo_t* aux;
	nodo_t* aux2;
	nodo_t* principio;
	principio = cabeza->curr;
	printf("cabeza->curr->nombre: %s\n", cabeza->curr->hijo->nombre);
	if (strcmp(ruta, cabeza->raiz->nombre) == 0)
	{
		aux = cabeza->raiz->hijo;
	}
	else{
		aux = cabeza->curr->hijo;
	}
	while(ruta != NULL){
		if (strcmp(aux->nombre, ruta) == 0)
		{
			cabeza->curr = aux->hijo;
			aux = cabeza->curr;
			printf("pasa\n");
		}

		else if (aux->adyacente == NULL)
		{
			printf("No se encontro la ruta\n");
			break;
		}
		else{
			aux2 = aux->adyacente;
			while(1)
	        {
	            if(strcmp(aux2->nombre, ruta)==0)
	            {
	            	cabeza->curr = aux2->hijo;
					aux = cabeza->curr;
					printf("if 1\n");
					break;
	            }
	            else if (aux2->adyacente != NULL)
	            {
	                aux = aux2;
	                aux2 = aux->adyacente;
	                printf("if 2\n");
	            }
				else if (aux2->adyacente == NULL)
				{
					printf("No se encontro la ruta");
					cabeza->curr = principio;
					break;
				}
	        }
	        printf("fuera de while true\n");	
		}
		
        
		ruta = strtok(NULL, " ");
	}
	printf("presione enter para continuar\n");
	while(getchar()!='\n');
	getchar();
}

void generar_archivo(){
	char nombre[15];
	nodo_t* aux;
	char ruta[1000];
	char slash[100] = "\\";
	strcpy(ruta, cabeza->curr->path);
	strcat(ruta,strcat(slash,nombre));
	printf("nombre del archivo:");
	scanf("%s", nombre);
	aux = (nodo_t *) malloc(sizeof(nodo_t));
	aux->nombre = nombre;
	aux->path = ruta;
	aux->adyacente = cabeza->curr->hijo;
	cabeza->curr->hijo = aux;
	aux->hijo = NULL;
	aux->tipo = 1;
	printf("Archivo generado <3\n");
	printf("presione enter para continuar\n");
	while(getchar()!='\n');
	getchar();
}

void generar_carpeta(){
	char nombre[15];
	printf("nombre de carpeta: ");
	scanf("%s", nombre);
	char ruta[1000];
	char slash[100] = "\\";
	strcpy(ruta, cabeza->curr->path);
	strcat(ruta,strcat(slash,nombre));
	nodo_t* aux;
	aux = (nodo_t *) malloc(sizeof(nodo_t));
	aux->nombre = nombre;
	aux->path = ruta;
	aux->adyacente = cabeza->curr->hijo;
	cabeza->curr->hijo = aux;
	printf("nombre: %s\n", cabeza->curr->hijo->nombre);
	printf("aux nombre: %s\n", aux->nombre);
	aux->hijo = NULL;
	aux->tipo = 0;
	printf("Carpeta generada <3\n");
	printf("presione enter para continuar\n");
	while(getchar()!='\n');
	getchar();
}

void crear(){
	int numero0;
	printf("\n------MENU CREACION------\n");
	printf("Opciones:\n");
	printf("1-Archivo\n");
	printf("2-Carpeta\n");
	scanf("%d", &numero0);
	if (numero0 == 1)
	{
		generar_archivo();
	}
	else if (numero0 == 2)
	{
		generar_carpeta();
	}
	else{
		printf("Error \n");
	}
}

void anidados(nodo_t* nodo){ 
	if (nodo->hijo == NULL)
	{
		return;
	}
	nodo_t* tmp = nodo->hijo;
	if (tmp->adyacente != NULL)
	{
		int flag = 1;
		nodo_t* tmp2 = nodo->hijo->adyacente;
		while(flag == 1)
		{
			if(tmp2->adyacente != NULL)
			{
				tmp->adyacente = tmp2->adyacente;
				free(tmp2);
			}
			else {
				tmp->adyacente = NULL;
				free(tmp2);
				flag = 0;
			}
		}
	}
	else {
		free(tmp);
		return;
	}
	return;
}

void eliminar(){ 
	char name[15];
	scanf("%s", name);
	if (cabeza->curr == NULL)
	{
		printf("La carpeta/archivo no existe\n");
		return;
	}
	nodo_t* tmp = cabeza->curr;
	if (tmp->adyacente != NULL)
	{
		nodo_t* tmp2 = cabeza->curr->adyacente;
		while(1)
		{
			if(tmp2->nombre == name)
			{
				if(tmp2->adyacente != NULL)
				{
					tmp->adyacente = tmp2->adyacente;
					anidados(tmp2);
					free(tmp2);
					printf("La carpeta/archivo fue eliminada correctamente\n");
					return;
				}
				else {
					tmp->adyacente = NULL;
					anidados(tmp2);
					free(tmp2);
					printf("La carpeta/archivo fue eliminada correctamente\n");
					return;
				}
				
			}
			else if (tmp2->adyacente != NULL)
			{
				tmp = tmp2;
				tmp2 = tmp->adyacente;
			}
			else {
				printf("La carpeta/archivo no existe\n");
				return;
			}
		}
	}
	else if (tmp->nombre == name)
	{
		cabeza->curr = NULL;
		cabeza->raiz = NULL;
		anidados(tmp);
		free(tmp);
		printf("La carpeta/archivo fue eliminada correctamente\n");
		return;
	}
	printf("La carpeta/archivo no existe\n");
}

void archivar(){
}

void mover(){
}

int main(){
	cabeza = (cabezera_t *)malloc(sizeof(cabezera_t));
	cabeza -> raiz = NULL;
	cabeza -> curr = NULL;
	incializar();
	int numero1;
	int flag = 1;
	while(flag){
		numero1 = menu_principal();
		switch(numero1){
			case 0:
				flag = 0;
				break;

			case 1:
				mostrar_directorio();
				break;

			case 2:
				moverse_a_directorio();
				break;

			case 3:
				crear();
				break;

			case 4:
				ver_causa();
				break;

			case 5:
				modificar_nombre();
				break;

			case 6:
				eliminar();
				break;

			case 7:
				mover();
				break;

			case 8:
				archivar();
				break;
		}
		system("cls");
	}
	printf("Adios\n");
	return 0;
}

/*
	Root
	  I-->casos
	  I 	I------>archivo2
	  I
	  I-->archivo1
*/
