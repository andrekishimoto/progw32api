//--------------------------------------------------------------------------
// Programação Windows: C e Win32 API com ênfase em Multimídia
// Autor: André Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, André Kishimoto
//
// prog03-7.cpp - Caixa de diálogo modal e modeless
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

// Cabeçalho dos recursos
#include "prog03-7-res.h"

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog03-7"
// Título da janela 
#define WINDOW_TITLE		"Prog 03-7"
// Largura da janela 
#define WINDOW_WIDTH		320
// Altura da janela 
#define WINDOW_HEIGHT		240

//--------------------------------------------------------------------------
// Variáveis globais
//--------------------------------------------------------------------------
HWND g_hDlg = NULL;
HINSTANCE hInstance = NULL;
char strFrase[255] = { 0 };
char modal = '\0';

//--------------------------------------------------------------------------
// Protótipo das funções
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

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
  wcl.lpszMenuName = MAKEINTRESOURCE(IDM_MEUMENU); // Carrega menu do recurso
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
			::hInstance = hInstance;

      // Mostra a janela
      ShowWindow(hWnd, nCmdShow);
      UpdateWindow(hWnd);

			// Armazena dados da mensagem que será obtida
      MSG msg;

      // Loop de mensagens, enquanto mensagem não for WM_QUIT,
      // obtém mensagem da fila de mensagens
      while(GetMessage(&msg, NULL, 0, 0) > 0)
      {
				// Verifica se a caixa de diálogo modeless foi criada ou
				// se a mensagem é uma mensagem para a caixa de diálogo
				if((!IsWindow(g_hDlg)) || (!IsDialogMessage(g_hDlg, &msg)))
				{
					// Traduz teclas virtuais ou aceleradoras (de atalho)
					TranslateMessage(&msg);

					// Envia mensagem para a função que processa mensagens (WindowProc)
					DispatchMessage(&msg);
				}
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
				case IDM_CAIXA_ABRIR_MODAL:
				{
					modal = 's';
					// DialogBox() / EndDialog() cria / destrói uma caixa de diálogo Modal
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_MEUDIALOGO), hWnd, (DLGPROC)DialogProc);
				} break;

				case IDM_CAIXA_ABRIR_MODELESS:
				{
					modal = 'n';
					// Verifica se caixa de diálogo já foi criada
					if(!IsWindow(g_hDlg))
					{
						// CreateDialog() / DestroyWindow() cria / destrói uma caixa de diálogo Modeless
						g_hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MEUDIALOGO), hWnd, (DLGPROC)DialogProc);
						ShowWindow(g_hDlg, SW_SHOW);
					}
					else
						SetFocus(g_hDlg);
				} break;

				case IDM_CAIXA_SAIR:
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
    } break;

  }
}

//--------------------------------------------------------------------------
// DialogProc() -> Processa as mensagens enviadas para a caixa de diálogo
//--------------------------------------------------------------------------
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static int checado;

  // Verifica qual foi a mensagem enviada
  switch(uMsg)
  {

    case WM_INITDIALOG: // Caixa de diálogo foi criada
		{
			checado = IsDlgButtonChecked(hDlg, IDC_CHECKBOX1);

      // Retorna TRUE, significando que a mensagem foi processada corretamente
			return(TRUE);
		} break;

		case WM_COMMAND: // Item do menu, tecla de atalho ou controle ativado
		{
			switch(LOWORD(wParam))
			{
		    case IDCANCEL:
				{
					// Verifica se checkbox está checado
					if(checado == BST_CHECKED)
					{
						// Copia mensagem do textbox e mostra mensagem na tela
						GetDlgItemText(hDlg, IDC_EDIT1, strFrase, 255);
						MessageBox(g_hDlg, strFrase, "Mensagem:", MB_OK);
					}

					// Destrói a caixa de diálogo
					if(modal == 's')
						EndDialog(hDlg, wParam);
					else
					{
						DestroyWindow(hDlg);
						g_hDlg = NULL;
					}

					return(TRUE);
				} break;

				case IDC_BOTAO1:
				{
					// Verifica se checkbox está checado
					if(checado == BST_CHECKED)
					{
						// Copia mensagem do textbox e mostra mensagem na tela
						GetDlgItemText(hDlg, IDC_EDIT1, strFrase, 255);
						MessageBox(g_hDlg, strFrase, "Mensagem:", MB_OK);
					}
	      
					// Destrói a caixa de diálogo
					if(modal == 's')
						EndDialog(hDlg, wParam);
					else
					{
						DestroyWindow(hDlg);
						g_hDlg = NULL;
					}
		
					return(TRUE);
				} break;

				case IDC_CHECKBOX1:
				{
					// Muda status do checkbox
					checado = IsDlgButtonChecked(hDlg, IDC_CHECKBOX1);
				  CheckDlgButton(hDlg, IDC_CHECKBOX1, checado);
				} break;
		  }
		
			return(TRUE);
		} break;

		default: // Outra mensagem
		{
			// Retorna FALSE, deixando o Windows processar a mensagem
			return(FALSE);
		} break;

  }
}