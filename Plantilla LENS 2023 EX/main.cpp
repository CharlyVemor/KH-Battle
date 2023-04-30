#include <windows.h>
#include <iostream> //Libreria para entrada y salida de datos
#include <fstream> //Libreria para manejar archivos
#include <string> //Libreria para utilizar tipos de datos string
#include "Librerias/Dibujar bitmaps/gdipload.h"
#include "Librerias/Musica/libzplay.h"
#include "GamepadXbox.h" //Clase para detección de gamepad

using namespace libZPlay;//Se declara que se usara la libreria de libZPlay para reproductor de musica
using namespace std; //Utiliza el espacio de nombres de la libreria STD de c++

GamepadXbox* ControlXBOX;

struct Musica
{
	string Dir;
	string Nombre;
};
Musica Cancion[5];

struct FrameArray{
	int x,y;
	int ancho,alto;
};//Similar a la estructura rect de winapi

struct Input
{
	enum Keys
	{
		Backspace = 0x08, Tab,
		Clear = 0x0C, Enter,
		Shift = 0x10, Control, Alt,
		Escape = 0x1B,
		Space = 0x20, PageUp, PageDown, End, Home, Left, Up, Right, Down,
		Zero = 0x30, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NumPad0 = 0x60, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};
}input;
bool KEYS[256];

enum Acciones 
{
	Ataque,
	Defensa,
	Cura
};

enum AnimacionesPersonajes
{
	Intro,
	Idle,
	Running,
	RunningBack,
	GetHit,
	Hit,
	Defend,
	Heal,
	Die
};

enum AnimacionesUI {
	StaticUI,
	Arrow,
	NUM0,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9
};

enum Stages
{
	Inicio,
	Stage1,
	Stage2,
	Stage3,
	Winner,
	GameOver
};

enum Frames {
	Frame0,
	Frame1,
	Frame2,
	Frame3,
	Frame4,
	Frame5,
	Frame6,
	Frame7,
	Frame8,
	Frame9,
	Frame10,
	Frame11,
	Frame12,
	Frame13,
	Frame14,
	Frame15,
	Frame16,
	Frame17,
	Frame18,
	Frame19,
	Frame20
};

struct DatosPersonaje{
	//Ruta donde esta nuestra imagen
	wstring PersonajeBMP;
	miBitmap HojaSprite;

	//Posicion relativa de nuestro Sprite en la ventana
	int XCurrentCoordDraw = 0;
	int YCurrentCoordDraw = 0;

	//Dimensiones en pixeles que se requieren para dibujarlo en la ventana
	int HeightDraw = 3;
	int WeightDraw = 41;

	int Animaciones = 9;

	int idAnimacionIntro = 0;
	int FramesAnimacionIntro = 0;

	int idAnimacionIdle = 1;
	int FramesAnimacionIdle = 0;//6;

	int idAnimacionCaminando = 2;
	int FramesAnimacionCaminando = 0;//8;

	int idAnimacionCaminandoRev = 3;
	int FramesAnimacionCaminandoRev = 0;//8;

	int idAnimacionGolpeado = 4;
	int FramesAnimacionGolpeado = 0;

	int idAnimacionAtacando = 5;
	int FramesAnimacionAtacando = 0;//6;

	int idAnimacionDefendiendo = 6;
	int FramesAnimacionDefendiendo = 0;

	int idAnimacionCurando = 7;
	int FramesAnimacionCurando = 0;

	int idAnimacionMuerto = 8;
	int FramesAnimacionMuerto = 0;

	FrameArray** FrameSpriteArray;
};
DatosPersonaje Sora;
DatosPersonaje Riku;
DatosPersonaje Axel;
DatosPersonaje Marluxia;

const int SoraHealth = 200;
const int RikuHealth = 150;
const int AxelHealth = 250;
const int MarluxiaHealth = 400;
const int WidthHealthbar = 180;

struct DatosUI {
	//Posicion relativa de nuestro Sprite en la ventana
	int XCurrentCoordDraw = 0;
	int YCurrentCoordDraw = 0;

	//Dimensiones en pixeles que se requieren para dibujarlo en la ventana
	int HeightDraw = 3;
	int WeightDraw = 41;

	int Estatico = 1;
	int AnimacionesFlecha = 1;
	int AnimacionesNumeros = 10;

	int idBasicUI = 0;
	int FramesBasicUI = 1;

	int idAnimacionFlecha = 1;
	int FramesAnimacionFlecha = 4;

	int FramesAnimacionNumeros = 9;

	int idAnimacion0 = 2;

	int idAnimacion1 = 3;

	int idAnimacion2 = 4;

	int idAnimacion3 = 5;

	int idAnimacion4 = 6;

	int idAnimacion5 = 7;

	int idAnimacion6 = 8;

	int idAnimacion7 = 9;

	int idAnimacion8 = 10;

	int idAnimacion9 = 11;

	//Queria poner numeros verdes para cuando los personasjes se curaran
	//y dejar que los anteriores fueran rojos para los ataques
	//Pero el programa no me dejo ponerlas :(
	//Me daba error (Creo que por la gran cantidad de arrays de frames)
	//Actualmente los numeros estan en un color neutral, para poder ser usado en Atacar y Curar

	/*int idAnimacion0Verde = 12;

	int idAnimacion1Verde = 13;

	int idAnimacion2Verde = 14;

	int idAnimacion3Verde = 15;

	int idAnimacion4Verde = 16;

	int idAnimacion5Verde = 17;

	int idAnimacion6Verde = 18;

	int idAnimacion7Verde = 19;

	int idAnimacion8Verde = 20;

	int idAnimacion9Verde = 21;*/

	FrameArray** FrameSpriteArray;
};
DatosUI BasicUI;
DatosUI Flecha;
DatosUI BarraPlayer;
DatosUI BarraEnemy;
DatosUI VidaPlayer;
DatosUI VidaEnemy;
DatosUI Numero1;
DatosUI Numero2;
DatosUI EnemyStock1;
DatosUI EnemyStock2;
DatosUI EnemyStock3;
DatosUI PlayerStock;

struct DatosEfectos {
	//Posicion relativa de nuestro Sprite en la ventana
	int XCurrentCoordDraw = 0;
	int YCurrentCoordDraw = 0;

	//Dimensiones en pixeles que se requieren para dibujarlo en la ventana
	int HeightDraw = 3;
	int WeightDraw = 41;

	int idAnimacionCorazon = 0;
	int FramesAnimacionCorazon = 8;

	int idAnimacionEscudo = 1;
	int FramesAnimacionEscudo = 4;

	int idAnimacionGuadaña = 2;
	int FramesAnimacionGuadaña = 7;

	FrameArray* FrameSpriteArray;
};
DatosEfectos Corazon;
DatosEfectos Escudo;
DatosEfectos Guadaña;

struct DatosStage{
	int Escenarios = 5;
	miBitmap ImagenEscenario1;
	WCHAR Bmp1[MAX_PATH] = L"Recursos/Pantalla-de-titulo-KHB.bmp";
	miBitmap ImagenEscenario2;
	WCHAR Bmp2[MAX_PATH] = L"Recursos/Twilight Town.bmp";
	miBitmap ImagenEscenario3;
	WCHAR Bmp3[MAX_PATH] = L"Recursos/Floating Island.bmp";
	miBitmap ImagenEscenario4;
	WCHAR Bmp4[MAX_PATH] = L"Recursos/Castle Oblivion.bmp";
	miBitmap ImagenEscenario5;
	WCHAR Bmp5[MAX_PATH] = L"Recursos/WinScreen.bmp";
}miStage;

//Variables Globales
const int ANCHO_VENTANA = 415*2;
const int ALTO_VENTANA = 367*2;
const int BPP = 4;
const int TICK = 99;//99
//const unsigned int TRANSPARENCY = 0xFF00FF00;
ZPlay* player = CreateZPlay();//Generamos un objeto puntero para nuestro reproductor
//No se si me de tiempo para agregar efectos de sonido, pero por si acaso xd
ZPlay* PlayerVoice = CreateZPlay();
ZPlay* EnemyVoice = CreateZPlay();
ZPlay* SoundEffects = CreateZPlay();
TStreamStatus status;
bool pausa = false;
int* ptrBufferPixelsWindow;

int CurrentStage = 0;

int SoraAnimacionActual = 0;
int SoraFrameActual = 0;
int SoraDelayFrameAnimation = 0;

int RikuAnimacionActual = 0;
int RikuFrameActual = 0;
int RikuDelayFrameAnimation = 0;

int AxelAnimacionActual = 0;
int AxelFrameActual = 0;
int AxelDelayFrameAnimation = 0;

int MarluxiaAnimacionActual = 0;
int MarluxiaFrameActual = 0;
int MarluxiaDelayFrameAnimation = 0;

int FlechaFrameActual = 0;

int Num1AnimacionActual = 0;
int Num2AnimacionActual = 0;
int NumFrameActual = 0;
int NumDelayFrameAnimation = 0;

int CorazonFrameActual = 0;

int GuadañaFrameActual = 0;

int EscudoJugadorFrameActual = 0;
int EscudoEnemigoFrameActual = 0;

int Accion = 0;

wstring UI_BMP = L"Recursos/UI.bmp";
miBitmap UIHojaSprite;
wstring GameOver_BMP = L"Recursos/Game Over.bmp";
miBitmap GameOverHojaSprite;

int PlayerHealth = 0;
int EnemyHealth = 0;
unsigned int TotalDamage = 0;
unsigned int TotalHealth = 0;
int Score = 0;

bool PlayerShield = false;
bool EnemyShield = false;
bool EnemyHadShield = false;
bool PlayerHadShield = false;
bool ShowNums = false;
bool ShowUI = false;
bool ShowPlayerHealth;
bool ShowEnemyHealth;
bool ShowIntroMarluxia = false;

bool PantallaInicio = true;
bool InGame = false;
bool InGameOver = false;
bool WinScreen = false;
bool PlayerTurn = false;
bool EnemyTurn = false;

//Da el turno despues de cuando los numeros terminan de desaparecer
bool GiveTurnPlayAtAnimEnd = false;
bool GiveTurnEnemyAtAnimEnd = false;

//Definicion de funciones
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void LimpiarFondo(int *ptrBuffer, unsigned int color, int area);
void TranScaleblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int escalaX, int escalaY, bool borrafondo);
void mirrorxblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int escalaX, int escalaY, const unsigned int TRANSPARENCY);
void MainRender(HWND hWnd);
void Init();
void KeysEvents();
void Vibracion();
void ReproductorPausa();
void ReproductorReproduce();
void ReproductorInicializaYReproduce();
void ReproductorCambiarCancionYReproduce(int);
void CargaFramesSpritePersonajes();
void CargaFramesSpriteUI();
void CargaFramesEfectos();
void CambiarStage(int Stage);
void RikuAI();
void AxelAI();
void MarluxiaAI(bool EnemShield, int health);
void DisplayNumber(int Number);
void WriteHighscores();
void SetDamage(bool ToPlayer);
void SetHealing(bool IsPlayer);
void HealSoraMASM(int Heal);
void HealRikuMASM(int Heal);
void HealAxelMASM(int Heal);
void HealMarluxiaMASM(int Heal);
void DamageSoraMASM(int Damage);
void DamageRikuMASM(int Damage);
void DamageAxelMASM(int Damage);
void DamageMarluxiaMASM(int Damage);

int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow)
{
	WNDCLASSEX wc;									// Windows Class Structure
	HWND hWnd;
	MSG msg;

	TCHAR szAppName[] = TEXT("MyWinAPIApp");		
	TCHAR szAppTitle[] = TEXT("Kingdom Hearts Battle");

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;
	wc.hIcon			= (HICON)LoadImage(NULL, L"Recursos/KH_Battle.ico", IMAGE_ICON, 256, 256, LR_LOADFROMFILE | LR_SHARED); //Load Custom Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);			// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu	
	wc.lpszClassName	= szAppName;							// Set The Class Name
	wc.hIconSm			= NULL;
	
	if (!RegisterClassEx(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,
			L"Fallo al registrar clase (Failed To Register The Window Class).",
			L"ERROR",
			MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	hWnd = CreateWindowEx(	
		WS_EX_CLIENTEDGE | WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,	// Extended Style For The Window
		szAppName,							// Class Name
		szAppTitle,							// Window Title
		WS_OVERLAPPEDWINDOW |				// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		ANCHO_VENTANA+20,								// Calculate Window Width
		ALTO_VENTANA+43,								// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL);								// Pass this class To WM_CREATE								

	if(hWnd == NULL) {
		MessageBox(NULL, 
			L"Error al crear ventana (Window Creation Error).", 
			L"ERROR", 
			MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	ControlXBOX = new GamepadXbox(1);
	srand((unsigned)time(NULL));
		
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	Init();
	ShowWindow(hWnd, nCmdShow);
	SetFocus(hWnd);

	SetTimer(hWnd, TICK, TICK, NULL);
	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)// Check For Windows Messages
	{	
		case WM_CREATE: {	//Función que me copie de internet nomas para poner la ventana en el centro de la pantalla
		RECT rc;

		GetWindowRect(hWnd, &rc);

		int xPos = ((GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2);
		int yPos = ((GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2) - 15;

		SetWindowPos(hWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		break;

		case WM_KEYDOWN:
		{
			KEYS[wParam] = true;
		}
			break;
		case WM_KEYUP:
		{
			KEYS[wParam] = false;
			switch (wParam) {
				case VK_RETURN:
				{
					if (PantallaInicio) {
						PantallaInicio = false;
						InGame = true;
						CambiarStage(Stage1);
					}
					else if (PlayerTurn){
						switch (Accion) {
						case Ataque:
							SoraFrameActual = 0;
							PlayerTurn = false;
							SoraAnimacionActual = Running;
							break;

						case Defensa:
							SoraFrameActual = 0;
							PlayerTurn = false;
							SoraAnimacionActual = Defend;
							break;

						case Cura:
							SoraFrameActual = 0;
							PlayerTurn = false;
							SoraAnimacionActual = Heal;
							break;
						}
					}
				}
				return 0;

				case VK_F1://F1
				{
					ReproductorPausa();
				}
				return 0;

				case VK_F2://F2
				{
					ReproductorReproduce();
				}
				return 0;
				
				//Tests
				case VK_F5://F5
				{
					CambiarStage(GameOver);
				}
				return 0;
				case VK_F6://F6
				{
					CambiarStage(Stage2);
				}
				return 0;
				case VK_F7://F7
				{
					CambiarStage(Stage3);
				}
				return 0;
				case VK_F8://F8
				{
					CambiarStage(Winner);
				}
				return 0;

				//Voy a dejar estos cheatcodes por si las peleas duran mucho xd
				case 0x61: //Numpad1
				{
					SetDamage(false); //Dañar al contrincante
				}
				return 0;
				case 0x62: //Numpad2
				{
					SetHealing(true); //Curar al jugador
				}
				return 0;
				case 0x63: //Numpad3
				{
					SetDamage(true); //Dañar al jugador
				}
				
				case VK_BACK:
				{
					if (PlayerTurn) {
						PlayerTurn = false;
						EnemyTurn = true;
					}
					else {
						PlayerTurn = true;
						EnemyTurn = false;
					}
				}
				return 0;
			}
		}
			break;
		case WM_TIMER:
			if(wParam == TICK)
			{
				MainRender(hWnd);
				switch (CurrentStage) {
				case Stage1:
					if (EnemyTurn) {
						RikuAI();
					}
					if (SoraAnimacionActual == Hit && SoraFrameActual == 5 && EnemyHealth == 0) {
						CambiarStage(Stage2);
					}
					break;

				case Stage2:
					if (EnemyTurn) {
						AxelAI();
					}
					if (SoraAnimacionActual == Hit && SoraFrameActual == 5 && EnemyHealth == 0) {
						CambiarStage(Stage3);
					}
					break;

				case Stage3:
					if (EnemyTurn) {
						MarluxiaAI(EnemyShield, EnemyHealth);
					}
					if (SoraAnimacionActual == Hit && SoraFrameActual == 5 && EnemyHealth == 0) {
						CambiarStage(Winner);
					}
					break;
				}
				player->GetStatus(&status);
				if (!pausa && status.fPlay == 0){
					ReproductorReproduce();
				}
			}
			break;
		case WM_PAINT:
			{
				HDC hdc; 
				PAINTSTRUCT ps;
				hdc = BeginPaint(hWnd, &ps);
				HBITMAP h_CMC = CreateBitmap(ANCHO_VENTANA, ALTO_VENTANA, 1, 32, ptrBufferPixelsWindow);
				HDC hdcMem = CreateCompatibleDC(hdc);
				HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, h_CMC);//Creacion de marco para proyecto

				BitBlt(hdc, 0, 0, ANCHO_VENTANA, ALTO_VENTANA, hdcMem, 0, 0, SRCCOPY);

				DeleteObject(h_CMC);
				//SelectObject(hdcMem, hbmOld);
				DeleteDC(hdcMem);
				//DeleteObject(hbmOld);
			}
			break;		
		/*case WM_KEYDOWN:							
			{
				KEYS[ wParam ] = true;
			}
			break;*/
		case WM_CLOSE: 
			{
				if (CurrentStage == Winner) {
					WriteHighscores();
				}
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY: //Send A Quit Message
			{
				KillTimer(hWnd, TICK);
				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

/* Inicializacion de variables y reserva de memoria.
	*/
void Init()
{
	//Empezamos a reproducir musica
	ReproductorInicializaYReproduce();

	CargaFramesSpritePersonajes();

	CargaFramesSpriteUI();

	CargaFramesEfectos();

	//Cargamos imagen bitmap de nuestro escenario
	miStage.ImagenEscenario1 = gdipLoad(miStage.Bmp1);
	miStage.ImagenEscenario2 = gdipLoad(miStage.Bmp2);
	miStage.ImagenEscenario3 = gdipLoad(miStage.Bmp3);
	miStage.ImagenEscenario4 = gdipLoad(miStage.Bmp4);
	miStage.ImagenEscenario5 = gdipLoad(miStage.Bmp5);

	//Definimos un puntero del total de pixeles que tiene nuestra ventana
	ptrBufferPixelsWindow = new int[ANCHO_VENTANA * ALTO_VENTANA];
}

void CargaFramesSpritePersonajes()
{

	Sora.FramesAnimacionIntro = 17;			//Summon cards -> Keyblade transicion
	Sora.FramesAnimacionIdle = 6;			//Idle
	Sora.FramesAnimacionCaminando = 8;		//Run
	Sora.FramesAnimacionCaminandoRev = 8;	//RunningBack
	Sora.FramesAnimacionGolpeado = 7;		//Get Hit
	Sora.FramesAnimacionAtacando = 7;		//Ataque horizontal con keyblade
	Sora.FramesAnimacionDefendiendo = 5;	//"Defensa" con strike directo de keyblade
	Sora.FramesAnimacionCurando = 7;		//Usa item
	Sora.FramesAnimacionMuerto = 6;			//Flotando, Exclusivo del personaje jugador

	Riku.FramesAnimacionIntro = 6;			//Intro simple
	Riku.FramesAnimacionIdle = 6;			//Idle
	Riku.FramesAnimacionCaminando = 6;		//Correr
	Riku.FramesAnimacionCaminandoRev = 6;	//Correr mirrored
	Riku.FramesAnimacionGolpeado = 5;		//Get Hit
	Riku.FramesAnimacionAtacando = 7;		//Slash estando quieto
	Riku.FramesAnimacionDefendiendo = 5;	//Alza keyblade y la baja
	Riku.FramesAnimacionCurando = 7;		//Usa item

	Axel.FramesAnimacionIntro = 16;			//Usar Frames Seleccionados
	Axel.FramesAnimacionIdle = 6;			//Idle
	Axel.FramesAnimacionCaminando = 13;		//Teletransportacion de ida	--- 12 frames + frame en blanco extra
	Axel.FramesAnimacionCaminandoRev = 13;	//Teletransportacion de regreso --- 12 frames + frame en blanco extra
	Axel.FramesAnimacionGolpeado = 5;		//Get Hit
	Axel.FramesAnimacionAtacando = 8;		//Ataque fisico
	Axel.FramesAnimacionDefendiendo = 8;	//Grito -> Expande brazos
	Axel.FramesAnimacionCurando = 11;		//6 frames de Arma en fuego y retroceder animacion 5 frames

	Marluxia.FramesAnimacionIntro = 14;			//Recoger guadaña
	Marluxia.FramesAnimacionIdle = 5;			//Idle
	Marluxia.FramesAnimacionCaminando = 4;		//Flotando
	Marluxia.FramesAnimacionCaminandoRev = 4;	//Flotando mirrored
	Marluxia.FramesAnimacionGolpeado = 5;		//Get Hit
	Marluxia.FramesAnimacionAtacando = 8;		//Slash con guadaña, checar sprites pq vienen movidos
	Marluxia.FramesAnimacionDefendiendo = 10;	//Toque de guadaña al piso
	Marluxia.FramesAnimacionCurando = 10;		//Usa item

	Sora.PersonajeBMP = L"Recursos/Sora.bmp";
	Riku.PersonajeBMP = L"Recursos/Riku.bmp";
	Axel.PersonajeBMP = L"Recursos/Axel.bmp";
	Marluxia.PersonajeBMP = L"Recursos/Marluxia.bmp";

	//Cargamos primero las hojas de sprite

	//Personajes
	Sora.HojaSprite = gdipLoad((WCHAR*)Sora.PersonajeBMP.c_str());
	Riku.HojaSprite = gdipLoad((WCHAR*)Riku.PersonajeBMP.c_str());
	Axel.HojaSprite = gdipLoad((WCHAR*)Axel.PersonajeBMP.c_str());
	Marluxia.HojaSprite = gdipLoad((WCHAR*)Marluxia.PersonajeBMP.c_str());

	//Definiendo las coordenadas iniciales en pantalla donde iniciaremos

	//Personajes
	Sora.XCurrentCoordDraw = 600;
	Sora.YCurrentCoordDraw = 410;

	Riku.XCurrentCoordDraw = 110;
	Riku.YCurrentCoordDraw = 400;

	Axel.XCurrentCoordDraw = 70;
	Axel.YCurrentCoordDraw = 300;

	Marluxia.XCurrentCoordDraw = 159;
	Marluxia.YCurrentCoordDraw = 223;

	//Definiendo las dimenciones en base al # de Animaciones
	Sora.FrameSpriteArray = new FrameArray * [Sora.Animaciones];
	Riku.FrameSpriteArray = new FrameArray * [Riku.Animaciones];
	Axel.FrameSpriteArray = new FrameArray * [Axel.Animaciones];
	Marluxia.FrameSpriteArray = new FrameArray * [Marluxia.Animaciones];

	//Definiendo la cantidad de Frames en base a cada Animacion

	Sora.FrameSpriteArray[Sora.idAnimacionIntro] = new FrameArray[Sora.FramesAnimacionIntro];
	Sora.FrameSpriteArray[Sora.idAnimacionIdle] = new FrameArray[Sora.FramesAnimacionIdle];
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando] = new FrameArray[Sora.FramesAnimacionCaminando];
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev] = new FrameArray[Sora.FramesAnimacionCaminandoRev];
	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado] = new FrameArray[Sora.FramesAnimacionGolpeado];
	Sora.FrameSpriteArray[Sora.idAnimacionAtacando] = new FrameArray[Sora.FramesAnimacionAtacando];
	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo] = new FrameArray[Sora.FramesAnimacionDefendiendo];
	Sora.FrameSpriteArray[Sora.idAnimacionCurando] = new FrameArray[Sora.FramesAnimacionCurando];
	Sora.FrameSpriteArray[Sora.idAnimacionMuerto] = new FrameArray[Sora.FramesAnimacionMuerto];

	Riku.FrameSpriteArray[Riku.idAnimacionIntro] = new FrameArray[Riku.FramesAnimacionIntro];
	Riku.FrameSpriteArray[Riku.idAnimacionIdle] = new FrameArray[Riku.FramesAnimacionIdle];
	Riku.FrameSpriteArray[Riku.idAnimacionCaminando] = new FrameArray[Riku.FramesAnimacionCaminando];
	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev] = new FrameArray[Riku.FramesAnimacionCaminandoRev];
	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado] = new FrameArray[Riku.FramesAnimacionGolpeado];
	Riku.FrameSpriteArray[Riku.idAnimacionAtacando] = new FrameArray[Riku.FramesAnimacionAtacando];
	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo] = new FrameArray[Riku.FramesAnimacionDefendiendo];
	Riku.FrameSpriteArray[Riku.idAnimacionCurando] = new FrameArray[Riku.FramesAnimacionCurando];

	Axel.FrameSpriteArray[Axel.idAnimacionIntro] = new FrameArray[Axel.FramesAnimacionIntro];
	Axel.FrameSpriteArray[Axel.idAnimacionIdle] = new FrameArray[Axel.FramesAnimacionIdle];
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando] = new FrameArray[Axel.FramesAnimacionCaminando];
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev] = new FrameArray[Axel.FramesAnimacionCaminandoRev];
	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado] = new FrameArray[Axel.FramesAnimacionGolpeado];
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando] = new FrameArray[Axel.FramesAnimacionAtacando];
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo] = new FrameArray[Axel.FramesAnimacionDefendiendo];
	Axel.FrameSpriteArray[Axel.idAnimacionCurando] = new FrameArray[Axel.FramesAnimacionCurando];

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro] = new FrameArray[Marluxia.FramesAnimacionIntro];
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle] = new FrameArray[Marluxia.FramesAnimacionIdle];
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando] = new FrameArray[Marluxia.FramesAnimacionCaminando];
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev] = new FrameArray[Marluxia.FramesAnimacionCaminandoRev];
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado] = new FrameArray[Marluxia.FramesAnimacionGolpeado];
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando] = new FrameArray[Marluxia.FramesAnimacionAtacando];
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo] = new FrameArray[Marluxia.FramesAnimacionDefendiendo];
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando] = new FrameArray[Marluxia.FramesAnimacionCurando];

	//Cargando Frames a nuestro arreglo del sprite

	//Personajes

	// -------- Animacion 1 Intro -------- //
