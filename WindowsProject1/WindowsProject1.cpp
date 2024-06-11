// WindowsProject1.cpp : Defines the entry point for the application.
//
#include "framework.h";
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#ifndef  M_PI
#define  M_PI  3.1415926535897932384626433
#endif

#include "WindowsProject1.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void rect(HDC hdc, int sx, int sy, int lx, int ly) {
    
    MoveToEx(hdc, sx, sy, NULL);
    LineTo(hdc, sx + lx, sy);
    LineTo(hdc, sx + lx, sy + ly);
    LineTo(hdc, sx, sy + ly);
    LineTo(hdc, sx, sy);




}


typedef struct {
    float x;
    float y;
} myCoord;

typedef struct {
    float x;
    float y;
} myVector;

typedef struct {
    myCoord c1;
    myCoord c2;
} myLine;



// determines intersection point given 2 lines

// right now does not handle no intersection

static bool intersect(myLine l1, myLine l2, myCoord *inter) {
    float dx1 = l1.c2.x - l1.c1.x;
    float dx2 = l2.c2.x - l2.c1.x;
    float dy1 = l1.c2.y - l1.c1.y;
    float dy2 = l2.c2.y - l2.c1.y;
    float t1;
    float t2;

    t1 = (dx2 * (l2.c1.y - l1.c1.y) + dy2 * (l1.c1.x - l2.c1.x)) / (dx2 * dy1 - dy2 * dx1);
    t2 = (dx1 * (l1.c1.y - l2.c1.y) + dy1 * (l2.c1.x - l1.c1.x)) / (dx1 * dy2 - dy1 * dx2);
    /*
    if (l1.c1.x + t1 * dx1 == l2.c1.x + t2 * dx2 && l1.c1.y + t1 * dy1 == l2.c1.y + t2 * dy2) {
        inter->x = l1.c1.x + t1 * dx1;
        inter->y = l1.c1.y + t1 * dy1;
        return true;
    }
    */
    
    if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
        inter->x = l1.c1.x + t1 * dx1;
        inter->y = l1.c1.y + t1 * dy1;
        return true;
    }
    

    

    return false;
    

}


myCoord elemList[10];



// returns perpendicular line to camera
// assumes camera is stuck at (0,0)
// to fix camera, + 0 to + x or y of initial position
// view is a unit vector
// vpl is view port length, how wide the perspective is
// cVal is far away the normal line is from the camera, strength of perspective
myLine drawNormal(float vpl, int cVal, myVector view) {
    myCoord midpoint;

    midpoint.x = cVal * view.x + 0;
    midpoint.y = cVal * view.y + 0;

    float slope = (-view.x) / view.y;

    float xDist = (vpl / 2) / (float) sqrt(1 + slope * slope);

    float yDist = slope * xDist;

    myCoord c1;
    myCoord c2;
    myLine n;

    c1.x = midpoint.x - xDist;
    c1.y = midpoint.y - yDist;
    c2.x = midpoint.x + xDist;
    c2.y = midpoint.y + yDist;

    n.c1 = c1;
    n.c2 = c2;

    return n;
}

// converts imaginary coordinate system into api top left system
// origin is in top left format, loc is in calculation format
myCoord convertToScreen(myCoord origin, myCoord loc) {
    myCoord c;
    c.x = origin.x + loc.x;
    c.y = origin.y - loc.y;
    return c;
}
// converts screen system to imaginary system
// origin is in top left format, loc is in top left format
myCoord convertToCalc(myCoord origin, myCoord loc) {
    myCoord c;
    c.x = loc.x - origin.x;
    c.y = origin.y - loc.y;
    return c;
}

float angle = M_PI / 2;
myVector view;
void convertAngleToVec(float angle, myVector *vec) {
    float x = (float)cos(angle);
    float y = (float)sin(angle);
    vec->x = x;
    vec->y = y;
}




int test = 500;








//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    {
        switch (wParam) {
        case VK_RIGHT:
        {
            test = test + 10;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case VK_LEFT:
        {
            test = test - 10;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case 0x44:
        {
            angle = angle - 0.25;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case 0x41:
        {
            angle = angle + 0.25;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case 0x51:
        {
            SetWindowPos(hWnd, 0, 0, 0, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN), NULL);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        
        default: 
            break;
        
        }
        
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            /*
            // TODO: Add any drawing code that uses hdc here...
           
            //HPEN hPenOld;

            // Draw a red line
            //HPEN hLinePen;
            
            //hLinePen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
            //hPenOld = (HPEN)SelectObject(hdc, hLinePen);

            DRAW HERE**

            //SelectObject(hdc, hPenOld);
            //DeleteObject(hLinePen);

            */

            /*
            POINT p;
            LPPOINT lp = (LPPOINT) &p;
            int maxlen = 100;
            char* s = (char*)malloc(maxlen);
            int slen;

            MoveToEx(hdc, 100, 100, lp);
            LineTo(hdc, 500, 250);

            slen = sprintf_s((char*) s, maxlen,  "%d %d", lp->x, lp->y);
            TextOutA(hdc, 200, 200, (LPCSTR)s, slen);

            MoveToEx(hdc, 100, 100, lp);

            slen = sprintf_s((char*)s, maxlen, "%d %d", lp->x, lp->y);
            TextOutA(hdc, 200, 250, (LPCSTR)s, slen);


            free(s);
            */                                      
            




            //rect(hdc, 100, 100, 200, 100);


            
            
            

            HPEN hPenOld;

            HPEN hLinePen;

            hLinePen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
            hPenOld = (HPEN)SelectObject(hdc, hLinePen);

            convertAngleToVec(angle, &view);

            


            myCoord camera;
            camera.x = 200;
            camera.y = 200;



            //camera
            rect(hdc, camera.x, camera.y, 2, 2);
            SelectObject(hdc, hPenOld);
            DeleteObject(hLinePen);

            hLinePen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
            hPenOld = (HPEN)SelectObject(hdc, hLinePen);


            myLine norm = drawNormal(100, 50, view);
            MoveToEx(hdc, convertToScreen(camera, norm.c1).x, convertToScreen(camera, norm.c1).y, NULL);
            LineTo(hdc, convertToScreen(camera, norm.c2).x, convertToScreen(camera, norm.c2).y);

            SelectObject(hdc, hPenOld);
            DeleteObject(hLinePen);

            hLinePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
            hPenOld = (HPEN)SelectObject(hdc, hLinePen);




            myCoord point;
            point.x = test;
            point.y = 100;
            rect(hdc, test, 100, 10, 10);


            MoveToEx(hdc, point.x, point.y, NULL);
            LineTo(hdc, camera.x, camera.y);

            myLine l;
            l.c1 = convertToCalc(camera, { point.x, point.y });
            l.c2 = convertToCalc(camera, camera);

            myCoord inter;
            if (intersect(norm, l, &inter)) {
                rect(hdc, convertToScreen(camera, inter).x, convertToScreen(camera, inter).y, 2, 2);
            }
            else {
                Arc(hdc, 100, 100, 200, 200, 0, 0, 0, 0);
            }

            SelectObject(hdc, hPenOld);
            DeleteObject(hLinePen);


            

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
