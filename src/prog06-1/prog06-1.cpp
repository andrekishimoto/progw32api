//--------------------------------------------------------------------------
// Programação Windows: C e Win32 API com ênfase em Multimídia
// Autor: André Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, André Kishimoto
//
// prog06-1.cpp - Mostrando bitmaps com BitBlt() e StretchBlt()
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog06-1"
// Título da janela 
#define WINDOW_TITLE		"Prog 06-1"
// Largura da janela 
#define WINDOW_WIDTH		330
// Altura da janela 
#define WINDOW_HEIGHT		240

//--------------------------------------------------------------------------
// Variáveis globais
//--------------------------------------------------------------------------
HDC hMemDC = NULL; // DC de memória
HBITMAP hBmp = NULL; // Bitmap compatível

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

  // Flag indicando se mostra ou não bitmap invertido
  static bool bInverteBmp = false;

  // Verifica qual foi a mensagem enviada
  switch(uMsg)
  {

    case WM_CREATE: // Janela foi criada
    {
      // Cria DC de memória (hMemDC é global)
      hDC = GetDC(hWnd);
      hMemDC = CreateCompatibleDC(hDC);

      // Cria / carrega bitmap do arquivo "prog06-1.bmp" (hBmp é global)
      hBmp = (HBITMAP)LoadImage(NULL, "prog06-1.bmp", IMAGE_BITMAP, 160, 120, LR_LOADFROMFILE);

      // Seleciona bitmap no DC de memória (configura DC de memória)
      SelectObject(hMemDC, hBmp);

      // Retorna 0, significando que a mensagem foi processada corretamente
      return(0);
    } break;

    case WM_PAINT: // Janela (ou parte dela) precisa ser atualizada
    {
      // Obtém DC de vídeo
      hDC = BeginPaint(hWnd, &psPaint);

      // Faz transferência de bits entre os DC's de memória e vídeo
      BitBlt(hDC, 0, 0, 160, 120, hMemDC, 0, 0, SRCCOPY);

		  // Verifica se bitmap não está invertido
		  if(!bInverteBmp)
			{
				// Faz transferência de bits entre os DC's de memória e vídeo
        StretchBlt(hDC, 160, 0, 160, 120, hMemDC, 0, 0, 160, 120, SRCCOPY);

        // Faz transferência de bits entre os DC's de memória e vídeo
        // (alargando a figura)
        StretchBlt(hDC, 0, 120, 240, 90, hMemDC, 0, 0, 160, 120, SRCCOPY);

        // Faz transferência de bits entre os DC's de memória e vídeo
        // (comprimindo a figura)
        StretchBlt(hDC, 240, 120, 80, 60, hMemDC, 0, 0, 160, 120, SRCCOPY);
			}
			else // Mostrar bitmap invertido
			{
        // Parâmetros de StretchBlt() para inverter bitmap:
        // nXOriginDest = Ponto X de destino + largura do bitmap;
        // nYOriginDest = Ponto Y de destino + altura do bitmap;
        // nWidthDest = Valor negativo (inverte imagem na horizontal);
        // nHeightDest = Valor negativo (inverte imagem na vertical);

        // Faz transferência de bits entre os DC's de memória e vídeo
				// (bitmap invertido na horizontal)
        StretchBlt(hDC, 320, 0, -160, 120, hMemDC, 0, 0, 160, 120, SRCCOPY);

        // Faz transferência de bits entre os DC's de memória e vídeo
        // (alargando a figura, bitmap invertido na horizontal e vertical)
        StretchBlt(hDC, 240, 210, -240, -90, hMemDC, 0, 0, 160, 120, SRCCOPY);

        // Faz transferência de bits entre os DC's de memória e vídeo
        // (comprimindo a figura, bitmap invertido na vertical)
        StretchBlt(hDC, 240, 180, 80, -60, hMemDC, 0, 0, 160, 120, SRCCOPY);
			}

      // Libera DC de vídeo
      EndPaint(hWnd, &psPaint);

      return(0);
    } break;

    case WM_CLOSE: // Janela foi fechada
    {
      // Deleta bitmap
      DeleteObject(SelectObject(hMemDC, hBmp));

      // Deleta DC de memória
      DeleteDC(hMemDC);

      // Destrói a janela
      DestroyWindow(hWnd);

      return(0);
    } break;

		case WM_LBUTTONDOWN: // Botão esquerdo do mouse pressionado
		{
			// Muda flag que indica se bitmap está invertido ou não
			bInverteBmp = !bInverteBmp;

			// Invalida área cliente (para atualização da mesma)
			InvalidateRect(hWnd, NULL, FALSE);

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
    } break;

  }
}