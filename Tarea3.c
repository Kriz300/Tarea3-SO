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
	char causa[30];
	struct nodo* adyacente;
	struct nodo* hijo;			// NULL => si es archivo, dado que no puede poseer "hijos"
	unsigned int tipo;			// 0 => carpetas | 1 => archivos
} nodo_t;

typedef struct cabezera{
	nodo_t* raiz;
	nodo_t* curr;
} cabezera_t;

cabezera_t * cabeza;
//char cwd[1024];

//-----------------------------------

int menu_principal(){
	int numero0;
	printf("------MENU PRINCIPAL------\n");
	printf("Opciones:\n");
	printf("0-Salir\n");	//LISTO
	printf("1-Ver directorio actual\n");	//LISTO
	printf("2-Mostrar Elementos\n");
	printf("3-Moverse a un directorio\n"); //LISTO
	printf("4-Crear Archivo o Carpeta\n");	//LISTO
	printf("5-Ver causa\n");			//LISTO
	printf("6-Modificar nombre\n");		//Probar
	printf("7-Modificar causa\n");		//Probar
	printf("8-Eliminar\n");				//LISTO
	printf("9-Mover archivo o carpeta\n"); //LISTO
	printf("Ingrese su opcion: ");
	scanf("%d", &numero0);
	return numero0;
}

void incializar(){
	char* nombre = "root";
	nodo_t* aux;
	aux = (nodo_t *) malloc(sizeof(nodo_t));
	strcpy(aux->nombre, nombre);
	strcpy(aux->path, "~/root");
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
	nodo_t *aux = cabeza->curr->hijo;
	printf("Ingrese el nombre del archivo a revisar:");
	scanf("%s", nombre1);
	while(flag){
		if(strcmp(aux->nombre, nombre1) == 0){
			if (aux->tipo != 1)
			{
				printf("Esta ubicado en una carpeta\n");
				return;
			}
			printf("Nombre: %s\n", aux->nombre);
			printf("causa: %s\n", aux->causa);
			return;
		}
		else if(aux->adyacente == NULL){
			printf("No se encontro la causa :c\n");
			return;
		}
		aux = aux->adyacente;
	}
}

void mostrar_directorio(){
	printf("%s\n",cabeza->curr->nombre);
	printf("%s\n",cabeza->curr->path);
	return;
}

void mostrar_elementos(){ //Probar
	int flag = 1;
	nodo_t *tmp = cabeza->curr->hijo;
	if (tmp == NULL)
	{
		printf("No hay Elementos\n");
		return;
	}

	while(flag){
		if (tmp->adyacente == NULL)
		{
			if (tmp->tipo == 0)
			{
				printf("nombre: %s |", tmp->nombre);
				printf(" Tipo: Carpeta\n");
			}
			else{
				printf("nombre: %s  |", tmp->nombre);
				printf(" Tipo: Archivo\n");
			}
			return;
		}
		if (tmp->tipo == 0)
		{
			printf("nombre: %s  |", tmp->nombre);
			printf(" Tipo: Carpeta\n");
		}
		else
		{
			printf("nombre: %s  |", tmp->nombre);
			printf(" Tipo: Archivo\n");
		}
		tmp = tmp->adyacente;
	}
}

void modificar_nombre(){
	nodo_t *tmp = cabeza->curr->hijo;
	char nombre_despues[15];
	char nombre_anterior[15];
	int flag = 1;
	printf("Indique el nombre del archivo a cambiar el nombre: \n");
	scanf("%s", nombre_anterior);
	printf("Indique el nombre al cual quiere cambiar: \n");
	scanf("%s", nombre_despues);
	while(flag){
		if(strcmp(tmp->nombre, nombre_anterior)==0){
			strcpy(tmp->nombre, nombre_despues);
			flag = 0;
		}
		else if(tmp->adyacente == NULL){
			printf("No se encontró el nombre :c\n");
			flag = 0;
		}
		tmp = tmp->adyacente;
	}
}

void modificar_causa(){
	nodo_t *tmp = cabeza->curr->hijo;
	char causa_despues[30];
	char causa_anterior[30];
	char nombre_archivo[15];
	int flag = 1;
	int decision;
	printf("Ingrese el nombre del archivo a cambiar su causa: \n");
	scanf("%s", nombre_archivo);
	while(flag){
		if(strcmp(tmp->nombre, nombre_archivo)==0){
			if (tmp->tipo != 1)
			{
				printf("Es una carpeta\n");
				return;
			}
			strcpy(causa_anterior,tmp->causa);
			printf("Ingrese la causa nueva: \n");
			scanf("%s", causa_despues);
			strcpy(tmp->causa, causa_despues);
			printf("Causa cambiada \n");
			printf("Desea archivar los cambios? \n");
			printf("1-Si\n");
			printf("2-No\n");
			scanf("%d",&decision);
			if (decision == 1)
			{
				printf("Cambios archivados \n");
				return;
			}
			else if (decision == 2)
			{
				strcpy(tmp->causa, causa_anterior);
				printf("Cambios no archivados\n");
			}
			return;
		}
		else if(tmp->adyacente == NULL){
			printf("no se encontro el nombre del archivo :c\n");
			return;
		}
		tmp = tmp->adyacente;
	}
}

void moverse_a_directorio(char* directorio){

    printf("directorio: %s\n",directorio);
    char * ruta = strtok(directorio, "/");
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
                if(strcmp(aux2->nombre, ruta)==0  && aux2->tipo != 1)
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
                    printf("No se encontro la ruta \n");
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
    return;
}

