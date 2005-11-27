# include "stdio.h"
# include "stdlib.h"
# include "string.h"
# include "conio.h"

// tamaño de la cadena mas larga
# define MAX_NOMBRE			1024
// numero maximo de lineas a ver por pagina
# define LINEAS_POR_PAGINA	5
// numero maximo de secciones o destinos
# define MAX_SECCION		10

/*
	estrucutura con la definicion de los datos del visitante.
	typedef struct _DATOS {

		char *m_szCedula;
		char *m_szNombre;
	} DATOS, *PDATOS;
	m_szCedula:
		cedula del visitante.
	m_szNombre: 
		nombre del visitante.
*/

typedef struct _DATOS {

	char *m_szCedula;
	char *m_szNombre;
} DATOS, *PDATOS;

/*
	estructura con la cola que contiene los datos de los usuarios.
	typedef struct _COLA {
		
		DATOS	*datos;
		struct _COLA *m_pSig;
	} COLA, *PCOLA;
		datos:
			estrutura con los datos del usuario.
		m_pSig:
			Siguiente dato en la cola.	
*/

typedef struct _COLA {
	
	DATOS	*datos;
	struct _COLA *m_pSig;
} COLA, *PCOLA;

/*
	Libera la memoria usuada por la estrutura datos.
	void FreeDatos(PDATOS &datos);
	parametros:
		datos[salida]:
			estrutura con los datos.
	comentarios:
		esta funcion siempre libera la memoria la varible datos y la devuelve con el valor vacio(NULL).
*/

void FreeDatos(PDATOS &datos) {

	if( datos) {
		if( datos->m_szCedula)
			free(datos->m_szCedula);
		if( datos->m_szNombre)
			free(datos->m_szNombre);
		free(datos);
		datos=NULL;
	}
}

/*
	Copia los datos de una estructura a otra.
	void CopyDatos(PDATOS &destino, const PDATOS nuevo);
	parametros:
		destino[salida]:
			destino hacia donde se va a copiar la estructura nuevo.
		nuevo[entrada]:
			datos a copiar.
	comentarios:
		si la estructura destino no esta vacia, se limpia memoria antes de ser copiados.
*/

void CopyDatos(PDATOS &destino, const PDATOS nuevo) {
	
	if( !nuevo)
		return;
	if( !destino) {
		if(!(destino=(PDATOS)malloc(sizeof(DATOS))))
			return;
	} else {
		if( destino->m_szCedula)
			free(destino->m_szCedula);
		if( destino->m_szNombre)
			free(destino->m_szNombre);
	}
	destino->m_szCedula= NULL;
	destino->m_szNombre= NULL;
	if( nuevo->m_szCedula) {
		if(!(destino->m_szCedula= (char *) malloc(strlen(nuevo->m_szCedula)+1)))
			return;
		strcpy(destino->m_szCedula, nuevo->m_szCedula);
	}
	if( nuevo->m_szNombre) {
		if(!(destino->m_szNombre= (char *) malloc(strlen(nuevo->m_szNombre)+1)))
			return;
		strcpy(destino->m_szNombre, nuevo->m_szNombre);
	}
}

/*
	Ingresa nuevos datos al final de la cola.
	PCOLA Push(PCOLA &cola, const PDATOS nuevo);
    parametros:
		cola[entrada/salida]:
			cola donde se almacenara el nuevo dato.
		nuevo[entrada]:
			datos a ingresar en la cola.
	retorno:
		la funcion devuelve un apuntador tipo PCOLA a la posicion del ultimo elemento ingresado.
*/

PCOLA Push(PCOLA &cola, const PDATOS nuevo) {
	
	if( !cola) {
		if(!(cola=(PCOLA)malloc(sizeof(COLA))))
			return NULL;
		cola->m_pSig=NULL;
		cola->datos= NULL;
		CopyDatos(cola->datos, nuevo);
		return cola;
	}
	PCOLA *info= &cola->m_pSig;
	while( *info)
		info= &(*info)->m_pSig;
	return Push(*info, nuevo);
}