#pragma region IntroAnim
	//Sora
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame0].x = 532; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame0].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame0].ancho = 35; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame0].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame1].x = 570; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame1].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame1].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame1].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame2].x = 608; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame2].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame2].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame2].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame3].x = 640; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame3].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame3].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame3].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame4].x = 674; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame4].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame4].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame4].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame5].x = 710; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame5].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame5].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame5].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame6].x = 746; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame6].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame6].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame6].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame7].x = 779; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame7].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame7].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame7].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame8].x = 710; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame8].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame8].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame8].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame9].x = 746; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame9].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame9].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame9].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame10].x = 779; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame10].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame10].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame10].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame11].x = 608; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame11].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame11].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame11].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame12].x = 570; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame12].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame12].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame12].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame13].x = 532; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame13].y = 1477;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame13].ancho = 35; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame13].alto = 61;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame14].x = 535; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame14].y = 743;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame14].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame14].alto = 54;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame15].x = 574; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame15].y = 743;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame15].ancho = 36; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame15].alto = 54;

	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame16].x = 630; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame16].y = 743;
	Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame16].ancho = 26; Sora.FrameSpriteArray[Sora.idAnimacionIntro][Frame16].alto = 54;

	//Riku
	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame0].x = 67; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame0].y = 3332;
	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame0].ancho = 40; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame0].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame1].x = 114; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame1].y = 3332;
	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame1].ancho = 55; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame1].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame2].x = 179; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame2].y = 3332;
	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame2].ancho = 50; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame2].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame3].x = 240; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame3].y = 3332;
	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame3].ancho = 47; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame3].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame4].x = 307; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame4].y = 3332;
	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame4].ancho = 36; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame4].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame5].x = 357; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame5].y = 3332;
	Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame5].ancho = 45; Riku.FrameSpriteArray[Riku.idAnimacionIntro][Frame5].alto = 58;

	//Axel
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame0].x = 5; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame0].y = 290;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame0].ancho = 42; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame0].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame1].x = 52; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame1].y = 290;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame1].ancho = 50; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame1].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame2].x = 114; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame2].y = 290;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame2].ancho = 54; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame2].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame3].x = 175; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame3].y = 290;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame3].ancho = 36; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame3].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame4].x = 221; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame4].y = 290;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame4].ancho = 41; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame4].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame5].x = 273; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame5].y = 290;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame5].ancho = 55; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame5].alto = 80;


	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame6].x = 143; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame6].y = 374;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame6].ancho = 55; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame6].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame7].x = 207; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame7].y = 374;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame7].ancho = 55; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame7].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame8].x = 269; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame8].y = 374;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame8].ancho = 68; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame8].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame9].x = 349; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame9].y = 374;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame9].ancho = 68; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame9].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame10].x = 429; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame10].y = 374;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame10].ancho = 66; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame10].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame11].x = 500; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame11].y = 374;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame11].ancho = 72; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame11].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame12].x = 579; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame12].y = 374;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame12].ancho = 72; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame12].alto = 80;


	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame13].x = 420; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame13].y = 555;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame13].ancho = 75; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame13].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame14].x = 733; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame14].y = 555;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame14].ancho = 75; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame14].alto = 80;

	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame15].x = 812; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame15].y = 555;
	Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame15].ancho = 75; Axel.FrameSpriteArray[Axel.idAnimacionIntro][Frame15].alto = 80;

	//Marluxia
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame0].x = 2; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame0].y = 121;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame0].ancho = 27; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame0].alto = 118;
	
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame1].x = 36; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame1].y = 121;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame1].ancho = 28; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame1].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame2].x = 73; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame2].y = 121;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame2].ancho = 26; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame2].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame3].x = 112; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame3].y = 121;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame3].ancho = 26; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame3].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame4].x = 147; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame4].y = 121;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame4].ancho = 29; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame4].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame5].x = 183; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame5].y = 121;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame5].ancho = 28; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame5].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame6].x = 224; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame6].y = 121;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame6].ancho = 25; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame6].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame7].x = 262; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame7].y = 121;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame7].ancho = 26; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame7].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame8].x = 2; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame8].y = 349;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame8].ancho = 76; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame8].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame9].x = 85; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame9].y = 349;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame9].ancho = 76; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame9].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame10].x = 169; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame10].y = 349;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame10].ancho = 65; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame10].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame11].x = 239; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame11].y = 349;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame11].ancho = 64; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame11].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame12].x = 309; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame12].y = 349;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame12].ancho = 82; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame12].alto = 118;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame13].x = 394; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame13].y = 349;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame13].ancho = 81; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIntro][Frame13].alto = 118;
#pragma endregion 

	// -------- Animacion 2 Idle -------- //
#pragma region IdleAnim
	//Sora
	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame0].x = 535; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame0].y = 801;
	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame0].ancho = 39; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame0].alto = 44;

	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame1].x = 581; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame1].y = 801;
	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame1].ancho = 39; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame1].alto = 44;

	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame2].x = 627; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame2].y = 800;
	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame2].ancho = 39; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame2].alto = 44;

	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame3].x = 672; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame3].y = 801;
	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame3].ancho = 39; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame3].alto = 44;

	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame4].x = 717; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame4].y = 801;
	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame4].ancho = 39; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame4].alto = 44;

	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame5].x = 761; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame5].y = 801;
	Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame5].ancho = 39; Sora.FrameSpriteArray[Sora.idAnimacionIdle][Frame5].alto = 44;

	//Riku
	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame0].x = 79; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame0].y = 3405;
	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame0].ancho = 47; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame0].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame1].x = 131; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame1].y = 3405;
	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame1].ancho = 47; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame1].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame2].x = 186; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame2].y = 3405;
	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame2].ancho = 47; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame2].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame3].x = 239; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame3].y = 3405;
	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame3].ancho = 47; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame3].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame4].x = 289; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame4].y = 3405;
	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame4].ancho = 47; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame4].alto = 58;

	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame5].x = 340; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame5].y = 3405;
	Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame5].ancho = 47; Riku.FrameSpriteArray[Riku.idAnimacionIdle][Frame5].alto = 58;

	//Axel
	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame0].x = 3; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame0].y = 666;
	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame0].ancho = 80; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame0].alto = 73;

	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame1].x = 89; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame1].y = 666;
	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame1].ancho = 80; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame1].alto = 73;

	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame2].x = 175; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame2].y = 666;
	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame2].ancho = 79; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame2].alto = 73;

	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame3].x = 261; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame3].y = 666;
	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame3].ancho = 77; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame3].alto = 73;

	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame4].x = 341; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame4].y = 666;
	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame4].ancho = 79; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame4].alto = 73;

	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame5].x = 422; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame5].y = 666;
	Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame5].ancho = 80; Axel.FrameSpriteArray[Axel.idAnimacionIdle][Frame5].alto = 73;

	//Marluxia
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame0].x = 3; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame0].y = 471;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame0].ancho = 82; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame0].alto = 72;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame1].x = 88; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame1].y = 471;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame1].ancho = 82; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame1].alto = 72;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame2].x = 173; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame2].y = 471;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame2].ancho = 82; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame2].alto = 72;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame3].x = 258; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame3].y = 471;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame3].ancho = 82; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame3].alto = 72;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame4].x = 344; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame4].y = 471;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame4].ancho = 82; Marluxia.FrameSpriteArray[Marluxia.idAnimacionIdle][Frame4].alto = 72;
#pragma endregion

	// -------- Animacion 3 Running -------- //
#pragma region RunningAnim
	//Sora
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame0].x = 0; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame0].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame0].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame0].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame1].x = 36; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame1].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame1].ancho = 42; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame1].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame2].x = 83; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame2].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame2].ancho = 47; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame2].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame3].x = 133; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame3].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame3].ancho = 40; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame3].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame4].x = 177; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame4].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame4].ancho = 31; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame4].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame5].x = 213; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame5].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame5].ancho = 42; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame5].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame6].x = 261; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame6].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame6].ancho = 47; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame6].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame7].x = 310; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame7].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame7].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionCaminando][Frame7].alto = 45;

	//Riku
	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame0].x = 22; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame0].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame0].ancho = 39; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame0].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame1].x = 66; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame1].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame1].ancho = 47; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame1].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame2].x = 121; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame2].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame2].ancho = 58; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame2].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame3].x = 254; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame3].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame3].ancho = 39; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame3].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame4].x = 311; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame4].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame4].ancho = 42; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame4].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame5].x = 363; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame5].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame5].ancho = 54; Riku.FrameSpriteArray[Riku.idAnimacionCaminando][Frame5].alto = 51;

	//Axel
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame0].x = 3; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame0].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame0].ancho = 71; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame0].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame1].x = 81; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame1].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame1].ancho = 62; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame1].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame2].x = 149; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame2].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame2].ancho = 53; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame2].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame3].x = 206; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame3].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame3].ancho = 47; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame3].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame4].x = 260; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame4].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame4].ancho = 29; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame4].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame5].x = 299; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame5].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame5].ancho = 23; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame5].alto = 123;

	//Frame en blanco
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame6].x = 299; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame6].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame6].ancho = 1; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame6].alto = 1;

	//Mismos primeros 6 frames en reversa
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame7].x = 299; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame7].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame7].ancho = 23; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame7].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame8].x = 260; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame8].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame8].ancho = 29; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame8].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame9].x = 206; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame9].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame9].ancho = 47; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame9].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame10].x = 149; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame10].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame10].ancho = 53; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame10].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame11].x = 81; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame11].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame11].ancho = 62; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame11].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame12].x = 3; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame12].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame12].ancho = 71; Axel.FrameSpriteArray[Axel.idAnimacionCaminando][Frame12].alto = 123;

	//Marluxia
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame0].x = 3; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame0].y = 546;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame0].ancho = 62; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame0].alto = 67;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame1].x = 76; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame1].y = 546;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame1].ancho = 62; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame1].alto = 67;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame2].x = 147; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame2].y = 546;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame2].ancho = 62; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame2].alto = 67;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame3].x = 217; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame3].y = 546;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame3].ancho = 62; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminando][Frame3].alto = 67;
#pragma endregion

	// -------- Animacion 4 RunningBack -------- //
#pragma region RunningBackAnim
	//Sora
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame0].x = 0; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame0].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame0].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame0].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame1].x = 36; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame1].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame1].ancho = 42; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame1].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame2].x = 83; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame2].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame2].ancho = 47; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame2].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame3].x = 133; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame3].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame3].ancho = 40; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame3].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame4].x = 177; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame4].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame4].ancho = 31; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame4].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame5].x = 213; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame5].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame5].ancho = 42; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame5].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame6].x = 261; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame6].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame6].ancho = 47; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame6].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame7].x = 310; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame7].y = 580;
	Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame7].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionCaminandoRev][Frame7].alto = 45;

	//Riku
	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame0].x = 22; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame0].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame0].ancho = 39; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame0].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame1].x = 66; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame1].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame1].ancho = 47; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame1].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame2].x = 121; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame2].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame2].ancho = 58; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame2].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame3].x = 254; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame3].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame3].ancho = 39; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame3].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame4].x = 311; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame4].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame4].ancho = 42; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame4].alto = 51;

	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame5].x = 363; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame5].y = 1830;
	Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame5].ancho = 54; Riku.FrameSpriteArray[Riku.idAnimacionCaminandoRev][Frame5].alto = 51;

	//Axel
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame0].x = 3; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame0].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame0].ancho = 71; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame0].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame1].x = 81; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame1].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame1].ancho = 62; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame1].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame2].x = 149; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame2].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame2].ancho = 53; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame2].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame3].x = 206; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame3].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame3].ancho = 47; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame3].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame4].x = 260; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame4].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame4].ancho = 29; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame4].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame5].x = 299; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame5].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame5].ancho = 23; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame5].alto = 123;

	//Frame en blanco
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame6].x = 299; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame6].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame6].ancho = 1; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame6].alto = 1;

	//Mismos primeros 6 frames en reversa
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame7].x = 299; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame7].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame7].ancho = 23; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame7].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame8].x = 260; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame8].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame8].ancho = 29; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame8].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame9].x = 206; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame9].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame9].ancho = 47; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame9].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame10].x = 149; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame10].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame10].ancho = 53; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame10].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame11].x = 81; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame11].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame11].ancho = 62; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame11].alto = 123;

	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame12].x = 3; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame12].y = 746;
	Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame12].ancho = 71; Axel.FrameSpriteArray[Axel.idAnimacionCaminandoRev][Frame12].alto = 123;

	//Marluxia
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame0].x = 3; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame0].y = 546;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame0].ancho = 62; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame0].alto = 67;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame1].x = 76; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame1].y = 546;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame1].ancho = 62; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame1].alto = 67;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame2].x = 147; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame2].y = 546;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame2].ancho = 62; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame2].alto = 67;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame3].x = 217; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame3].y = 546;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame3].ancho = 62; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCaminandoRev][Frame3].alto = 67;
#pragma endregion

	// -------- Animacion 5 GetHit -------- //
#pragma region GettingHitAnim
	//Sora
	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame0].x = 535; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame0].y = 1543;
	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame0].ancho = 39; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame0].alto = 57;

	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame1].x = 579; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame1].y = 1543;
	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame1].ancho = 45; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame1].alto = 57;

	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame2].x = 633; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame2].y = 1543;
	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame2].ancho = 40; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame2].alto = 57;

	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame3].x = 680; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame3].y = 1543;
	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame3].ancho = 44; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame3].alto = 57;

	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame4].x = 729; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame4].y = 1543;
	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame4].ancho = 56; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame4].alto = 57;

	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame5].x = 788; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame5].y = 1543;
	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame5].ancho = 45; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame5].alto = 57;

	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame6].x = 838; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame6].y = 1541;
	Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame6].ancho = 26; Sora.FrameSpriteArray[Sora.idAnimacionGolpeado][Frame6].alto = 57;

	//Riku
	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame0].x = 75; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame0].y = 3469;
	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame0].ancho = 50; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame0].alto = 57;

	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame1].x = 139; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame1].y = 3469;
	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame1].ancho = 46; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame1].alto = 57;

	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame2].x = 202; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame2].y = 3469;
	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame2].ancho = 40; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame2].alto = 57;

	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame3].x = 255; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame3].y = 3469;
	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame3].ancho = 45; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame3].alto = 57;

	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame4].x = 310; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame4].y = 3469;
	Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame4].ancho = 48; Riku.FrameSpriteArray[Riku.idAnimacionGolpeado][Frame4].alto = 57;

	//Axel
	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame0].x = 3; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame0].y = 1379;
	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame0].ancho = 78; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame0].alto = 70;

	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame1].x = 85; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame1].y = 1379;
	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame1].ancho = 59; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame1].alto = 70;

	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame2].x = 147; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame2].y = 1379;
	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame2].ancho = 68; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame2].alto = 70;

	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame3].x = 220; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame3].y = 1379;
	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame3].ancho = 75; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame3].alto = 70;

	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame4].x = 296; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame4].y = 1379;
	Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame4].ancho = 73; Axel.FrameSpriteArray[Axel.idAnimacionGolpeado][Frame4].alto = 70;

	//Marluxia
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame0].x = 3; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame0].y = 1369;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame0].ancho = 84; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame0].alto = 84;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame1].x = 96; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame1].y = 1369;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame1].ancho = 85; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame1].alto = 84;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame2].x = 190; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame2].y = 1369;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame2].ancho = 58; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame2].alto = 84;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame3].x = 253; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame3].y = 1369;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame3].ancho = 71; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame3].alto = 84;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame4].x = 335; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame4].y = 1369;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame4].ancho = 37; Marluxia.FrameSpriteArray[Marluxia.idAnimacionGolpeado][Frame4].alto = 84;
#pragma endregion

	// -------- Animacion 6 Hit -------- //
#pragma region HitAnim
	//Sora
	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame0].x = 535; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame0].y = 959;
	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame0].ancho = 57; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame0].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame1].x = 595; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame1].y = 959;
	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame1].ancho = 45; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame1].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame2].x = 645; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame2].y = 959;
	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame2].ancho = 54; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame2].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame3].x = 703; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame3].y = 959;
	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame3].ancho = 72; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame3].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame4].x = 779; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame4].y = 959;
	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame4].ancho = 41; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame4].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame5].x = 823; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame5].y = 959;
	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame5].ancho = 49; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame5].alto = 45;

	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame6].x = 878; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame6].y = 959;
	Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame6].ancho = 60; Sora.FrameSpriteArray[Sora.idAnimacionAtacando][Frame6].alto = 45;

	//Riku
	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame0].x = 24; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame0].y = 3592;
	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame0].ancho = 57; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame0].alto = 66;

	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame1].x = 94; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame1].y = 3592;
	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame1].ancho = 65; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame1].alto = 66;

	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame2].x = 171; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame2].y = 3592;
	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame2].ancho = 55; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame2].alto = 66;

	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame3].x = 230; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame3].y = 3592;
	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame3].ancho = 74; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame3].alto = 66;

	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame4].x = 312; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame4].y = 3592;
	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame4].ancho = 63; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame4].alto = 66;

	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame5].x = 390; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame5].y = 3592;
	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame5].ancho = 39; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame5].alto = 66;

	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame6].x = 442; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame6].y = 3592;
	Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame6].ancho = 36; Riku.FrameSpriteArray[Riku.idAnimacionAtacando][Frame6].alto = 66;

	//Axel
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame0].x = 3; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame0].y = 1036;
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame0].ancho = 58; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame0].alto = 89;

	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame1].x = 66; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame1].y = 1036;
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame1].ancho = 76; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame1].alto = 89;

	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame2].x = 146; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame2].y = 1036;
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame2].ancho = 84; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame2].alto = 89;

	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame3].x = 232; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame3].y = 1036;
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame3].ancho = 63; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame3].alto = 89;

	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame4].x = 297; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame4].y = 1036;
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame4].ancho = 67; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame4].alto = 89;

	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame5].x = 369; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame5].y = 1036;
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame5].ancho = 71; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame5].alto = 89;

	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame6].x = 440; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame6].y = 1036;
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame6].ancho = 58; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame6].alto = 89;

	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame7].x = 502; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame7].y = 1036;
	Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame7].ancho = 80; Axel.FrameSpriteArray[Axel.idAnimacionAtacando][Frame7].alto = 89;

	//Marluxia
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame0].x = 2; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame0].y = 727;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame0].ancho = 82; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame0].alto = 140;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame1].x = 87; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame1].y = 727;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame1].ancho = 71; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame1].alto = 140;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame2].x = 160; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame2].y = 727;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame2].ancho = 55; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame2].alto = 140;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame3].x = 221; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame3].y = 727;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame3].ancho = 82; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame3].alto = 140;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame4].x = 321; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame4].y = 727;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame4].ancho = 92; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame4].alto = 140;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame5].x = 418; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame5].y = 727;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame5].ancho = 95; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame5].alto = 140;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame6].x = 518; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame6].y = 727;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame6].ancho = 106; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame6].alto = 140;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame7].x = 631; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame7].y = 727;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame7].ancho = 91; Marluxia.FrameSpriteArray[Marluxia.idAnimacionAtacando][Frame7].alto = 140;
#pragma endregion

	// -------- Animacion 7 Defend -------- //
#pragma region DefendAnim
	//Sora
	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame0].x = 535; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame0].y = 848;
	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame0].ancho = 41; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame0].alto = 47;

	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame1].x = 582; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame1].y = 848;
	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame1].ancho = 41; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame1].alto = 47;

	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame2].x = 628; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame2].y = 848;
	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame2].ancho = 68; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame2].alto = 47;

	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame3].x = 701; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame3].y = 848;
	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame3].ancho = 76; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame3].alto = 47;

	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame4].x = 785; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame4].y = 848;
	Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame4].ancho = 46; Sora.FrameSpriteArray[Sora.idAnimacionDefendiendo][Frame4].alto = 47;

	//Riku
	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame0].x = 91; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame0].y = 3967;
	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame0].ancho = 44; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame0].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame1].x = 151; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame1].y = 3967;
	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame1].ancho = 54; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame1].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame2].x = 216; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame2].y = 3967;
	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame2].ancho = 43; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame2].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame3].x = 272; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame3].y = 3967;
	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame3].ancho = 48; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame3].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame4].x = 341; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame4].y = 3967;
	Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame4].ancho = 35; Riku.FrameSpriteArray[Riku.idAnimacionDefendiendo][Frame4].alto = 74;

	//Axel
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame0].x = 7; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame0].y = 872;
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame0].ancho = 71; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame0].alto = 87;

	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame1].x = 81; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame1].y = 872;
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame1].ancho = 48; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame1].alto = 87;

	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame2].x = 137; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame2].y = 872;
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame2].ancho = 47; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame2].alto = 87;

	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame3].x = 199; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame3].y = 872;
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame3].ancho = 76; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame3].alto = 87;

	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame4].x = 278; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame4].y = 872;
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame4].ancho = 76; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame4].alto = 87;

	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame5].x = 359; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame5].y = 872;
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame5].ancho = 76; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame5].alto = 87;

	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame6].x = 441; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame6].y = 872;
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame6].ancho = 76; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame6].alto = 87;

	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame7].x = 520; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame7].y = 872;
	Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame7].ancho = 76; Axel.FrameSpriteArray[Axel.idAnimacionDefendiendo][Frame7].alto = 87;

	//Marluxia
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame0].x = 1; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame0].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame0].ancho = 85; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame0].alto = 103;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame1].x = 90; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame1].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame1].ancho = 72; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame1].alto = 103;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame2].x = 164; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame2].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame2].ancho = 59; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame2].alto = 103;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame3].x = 227; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame3].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame3].ancho = 59; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame3].alto = 103;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame4].x = 291; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame4].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame4].ancho = 59; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame4].alto = 103;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame5].x = 355; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame5].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame5].ancho = 59; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame5].alto = 103;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame6].x = 419; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame6].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame6].ancho = 59; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame6].alto = 103;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame7].x = 483; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame7].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame7].ancho = 59; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame7].alto = 103;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame8].x = 547; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame8].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame8].ancho = 64; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame8].alto = 103;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame9].x = 619; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame9].y = 881;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame9].ancho = 57; Marluxia.FrameSpriteArray[Marluxia.idAnimacionDefendiendo][Frame9].alto = 103;
#pragma endregion

	// -------- Animacion 8 Heal -------- //
