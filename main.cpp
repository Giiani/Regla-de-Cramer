/*
REGLA DE CRAMER
Abadie Facundo Cesar
Pennisi Gianfranco
*/
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#define ID_MAT_A00 100
#define ID_MAT_A01 101
#define ID_MAT_A02 102
#define ID_MAT_A03 103
#define ID_MAT_A10 110
#define ID_MAT_A11 111
#define ID_MAT_A12 112
#define ID_MAT_A13 113
#define ID_MAT_A20 120
#define ID_MAT_A21 121
#define ID_MAT_A22 122
#define ID_MAT_A23 123
#define ID_MAT_A30 130
#define ID_MAT_A31 131
#define ID_MAT_A32 132
#define ID_MAT_A33 133

#define ID_MAT_b00 200
#define ID_MAT_b01 201
#define ID_MAT_b02 202
#define ID_MAT_b03 203

#define ID_MAT_x00 300
#define ID_MAT_x01 301
#define ID_MAT_x02 302
#define ID_MAT_x03 303

#define ID_DIMENSION        401
#define ID_BOTON_2x2        402
#define ID_BOTON_3x3        403
#define ID_BOTON_4x4        404
#define ID_BOTON_BORRAR     406
#define ID_BOTON_CALCULAR   407
#define ID_BOTON_SALIR      408

#define ID_ESTAT1 501

#define N 4

int dimension=NULL;
int i,j;
double matA[N][N];
double matB[N];
double matX[N];
double det1, det2;
char txt[64], txt2[64];

// Prototipos:
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
void InsertarMenu(HWND);
void InitForm(HWND, WPARAM, LPARAM);

double det_mat_N (double mat[N][N], int dim);
double det_mat_adj (double mat[N][N], int dim, int m, int n);
double mat_adj (double mat[N][N], double mat_aux[N][N], int dim, int m, int n);
double cramer (double mat[N][N], double arr[N], int n, int x);

// Definición de funciones:
//------------------------------------------------
// WinMain: función de inicio del programa
//------------------------------------------------
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    /* Declaración: */
    HWND hwnd;
    MSG mensaje;
    WNDCLASSEX wincl;
    /* Inicialización: */
    /* Estructura de la ventana */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "NUESTRA_CLASE";
    wincl.lpfnWndProc = WinProc;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    /* Usar icono y puntero por defecto */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    /* Registrar la clase de ventana, si falla, salir del programa */
    if(!RegisterClassEx(&wincl))
        return 0;

    hwnd = CreateWindowEx( 0,
                           "NUESTRA_CLASE",
                           "Resolucion de sistemas de ecuaciones lineales mediante la Regla de Cramer",
                           WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                           750, 350,  /* tamaño en píxeles */
                           HWND_DESKTOP,
                           NULL,
                           hThisInstance,
                           NULL );

    InsertarMenu(hwnd);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    /* Bucle de mensajes: */
    while(TRUE == GetMessage(&mensaje, 0, 0, 0))
    {
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }
    return mensaje.wParam;
}