/*
	Libera la memoria usada por la cola
	void FreeCola(PCOLA &cola)
	parametros:
		cola[entrada]:
			limpia el contenido completo de la cola.
	comentarios:
		tambien librera la memoria usuada por la estructura datos dentro de la cola.
*/

void FreeCola(PCOLA &cola) {
	
	if( cola) {
		PCOLA info= cola, aux=NULL;
		while( info) {
			aux= info->m_pSig;
			FreeDatos(info->datos);
			free(info);
			info=aux;
		}
		cola= NULL;
	}
}

/*
	Ingresa un nuevo dato a la cola.
	PCOLA Pushc(PCOLA &cola,char *nombre,char *cedula=NULL);
	parametros:
		cola[entrada/salida]:
			estructura que contiene la cola.
		nombre[entrada]:
			nombre del usuario.
		cedula[entrada]:
			cedula del usuario.
	retorno:
		la funcion devuelve un apuntador tipo PCOLA a la posicion del ultimo elemento ingresado.
*/

PCOLA Pushc(PCOLA &cola,char *nombre,char *cedula=NULL) {

	DATOS dato;
	dato.m_szCedula= cedula;
	dato.m_szNombre= nombre;
	return Push(cola, &dato);
}

/*
	Elimina el dato superior de la cola.
	void Pop(PCOLA &cola);
	parametros:
		cola[entrada/salida]:
			estructura con la cola.
*/

void Pop(PCOLA &cola) {

	if( !cola)
		return;
	PCOLA *ant= &cola;
	PCOLA info= cola->m_pSig;
	while( info) {
		
		CopyDatos((*ant)->datos, info->datos);
		ant= &(*ant)->m_pSig;
		info=info->m_pSig;
	}
	FreeCola(*ant);
	return ;
}

/*
	Elimina un elemento especifico de la cola.
	void DeleteAt(PCOLA &cola, int index);
	Parametros:
		cola[entrada/salida]:
			estructura con la cola.
		index[entrada]:
			indice con el numero del elemento a eliminar.
*/

void DeleteAt(PCOLA &cola, int index) {
	
	PCOLA *info= &cola;
	int i=0;

	while( *info) {

		if( i==index) {
			Pop(*info);
			break;
		}
		i++;
		info= &(*info)->m_pSig;
	}
}

/*
	Enumera la cantidad de elementos de la cola
	int GetCount(const PCOLA cola)
	parametros:
		cola[entrada]:
			estructura con la cola.
	retorno:
		un entero con la cantidad de elementos de la cola.
*/

int GetCount(const PCOLA cola) {
	
	int i=0;
	PCOLA info= cola;

	while(info) {
		i++;
		info= info->m_pSig;
	}
	return i;
}

/*
	Obtiene un elemento de la cola.
	PCOLA GetAt(const PCOLA cola, int index) 
	parametros:
		cola[entrada]:
			estructura con la cola.
		index[entrada]:
			indice con el elemento a buscar.
	retorno:
		si el elemento existe el apuntador hacia el elemento, de otra forma
		NULL.
*/

PCOLA GetAt(const PCOLA cola, int index) {

	int i=0;
	PCOLA info= cola;

	while(info) {
		if(i==index)
			return info;
		i++;
		info= info->m_pSig;
	}
	return NULL;
}

/*
	Obtiene los datos de un elemento de la cola.
	PDATOS GetDatosAt( const PCOLA cola, int index);
	Parametros:
		cola[entrada]:
			estructura con la cola.
		index[entrada]:
			indice con el elemento a buscar.
	retorno:
		si el elemento existe el apuntador hacia el elemento, de otra forma
		NULL.
	comentarios:
		esta funcion es similar a "GetAt" solo que no devuelve el elemento de la
		cola, si no los datos del elemento.
*/

PDATOS GetDatosAt( const PCOLA cola, int index) {
	
	PCOLA info= GetAt(cola, index);
	if( info) 
		return info->datos;
	return NULL;
}

/*
	Despliega un menu para interaccion con el usuario.
	int ShowMenu(const PCOLA menu, char *titulo, char cancelar=0);
	parametros:
		menu[entrada]:
			una estructura tipo cola con el menu.
		titulo[entrada]:
			el titulo del menu.
		cancelar[entrada]:
			si se puede cancelar dentro del menu.
	retorno:
		la opcion seleccionada por el usuario, comenzando desde uno hasta
		el numero de elementos de la cola.
	comentarios:
		este menu solo despliega de la estructura datos la variable "m_szNombre".
		Y no sale hasta que se seleccione un valor valido.
*/