#pragma region HealAnim
	//Sora
	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame0].x = 535; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame0].y = 1364;
	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame0].ancho = 31; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame0].alto = 59;

	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame1].x = 572; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame1].y = 1364;
	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame1].ancho = 34; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame1].alto = 59;

	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame2].x = 611; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame2].y = 1364;
	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame2].ancho = 36; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame2].alto = 59;

	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame3].x = 653; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame3].y = 1364;
	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame3].ancho = 32; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame3].alto = 59;

	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame4].x = 692; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame4].y = 1364;
	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame4].ancho = 32; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame4].alto = 59;

	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame5].x = 731; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame5].y = 1364;
	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame5].ancho = 38; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame5].alto = 59;

	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame6].x = 775; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame6].y = 1364;
	Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame6].ancho = 33; Sora.FrameSpriteArray[Sora.idAnimacionCurando][Frame6].alto = 59;

	//Riku
	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame0].x = 73; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame0].y = 4063;
	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame0].ancho = 40; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame0].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame1].x = 124; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame1].y = 4063;
	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame1].ancho = 45; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame1].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame2].x = 179; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame2].y = 4063;
	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame2].ancho = 38; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame2].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame3].x = 231; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame3].y = 4063;
	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame3].ancho = 34; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame3].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame4].x = 280; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame4].y = 4063;
	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame4].ancho = 31; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame4].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame5].x = 324; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame5].y = 4063;
	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame5].ancho = 37; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame5].alto = 74;

	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame6].x = 372; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame6].y = 4063;
	Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame6].ancho = 40; Riku.FrameSpriteArray[Riku.idAnimacionCurando][Frame6].alto = 74;

	//Axel
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame0].x = 2; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame0].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame0].ancho = 80; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame0].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame1].x = 86; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame1].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame1].ancho = 64; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame1].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame2].x = 159; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame2].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame2].ancho = 47; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame2].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame3].x = 209; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame3].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame3].ancho = 67; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame3].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame4].x = 279; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame4].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame4].ancho = 85; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame4].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame5].x = 366; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame5].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame5].ancho = 85; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame5].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame6].x = 279; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame6].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame6].ancho = 85; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame6].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame7].x = 209; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame7].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame7].ancho = 67; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame7].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame8].x = 159; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame8].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame8].ancho = 47; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame8].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame9].x = 86; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame9].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame9].ancho = 64; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame9].alto = 95;

	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame10].x = 2; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame10].y = 1175;
	Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame10].ancho = 80; Axel.FrameSpriteArray[Axel.idAnimacionCurando][Frame10].alto = 95;

	//Marluxia
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame0].x = 3; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame0].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame0].ancho = 84; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame0].alto = 90;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame1].x = 95; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame1].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame1].ancho = 78; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame1].alto = 90;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame2].x = 179; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame2].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame2].ancho = 63; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame2].alto = 90;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame3].x = 249; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame3].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame3].ancho = 39; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame3].alto = 90;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame4].x = 298; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame4].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame4].ancho = 63; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame4].alto = 90;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame5].x = 367; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame5].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame5].ancho = 76; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame5].alto = 90;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame6].x = 448; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame6].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame6].ancho = 86; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame6].alto = 90;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame7].x = 539; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame7].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame7].ancho = 83; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame7].alto = 90;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame8].x = 629; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame8].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame8].ancho = 45; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame8].alto = 90;

	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame9].x = 677; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame9].y = 1060;
	Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame9].ancho = 57; Marluxia.FrameSpriteArray[Marluxia.idAnimacionCurando][Frame9].alto = 90;
#pragma endregion

	// -------- Animacion 9 Die -------- //
#pragma region DieAnim
//Sora
	Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame0].x = 773; Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame0].y = 1629;
	Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame0].ancho = 55; Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame0].alto = 24;

	Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame1].x = 832; Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame1].y = 1629;
	Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame1].ancho = 53; Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame1].alto = 24;

	Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame2].x = 889; Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame2].y = 1629;
	Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame2].ancho = 54; Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame2].alto = 24;

	Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame3].x = 832; Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame3].y = 1629;
	Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame3].ancho = 53; Sora.FrameSpriteArray[Sora.idAnimacionMuerto][Frame3].alto = 24;
#pragma endregion
}

void CargaFramesSpriteUI() {

	UIHojaSprite = gdipLoad((WCHAR*)UI_BMP.c_str());

	BasicUI.XCurrentCoordDraw = 295;
	BasicUI.YCurrentCoordDraw = 530;

	Flecha.XCurrentCoordDraw = 315;
	Flecha.YCurrentCoordDraw = 546;

	BarraEnemy.XCurrentCoordDraw = 18;
	BarraEnemy.YCurrentCoordDraw = 49;

	BarraPlayer.XCurrentCoordDraw = 436;
	BarraPlayer.YCurrentCoordDraw = 49;

	VidaEnemy.XCurrentCoordDraw = 24;
	VidaEnemy.YCurrentCoordDraw = 54;

	VidaPlayer.XCurrentCoordDraw = 442;
	VidaPlayer.YCurrentCoordDraw = 54;

	Numero1.XCurrentCoordDraw = 0;
	Numero1.YCurrentCoordDraw = 0;

	Numero2.XCurrentCoordDraw = 0;
	Numero2.YCurrentCoordDraw = 0;

	PlayerStock.XCurrentCoordDraw = 670;
	PlayerStock.YCurrentCoordDraw = 100;

	BasicUI.FrameSpriteArray = new FrameArray * [BasicUI.Estatico];
	Flecha.FrameSpriteArray = new FrameArray * [Flecha.AnimacionesFlecha];
	BarraPlayer.FrameSpriteArray = new FrameArray * [BarraPlayer.Estatico];
	BarraEnemy.FrameSpriteArray = new FrameArray * [BarraEnemy.Estatico];
	VidaPlayer.FrameSpriteArray = new FrameArray * [VidaPlayer.Estatico];
	VidaEnemy.FrameSpriteArray = new FrameArray * [VidaEnemy.Estatico];
	Numero1.FrameSpriteArray = new FrameArray * [Numero1.AnimacionesNumeros];
	Numero2.FrameSpriteArray = new FrameArray * [Numero2.AnimacionesNumeros];
	PlayerStock.FrameSpriteArray = new FrameArray * [PlayerStock.Estatico];
	EnemyStock1.FrameSpriteArray = new FrameArray * [EnemyStock1.Estatico];
	EnemyStock2.FrameSpriteArray = new FrameArray * [EnemyStock2.Estatico];
	EnemyStock3.FrameSpriteArray = new FrameArray * [EnemyStock3.Estatico];

	BasicUI.FrameSpriteArray[BasicUI.idBasicUI] = new FrameArray[BasicUI.FramesBasicUI];
	Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha] = new FrameArray[Flecha.FramesAnimacionFlecha];
	BarraPlayer.FrameSpriteArray[BarraPlayer.idBasicUI] = new FrameArray[BarraPlayer.FramesBasicUI];
	BarraEnemy.FrameSpriteArray[BarraEnemy.idBasicUI] = new FrameArray[BarraEnemy.FramesBasicUI];
	VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI] = new FrameArray[VidaPlayer.FramesBasicUI];
	VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI] = new FrameArray[VidaEnemy.FramesBasicUI];
	PlayerStock.FrameSpriteArray[PlayerStock.idBasicUI] = new FrameArray[PlayerStock.FramesBasicUI];
	EnemyStock1.FrameSpriteArray[EnemyStock1.idBasicUI] = new FrameArray[EnemyStock1.FramesBasicUI];
	EnemyStock2.FrameSpriteArray[EnemyStock2.idBasicUI] = new FrameArray[EnemyStock2.FramesBasicUI];
	EnemyStock3.FrameSpriteArray[EnemyStock3.idBasicUI] = new FrameArray[EnemyStock3.FramesBasicUI];

	Numero1.FrameSpriteArray[Numero1.idAnimacion0] = new FrameArray[Numero1.FramesAnimacionNumeros];
	Numero1.FrameSpriteArray[Numero1.idAnimacion1] = new FrameArray[Numero1.FramesAnimacionNumeros];
	Numero1.FrameSpriteArray[Numero1.idAnimacion2] = new FrameArray[Numero1.FramesAnimacionNumeros];
	Numero1.FrameSpriteArray[Numero1.idAnimacion3] = new FrameArray[Numero1.FramesAnimacionNumeros];
	Numero1.FrameSpriteArray[Numero1.idAnimacion4] = new FrameArray[Numero1.FramesAnimacionNumeros];
	Numero1.FrameSpriteArray[Numero1.idAnimacion5] = new FrameArray[Numero1.FramesAnimacionNumeros];
	Numero1.FrameSpriteArray[Numero1.idAnimacion6] = new FrameArray[Numero1.FramesAnimacionNumeros];
	Numero1.FrameSpriteArray[Numero1.idAnimacion7] = new FrameArray[Numero1.FramesAnimacionNumeros];
	Numero1.FrameSpriteArray[Numero1.idAnimacion8] = new FrameArray[Numero1.FramesAnimacionNumeros];
	Numero1.FrameSpriteArray[Numero1.idAnimacion9] = new FrameArray[Numero1.FramesAnimacionNumeros];

	Numero2.FrameSpriteArray[Numero2.idAnimacion0] = new FrameArray[Numero2.FramesAnimacionNumeros];
	Numero2.FrameSpriteArray[Numero2.idAnimacion1] = new FrameArray[Numero2.FramesAnimacionNumeros];
	Numero2.FrameSpriteArray[Numero2.idAnimacion2] = new FrameArray[Numero2.FramesAnimacionNumeros];
	Numero2.FrameSpriteArray[Numero2.idAnimacion3] = new FrameArray[Numero2.FramesAnimacionNumeros];
	Numero2.FrameSpriteArray[Numero2.idAnimacion4] = new FrameArray[Numero2.FramesAnimacionNumeros];
	Numero2.FrameSpriteArray[Numero2.idAnimacion5] = new FrameArray[Numero2.FramesAnimacionNumeros];
	Numero2.FrameSpriteArray[Numero2.idAnimacion6] = new FrameArray[Numero2.FramesAnimacionNumeros];
	Numero2.FrameSpriteArray[Numero2.idAnimacion7] = new FrameArray[Numero2.FramesAnimacionNumeros];
	Numero2.FrameSpriteArray[Numero2.idAnimacion8] = new FrameArray[Numero2.FramesAnimacionNumeros];
	Numero2.FrameSpriteArray[Numero2.idAnimacion9] = new FrameArray[Numero2.FramesAnimacionNumeros];

	// -------- UI Estatico -------- //
	BasicUI.FrameSpriteArray[BasicUI.idBasicUI][Frame0].x = 17; BasicUI.FrameSpriteArray[BasicUI.idBasicUI][Frame0].y = 37;
	BasicUI.FrameSpriteArray[BasicUI.idBasicUI][Frame0].ancho = 78; BasicUI.FrameSpriteArray[BasicUI.idBasicUI][Frame0].alto = 65;

	BarraPlayer.FrameSpriteArray[BarraPlayer.idBasicUI][Frame0].x = 186; BarraPlayer.FrameSpriteArray[BarraPlayer.idBasicUI][Frame0].y = 17;
	BarraPlayer.FrameSpriteArray[BarraPlayer.idBasicUI][Frame0].ancho = 186; BarraPlayer.FrameSpriteArray[BarraPlayer.idBasicUI][Frame0].alto = 18;

	BarraEnemy.FrameSpriteArray[BarraEnemy.idBasicUI][Frame0].x = 186; BarraEnemy.FrameSpriteArray[BarraEnemy.idBasicUI][Frame0].y = 17;
	BarraEnemy.FrameSpriteArray[BarraEnemy.idBasicUI][Frame0].ancho = 186; BarraEnemy.FrameSpriteArray[BarraEnemy.idBasicUI][Frame0].alto = 18;

	VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].x = 2; VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].y = 3;
	VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho = 180; VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].alto = 13;

	VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].x = 2; VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].y = 3;
	VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 180; VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].alto = 13;

	PlayerStock.FrameSpriteArray[PlayerStock.idBasicUI][Frame0].x = 503; PlayerStock.FrameSpriteArray[PlayerStock.idBasicUI][Frame0].y = 391;
	PlayerStock.FrameSpriteArray[PlayerStock.idBasicUI][Frame0].ancho = 66; PlayerStock.FrameSpriteArray[PlayerStock.idBasicUI][Frame0].alto = 88;

	EnemyStock1.FrameSpriteArray[EnemyStock1.idBasicUI][Frame0].x = 58; EnemyStock1.FrameSpriteArray[EnemyStock1.idBasicUI][Frame0].y = 16;
	EnemyStock1.FrameSpriteArray[EnemyStock1.idBasicUI][Frame0].ancho = 60; EnemyStock1.FrameSpriteArray[EnemyStock1.idBasicUI][Frame0].alto = 83;

	EnemyStock2.FrameSpriteArray[EnemyStock2.idBasicUI][Frame0].x = 542; EnemyStock2.FrameSpriteArray[EnemyStock2.idBasicUI][Frame0].y = 5;
	EnemyStock2.FrameSpriteArray[EnemyStock2.idBasicUI][Frame0].ancho = 66; EnemyStock2.FrameSpriteArray[EnemyStock2.idBasicUI][Frame0].alto = 96;

	EnemyStock3.FrameSpriteArray[EnemyStock3.idBasicUI][Frame0].x = 376; EnemyStock3.FrameSpriteArray[EnemyStock3.idBasicUI][Frame0].y = 3;
	EnemyStock3.FrameSpriteArray[EnemyStock3.idBasicUI][Frame0].ancho = 60; EnemyStock3.FrameSpriteArray[EnemyStock3.idBasicUI][Frame0].alto = 87;

	// -------- Animacion Numeros -------- //

	//Num1
	//0
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame0].x = 97; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame0].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame1].x = 97; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame1].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame2].x = 97; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame2].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame3].x = 97; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame3].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame4].x = 97; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame4].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame5].x = 97; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame5].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame6].x = 97; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame6].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame7].x = 97; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame7].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame8].x = 97; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame8].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion0][Frame8].alto = 12;

	//1
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame0].x = 108; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame0].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame1].x = 108; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame1].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame2].x = 108; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame2].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame3].x = 108; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame3].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame4].x = 108; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame4].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame5].x = 108; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame5].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame6].x = 108; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame6].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame7].x = 108; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame7].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame8].x = 108; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame8].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion1][Frame8].alto = 12;

	//2
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame0].x = 118; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame0].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame1].x = 118; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame1].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame2].x = 118; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame2].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame3].x = 118; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame3].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame4].x = 118; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame4].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame5].x = 118; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame5].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame6].x = 118; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame6].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame7].x = 118; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame7].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame8].x = 118; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame8].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion2][Frame8].alto = 12;

	//3
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame0].x = 129; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame0].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame1].x = 129; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame1].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame2].x = 129; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame2].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame3].x = 129; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame3].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame4].x = 129; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame4].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame5].x = 129; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame5].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame6].x = 129; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame6].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame7].x = 129; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame7].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame8].x = 129; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame8].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion3][Frame8].alto = 12;

	//4
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame0].x = 140; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame0].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame1].x = 140; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame1].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame2].x = 140; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame2].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame3].x = 140; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame3].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame4].x = 140; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame4].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame5].x = 140; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame5].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame6].x = 140; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame6].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame7].x = 140; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame7].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame8].x = 140; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame8].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion4][Frame8].alto = 12;

	//5
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame0].x = 151; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame0].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame1].x = 151; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame1].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame2].x = 151; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame2].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame3].x = 151; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame3].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame4].x = 151; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame4].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame5].x = 151; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame5].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame6].x = 151; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame6].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame7].x = 151; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame7].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame8].x = 151; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame8].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion5][Frame8].alto = 12;

	//6
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame0].x = 162; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame0].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame1].x = 162; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame1].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame2].x = 162; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame2].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame3].x = 162; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame3].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame4].x = 162; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame4].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame5].x = 162; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame5].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame6].x = 162; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame6].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame7].x = 162; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame7].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame8].x = 162; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame8].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion6][Frame8].alto = 12;

	//7
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame0].x = 173; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame0].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame1].x = 173; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame1].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame2].x = 173; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame2].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame3].x = 173; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame3].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame4].x = 173; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame4].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame5].x = 173; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame5].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame6].x = 173; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame6].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame7].x = 173; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame7].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame8].x = 173; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame8].ancho = 9; Numero1.FrameSpriteArray[Numero1.idAnimacion7][Frame8].alto = 12;

	//8
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame0].x = 183; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame0].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame1].x = 183; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame1].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame2].x = 183; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame2].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame3].x = 183; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame3].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame4].x = 183; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame4].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame5].x = 183; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame5].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame6].x = 183; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame6].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame7].x = 183; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame7].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame8].x = 183; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame8].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion8][Frame8].alto = 12;

	//9
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame0].x = 194; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame0].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame0].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame0].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame1].x = 194; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame1].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame1].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame1].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame2].x = 194; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame2].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame2].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame2].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame3].x = 194; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame3].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame3].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame3].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame4].x = 194; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame4].y = 38;
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame4].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame4].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame5].x = 194; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame5].y = 51;
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame5].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame5].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame6].x = 194; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame6].y = 64;
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame6].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame6].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame7].x = 194; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame7].y = 77;
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame7].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame7].alto = 12;

	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame8].x = 194; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame8].y = 90;
	Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame8].ancho = 10; Numero1.FrameSpriteArray[Numero1.idAnimacion9][Frame8].alto = 12;


	//Num2
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame0].x = 97; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame0].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame1].x = 97; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame1].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame2].x = 97; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame2].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame3].x = 97; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame3].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame4].x = 97; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame4].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame5].x = 97; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame5].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame6].x = 97; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame6].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame7].x = 97; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame7].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame8].x = 97; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame8].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion0][Frame8].alto = 12;

	//1
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame0].x = 108; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame0].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame1].x = 108; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame1].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame2].x = 108; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame2].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame3].x = 108; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame3].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame4].x = 108; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame4].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame5].x = 108; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame5].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame6].x = 108; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame6].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame7].x = 108; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame7].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame8].x = 108; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame8].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion1][Frame8].alto = 12;

	//2
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame0].x = 118; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame0].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame1].x = 118; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame1].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame2].x = 118; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame2].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame3].x = 118; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame3].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame4].x = 118; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame4].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame5].x = 118; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame5].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame6].x = 118; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame6].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame7].x = 118; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame7].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame8].x = 118; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame8].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion2][Frame8].alto = 12;

	//3
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame0].x = 129; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame0].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame1].x = 129; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame1].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame2].x = 129; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame2].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame3].x = 129; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame3].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame4].x = 129; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame4].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame5].x = 129; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame5].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame6].x = 129; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame6].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame7].x = 129; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame7].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame8].x = 129; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame8].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion3][Frame8].alto = 12;

	//4
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame0].x = 140; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame0].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame1].x = 140; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame1].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame2].x = 140; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame2].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame3].x = 140; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame3].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame4].x = 140; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame4].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame5].x = 140; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame5].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame6].x = 140; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame6].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame7].x = 140; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame7].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame8].x = 140; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame8].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion4][Frame8].alto = 12;

	//5
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame0].x = 151; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame0].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame1].x = 151; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame1].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame2].x = 151; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame2].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame3].x = 151; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame3].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame4].x = 151; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame4].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame5].x = 151; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame5].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame6].x = 151; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame6].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame7].x = 151; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame7].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame8].x = 151; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame8].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion5][Frame8].alto = 12;

	//6
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame0].x = 162; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame0].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame1].x = 162; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame1].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame2].x = 162; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame2].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame3].x = 162; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame3].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame4].x = 162; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame4].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame5].x = 162; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame5].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame6].x = 162; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame6].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame7].x = 162; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame7].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame8].x = 162; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame8].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion6][Frame8].alto = 12;

	//7
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame0].x = 173; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame0].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame1].x = 173; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame1].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame2].x = 173; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame2].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame3].x = 173; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame3].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame4].x = 173; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame4].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame5].x = 173; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame5].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame6].x = 173; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame6].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame7].x = 173; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame7].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame8].x = 173; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame8].ancho = 9; Numero2.FrameSpriteArray[Numero2.idAnimacion7][Frame8].alto = 12;

	//8
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame0].x = 183; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame0].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame1].x = 183; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame1].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame2].x = 183; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame2].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame3].x = 183; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame3].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame4].x = 183; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame4].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame5].x = 183; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame5].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame6].x = 183; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame6].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame7].x = 183; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame7].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame8].x = 183; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame8].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion8][Frame8].alto = 12;

	//9
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame0].x = 194; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame0].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame0].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame0].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame1].x = 194; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame1].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame1].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame1].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame2].x = 194; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame2].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame2].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame2].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame3].x = 194; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame3].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame3].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame3].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame4].x = 194; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame4].y = 38;
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame4].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame4].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame5].x = 194; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame5].y = 51;
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame5].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame5].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame6].x = 194; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame6].y = 64;
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame6].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame6].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame7].x = 194; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame7].y = 77;
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame7].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame7].alto = 12;

	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame8].x = 194; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame8].y = 90;
	Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame8].ancho = 10; Numero2.FrameSpriteArray[Numero2.idAnimacion9][Frame8].alto = 12;

	// -------- Animacion Flecha -------- //
	Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame0].x = 6; Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame0].y = 44;
	Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame0].ancho = 8; Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame0].alto = 11;
	
	Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame1].x = 6; Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame1].y = 44;
	Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame1].ancho = 8; Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame1].alto = 11;
	
	Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame2].x = 6; Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame2].y = 44;
	Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame2].ancho = 8; Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame2].alto = 11;
	
	Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame3].x = 6; Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame3].y = 44;
	Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame3].ancho = 8; Flecha.FrameSpriteArray[Flecha.idAnimacionFlecha][Frame3].alto = 11;

}

