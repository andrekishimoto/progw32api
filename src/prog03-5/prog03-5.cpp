//--------------------------------------------------------------------------
// Programação Windows: C e Win32 API com ênfase em Multimídia
// Autor: André Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, André Kishimoto
//
// prog03-5.cpp - Menus e teclas de atalho
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

// Cabeçalho dos recursos
#include "prog03-5-res.h"

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog03-5"
// Título da janela 
#define WINDOW_TITLE		"Prog 03-5"
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
  wcl.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEUICONE)); // Carrega ícone do recurso
  wcl.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MEUCURSOR)); // Carrega cursor do recurso
  wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wcl.lpszMenuName = NULL;
//  wcl.lpszMenuName = MAKEINTRESOURCE(IDM_MEUMENU); // Carrega menu do recurso (método 1)
  wcl.lpszClassName = WINDOW_CLASS;
  wcl.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEUICONE)); // Carrega ícone do recurso

  // Cria identificador para o menu do recurso
  HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MEUMENU));

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
		   NULL, // Ou passa o identificador hMenu, carregando menu do recurso (método 2)
		   hInstance,
		   NULL);

    // Verifica se a janela foi criada
    if(hWnd)
    {
	  // Define menu da janela (método 3)
	  SetMenu(hWnd, hMenu);

	  // Carrega teclas de atalho e cria identificador para elas
  	  HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDA_MEUATALHO));

      // Mostra a janela
      ShowWindow(hWnd, nCmdShow);
      UpdateWindow(hWnd);

	  // Armazena dados da mensagem que será obtida
      MSG msg;

      // Loop de mensagens, enquanto mensagem não for WM_QUIT,
      // obtém mensagem da fila de mensagens
      while(GetMessage(&msg, NULL, 0, 0) > 0)
      {
		// Verifica se tecla de atalho foi ativada, se for, não executa
		// TranslateMessage() nem DispatchMessage(), pois já foram processadas
	    if(!TranslateAccelerator(hWnd, hAccel, &msg))
		{
          // Traduz teclas virtuais ou aceleradoras (de atalho)
          TranslateMessage(&msg);

          // Envia mensagem para a função que processa mensagens (WindowProc)
          DispatchMessage(&msg);
		}
      }

	  // Destrói o menu e libera memória
	  DestroyMenu(hMenu);

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

	case WM_COMMAND: // Item do menu, tecla de atalho ou controle ativado
	{
      // Verifica bit menos significativo de wParam (ID’s)
	  switch(LOWORD(wParam))
	  {
	    case ID_DO_MENU1_ITEM1:
		{
		  MessageBox(hWnd, "Menu 1... Item 1", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU1_ITEM2:
		{
		  MessageBox(hWnd, "Menu 1... Item 2", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU1_ITEMX:
		{
		  MessageBox(hWnd, "Menu 1... Item X", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM1:
		{
		  MessageBox(hWnd, "Menu 2... Item 1", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM2:
		{
		  MessageBox(hWnd, "Menu 2... Item 2", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEMX:
		{
		  MessageBox(hWnd, "Menu 2... Item X", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM3_1:
		{
		  MessageBox(hWnd, "Menu 2... Item 3... Item 3.1", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM3_2:
		{
		  MessageBox(hWnd, "Menu 2... Item 3... Item 3.2", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM3_X:
		{
		  MessageBox(hWnd, "Menu 2... Item 3... Item 3.X", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case IDA_ATALHO_ESC:
		{
		  // Destrói a janela
          DestroyWindow(hWnd);
		} break;
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