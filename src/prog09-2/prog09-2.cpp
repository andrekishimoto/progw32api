//--------------------------------------------------------------------------
// Programação Windows: C e Win32 API com ênfase em Multimídia
// Autor: André Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, André Kishimoto
//
// prog09-2.cpp – Registro do Windows
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>
#include "prog09-2-res.h"

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
// Nome da classe da janela
#define WINDOW_CLASS		"prog09-2"
// Título da janela
#define WINDOW_TITLE		"Prog 09-2"
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
      PostQuitMessage(WM_QUIT);

      return(0);
    } break;

		case WM_COMMAND: // Item do menu, tecla de atalho ou controle ativado
		{
			switch(LOWORD(wParam))
			{
				case IDM_LER:
				{
					HKEY hKey = NULL; // Identificador do registro aberto (incluindo subchaves)
					LONG lResultado = 0; // Verifica se ação com o registro foi bem sucedida

					// Abre o registro para leitura, a partir da chave-raiz HKEY_LOCAL_MACHINE
					// e na subchave SOFTWARE\___prog09-2
					// Salvando o identificador desse local em hKey
					lResultado = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\___prog09-2", 0, KEY_READ, &hKey);

					// Registro aberto com sucesso
					if(lResultado == ERROR_SUCCESS)
					{
						// Obtém o valor "(Padrão)" da subchave
						// PS: Padrão no caso é uma string NULL, ou seja, "")
						// Salva o valor em byteValor e a quantidade de caracteres lidos em dwTamanho
						BYTE byteValor[255];
						DWORD dwTamanho;
						RegQueryValueEx(hKey, "", NULL, NULL, byteValor, &dwTamanho);

						// Mostra valor obtido na tela
						MessageBox(hWnd, (LPCSTR)byteValor, "Valor obtido no registro:", MB_ICONINFORMATION | MB_OK);

						// Fecha o registro, usando o identificador hKey
						RegCloseKey(hKey);
					}
					else // Registro não pôde ser aberto
						MessageBox(hWnd, "Erro ao tentar abrir chave do registro.", "Erro!", MB_ICONERROR | MB_OK);
				} break;

				case IDM_CRIAR:
				{
					HKEY hKey = NULL; // Identificador do registro aberto (incluindo subchaves)
					LONG lResultado = 0; // Verifica se ação com o registro foi bem sucedida

					// Cria uma subchave no registro para escrita, a partir da chave-raiz
					// HKEY_LOCAL_MACHINE, na subchave SOFTWARE
					// Salvando o identificador do novo local (HKEY_LOCAL_MACHINE\SOFTWARE\___prog09-2) em hKey
					// Em dwAcao, é armazenado se a subchave indicada já existia ou não
					DWORD dwAcao;
					lResultado = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\___prog09-2", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwAcao);

					// Ação no registro realizada com sucesso
					if(lResultado == ERROR_SUCCESS)
					{
						// Subchave não existia, criou uma nova
						if(dwAcao == REG_CREATED_NEW_KEY)
							MessageBox(hWnd, "Uma nova chave foi criada.", "Aviso!", MB_ICONINFORMATION | MB_OK);
						// Subchave já existia, apenas abriu a mesma
						if(dwAcao == REG_OPENED_EXISTING_KEY)
							MessageBox(hWnd, "Chave já existente, foi aberta.", "Aviso!", MB_ICONINFORMATION | MB_OK);

						// Muda o valor "(Padrão)" da subchave, sendo do tipo REG_SZ (string terminada com \0)
						lResultado = RegSetValueEx(hKey, "", 0, REG_SZ, (BYTE *)"{Adicionado por prog09-2}\0", 26);
						if(lResultado == ERROR_SUCCESS)
							MessageBox(hWnd, "Valor \"(Padrão)\" foi modificado.", "Aviso!", MB_ICONINFORMATION | MB_OK);
						else
              MessageBox(hWnd, "Erro ao modificar valor \"(Padrão)\".", "Erro!", MB_ICONERROR | MB_OK);

						// Cria um novo valor, "valor1" dentro da subchave HKEY_LOCAL_MACHINE\SOFTWARE\___prog09-2
						// O novo valor é do tipo número de 32-bit (REG_DWORD)
						// Note que se o valor "valor1" já existisse, o comando abaixo iria apenas
						// modificar seu valor, como feito na chamada à função acima (que modificou o valor "(Padrão)")
						DWORD dwValor = 2005;
						lResultado = RegSetValueEx(hKey, "valor1", 0, REG_DWORD, (BYTE *)&dwValor, sizeof(DWORD));
						if(lResultado == ERROR_SUCCESS)
							MessageBox(hWnd, "Valor \"valor1\" foi adicionado/modificado.", "Aviso!", MB_ICONINFORMATION | MB_OK);
						else
							MessageBox(hWnd, "Erro ao adicionar/modificar valor \"valor1\".", "Erro!", MB_ICONERROR | MB_OK);

						// Fecha o registro, usando o identificador hKey
						RegCloseKey(hKey);
					}
					else // Chave no registro não pôde ser criado
						MessageBox(hWnd, "Erro ao tentar criar chave no registro.", "Erro!", MB_ICONERROR | MB_OK);
				} break;

				case IDM_EXCLUIR:
				{
					HKEY hKey = NULL; // Identificador do registro aberto (incluindo subchaves)
					LONG lResultado = 0; // Verifica se ação com o registro foi bem sucedida

					// Abre o registro para escrita, a partir da chave-raiz HKEY_LOCAL_MACHINE
					// e na subchave SOFTWARE\___prog09-2
					// Salvando o identificador desse local em hKey
					lResultado = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\___prog09-2", 0, KEY_WRITE, &hKey);

					// Registro aberto com sucesso
					if(lResultado == ERROR_SUCCESS)
					{
						// Exclui o valor "valor1" e verifica se foi excluído ou não
						lResultado = RegDeleteValue(hKey, "valor1");
						if(lResultado == ERROR_SUCCESS)
							MessageBox(hWnd, "Valor \"valor1\" foi excluído.", "Aviso!", MB_ICONINFORMATION | MB_OK);
						else
							MessageBox(hWnd, "Erro ao excluir valor \"valor1\".", "Erro!", MB_ICONERROR | MB_OK);

						// Exclui a subchave HKEY_LOCAL_MACHINE\SOFTWARE\___prog09-2 e verifica se foi excluída ou não
						lResultado = RegDeleteKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\___prog09-2");
						if(lResultado == ERROR_SUCCESS)
							MessageBox(hWnd, "Subchave HKEY_LOCAL_MACHINE\\SOFTWARE\\___prog09-2 foi excluída.", "Aviso!", MB_ICONINFORMATION | MB_OK);
						else
							MessageBox(hWnd, "Erro ao excluir subchave HKEY_LOCAL_MACHINE\\SOFTWARE\\___prog09-2.", "Erro!", MB_ICONERROR | MB_OK);

						// Fecha o registro, usando o identificador hKey
						RegCloseKey(hKey);
					}
					else // Registro não pôde ser aberto
						MessageBox(hWnd, "Erro ao tentar abrir chave do registro.", "Erro!", MB_ICONERROR | MB_OK);
				} break;

				case IDM_SAIR:
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