int ShowMenu(const PCOLA menu, char *titulo, char cancelar=0) {

	PDATOS info;
	printf( "\n%s\n\n", titulo);
	int max=0;
	for( int i=0; i<GetCount(menu); i++,max++) {
		info= GetDatosAt(menu, i);
		printf("%d. %s\n", max+1, info->m_szNombre);
	}
	if( cancelar) {
		printf("%d. Cancelar\n", max+1);
		max++;
	}
	printf("\n");
	char opcion[255];
	int nopcion;
	do {
		printf("Seleccione una opcion: ");
		fgets(opcion, 255, stdin);
		nopcion=atoi(opcion);
	} while(nopcion<1 || nopcion>max);
	printf("\n");
	return nopcion;
}

/*
	Muestra los datos del usuario.
	void ShowUsuario(PDATOS datos);
	parametros:
		datos[entrada]:
			datos del usuario a desplegar.
*/

void ShowUsuario(PDATOS datos) {

	if( datos) {
		printf("Cedula: %s\n", datos->m_szCedula);
		printf("Nombre: %s\n", datos->m_szNombre);
	}
}

/*
	Carga las dependencias desde un archivo.
	void LoadDependencias(const char *archivo, PCOLA &pCola);
	parametros:
		archivo[entrada]:
			nombre del archivo a abrir.
		pCola[salida]:
			cola destino de los datos.
	comentarios:
		la estructura pCola se vacia completamente, antes de realizar
		la carga del archivo, solo se carga a la variable "m_szNombre".
*/

void LoadDependencias(const char *archivo, PCOLA &pCola) {

	FILE *f;
	char nombre[MAX_NOMBRE];
	FreeCola(pCola);
	if(!(f= fopen(archivo, "r"))) {
		return;
	}

	while(!feof(f)) {

		if( fgets(nombre, MAX_NOMBRE, f) && strlen(nombre)) {
			nombre[strlen(nombre)-1]='\0';
			Pushc(pCola, nombre);
		}
	}
	fclose(f);
}

/*
	Almacena las dependencias en un archivo.
	void StoreDependencias( const char *archivo, const PCOLA cola);
	parametros:
		archivo[entrada]:
			nombre del archivo a abrir
		cola[entrada]:
			cola que se va a almacenar.
	comentarios:
		el archivo se vacia completamente antes de ser guardado.
		solo se almacena de la estructura datos la variable "m_szNombre".
*/

void StoreDependencias( const char *archivo, const PCOLA cola) {

	FILE *f;
	PDATOS datos;

	if(!(f= fopen(archivo, "w"))) {
		return;
	}

	for( int i=0; i<GetCount(cola); i++) {

		datos= GetDatosAt(cola, i);
		fprintf(f, "%s\n", datos->m_szNombre);
	}
	fclose(f);
}

