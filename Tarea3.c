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
	char nombre[20];
	char path[30];
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
	printf("2-Moverse a un directorio\n"); //LISTO
	printf("3-Crear Archivo o Carpeta\n");	//LISTO
	printf("4-Ver causa\n");			
	printf("5-Modificar nombre\n");		
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
	strcpy(aux->nombre, nombre);
	strcpy(aux->path, "~\\root");
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
		if(strcmp(cabeza->curr->nombre, nombre_anterior)==0){
			strcpy(cabeza->curr->nombre, nombre_despues);
			flag = 0;
		}
		else if(cabeza->curr->adyacente == NULL){
			printf("No se encontró el nombre :c\n");
			flag = 0;
		}
		cabeza->curr = cabeza->curr->adyacente;
	}	
}

void moverse_a_directorio(char* directorio){

    printf("directorio: %s\n",directorio);
    char * ruta = strtok(directorio, "\\");
    nodo_t* aux;
    nodo_t* aux2;
    nodo_t* principio;
    principio = cabeza->curr;


    if (strcmp(ruta, cabeza->raiz->nombre) == 0)
    {
        aux = cabeza->raiz;
    }
    else{
        if (cabeza->curr->hijo == NULL)
        {
        	printf("No se encontro la ruta\n");
        	printf("presione enter para continuar\n");
    		while(getchar()!='\n');
	    	getchar();
            return;
        }
        aux = cabeza->curr->hijo;
    }
    int flag = 0;
    while(ruta != NULL){
        if (strcmp(aux->nombre, ruta) == 0 && aux->tipo != 1)
        {
            cabeza->curr = aux;
            if (cabeza->curr->hijo == NULL)
            {
                break;
            }
            aux = cabeza->curr->hijo;
        }

        else if (aux->adyacente == NULL || flag == 1)
        {
            cabeza->curr = principio;
            printf("No se encontro la ruta\n");
            break;
        }
        else{
            aux2 = aux->adyacente;
            while(1)
            {
                if(strcmp(aux2->nombre, ruta)==0  && aux->tipo != 1)
                {
                    cabeza->curr = aux2;
                    if (cabeza->curr->hijo == NULL)
                    {
                        flag = 1;
                        break;
                    }
                    aux = cabeza->curr->hijo;
                    break;
                }
                else if (aux2->adyacente == NULL)
                {
                    printf("No se encontro la ruta");
                    cabeza->curr = principio;
                    break;
                }
                aux = aux2;
                aux2 = aux->adyacente;
            }   
        }
        ruta = strtok(NULL, " ");
    }
    printf("nombre: %s\n", cabeza->curr->nombre);
    printf("path actual: %s\n", cabeza->curr->path);
    printf("presione enter para continuar\n");
    while(getchar()!='\n');
    getchar();
}

void generar_archivo(char* nombre, char* ruta){
	nodo_t* aux;
	char slash[100] = "\\";
	strcpy(ruta, cabeza->curr->path);
	strcat(ruta,strcat(slash,nombre));
	printf("ruta: %s\n", ruta);
	printf("nombre del archivo:");
	scanf("%s", nombre);
	aux = (nodo_t *) malloc(sizeof(nodo_t));
	strcpy(aux->nombre, nombre);
	strcpy(aux->path, ruta);
	aux->adyacente = cabeza->curr->hijo;
	cabeza->curr->hijo = aux;
	aux->hijo = NULL;
	aux->tipo = 1;
	printf("Archivo generado <3\n");
	printf("presione enter para continuar\n");
	while(getchar()!='\n');
	getchar();
}

void generar_carpeta(char* nombre, char* ruta){
	char slash[100] = "\\";
	strcpy(ruta, cabeza->curr->path);
	strcat(ruta,strcat(slash,nombre));
	printf("ruta: %s\n", ruta);
	nodo_t* aux;
	aux = (nodo_t *) malloc(sizeof(nodo_t));
	strcpy(aux->nombre, nombre);
	strcpy(aux->path, ruta);
	aux->adyacente = cabeza->curr->hijo;
	cabeza->curr->hijo = aux;
	aux->hijo = NULL;
	aux->tipo = 0;
	printf("Carpeta generada <3\n");
	printf("presione enter para continuar\n");
	while(getchar()!='\n');
	getchar();
}

