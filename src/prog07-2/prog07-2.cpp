//--------------------------------------------------------------------------
// Programação Windows: C e Win32 API com ênfase em Multimídia
// Autor: André Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, André Kishimoto
//
// prog07-2.cpp - Janelas não-retangulares
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog07-2"
// Título da janela 
#define WINDOW_TITLE		"Prog 07-2"
// Largura da janela 
#define WINDOW_WIDTH		200
// Altura da janela 
#define WINDOW_HEIGHT		394

//--------------------------------------------------------------------------
// Protótipo das funções
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void WindowSkin(HWND, HDC, HBITMAP);

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
		   WS_POPUP | WS_VISIBLE, // Não utiliza barra de título no programa
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

	// Identificadores do bitmap e DC de memória
	static HBITMAP hBmp = NULL;
	static HDC hMemDC = NULL;
  
  // Verifica qual foi a mensagem enviada
  switch(uMsg)
  {
    case WM_CREATE: // Janela foi criada
    {
      // Cria DC de memória (hMemDC é global)
      hDC = GetDC(hWnd);
      hMemDC = CreateCompatibleDC(hDC);

			// Cria DIB Section, carregando bitmap do arquivo "skin.bmp"
			hBmp = (HBITMAP)LoadImage(NULL, "skin.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

			// Modifica janela do programa
			WindowSkin(hWnd, hMemDC, hBmp);

			// Libera DC de vídeo
			ReleaseDC(hWnd, hDC);

      // Retorna 0, significando que a mensagem foi processada corretamente
      return(0);
    } break;

    case WM_PAINT: // Janela (ou parte dela) precisa ser atualizada
    {
      // Obtém DC de vídeo
      hDC = BeginPaint(hWnd, &psPaint);

			// Faz transferência de bits entre os DC's de memória e vídeo
			BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hMemDC, 0, 0, SRCCOPY);

      // Libera DC de vídeo
      EndPaint(hWnd, &psPaint);

      return(0);
    } break;

		case WM_LBUTTONDOWN: // Botão esquerdo do mouse pressionado
		{
			// Se a posição y do cursor do mouse for menor que 100
			if(HIWORD(lParam) < 100)
				// Envia mensagem WM_NCLBUTTONDOWN, indicando que foi um clique na barra de título
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
			
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

//--------------------------------------------------------------------------
// WindowSkin() -> Cria janela no formato de uma figura
//--------------------------------------------------------------------------
void WindowSkin(HWND hWnd, HDC hMemDC, HBITMAP hBmp)
{
	// Cria região no formato da figura
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	HRGN hRgnTemp = hRgn;

	// Cor transparente
	COLORREF rgbTransparente = RGB(255, 0, 255);

	// Salva o primeiro ponto não-transparente da posição x da imagem
	int ix = 0;

	// Seleciona bitmap no DC de memória (configura DC de memória)
	SelectObject(hMemDC, hBmp);

	// Varre o bitmap para obter cores e o formato da figura
	for(int y = 0; y <= WINDOW_HEIGHT; y++)
	{
		for(int x = 0; x <= WINDOW_WIDTH; x++)
		{
			// Verifica se a cor do ponto atual é transparente
			if(GetPixel(hMemDC, x, y) != rgbTransparente)
			{
				// Se não for, salva o primeiro ponto não-transparente da posição x da imagem
				ix = x;

				// E incrementa a posição x até achar um ponto que é transparente
				// ou até chegar ao final da imagem (na horizontal)
				while((x <= WINDOW_WIDTH) && (GetPixel(hMemDC, x, y) != rgbTransparente))
					x++;

				// Depois cria uma região temporária onde:
				// x = primeiro ponto não-transparente até último ponto não-transparente
				// y = posição y atual até próximo y
				hRgnTemp = CreateRectRgn(ix, y, x, y + 1);

				// Combina a região atual (hRgn) com a região temporária (hRgnTemp)
				CombineRgn(hRgn, hRgn, hRgnTemp, RGN_OR);
			}
		}
	}

	// Depois que varreu todo o bitmap, verificando seu formato (de acordo com
	// as partes transparentes e não-transparentes da imagem), define o formato
	// da janela conforme a região atual (hRgn).
	SetWindowRgn(hWnd, hRgn, TRUE);
}