int main()
{
	PCOLA pCola[MAX_SECCION];
	PCOLA menu=NULL;
	PCOLA menuDependencias=NULL;
	PCOLA pDependencias= NULL;
	char nombre[MAX_NOMBRE];
	char cedula[MAX_NOMBRE];
	int pos=0,i;

	for(i=0; i<MAX_SECCION; i++)
		pCola[i]= NULL;

	// cargamos las dependencias
	LoadDependencias("dep.txt", pDependencias);

	// creamos el menu principal
	Pushc(menu, "Ingresar un nuevo visitante");
	Pushc(menu, "Siguiente Turno");
	Pushc(menu, "Listar Turnos");
	Pushc(menu, "Dependencias");
	Pushc(menu, "Salir");

	// creamos el menu de dependencias
	Pushc(menuDependencias, "Ingresar");
	Pushc(menuDependencias, "Eliminar");
	Pushc(menuDependencias, "Listar");


	while((pos=ShowMenu(menu, "Menu Principal"))<5) {

		switch(pos) {
			case 1: // ingresar un nuevo usuario
				if( !pDependencias) { // no hay dependencias
					printf("No hay dependencias creelas en la seccion dependencias\n");
					break;
				}
				printf("Ingrese la cedula del usuario: ");
				fgets(cedula, MAX_NOMBRE, stdin);
				if( cedula[0]=='\n')
					break;
				cedula[strlen(cedula)-1]='\0';
				printf("Ingrese el nombre del usuario: ");
				fgets(nombre, MAX_NOMBRE, stdin);
				if( nombre[0]=='\n')
					break;
				nombre[strlen(nombre)-1]='\0';
				pos= ShowMenu(pDependencias, "Destino", 1)-1;
				if( pos==GetCount(pDependencias)) // se selecciono cancelar.
					break;
		Pushc(pCola[pos], nombre, cedula);
				break;
			case 2: // sacar un nuevo visitante
				if( !pDependencias) { // no hay dependencias
					printf("No hay dependencias creelas en la seccion dependencias\n");
					break;
				}
				pos= ShowMenu(pDependencias, "Destino", 1)-1;
				if( pos==GetCount(pDependencias)) // se selecciono cancelar.
					break;
				if( !pCola[pos]) {
					printf("No hay usuarios para este destino\n");
					break;
				}
				ShowUsuario(pCola[pos]->datos);
				printf("¿Esta seguro de seleccionar a este usuario?(s/n): "); // confirmacion para eliminar
				fgets(nombre, MAX_NOMBRE, stdin);
				if( nombre[0]=='s' || nombre[0]=='S')
					Pop(pCola[pos]);
				break;
			case 3: // listar todos los visitantes
				if( !pDependencias) { // no hay dependencias
					printf("No hay dependencias creelas en la seccion dependencias\n");
					break;
				}
				pos= ShowMenu(pDependencias, "Destino", 1)-1;
				if( pos==GetCount(pDependencias))  // se selecciono cancelar.
					break;
				if( !pCola[pos]) {
					printf("No hay usuarios para este destino\n");
					break;
				}
				for( i=0; i<GetCount(pCola[pos]); i++) {
					if( i && i%LINEAS_POR_PAGINA==0) {
						// en caso de que se supere la cantidad maxima de lineas.
						// solo para visibilidad.
						printf("\nPresione Enter para continuar o 'q' para terminar...\n");
						fgets(nombre, MAX_NOMBRE, stdin);
						if( nombre[0]=='q' || nombre[0]=='Q')
							break;
					}
					printf("\n***Turno %d****\n\n", i+1);
					ShowUsuario(GetDatosAt(pCola[pos], i));
				}
				break;
			case 4: // dependencias
				// desplegamos el submenu dependencias.
				while( (pos=ShowMenu(menuDependencias, "Dependencias", 1))<4) {
					switch(pos) {
						case 1: // ingresar una nueva
							if( GetCount(pDependencias)>MAX_SECCION) {
								printf("Numero maximo de dependencias");
								break;
							}
							printf("Ingrese el nombre de la dependencia: ");
							fgets(nombre, MAX_NOMBRE, stdin);
							if( nombre[0]=='\n')
								break;
							nombre[strlen(nombre)-1]='\0';
							Pushc(pDependencias, nombre);
							break;
						case 2: // eliminar una dependencia
							pos= ShowMenu(pDependencias, "Dependencia a eliminar", 1)-1;
							if( pos==GetCount(pDependencias))
								break;
							DeleteAt(pDependencias, pos);
							break;
						case 3: // listar dependencias
							printf("**Dependencias**\n\n");
							for( i=0; i<GetCount(pDependencias); i++) {
								printf("%d. %s\n", i+1, GetDatosAt(pDependencias, i)->m_szNombre);
							}
							printf("\n");
							break;
					}
				}
				break;
		}
	}
	// guardamos las dependencias
	StoreDependencias("dep.txt", pDependencias);
	// liberamos toda la memoria
	for(i=0; i<MAX_SECCION; i++)
		FreeCola(pCola[i]);
	FreeCola(pDependencias);
	// tambien el de los menus.
	FreeCola(menu);
	FreeCola(menuDependencias);
	return 0;
}
