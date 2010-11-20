#include <windows.h>
#include <wingdi.h>

#define BACKGROUND RGB(0, 28, 28)

/* Declare Windows procedure */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HDC hdc;
PAINTSTRUCT ps;
COLORREF fontColor = BACKGROUND;
char text[3] = "   ";
int locked = 0,
    xOffset,
    yOffset;

/* Make the class name into a global variable */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszArgument, int nFunsterStil) {
  HWND hwnd;        // This is the handle for our window
  MSG messages;     // Here messages to the application are saved
  WNDCLASSEX wincl; // Data structure for the windowclass
  static HFONT font;           /* temp big font */    
  int xSize,
      ySize,
      fontH = 300,
      fontW = 200;

  // The Window structure
  wincl.hInstance = hThisInstance;
  wincl.lpszClassName = szClassName;
  wincl.lpfnWndProc = WindowProcedure; // This function is called by windows
  wincl.style = CS_DBLCLKS;            // Catch double-clicks
  wincl.cbSize = sizeof (WNDCLASSEX);

  // Use default icon and mouse-pointer
  wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
  wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
  wincl.lpszMenuName = NULL; // No menu
  wincl.cbClsExtra = 0;      // No extra bytes after the window class
  wincl.cbWndExtra = 0;      // structure or the window instance

  // Use Windows's default color as the background of the window
  wincl.hbrBackground = (HBRUSH) CreateSolidBrush(BACKGROUND);

  // Register the window class, and if it fails quit the program
  if (!RegisterClassEx (&wincl))
    return 0;

  xSize = GetSystemMetrics(SM_CXSCREEN);
  ySize = GetSystemMetrics(SM_CYSCREEN);

  xOffset = (xSize / 2) - (1.0 * fontW);
  yOffset = (ySize / 2) - (0.5 * fontH);
  // The class is registered, let's create the program
  hwnd = CreateWindowEx(
    0,                        // Extended possibilites for variation
    szClassName,              // Classname
    "Windows App",            // Title Text
    WS_EX_TOPMOST | WS_POPUP, // fullscreen window
    CW_USEDEFAULT,            // Windows decides the position
    CW_USEDEFAULT,            // where the window ends up on the screen
    xSize,                    // The programs width
    ySize,                    // and height in pixels
    HWND_DESKTOP,             // The window is a child-window to desktop
    NULL,                     // No menu
    hThisInstance,            // Program Instance handler
    NULL);                    // No Window Creation data

  // Make the window visible on the screen
  ShowWindow(hwnd, nFunsterStil);

  hdc = BeginPaint(hwnd, &ps);
  SetBkColor(hdc, BACKGROUND);
  font = CreateFont(fontH, fontW, 0, 0,
      FW_NORMAL, FALSE, FALSE, FALSE,
      ANSI_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
      DEFAULT_PITCH | FF_ROMAN,
      "Times New Roman");

  SelectObject(hdc, font);
  //SetBkMode(hdc, TRANSPARENT);

  // Run the message loop. It will run until GetMessage() returns 0
  while (GetMessage (&messages, NULL, 0, 0)) {
    // Translate virtual-key messages into character messages
    TranslateMessage(&messages);
    // Send message to WindowProcedure
    DispatchMessage(&messages);
  }//while

  // The program return-value is 0 - The value that PostQuitMessage() gave
  return messages.wParam;
}//WinMain

/* This function is called by the Windows function DispatchMessage() */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, 
                                 LPARAM lParam) {
  switch (message) {      // handle the messages
    case WM_MOUSEMOVE :
      SetTextColor(hdc, fontColor);
      TextOut(hdc, xOffset, yOffset, text, 3);
      break;
    case WM_KEYDOWN:    // Keystroke.
      switch (wParam) { // First look at the quit and reset keys.
        case 0x51:      // `q' pressed.
          PostQuitMessage(0);
          break;
        case 0x52:      // `r' pressed.
          text[0] = ' ';
          locked = 0;
          break;
      }//switch

      if (locked)
        break;

      switch (wParam) { // Now look at the rest of the keystrokes.
        case 0x5a:                        // `z' pressed.
          fontColor = RGB(255, 0, 255);   // Cyan.
          text[0] = 'A';
          locked = 1;
          break;
        case 0x58:                        // `x' pressed.
          fontColor = RGB(0, 255, 0);     // Green.
          text[0] = 'B';
          locked = 1;
          break;
        case 0x43:                        // `c' pressed.
          fontColor = RGB(0, 255, 255);   // Magenta.
          text[0] = 'C';
          locked = 1;
          break;
        case 0x56:                        // `v' pressed.
          fontColor = RGB(255, 255, 0);   // Yellow.
          text[0] = 'D';
          locked = 1;
          break;
        case 0x42:                        // `b' pressed.
          fontColor = RGB(0, 0, 255);     // Blue.
          text[0] = 'E';
          locked = 1;
          break;
        case 0x4e:                        // `n' pressed.
          fontColor = RGB(255, 0, 0);     // Red.
          text[0] = 'F';
          locked = 1;
          break;
        case 0x4d:                        // `m' pressed.
          fontColor = RGB(255, 255, 255); // Black.
          text[0] = 'G';
          locked = 1;
          break;
      }//switch
      SetTextColor(hdc, fontColor);
      TextOut(hdc, xOffset, yOffset, text, 3);
      break;
    default: // for messages that we don't deal with
      return DefWindowProc(hwnd, message, wParam, lParam);
  }//switch

  return 0;
}//WindowProcedure