//----------------------------------------------------
// Procesamiento de los mensajes
//----------------------------------------------------
LRESULT CALLBACK WinProc(
    HWND hwnd,     // Manipulador de ventana
    UINT msg,      // Mensaje
    WPARAM wParam, // Parámetro palabra, varía
    LPARAM lParam  // Parámetro doble palabra, varía
)
{
    int i,j;


    switch (msg) /* manipulador del mensaje */
    {
    case WM_CREATE:
        InitForm(hwnd, wParam, lParam);
        break;

    case WM_DESTROY:
        PostQuitMessage(0); /* envía un mensaje WM_QUIT a la cola de mensajes */
        break;

    case WM_COMMAND:
        switch( LOWORD(wParam) )
        {
        case ID_BOTON_2x2:
            dimension=2;break;
        case ID_BOTON_3x3:
            dimension=3;break;
        case ID_BOTON_4x4:
            dimension=4;break;

        case ID_BOTON_CALCULAR:

                if(dimension == NULL)
                {
                    MessageBox(hwnd, "Seleccione Dimension", "Error", MB_OK | MB_ICONERROR);
                    break;
                }


                for(i=0;i<dimension;i++)
                    for(j=0;j<dimension;j++)
                    {
                        GetDlgItemText(hwnd,(100+j*10+i), txt,100);
                        matA[i][j]= atof(txt);
                    }

                for(j=0;j<dimension;j++)
                    {
                        GetDlgItemText(hwnd,(200+j), txt,100);
                        matB[j] = atof(txt);
                    }

                        det2 = det_mat_N(matA,dimension);
                        if(det2 == 0.0)
                        {
                            MessageBox(hwnd, "Determinante = 0", "Error", MB_OK | MB_ICONERROR);
                            for( i=0; i<N; i++)
                            SetDlgItemText(hwnd, (300+i) , "Error");
                        }
                        else
                        {
                            double cramer_aux;
                            for( i=0; i<dimension; i++)
                            {
                                cramer_aux = cramer(matA, matB,dimension,i);
                                printf(" X%d = %.3lf \n\n",i, cramer_aux);
                                sprintf(txt2, "%lf", cramer_aux);
                                SetDlgItemText(hwnd, (300+i), txt2);
                            }
                        }
                break;

        case ID_BOTON_BORRAR:
            for(i=0;i<4;i++)
            {
                for(j=0;j<4;j++)
                    SetDlgItemText(hwnd,(100+i*10+j),"");
                SetDlgItemText(hwnd,(200+i),"");
                SetDlgItemText(hwnd,(300+i),"");
            }
            break;

        case ID_BOTON_SALIR:
            if( MessageBox( NULL, "Si pone Aceptar cerrara el programa", "¿Desea cerrar el programa?", MB_OKCANCEL ) == IDOK )
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }

        break;
    default: // para los mensajes de los que no nos ocupamos
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

//-------------------------------------------------
// Inserta el menú en el formulario indicado
//-------------------------------------------------
void InsertarMenu(HWND hWnd)
{
    HMENU hMenu1;
    hMenu1 = CreateMenu();
    SetMenu (hWnd, hMenu1); // Asigna el menú a la ventana hWnd
}

//-------------------------------------------------
// Crea y dimensiona los controles del formulario
//-------------------------------------------------
void InitForm(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    // Creación del formulario. Crea e inicializa los controles de la ventana.

    HWND hDimension, hButton2x2, hButton3x3, hButton4x4, hButtonBorrar, hButtonCalcular, hButtonSalir;
    HWND hLetras, hGroup,hEdit;
    int i,j;

     HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
    // Insertar botones
    hGroup = CreateWindow(  "BUTTON",    /* Nombre de la clase */
                             "Dimension", /* Texto del título */
                             BS_GROUPBOX | WS_GROUP | WS_CHILD | WS_VISIBLE, /* Estilo */
                             50, 50,     /* Posición */
                             100, 100,   /* Tamaño */
                             hwnd,       /* Ventana padre */
                             (HMENU)ID_DIMENSION, /* Identificador del control */
                             hInstance,  /* Instancia */
                             NULL); /* Sin datos de creación de ventana */
    if(hGroup == NULL)
        MessageBox(hwnd, "No se pudo crear una Etiqueta DIMENSION", "Error", MB_OK | MB_ICONERROR);
    hButton2x2 = CreateWindow(
                           "BUTTON",
                           "2 x 2",
                           BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
                           60,75,
                           80,20,
                           hwnd,
                           (HMENU)ID_BOTON_2x2,
                           GetModuleHandle(NULL),
                           NULL);

    if(hButton2x2 == NULL)
        MessageBox(hwnd, "No se pudo crear un botón 2x2", "Error", MB_OK | MB_ICONERROR);
    hButton3x3 = CreateWindow(
                           "BUTTON",
                           "3 x 3",
                           BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
                           60,95,
                           80,20,
                           hwnd,
                           (HMENU)ID_BOTON_3x3,
                           GetModuleHandle(NULL),
                           NULL);

    if(hButton3x3 == NULL)
        MessageBox(hwnd, "No se pudo crear un botón 3x3", "Error", MB_OK | MB_ICONERROR);
     hButton4x4 = CreateWindow(
                           "BUTTON",
                           "4 x 4",
                           BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
                           60,115,
                           80,20,
                           hwnd,
                           (HMENU)ID_BOTON_4x4,
                           GetModuleHandle(NULL),
                           NULL);

    if(hButton4x4 == NULL)
        MessageBox(hwnd, "No se pudo crear un botón 4x4", "Error", MB_OK | MB_ICONERROR);

    hButtonBorrar = CreateWindow("BUTTON",
                           "Borrar valores",
                           BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
                           450,200,
                           100,25,
                           hwnd,
                           (HMENU)ID_BOTON_BORRAR,
                           GetModuleHandle(NULL),
                           NULL);
    if(hButtonBorrar == NULL)
        MessageBox(hwnd, "No se pudo crear un botón borrar", "Error", MB_OK | MB_ICONERROR);

    hButtonCalcular = CreateWindow("BUTTON",
                           "Calcular",
                           BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
                           560,200,
                           100,25,
                           hwnd,
                           (HMENU)ID_BOTON_CALCULAR,
                           GetModuleHandle(NULL),
                           NULL);
    if(hButtonBorrar == NULL)
        MessageBox(hwnd, "No se pudo crear un botón borrar", "Error", MB_OK | MB_ICONERROR);

    hButtonSalir = CreateWindow("BUTTON",
                           "Salir",
                           BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
                           560,230,
                           100,25,
                           hwnd,
                           (HMENU)ID_BOTON_SALIR,
                           GetModuleHandle(NULL),
                           NULL);
    if(hButtonSalir == NULL)
        MessageBox(hwnd, "No se pudo crear un botón salir", "Error", MB_OK | MB_ICONERROR);

    hLetras = CreateWindowEx( 0,
                             "STATIC", /* Nombre de la clase */
                             "A", /* Texto del título */
                             SS_SIMPLE | WS_CHILD | WS_VISIBLE, /* Estilo */
                             310, 10, /* Posición */
                             15, 15, /* Tamaño */
                             hwnd, /* Ventana padre */
                             (HMENU)ID_ESTAT1, /* Identificador del control */
                             hInstance, /* Instancia */
                             NULL); /* Sin datos de creación de ventana */
    hLetras = CreateWindowEx(0,"STATIC","b",SS_SIMPLE | WS_CHILD | WS_VISIBLE,520, 10,15, 15,hwnd,(HMENU)ID_ESTAT1,hInstance,NULL);
    hLetras = CreateWindowEx(0,"STATIC","x",SS_SIMPLE | WS_CHILD | WS_VISIBLE,630, 10,15, 15,hwnd,(HMENU)ID_ESTAT1,hInstance,NULL);

    if(hLetras == NULL)
        MessageBox(hwnd, "No se pudo crear una Etiqueta A b x", "Error", MB_OK | MB_ICONERROR);

    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
        {
              hEdit=CreateWindowEx(0,
                            "EDIT",  // Nombre de la clase
                            "",      // Texto del título, no tiene
                            ES_RIGHT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, // Estilo
                            (200+(60*i)), (50+(30*j)),  // Posición
                            50, 20, // Tamaño
                            hwnd,    // Ventana padre
                            (HMENU) (100+i*10+j) , // Identificador del control
                            hInstance, // Instancia
                            NULL);     // Sin datos de creación de ventana
        }

    for(i=0;i<4;i++)
    {
            hEdit=CreateWindowEx(0,
                        "EDIT",  // Nombre de la clase
                        "",      // Texto del título, no tiene
                        ES_RIGHT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, // Estilo
                        500, (50+(30*i)),  // Posición
                        50, 20, // Tamaño
                        hwnd,    // Ventana padre
                        (HMENU)(200+i), // Identificador del control
                        hInstance, // Instancia
                        NULL);     // Sin datos de creación de ventana
    }

    for(i=0;i<4;i++)
    {
            hEdit=CreateWindowEx(0,
                        "STATIC",  // Nombre de la clase
                        "",      // Texto del título, no tiene
                        ES_RIGHT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, // Estilo
                        580, (50+(30*i)),  // Posición
                        80, 20, // Tamaño
                        hwnd,    // Ventana padre
                        (HMENU)(300+i), // Identificador del control
                        hInstance, // Instancia
                        NULL);     // Sin datos de creación de ventana
        }
}

double det_mat_N (double mat[N][N],int dim)
{
    int i, j, dim_aux;
    double det;

    if(dim == 1)
    {
        det = mat[0][0];
        return det;
    }
    else
    {
        for(i=0, j=0, det=0, dim_aux=dim; i<dim; i++)
        {
            det += pow(-1,i+j) * mat[i][j] * det_mat_adj(mat, dim_aux, i, j);
        }
    }
    return det;
}

double det_mat_adj (double mat[N][N], int dim, int m, int n)
{
    double mat_aux[N][N];

    if( (mat_adj(mat, mat_aux, dim, m, n)) == 1)
        return (mat_aux[0][0]);

    return (det_mat_N(mat_aux,dim-1));
}

double mat_adj(double mat[N][N], double mat_aux[N][N], int dim, int m, int n)
{
    int i, j, p, q;

    for (j=0, q=0; j<dim; j++)
    if (j!=n)
    {
        for (i=0, p=0; i<dim; i++)
            if (i!=m)
            {
                mat_aux[p][q]=mat[i][j];
                p++;
            }
        q++;
    }

    return dim-1;
}

double cramer( double mat[N][N], double arr[N], int n, int x)
{
    int l, m, j;
    double res, aux[N][N];

    det2 = det_mat_N(mat,n);

    for( l=0; l<n; l++)
        for( m=0; m<n; m++)
            aux[l][m] = mat[l][m];


    for( j=0; j<n; j++)
        aux[j][x] = arr[j];

    det1 = det_mat_N(aux,n);
    res = det1 / det2;

    return res;
}
