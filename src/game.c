#include <windows.h>
#include <wingdi.h>
#include <stdio.h>

#define BACKGROUND RGB(0, 28, 28)
#define NUMBEROFBUTTONS 7

// Declare Windows procedure
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

  HWND hwnd;         // This is the handle for our window
HDC hdc;
PAINTSTRUCT ps;
COLORREF fontColor = BACKGROUND;
char text[3] = "   ",
     timer[3] = "   ";
int pressed[NUMBEROFBUTTONS],
    locked,
    xOffset,
    yOffset,
    t_xOffset,
    t_yOffset,
    numberOfButtonsPressed,
    served,
    timer_running = 0;

// Make the class name into a global variable
char szClassName[ ] = "WindowsApp";

// Display a letter on the screen.
//
void showButton(void) {
  SetTextColor(hdc, fontColor);
  TextOut(hdc, xOffset, yOffset, text, 3);
  SetTextColor(hdc, RGB(255, 255, 255));
  TextOut(hdc, t_xOffset, t_yOffset, timer, 3);
}//showButton

// Display the timer on the screen.
//
DWORD WINAPI runTimer(LPVOID args) {
  int i;

  if (timer_running)
    return 0;

  timer_running = 1;

  for (i = 5; i >= 0; i--) {
    timer[0] = '0' + i;
    showButton();
    RedrawWindow(hwnd , NULL , NULL , RDW_INVALIDATE);
    Sleep(1000);
  }//for

  timer_running = 0;

  return 0;
}//runTimer

// Reset the buttons.
//
void reset(void) {
  int i;

  while (timer_running)
    Sleep(1000);

  for (i = 0; i < NUMBEROFBUTTONS; i++)
    pressed[i] = 0;

  numberOfButtonsPressed = 0;
  served = 0;
  text[0] = ' ';
  timer[0] = ' ';
  locked = 0;
  showButton();
}//reset

// A button was pressed.
//
// Arguments:
//   int number ; Number of the pressed button.
//
void pressButton(int number) {
  if (pressed[number])
    return;

  numberOfButtonsPressed++;
  pressed[number] = numberOfButtonsPressed;
}//pressButton

// Look who's turn it is.
//
void serve(void) {
  int i;

  if (served == numberOfButtonsPressed)
    return;

  served++;
  for (i = 0; i < NUMBEROFBUTTONS; i++)
    if (pressed[i] == served) {
      switch (i) { // Now look at the rest of the keystrokes.
        case 0:                           // `z' pressed.
          fontColor = RGB(255, 0, 255);   // Cyan.
          text[0] = 'A';
          break;
        case 1:                           // `x' pressed.
          fontColor = RGB(0, 255, 0);     // Green.
          text[0] = 'B';
          break;
        case 2:                           // `c' pressed.
          fontColor = RGB(0, 255, 255);   // Magenta.
          text[0] = 'C';
          break;
        case 3:                           // `v' pressed.
          fontColor = RGB(255, 255, 0);   // Yellow.
          text[0] = 'D';
          break;
        case 4:                           // `b' pressed.
          fontColor = RGB(0, 0, 255);     // Blue.
          text[0] = 'E';
          break;
        case 5:                           // `n' pressed.
          fontColor = RGB(255, 0, 0);     // Red.
          text[0] = 'F';
          break;
        case 6:                           // `m' pressed.
          fontColor = RGB(255, 255, 255); // White.
          text[0] = 'G';
          break;
      }//switch
      locked = 1;
      CreateThread(NULL, 0, runTimer, NULL, 0, 0);

      return;
    }//if
}//serve

// This function is called by the Windows function DispatchMessage()
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, 
                                 LPARAM lParam) {
  switch (message) {      // handle the messages
    case WM_MOUSEMOVE :
      showButton();
      break;
    case WM_KEYDOWN:          // Keystroke.
      switch (wParam) {       // First look at the quit and reset keys.
        case 0x51:            // `q' pressed.
          PostQuitMessage(0); // Exit.
          break;
        case 0x52:            // `r' pressed.
          reset();            // Reset.
          break;
        case 0x53:            // `s' pressed.
          if (!timer_running)
            locked = 0;       // Serve the next person.
          break;
        case 0x5a:            // `z' pressed.
          pressButton(0);     // Button.
          break;
        case 0x58:            // `x' pressed.
          pressButton(1);     // Button.
          break;
        case 0x43:            // `c' pressed.
          pressButton(2);     // Button.
          break;
        case 0x56:            // `v' pressed.
          pressButton(3);     // Button.
          break;
        case 0x42:            // `b' pressed.
          pressButton(4);     // Button.
          break;
        case 0x4e:            // `n' pressed.
          pressButton(5);     // Button.
          break;
        case 0x4d:            // `m' pressed.
          pressButton(6);     // Button.
          break;
      }//switch
      if (!locked)
        serve();
      break;
    default: // for messages that we don't deal with
      return DefWindowProc(hwnd, message, wParam, lParam);
  }//switch

  return 0;
}//WindowProcedure

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszArgument, int nFunsterStil) {
  MSG messages;      // Here messages to the application are saved
  WNDCLASSEX wincl;  // Data structure for the windowclass
  static HFONT font; // temp big font
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

  t_xOffset = (xSize / 4) - (1.0 * fontW);
  t_yOffset = (ySize / 7) - (0.5 * fontH);

  reset();

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