void CargaFramesEfectos() {
	GameOverHojaSprite = gdipLoad((WCHAR*)GameOver_BMP.c_str());

	Corazon.XCurrentCoordDraw = 365;
	Corazon.YCurrentCoordDraw = 260;

	Guadaña.XCurrentCoordDraw = Marluxia.XCurrentCoordDraw - 123;
	Guadaña.YCurrentCoordDraw = Marluxia.YCurrentCoordDraw;

	Corazon.FrameSpriteArray = new FrameArray[Corazon.FramesAnimacionCorazon];
	Escudo.FrameSpriteArray = new FrameArray[Escudo.FramesAnimacionEscudo];
	Guadaña.FrameSpriteArray = new FrameArray[Guadaña.FramesAnimacionGuadaña];


	// ---------- Animacion Corazon ----------- //
	Corazon.FrameSpriteArray[Frame0].x = 2; Corazon.FrameSpriteArray[Frame0].y = 80;
	Corazon.FrameSpriteArray[Frame0].ancho = 38; Corazon.FrameSpriteArray[Frame0].alto = 46;

	Corazon.FrameSpriteArray[Frame1].x = 41; Corazon.FrameSpriteArray[Frame1].y = 80;
	Corazon.FrameSpriteArray[Frame1].ancho = 38; Corazon.FrameSpriteArray[Frame1].alto = 46;

	Corazon.FrameSpriteArray[Frame2].x = 80; Corazon.FrameSpriteArray[Frame2].y = 80;
	Corazon.FrameSpriteArray[Frame2].ancho = 38; Corazon.FrameSpriteArray[Frame2].alto = 46;

	Corazon.FrameSpriteArray[Frame3].x = 119; Corazon.FrameSpriteArray[Frame3].y = 80;
	Corazon.FrameSpriteArray[Frame3].ancho = 38; Corazon.FrameSpriteArray[Frame3].alto = 46;

	Corazon.FrameSpriteArray[Frame4].x = 158; Corazon.FrameSpriteArray[Frame4].y = 80;
	Corazon.FrameSpriteArray[Frame4].ancho = 38; Corazon.FrameSpriteArray[Frame4].alto = 46;

	Corazon.FrameSpriteArray[Frame5].x = 197; Corazon.FrameSpriteArray[Frame5].y = 80;
	Corazon.FrameSpriteArray[Frame5].ancho = 38; Corazon.FrameSpriteArray[Frame5].alto = 46;

	Corazon.FrameSpriteArray[Frame6].x = 236; Corazon.FrameSpriteArray[Frame6].y = 80;
	Corazon.FrameSpriteArray[Frame6].ancho = 38; Corazon.FrameSpriteArray[Frame6].alto = 46;

	Corazon.FrameSpriteArray[Frame7].x = 275; Corazon.FrameSpriteArray[Frame7].y = 80;
	Corazon.FrameSpriteArray[Frame7].ancho = 38; Corazon.FrameSpriteArray[Frame7].alto = 46;

	// ---------- Animacion Escudo ----------- //
	Escudo.FrameSpriteArray[Frame0].x = 505; Escudo.FrameSpriteArray[Frame0].y = 632;
	Escudo.FrameSpriteArray[Frame0].ancho = 60; Escudo.FrameSpriteArray[Frame0].alto = 16;

	Escudo.FrameSpriteArray[Frame1].x = 453; Escudo.FrameSpriteArray[Frame1].y = 616;
	Escudo.FrameSpriteArray[Frame1].ancho = 44; Escudo.FrameSpriteArray[Frame1].alto = 32;

	Escudo.FrameSpriteArray[Frame2].x = 418; Escudo.FrameSpriteArray[Frame2].y = 593;
	Escudo.FrameSpriteArray[Frame2].ancho = 32; Escudo.FrameSpriteArray[Frame2].alto = 56;

	Escudo.FrameSpriteArray[Frame3].x = 385; Escudo.FrameSpriteArray[Frame3].y = 584;
	Escudo.FrameSpriteArray[Frame3].ancho = 28; Escudo.FrameSpriteArray[Frame3].alto = 64;

	// ---------- Animacion Guadaña ----------- //
	Guadaña.FrameSpriteArray[Frame0].x = 32; Guadaña.FrameSpriteArray[Frame0].y = 283;
	Guadaña.FrameSpriteArray[Frame0].ancho = 76; Guadaña.FrameSpriteArray[Frame0].alto = 58;

	Guadaña.FrameSpriteArray[Frame1].x = 112; Guadaña.FrameSpriteArray[Frame1].y = 283;
	Guadaña.FrameSpriteArray[Frame1].ancho = 76; Guadaña.FrameSpriteArray[Frame1].alto = 58;

	Guadaña.FrameSpriteArray[Frame2].x = 193; Guadaña.FrameSpriteArray[Frame2].y = 283;
	Guadaña.FrameSpriteArray[Frame2].ancho = 76; Guadaña.FrameSpriteArray[Frame2].alto = 58;

	Guadaña.FrameSpriteArray[Frame3].x = 277; Guadaña.FrameSpriteArray[Frame3].y = 283;
	Guadaña.FrameSpriteArray[Frame3].ancho = 76; Guadaña.FrameSpriteArray[Frame3].alto = 58;

	Guadaña.FrameSpriteArray[Frame4].x = 362; Guadaña.FrameSpriteArray[Frame4].y = 283;
	Guadaña.FrameSpriteArray[Frame4].ancho = 76; Guadaña.FrameSpriteArray[Frame4].alto = 58;

	Guadaña.FrameSpriteArray[Frame5].x = 451; Guadaña.FrameSpriteArray[Frame5].y = 283;
	Guadaña.FrameSpriteArray[Frame5].ancho = 76; Guadaña.FrameSpriteArray[Frame5].alto = 58;

	Guadaña.FrameSpriteArray[Frame6].x = 541; Guadaña.FrameSpriteArray[Frame6].y = 283;
	Guadaña.FrameSpriteArray[Frame6].ancho = 76; Guadaña.FrameSpriteArray[Frame6].alto = 58;
}

/* Funcion principal. Encargada de hacer el redibujado en pantalla cada intervalo (o "tick") del timer que se haya creado.
	@param hWnd. Manejador de la ventana.
	*/
void DibujaPixeles()
{
	if (PantallaInicio) {
		TranScaleblt(ptrBufferPixelsWindow, (miStage.ImagenEscenario1.pixeles), 0, 0, 0, 0,
			miStage.ImagenEscenario1.ancho, miStage.ImagenEscenario1.alto, ANCHO_VENTANA, 
			miStage.ImagenEscenario1.ancho, 2, 2, false);
	}
	else if (InGame){

		switch (CurrentStage) {

		case Stage1: 
		{
			//Dibujamos el fondo
			TranScaleblt(ptrBufferPixelsWindow, (miStage.ImagenEscenario2.pixeles), 0, 0, 0, 0,
				miStage.ImagenEscenario2.ancho, miStage.ImagenEscenario2.alto, ANCHO_VENTANA,
				miStage.ImagenEscenario2.ancho, 2, 2, false);

			if (PlayerShield) {
				//1 = unica animacion que tiene
				TranScaleblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					(Sora.XCurrentCoordDraw - 50), (Sora.YCurrentCoordDraw - 30),
					Escudo.FrameSpriteArray[EscudoJugadorFrameActual].x, Escudo.FrameSpriteArray[EscudoJugadorFrameActual].y,
					Escudo.FrameSpriteArray[EscudoJugadorFrameActual].ancho, Escudo.FrameSpriteArray[EscudoJugadorFrameActual].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, true);
			}
			if (EnemyShield) {
				mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					(Riku.XCurrentCoordDraw + 100), (Riku.YCurrentCoordDraw - 10),
					Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].x, Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].y,
					Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].ancho, Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
			}

			if (SoraAnimacionActual == Hit || SoraAnimacionActual == RunningBack) { //Si es el turno del jugador, se toma prioridad y se dibujara en una capa arriba de la del enemigo y viceversa
				if (RikuAnimacionActual == GetHit) {
					Riku.XCurrentCoordDraw += 15;
					switch (RikuFrameActual) {
					case Frame1:
						Riku.XCurrentCoordDraw -= 40;
						mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
							Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
							ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
						Riku.XCurrentCoordDraw += 40;
						break;

					case Frame2:
						Riku.XCurrentCoordDraw -= 10;
						mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
							Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
							ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
						Riku.XCurrentCoordDraw += 10;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
							Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
							ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
						break;
					}
					Riku.XCurrentCoordDraw -= 15;
				}
				else {
					mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
						Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
						Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
						Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
						ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
				}

				if (SoraAnimacionActual == Hit) {
					switch (SoraFrameActual) {
					case Frame3:
						Sora.XCurrentCoordDraw -= 150;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 150;
						break;

					case Frame4:
						Sora.XCurrentCoordDraw -= 60;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 60;
						break;

					case Frame5:
						Sora.XCurrentCoordDraw -= 60;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 60;
						break;

					case Frame6:
						Sora.XCurrentCoordDraw -= 60;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 60;
						break;

					default:
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						break;
					}
				}
				else if (SoraAnimacionActual == RunningBack) {
					mirrorxblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
						Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
						ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, 0xFFFFFFFF);
				}
			}
			else if (RikuAnimacionActual == Hit || RikuAnimacionActual == RunningBack) {
				if (SoraAnimacionActual == GetHit) {
					Sora.YCurrentCoordDraw -= 14;
					if (SoraFrameActual == Frame5) {
						Sora.XCurrentCoordDraw -= 2;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 2;
					}
					else {
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
					}
					Sora.YCurrentCoordDraw += 14;
				}
				else {
					TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
						Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
						ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
				}
				
				if (RikuAnimacionActual == Hit) {
					Riku.YCurrentCoordDraw -= 30;
					Riku.XCurrentCoordDraw += 52;
					switch (RikuFrameActual) {
					case Frame0:
						Riku.XCurrentCoordDraw -= 120;
						mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
							Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
							ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
						Riku.XCurrentCoordDraw += 120;
						break;

					case Frame1:
						Riku.XCurrentCoordDraw -= 132;
						mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
							Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
							ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
						Riku.XCurrentCoordDraw += 132;
						break;

					case Frame2:
						Riku.XCurrentCoordDraw -= 100;
						mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
							Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
							ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
						Riku.XCurrentCoordDraw += 100;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
							Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
							ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
						break;
					}
					Riku.XCurrentCoordDraw -= 52;
					Riku.YCurrentCoordDraw += 30;
				}
				else if (RikuAnimacionActual == RunningBack){
					TranScaleblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
						Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
						Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
						Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
						ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, true);
				}
			}
			else {
				//Arregla algunas animaciones
				switch (SoraAnimacionActual) {

				case Intro:
					switch (SoraFrameActual) {
					case Frame14:
						Sora.XCurrentCoordDraw -= 1;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 1;
						break;

					case Frame15:
						Sora.XCurrentCoordDraw -= 12;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 12;
						break;

					default:
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						break;
					}
					break;

				case Heal:
					Sora.YCurrentCoordDraw -= 45;
					switch (SoraFrameActual) {
					case Frame1:
						Sora.XCurrentCoordDraw -= 6;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 6;
						break;

					case Frame2:
						Sora.XCurrentCoordDraw -= 6;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 6;
						break;

					case Frame5:
						Sora.XCurrentCoordDraw -= 18;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 18;
						break;

					case Frame6:
						Sora.XCurrentCoordDraw -= 6;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 6;
						break;

					default:
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						break;
					}
					Sora.YCurrentCoordDraw += 45;
					break;

				default:
					TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
						Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
						ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
					break;
				}

				//Arregla animaciones
				switch (RikuAnimacionActual) {

					case Defend:
						Riku.YCurrentCoordDraw -= 42;
						Riku.XCurrentCoordDraw += 15;
						if (RikuFrameActual == Frame1) {
							Riku.XCurrentCoordDraw -= 40;
							mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
								Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
								ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
							Riku.XCurrentCoordDraw += 40;
						}
						else {
							mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
								Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
								ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
						}
						Riku.YCurrentCoordDraw += 42;
						Riku.XCurrentCoordDraw -= 15;
						break;

					case Heal:
						Riku.YCurrentCoordDraw -= 27;
						Riku.XCurrentCoordDraw += 10;
						switch (RikuFrameActual) {
						case Frame3:
							Riku.XCurrentCoordDraw -= 16;
							mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
								Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
								ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
							Riku.XCurrentCoordDraw += 16;
							break;

						case Frame4:
							Riku.XCurrentCoordDraw -= 13;
							mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
								Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
								ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
							Riku.XCurrentCoordDraw += 13;
							break;

						case Frame5:
							Riku.XCurrentCoordDraw -= 18;
							mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
								Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
								ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
							Riku.XCurrentCoordDraw += 18;
							break;


						default:
							mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
								Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
								Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
								ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
							break;
						}
						Riku.YCurrentCoordDraw += 27;
						Riku.XCurrentCoordDraw -= 10;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
							Riku.XCurrentCoordDraw, Riku.YCurrentCoordDraw,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].x, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].y,
							Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].ancho, Riku.FrameSpriteArray[RikuAnimacionActual][RikuFrameActual].alto,
							ANCHO_VENTANA, Riku.HojaSprite.ancho, 3, 3, 0xFF367575);
					break;
				}
			}

			//Se dibuja el UI
			#pragma region UI_Draw
			if (PlayerTurn) {
				//Acciones
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					BasicUI.XCurrentCoordDraw, BasicUI.YCurrentCoordDraw,
					BasicUI.FrameSpriteArray[StaticUI][Frame0].x, BasicUI.FrameSpriteArray[StaticUI][Frame0].y,
					BasicUI.FrameSpriteArray[StaticUI][Frame0].ancho, BasicUI.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, false);

				//Flecha
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					Flecha.XCurrentCoordDraw, Flecha.YCurrentCoordDraw,
					Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].x, Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].y,
					Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].ancho, Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, true);
			}

			//Barra para jugador
			TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
				BarraPlayer.XCurrentCoordDraw, BarraPlayer.YCurrentCoordDraw,
				BarraPlayer.FrameSpriteArray[StaticUI][Frame0].x, BarraPlayer.FrameSpriteArray[StaticUI][Frame0].y,
				BarraPlayer.FrameSpriteArray[StaticUI][Frame0].ancho, BarraPlayer.FrameSpriteArray[StaticUI][Frame0].alto,
				ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);

			//Barra para enemigo
			TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
				BarraEnemy.XCurrentCoordDraw, BarraEnemy.YCurrentCoordDraw,
				BarraEnemy.FrameSpriteArray[StaticUI][Frame0].x, BarraEnemy.FrameSpriteArray[StaticUI][Frame0].y,
				BarraEnemy.FrameSpriteArray[StaticUI][Frame0].ancho, BarraEnemy.FrameSpriteArray[StaticUI][Frame0].alto,
				ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);

			if (ShowPlayerHealth) {
				//Barra verde de vida para jugador
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					VidaPlayer.XCurrentCoordDraw, VidaPlayer.YCurrentCoordDraw,
					VidaPlayer.FrameSpriteArray[StaticUI][Frame0].x, VidaPlayer.FrameSpriteArray[StaticUI][Frame0].y,
					VidaPlayer.FrameSpriteArray[StaticUI][Frame0].ancho, VidaPlayer.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);
			}
			if (ShowEnemyHealth) {
				//Barra verde de vida para enemigo
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					VidaEnemy.XCurrentCoordDraw, VidaEnemy.YCurrentCoordDraw,
					VidaEnemy.FrameSpriteArray[StaticUI][Frame0].x, VidaEnemy.FrameSpriteArray[StaticUI][Frame0].y,
					VidaEnemy.FrameSpriteArray[StaticUI][Frame0].ancho, VidaEnemy.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);
			}

			if (ShowUI) {
				//Sora Stock
				TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
					PlayerStock.XCurrentCoordDraw, PlayerStock.YCurrentCoordDraw,
					PlayerStock.FrameSpriteArray[StaticUI][Frame0].x, PlayerStock.FrameSpriteArray[StaticUI][Frame0].y,
					PlayerStock.FrameSpriteArray[StaticUI][Frame0].ancho, PlayerStock.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, Sora.HojaSprite.ancho, 2, 2, true);

				//Riku Stock
				TranScaleblt(ptrBufferPixelsWindow, (Riku.HojaSprite.pixeles),
					20, 115,
					EnemyStock1.FrameSpriteArray[StaticUI][Frame0].x, EnemyStock1.FrameSpriteArray[StaticUI][Frame0].y,
					EnemyStock1.FrameSpriteArray[StaticUI][Frame0].ancho, EnemyStock1.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, Riku.HojaSprite.ancho, 2, 2, true);

				//Axel Stock
				mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
					130, 90,
					EnemyStock2.FrameSpriteArray[StaticUI][Frame0].x, EnemyStock2.FrameSpriteArray[StaticUI][Frame0].y,
					EnemyStock2.FrameSpriteArray[StaticUI][Frame0].ancho, EnemyStock2.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, Axel.HojaSprite.ancho, 2, 2, 0xFF8080FF);

				//Marluxia Stock
				mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					260, 105,
					EnemyStock3.FrameSpriteArray[StaticUI][Frame0].x, EnemyStock3.FrameSpriteArray[StaticUI][Frame0].y,
					EnemyStock3.FrameSpriteArray[StaticUI][Frame0].ancho, EnemyStock3.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 2, 2, 0xFFC8BFE7);
			}

			if (ShowNums) {
				//Numero1
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					Numero1.XCurrentCoordDraw, Numero1.YCurrentCoordDraw,
					Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].x, Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].y,
					Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].ancho, Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, true);

				//Numero2
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					Numero2.XCurrentCoordDraw, Numero2.YCurrentCoordDraw,
					Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].x, Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].y,
					Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].ancho, Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, true);
			}
			break;
