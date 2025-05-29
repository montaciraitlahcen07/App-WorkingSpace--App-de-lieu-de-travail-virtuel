    #include <windows.h>
    #include <stdbool.h>

    #define MAX_BUTTON_WIDTHP 180
    #define MAX_BUTTON_HEIGHTP 72
    #define MIN_BUTTON_WIDTHP 135
    #define MIN_BUTTON_HEIGHTP 42

    // UI for message 
    bool UiMessage=FALSE;
    // Inbox 
    RECT Choice_1_Inbox;
    RECT Choice_1_Inbox_Button;
    // General
    RECT Choice_1_General;
    RECT Choice_1_General_Button;
    // when the message button is clicked 
    bool MessageButtonClicked = FALSE;
    // when the button inbox is clicked 
    bool UiInbox = FALSE;
    // when the button general is clicked
    bool UiGeneral = FALSE;
    // for the inbox ui
    RECT ChatRect;
    HWND HandleSearch;
    // creatign the interface when the user click on the button message inbox
    void CreateMessageUi(HDC Mdc,HWND HandleWnd,RECT WindowSize,float CurrentHInbox,float CurrentVInbox,float CurrentHGeneral,float CurrentVGeneral)
    {
        GetClientRect(HandleWnd,&WindowSize); 
        int ButtonWidth=MeasureWindowSize((WindowSize.right-WindowSize.left)*0.13f,MIN_BUTTON_WIDTHP,MAX_BUTTON_WIDTHP);
        int ButtonHeight=MeasureWindowSize((WindowSize.bottom-WindowSize.top)*0.12f,MIN_BUTTON_HEIGHTP,MAX_BUTTON_HEIGHTP);
        // for making the button inbox is size
        Choice_1_Inbox.left=Choice_1_Button.right*0.97+(((WindowSize.right)-(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.025))/2)-300-(CurrentHInbox/2);
        Choice_1_Inbox.top=(WindowSize.bottom-WindowSize.top)*0.27f-(CurrentVInbox/2);
        Choice_1_Inbox.right=Choice_1_Inbox.left+ButtonWidth+(CurrentHInbox/2);
        Choice_1_Inbox.bottom=Choice_1_Inbox.top+ButtonHeight+(CurrentVInbox/2);
        Choice_1_Inbox_Button=Choice_1_Inbox;
        Choice_1_Inbox.top+=(WindowSize.bottom-WindowSize.top)*0.035;
        Choice_1_Inbox.bottom+=(WindowSize.bottom-WindowSize.top)*0.043;

        // for making the button general is size
        Choice_1_General.left=Choice_1_Button.right+(((WindowSize.right)-(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.025))/2)+127-(CurrentHGeneral/2);
        Choice_1_General.top=(WindowSize.bottom-WindowSize.top)*0.27f-(CurrentVGeneral/2);
        Choice_1_General.right=Choice_1_General.left+ButtonWidth+(CurrentHGeneral/2);
        Choice_1_General.bottom=Choice_1_General.top+ButtonHeight+(CurrentVGeneral/2);
        Choice_1_General_Button=Choice_1_General;
        Choice_1_General.top+=(WindowSize.bottom-WindowSize.top)*0.035;
        Choice_1_General.bottom+=(WindowSize.bottom-WindowSize.top)*0.043;

        HPEN Pen=CreatePen(BS_SOLID,2,RGB(180, 180, 190));
        HPEN OldPen=SelectObject(Mdc,Pen);
        HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
        HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
        int lineX =Choice_1_Button.right*0.93+(((WindowSize.right)-(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.025))/2); 
        int lineTop = PanelRect.bottom +(WindowSize.bottom-WindowSize.top)*0.047;
        int lineBottom = Choice_1_Button.bottom + (WindowSize.bottom-WindowSize.top)*0.055; 
        MoveToEx(Mdc, lineX, lineTop, NULL);
        LineTo(Mdc, lineX, lineBottom);
        HFONT Font=CreateFont(
        25,
        12,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        "Segoe UI");
        HFONT OldFont=SelectObject(Mdc,Font);
        // for inbox 
        RoundRect(Mdc,Choice_1_Inbox_Button.left,Choice_1_Inbox_Button.top,Choice_1_Inbox_Button.right,Choice_1_Inbox_Button.bottom,32,32);
        DrawText(Mdc,"Inbox",-1,&Choice_1_Inbox,DT_SINGLELINE | DT_CENTER);
        // for general
        RoundRect(Mdc,Choice_1_General_Button.left,Choice_1_General_Button.top,Choice_1_General_Button.right,Choice_1_General_Button.bottom,32,32);
        DrawText(Mdc,"General",-1,&Choice_1_General,DT_SINGLELINE | DT_CENTER);
        SelectObject(Mdc, OldPen);
        DeleteObject(Pen);
        SelectObject(Mdc,OldFont);
        SelectObject(Mdc,OldButtonColor);
        DeleteObject(ButtonColor);
        DeleteObject(Font);
    }
    void CreateInboxUi(HDC Mdc,HWND HandleWnd,RECT WindowSize,HINSTANCE IDhInstance)
    {
        ChatRect.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.018;
        ChatRect.top = WindowSize.top+(WindowSize.bottom-WindowSize.top)*0.176+ (WindowSize.bottom - WindowSize.top)*0.043;
        ChatRect.right = ChatRect.left + (WindowSize.right - WindowSize.left)*0.1;
        ChatRect.bottom = ChatRect.top + (WindowSize.bottom - WindowSize.top)*0.04;
        HFONT Font=CreateFont( 25,10,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Arial");
        HFONT OldFont=(HFONT)SelectObject(Mdc,Font);
        HPEN Pen=CreatePen(PS_DOT,1,RGB(244, 239, 206));
        HPEN OldPen=SelectObject(Mdc,Pen);
        DrawText(Mdc,"Chat",-1,&ChatRect,DT_SINGLELINE | DT_CENTER);
        SelectObject(Mdc,OldFont);
        DeleteObject(Font);
        SelectObject(Mdc,OldPen);
        DeleteObject(Pen);
      
    }
    void LineDifferenceMessage(HDC Mdc,HWND HandleWnd,RECT WindowSize)
    {
        GetClientRect(HandleWnd,&WindowSize);
        HPEN Pen=CreatePen(BS_SOLID,2,RGB(180, 180, 190));
        HPEN OldPen=SelectObject(Mdc,Pen);
        int lineX = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.285; 
        int lineTop = ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.02 + 35;
        int lineBottom = WindowSize.bottom - (WindowSize.bottom-WindowSize.top)*0.05; 
        MoveToEx(Mdc, lineX, lineTop, NULL);
        LineTo(Mdc, lineX, lineBottom);
        SelectObject(Mdc, OldPen);
        DeleteObject(Pen);
    }
void DrawMessageBubbleLogoLeft(HDC hdc, int x, int y, int width, int height, int strokeWidth,RECT WindowSize)
{
    RECT ChatString;
    HPEN hPen = CreatePen(PS_SOLID, strokeWidth, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    
    int cornerRadius = height / 6;
    int tailWidth = width / 8;
    int tailHeight = height / 4;
    
    int bubbleLeft = x;
    int bubbleTop = y;
    int bubbleRight = x + width;
    int bubbleBottom = y + height - tailHeight;
    ChatString.left =x; 
    ChatString.top = y + (WindowSize.right - WindowSize.left)*0.022;
    ChatString.right = x + width;
    ChatString.bottom = y + height + tailHeight - (WindowSize.right - WindowSize.left)*0.05;
    
    RoundRect(hdc, bubbleLeft, bubbleTop, bubbleRight, bubbleBottom, 
    cornerRadius * 2, cornerRadius * 2);
        
    POINT tailPoints[3];
    tailPoints[0].x = bubbleLeft + tailWidth;
    tailPoints[0].y = bubbleBottom - strokeWidth/2;
    tailPoints[1].x = bubbleLeft + tailWidth * 2;
    tailPoints[1].y = bubbleBottom - strokeWidth/2;
    tailPoints[2].x = bubbleLeft + tailWidth / 2;  
    tailPoints[2].y = y + height;                  
    
    MoveToEx(hdc, tailPoints[0].x, tailPoints[0].y, NULL);
    LineTo(hdc, tailPoints[1].x, tailPoints[1].y);
    LineTo(hdc, tailPoints[2].x, tailPoints[2].y);
    LineTo(hdc, tailPoints[0].x, tailPoints[0].y);
    
    HFONT Font=CreateFont( 25,10,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
    DEFAULT_PITCH|FF_SWISS,"Arial");
    HFONT OldFont=(HFONT)SelectObject(hdc,Font);
    HPEN Pen=CreatePen(PS_DOT,3,RGB(244, 239, 206));
    HPEN OldPen=SelectObject(hdc,Pen);
    DrawText(hdc,"Chat",-1,&ChatString,DT_SINGLELINE | DT_CENTER);
    SelectObject(hdc,OldFont);
    SelectObject(hdc,OldPen);
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    
    DeleteObject(Pen);
    DeleteObject(Font);
    DeleteObject(hPen);
}

void DrawMessageBubbleLogoRight(HDC hdc, int x, int y, int width, int height, int strokeWidth,RECT WindowSize)
{
    RECT ChatString;
    HPEN hPen = CreatePen(PS_SOLID, strokeWidth, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    
    int cornerRadius = height / 6;
    int tailWidth = width / 8;
    int tailHeight = height / 4;
    
    int bubbleLeft = x;
    int bubbleTop = y;
    int bubbleRight = x + width;
    int bubbleBottom = y + height - tailHeight;
    ChatString.left =x; 
    ChatString.top = y + (WindowSize.right - WindowSize.left)*0.016;
    ChatString.right = x + width;
    ChatString.bottom = y + height + tailHeight - (WindowSize.right - WindowSize.left)*0.025;

    RoundRect(hdc, bubbleLeft, bubbleTop, bubbleRight, bubbleBottom, 
    cornerRadius * 2, cornerRadius * 2);
    
    POINT tailPoints[3];
    tailPoints[0].x = bubbleLeft + tailWidth;
    tailPoints[0].y = bubbleBottom - strokeWidth/2;
    tailPoints[1].x = bubbleLeft + tailWidth * 2;
    tailPoints[1].y = bubbleBottom - strokeWidth/2;
    tailPoints[2].x = bubbleLeft + tailWidth / 2;  
    tailPoints[2].y = y + height ;                  
    
    MoveToEx(hdc, tailPoints[0].x, tailPoints[0].y, NULL);
    LineTo(hdc, tailPoints[1].x, tailPoints[1].y);
    LineTo(hdc, tailPoints[2].x, tailPoints[2].y);
    LineTo(hdc, tailPoints[0].x, tailPoints[0].y);
    
    HFONT Font=CreateFont( 25,10,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
    DEFAULT_PITCH|FF_SWISS,"Arial");
    HFONT OldFont=(HFONT)SelectObject(hdc,Font);
    HPEN Pen=CreatePen(PS_DOT,3,RGB(244, 239, 206));
    HPEN OldPen=SelectObject(hdc,Pen);
    DrawText(hdc,"Here",-1,&ChatString,DT_SINGLELINE | DT_CENTER);
    SelectObject(hdc,OldFont);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc,OldPen);
    
    DeleteObject(Pen);
    DeleteObject(Font);
    DeleteObject(hPen);
}
// creating Search button it is located in the left of the bar of the search
RECT SearchAnimation;
void CreateSearchButton(HDC Mdc,float CurrentHSearch,float CurrentVSearch,RECT WindowSize,RECT ChatRect,int strokeWidth)
{
    SearchAnimation.left =  ChatRect.right + (WindowSize.right - WindowSize.left)*0.112 - (CurrentHSearch/2);
    SearchAnimation.top = ChatRect.top + (WindowSize.bottom - WindowSize.top)*0.0745 - (CurrentVSearch/2);
    SearchAnimation.right = SearchAnimation.left + (WindowSize.right - WindowSize.left)*0.04 + (CurrentHSearch/2);
    SearchAnimation.bottom = ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.0725 + (CurrentVSearch/2);

    HFONT Font=CreateFont(
    25,
    12,
    0,
    0,
    FW_NORMAL,
    FALSE,
    FALSE,
    FALSE,
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,
    "Segoe UI");
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    HFONT OldFont=SelectObject(Mdc,Font);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,SearchAnimation.left,SearchAnimation.top-15,SearchAnimation.right,SearchAnimation.bottom+2,19,24);

    HPEN hPen = CreatePen(PS_SOLID, strokeWidth, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(Mdc, hPen);
    Ellipse(Mdc,SearchAnimation.left + (WindowSize.right - WindowSize.left)*0.012,
    SearchAnimation.top -(WindowSize.bottom - WindowSize.top)*0.0116 + (WindowSize.bottom - WindowSize.top)*0.005,
    SearchAnimation.left + (WindowSize.right - WindowSize.left)*0.012 + (WindowSize.right - WindowSize.left)*0.012,
    SearchAnimation.top -(WindowSize.bottom - WindowSize.top)*0.0116+ (WindowSize.bottom - WindowSize.top)*0.0248 + (WindowSize.bottom - WindowSize.top)*0.005);

    MoveToEx(Mdc,SearchAnimation.left + (WindowSize.right - WindowSize.left)*0.01571 + (WindowSize.right - WindowSize.left)*0.012 - (WindowSize.right - WindowSize.left)*0.0056,
    SearchAnimation.top -(WindowSize.bottom - WindowSize.top)*0.0116+ (WindowSize.bottom - WindowSize.top)*0.02175 + (WindowSize.bottom - WindowSize.top)*0.005,NULL);
    LineTo(Mdc,SearchAnimation.left + (WindowSize.right - WindowSize.left)*0.0291,
    SearchAnimation.top -(WindowSize.bottom - WindowSize.top)*0.0115+ (WindowSize.bottom - WindowSize.top)*0.036 + (WindowSize.bottom - WindowSize.top)*0.005);

    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    SelectObject(Mdc,hOldPen);
    SelectObject(Mdc,OldButtonColor);

    DeleteObject(Font);
    DeleteObject(hPen);
    DeleteObject(Pen);
    DeleteObject(ButtonColor);
}
// create scrollbar for contact
HWND ScrollBar;
RECT ScrollBarRect;
SCROLLINFO SCRL = {0};
bool Scroll = TRUE;
void CreateScrollBar(HWND HandleWnd,HINSTANCE IDhInstance,RECT WindowSize)
{
    // when the window resize we change the placement of the child window
    MoveWindow(ScrollBar,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.03,
    ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.08,
    (WindowSize.right - WindowSize.left)/2 - ((WindowSize.right - WindowSize.left)*0.25),
    (WindowSize.bottom-WindowSize.top)*0.643,TRUE);
    if(Scroll)
    {
        ScrollBar = CreateWindowEx(
        0,
        "STATIC",
        NULL,
        WS_VISIBLE | WS_VSCROLL | WS_CHILD ,
        Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.03,
        ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.08,
        (WindowSize.right - WindowSize.left)/2 - ((WindowSize.right - WindowSize.left)*0.25),
        (WindowSize.bottom-WindowSize.top)*0.643,
        HandleWnd, NULL, IDhInstance, NULL
        );
        SCRL.cbSize = sizeof(SCROLLINFO);
        SCRL.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        SCRL.nMin = 0;
        SCRL.nMax = 800;
        SCRL.nPage = 50;
        SCRL.nPos = 0;
        SetScrollInfo(ScrollBar,SB_VERT,&SCRL,TRUE);
    }
}
