//--------------------------------------------------------------------------
// Programação Windows: C e Win32 API com ênfase em Multimídia
// Autor: André Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, André Kishimoto
//
// prog08-2.cpp – Uso de timers
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "prog08-2.h"

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
// Nome da classe da janela
#define WINDOW_CLASS		"prog08-2"
// Título da janela
#define WINDOW_TITLE		"Prog 08-2"
// Largura da janela
#define WINDOW_WIDTH		320
// Altura da janela
#define WINDOW_HEIGHT		240

//--------------------------------------------------------------------------
// Protótipo das funções
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

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

	// Contador para demonstrar o funcionamento do timer
	static int i = 0;
	char c[2] = { 0 };

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
      /* Devemos avisar manualmente ao Windows que a janela já foi atualizada, pois não é um processo automático. Se isso não for feito, o Windows não irá parar de enviar a mensagem WM_PAINT ao programa. */

      // O código abaixo avisa o Windows que a janela já foi atualizada.
      hDC = BeginPaint(hWnd, &psPaint);
      EndPaint(hWnd, &psPaint);

      return(0);
    } break;

    case WM_CLOSE: // Janela foi fechada
		{
			// Desativa/destrói os timers (para que não haja falta de recursos)
			KillTimer(hWnd, IDT_TIMER1);
			KillTimer(hWnd, IDT_TIMER2);

			// Destrói a janela
      DestroyWindow(hWnd);

      return(0);
    } break;

    case WM_DESTROY: // Janela foi destruída
    {
      // Envia mensagem WM_QUIT para o loop de mensagens
      PostQuitMessage(WM_QUIT);

      return(0);
    } break;

    case WM_KEYDOWN: // Obtém tecla virtual
    {
			switch(wParam)
			{
				case VK_F1: // Ativa primeiro timer
				{
					// Muda título do programa
					SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"Timer 1 ativado!");

					// Ativa o primeiro timer com mensagem WM_TIMER
					SetTimer(hWnd, IDT_TIMER1, 300, NULL);
					i = 0;
				} break;

				case VK_F2: // Desativa primeiro timer
				{
					// Muda título do programa
					SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"Timer 1 desativado!");

					// Desativa/destrói o primeiro timer
					KillTimer(hWnd, IDT_TIMER1);
				} break;

				case VK_F3: // Ativa segundo timer
				{
					// Muda título do programa
					SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"Timer 2 ativado!");

					// Ativa o segundo timer com o uso de uma função callback
					SetTimer(hWnd, IDT_TIMER2, 1000, (TIMERPROC)TimerProc);
				} break;

				case VK_F4: // Desativa segundo timer
				{
					// Muda título do programa
					SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"Timer 2 desativado!");

					// Desativa/destrói o segundo timer
					KillTimer(hWnd, IDT_TIMER2);
				} break;
			}

      return(0);
    } break;

		case WM_TIMER: // Algum timer foi ativado
		{
			// Identificador do timer ativado é IDT_TIMER1
			if(wParam == IDT_TIMER1)
			{
				// Incrementa contador (até chegar a 9) e mostra o valor na área cliente
				i ++;
				if(i > 9)
					i = 0;
				hDC = GetDC(hWnd);
				sprintf(c, "%d", i);
				TextOut(hDC, 8, 8, c, (int)strlen(c));
				ReleaseDC(hWnd, hDC);
			}

			return(0);
		} break;

    default: // Outra mensagem
    {
      /* Deixa o Windows processar as mensagens que não foram verificadas na função */
      return(DefWindowProc(hWnd, uMsg, wParam, lParam));
    }

  }
}

//--------------------------------------------------------------------------
// TimerProc() -> Processa os eventos de timer
//--------------------------------------------------------------------------
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	// Armazena hora atual
	char cHora[9] = { 0 };

	// Identificador do timer ativado é IDT_TIMER2
	if(idEvent == IDT_TIMER2)
	{
		// Obtém hora atual (a cada segundo) e mostra na área cliente
		_strtime(cHora);
		HDC hDC = GetDC(hWnd);
		TextOut(hDC, 8, 28, cHora, (int)strlen(cHora));
		ReleaseDC(hWnd, hDC);
  }
}