void generar_archivo(char* nombre, char* ruta, char *causa){
	if (cabeza->curr->tipo != 0)
	{
		printf("Esta ubicado en un archivo\n");
		return;
	}
	nodo_t* aux;
	char slash[100] = "/";
	strcpy(ruta, cabeza->curr->path);
	strcat(ruta,strcat(slash,nombre));
	printf("ruta: %s\n", ruta);
	aux = (nodo_t *) malloc(sizeof(nodo_t));
	strcpy(aux->nombre, nombre);
	strcpy(aux->path, ruta);
	aux->adyacente = cabeza->curr->hijo;
	cabeza->curr->hijo = aux;
	strcpy(aux->causa, causa);
	aux->hijo = NULL;
	aux->tipo = 1;
	printf("Archivo generado <3\n");
	return;
}

void generar_carpeta(char* nombre, char* ruta){
	if (cabeza->curr->tipo != 0)
	{
		printf("Esta ubicado en un archivo\n");
		printf("presione enter para continuar\n");
		while(getchar()!='\n');
		getchar();
	}
	char slash[100] = "/";
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
	return;
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
		return;
	}
	nodo_t* tmp = cabeza->curr->hijo;
	if (tmp->adyacente != NULL)
	{
		if (strcmp(name, tmp->nombre) == 0)
		{
			cabeza->curr->hijo = tmp->adyacente;
			anidados(tmp);
			free(tmp);
			printf("La carpeta/archivo fue eliminada correctamente\n");
			return;
		}
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
	else if (strcmp(name, tmp->nombre) == 0)
	{
		cabeza->curr->hijo = NULL;
		anidados(tmp);
		free(tmp);
		printf("La carpeta/archivo fue eliminada correctamente\n");
		return;
	}
	printf("La carpeta/archivo no existe\n");
	return;
}

void mover(char* directorio, char* nombre, char* ruta){
	char slash[100] = "/";
	nodo_t* primero = cabeza->curr;//hola
	nodo_t* aux, *tmp, * aux2;
	nodo_t *ant = NULL;
	nodo_t *sig = NULL;
	tmp = cabeza->curr;
	if (cabeza->curr->hijo == NULL)
	{
		printf("No existe el archivo \n");
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
					return;
				}
				ant = aux2;
				aux2 = ant->adyacente;
			}
		}
		else
		{
			printf("No existe el archivo \n");
			return;
		}
	}
	//aux2 = wena
	if (strcmp(aux->nombre, tmp->nombre) == 0 && strcmp(aux->path, tmp->path) == 0)
	{
		printf("Fallo el movimiento \n");
		return;
	}
	moverse_a_directorio(directorio);//root
	if (cabeza->curr->tipo != 0)
	{
		printf("El destino no es una carpeta \n");
		cabeza->curr = primero;
		return;
	}
	if (strcmp(tmp->path, cabeza->curr->path) == 0)
	{
		printf("Fallo el movimiento \n");
		cabeza->curr = primero;
		return;
	}
	if (strcmp(aux->nombre, nombre) == 0)
	{
		if (aux->adyacente != NULL)
		{
			printf("test");
			sig = aux->adyacente;
		}
	}
	if (cabeza->curr->hijo == NULL)
	{
		if (ant != NULL)
		{
			if (ant->adyacente != NULL)
			{
				ant->adyacente = aux2->adyacente;
			}
			ant->adyacente = NULL;
		}
		strcpy(ruta, cabeza->curr->path);
		strcat(ruta,strcat(slash,nombre));
		strcpy(aux->path, ruta);
		cabeza->curr->hijo = aux;
	}
	else {
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
			strcpy(ruta, cabeza->curr->path);
			strcat(ruta,strcat(slash,nombre));
			strcpy(aux->path, ruta);
			aux->adyacente = cabeza->curr->hijo;
			cabeza->curr->hijo = aux;
			if (sig != NULL)
			{
				primero->hijo = sig;
			}
			else
			{
				primero->hijo = NULL;
			}
		}
		else{
			aux2 = aux->adyacente;
			while(1)
			{
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
					strcpy(ruta, cabeza->curr->path);
					strcat(ruta,strcat(slash,nombre));
					strcpy(aux2->path, ruta);
					aux2->adyacente = cabeza->curr->hijo;
					cabeza->curr->hijo = aux2;
					if (sig != NULL)
					{
						primero->hijo = sig;
					}
					else
					{
						primero->hijo = NULL;
					}
					break;
				}
				else
				{
					aux = aux2;
                	aux2 = aux->adyacente;
				}
			} 
		}
		
	}
	cabeza->curr = primero;
	printf("movimiento existoso. \n");
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
	char causa[30];
	
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
				mostrar_elementos();
				break;

			case 3:				
				printf("nombre de carpeta: ");
				scanf("%s", directorio);
				moverse_a_directorio(directorio);
				break;

			case 4:
				printf("\n------MENU CREACION------\n");
				printf("Opciones:\n");
				printf("1-Archivo\n");
				printf("2-Carpeta\n");
				scanf("%d", &numero0);
				printf("nombre: \n");
				scanf("%s", nombre);
				if (numero0 == 1)
					{
						printf("Causa: \n");
						scanf("%s", causa);
						generar_archivo(nombre, ruta, causa);
					}
				else if (numero0 == 2)
					{
						generar_carpeta(nombre, ruta);
					}
				else{
					printf("Error \n");
				}
				break;

			case 5:
				ver_causa();
				break;

			case 6:
				modificar_nombre();
				break;

			case 7:
				modificar_causa();
				break;

			case 8:
				eliminar();
				break;

			case 9:
				printf("nombre de carpeta/archivo: ");
				scanf("%s", nombre);
				printf("a donde quiere moverlo: ");
				scanf("%s", directorio);
				mover(directorio, nombre, ruta);
				break;
		}
		printf("presione enter para continuar\n");
		while(getchar()!='\n');
		getchar();
		system("clear");
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