#pragma endregion
		}

		case Stage2:
		{
			//Dibujamos el fondo
			TranScaleblt(ptrBufferPixelsWindow, (miStage.ImagenEscenario3.pixeles), 0, 0, 0, 0,
				miStage.ImagenEscenario3.ancho, miStage.ImagenEscenario3.alto, ANCHO_VENTANA,
				miStage.ImagenEscenario3.ancho, 2, 2, false);

			if (PlayerShield) {
				//1 = unica animacion que tiene
				TranScaleblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					(Sora.XCurrentCoordDraw - 50), (Sora.YCurrentCoordDraw - 30),
					Escudo.FrameSpriteArray[EscudoJugadorFrameActual].x, Escudo.FrameSpriteArray[EscudoJugadorFrameActual].y,
					Escudo.FrameSpriteArray[EscudoJugadorFrameActual].ancho, Escudo.FrameSpriteArray[EscudoJugadorFrameActual].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, true);
			}
			if (EnemyShield) {
				mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					(Axel.XCurrentCoordDraw + 180), (Axel.YCurrentCoordDraw + 39),
					Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].x, Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].y,
					Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].ancho, Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
			}

			if (SoraAnimacionActual == Hit || SoraAnimacionActual == RunningBack) { //Si es el turno del jugador, se toma prioridad y se dibujara en una capa arriba de la del enemigo y viceversa
				if (AxelAnimacionActual == GetHit) {
					switch (AxelFrameActual) {

					case Frame0:
						Axel.XCurrentCoordDraw -= 18;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 18;
						break;

					case Frame1:
						Axel.XCurrentCoordDraw += 3;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 3;
						break;

					case Frame2:
						Axel.XCurrentCoordDraw -= 15;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 15;
						break;

					case Frame3:
						Axel.XCurrentCoordDraw -= 57;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 57;
						break;

					case Frame4:
						Axel.XCurrentCoordDraw -= 33;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 33;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						break;
					}
				}
				else {
					//Idle
					Axel.XCurrentCoordDraw -= 27;
					Axel.YCurrentCoordDraw += 21;
					switch (AxelFrameActual) {

					case Frame0:
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.YCurrentCoordDraw -= 3;
						break;

					case Frame1:
						Axel.XCurrentCoordDraw += 3;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 3;
						Axel.YCurrentCoordDraw -= 3;
						break;

					case Frame2:
						Axel.XCurrentCoordDraw += 9;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 9;
						Axel.YCurrentCoordDraw -= 3;
						break;

					case Frame3:
						Axel.XCurrentCoordDraw += 12;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 12;
						Axel.YCurrentCoordDraw += 3;
						break;

					case Frame4:
						Axel.XCurrentCoordDraw += 9;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 9;
						Axel.YCurrentCoordDraw += 3;
						break;

					case Frame5:
						Axel.XCurrentCoordDraw += 3;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 3;
						Axel.YCurrentCoordDraw += 3;
						break;
					}
					Axel.XCurrentCoordDraw += 27;
					Axel.YCurrentCoordDraw -= 21;
				}
				
				if (SoraAnimacionActual == Hit) {
					switch (SoraFrameActual) {
					case Frame3:
						Sora.XCurrentCoordDraw -= 150;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 150;
						break;

					case Frame4:
						Sora.XCurrentCoordDraw -= 60;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 60;
						break;

					case Frame5:
						Sora.XCurrentCoordDraw -= 60;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 60;
						break;

					case Frame6:
						Sora.XCurrentCoordDraw -= 60;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 60;
						break;

					default:
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						break;
					}
				}
				else if (SoraAnimacionActual == RunningBack) {
					mirrorxblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
						Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
						ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, 0xFFFFFFFF);
				}
			}
			else if (AxelAnimacionActual == Hit || AxelAnimacionActual == RunningBack) {
				if (SoraAnimacionActual == GetHit) {
					Sora.YCurrentCoordDraw -= 14;
					if (SoraFrameActual == Frame5) {
						Sora.XCurrentCoordDraw -= 2;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 2;
					}
					else {
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
					}
					Sora.YCurrentCoordDraw += 14;
				}
				else {
					TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
						Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
						ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
				}

				if (AxelAnimacionActual == Hit) {
					switch (AxelFrameActual) {

					case Frame1:
						Axel.XCurrentCoordDraw += 27;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 27;
						break;

					case Frame2:
						Axel.XCurrentCoordDraw += 45;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 45;
						break;

					case Frame3:
						Axel.XCurrentCoordDraw += 15;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 15;
						break;

					case Frame4:
						Axel.XCurrentCoordDraw += 24;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 24;
						break;

					case Frame5:
						Axel.XCurrentCoordDraw += 36;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 36;
						break;
					
					case Frame6:
						Axel.XCurrentCoordDraw -= 9;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 9;
						break;

					case Frame7:
						Axel.XCurrentCoordDraw -= 18;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 18;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						break;
					}
				}
				else if (AxelAnimacionActual == RunningBack){
					Axel.YCurrentCoordDraw -= 123;
					if (AxelFrameActual == 6) {
						Axel.XCurrentCoordDraw = 70;
					}
					mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
						Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
						Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
						Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
						ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
					Axel.YCurrentCoordDraw += 123;
				}
			}
			else {

				//Arregla algunas animaciones
				switch (SoraAnimacionActual) {

				case Intro:
					switch (SoraFrameActual) {
					case Frame14:
						Sora.XCurrentCoordDraw -= 1;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 1;
						break;

					case Frame15:
						Sora.XCurrentCoordDraw -= 12;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 12;
						break;

					default:
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						break;
					}
					break;

				case Heal:
					Sora.YCurrentCoordDraw -= 45;
					switch (SoraFrameActual) {
					case Frame1:
						Sora.XCurrentCoordDraw -= 6;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 6;
						break;

					case Frame2:
						Sora.XCurrentCoordDraw -= 6;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 6;
						break;

					case Frame5:
						Sora.XCurrentCoordDraw -= 18;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 18;
						break;

					case Frame6:
						Sora.XCurrentCoordDraw -= 6;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 6;
						break;

					default:
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						break;
					}
					Sora.YCurrentCoordDraw += 45;
					break;

				default:
					TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
						Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
						ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
					break;
				}

				//Arregla animaciones
				switch (AxelAnimacionActual) {

				case Intro:
				{
					switch (AxelFrameActual) {

					case Frame1:
						Axel.XCurrentCoordDraw -= 6;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 6;
						break;

					case Frame2:
						Axel.XCurrentCoordDraw -= 27;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 27;
						break;

					case Frame3:
						Axel.XCurrentCoordDraw += 15;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 15;
						break;

					case Frame5:
						Axel.XCurrentCoordDraw -= 15;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 15;
						break;

					case Frame6:
						Axel.XCurrentCoordDraw -= 15;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 15;
						break;

					case Frame7:
						Axel.XCurrentCoordDraw -= 15;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 15;
						break;

					case Frame8:
						Axel.XCurrentCoordDraw -= 33;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 33;
						break;

					case Frame9:
						Axel.XCurrentCoordDraw -= 33;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 33;
						break;

					case Frame10:
						Axel.XCurrentCoordDraw -= 27;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 27;
						break;

					case Frame11:
						Axel.XCurrentCoordDraw -= 36;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 36;
						break;

					case Frame12:
						Axel.XCurrentCoordDraw -= 36;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 36;
						break;

					case Frame13:
						Axel.XCurrentCoordDraw -= 42;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 42;
						break;

					case Frame14:
						Axel.XCurrentCoordDraw -= 42;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 42;
						break;

					case Frame15:
						Axel.XCurrentCoordDraw -= 42;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 42;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						break;
					}
					break;
				}

				case Idle:
				{
					Axel.XCurrentCoordDraw -= 27;
					Axel.YCurrentCoordDraw += 21;
					switch (AxelFrameActual) {

					case Frame0:
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.YCurrentCoordDraw -= 3;
						break;

					case Frame1:
						Axel.XCurrentCoordDraw += 3;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 3;
						Axel.YCurrentCoordDraw -= 3;
						break;

					case Frame2:
						Axel.XCurrentCoordDraw += 9;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 9;
						Axel.YCurrentCoordDraw -= 3;
						break;

					case Frame3:
						Axel.XCurrentCoordDraw += 12;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 12;
						Axel.YCurrentCoordDraw += 3;
						break;

					case Frame4:
						Axel.XCurrentCoordDraw += 9;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 9;
						Axel.YCurrentCoordDraw += 3;
						break;

					case Frame5:
						Axel.XCurrentCoordDraw += 3;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 3;
						Axel.YCurrentCoordDraw += 3;
						break;
					}
					Axel.XCurrentCoordDraw += 27;
					Axel.YCurrentCoordDraw -= 21;
					break;
				}

				case Running:
				{
					Axel.YCurrentCoordDraw -= 123;
					if (AxelFrameActual == 6) {
						Axel.XCurrentCoordDraw = 350;
					}
					mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
						Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
						Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
						Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
						ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
					Axel.YCurrentCoordDraw += 123;
					break;
				}

				case Defend:
				{
					Axel.YCurrentCoordDraw -= 21;
					switch (AxelFrameActual) {
					case Frame0:
						Axel.XCurrentCoordDraw -= 51;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 51;
						break;

					case Frame1:
						Axel.XCurrentCoordDraw += 18;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 18;
						break;

					case Frame2:
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						break;

					default:
						Axel.XCurrentCoordDraw -= 39;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 39;
						break;
					}
					Axel.YCurrentCoordDraw += 21;
					break;
				}

				case Heal:
				{
					Axel.YCurrentCoordDraw -= 45;
					switch (AxelFrameActual) {

					case Frame0:
						Axel.XCurrentCoordDraw -= 24;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 24;
						break;

					case Frame1:
						Axel.XCurrentCoordDraw += 6;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 6;
						break;

					case Frame2:
						Axel.XCurrentCoordDraw += 12;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 12;
						break;

					case Frame3:
						Axel.XCurrentCoordDraw -= 9;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 9;
						break;

					case Frame4:
						Axel.XCurrentCoordDraw -= 42;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 42;
						break;

					case Frame5:
						Axel.XCurrentCoordDraw -= 42;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 42;
						break;

					case Frame6:
						Axel.XCurrentCoordDraw -= 42;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 42;
						break;

					case Frame7:
						Axel.XCurrentCoordDraw -= 9;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 9;
						break;

					case Frame8:
						Axel.XCurrentCoordDraw += 12;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 12;
						break;

					case Frame9:
						Axel.XCurrentCoordDraw += 6;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw -= 6;
						break;

					case Frame10:
						Axel.XCurrentCoordDraw -= 24;
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						Axel.XCurrentCoordDraw += 24;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
							Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
							Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
							ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
						break;
					}
					Axel.YCurrentCoordDraw += 45;
					break;
				}

				default:
					mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
						Axel.XCurrentCoordDraw, Axel.YCurrentCoordDraw,
						Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].x, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].y,
						Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].ancho, Axel.FrameSpriteArray[AxelAnimacionActual][AxelFrameActual].alto,
						ANCHO_VENTANA, Axel.HojaSprite.ancho, 3, 3, 0xFF8080FF);
					break;
				}
			}

			//UI
			if (PlayerTurn) {
				//Acciones
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					BasicUI.XCurrentCoordDraw, BasicUI.YCurrentCoordDraw,
					BasicUI.FrameSpriteArray[StaticUI][Frame0].x, BasicUI.FrameSpriteArray[StaticUI][Frame0].y,
					BasicUI.FrameSpriteArray[StaticUI][Frame0].ancho, BasicUI.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, false);

				//Flecha
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					Flecha.XCurrentCoordDraw, Flecha.YCurrentCoordDraw,
					Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].x, Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].y,
					Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].ancho, Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, true);
			}

			//Barra para jugador
			TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
				BarraPlayer.XCurrentCoordDraw, BarraPlayer.YCurrentCoordDraw,
				BarraPlayer.FrameSpriteArray[StaticUI][Frame0].x, BarraPlayer.FrameSpriteArray[StaticUI][Frame0].y,
				BarraPlayer.FrameSpriteArray[StaticUI][Frame0].ancho, BarraPlayer.FrameSpriteArray[StaticUI][Frame0].alto,
				ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);

			//Barra para enemigo
			TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
				BarraEnemy.XCurrentCoordDraw, BarraEnemy.YCurrentCoordDraw,
				BarraEnemy.FrameSpriteArray[StaticUI][Frame0].x, BarraEnemy.FrameSpriteArray[StaticUI][Frame0].y,
				BarraEnemy.FrameSpriteArray[StaticUI][Frame0].ancho, BarraEnemy.FrameSpriteArray[StaticUI][Frame0].alto,
				ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);

			if (ShowPlayerHealth) {
				//Barra verde de vida para jugador
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					VidaPlayer.XCurrentCoordDraw, VidaPlayer.YCurrentCoordDraw,
					VidaPlayer.FrameSpriteArray[StaticUI][Frame0].x, VidaPlayer.FrameSpriteArray[StaticUI][Frame0].y,
					VidaPlayer.FrameSpriteArray[StaticUI][Frame0].ancho, VidaPlayer.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);
			}
			if (ShowEnemyHealth) {
				//Barra verde de vida para enemigo
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					VidaEnemy.XCurrentCoordDraw, VidaEnemy.YCurrentCoordDraw,
					VidaEnemy.FrameSpriteArray[StaticUI][Frame0].x, VidaEnemy.FrameSpriteArray[StaticUI][Frame0].y,
					VidaEnemy.FrameSpriteArray[StaticUI][Frame0].ancho, VidaEnemy.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);
			}

			if (ShowUI) {

				//Sora Stock
				TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
					PlayerStock.XCurrentCoordDraw, PlayerStock.YCurrentCoordDraw,
					PlayerStock.FrameSpriteArray[StaticUI][Frame0].x, PlayerStock.FrameSpriteArray[StaticUI][Frame0].y,
					PlayerStock.FrameSpriteArray[StaticUI][Frame0].ancho, PlayerStock.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, Sora.HojaSprite.ancho, 2, 2, true);

				//Axel Stock
				mirrorxblt(ptrBufferPixelsWindow, (Axel.HojaSprite.pixeles),
					20, 90,
					EnemyStock2.FrameSpriteArray[StaticUI][Frame0].x, EnemyStock2.FrameSpriteArray[StaticUI][Frame0].y,
					EnemyStock2.FrameSpriteArray[StaticUI][Frame0].ancho, EnemyStock2.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, Axel.HojaSprite.ancho, 2, 2, 0xFF8080FF);

				//Marluxia Stock
				mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					150, 105,
					EnemyStock3.FrameSpriteArray[StaticUI][Frame0].x, EnemyStock3.FrameSpriteArray[StaticUI][Frame0].y,
					EnemyStock3.FrameSpriteArray[StaticUI][Frame0].ancho, EnemyStock3.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 2, 2, 0xFFC8BFE7);
			}

			if (ShowNums) {
				//Numero1
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					Numero1.XCurrentCoordDraw, Numero1.YCurrentCoordDraw,
					Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].x, Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].y,
					Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].ancho, Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, true);

				//Numero2
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					Numero2.XCurrentCoordDraw, Numero2.YCurrentCoordDraw,
					Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].x, Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].y,
					Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].ancho, Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, true);
			}
			break;
		}

		case Stage3:
		{
			//Dibujamos el fondo			
			TranScaleblt(ptrBufferPixelsWindow, (miStage.ImagenEscenario4.pixeles), 0, 0, 0, 0,
				miStage.ImagenEscenario4.ancho, miStage.ImagenEscenario4.alto, ANCHO_VENTANA,
				miStage.ImagenEscenario4.ancho, 2, 2, false);

			if (ShowIntroMarluxia) {
				mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					Guadaña.XCurrentCoordDraw, Guadaña.YCurrentCoordDraw,
					Guadaña.FrameSpriteArray[GuadañaFrameActual].x, Guadaña.FrameSpriteArray[GuadañaFrameActual].y,
					Guadaña.FrameSpriteArray[GuadañaFrameActual].ancho, Guadaña.FrameSpriteArray[GuadañaFrameActual].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFF0000FF);
			}

			if (PlayerShield) {
				//1 = unica animacion que tiene
				TranScaleblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					(Sora.XCurrentCoordDraw - 50), (Sora.YCurrentCoordDraw - 30),
					Escudo.FrameSpriteArray[EscudoJugadorFrameActual].x, Escudo.FrameSpriteArray[EscudoJugadorFrameActual].y,
					Escudo.FrameSpriteArray[EscudoJugadorFrameActual].ancho, Escudo.FrameSpriteArray[EscudoJugadorFrameActual].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, true);
			}
			if (EnemyShield) {
				mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					(Marluxia.XCurrentCoordDraw + 198), (Marluxia.YCurrentCoordDraw + 14),
					Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].x, Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].y,
					Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].ancho, Escudo.FrameSpriteArray[EscudoEnemigoFrameActual].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
			}

			if (SoraAnimacionActual == Hit || SoraAnimacionActual == RunningBack) { //Si es el turno del jugador, se toma prioridad y se dibujara en una capa arriba de la del enemigo y viceversa
				if (MarluxiaAnimacionActual == GetHit) {
					Marluxia.YCurrentCoordDraw -= 36;
					switch (MarluxiaFrameActual) {

					case Frame0:
						Marluxia.XCurrentCoordDraw += 39;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 39;
						break;

					case Frame1:
						Marluxia.XCurrentCoordDraw -= 12;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 12;
						break;

					case Frame2:
						Marluxia.XCurrentCoordDraw -= 21;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 21;
						break;

					case Frame3:
						Marluxia.XCurrentCoordDraw -= 48;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 48;
						break;

					case Frame4:
						Marluxia.XCurrentCoordDraw += 48;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 48;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						break;
					}
					Marluxia.YCurrentCoordDraw += 36;
				}
				else {
					//Idle
					switch (MarluxiaFrameActual) {
					case Frame0:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.YCurrentCoordDraw -= 3;
						break;

					case Frame1:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.YCurrentCoordDraw -= 3;
						break;

					case Frame3:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.YCurrentCoordDraw += 3;
						break;

					case Frame4:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.YCurrentCoordDraw += 3;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						break;
					}
				}

				if (SoraAnimacionActual == Hit) {
					switch (SoraFrameActual) {
					case Frame3:
						Sora.XCurrentCoordDraw -= 150;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 150;
						break;

					case Frame4:
						Sora.XCurrentCoordDraw -= 60;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 60;
						break;

					case Frame5:
						Sora.XCurrentCoordDraw -= 60;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 60;
						break;

					case Frame6:
						Sora.XCurrentCoordDraw -= 60;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 60;
						break;

					default:
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						break;
					}
				}
				else if (SoraAnimacionActual == RunningBack) {
					mirrorxblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
						Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
						ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, 0xFFFFFFFF);
				}
			}
			else if (MarluxiaAnimacionActual == Hit || MarluxiaAnimacionActual == RunningBack) {
				if (SoraAnimacionActual == GetHit) {
					Sora.YCurrentCoordDraw -= 14;
					if (SoraFrameActual == Frame5) {
						Sora.XCurrentCoordDraw -= 2;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 2;
					}
					else {
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
					}
					Sora.YCurrentCoordDraw += 14;
				}
				else {
					TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
						Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
						ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
				}

				if (MarluxiaAnimacionActual == Hit) {
					Marluxia.XCurrentCoordDraw += 45;
					Marluxia.YCurrentCoordDraw -= 172;
					switch (MarluxiaFrameActual) {

					case Frame1:
						Marluxia.XCurrentCoordDraw += 9;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 9;
						break;

					case Frame2:
						Marluxia.XCurrentCoordDraw += 9;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 9;
						break;

					case Frame3:
						Marluxia.XCurrentCoordDraw += 9;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 9;
						break;

					case Frame4:
						Marluxia.XCurrentCoordDraw -= 27;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 27;
						break;

					case Frame5:
						Marluxia.XCurrentCoordDraw -= 147;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 147;
						break;

					case Frame6:
						Marluxia.XCurrentCoordDraw -= 180;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 180;
						break;

					case Frame7:
						Marluxia.XCurrentCoordDraw -= 120;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 120;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						break;
					}
					Marluxia.XCurrentCoordDraw -= 45;
					Marluxia.YCurrentCoordDraw += 172;
				}
				else if (MarluxiaAnimacionActual == RunningBack) {
					mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
						Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
						Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
						Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
						ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
				}
			}
			else {

				//Arregla algunas animaciones
				switch (SoraAnimacionActual) {

				case Intro:
					switch (SoraFrameActual) {
					case Frame14:
						Sora.XCurrentCoordDraw -= 1;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 1;
						break;

					case Frame15:
						Sora.XCurrentCoordDraw -= 12;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 12;
						break;

					default:
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						break;
					}
					break;

				case Heal:
					Sora.YCurrentCoordDraw -= 45;
					switch (SoraFrameActual) {
					case Frame1:
						Sora.XCurrentCoordDraw -= 6;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 6;
						break;

					case Frame2:
						Sora.XCurrentCoordDraw -= 6;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 6;
						break;

					case Frame5:
						Sora.XCurrentCoordDraw -= 18;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 18;
						break;

					case Frame6:
						Sora.XCurrentCoordDraw -= 6;
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						Sora.XCurrentCoordDraw += 6;
						break;

					default:
						TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
							Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
							Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
							ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
						break;
					}
					Sora.YCurrentCoordDraw += 45;
					break;

				default:
					TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
						Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
						Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
						ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);
					break;
				}

				//Arregla animaciones
				switch (MarluxiaAnimacionActual) {
				case Intro:
					switch (MarluxiaFrameActual) {

					case Frame1:
						Marluxia.XCurrentCoordDraw -= 9;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 9;
						break;

					case Frame2:
						Marluxia.XCurrentCoordDraw -= 6;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 6;
						break;

					case Frame3:
						Marluxia.XCurrentCoordDraw -= 6;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 6;
						break;

					case Frame4:
						Marluxia.XCurrentCoordDraw -= 15;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 15;
						break;

					case Frame5:
						Marluxia.XCurrentCoordDraw -= 6;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 6;
						break;

					case Frame6:
						Marluxia.XCurrentCoordDraw -= 3;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 3;
						break;

					case Frame7:
						Marluxia.XCurrentCoordDraw -= 6;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 6;
						break;

					case Frame8:
						Marluxia.XCurrentCoordDraw -= 123;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 123;
						break;

					case Frame9:
						Marluxia.XCurrentCoordDraw -= 114;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 114;
						break;

					case Frame10:
						Marluxia.XCurrentCoordDraw -= 48;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 48;
						break;

					case Frame11:
						Marluxia.XCurrentCoordDraw -= 6;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 6;
						break;

					case Frame12:
						Marluxia.XCurrentCoordDraw -= 57;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 57;
						break;

					case Frame13:
						Marluxia.XCurrentCoordDraw -= 57;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 57;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						break;
					}
					break;

				case Idle:
					switch (MarluxiaFrameActual) {
					case Frame0:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.YCurrentCoordDraw -= 3;
						break;

					case Frame1:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.YCurrentCoordDraw -= 3;
						break;

					case Frame3:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.YCurrentCoordDraw += 3;
						break;

					case Frame4:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.YCurrentCoordDraw += 3;
						break;

					default:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						break;
					}
					break;

				case Defend:
					Marluxia.YCurrentCoordDraw -= 90;
					switch (MarluxiaFrameActual) {

					case Frame0:
						Marluxia.XCurrentCoordDraw -= 9;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 9;
						break;

					case Frame1:
						Marluxia.XCurrentCoordDraw += 3;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 3;
						break;

					case Frame2:
						Marluxia.XCurrentCoordDraw += 18;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 18;
						break;

					case Frame3:
						Marluxia.XCurrentCoordDraw += 12;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 12;
						break;

					case Frame7:
						Marluxia.XCurrentCoordDraw -= 9;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 9;
						break;

					case Frame8:
						Marluxia.XCurrentCoordDraw -= 36;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 36;
						break;

					case Frame9:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						break;

					default:
						Marluxia.XCurrentCoordDraw += 6;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 6;
						break;
					}
					Marluxia.YCurrentCoordDraw += 90;
					break;

				case Heal:
					Marluxia.YCurrentCoordDraw -= 39;
					switch (MarluxiaFrameActual) {

					case Frame1:
						Marluxia.XCurrentCoordDraw += 12;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 12;
						break;

					case Frame2:
						Marluxia.XCurrentCoordDraw -= 6;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 6;
						break;

					case Frame3:
						Marluxia.XCurrentCoordDraw += 3;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 3;
						break;

					case Frame4:
						Marluxia.XCurrentCoordDraw -= 57;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 57;
						break;

					case Frame5:
						Marluxia.XCurrentCoordDraw -= 75;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 75;
						break;

					case Frame6:
						Marluxia.XCurrentCoordDraw -= 87;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 87;
						break;

					case Frame7:
						Marluxia.XCurrentCoordDraw -= 75;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw += 75;
						break;

					case Frame8:
						Marluxia.XCurrentCoordDraw += 24;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 24;
						break;

					case Frame9:
						Marluxia.XCurrentCoordDraw += 9;
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						Marluxia.XCurrentCoordDraw -= 9;
						break;
					
					default:
						mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
							Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
							Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
							ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
						break;
					}
					Marluxia.YCurrentCoordDraw += 39;
					break;

				default:
					mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
						Marluxia.XCurrentCoordDraw, Marluxia.YCurrentCoordDraw,
						Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].x, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].y,
						Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].ancho, Marluxia.FrameSpriteArray[MarluxiaAnimacionActual][MarluxiaFrameActual].alto,
						ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 3, 3, 0xFFC8BFE7);
					break;
				}
			}

			//UI
			if (PlayerTurn) {
				//Acciones
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					BasicUI.XCurrentCoordDraw, BasicUI.YCurrentCoordDraw,
					BasicUI.FrameSpriteArray[StaticUI][Frame0].x, BasicUI.FrameSpriteArray[StaticUI][Frame0].y,
					BasicUI.FrameSpriteArray[StaticUI][Frame0].ancho, BasicUI.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, false);

				//Flecha
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					Flecha.XCurrentCoordDraw, Flecha.YCurrentCoordDraw,
					Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].x, Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].y,
					Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].ancho, Flecha.FrameSpriteArray[Arrow][FlechaFrameActual].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, true);
			}

			//Barra para jugador
			TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
				BarraPlayer.XCurrentCoordDraw, BarraPlayer.YCurrentCoordDraw,
				BarraPlayer.FrameSpriteArray[StaticUI][Frame0].x, BarraPlayer.FrameSpriteArray[StaticUI][Frame0].y,
				BarraPlayer.FrameSpriteArray[StaticUI][Frame0].ancho, BarraPlayer.FrameSpriteArray[StaticUI][Frame0].alto,
				ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);

			//Barra para enemigo
			TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
				BarraEnemy.XCurrentCoordDraw, BarraEnemy.YCurrentCoordDraw,
				BarraEnemy.FrameSpriteArray[StaticUI][Frame0].x, BarraEnemy.FrameSpriteArray[StaticUI][Frame0].y,
				BarraEnemy.FrameSpriteArray[StaticUI][Frame0].ancho, BarraEnemy.FrameSpriteArray[StaticUI][Frame0].alto,
				ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);

			if (ShowPlayerHealth) {
				//Barra verde de vida para jugador
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					VidaPlayer.XCurrentCoordDraw, VidaPlayer.YCurrentCoordDraw,
					VidaPlayer.FrameSpriteArray[StaticUI][Frame0].x, VidaPlayer.FrameSpriteArray[StaticUI][Frame0].y,
					VidaPlayer.FrameSpriteArray[StaticUI][Frame0].ancho, VidaPlayer.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);
			}
			if (ShowEnemyHealth) {
				//Barra verde de vida para enemigo
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					VidaEnemy.XCurrentCoordDraw, VidaEnemy.YCurrentCoordDraw,
					VidaEnemy.FrameSpriteArray[StaticUI][Frame0].x, VidaEnemy.FrameSpriteArray[StaticUI][Frame0].y,
					VidaEnemy.FrameSpriteArray[StaticUI][Frame0].ancho, VidaEnemy.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 2, 2, false);
			}

			if (ShowUI) {

				//Sora Stock
				TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
					PlayerStock.XCurrentCoordDraw, PlayerStock.YCurrentCoordDraw,
					PlayerStock.FrameSpriteArray[StaticUI][Frame0].x, PlayerStock.FrameSpriteArray[StaticUI][Frame0].y,
					PlayerStock.FrameSpriteArray[StaticUI][Frame0].ancho, PlayerStock.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, Sora.HojaSprite.ancho, 2, 2, true);

				//Marluxia Stock
				mirrorxblt(ptrBufferPixelsWindow, (Marluxia.HojaSprite.pixeles),
					20, 105,
					EnemyStock3.FrameSpriteArray[StaticUI][Frame0].x, EnemyStock3.FrameSpriteArray[StaticUI][Frame0].y,
					EnemyStock3.FrameSpriteArray[StaticUI][Frame0].ancho, EnemyStock3.FrameSpriteArray[StaticUI][Frame0].alto,
					ANCHO_VENTANA, Marluxia.HojaSprite.ancho, 2, 2, 0xFFC8BFE7);
			}

			if (ShowNums) {
				//Numero1
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					Numero1.XCurrentCoordDraw, Numero1.YCurrentCoordDraw,
					Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].x, Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].y,
					Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].ancho, Numero1.FrameSpriteArray[Num1AnimacionActual][NumFrameActual].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, true);

				//Numero2
				TranScaleblt(ptrBufferPixelsWindow, (UIHojaSprite.pixeles),
					Numero2.XCurrentCoordDraw, Numero2.YCurrentCoordDraw,
					Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].x, Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].y,
					Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].ancho, Numero2.FrameSpriteArray[Num2AnimacionActual][NumFrameActual].alto,
					ANCHO_VENTANA, UIHojaSprite.ancho, 3, 3, true);
			}
			break;
		}
		}
	}
	else if (WinScreen) {
		TranScaleblt(ptrBufferPixelsWindow, (miStage.ImagenEscenario5.pixeles), 0, 0, 0, 0,
			miStage.ImagenEscenario5.ancho, miStage.ImagenEscenario5.alto, ANCHO_VENTANA,
			miStage.ImagenEscenario5.ancho, 2, 2, false);
	}
	else if(InGameOver) {
		TranScaleblt(ptrBufferPixelsWindow, (Sora.HojaSprite.pixeles),
			Sora.XCurrentCoordDraw, Sora.YCurrentCoordDraw,
			Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].x, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].y,
			Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].ancho, Sora.FrameSpriteArray[SoraAnimacionActual][SoraFrameActual].alto,
			ANCHO_VENTANA, Sora.HojaSprite.ancho, 3, 3, true);

		// 0 = La unica animacion que tiene
		TranScaleblt(ptrBufferPixelsWindow, (GameOverHojaSprite.pixeles),
			Corazon.XCurrentCoordDraw, Corazon.YCurrentCoordDraw,
			Corazon.FrameSpriteArray[CorazonFrameActual].x, Corazon.FrameSpriteArray[CorazonFrameActual].y,
			Corazon.FrameSpriteArray[CorazonFrameActual].ancho, Corazon.FrameSpriteArray[CorazonFrameActual].alto,
			ANCHO_VENTANA, GameOverHojaSprite.ancho, 3, 3, false);
	}
}

