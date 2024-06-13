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

typedef struct {
    float x;
    float y;
    float z;
} my3DCoord;



myCoord getScreenCoords(my3DCoord origin, my3DCoord point, float cVal, myCoord center) {
    myCoord c;

    float ratio = cVal / (point.z - origin.z);

    c.x = center.x + ((point.x - origin.x) * ratio);
    c.y = center.y - ((point.y - origin.y) * ratio);


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




int xShift = 0;
int yShift = 0;
int zShift = 0;







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
            xShift += 5;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case VK_LEFT:
        {
            xShift -= 5;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case VK_UP:
        {
            zShift += 5;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case VK_DOWN:
        {
            zShift -= 5;
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
        case 0x57:
        {
            yShift += 5;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case 0x53:
        {
            yShift -= 5;
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

            HPEN hPenOld;
            HPEN hLinePen;

            hLinePen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
            hPenOld = (HPEN)SelectObject(hdc, hLinePen);


            RECT window;
            GetWindowRect(hWnd, &window);
            myCoord drawOrigin;
            drawOrigin.x = (window.right - window.left) / 2;
            drawOrigin.y = (window.bottom - window.top) / 2;
            rect(hdc, drawOrigin.x, drawOrigin.y, 3, 3);


            my3DCoord camera{
                0 + xShift,
                0 + yShift,
                0 + zShift
            };
            my3DCoord c1{
                200,
                200,
                200
            };
            my3DCoord c2{
                -200,
                200,
                200
            };
            my3DCoord c3{
                -200,
                -200,
                200
            };
            my3DCoord c4{
                200,
                -200,
                200
            };
            my3DCoord c5{
                200,
                200,
                400
            }; 
            my3DCoord c6{
                -200,
                200,
                400
            }; 
            my3DCoord c7{
                -200,
                -200,
                400
            }; 
            my3DCoord c8{
                200,
                -200,
                400
            };
            myCoord c1_projection = getScreenCoords(camera, c1, 50, drawOrigin);
            myCoord c2_projection = getScreenCoords(camera, c2, 50, drawOrigin);
            myCoord c3_projection = getScreenCoords(camera, c3, 50, drawOrigin);
            myCoord c4_projection = getScreenCoords(camera, c4, 50, drawOrigin);
            myCoord c5_projection = getScreenCoords(camera, c5, 50, drawOrigin);
            myCoord c6_projection = getScreenCoords(camera, c6, 50, drawOrigin);
            myCoord c7_projection = getScreenCoords(camera, c7, 50, drawOrigin);
            myCoord c8_projection = getScreenCoords(camera, c8, 50, drawOrigin);


            MoveToEx(hdc, c1_projection.x, c1_projection.y, NULL);
            LineTo(hdc, c2_projection.x, c2_projection.y);

            LineTo(hdc, c3_projection.x, c3_projection.y);
            LineTo(hdc, c4_projection.x, c4_projection.y);
            LineTo(hdc, c1_projection.x, c1_projection.y);

            LineTo(hdc, c5_projection.x, c5_projection.y);
            LineTo(hdc, c6_projection.x, c6_projection.y);
            LineTo(hdc, c7_projection.x, c7_projection.y);
            LineTo(hdc, c8_projection.x, c8_projection.y);
            LineTo(hdc, c5_projection.x, c5_projection.y);

            MoveToEx(hdc, c2_projection.x, c2_projection.y, NULL);
            LineTo(hdc, c6_projection.x, c6_projection.y);

            MoveToEx(hdc, c3_projection.x, c3_projection.y, NULL);
            LineTo(hdc, c7_projection.x, c7_projection.y);

            MoveToEx(hdc, c4_projection.x, c4_projection.y, NULL);
            LineTo(hdc, c8_projection.x, c8_projection.y);

            



            /*
            rect(hdc, c1_projection.x, c1_projection.y, 3, 3);
            rect(hdc, c2_projection.x, c2_projection.y, 3, 3);
            rect(hdc, c3_projection.x, c3_projection.y, 3, 3);
            rect(hdc, c4_projection.x, c4_projection.y, 3, 3);
            rect(hdc, c5_projection.x, c5_projection.y, 3, 3);
            rect(hdc, c6_projection.x, c6_projection.y, 3, 3);
            rect(hdc, c7_projection.x, c7_projection.y, 3, 3);
            rect(hdc, c8_projection.x, c8_projection.y, 3, 3);
            */





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
