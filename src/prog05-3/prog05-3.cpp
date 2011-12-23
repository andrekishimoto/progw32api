//--------------------------------------------------------------------------
// Programação Windows: C e Win32 API com ênfase em Multimídia
// Autor: André Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, André Kishimoto
//
// prog05-3.cpp - Desenhando linhas curvas
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog05-3"
// Título da janela 
#define WINDOW_TITLE		"Prog 05-3"
// Largura da janela 
#define WINDOW_WIDTH		320
// Altura da janela 
#define WINDOW_HEIGHT		240

//--------------------------------------------------------------------------
// Protótipo das funções
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

//--------------------------------------------------------------------------
// WinMain() -> Função principal
//--------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
  // Cria a classe da janela e especifica seus atributos
  WNDCLASSEX wcl;
  wcl.cbSize = sizeof(WNDCLASSEX);
  wcl.style = CS_HREDRAW | CS_VREDRAW;
  wcl.lpfnWndProc = (WNDPROC)WindowProc;
  wcl.cbClsExtra = 0;
  wcl.cbWndExtra = 0;
  wcl.hInstance = hInstance;
  wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wcl.lpszMenuName = NULL;
  wcl.lpszClassName = WINDOW_CLASS;
  wcl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  // Registra a classe da janela
  if(RegisterClassEx(&wcl))
  {
    // Cria a janela principal do programa
    HWND hWnd = NULL;
    hWnd = CreateWindowEx(
           NULL,
		   WINDOW_CLASS,
		   WINDOW_TITLE,
		   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		   (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2,
		   (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2,
		   WINDOW_WIDTH,
		   WINDOW_HEIGHT,
		   HWND_DESKTOP,
		   NULL,
		   hInstance,
		   NULL);

    // Verifica se a janela foi criada
    if(hWnd)
    {
      // Mostra a janela
      ShowWindow(hWnd, nCmdShow);
      UpdateWindow(hWnd);

      // Armazena dados da mensagem que será obtida
      MSG msg;

      // Loop de mensagens, enquanto mensagem não for WM_QUIT,
      // obtém mensagem da fila de mensagens
      while(GetMessage(&msg, NULL, 0, 0) > 0)
      {
        // Traduz teclas virtuais ou aceleradoras (de atalho)
        TranslateMessage(&msg);

        // Envia mensagem para a função que processa mensagens (WindowProc)
        DispatchMessage(&msg);
      }

      // Retorna ao Windows com valor de msg.wParam
      return((int)msg.wParam);
     }
     // Se a janela não foi criada
     else
     {
       // Exibe mensagem de erro e sai do programa
       MessageBox(NULL, "Não foi possível criar janela.", "Erro!", MB_OK | MB_ICONERROR);

       return(0);
     }
  }
  // Se a classe da janela não foi registrada
  else
  {
    // Exibe mensagem de erro e sai do programa
    MessageBox(NULL, "Não foi possível registrar a classe da janela.", "Erro!", MB_OK | MB_ICONERROR);

    return(0);
  }
	
  // Retorna ao Windows sem passar pelo loop de mensagens
  return(0);
}

//--------------------------------------------------------------------------
// WindowProc() -> Processa as mensagens enviadas para o programa
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  // Variáveis para manipulação da parte gráfica do programa
  HDC hDC = NULL;
  PAINTSTRUCT psPaint;

  // Canetas e pincéis
  HPEN hPen = NULL;
  HPEN hPenOld = NULL;
  HBRUSH hBrush = NULL;
  HBRUSH hBrushOld = NULL;
  
  // Verifica qual foi a mensagem enviada
  switch(uMsg)
  {

    case WM_CREATE: // Janela foi criada
    {
      // Retorna 0, significando que a mensagem foi processada corretamente
      return(0);
    } break;
	
    case WM_PAINT: // Janela (ou parte dela) precisa ser atualizada
    {
	  RECT rcRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	  // Obtém identificador do DC e preenche PAINTSTRUCT
      hDC = BeginPaint(hWnd, &psPaint);

	  // Cria e seleciona nova caneta no DC e salva caneta antiga
	  hPen = CreatePen(PS_SOLID, 1, RGB(0, 128, 0));
	  hPenOld = (HPEN)SelectObject(hDC, hPen);

	  // Desenha um arco
	  Arc(hDC, 10, 10, 50, 50, 50, 40, 0, 0);

	  // Define extremidades e pontos de controle da curva
	  POINT ptUmaCurva[4];
	  ptUmaCurva[0].x = 290; ptUmaCurva[0].y = 5;   // extremidade
	  ptUmaCurva[1].x = 70 ; ptUmaCurva[1].y = 10;  // pt controle
	  ptUmaCurva[2].x = 190; ptUmaCurva[2].y = 100; // pt controle
	  ptUmaCurva[3].x = 200; ptUmaCurva[3].y = 100; // extremidade

	  // Desenha a curva
	  PolyBezier(hDC, ptUmaCurva, 4);

	  // Define extremidades e pontos de controle das 3 curvas
	  POINT ptTresCurvas[10];
	  ptTresCurvas[0].x = 10 ; ptTresCurvas[0].y = 100; // extremidade
	  ptTresCurvas[1].x = 40 ; ptTresCurvas[1].y = 30;  // pt controle
	  ptTresCurvas[2].x = 50 ; ptTresCurvas[2].y = 130; // pt controle
	  ptTresCurvas[3].x = 100; ptTresCurvas[3].y = 100; // extremidade
	  ptTresCurvas[4].x = 140; ptTresCurvas[4].y = 75;  // pt controle
	  ptTresCurvas[5].x = 170; ptTresCurvas[5].y = 90;  // pt controle
	  ptTresCurvas[6].x = 200; ptTresCurvas[6].y = 200; // extremidade
	  ptTresCurvas[7].x = 220; ptTresCurvas[7].y = 180; // pt controle
	  ptTresCurvas[8].x = 200; ptTresCurvas[8].y = 140; // pt controle
	  ptTresCurvas[9].x = 240; ptTresCurvas[9].y = 100; // extremidade

	  // Desenha as 3 curvas
	  PolyBezier(hDC, ptTresCurvas, 10);

	  // Restaura pincel antigo e deleta novo pincel
	  SelectObject(hDC, hBrushOld);
	  DeleteObject(hBrush);

	  // Restaura caneta antiga e deleta nova caneta
	  SelectObject(hDC, hPenOld);
	  DeleteObject(hPen);

	  // Libera DC e valida área
      EndPaint(hWnd, &psPaint);

      return(0);
    } break;

    case WM_CLOSE: // Janela foi fechada
    {
      // Destrói a janela
      DestroyWindow(hWnd);

      return(0);
    } break;
	
    case WM_DESTROY: // Janela foi destruída
    {
      // Envia mensagem WM_QUIT para o loop de mensagens
      PostQuitMessage(0);

      return(0);
    } break;

    default: // Outra mensagem
    {
      /* Deixa o Windows processar as mensagens que não foram verificadas na função */
	  return(DefWindowProc(hWnd, uMsg, wParam, lParam));
    }

  }
}