void ActualizaAnimacion(HWND hWnd){

	switch (SoraAnimacionActual) {

	case Intro:
	{
		if (!PantallaInicio) {
			if (TICK % TICK == 0 && SoraFrameActual == 0)//125
			{
				SoraDelayFrameAnimation++;
			}
			if (SoraDelayFrameAnimation % 3 == 0)
			{
				SoraFrameActual++;
				if (SoraFrameActual > 16) {
					SoraFrameActual = 0;
					Sora.YCurrentCoordDraw += 20;
					PlayerTurn = true;
					ShowUI = true;
					SoraAnimacionActual = Idle;
					break;
				}
			}
		}
		break;
	}
	case Idle:
	{
		if (PlayerHadShield) {
			PlayerShield = true;
			PlayerHadShield = false;
		}
		if (TICK % TICK == 0 && SoraFrameActual == 0)//125
		{
			SoraDelayFrameAnimation++;
		}
		//else if (TICK % TICK == 0 && FrameActual == 2)
		//{
		//	DelayFrameAnimation += 3;
		//}
		if (SoraDelayFrameAnimation % 3 == 0)
		{
			SoraFrameActual++;
			if (SoraFrameActual > 5) SoraFrameActual = 0;
		}
		break;
	}
	case Running: 
	{
		if (PlayerShield) {
			PlayerHadShield = true;
			PlayerShield = false;
			EscudoJugadorFrameActual = 0;
		}
		if (TICK % TICK == 0 && SoraFrameActual == 0)//125
		{
			SoraDelayFrameAnimation++;
		}
		if (SoraDelayFrameAnimation % 1 == 0)
		{
			SoraFrameActual++;
			if (Sora.XCurrentCoordDraw > 300) {
				Sora.XCurrentCoordDraw -= 30;
			}
			if (SoraFrameActual > 7) {
				SoraFrameActual = 0;
			}
			if (Sora.XCurrentCoordDraw <= 300) {
				SoraFrameActual = 0;
				SoraAnimacionActual = Hit;
				break;
			}
		}
		break;
	}
	case RunningBack: 
	{
		if (TICK % TICK == 0 && SoraFrameActual == 0)//125
		{
			SoraDelayFrameAnimation++;
		}
		if (SoraDelayFrameAnimation % 1 == 0)
		{
			SoraFrameActual++;
			if (Sora.XCurrentCoordDraw < 600) {
				Sora.XCurrentCoordDraw += 30;
			}
			if (SoraFrameActual > 7) SoraFrameActual = 0;
			if (Sora.XCurrentCoordDraw >= 600) {
				SoraFrameActual = 0;
				Sora.XCurrentCoordDraw = 600;
				EnemyTurn = true;
				SoraAnimacionActual = Idle;
				break;
			}
		}
		break;
	}
	case GetHit: 
	{
		if (TICK % TICK == 0 && SoraFrameActual == 0)//125
		{
			SoraDelayFrameAnimation++;
		}
		if (TICK % TICK == 0 && SoraFrameActual == 3)//125
		{
			if (PlayerHealth == 0) {
				CambiarStage(GameOver);
			}
		}
		if (SoraDelayFrameAnimation % 1 == 0)
		{
			SoraFrameActual++;

			if (SoraFrameActual > 6) {
				SoraFrameActual = 0;
				SoraAnimacionActual = Idle;
				break;
			}
		}
		break;
	}
	case Hit: 
	{
		if (TICK % TICK == 0 && SoraFrameActual == 0)//125
		{
			SoraDelayFrameAnimation++;
		}
		if (TICK % TICK == 0 && SoraFrameActual == 2)//125
		{
			/*effects->OpenFile(Cancion[2].Dir.c_str(), sfAutodetect);
			effects->Play();*/

			ShowNums = true;
			NumFrameActual = 0;

			switch (CurrentStage) {
			case Stage1:
				Numero1.XCurrentCoordDraw = Riku.XCurrentCoordDraw + 21;
				Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

				Numero1.YCurrentCoordDraw = Riku.YCurrentCoordDraw - 40;
				Numero2.YCurrentCoordDraw = Riku.YCurrentCoordDraw - 40;

				RikuFrameActual = 0;
				RikuAnimacionActual = GetHit;
				break;
			case Stage2:
				Numero1.XCurrentCoordDraw = Axel.XCurrentCoordDraw + 51;
				Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

				Numero1.YCurrentCoordDraw = Axel.YCurrentCoordDraw - 10;
				Numero2.YCurrentCoordDraw = Axel.YCurrentCoordDraw - 10;

				AxelFrameActual = 0;
				Axel.YCurrentCoordDraw = 342;
				AxelAnimacionActual = GetHit;
				break;
			case Stage3:
				Numero1.XCurrentCoordDraw = Marluxia.XCurrentCoordDraw + 51;
				Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

				Numero1.YCurrentCoordDraw = Marluxia.YCurrentCoordDraw - 45;
				Numero2.YCurrentCoordDraw = Marluxia.YCurrentCoordDraw - 45;

				MarluxiaFrameActual = 0;
				MarluxiaAnimacionActual = GetHit;
				break;
			}
			SetDamage(false);
			if (EnemyShield) {
				EscudoEnemigoFrameActual = 0;
				EnemyShield = false;
			}
		}
		if (SoraDelayFrameAnimation % 1 == 0)
		{
			SoraFrameActual++;

			if (SoraFrameActual > 6) {
				SoraFrameActual = 0;
				SoraAnimacionActual = RunningBack;
				break;
			}
		}
		break;
	}
	case Defend:
	{
		if (TICK % TICK == 0 && SoraFrameActual == 0) {
			SoraDelayFrameAnimation++;
		}
		if (TICK % TICK == 0 && SoraFrameActual == 3) {
			SoraDelayFrameAnimation++;
		}
		if (SoraDelayFrameAnimation % 3 == 0)
		{
			SoraFrameActual++;

			if (SoraFrameActual == 2) {
				EscudoJugadorFrameActual = 0;
				PlayerShield = true;
			}

			if (SoraFrameActual > 4) {
				SoraFrameActual = 0;
				EnemyTurn = true;
				SoraAnimacionActual = Idle;
			}
		}
		break;
	}
	case Heal:
	{
		if (TICK % TICK == 0 && SoraFrameActual == 0) {
			SoraDelayFrameAnimation++;

			Numero1.XCurrentCoordDraw = Sora.XCurrentCoordDraw + 9;
			Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

			Numero1.YCurrentCoordDraw = Sora.YCurrentCoordDraw - 80;
			Numero2.YCurrentCoordDraw = Sora.YCurrentCoordDraw - 80;

			SetHealing(true);
		}
		if (SoraDelayFrameAnimation % 1 == 0)
		{
			SoraFrameActual++;
			if (SoraFrameActual > 6) {
				SoraFrameActual = 0;
				SoraAnimacionActual = Idle;
			}
		}
		break;
	}
	case Die: 
	{
		if (TICK % TICK == 0 && SoraFrameActual == 0) {
			SoraDelayFrameAnimation++;
		}
		if (TICK % TICK == 0 && SoraFrameActual == 1) {
			SoraDelayFrameAnimation++;
		}
		if (TICK % TICK == 0 && SoraFrameActual == 2) {
			SoraDelayFrameAnimation++;
		}
		if (TICK % TICK == 0 && SoraFrameActual == 3) {
			SoraDelayFrameAnimation++;
		}
		if (SoraDelayFrameAnimation % 4 == 0)
		{
			SoraFrameActual++;

			if (SoraFrameActual > 3) {
				SoraFrameActual = 0;
			}
		}
		break;
	}
	}

	switch (CurrentStage) {
		case Stage1:
			switch (RikuAnimacionActual) {
		case Intro:
		{
			if (TICK % TICK == 0 && RikuFrameActual == 0)//125
			{
				RikuDelayFrameAnimation++;
			}
			if (RikuDelayFrameAnimation % 3 == 0)
			{
				RikuFrameActual++;
				if (RikuFrameActual > 5) {
					RikuFrameActual = 0;
					RikuAnimacionActual = Idle;
					break;
				}
			}
			break;
		}
		case Idle:
		{
			if (EnemyHadShield) {
				EnemyShield = true;
				EnemyHadShield = false;
			}
			if (TICK % TICK == 0 && RikuFrameActual == 0)//125
			{
				RikuDelayFrameAnimation++;
			}
			if (RikuDelayFrameAnimation % 3 == 0)
			{
				RikuFrameActual++;
				if (RikuFrameActual > 5) RikuFrameActual = 0;
			}
			break;
		}
		case Running:
		{
			if (EnemyShield) {
				EnemyHadShield = true;
				EnemyShield = false;
				EscudoEnemigoFrameActual = 0;
			}
			if (TICK % TICK == 0 && RikuFrameActual == 0)//125
			{
				RikuDelayFrameAnimation++;
			}
			if (RikuDelayFrameAnimation % 1 == 0)
			{
				RikuFrameActual++;
				if (Riku.XCurrentCoordDraw < 380) {
					Riku.XCurrentCoordDraw += 27;
				}
				if (RikuFrameActual > 5) {
					RikuFrameActual = 0;
				}
				if (Riku.XCurrentCoordDraw >= 380) {
					RikuFrameActual = 0;
					RikuAnimacionActual = Hit;
					break;
				}
			}
			break;
		}
		case RunningBack:
		{
			if (TICK % TICK == 0 && RikuFrameActual == 0)//125
			{
				RikuDelayFrameAnimation++;
			}
			if (RikuDelayFrameAnimation % 1 == 0)
			{
				RikuFrameActual++;
				if (Riku.XCurrentCoordDraw > 110) {
					Riku.XCurrentCoordDraw -= 27;
				}
				if (RikuFrameActual > 5) {
					RikuFrameActual = 0;
				}
				if (Riku.XCurrentCoordDraw <= 110) {
					RikuFrameActual = 0;
					Riku.XCurrentCoordDraw = 110;
					PlayerTurn = true;
					RikuAnimacionActual = Idle;
					break;
				}
			}
			break;
		}
		case GetHit:
		{
			if (TICK % TICK == 0 && RikuFrameActual == 0)//125
			{
				RikuDelayFrameAnimation++;
			}
			if (RikuDelayFrameAnimation % 1 == 0)
			{
				RikuFrameActual++;

				if (RikuFrameActual > 4) {
					RikuFrameActual = 0;
					RikuAnimacionActual = Idle;
					break;
				}
			}
			break;
		}
		case Hit:
		{
			if (TICK % TICK == 0 && RikuFrameActual == 0)//125
			{
				RikuDelayFrameAnimation++;
			}
			if (TICK % TICK == 0 && RikuFrameActual == 2)//125
			{
				/*effects->OpenFile(Cancion[2].Dir.c_str(), sfAutodetect);
				effects->Play();*/

				Numero1.XCurrentCoordDraw = Sora.XCurrentCoordDraw + 9;
				Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

				Numero1.YCurrentCoordDraw = Sora.YCurrentCoordDraw - 60;
				Numero2.YCurrentCoordDraw = Sora.YCurrentCoordDraw - 60;

				SoraFrameActual = 0;
				SoraAnimacionActual = GetHit;

				SetDamage(true);

				if (PlayerShield) {
					EscudoJugadorFrameActual = 0;
					PlayerShield = false;
				}
			}
			if (RikuDelayFrameAnimation % 1 == 0)
			{
				RikuFrameActual++;

				if (RikuFrameActual > 6) {
					RikuFrameActual = 0;
					RikuAnimacionActual = RunningBack;
					break;
				}
			}
			break;
		}
		case Defend:
		{
			if (TICK % TICK == 0 && RikuFrameActual == 0) {
				RikuDelayFrameAnimation++;
			}
			if (RikuDelayFrameAnimation % 2 == 0)
			{
				RikuFrameActual++;

				if (RikuFrameActual == 2) {
					EscudoEnemigoFrameActual = 0;
					EnemyShield = true;
				}

				if (RikuFrameActual > 4) {
					RikuFrameActual = 0;
					PlayerTurn = true;
					RikuAnimacionActual = Idle;
				}
			}
			break;
		}
		case Heal:
		{
			if (TICK % TICK == 0 && RikuFrameActual == 0) {
				RikuDelayFrameAnimation++;

				Numero1.XCurrentCoordDraw = Riku.XCurrentCoordDraw + 21;
				Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

				Numero1.YCurrentCoordDraw = Riku.YCurrentCoordDraw - 40;
				Numero2.YCurrentCoordDraw = Riku.YCurrentCoordDraw - 40;

				SetHealing(false);
			}
			if (RikuDelayFrameAnimation % 1 == 0)
			{
				RikuFrameActual++;

				if (RikuFrameActual > 6) {
					RikuFrameActual = 0;
					RikuAnimacionActual = Idle;
				}
			}
			break;
		}
		}
			break;

		case Stage2:
			switch (AxelAnimacionActual) {
		case Intro:
		{
				if (TICK % TICK == 0 && AxelFrameActual == 0)//125
				{
					AxelDelayFrameAnimation++;
				}

				if (AxelDelayFrameAnimation % 1 == 0)
				{
					AxelFrameActual++;
					if (AxelFrameActual > 15) {
						AxelFrameActual = 0;
						AxelAnimacionActual = Idle;
						break;
					}
				}
				break;
			}
		case Idle:
		{
			if (EnemyHadShield) {
				EnemyShield = true;
				EnemyHadShield = false;
			}
			if (TICK % TICK == 0 && AxelFrameActual < 6)//125
				{
					AxelDelayFrameAnimation++;
				}
			if (AxelDelayFrameAnimation % 2 == 0)
				{
					AxelFrameActual++;
					if (AxelFrameActual > 5) {
						AxelFrameActual = 0;
					}
				}
			break;
		}
		case Running:
		{
			if (EnemyShield) {
				EnemyHadShield = true;
				EnemyShield = false;
				EscudoEnemigoFrameActual = 0;
			}
			if (TICK % TICK == 0 && AxelFrameActual == 0)//125
				{
					AxelDelayFrameAnimation++;
				}
			if (TICK % TICK == 0 && AxelFrameActual == 6)//125
				{
					AxelDelayFrameAnimation++;
				}
			if (AxelDelayFrameAnimation % 3 == 0)
				{
					AxelFrameActual++;
					if (AxelFrameActual > 12) {
						AxelFrameActual = 0;
						Axel.YCurrentCoordDraw = 300;
						AxelAnimacionActual = Hit;
					}
				}
			break;
		}
		case RunningBack:
		{
			if (TICK % TICK == 0 && AxelFrameActual == 0)//125
				{
					AxelDelayFrameAnimation++;
				}
			if (TICK % TICK == 0 && AxelFrameActual == 6)//125
				{
					AxelDelayFrameAnimation++;
				}
			if (AxelDelayFrameAnimation % 3 == 0)
				{
					AxelFrameActual++;
					if (AxelFrameActual > 12) {
						AxelFrameActual = 0;
						Axel.YCurrentCoordDraw = 300;
						AxelAnimacionActual = Idle;
						PlayerTurn = true;
					}
				}
			break;
		}
		case GetHit:
		{
			if (TICK % TICK == 0 && AxelFrameActual == 0)//125
				{
					AxelDelayFrameAnimation++;
				}
			if (AxelDelayFrameAnimation % 1 == 0)
				{
					AxelFrameActual++;
					if (AxelFrameActual > 4) {
						AxelFrameActual = 0;
						Axel.YCurrentCoordDraw = 300;
						AxelAnimacionActual = Idle;
					}
				}
			break;
		}
		case Hit:
		{
			if (TICK % TICK == 0 && AxelFrameActual == 0)//125
				{
					AxelDelayFrameAnimation++;
				}
			if (TICK % TICK == 0 && AxelFrameActual == 2)//125
			{
				/*effects->OpenFile(Cancion[2].Dir.c_str(), sfAutodetect);
				effects->Play();*/

				Numero1.XCurrentCoordDraw = Sora.XCurrentCoordDraw + 9;
				Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

				Numero1.YCurrentCoordDraw = Sora.YCurrentCoordDraw - 60;
				Numero2.YCurrentCoordDraw = Sora.YCurrentCoordDraw - 60;

				SoraFrameActual = 0;
				SoraAnimacionActual = GetHit;

				SetDamage(true);

				if (PlayerShield) {
					EscudoJugadorFrameActual = 0;
					PlayerShield = false;
				}
			}
			if (AxelDelayFrameAnimation % 1 == 0)
				{
					AxelFrameActual++;
					if (AxelFrameActual > 7) {
						AxelFrameActual = 0;
						AxelAnimacionActual = RunningBack;
					}
				}
			break;
		}
		case Defend:
		{
			if (TICK % TICK == 0 && AxelFrameActual == 0)//125
				{
					AxelDelayFrameAnimation++;
				}
			if (AxelDelayFrameAnimation % 1 == 0)
				{
					AxelFrameActual++;
					if (AxelFrameActual == 3) {
						EscudoEnemigoFrameActual = 0;
						EnemyShield = true;
					}
					if (AxelFrameActual > 7) {
						AxelFrameActual = 0;
						Axel.YCurrentCoordDraw = 300;
						AxelAnimacionActual = Idle;
						PlayerTurn = true;
					}
				}
			break;
		}
		case Heal:
		{
			if (TICK % TICK == 0 && AxelFrameActual == 0)//125
			{
				AxelDelayFrameAnimation++;

				Numero1.XCurrentCoordDraw = Axel.XCurrentCoordDraw + 48;
				Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

				Numero1.YCurrentCoordDraw = Axel.YCurrentCoordDraw - 20;
				Numero2.YCurrentCoordDraw = Axel.YCurrentCoordDraw - 20;

				SetHealing(false);
			}
			if (AxelDelayFrameAnimation % 1 == 0)
			{
				AxelFrameActual++;
				if (AxelFrameActual > 10) {
					AxelFrameActual = 0;
					Axel.YCurrentCoordDraw = 300;
					AxelAnimacionActual = Idle;
				}
			}
		}
		}
			break;

		case Stage3:
			switch (MarluxiaAnimacionActual) {
		case Intro: 
		{
			if (TICK % TICK == 0 && MarluxiaFrameActual == 0)//125
			{
				MarluxiaDelayFrameAnimation++;
			}
			if (MarluxiaDelayFrameAnimation % 3 == 0)
			{
				MarluxiaFrameActual++;
				if (MarluxiaFrameActual > 13) {
					MarluxiaFrameActual = 0;
					MarluxiaAnimacionActual = Idle;
					Marluxia.YCurrentCoordDraw += 138;
					Marluxia.XCurrentCoordDraw -= 72;
				}
			}
			break;
		}
		case Idle:
		{
			if (EnemyHadShield) {
				EnemyShield = true;
				EnemyHadShield = false;
			}
			if (TICK % TICK == 0 && MarluxiaFrameActual < 5 && MarluxiaFrameActual != 2)//125
			{
				MarluxiaDelayFrameAnimation++;
			}
			if (MarluxiaDelayFrameAnimation % 3 == 0)
			{
				MarluxiaFrameActual++;
				if (MarluxiaFrameActual > 4) MarluxiaFrameActual = 0;
			}
			break;
		}
		case Running:
		{
			if (EnemyShield) {
					EnemyHadShield = true;
					EnemyShield = false;
					EscudoEnemigoFrameActual = 0;
				}
			if (TICK % TICK == 0 && MarluxiaFrameActual < 4)//125
				{
					MarluxiaDelayFrameAnimation++;
				}
			if (Marluxia.XCurrentCoordDraw < 380) {
					Marluxia.XCurrentCoordDraw += 15;
				}
			if (MarluxiaDelayFrameAnimation % 2 == 0)
			{
				MarluxiaFrameActual++;
				if (MarluxiaFrameActual > 3) {
					MarluxiaFrameActual = 0;
				}
				if (Marluxia.XCurrentCoordDraw >= 380) {
					MarluxiaFrameActual = 0;
					MarluxiaAnimacionActual = Hit;
					break;
				}
			}
			break;
		}
		case RunningBack:
		{
			if (TICK % TICK == 0 && MarluxiaFrameActual < 4)//125
				{
					MarluxiaDelayFrameAnimation++;
				}
			if (Marluxia.XCurrentCoordDraw > 87) {
					Marluxia.XCurrentCoordDraw -= 15;
				}
			if (MarluxiaDelayFrameAnimation % 2 == 0)
				{
					MarluxiaFrameActual++;
					if (MarluxiaFrameActual > 3) {
						MarluxiaFrameActual = 0;
					}
					if (Marluxia.XCurrentCoordDraw <= 87) {
						MarluxiaFrameActual = 0;
						Marluxia.XCurrentCoordDraw = 87;
						MarluxiaAnimacionActual = Idle;
						PlayerTurn = true;
						break;
					}
				}
			break;
		}
		case GetHit: 
		{
			if (TICK % TICK == 0 && MarluxiaFrameActual == 0)//125
				{
					MarluxiaDelayFrameAnimation++;
				}
			if (MarluxiaDelayFrameAnimation % 1 == 0)
				{
					MarluxiaFrameActual++;
					if (MarluxiaFrameActual > 4) { 
						MarluxiaFrameActual = 0;
						MarluxiaAnimacionActual = Idle;
						Marluxia.YCurrentCoordDraw = 361;
					}
				}
			break;
		}
		case Hit:
		{
			if (TICK % TICK == 0 && MarluxiaFrameActual == 0)//125
				{
					MarluxiaDelayFrameAnimation++;
				}
			if (TICK % TICK == 0 && MarluxiaFrameActual == 3)//125
				{
						/*effects->OpenFile(Cancion[2].Dir.c_str(), sfAutodetect);
						effects->Play();*/
					Numero1.XCurrentCoordDraw = Sora.XCurrentCoordDraw + 9;
					Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

					Numero1.YCurrentCoordDraw = Sora.YCurrentCoordDraw - 60;
					Numero2.YCurrentCoordDraw = Sora.YCurrentCoordDraw - 60;

					SoraFrameActual = 0;
					SoraAnimacionActual = GetHit;

					SetDamage(true);

					if (PlayerShield) {
					EscudoJugadorFrameActual = 0;
					PlayerShield = false;
				}
				}
			if (MarluxiaDelayFrameAnimation % 1 == 0)
				{
					MarluxiaFrameActual++;
					if (MarluxiaFrameActual > 7) { 
						MarluxiaFrameActual = 0;
						MarluxiaAnimacionActual = RunningBack;
					}
				}
			break;
		}
		case Defend: 
		{
			if (TICK % TICK == 0 && MarluxiaFrameActual == 0)//125
				{
					MarluxiaDelayFrameAnimation++;
				}
			if (MarluxiaDelayFrameAnimation % 1 == 0)
				{
					MarluxiaFrameActual++;
					if (MarluxiaFrameActual == 4) {
						EscudoEnemigoFrameActual = 0;
						EnemyShield = true;
					}
					if (MarluxiaFrameActual > 9) {
						MarluxiaFrameActual = 0;
						MarluxiaAnimacionActual = Idle;
						Marluxia.YCurrentCoordDraw = 361;
						PlayerTurn = true;
					}
				}
			break;
		}
		case Heal: 
		{
			if (TICK % TICK == 0 && MarluxiaFrameActual == 0)//125
			{
				MarluxiaDelayFrameAnimation++;

				Numero1.XCurrentCoordDraw = Marluxia.XCurrentCoordDraw + 48;
				Numero2.XCurrentCoordDraw = Numero1.XCurrentCoordDraw + 39;

				Numero1.YCurrentCoordDraw = Marluxia.YCurrentCoordDraw - 60;
				Numero2.YCurrentCoordDraw = Marluxia.YCurrentCoordDraw - 60;

				SetHealing(false);
			}
			if (MarluxiaDelayFrameAnimation % 1 == 0)
			{
				MarluxiaFrameActual++;
				if (MarluxiaFrameActual > 9) {
					MarluxiaFrameActual = 0;
					MarluxiaAnimacionActual = Idle;
					Marluxia.YCurrentCoordDraw = 361;
				}
			}
			break;
		}
		}
			break;
	}

	if (ShowIntroMarluxia && MarluxiaFrameActual < 9 && MarluxiaFrameActual != 0) {
		GuadañaFrameActual++;
		if (GuadañaFrameActual > 6) {
			GuadañaFrameActual = 0;
			ShowIntroMarluxia = false;
		}
	}

	if (ShowNums) {
		if (SoraAnimacionActual == Heal){
			GiveTurnEnemyAtAnimEnd = true;
		}
		else if (RikuAnimacionActual == Heal || AxelAnimacionActual == Heal || MarluxiaAnimacionActual == Heal) {
			GiveTurnPlayAtAnimEnd = true;
		}
		if (TICK % TICK == 0 && NumFrameActual == 4)//125
		{
			NumDelayFrameAnimation++;
		}
		if (NumDelayFrameAnimation % 6 == 0) {
			NumFrameActual++;

			if (NumFrameActual > 8) {
				if (GiveTurnEnemyAtAnimEnd) {
					EnemyTurn = true;
				}
				else if (GiveTurnPlayAtAnimEnd) {
					PlayerTurn = true;
				}
				NumFrameActual = 0;
				ShowNums = false;
				GiveTurnEnemyAtAnimEnd = false;
				GiveTurnPlayAtAnimEnd = false;
			}
		}
	}

	if (PlayerShield) {
		if (EscudoJugadorFrameActual != 3) {
			EscudoJugadorFrameActual++;
		}
	}

	if (EnemyShield) {
		if (EscudoEnemigoFrameActual != 3) {
			EscudoEnemigoFrameActual++;
		}
	}

	if (PlayerTurn) {
		FlechaFrameActual++;
		if (FlechaFrameActual == 1) {
			Flecha.XCurrentCoordDraw -= 4;
		}
		if (FlechaFrameActual == 2) {
			Flecha.XCurrentCoordDraw -= 4;
		}
		if (FlechaFrameActual == 3) {
			Flecha.XCurrentCoordDraw += 4;
		}
		if (FlechaFrameActual > 3) {
			FlechaFrameActual = 0;
			Flecha.XCurrentCoordDraw += 4;
		}
	}
	else if (InGameOver) {
		CorazonFrameActual++;
		if (CorazonFrameActual > 7) {
			CorazonFrameActual = 0;
		}
	}

	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}

void MainRender(HWND hWnd) 
{
	LimpiarFondo(ptrBufferPixelsWindow, 0xFF000000, (ANCHO_VENTANA * ALTO_VENTANA));
	Vibracion();
	KeysEvents();
	DibujaPixeles();
	ActualizaAnimacion(hWnd);
}

