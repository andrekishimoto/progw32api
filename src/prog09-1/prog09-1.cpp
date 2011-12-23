//--------------------------------------------------------------------------
// Programação Windows: C e Win32 API com ênfase em Multimídia
// Autor: André Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, André Kishimoto
//
// prog09-1.cpp – Arquivos
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>
#include "prog09-1-res.h"

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
// Nome da classe da janela
#define WINDOW_CLASS		"prog09-1"
// Título da janela
#define WINDOW_TITLE		"Prog 09-1"
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
  wcl.lpszMenuName = MAKEINTRESOURCE(IDM_MEUMENU);
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

	// Identificador do arquivo
	static HANDLE hFile = NULL;

	// Identificador da caixa de texto e do botão
	static HWND hWndTexto = NULL;
	static HWND hWndBotao = NULL;

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
			// Se arquivo está aberto, fecha antes de sair
			if(hFile)
				CloseHandle(hFile);

			// Destrói caixa de texto e botão
			if(hWndTexto)
				DestroyWindow(hWndTexto);
			if(hWndBotao)
				DestroyWindow(hWndBotao);

      // Envia mensagem WM_QUIT para o loop de mensagens
      PostQuitMessage(WM_QUIT);

      return(0);
    } break;

		case WM_SIZE:
		{
			// Muda tamanho da caixa de texto conforme o tamanho da área cliente
			MoveWindow(hWndTexto, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);

			return(0);
		} break;

		case WM_COMMAND: // Item do menu, tecla de atalho ou controle ativado
		{
			// Verifica bit menos significativo de wParam (ID’s)
			switch(LOWORD(wParam))
			{
				case IDM_ABRIR:
				{
					// Se arquivo ainda não foi aberto
					if(hFile == NULL)
					{
						// Abre/cria arquivo
						hFile = CreateFile("dados.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					
						// Verifica se identificador do arquivo é válido
						if(hFile == INVALID_HANDLE_VALUE)
							MessageBox(hWnd, "Erro ao abrir/criar arquivo.", "Erro!", MB_ICONERROR | MB_OK);
						else // Arquivo é válido
						{
							MessageBox(hWnd, "Arquivo aberto/criado.", "Aviso!", MB_ICONINFORMATION | MB_OK);
							SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"Arquivo aberto/criado.");

							// Cria botões para usuário selecionar opção
							hWndBotao = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", "Clique aqui!", WS_CHILD | WS_VISIBLE, 10, 10, 100, 30, hWnd, (HMENU)IDC_BOTAO, NULL, NULL);
						}
					}
				} break;

				case IDM_SALVAR:
				{
					if(hFile != INVALID_HANDLE_VALUE) // Arquivo está aberto
					{
						// Obtém quantidade de caracteres da caixa de texto
						int tam = GetWindowTextLength(hWndTexto);

						// Obtém conteúdo da caixa de texto, alocando memória para os dados
						// (alocação no modo Windows)
						LPSTR lpstrBuffer = (LPSTR)GlobalAlloc(GPTR, tam + 1);
						GetWindowText(hWndTexto, lpstrBuffer, tam + 1);

						// Armazena quantos bytes foram escritos no arquivo
						DWORD dwBytesEscritos;

						// Modifica posição do arquivo para o início
						SetFilePointer(hFile, NULL, NULL, FILE_BEGIN);

						// Grava conteúdo da caixa de texto no arquivo
						WriteFile(hFile, lpstrBuffer, tam, &dwBytesEscritos, NULL);

						// Define fim do arquivo
						SetEndOfFile(hFile);

						// Libera memória dos dados
						GlobalFree(lpstrBuffer);

						MessageBox(hWnd, "Conteúdo escrito no arquivo.", "Aviso!", MB_ICONINFORMATION | MB_OK);
					}
					else // Arquivo não foi aberto
						MessageBox(hWnd, "Erro ao escrever no arquivo.", "Erro!", MB_ICONERROR | MB_OK);
				} break;

				case IDM_FECHAR:
				{
					// Se o arquivo está aberto, fecha
					if(hFile)
					{
						CloseHandle(hFile);
						hFile = NULL;
						MessageBox(hWnd, "Arquivo fechado.", "Aviso", MB_ICONINFORMATION | MB_OK);
					}

					// Destrói caixa de texto e botão
					if(hWndTexto)
					{
						DestroyWindow(hWndTexto);
						hWndTexto = NULL;
					}
					if(hWndBotao)
					{
						DestroyWindow(hWndBotao);
						hWndBotao = NULL;
					}
				} break;

				case IDM_EXCLUIR:
				{
					// Tenta excluir arquivo e mostra resultado para usuário
					if(DeleteFile("dados.txt"))
						MessageBox(hWnd, "Arquivo excluído.", "Aviso!", MB_ICONINFORMATION | MB_OK);
					else
						MessageBox(hWnd, "Arquivo não pode ser excluído.", "Erro!", MB_ICONERROR | MB_OK);
				} break;

				case IDM_SAIR:
				{
					// Destrói a janela
		      DestroyWindow(hWnd);
				} break;

				case IDC_BOTAO:
				{
					// Destrói botão
					if(hWndBotao)
					{
						DestroyWindow(hWndBotao);
						hWndBotao = NULL;
					}

					// Cria caixa de texto para o usuário entrar com os dados
					hWndTexto = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL,
											0, 0, 312, 194, hWnd, NULL, NULL, NULL);

					if(hFile != INVALID_HANDLE_VALUE) // Arquivo está aberto
					{
						// Obtém tamanho do arquivo
						int tam = GetFileSize(hFile, NULL);

						// Obtém conteúdo do arquivo, alocando memória para os dados
						// (alocação no modo Windows)
						LPSTR lpstrBuffer = (LPSTR)GlobalAlloc(GPTR, tam + 1);

						// Armazena quantos bytes foram escritos no arquivo
						DWORD dwBytesEscritos;

						// Lê conteúdo do arquivo
						ReadFile(hFile, lpstrBuffer, tam, &dwBytesEscritos, NULL);

						// Coloca conteúdo do arquivo na caixa de texto
						SetWindowText(hWndTexto, lpstrBuffer);

						// Libera memória dos dados
						GlobalFree(lpstrBuffer);

						MessageBox(hWnd, "Conteúdo lido do arquivo.", "Aviso!", MB_ICONINFORMATION | MB_OK);

						// Define cursor na caixa de texto
						SetFocus(hWndTexto);
					}
					else // Arquivo não foi aberto
						MessageBox(hWnd, "Erro ao ler o arquivo.", "Erro!", MB_ICONERROR | MB_OK);
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