void anidados(nodo_t* nodo){ 
	if (nodo->hijo == NULL)
	{
		return;
	}
	nodo_t* tmp = nodo->hijo;
	nodo_t* tmp2;
	if (tmp->adyacente != NULL)
	{
		int flag = 1;
		tmp2 = tmp->adyacente;
		while(flag == 1)
		{
			if(tmp2->adyacente != NULL)
			{
				tmp->adyacente = tmp2->adyacente;
				anidados(tmp2);
				free(tmp2);
			}
			else {
				tmp->adyacente = NULL;
				anidados(tmp2);
				free(tmp2);
				flag = 0;
			}
		}
	}
	else {
		anidados(tmp);
		return;
	}
	return;
}

void eliminar(){ 
	char name[15];
	printf("nombre del elemento a eliminar: ");
	scanf("%s", name);
	if (cabeza->curr == NULL || cabeza->curr->hijo == NULL)
	{
		printf("La carpeta/archivo no existe\n");
		printf("presione enter para continuar\n");
		while(getchar()!='\n');
		getchar();
		return;
	}
	nodo_t* tmp = cabeza->curr->hijo;
	if (tmp->adyacente != NULL)
	{
		nodo_t* tmp2 = tmp->adyacente;
		while(1)
		{
			if(strcmp(name, tmp2->nombre) == 0)
			{
				if(tmp2->adyacente != NULL)
				{
					tmp->adyacente = tmp2->adyacente;
					anidados(tmp2);
					free(tmp2);
					printf("La carpeta/archivo fue eliminada correctamente\n");
					printf("presione enter para continuar\n");
					while(getchar()!='\n');
					getchar();
					return;
				}
				else {
					tmp->adyacente = NULL;
					anidados(tmp2);
					free(tmp2);
					printf("La carpeta/archivo fue eliminada correctamente\n");
					printf("presione enter para continuar\n");
					while(getchar()!='\n');
					getchar();
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
				printf("presione enter para continuar\n");
				while(getchar()!='\n');
				getchar();
				return;
			}
		}
	}
	else if (strcmp(name, tmp->nombre) == 0)
	{
		anidados(tmp);
		free(tmp);
		printf("La carpeta/archivo fue eliminada correctamente\n");
		printf("presione enter para continuar\n");
		while(getchar()!='\n');
		getchar();
		return;
	}
	printf("La carpeta/archivo no existe\n");
	printf("presione enter para continuar\n");
	while(getchar()!='\n');
	getchar();
}

void archivar(){
}

void mover(char* directorio, char* nombre, char* ruta){
	char slash[100] = "\\";
	nodo_t* primero = cabeza->curr;//hola
	nodo_t* aux, *tmp, * aux2;
	nodo_t *ant = NULL;
	tmp = cabeza->curr;
	if (cabeza->curr->hijo == NULL)
	{
		printf("No existe el archivo \n");
		printf("presione enter para continuar\n");
		while(getchar()!='\n');
		getchar();
		return;
	}
	aux = cabeza->curr->hijo;
	if (strcmp(aux->nombre, nombre) != 0)
	{
		if (aux->adyacente != NULL)
		{
			aux2 = aux->adyacente;
			ant = aux;
			while(1)
			{
				if (strcmp(aux2->nombre, nombre) == 0)
				{
					cabeza->curr = aux2;
					break;
				}
				else if (aux2->adyacente == NULL)
				{
					printf("No existe el archivo \n");
					printf("presione enter para continuar\n");
					while(getchar()!='\n');
					getchar();
					return;
				}
				ant = aux2;
				aux2 = ant->adyacente;
			}
		}
		else
		{
			printf("No existe el archivo \n");
			printf("presione enter para continuar\n");
			while(getchar()!='\n');
			getchar();
			return;
		}
	}
	//aux2 = wena
	if (strcmp(aux->nombre, tmp->nombre) == 0 && strcmp(aux->path, tmp->path) == 0)
	{
		printf("Fallo el movimiento \n");
		printf("presione enter para continuar\n");
		while(getchar()!='\n');
		getchar();
		return;
	}
	moverse_a_directorio(directorio);//root
	printf("iniciando \n");
	if (strcmp(tmp->path, cabeza->curr->path) == 0)
	{
		printf("Fallo el movimiento \n");
		printf("presione enter para continuar\n");
		while(getchar()!='\n');
		getchar();
		return;
	}
	else if (cabeza->curr->hijo == NULL)
	{
		if (ant != NULL)
		{
			if (ant->adyacente != NULL)
			{
				ant->adyacente = aux2->adyacente;
			}
			ant->adyacente = NULL;
		}
		printf("chantado \n");
		strcpy(ruta, cabeza->curr->path);
		strcat(ruta,strcat(slash,nombre));
		strcpy(aux->path, ruta);
		cabeza->curr->hijo = aux;
	}
	else {
		printf("else G \n");
		if (aux->adyacente == NULL)
		{
			if (ant != NULL)
			{
				if (ant->adyacente != NULL)
				{
					ant->adyacente = aux2->adyacente;
				}
				ant->adyacente = NULL;
			}
			printf("if \n");
			strcpy(ruta, cabeza->curr->path);
			strcat(ruta,strcat(slash,nombre));
			strcpy(aux->path, ruta);
			aux->adyacente = cabeza->curr->hijo;
			cabeza->curr->hijo = aux;

		}
		else{
			printf("else c \n");
			aux2 = aux->adyacente;
			while(1)
			{
				printf("while \n");
				if (aux2->adyacente == NULL)
				{
					if (ant != NULL)
					{
						if (ant->adyacente != NULL)
						{
							ant->adyacente = aux2->adyacente;
						}
						ant->adyacente = NULL;
					}
					printf("entro \n");
					strcpy(ruta, cabeza->curr->path);
					strcat(ruta,strcat(slash,nombre));
					strcpy(aux2->path, ruta);
					aux2->adyacente = cabeza->curr->hijo;
					cabeza->curr->hijo = aux2;
					break;
				}
				else
				{
					printf("ciclo \n");
					aux = aux2;
                	aux2 = aux->adyacente;
				}
			} 
		}
		
	}
	printf("GG \n");
	cabeza->curr = primero;
	printf("movimiento existoso. \n");
	printf("presione enter para continuar\n");
	while(getchar()!='\n');
	getchar();
	return;
}

int main(){
	cabeza = (cabezera_t *)malloc(sizeof(cabezera_t));
	cabeza -> raiz = NULL;
	cabeza -> curr = NULL;
	incializar();
	int numero1;
	int flag = 1;
	char directorio[15];
	int numero0;
	char nombre[15];
	char ruta[1000];
	
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
				printf("nombre de carpeta: ");
				scanf("%s", directorio);
				moverse_a_directorio(directorio);
				break;

			case 3:
				printf("\n------MENU CREACION------\n");
				printf("Opciones:\n");
				printf("1-Archivo\n");
				printf("2-Carpeta\n");
				scanf("%d", &numero0);
				printf("nombre: \n");
				scanf("%s", nombre);
				if (numero0 == 1)
					{
						generar_archivo(nombre, ruta);
					}
				else if (numero0 == 2)
					{
						generar_carpeta(nombre, ruta);
					}
				else{
					printf("Error \n");
				}
				break;

			case 4:
				ver_causa();
				break;

			case 5:
				printf("nombre de carpeta: ");
				scanf("%s", nombre);
				modificar_nombre(nombre);
				break;

			case 6:
				eliminar();
				break;

			case 7:
				printf("nombre de carpeta/archivo: ");
				scanf("%s", nombre);
				printf("a donde quiere moverlo: ");
				scanf("%s", directorio);
				mover(directorio, nombre, ruta);
				break;

			case 8:
				archivar();
				break;
		}
		system("clear"); // cambiar
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