void KeysEvents()
{	
	if (ControlXBOX->IsConnected() && PantallaInicio) {
		bool ButtonStart = ((ControlXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
		if (ButtonStart) {
			PantallaInicio = false;
			InGame = true;
			CambiarStage(Stage1);
		}
	}
	if (PlayerTurn) {
		if (ControlXBOX->IsConnected()) {
			bool DPAD_UP = ((ControlXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
			bool DPAD_DOWN = ((ControlXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
			bool ButtonA = ((ControlXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);

			if (ButtonA) {
				switch (Accion) {
				case Ataque:
					SoraFrameActual = 0;
					PlayerTurn = false;
					SoraAnimacionActual = Running;
					break;

				case Defensa:
					SoraFrameActual = 0;
					PlayerTurn = false;
					SoraAnimacionActual = Defend;
					break;

				case Cura:
					SoraFrameActual = 0;
					PlayerTurn = false;
					SoraAnimacionActual = Heal;
					break;
				}
			}

			if (DPAD_UP) {
				if (Flecha.YCurrentCoordDraw == 546) {
					Flecha.YCurrentCoordDraw += 122;
					Accion = Cura;
				}
				else {
					Accion -= 1;
					Flecha.YCurrentCoordDraw -= 61;
				}
			}

			if (DPAD_DOWN) {
				if (Flecha.YCurrentCoordDraw == 668) {
					Flecha.YCurrentCoordDraw -= 122;
					Accion = Ataque;
				}
				else {
					Accion += 1;
					Flecha.YCurrentCoordDraw += 61;
				}
			}
		}
		if (KEYS[input.W] || KEYS[input.Up])
		{
			if (Flecha.YCurrentCoordDraw == 546) {
				Flecha.YCurrentCoordDraw += 122;
				Accion = Cura;
			}
			else {
				Accion -= 1;
				Flecha.YCurrentCoordDraw -= 61;
			}
		}
		if (KEYS[input.S] || KEYS[input.Down])
		{
			if (Flecha.YCurrentCoordDraw == 668) {
				Flecha.YCurrentCoordDraw -= 122;
				Accion = Ataque;
			}
			else {
				Accion += 1;
				Flecha.YCurrentCoordDraw += 61;
			}
		}
	}
}

void Vibracion() {
	if (ControlXBOX->IsConnected() && SoraAnimacionActual == GetHit && SoraFrameActual < 3) {
		ControlXBOX->Vibrate(26212, 26212);
	}
	else if (ControlXBOX->IsConnected() && SoraAnimacionActual == Hit && SoraFrameActual > 2 && SoraFrameActual < 4) {
		ControlXBOX->Vibrate(10000, 8000);
	}
	else if (ControlXBOX->IsConnected()) {
		ControlXBOX->Vibrate(0, 0);
	}
}

#pragma region LENS_CODE
/* Pinta el fondo de la ventana de acuerdo al color especificado.
	@param *ptrBuffer.	Puntero al area de memoria reservada para el proceso de dibujado.
	@param color.		Color expresado en formato hexadecimal.
	@param area.		Area de la ventana.
	*/
void LimpiarFondo(int *ptrBuffer, unsigned int colorFondo, int area) 
{
	__asm {
			mov edi, ptrBuffer
			mov ecx, area
			mov eax, colorFondo

			draw: 	
				mov [edi], eax
				add edi, BPP
			loop draw

			rep stosd
	}
}

/* Funcion que pinta una figura rectangular en pantalla.
	@param *ptrBuffer.	Puntero al area de memoria reservada para el dibujado.
	@param color.		Color de la figura expresado en formato hexadecimal.
	@param anchoWnd.	Ancho total de la ventana.
	@param altoWnd.		Alto total de la ventana.
	@param dmnFigura.	Especifica las dimensiones de la figura en relacion con la ventana.
	@param posFigura.	Posiciona la figura en la ventana.
	*/

void TranScaleblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int escalaX, int escalaY, bool borrafondo){
	//blt = block transfer, transferencia de bloque de imagen
	int bytesporlineafondo = anchodefondo * 4;
	int bytesporlineaorigen = anchodeorigen * 4;
	int bytesporlineaimagen = ancho * 4;

	__asm{

			mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
			//Conseguimos el pixel inicial donde empezaremos a dibujar
			mov eax, inicioYDestino
			mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
			mov ebx, eax //ebx contendra el resultado anterior
			mov eax, 4
			mul inicioXDestino //inicioX*4, para asi encontrar la columna donde queremos empezar a dibujar
			add eax, ebx //posicion de columna + posicion de linea
			add edi, eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada
			mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
			//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
			mov eax, inicioYOrigen
			mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
			mov ebx, eax //ebx contendra el resultado anterior
			mov eax, 4
			mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
			add eax, ebx //posicion de columna + posicion de linea
			add esi, eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada
			mov eax, [esi]
			mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos
			XOR ebx, ebx
			mov bl, borrafondo //guarda si se borrara el fondo
			lazollenarY :
				push ecx
				mov ecx, escalaY
				escaladoY :
					push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
					mov ecx, ancho //la cantidad de columnas que dibujaremos
					lazollenarX :
						//mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4
						push ecx
						mov ecx, escalaX
							escaladoX :
							TEST bl, bl	//Si bl no es true, no se borra el color del primer pixel
							je skipND
							cmp eax, [esi]
							je nodibujar
							skipND:
							mov edx, [esi]
							mov[edi], edx
								nodibujar :
								add edi, 4
						loop escaladoX
						add esi, 4
						pop ecx
					loop lazollenarX
					add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
					push eax
					mov eax, bytesporlineaimagen
					mul escalaX
					sub edi, eax //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado
					pop eax
					sub esi, bytesporlineaimagen
					pop ecx
				loop escaladoY
			//Lo mismo para esi
			add esi, bytesporlineaorigen
			pop ecx //recuperamos el valor del contador del ciclo exterior
			loop lazollenarY
	}
}	

void mirrorxblt(int* punteroDestino,//Buffer donde comenzara a pasar los datos
	int* punteroOrigen, // Buffer que copiara los datos del bmp
	int inicioXDestino,//ventana
	int inicioYDestino,
	int inicioXOrigen,//bmp
	int inicioYOrigen,
	int ancho,//ancho del sprite
	int alto,//alto del sprite
	int anchodefondo,//dimensiones
	int anchodeorigen,
	int escalaX,//tamaño de escala X bmp
	int escalaY,//tamaño de escala Y bmp
	const unsigned int TRANSPARENCY) {//Color Transparencia
	//blt = block transfer, transferencia de bloque de imagen
	int bytesporlineafondo = anchodefondo * 4;//fondo-destino total
	int bytesporlineaorigen = anchodeorigen * 4;//bmp total
	int bytesporlineaimagen = ancho * 4;

	__asm {

		mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
		//Conseguimos el pixel inicial donde empezaremos a dibujar
		mov eax, inicioYDestino
		mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
		mov ebx, eax //ebx contendra el resultado anterior
		mov eax, 4
		mul inicioXDestino //inicioX*4, para asi encontrar la columnda donde queremos empezar a dibujar
		add eax, ebx //posicion de columna + posicion de linea
		add edi, eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada

		mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
		//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
		mov eax, inicioYOrigen
		mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
		mov ebx, eax //ebx contendra el resultado anterior
		mov eax, 4
		mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
		add eax, ebx //posicion de columna + posicion de linea
		add esi, eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada


		mov eax, TRANSPARENCY
		mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos
		lazollenarY :
		push ecx
			mov ecx, escalaY//es como el ancho
			sub esi, 4
			escaladoY :
			push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
			mov ecx, ancho //la cantidad de columnas que dibujaremos
			add esi, bytesporlineaimagen

			lazollenarX :
		//movsd //mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4
		push ecx
			mov ecx, escalaX
			escaladoX :
		cmp eax, [esi]
			je nodibujar
			mov edx, [esi]
			mov[edi], edx
			nodibujar :
		add edi, 4
			loop escaladoX
			sub esi, 4
			pop ecx
			loop lazollenarX
			add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
			push eax
			mov eax, escalaX
			mul bytesporlineaimagen
			mov ebx, eax
			pop eax
			sub edi, ebx //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado
			//sub edi, bytesporlineaimagen //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado

			pop ecx
			loop escaladoY
			//Lo mismo para esi
			add esi, bytesporlineaorigen
			add esi, 4
			//sub esi, bytesporlineaimagen
			pop ecx //recuperamos el valor del contador del ciclo exterior
			loop lazollenarY
	}

}


//Funciones que utilizan ensamblador para calcular los puntajes

void WriteHighscores() {
	wfstream HighScores;
	HighScores.open("HighScores.txt", ios::in | ios::out);
	wstring Line;
	int FileScores[3] = { 0,0,0 };
	getline(HighScores, Line);
	for (int i = 0; i < 3; i++) {
		getline(HighScores, Line);
		FileScores[i] = _wtoi(Line.substr(Line.find_last_of(L"-") + 1).c_str());
	}

	__asm {
		lea edi, [FileScores] //Para modificar
		lea esi, [FileScores] //Para usar como constante, No fue usado xd
		mov eax, TotalDamage
		mov ebx, TotalHealth

		mul ebx

		//eax tiene el puntaje total

		mov ebx, edi //Para usar como auxiliar
		cmp eax, [edi] //Puntaje == FileScores[0]
		ja ReplaceFirstOne

		add edi, 4
		cmp eax, [edi] //Puntaje == FileScores[1]
		ja ReplaceSecondOne

		add edi, 4
		cmp eax, [edi] //Puntaje == FileScores[2]
		ja ReplaceThirdOne

		ReplaceFirstOne : //edi empieza en [0]
		add edi, 8 //edi, posición 3 [2]
			mov ebx, edi //ebx = edi [2]
			sub ebx, 4 //ebx, posición 2 [1]
			mov ecx, [ebx]
			mov[edi], ecx //Se mueve la posición 2 a la 3, [2] = [1]
			sub edi, 4 //edi, posición 2 [1]
			sub ebx, 4 //ebx, posición 1 [0]
			mov ecx, [ebx]
			mov[edi], ecx //Se mueve la posición 1 a la 2, [1] = [0]
			sub edi, 4 //edi, posición 1 [0]
			mov[edi], eax //Se sobreescribe la posición 1 original por la nueva, [0] = Puntaje
			jmp FinishScores

			ReplaceSecondOne : //edi empieza en [1]
		add edi, 4 //edi, posición 3 [2]
			mov ebx, edi//ebx = edi [2]
			sub ebx, 4 //ebx, posición 2 [1]
			mov ecx, [ebx]
			mov[edi], ecx//Se mueve la posición 2 a la 3, [2] = [1]
			sub edi, 4 //edi, posición 2 [1]
			mov[edi], eax //Se sobreescribe la posición 2 original por la nueva, [1] = Puntaje
			jmp FinishScores

			ReplaceThirdOne : //edi empieza en [2]
		mov[edi], eax //Se sobreescribe la posición 3 original por la nueva, [2] = Puntaje

			FinishScores :
	}

	HighScores.seekg(0);
	HighScores << L"HIGHSCORES" << endl;
	HighScores << L"1.-" << FileScores[0] << endl;
	HighScores << L"2.-" << FileScores[1] << endl;
	HighScores << L"3.-" << FileScores[2] << endl;
	HighScores.close();
}

void CambiarStage(int Stage) {
	ReproductorCambiarCancionYReproduce(Stage);
	SoraFrameActual = 0;
	SoraAnimacionActual = Intro;
	Sora.XCurrentCoordDraw = 600; //Posición default de Sora
	PlayerShield = false;
	EnemyShield = false;
	ShowPlayerHealth = true;
	ShowEnemyHealth = true;
	PlayerTurn = false;
	ShowUI = false;
	ShowNums = false;
	NumFrameActual = 0;

	//Se suma la vida restante despues de cambiar de stage
	//Por ejemplo, si sobreviviste con 50 puntos en Stage1, 80 puntos en Stage2, 20 puntos en Stage3
	//Terminara siendo 150 el total, esto se calcula para calcular el puntaje final
	__asm {
		mov eax, Stage
		cmp eax, 1
		ja CheckGameOver
		jmp SkipToEnd

		CheckGameOver :
		cmp eax, 5
			je SkipToEnd

			mov eax, TotalHealth
			mov ebx, PlayerHealth
			add eax, ebx
			mov TotalHealth, eax

			SkipToEnd :
	}

	//c++
	//if (Stage == Stage2 || Stage == Stage3 || Stage == Winner) {
	//	TotalHealth += PlayerHealth;
	//}

	PlayerHealth = SoraHealth;
	VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho = WidthHealthbar;
	VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = WidthHealthbar;

	switch (Stage) {
	case Stage1:
		RikuFrameActual = 0;
		RikuAnimacionActual = Intro;
		EnemyHealth = RikuHealth;
		break;

	case Stage2:
		Sora.YCurrentCoordDraw = 390;
		AxelFrameActual = 0;
		AxelAnimacionActual = Intro;
		EnemyHealth = AxelHealth;
		break;

	case Stage3:
		Sora.YCurrentCoordDraw = 430;
		MarluxiaFrameActual = 0;
		MarluxiaAnimacionActual = Intro;
		EnemyHealth = MarluxiaHealth;
		ShowIntroMarluxia = true;
		GuadañaFrameActual = 0;
		break;

	case Winner:
		InGame = false;
		WinScreen = true;
		break;

	case GameOver:
		InGame = false;
		InGameOver = true;
		SoraFrameActual = 0;
		Sora.XCurrentCoordDraw = 335;
		Sora.YCurrentCoordDraw = 400;
		SoraAnimacionActual = Die;
		player->Pause();
		pausa = true;
		break;
	}
	CurrentStage = Stage;
}

//Funciones completamente al azar para controlar a los enemigos (1 y 2)

//Atacara o defendera y muy raramente se curara
void RikuAI() {
	unsigned int RandomNumber;
	srand((unsigned)time(NULL));
	RandomNumber = rand() % 200;

	unsigned short int Action = 0;
	// Si el numero random es impar, atacara
	// Si el numero random es par, defendera
	// Si el numero random es menor o igual a 10, se curara
	__asm {
		mov edx, 0
		mov eax, RandomNumber
		cmp eax, 10
		jbe beloworequal10

		mov ebx, 2
		div ebx

		cmp edx, 0
		je iseven

		; Si es impar
		mov Action, 1
		jmp isdone

		; Si es par
		iseven:
		mov Action, 2
		jmp isdone

		; Si es menor o igual a 10
		beloworequal10:
		mov Action, 3
		jmp isdone

		isdone:
	}
	switch (Action) {
	case 1:
		RikuFrameActual = 0;
		RikuAnimacionActual = Running;
		break;

	case 2:
		RikuFrameActual = 0;
		RikuAnimacionActual = Defend;
		break;

	case 3:
		RikuFrameActual = 0;
		RikuAnimacionActual = Heal;
		break;
	}
	EnemyTurn = false;
}

//60% Ataque 20% Defensa 20% Cura
void AxelAI() {
	unsigned int RandomNumber;
	srand((unsigned)time(NULL));
	RandomNumber = rand() % 300;

	unsigned short int Action = 0;
	//60% de probabilidad de ataque
	//20% de probabilidad de defensa
	//20% de probabilidad de cura
	__asm {
		mov eax, RandomNumber
		cmp eax, 180d
		jbe below180
		
		cmp eax, 240d
		jbe below240

		cmp eax, 240d
		ja above240

		below180:
		mov Action, 1
		jmp isdone

		below240 :
		mov Action, 2
		jmp isdone

		above240 :
		mov Action, 3
		jmp isdone

		isdone :
	}
	switch (Action) {
	case 1:
		Axel.YCurrentCoordDraw = 300;
		AxelFrameActual = 0;
		AxelAnimacionActual = Running;
		break;

	case 2:
		Axel.YCurrentCoordDraw = 300;
		AxelFrameActual = 0;
		AxelAnimacionActual = Defend;
		break;

	case 3:
		Axel.YCurrentCoordDraw = 300;
		AxelFrameActual = 0;
		AxelAnimacionActual = Heal;
		break;
	}
	EnemyTurn = false;
}

//Función parcialmente al azar para controlar al enemigo final

//Si tiene poca vida, se enfocara en curar o defender
//Si ya tiene escudo, no se lo volvera a poner
//Se enfocara primeramente en atacar
void MarluxiaAI(bool EnemShield, int health) {
	unsigned int RandomNumber;
	srand((unsigned)time(NULL));
	RandomNumber = rand() % 100;

	unsigned short int Action = 0;
	__asm {
		mov eax, RandomNumber
		mov edx, health
		XOR ebx, ebx
		mov bl, EnemShield

		cmp edx, 100d //Primero checa si tiene poca vida
		jbe LowHealth

		cmp bl, 1 //Checa si tiene escudo
		je Shield

		//Mucha vida, sin escudo
		cmp eax, 70d //Si no tiene poca vida ni escudo tendra 70% de probabilidad de atacar
		jbe Atacar

		cmp eax, 90d //Si no tiene poca vida ni escudo tendra 20% de probabilidad de curar
		jbe Curar

		cmp eax, 90d //Si no tiene poca vida ni escudo tendra 10% de probabilidad de defender
		ja Defender

		//Mucha vida, con escudo

		Shield:
		cmp eax, 80d//Si no tiene poca vida y tiene escudo, tendra 80% de probabilidad de ataque
		jbe Atacar

		cmp eax, 100d//Si no tiene poca vida y tiene escudo, tendra 20% de probabilidad de curar
		jbe Curar

		//Poca vida

		LowHealth: //Si tiene poca vida, se enfocara en defensa y cura, con poca probabilidad de ataque
		cmp bl, 1
		je LowHealthWithShield //Si tiene escudo, se enfocara mayormente en curarse y raramente atacar

		//Poca vida, sin escudo

		cmp eax, 50d  //Si tiene poca vida y no tiene escudo, 50% de defender
		jbe Defender

		cmp eax, 95d  //Si tiene poca vida y no tiene escudo, 45% de curar
		jbe Curar

		cmp eax, 95d  //Si tiene poca vida y no tiene escudo, 5% de atacar
		ja Atacar

		//Poca vida, con escudo

		LowHealthWithShield:

		cmp eax, 70d //70% de probabilidad de cura, si el numero es menor o igual a 70
		jbe Curar

		cmp eax, 70d //30% de probabilidad de ataque, si el numero es mayor a 70
		ja Atacar

		//Se asigna una acción

		Atacar:
		mov Action, 1
		jmp isdone

		Defender:
		mov Action, 2
		jmp isdone

		Curar:
		mov Action, 3

		isdone:
	}
	switch (Action) {
	case 1:
		MarluxiaFrameActual = 0;
		MarluxiaAnimacionActual = Running;
		Marluxia.YCurrentCoordDraw = 361;
		break;

	case 2:
		MarluxiaFrameActual = 0;
		MarluxiaAnimacionActual = Defend;
		Marluxia.YCurrentCoordDraw = 361;
		break;

	case 3:
		MarluxiaFrameActual = 0;
		MarluxiaAnimacionActual = Heal;
		Marluxia.YCurrentCoordDraw = 361;
		break;
	}
	EnemyTurn = false;
}

void HealSoraMASM(int Heal) {
	int* HealthSpritePointer = &VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho;
	__asm {
		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0

		mov ecx, Heal //Puntos a curar a ecx
		cmp ecx, 0	//Si el numero random es cero, se cambia a uno
		jne HealCalculations1

		mov ecx, 1

		HealCalculations1:
		//Multiplicación: eax guarda el primer valor de una multiplicación y es remplazado por su resultado al final
		//El registro o variable multiplicado no es afectado, en este caso ebx
		mov eax, ecx
			mov ebx, 100
			mul ebx

			//División: eax guarda el numero a dividir y es remplazado por su resultado al final, edx guarda el residuo
			//El registro o variable dividido no es afectado, en este caso ebx
			mov ebx, SoraHealth
			div ebx

			//eax tiene el resultado del porcentaje de cuanta vida se le esta curando al personaje [(PuntosDeCura * 100)/Salud]
			//edx tiene el residuo restante

			cmp edx, 5 // Si el residuo es 5 o más, se redondeara el porcentaje, para obtener resultados más aproximados
			jb NoExtraPercent1

			inc eax // Agrega un 1% al porcentaje

			NoExtraPercent1 :

		mov ebx, WidthHealthbar
			mul ebx //Porcentaje de vida aumentada * Pixeles totales del Sprite de vida

			mov ebx, 100
			div ebx // (Porcentaje * Pixeles) / 100

			//eax tiene el resultado del numero de pixeles al que se le aumentara al ancho del sprite

			mov edi, HealthSpritePointer
			add[edi], eax // Ahora obtenido el porcentaje de los puntos de vida adquiridos, es el mismo porcentaje que se sumara al sprite de vida (Representado en pixeles)

			cmp[edi], 180 // Si el sprite sobrepasa el maximo de pixeles del sprite (180), se baja a 180
			ja HealthOverflow1
			jmp AssignValues1

			HealthOverflow1 :
		mov[edi], 180

			AssignValues1 :

			mov edx, PlayerHealth
			add edx, ecx //Agrega vida a la vida actual del jugador
			mov ebx, SoraHealth

			cmp edx, ebx //Si la vida es mayor a la maxima posible, se regresa al maximo posible
			jbe SetHealthPlayer

			mov edx, ebx

			SetHealthPlayer :
		mov PlayerHealth, edx
	}
}

void HealRikuMASM(int Heal) {
	int* HealthSpritePointer = &VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho;

	__asm {
		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0

		mov ecx, Heal //Puntos a curar a ecx
		cmp ecx, 0	//Si el numero random es cero, se cambia a uno
		jne HealCalculations2

		mov ecx, 1

		HealCalculations2:
		//Multiplicación: eax guarda el primer valor de una multiplicación y es remplazado por su resultado al final
		//El registro o variable multiplicado no es afectado, en este caso ebx
		mov eax, ecx
			mov ebx, 100
			mul ebx

			//División: eax guarda el numero a dividir y es remplazado por su resultado al final, edx guarda el residuo
			//El registro o variable dividido no es afectado, en este caso ebx
			mov ebx, RikuHealth
			div ebx

			//eax tiene el resultado del porcentaje de cuanta vida se le esta curando al personaje [(PuntosDeCura * 100)/Salud]
			//edx tiene el residuo restante

			cmp edx, 5 // Si el residuo es 5 o más, se redondeara el porcentaje, para obtener resultados más aproximados
			jb NoExtraPercent2

			inc eax // Agrega un 1% al porcentaje

			NoExtraPercent2 :

		mov ebx, WidthHealthbar
			mul ebx //Porcentaje de vida aumentada * Pixeles totales del Sprite de vida

			mov ebx, 100
			div ebx // (Porcentaje * Pixeles) / 100

			//eax tiene el resultado del numero de pixeles al que se le aumentara al ancho del sprite

			mov edi, HealthSpritePointer
			add[edi], eax // Ahora obtenido el porcentaje de los puntos de vida adquiridos, es el mismo porcentaje que se sumara al sprite de vida (Representado en pixeles)

			cmp[edi], 180 // Si el sprite sobrepasa el maximo de pixeles del sprite (180), se baja a 180
			ja HealthOverflow2
			jmp AssignValues2

			HealthOverflow2 :
		mov[edi], 180

			AssignValues2 :

			mov edx, EnemyHealth
			add edx, ecx //Agrega vida a la vida actual de Riku
			mov ebx, RikuHealth

			cmp edx, ebx //Si la vida es mayor a la maxima posible, se regresa al maximo posible
			jbe SetHealthRiku

			mov edx, ebx

			SetHealthRiku :
		mov EnemyHealth, edx
	}
}

void HealAxelMASM(int Heal) {
	int* HealthSpritePointer = &VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho;

	__asm {
		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0

		mov ecx, Heal //Puntos a curar a ecx
		cmp ecx, 0	//Si el numero random es cero, se cambia a uno
		jne HealCalculations3

		mov ecx, 1

		HealCalculations3:
		//Multiplicación: eax guarda el primer valor de una multiplicación y es remplazado por su resultado al final
		//El registro o variable multiplicado no es afectado, en este caso ebx
		mov eax, ecx
			mov ebx, 100
			mul ebx

			//División: eax guarda el numero a dividir y es remplazado por su resultado al final, edx guarda el residuo
			//El registro o variable dividido no es afectado, en este caso ebx
			mov ebx, AxelHealth
			div ebx

			//eax tiene el resultado del porcentaje de cuanta vida se le esta curando al personaje [(PuntosDeCura * 100)/Salud]
			//edx tiene el residuo restante

			cmp edx, 5 // Si el residuo es 5 o más, se redondeara el porcentaje, para obtener resultados más aproximados
			jb NoExtraPercent3

			inc eax // Agrega un 1% al porcentaje

			NoExtraPercent3 :

		mov ebx, WidthHealthbar
			mul ebx //Porcentaje de vida aumentada * Pixeles totales del Sprite de vida

			mov ebx, 100
			div ebx // (Porcentaje * Pixeles) / 100

			//eax tiene el resultado del numero de pixeles al que se le aumentara al ancho del sprite

			mov edi, HealthSpritePointer
			add[edi], eax // Ahora obtenido el porcentaje de los puntos de vida adquiridos, es el mismo porcentaje que se sumara al sprite de vida (Representado en pixeles)

			cmp[edi], 180 // Si el sprite sobrepasa el maximo de pixeles del sprite (180), se baja a 180
			ja HealthOverflow3
			jmp AssignValues3

			HealthOverflow3 :
		mov[edi], 180

			AssignValues3 :

			mov edx, EnemyHealth
			add edx, ecx //Agrega vida a la vida actual de Axel
			mov ebx, AxelHealth

			cmp edx, ebx //Si la vida es mayor a la maxima posible, se regresa al maximo posible
			jbe SetHealthAxel

			mov edx, ebx

			SetHealthAxel :
		mov EnemyHealth, edx
	}
}

void HealMarluxiaMASM(int Heal) {
	int* HealthSpritePointer = &VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho;

	__asm {
		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0

		mov ecx, Heal //Puntos a curar a ecx
		cmp ecx, 0	//Si el numero random es cero, se cambia a uno
		jne HealCalculations4

		mov ecx, 1

		HealCalculations4:
		//Multiplicación: eax guarda el primer valor de una multiplicación y es remplazado por su resultado al final
		//El registro o variable multiplicado no es afectado, en este caso ebx
		mov eax, ecx
			mov ebx, 100
			mul ebx

			//División: eax guarda el numero a dividir y es remplazado por su resultado al final, edx guarda el residuo
			//El registro o variable dividido no es afectado, en este caso ebx
			mov ebx, MarluxiaHealth
			div ebx

			//eax tiene el resultado del porcentaje de cuanta vida se le esta curando al personaje [(PuntosDeCura * 100)/Salud]
			//edx tiene el residuo restante

			cmp edx, 5 // Si el residuo es 5 o más, se redondeara el porcentaje, para obtener resultados más aproximados
			jb NoExtraPercent4

			inc eax // Agrega un 1% al porcentaje

			NoExtraPercent4 :

		mov ebx, WidthHealthbar
			mul ebx //Porcentaje de vida aumentada * Pixeles totales del Sprite de vida

			mov ebx, 100
			div ebx // (Porcentaje * Pixeles) / 100

			//eax tiene el resultado del numero de pixeles al que se le aumentara al ancho del sprite

			mov edi, HealthSpritePointer
			add[edi], eax // Ahora obtenido el porcentaje de los puntos de vida adquiridos, es el mismo porcentaje que se sumara al sprite de vida (Representado en pixeles)

			cmp[edi], 180 // Si el sprite sobrepasa el maximo de pixeles del sprite (180), se baja a 180
			ja HealthOverflow4
			jmp AssignValues4

			HealthOverflow4 :
		mov[edi], 180

			AssignValues4 :

			mov edx, EnemyHealth
			add edx, ecx //Agrega vida a la vida actual de Marluxia
			mov ebx, MarluxiaHealth

			cmp edx, ebx //Si la vida es mayor a la maxima posible, se regresa al maximo posible
			jbe SetHealthMarluxia

			mov edx, ebx

			SetHealthMarluxia :
		mov EnemyHealth, edx
	}
}

void DamageSoraMASM(int Damage) {
	int* HealthSpritePointer = &VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho;
	__asm {
		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0

		mov ecx, Damage //Puntos a curar a ecx
		cmp ecx, 0	//Si el numero random es cero, se cambia a uno
		jne DamageCalculations5

		mov ecx, 1

		DamageCalculations5:
		//Multiplicación: eax guarda el primer valor de una multiplicación y es remplazado por su resultado al final
		//El registro o variable multiplicado no es afectado, en este caso ebx
		mov eax, ecx
			mov ebx, 100
			mul ebx

			//División: eax guarda el numero a dividir y es remplazado por su resultado al final, edx guarda el residuo
			//El registro o variable dividido no es afectado, en este caso ebx
			mov ebx, SoraHealth
			div ebx

			//eax tiene el resultado del porcentaje de cuanta vida se le esta quitando al personaje [(PuntosDeDaño * 100)/SaludTotal]
			//edx tiene el residuo restante

			cmp edx, 5 // Si el residuo es 5 o más, se redondeara el porcentaje, para obtener resultados más aproximados
			jb NoExtraPercent5

			inc eax // Agrega un 1% al porcentaje

			NoExtraPercent5 :

		mov ebx, WidthHealthbar
			mul ebx //Porcentaje de vida a restar * Pixeles totales del Sprite de la barra vida (180)

			mov ebx, 100
			div ebx // (Porcentaje * Pixeles) / 100

			//eax tiene el resultado del numero de pixeles al que se le restara al ancho del sprite

			mov edi, HealthSpritePointer
			sub [edi], eax // Ahora obtenido el porcentaje de los puntos de vida a restar, es el mismo porcentaje que se restara al sprite de vida (Representado en pixeles)

			mov edx, PlayerHealth
			sub edx, ecx //Resta vida a la vida actual del jugador

			cmp edx, 0 //Si la vida es 0, se dejara de dibujar la barra de vida
			je BackToZeroSora
			cmp edx, 0xFFFF0000 //Si la vida se vuelve negativa, se regresara a 0 y se dejara de dibujar la barra de vida
			ja BackToZeroSora
			jmp SetHealthDamageSora

			BackToZeroSora :
			mov edx, 0 //Vida = 0
			mov ShowPlayerHealth, 0 //No se mostrara el sprite de la barra de vida si se regresa a 0

			SetHealthDamageSora :
			mov PlayerHealth, edx
	}
	if (VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho < 1) {
		VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho = 1;
	}
}

void DamageRikuMASM(int Damage) {
	int* HealthSpritePointer = &VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho;
	__asm {
		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0

		mov ecx, Damage //Puntos a curar a ecx
		cmp ecx, 0	//Si el numero random es cero, se cambia a uno
		jne DamageCalculations6

		mov ecx, 1

		DamageCalculations6:
		//Multiplicación: eax guarda el primer valor de una multiplicación y es remplazado por su resultado al final
		//El registro o variable multiplicado no es afectado, en este caso ebx
		mov eax, ecx
			mov ebx, 100
			mul ebx

			//División: eax guarda el numero a dividir y es remplazado por su resultado al final, edx guarda el residuo
			//El registro o variable dividido no es afectado, en este caso ebx
			mov ebx, RikuHealth
			div ebx

			//eax tiene el resultado del porcentaje de cuanta vida se le esta quitando al personaje [(PuntosDeDaño * 100)/SaludTotal]
			//edx tiene el residuo restante

			cmp edx, 5 // Si el residuo es 5 o más, se redondeara el porcentaje, para obtener resultados más aproximados
			jb NoExtraPercent6

			inc eax // Agrega un 1% al porcentaje

			NoExtraPercent6 :

		mov ebx, WidthHealthbar
			mul ebx //Porcentaje de vida a restar * Pixeles totales del Sprite de la barra vida (180)

			mov ebx, 100
			div ebx // (Porcentaje * Pixeles) / 100

			//eax tiene el resultado del numero de pixeles al que se le restara al ancho del sprite

			mov edi, HealthSpritePointer
			sub[edi], eax // Ahora obtenido el porcentaje de los puntos de vida a restar, es el mismo porcentaje que se restara al sprite de vida (Representado en pixeles)

			mov edx, EnemyHealth
			sub edx, ecx //Resta vida a la vida actual del jugador

			cmp edx, 0 //Si la vida es 0, se dejara de dibujar la barra de vida
			je BackToZeroRiku
			cmp edx, 0xFFFF0000 //Si la vida se vuelve negativa, se regresara a 0 y se dejara de dibujar la barra de vida
			ja BackToZeroRiku
			jmp SetHealthDamageRiku

			BackToZeroRiku :
		mov edx, 0 //Vida = 0
			mov ShowEnemyHealth, 0 //No se mostrara el sprite de la barra de vida si se regresa a 0

			SetHealthDamageRiku :
			mov EnemyHealth, edx

			mov ebx, TotalDamage
			add ebx, ecx
			mov TotalDamage, ebx //Se suma al daño total hecho por el jugador para calcular el puntaje más tarde
	}
	if (VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho < 1) {
		VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 1;
	}
}

void DamageAxelMASM(int Damage) {
	int* HealthSpritePointer = &VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho;
	__asm {
		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0

		mov ecx, Damage //Puntos a curar a ecx
		cmp ecx, 0	//Si el numero random es cero, se cambia a uno
		jne DamageCalculations7

		mov ecx, 1

		DamageCalculations7:
		//Multiplicación: eax guarda el primer valor de una multiplicación y es remplazado por su resultado al final
		//El registro o variable multiplicado no es afectado, en este caso ebx
		mov eax, ecx
			mov ebx, 100
			mul ebx

			//División: eax guarda el numero a dividir y es remplazado por su resultado al final, edx guarda el residuo
			//El registro o variable dividido no es afectado, en este caso ebx
			mov ebx, AxelHealth
			div ebx

			//eax tiene el resultado del porcentaje de cuanta vida se le esta quitando al personaje [(PuntosDeDaño * 100)/SaludTotal]
			//edx tiene el residuo restante

			cmp edx, 5 // Si el residuo es 5 o más, se redondeara el porcentaje, para obtener resultados más aproximados
			jb NoExtraPercent7

			inc eax // Agrega un 1% al porcentaje

			NoExtraPercent7:

		mov ebx, WidthHealthbar
			mul ebx //Porcentaje de vida a restar * Pixeles totales del Sprite de la barra vida (180)

			mov ebx, 100
			div ebx // (Porcentaje * Pixeles) / 100

			//eax tiene el resultado del numero de pixeles al que se le restara al ancho del sprite

			mov edi, HealthSpritePointer
			sub[edi], eax // Ahora obtenido el porcentaje de los puntos de vida a restar, es el mismo porcentaje que se restara al sprite de vida (Representado en pixeles)

			mov edx, EnemyHealth
			sub edx, ecx //Resta vida a la vida actual del jugador

			cmp edx, 0 //Si la vida es 0, se dejara de dibujar la barra de vida
			je BackToZeroAxel
			cmp edx, 0xFFFF0000 //Si la vida se vuelve negativa, se regresara a 0 y se dejara de dibujar la barra de vida
			ja BackToZeroAxel
			jmp SetHealthDamageAxel

			BackToZeroAxel:
		mov edx, 0 //Vida = 0
			mov ShowEnemyHealth, 0 //No se mostrara el sprite de la barra de vida si se regresa a 0

			SetHealthDamageAxel:
			mov EnemyHealth, edx

			mov ebx, TotalDamage
			add ebx, ecx
			mov TotalDamage, ebx //Se suma al daño total hecho por el jugador para calcular el puntaje más tarde
	}
	if (VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho < 1) {
		VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 1;
	}
}

void DamageMarluxiaMASM(int Damage) {
	int* HealthSpritePointer = &VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho;
	__asm {
		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0

		mov ecx, Damage //Puntos a curar a ecx
		cmp ecx, 0	//Si el numero random es cero, se cambia a uno
		jne DamageCalculations8

		mov ecx, 1

		DamageCalculations8:
		//Multiplicación: eax guarda el primer valor de una multiplicación y es remplazado por su resultado al final
		//El registro o variable multiplicado no es afectado, en este caso ebx
		mov eax, ecx
			mov ebx, 100
			mul ebx

			//División: eax guarda el numero a dividir y es remplazado por su resultado al final, edx guarda el residuo
			//El registro o variable dividido no es afectado, en este caso ebx
			mov ebx, MarluxiaHealth
			div ebx

			//eax tiene el resultado del porcentaje de cuanta vida se le esta quitando al personaje [(PuntosDeDaño * 100)/SaludTotal]
			//edx tiene el residuo restante

			cmp edx, 5 // Si el residuo es 5 o más, se redondeara el porcentaje, para obtener resultados más aproximados
			jb NoExtraPercent8

			inc eax // Agrega un 1% al porcentaje

			NoExtraPercent8 :

		mov ebx, WidthHealthbar
			mul ebx //Porcentaje de vida a restar * Pixeles totales del Sprite de la barra vida (180)

			mov ebx, 100
			div ebx // (Porcentaje * Pixeles) / 100

			//eax tiene el resultado del numero de pixeles al que se le restara al ancho del sprite

			mov edi, HealthSpritePointer
			sub[edi], eax // Ahora obtenido el porcentaje de los puntos de vida a restar, es el mismo porcentaje que se restara al sprite de vida (Representado en pixeles)

			mov edx, EnemyHealth
			sub edx, ecx //Resta vida a la vida actual del jugador

			cmp edx, 0 //Si la vida es 0, se dejara de dibujar la barra de vida
			je BackToZeroMarluxia
			cmp edx, 0xFFFF0000 //Si la vida se vuelve negativa, se regresara a 0 y se dejara de dibujar la barra de vida
			ja BackToZeroMarluxia
			jmp SetHealthDamageMarluxia

			BackToZeroMarluxia:
		mov edx, 0 //Vida = 0
			mov ShowEnemyHealth, 0 //No se mostrara el sprite de la barra de vida si se regresa a 0

			SetHealthDamageMarluxia:
			mov EnemyHealth, edx

			mov ebx, TotalDamage
			add ebx, ecx
			mov TotalDamage, ebx //Se suma al daño total hecho por el jugador para calcular el puntaje más tarde
	}
	if (VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho < 1) {
		VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 1;
	}
}
#pragma endregion

void DisplayNumber(int Number) {
	ShowNums = true;
	NumFrameActual = 0;
	int Digito1 = 0, Digito2 = 0;
	if (Number < 100) {
		if (Number >= 10) {
			Digito2 = Number % 10;
			Digito1 = (Number / 10) % 10;
		}
		else {
			Digito2 = Number;
		}
		switch (Digito1) {
		case 1:
			Num1AnimacionActual = NUM1;
			break;

		case 2:
			Num1AnimacionActual = NUM2;
			break;

		case 3:
			Num1AnimacionActual = NUM3;
			break;

		case 4:
			Num1AnimacionActual = NUM4;
			break;

		case 5:
			Num1AnimacionActual = NUM5;
			break;

		case 6:
			Num1AnimacionActual = NUM6;
			break;

		case 7:
			Num1AnimacionActual = NUM7;
			break;

		case 8:
			Num1AnimacionActual = NUM8;
			break;

		case 9:
			Num1AnimacionActual = NUM9;
			break;

		default:
			Num1AnimacionActual = NUM0;
			break;
		}
		switch (Digito2) {
		case 1:
			Num2AnimacionActual = NUM1;
			break;

		case 2:
			Num2AnimacionActual = NUM2;
			break;

		case 3:
			Num2AnimacionActual = NUM3;
			break;

		case 4:
			Num2AnimacionActual = NUM4;
			break;

		case 5:
			Num2AnimacionActual = NUM5;
			break;

		case 6:
			Num2AnimacionActual = NUM6;
			break;

		case 7:
			Num2AnimacionActual = NUM7;
			break;

		case 8:
			Num2AnimacionActual = NUM8;
			break;

		case 9:
			Num2AnimacionActual = NUM9;
			break;

		default:
			Num2AnimacionActual = NUM0;
			break;
		}
	}
	else {
		Num1AnimacionActual = NUM0;
		Num2AnimacionActual = NUM0;
	}
}

//void SetDamage(bool ToPlayer) version c++
//
//void SetDamage(bool ToPlayer) {
//	int Damage = 0;
//	int DamagePercent = 0;
//	srand((unsigned)time(NULL));
//	switch (CurrentStage) {
//	case Stage1:
//		if (ToPlayer) {
//			if (PlayerShield) {
//				Damage = rand() % 10;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//			else {
//				Damage = rand() % 25;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//
//			//Calcula que porcentaje de daño se le hara al personaje
//			//Para poder modificar el largo de la barra de vida
//			//correspondiente
//			DamagePercent = (Damage * 100) / SoraHealth;
//			if ((Damage * 100) % SoraHealth >= 5) {
//				DamagePercent++;
//			}
//			VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho -= ((WidthHealthbar * DamagePercent) / 100);
//			if (VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho < 1) {
//				VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho = 1;
//				ShowPlayerHealth = false;
//			}
//		}
//		else {
//			if (EnemyShield) {
//				Damage = rand() % 10;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//			else {
//				Damage = rand() % 25;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//			DamagePercent = (Damage * 100) / RikuHealth;
//			if ((Damage * 100) % RikuHealth >= 5) {
//				DamagePercent++;
//			}
//			VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho -= ((WidthHealthbar * DamagePercent) / 100);
//			if (VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho < 1) {
//				VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 1;
//				ShowEnemyHealth = false;
//			}
//		}
//		break;
//
//	case Stage2:
//		if (ToPlayer) {
//			if (PlayerShield) {
//				Damage = rand() % 10;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//			else {
//				Damage = rand() % 30;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//			DamagePercent = ((Damage * 100) / SoraHealth);
//			if ((Damage * 100) % SoraHealth >= 5) {
//				DamagePercent++;
//			}
//			VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho -= ((WidthHealthbar * DamagePercent) / 100) + 1;
//			if (VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho < 1) {
//				VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho = 1;
//				ShowPlayerHealth = false;
//			}
//		}
//		else {
//			if (EnemyShield) {
//				Damage = rand() % 20;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//			else {
//				Damage = rand() % 35;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//			DamagePercent = (Damage * 100) / AxelHealth;
//			if ((Damage * 100) % AxelHealth >= 5) {
//				DamagePercent++;
//			}
//			VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho -= ((WidthHealthbar * DamagePercent) / 100);
//			if (VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho < 1) {
//				VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 1;
//				ShowEnemyHealth = false;
//			}
//		}
//		break;
//
//	case Stage3:
//		if (ToPlayer) {
//			if (PlayerShield) {
//				Damage = rand() % 15;
//			}
//			else {
//				Damage = rand() % 45;
//			}
//			DamagePercent = (Damage * 100) / SoraHealth;
//			if ((Damage * 100) % SoraHealth >= 5) {
//				DamagePercent++;
//			}
//			VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho -= ((WidthHealthbar * DamagePercent) / 100);
//			if (VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho < 1) {
//				VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho = 1;
//				ShowPlayerHealth = false;
//			}
//		}
//		else {
//			if (EnemyShield) {
//				Damage = rand() % 25;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//			else {
//				Damage = rand() % 65;
//				if (Damage == 0) {
//					Damage = 1;
//				}
//			}
//			DamagePercent = (Damage * 100) / MarluxiaHealth;
//			if ((Damage * 100) % MarluxiaHealth >= 5) {
//				DamagePercent++;
//			}
//			VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho -= ((WidthHealthbar * DamagePercent) / 100);
//			if (VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho < 1) {
//				VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 1;
//				ShowEnemyHealth = false;
//			}
//		}
//		break;
//	}
//
//	if (ToPlayer) {
//		PlayerHealth -= Damage;
//		if (PlayerHealth < 0) PlayerHealth = 0;
//		if (PlayerHealth != 0 && !ShowPlayerHealth) {
//			ShowPlayerHealth = true;
//		}
//	}
//	else{
//		EnemyHealth -= Damage;
//		if (EnemyHealth < 0) EnemyHealth = 0;
//		if (EnemyHealth != 0 && !ShowEnemyHealth) {
//			ShowEnemyHealth = true;
//		}
//		TotalDamage += Damage;
//	}
//	DisplayNumber(Damage);
//}

void SetDamage(bool ToPlayer) {
	int Damage = 0;
	srand((unsigned)time(NULL));
	switch (CurrentStage) {
	case Stage1:
		if (ToPlayer) {
			if (PlayerShield) {
				Damage = rand() % 10;
			}
			else{
				Damage = rand() % 25;
			}
			//Calcula que porcentaje de daño se le hara al personaje
			//Para poder modificar el largo de la barra de vida
			//correspondiente
			DamageSoraMASM(Damage);
		}
		else {
			if (EnemyShield) {
				Damage = rand() % 10;
			}
			else {
				Damage = rand() % 25;
			}
			DamageRikuMASM(Damage);
		}
		break;

	case Stage2:
		if (ToPlayer) {
			if (PlayerShield) {
				Damage = rand() % 10;
			}
			else {
				Damage = rand() % 30;
			}
			DamageSoraMASM(Damage);
		}
		else {
			if (EnemyShield) {
				Damage = rand() % 20;
			}
			else {
				Damage = rand() % 35;
			}
			DamageAxelMASM(Damage);
		}
		break;

	case Stage3:
		if (ToPlayer) {
			if (PlayerShield) {
				Damage = rand() % 15;
			}
			else {
				Damage = rand() % 45;
			}
			DamageSoraMASM(Damage);
		}
		else {
			if (EnemyShield) {
				Damage = rand() % 25;
			}
			else {
				Damage = rand() % 65;
			}
			DamageMarluxiaMASM(Damage);
		}
		break;
	}
	DisplayNumber(Damage);
}

//void SetHealing(bool ToPlayer) versión c++
// 
//void SetHealing(bool IsPlayer) {
//
//	int Heal = 0;
//	//Variable usada para la version en c++
//	int HealPercent = 0;
//	srand((unsigned)time(NULL));
//
//	switch (CurrentStage) {
//	case Stage1:
//		if (IsPlayer) {
//			Heal = rand() % 20;
//
//			//HealSoraMASM(Heal);
//
//			//Version de c++
//
//			if (Heal == 0) {
//				Heal = 1;
//			}
//			//Calcula que porcentaje de curación se le hara al personaje
//			//Para poder modificar el largo de la barra de vida
//			//correspondiente
//			HealPercent = (Heal * 100) / SoraHealth;
//			if ((Heal * 100) % SoraHealth >= 5) {
//				HealPercent++;
//			}
//			VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho += ((WidthHealthbar * HealPercent) / 100);
//			if (VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho > 180) {
//				VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho = 180;
//			}
//		}
//		else {
//			Heal = rand() % 15;
//
//			//HealRikuMASM(Heal);
//
//			//Version en c++
//
//			if (Heal == 0) {
//				Heal = 1;
//			}
//			HealPercent = (Heal * 100) / RikuHealth;
//			if ((Heal * 100) % RikuHealth >= 5) {
//				HealPercent++;
//			}
//			VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho += ((WidthHealthbar * HealPercent) / 100);
//			if (VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho > 180) {
//				VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 180;
//			}
//		}
//		break;
//
//	case Stage2:
//		if (IsPlayer) {
//			Heal = rand() % 35;
//
//			//HealSoraMASM(Heal);
//
//			//Version en c++
//
//			if (Heal == 0) {
//				Heal = 1;
//			}
//			HealPercent = ((Heal * 100) / SoraHealth);
//			if ((Heal * 100) % SoraHealth >= 5) {
//				HealPercent++;
//			}
//			VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho += ((WidthHealthbar * HealPercent) / 100) + 1;
//			if (VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho > 180) {
//				VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho = 180;
//			}
//		}
//		else {
//			Heal = rand() % 20;
//
//			//HealAxelMASM(Heal);
//
//			//Version en c++
//
//			if (Heal == 0) {
//				Heal = 1;
//			}
//			HealPercent = (Heal * 100) / AxelHealth;
//			if ((Heal * 100) % AxelHealth >= 5) {
//				HealPercent++;
//			}
//			VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho += ((WidthHealthbar * HealPercent) / 100);
//			if (VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho > 180) {
//				VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 180;
//			}
//		}
//		break;
//
//	case Stage3:
//		if (IsPlayer) {
//			Heal = rand() % 50;
//
//			//HealSoraMASM(Heal);
//
//			//Version en c++
//
//			if (Heal == 0) {
//				Heal = 1;
//			}
//			HealPercent = (Heal * 100) / SoraHealth;
//			if ((Heal * 100) % SoraHealth >= 5) {
//				HealPercent++;
//			}
//			VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho += ((WidthHealthbar * HealPercent) / 100);
//			if (VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho > 180) {
//				VidaPlayer.FrameSpriteArray[VidaPlayer.idBasicUI][Frame0].ancho = 180;
//			}
//		}
//		else {
//			Heal = rand() % 55;
//
//			//HealMarluxiaMASM(Heal);
//
//			//Version en c++
//
//			if (Heal == 0) {
//				Heal = 1;
//			}
//			HealPercent = (Heal * 100) / MarluxiaHealth;
//			if ((Heal * 100) % MarluxiaHealth >= 5) {
//				HealPercent++;
//			}
//			VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho += ((WidthHealthbar * HealPercent) / 100);
//			if (VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho > 180) {
//				VidaEnemy.FrameSpriteArray[VidaEnemy.idBasicUI][Frame0].ancho = 180;
//			}
//		}
//		break;
//	}
//	if (IsPlayer) {
//		PlayerHealth += Heal;
//		if (PlayerHealth > SoraHealth) PlayerHealth = SoraHealth;
//	}
//	else {
//		EnemyHealth += Heal;
//		switch (CurrentStage) {
//			case Stage1:
//				if (EnemyHealth > RikuHealth) EnemyHealth = RikuHealth;
//				break;
//			case Stage2:
//				if (EnemyHealth > AxelHealth) EnemyHealth = AxelHealth;
//				break;
//			case Stage3:
//				if (EnemyHealth > MarluxiaHealth) EnemyHealth = MarluxiaHealth;
//				break;
//		}
//	}
//	DisplayNumber(Heal);
//}

void SetHealing(bool IsPlayer) {
	int Heal = 0;
	srand(time(NULL));
	switch (CurrentStage) {
	case Stage1:
		if (IsPlayer) {
			Heal = rand() % 20;

			HealSoraMASM(Heal);
		}
		else {
			Heal = rand() % 15;

			HealRikuMASM(Heal);
		}
		break;

	case Stage2:
		if (IsPlayer) {
			Heal = rand() % 35;
			
			HealSoraMASM(Heal);
		}
		else {
			Heal = rand() % 20;
			
			HealAxelMASM(Heal);
		}
		break;

	case Stage3:
		if (IsPlayer) {
			Heal = rand() % 50;

			HealSoraMASM(Heal);
		}
		else {
			Heal = rand() % 55;
			
			HealMarluxiaMASM(Heal);
		}
		break;
	}
	if (Heal == 0) {
		Heal = 1;
	}
	DisplayNumber(Heal);
}

void ReproductorInicializaYReproduce() {
	Cancion[0].Nombre = "Inicio";
	Cancion[0].Dir = "Recursos/Dearly Beloved.mp3";
	Cancion[1].Nombre = "Stage1";
	Cancion[1].Dir = "Recursos/Sinister Sundown.mp3";
	Cancion[2].Nombre = "Stage2";
	Cancion[2].Dir = "Recursos/The 13th Struggle.mp3";
	Cancion[3].Nombre = "Stage3";
	Cancion[3].Dir = "Recursos/Lord of the Castle.mp3";
	Cancion[4].Nombre = "WinScreen";
	Cancion[4].Dir = "Recursos/Final Fantasy VII - Victory Fanfare Loop.mp3";
	ifstream inputFile(Cancion[0].Dir.c_str());

	if (!inputFile.good()) 
		printf("No file found");
	else 
		player->OpenFile(Cancion[0].Dir.c_str(), sfAutodetect);
	player->Play();

	TStreamTime pTime;
	pTime.sec = 1;
	player->Seek(tfSecond, &pTime, smFromCurrentForward);
	player->SetPlayerVolume(100, 100);
}

void ReproductorCambiarCancionYReproduce(int NumeroCancionAeproducir) {
	player->Stop();
	ifstream inputFile(Cancion[NumeroCancionAeproducir].Dir.c_str());

	if (!inputFile.good())
		printf("No file found");
	else
		player->OpenFile(Cancion[NumeroCancionAeproducir].Dir.c_str(), sfAutodetect);

	//Se baja el volumen de la canción del 3er stage
	if (NumeroCancionAeproducir == 3) {
		player->SetPlayerVolume(80, 80);
	}
	else {
		player->SetPlayerVolume(100, 100);
	}
	player->Play();
}

void ReproductorPausa() {
	player->Pause();
	pausa = true;
}

void ReproductorReproduce() {
	if(pausa)
		player->Resume();
	else 
		player->Play();
	pausa = false;
}