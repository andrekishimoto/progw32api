//--------------------------------------------------------------------------
// Programação Windows: C e Win32 API com ênfase em Multimídia
// Autor: André Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, André Kishimoto
//
// prog08-1.cpp – Reproduzindo sons wave e MIDI
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>

// Inclui a biblioteca "winmm.lib" ao projeto
#pragma comment(lib, "winmm.lib")

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
// Nome da classe da janela
#define WINDOW_CLASS		"prog08-1"
// Título da janela
#define WINDOW_TITLE		"Prog 08-1"
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

	// Identificadores dos dispositivos MCI
	static MCIDEVICEID mciDeviceID1 = NULL;
	static MCIDEVICEID mciDeviceID2 = NULL;
	static MCIDEVICEID mciDeviceID3 = NULL;

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
			// Fecha todos os dispositivos MCI
			mciSendCommand(mciDeviceID1, MCI_STOP, NULL, NULL);
			mciSendCommand(mciDeviceID1, MCI_CLOSE, NULL, NULL);
			mciSendCommand(mciDeviceID2, MCI_STOP, NULL, NULL);
			mciSendCommand(mciDeviceID2, MCI_CLOSE, NULL, NULL);
			mciSendCommand(mciDeviceID3, MCI_STOP, NULL, NULL);
			mciSendCommand(mciDeviceID3, MCI_CLOSE, NULL, NULL);

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

		case MM_MCINOTIFY: // Notificação de um dispositivo MCI
		{
			if(lParam == mciDeviceID1)
			{
				// Mostra mensagem na tela e volta posição do dispositivo mciDeviceID1 ao início
				MessageBox(hWnd, "Fim do som wave (F3).", "Aviso", MB_OK);
				mciSendCommand(mciDeviceID1, MCI_SEEK, MCI_SEEK_TO_START, NULL);
			}

			return(0);
		} break;

    case WM_KEYDOWN: // Obtém tecla virtual
    {
			switch(wParam)
			{
				case VK_F1: // Toca som wave com PlaySound()
				{
					// Muda título do programa
	        SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"PlaySound()");

					// Reproduz som do arquivo "teste1.wav" com PlaySound()
					PlaySound("teste1.wav", NULL, SND_FILENAME | SND_ASYNC);
				} break;
				
				case VK_F2: // Toca som wave com string de comando via mciSendString()
				{
					// Muda título do programa
	        SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"mciSendString()");

					// Reproduz som do arquivo "teste1.wav" com string de comando
					mciSendString("play teste1.wav", NULL, 0, NULL);
				} break;

				case VK_F3: // Toca som wave com mensagem de comando via mciSendCommand()
				{
					// Muda título do programa
	        SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"mciSendCommand(), wave");

					// Reproduz som do arquivo "teste1.wav" com mensagem de comando
					if(mciDeviceID1 == NULL)
					{
						// Configura o dispositivo
						MCI_OPEN_PARMS mciOpenParms;
						mciOpenParms.lpstrDeviceType = "waveaudio";
						mciOpenParms.lpstrElementName = "teste1.wav";
						// Abre o dispositivo
						mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpenParms);
						// Obtém identificador do dispositivo
						mciDeviceID1 = mciOpenParms.wDeviceID;
					}

					// Volta posição do dispositivo para o início
					//MCI_SEEK_PARMS mciSeekParms;
					//mciSendCommand(mciDeviceID1, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)&mciSeekParms);

					// Reproduz som com envio da mensagem MM_MCINOTIFY para a função callback
					// hWnd, que irá processar a mensagem
					MCI_PLAY_PARMS mciPlayParms;
					mciPlayParms.dwCallback = (DWORD)hWnd;
					mciSendCommand(mciDeviceID1, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPlayParms);
				} break;

				case VK_F4: // Toca som wave com mensagem de comando via mciSendCommand()
				{
					// Muda título do programa
	        SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"mciSendCommand(), wave");

					// Reproduz som do arquivo "teste2.wav" com mensagem de comando
					if(mciDeviceID2 == NULL)
					{
						// Configura o dispositivo
						MCI_OPEN_PARMS mciOpenParms;
						mciOpenParms.lpstrDeviceType = "waveaudio";
						mciOpenParms.lpstrElementName = "teste2.wav";
						// Abre o dispositivo
						mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpenParms);
						// Obtém identificador do dispositivo
						mciDeviceID2 = mciOpenParms.wDeviceID;
					}

					// Volta posição do dispositivo para o início
					mciSendCommand(mciDeviceID2, MCI_SEEK, MCI_SEEK_TO_START, NULL);
					// Reproduz o dispositivo
					mciSendCommand(mciDeviceID2, MCI_PLAY, NULL, NULL);
				} break;
	
				case VK_F5: // Toca MIDI com mensagem de comando via mciSendCommand()
				{
					// Muda título do programa
	        SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"mciSendCommand(), MIDI");

					// Reproduz midi do arquivo "teste.mid" com mensagem de comando
					if(mciDeviceID3 == NULL)
					{
						// Configura o dispositivo
						MCI_OPEN_PARMS mciOpenParms;
						mciOpenParms.lpstrDeviceType = "sequencer";
						mciOpenParms.lpstrElementName = "teste.mid";
						// Abre o dispositivo
						mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpenParms);
						// Obtém identificador do dispositivo
						mciDeviceID3 = mciOpenParms.wDeviceID;
					}

					// Volta posição do dispositivo para o início
					mciSendCommand(mciDeviceID3, MCI_SEEK, MCI_SEEK_TO_START, NULL);
					// Reproduz o dispositivo
					mciSendCommand(mciDeviceID3, MCI_PLAY, NULL, NULL);
				} break;

				case VK_F6: // Fecha todos os dispositivos MCI
				{
					// Muda título do programa
	        SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"mciSendCommand(), pára reprodução");

					// Fecha todos os dispositivos MCI
					mciSendCommand(mciDeviceID1, MCI_STOP, NULL, NULL);
					mciSendCommand(mciDeviceID1, MCI_CLOSE, NULL, NULL);
					mciDeviceID1 = NULL;
					mciSendCommand(mciDeviceID2, MCI_STOP, NULL, NULL);
					mciSendCommand(mciDeviceID2, MCI_CLOSE, NULL, NULL);
					mciDeviceID2 = NULL;
					mciSendCommand(mciDeviceID3, MCI_STOP, NULL, NULL);
					mciSendCommand(mciDeviceID3, MCI_CLOSE, NULL, NULL);
					mciDeviceID3 = NULL;
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