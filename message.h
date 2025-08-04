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
    // create the child window of scrollbar
    typedef struct
    {
        float min_val;
        float max_val;
        float current_val;
        float page_size;
        BOOL is_dragging;
        float drag_offset;
        RECT thumb_rect;
        BOOL thumb_hover;
        BOOL thumb_pressed;
    }ScrollbarInfo;
    ScrollbarInfo g_scrollbar;
    // create setting for conversation is thumb
    // 
    int choseentype;
    //
    ScrollbarInfo Conversation_thumb;
    void CalculateThumbRect(HWND hwnd, RECT* thumb_rect,RECT WindowSize);
    void DrawScrollBar(HDC hdc, HWND hwnd,RECT WindowSize);
    BOOL PointInRect(POINT pt, RECT* rect);
    void UpdateScrollValue(HWND hwnd, float new_val);
    void UpdateScrollbarRange(HWND hwnd,RECT ScrollBarRect,ScrollbarInfo *g_scrollbar);
    void DrawContentWithScroll(HDC Mdc_Child_1, HWND hwnd,RECT ScrollBarRect,Clients Message[100],ScrollbarInfo *g_scrollbar);
    void UiInboxConversation(HWND HandleWnd,HDC Mdc,CntTrd ConnectingTools,RECT WindowSize,RECT PanelRect,float CurrentHEmoji,float CurrentVEmoji,float CurrentHAttach,float CurrentVAttach,float CurrentHSend,float CurrentVSend);
    int GetClickedRecipient(POINT click_point);
    void BuildVisibleItemsList(HDC hdc, HWND hwnd, RECT ScrollBarRect, Clients Message[], ScrollbarInfo *g_scrollbar);
    // creatign the interface when the user click on the button message inbox
    void CreateMessageUi(HDC Mdc,HWND HandleWnd,RECT WindowSize,float CurrentHInbox,float CurrentVInbox,float CurrentHGeneral,float CurrentVGeneral,bool FontSize)
    {
        GetClientRect(HandleWnd,&WindowSize); 
        int ButtonWidth=MeasureWindowSize((WindowSize.right-WindowSize.left)*0.13f,MIN_BUTTON_WIDTHP,MAX_BUTTON_WIDTHP);
        int ButtonHeight=MeasureWindowSize((WindowSize.bottom-WindowSize.top)*0.12f,MIN_BUTTON_HEIGHTP,MAX_BUTTON_HEIGHTP);
        // for making the button inbox is size
        Choice_1_Inbox.left=Choice_1_Button.right*0.97+(((WindowSize.right)-(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.025))/2)-(WindowSize.right-WindowSize.left)*0.28-(CurrentHInbox/2);
        Choice_1_Inbox.top=(WindowSize.bottom-WindowSize.top)*0.27f-(CurrentVInbox/2);
        Choice_1_Inbox.right=Choice_1_Inbox.left+ButtonWidth+(CurrentHInbox/2);
        Choice_1_Inbox.bottom=Choice_1_Inbox.top+ButtonHeight+(CurrentVInbox/2);
        Choice_1_Inbox_Button=Choice_1_Inbox;
        Choice_1_Inbox.top+=(WindowSize.bottom-WindowSize.top)*0.035;
        Choice_1_Inbox.bottom+=(WindowSize.bottom-WindowSize.top)*0.043;

        // for making the button general is size
        Choice_1_General.left=Choice_1_Button.right+(((WindowSize.right)-(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.025))/2)+(WindowSize.right-WindowSize.left)*0.14-(CurrentHGeneral/2);
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
        HFONT Font;
        if(FontSize)
        {
            Font=CreateFont(
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
        }
        else
        {
            Font=CreateFont(
            22,
            10,
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
        }
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
        ChatRect.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.021;
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
// create scrollbar for contact
#define ID_CHILD_WINDOW 1015
#define SCROLLBAR_WIDTH 12

#define TRACK_COLOR RGB(240, 240, 240)
#define THUMB_COLOR RGB(170, 170, 170)
#define THUMB_HOVER_COLOR RGB(140, 140, 140)
#define THUMB_PRESSED_COLOR RGB(100, 100, 100)
HWND ScrollBar;
RECT ScrollBarRect;
// for calculating the thumb is rect
void CalculateThumbRect(HWND hwnd, RECT* thumb_rect,RECT WindowSize)
{
    RECT client_rect;
    GetClientRect(hwnd, &client_rect);   
    float scrollbar_height = (client_rect.bottom - client_rect.top - 4);
    float range = g_scrollbar.max_val - g_scrollbar.min_val;   
    if (range <= 0) range = 1;
    float thumb_height = max(20, (g_scrollbar.page_size * scrollbar_height) / (range + g_scrollbar.page_size));
    float track_height = scrollbar_height - thumb_height;
    float thumb_pos = 0;
    if (range > 0)
    {
        thumb_pos = (g_scrollbar.current_val * track_height) / range;
    }   
    thumb_rect->left = client_rect.right - SCROLLBAR_WIDTH - 2;
    thumb_rect->top =  thumb_pos;
    thumb_rect->right = client_rect.right - 2;
    thumb_rect->bottom = thumb_rect->top + thumb_height;
}
// for drawing the thumb 
void DrawScrollBar(HDC hdc, HWND hwnd,RECT WindowSize) {
    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    
    RECT track_rect = {
        client_rect.right - SCROLLBAR_WIDTH - 2,
        0,
        client_rect.right - 2,
        client_rect.bottom
    };
    
    HBRUSH track_brush = CreateSolidBrush(TRACK_COLOR);
    FillRect(hdc, &track_rect, track_brush);
    DeleteObject(track_brush);
    
    CalculateThumbRect(hwnd,&g_scrollbar.thumb_rect,WindowSize);
    
    COLORREF thumb_color = THUMB_COLOR;
    if (g_scrollbar.thumb_pressed) {
        thumb_color = THUMB_PRESSED_COLOR;
    } else if (g_scrollbar.thumb_hover) {
        thumb_color = THUMB_HOVER_COLOR;
    }
    
    HBRUSH thumb_brush = CreateSolidBrush(thumb_color);
    
    HPEN old_pen = SelectObject(hdc, CreatePen(PS_SOLID, 1, thumb_color));
    HBRUSH old_brush = SelectObject(hdc, thumb_brush);
    
    RoundRect(hdc, 
    g_scrollbar.thumb_rect.left + 2, 
    g_scrollbar.thumb_rect.top,
    g_scrollbar.thumb_rect.right - 2, 
    g_scrollbar.thumb_rect.bottom,
    6, 6);
    MoveWindow(hwnd,((WindowSize.right-WindowSize.left)*0.02f + MeasureWindowSize((WindowSize.right-WindowSize.left)*0.12f,MIN_BUTTON_WIDTH,MAX_BUTTON_WIDTH)) + (WindowSize.right-WindowSize.left)*0.03,
    (WindowSize.top+(WindowSize.bottom-WindowSize.top)*0.176+ (WindowSize.bottom - WindowSize.top)*0.043 + (WindowSize.bottom - WindowSize.top)*0.04) + (WindowSize.bottom - WindowSize.top)*0.08,
    (WindowSize.right - WindowSize.left)/2 - ((WindowSize.right - WindowSize.left)*0.25),
    (WindowSize.bottom-WindowSize.top)*0.643,TRUE);
    
    DeleteObject(SelectObject(hdc, old_pen));
    SelectObject(hdc, old_brush);
    DeleteObject(thumb_brush);
}
//for checking is the user hovering or clicking on the thumb
BOOL PointInRect(POINT pt, RECT* rect)
{
    return (pt.x >= rect->left && pt.x <= rect->right && 
            pt.y >= rect->top && pt.y <= rect->bottom);
}
// updating the thumb is place in the scrollbar 
void UpdateScrollValue(HWND hwnd, float new_val)
{
    new_val = max(g_scrollbar.min_val, min(g_scrollbar.max_val, new_val));
    if (new_val != g_scrollbar.current_val)
    {
        g_scrollbar.current_val = new_val;
    }
}
int total_items;
float window_height;
int items_per_page;
extern int i;
void UpdateScrollbarRange(HWND hwnd,RECT ScrollBarRect,ScrollbarInfo *g_scrollbar)
{
    GetClientRect(hwnd,&ScrollBarRect);
    total_items = i;
    window_height = (ScrollBarRect.bottom - ScrollBarRect.top) - 4;
    items_per_page = window_height / 80;
    g_scrollbar->min_val = 0;
    if(total_items <= items_per_page)
    {
        g_scrollbar->max_val = 1; 
        g_scrollbar->page_size = items_per_page;
    } 
    else
    {
        g_scrollbar->max_val = total_items - items_per_page + 1;
        g_scrollbar->page_size = items_per_page;
    }   
}
// storing index's of searched user
int ListSearchedRecipient[100] = {0};
int CompSearchedRecipient = 0;
int Index = -1;
void DrawContentWithScroll(HDC Mdc_Child_1, HWND hwnd,RECT ScrollBarRect,Clients Message[100],ScrollbarInfo *g_scrollbar)
{
    char buffer[50];
    GetWindowText(HandleSearch, buffer, sizeof(buffer));
    if (strlen(buffer) == 0)
    {
        float scroll_offset = g_scrollbar->current_val * 80;
        float visible_height = (ScrollBarRect.bottom - ScrollBarRect.top) - 4;
        i = countclientStatus;
        for (int j = 0; j < i;j++)
        { 
            float item_y = (j * 80) - scroll_offset;
            
            if (item_y < -80 || item_y > visible_height + 80)
            {
                continue;
            }
            SetTextColor(Mdc_Child_1,RGB(0,0,0));
            RECT item_rect = {ScrollBarRect.left, item_y, ScrollBarRect.right, item_y + (ScrollBarRect.bottom - ScrollBarRect.top)*0.2};
            HPEN Pen=CreatePen(BS_SOLID,2,RGB(180, 180, 190));
            HPEN OldPen=SelectObject(Mdc_Child_1,Pen);
            MoveToEx(Mdc_Child_1,item_rect.left + (ScrollBarRect.right - ScrollBarRect.left)*0.08,item_rect.bottom,NULL);
            LineTo(Mdc_Child_1,item_rect.right - (ScrollBarRect.right - ScrollBarRect.left)*0.11,item_rect.bottom);
            SelectObject(Mdc_Child_1, OldPen);
            DeleteObject(Pen);
            Pen=CreatePen(BS_SOLID,3,RGB(180, 180, 190));
            OldPen=SelectObject(Mdc_Child_1,Pen);
            item_rect.top+=(ScrollBarRect.bottom - ScrollBarRect.top)*0.07;
            item_rect.bottom-=(ScrollBarRect.bottom - ScrollBarRect.top)*0.04;
            item_rect.right -= (ScrollBarRect.right - ScrollBarRect.left) * 0.3;
            RECT IsActiveRect = item_rect;
            IsActiveRect.top += (ScrollBarRect.bottom - ScrollBarRect.top)*0.065;
            IsActiveRect.bottom += (ScrollBarRect.bottom - ScrollBarRect.top)*0.05;
            HFONT Font=CreateFont(14,10,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
            DEFAULT_PITCH|FF_SWISS,"Arial");
            HFONT OldFont=(HFONT)SelectObject(Mdc_Child_1,Font);
            DrawText(Mdc_Child_1,Message[j].Username,-1, &item_rect, DT_SINGLELINE | DT_CENTER);
            if(Message[j].IsActive)
            {
                SetTextColor(Mdc_Child_1,RGB(0,255, 0));
                DrawText(Mdc_Child_1,"Online",-1,&IsActiveRect, DT_SINGLELINE | DT_CENTER);
            }
            else
            {
                SetTextColor(Mdc_Child_1,RGB(255,0, 0));
                DrawText(Mdc_Child_1,"Offline",-1,&IsActiveRect, DT_SINGLELINE | DT_CENTER);
            }
            SelectObject(Mdc_Child_1,OldFont);
            DeleteObject(Font);
            SelectObject(Mdc_Child_1, OldPen);
            DeleteObject(Pen);           
        }
        if (i == 0)
        {
            RECT no_users_rect = {
                ScrollBarRect.left + (ScrollBarRect.right - ScrollBarRect.left) * 0.05, 
                ScrollBarRect.top + (ScrollBarRect.bottom - ScrollBarRect.top)/2 - (ScrollBarRect.bottom - ScrollBarRect.top)*0.11, 
                ScrollBarRect.right - (ScrollBarRect.right - ScrollBarRect.left) * 0.052, 
                ScrollBarRect.top + (ScrollBarRect.bottom - ScrollBarRect.top)/2 
            };
            
            HFONT Font = CreateFont(16,8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                                   DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "Arial");
            HFONT OldFont = (HFONT)SelectObject(Mdc_Child_1, Font);
            SetTextColor(Mdc_Child_1, RGB(128, 128, 128));
            DrawText(Mdc_Child_1, "No users available", -1, &no_users_rect, DT_SINGLELINE | DT_CENTER);
            SelectObject(Mdc_Child_1, OldFont);
            DeleteObject(Font);
        }
    }
    else
    {
        int Cmp = FillingSearchRecipientList(HandleSearch,countclientStatus,Message,ListSearchedRecipient,CompSearchedRecipient);
        float scroll_offset = g_scrollbar->current_val * 80;
        float visible_height = (ScrollBarRect.bottom - ScrollBarRect.top) - 4;
        for (int j = 0; j <Cmp;j++)
        { 
            float item_y = (j * 80) - scroll_offset;
            
            if (item_y < -80 || item_y > visible_height + 80)
            {
                continue;
            }
            SetTextColor(Mdc_Child_1,RGB(0,0,0));
            RECT item_rect = {ScrollBarRect.left, item_y, ScrollBarRect.right, item_y + (ScrollBarRect.bottom - ScrollBarRect.top)*0.2};
            HPEN Pen=CreatePen(BS_SOLID,2,RGB(180, 180, 190));
            HPEN OldPen=SelectObject(Mdc_Child_1,Pen);
            MoveToEx(Mdc_Child_1,item_rect.left + (ScrollBarRect.right - ScrollBarRect.left)*0.08,item_rect.bottom,NULL);
            LineTo(Mdc_Child_1,item_rect.right - (ScrollBarRect.right - ScrollBarRect.left)*0.11,item_rect.bottom);
            SelectObject(Mdc_Child_1, OldPen);
            DeleteObject(Pen);
            Pen=CreatePen(BS_SOLID,3,RGB(180, 180, 190));
            OldPen=SelectObject(Mdc_Child_1,Pen);
            item_rect.top+=(ScrollBarRect.bottom - ScrollBarRect.top)*0.07;
            item_rect.bottom-=(ScrollBarRect.bottom - ScrollBarRect.top)*0.04;
            item_rect.right -= (ScrollBarRect.right - ScrollBarRect.left) * 0.3;
            RECT IsActiveRect = item_rect;
            IsActiveRect.top += (ScrollBarRect.bottom - ScrollBarRect.top)*0.065;
            IsActiveRect.bottom += (ScrollBarRect.bottom - ScrollBarRect.top)*0.05;
            HFONT Font=CreateFont( 14,10,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
            DEFAULT_PITCH|FF_SWISS,"Arial");
            HFONT OldFont=(HFONT)SelectObject(Mdc_Child_1,Font);
            DrawText(Mdc_Child_1,Message[ListSearchedRecipient[j]].Username,-1, &item_rect, DT_SINGLELINE | DT_CENTER);
            if(Message[ListSearchedRecipient[j]].IsActive)
            {
                SetTextColor(Mdc_Child_1,RGB(0,255, 0));
                DrawText(Mdc_Child_1,"Online",-1,&IsActiveRect, DT_SINGLELINE | DT_CENTER);
            }
            else
            {
                SetTextColor(Mdc_Child_1,RGB(255,0, 0));
                DrawText(Mdc_Child_1,"Offline",-1,&IsActiveRect, DT_SINGLELINE | DT_CENTER);
            }
            SelectObject(Mdc_Child_1,OldFont);
            DeleteObject(Font);
            SelectObject(Mdc_Child_1, OldPen);
            DeleteObject(Pen);
            
        }
        if(Cmp == 0)
        {
            RECT no_users_rect = {
                ScrollBarRect.left + (ScrollBarRect.right - ScrollBarRect.left) * 0.05, 
                ScrollBarRect.top + (ScrollBarRect.bottom - ScrollBarRect.top)/2 - (ScrollBarRect.bottom - ScrollBarRect.top)*0.11, 
                ScrollBarRect.right - (ScrollBarRect.right - ScrollBarRect.left) * 0.052, 
                ScrollBarRect.top + (ScrollBarRect.bottom - ScrollBarRect.top)/2 
            };
            
            HFONT Font = CreateFont(16,8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                                   DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "Arial");
            HFONT OldFont = (HFONT)SelectObject(Mdc_Child_1, Font);
            SetTextColor(Mdc_Child_1, RGB(128, 128, 128));
            DrawText(Mdc_Child_1, "No users available", -1, &no_users_rect, DT_SINGLELINE | DT_CENTER);
            SelectObject(Mdc_Child_1, OldFont);
            DeleteObject(Font);
        }
    }
}
// this function it draw the converstion with recipient (adding the feature in it which is using file of the past conversation )
HWND MessageBarHandle;
void UiInboxConversation(HWND HandleWnd,HDC Mdc,CntTrd ConnectingTools,RECT WindowSize,RECT PanelRect,float CurrentHEmoji,float CurrentVEmoji,float CurrentHAttach,float CurrentVAttach,float CurrentHSend,float CurrentVSend)
{
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    GetClientRect(HandleWnd,&WindowSize);   
    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.009,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.834,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.07,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);
    // emoji circle Button
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.296 - CurrentHEmoji,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0872 - CurrentVEmoji,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.326 + CurrentHEmoji,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0285 + CurrentVEmoji);
    // Attach Button
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.335 - CurrentHAttach,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0872 - CurrentVAttach,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3655 + CurrentHAttach,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0285 + CurrentVAttach);
    // send message button 
    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.425 - CurrentHSend,
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0875 - CurrentVSend,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.44 +  
    (WindowSize.right - WindowSize.left)*0.02 + CurrentHSend,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.075 + (WindowSize.bottom - WindowSize.top)*0.05 + CurrentVSend,
    (WindowSize.right - WindowSize.left)*0.0175,(WindowSize.bottom - WindowSize.top)*0.026); 
    SelectObject(Mdc,OldPen);
    DeleteObject(Pen);
    SelectObject(Mdc,OldButtonColor);
    DeleteObject(OldButtonColor);
    // rendering the recipient is name
    RECT RecipientRect;
    RecipientRect.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3;
    RecipientRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.022;
    RecipientRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.52;
    RecipientRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.05;
    HBRUSH ButtonColorRecipient = CreateSolidBrush(RGB(0,0,0));
    HBRUSH OldButtonColorRecipient = SelectObject(Mdc,ButtonColorRecipient);
    HFONT Font=CreateFont(23,11,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,"Segoe UI");
    HFONT OldFont=(HFONT)SelectObject(Mdc,Font);
    HPEN PenRecipient=CreatePen(PS_DOT,2,RGB(0,0,0));
    HPEN OldPenRecipient=SelectObject(Mdc,PenRecipient);
    DrawText(Mdc,ConnectingTools.PrivateMessage.SelectedRecipient,-1,&RecipientRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldFont);
    DeleteObject(Font);
    // Emoji form
    // left eye
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.303 - CurrentHEmoji/4.2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0633 - CurrentVEmoji/4.2,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.307 + CurrentHEmoji/4.2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.058 + CurrentVEmoji/4.2);
    // right eye
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.316 - CurrentHEmoji/4.2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0633 - CurrentVEmoji/4.2,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.31953 + CurrentHEmoji/4.2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.058 + CurrentVEmoji/4.2);
    // smile 
    Arc(Mdc,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.303 - CurrentHEmoji/4.7,
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.054 - CurrentVEmoji/4.2,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.319 + CurrentHEmoji/4.2,     
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.04 + CurrentVEmoji/3,       
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.303 - CurrentHEmoji/4.2,     
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.048 + CurrentVEmoji/4.2,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.319 + CurrentHEmoji/4.2,
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.048 + CurrentVEmoji/4.2);
    // attach
    // first line
    MoveToEx(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3415 - CurrentHAttach/5.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0615 - CurrentVAttach/5.8,NULL);
    LineTo(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.358 + CurrentHAttach/5.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0615 + CurrentVAttach/5.8);
    // second line
    MoveToEx(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3415 - CurrentHAttach/5.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.05 - CurrentVAttach/5.8,NULL);
    LineTo(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.358 + CurrentHAttach/5.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.05 + CurrentVAttach/5.8);
    // first line is ellipse
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.34475 - CurrentHAttach/6.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.065 - CurrentVAttach/6.5,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3484 + CurrentHAttach/6.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0562 + CurrentVAttach/6.25);
    // second line is ellipse
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.35278 - CurrentHAttach/6.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0539 - CurrentVAttach/6.5,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3564 + CurrentHAttach/6.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.045 + CurrentVAttach/6.5);
    SelectObject(Mdc,OldButtonColorRecipient);
    DeleteObject(OldButtonColorRecipient);
    SelectObject(Mdc,OldPenRecipient);
    DeleteObject(PenRecipient);
    // search button where you can search in the conversation about some words or infos
    HBRUSH Gray = CreateSolidBrush(RGB(210, 210, 210));
    OldButtonColor=SelectObject(Mdc,Gray);
    HPEN penSend = CreatePen(PS_SOLID,3,RGB(0, 0, 0));
    HPEN oldPenSend = SelectObject(Mdc,penSend);
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.364 + (WindowSize.right - WindowSize.left)*0.43,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.024,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.368 + (WindowSize.right - WindowSize.left)*0.447,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.0452);
    MoveToEx(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.364 + (WindowSize.right - WindowSize.left)*0.43085,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.04222,NULL);
    LineTo(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.364 + (WindowSize.right - WindowSize.left)*0.42605,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.04705);
    SelectObject(Mdc,OldButtonColor);
    DeleteObject(Gray);
    // bracket on button send 
    // down line 
    MoveToEx(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.425 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.44 +  
    (WindowSize.right - WindowSize.left)*0.02)/2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.071 - CurrentVSend/5,NULL);
    LineTo(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.425 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.44 +  
    (WindowSize.right - WindowSize.left)*0.02)/2,
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.095 + (WindowSize.bottom - WindowSize.top)*0.0505 + CurrentVSend/5);
    // left line
    MoveToEx(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.425 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.44 +  
    (WindowSize.right - WindowSize.left)*0.02)/2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.071 - CurrentVSend/5,NULL);
    LineTo(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.38 + (WindowSize.right - WindowSize.left)*0.4222 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.441)/2,
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.059 - CurrentVSend/5);
    // right line 
    MoveToEx(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.425 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.44 +  
    (WindowSize.right - WindowSize.left)*0.02)/2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.071 - CurrentVSend/5,NULL);
    LineTo(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.377 + (WindowSize.right - WindowSize.left)*0.424 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3765 + (WindowSize.right - WindowSize.left)*0.44 +  
    (WindowSize.right - WindowSize.left)*0.03405)/2,
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.059 - CurrentVSend/5);
    SelectObject(Mdc,oldPenSend);
    DeleteObject(penSend);
    SelectObject(Mdc,OldButtonColorRecipient);
    DeleteObject(OldButtonColorRecipient);
}
// taking the index of the user who is been taken by the user for a conversation 
int GetClickedRecipient(POINT click_point)
{
    for (int i = 0; i < visible_item_count; i++)
    {
        if (visible_items[i].is_visible && PtInRect(&visible_items[i].item_rect, click_point))
        {
            return visible_items[i].recipient_index;
        }
    }
    return -1; 
}
// build a list of visible users
void BuildVisibleItemsList(HDC hdc, HWND hwnd, RECT ScrollBarRect, Clients Message[], ScrollbarInfo *g_scrollbar)
{
    visible_item_count = 0;
    char search_buffer[50];
    GetWindowText(HandleSearch, search_buffer, sizeof(search_buffer));
    float scroll_offset = g_scrollbar->current_val * 80;
    float visible_height = (ScrollBarRect.bottom - ScrollBarRect.top) - 4;
    
    int total_recipients = (strlen(search_buffer) == 0) ? countclientStatus : 
                          FillingSearchRecipientList(HandleSearch, countclientStatus, Message, ListSearchedRecipient, CompSearchedRecipient);
    
    for (int j = 0; j < total_recipients; j++)
    {
        float item_y = (j * 80) - scroll_offset;
        if(item_y < -80 || item_y > visible_height + 80)
        {
            continue;
        }
        int recipient_idx = (strlen(search_buffer) == 0) ? j : ListSearchedRecipient[j];
        visible_items[visible_item_count].recipient_index = recipient_idx;
        visible_items[visible_item_count].item_rect.left = ScrollBarRect.left;
        visible_items[visible_item_count].item_rect.top = ScrollBarRect.top + item_y;
        visible_items[visible_item_count].item_rect.right = ScrollBarRect.right - SCROLLBAR_WIDTH - 5;
        visible_items[visible_item_count].item_rect.bottom = ScrollBarRect.top + item_y + (ScrollBarRect.bottom - ScrollBarRect.top)*0.2;
        
        visible_items[visible_item_count].text_rect = visible_items[visible_item_count].item_rect;
        visible_items[visible_item_count].text_rect.top += 10;
        visible_items[visible_item_count].text_rect.bottom -= 30;
        visible_items[visible_item_count].text_rect.right -= (ScrollBarRect.right - ScrollBarRect.left) * 0.3;
        
        visible_items[visible_item_count].status_rect = visible_items[visible_item_count].text_rect;
        visible_items[visible_item_count].status_rect.top += 40;
        visible_items[visible_item_count].status_rect.bottom += 20;
        
        visible_items[visible_item_count].is_visible = true;
        visible_item_count++;
    }
}
// create scroll bar for conversation 
HWND ConversationScrollBar;
RECT ConversationScrollBarRect;
// for calculating the thumb is rect
void CalculateConversationThumbRect(HWND hwnd, RECT* thumb_rect,RECT WindowSize)
{
    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    float scrollbar_height = (client_rect.bottom - client_rect.top - 4);
    float range = Conversation_thumb.max_val - Conversation_thumb.min_val;
    if(range <= 0) range = 1;
    float thumb_height = max(20, (Conversation_thumb.page_size * scrollbar_height) / (range + Conversation_thumb.page_size));
    float track_height = scrollbar_height - thumb_height;
    int thumb_pos = 0;
    if(range > 0)
    {
        thumb_pos = (Conversation_thumb.current_val * track_height) / range;
    }
    thumb_rect->left = client_rect.right - SCROLLBAR_WIDTH - 2;
    thumb_rect->top = thumb_pos;
    thumb_rect->right = client_rect.right - 2;
    thumb_rect->bottom = thumb_rect->top + thumb_height;
}
// for drawing the thumb 
void DrawConversationScrollBar(HDC hdc, HWND hwnd,RECT WindowSize)
{
    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    RECT track_rect = {
        client_rect.right - SCROLLBAR_WIDTH - 2,
        0,
        client_rect.right - 2,
        client_rect.bottom
    };
    HBRUSH track_brush = CreateSolidBrush(TRACK_COLOR);
    FillRect(hdc, &track_rect, track_brush);
    DeleteObject(track_brush);  
    CalculateConversationThumbRect(hwnd,&Conversation_thumb.thumb_rect,WindowSize);    
    COLORREF thumb_color = THUMB_COLOR;
    if(Conversation_thumb.thumb_pressed)
    {
        thumb_color = THUMB_PRESSED_COLOR;
    }
    else if(Conversation_thumb.thumb_hover)
    {
        thumb_color = THUMB_HOVER_COLOR;
    }
    
    HBRUSH thumb_brush = CreateSolidBrush(thumb_color);
    
    HPEN old_pen = SelectObject(hdc, CreatePen(PS_SOLID, 1, thumb_color));
    HBRUSH old_brush = SelectObject(hdc, thumb_brush);
    
    RoundRect(hdc, 
    Conversation_thumb.thumb_rect.left + 2, 
    Conversation_thumb.thumb_rect.top,
    Conversation_thumb.thumb_rect.right - 2, 
    Conversation_thumb.thumb_rect.bottom,
    6, 6);
    float ConversationHeight = ((WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.09) - (PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.074));
    MoveWindow(hwnd,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2877,
    PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.072,
    WindowSize.right - (Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2877),
    ConversationHeight,TRUE);
    DeleteObject(SelectObject(hdc, old_pen));
    SelectObject(hdc, old_brush);
    DeleteObject(thumb_brush);
}
// updating the thumb is place in the scrollbar 
void UpdateConversationScrollValue(HWND hwnd, float new_val)
{
    new_val = max(Conversation_thumb.min_val, min(Conversation_thumb.max_val, new_val));
    if (new_val != Conversation_thumb.current_val)
    {
        Conversation_thumb.current_val = new_val;
    }
}
int Conversation_total_items;
float Conversation_window_height;
int Conversation_items_per_page;
extern int i;
void UpdateConversationScrollbarRange(HWND hwnd,RECT ConversationScrollBarRect,ScrollbarInfo *Conversation_thumb,int Message_Count)
{
    GetClientRect(hwnd,&ConversationScrollBarRect);
    Conversation_total_items = Message_Count;
    Conversation_window_height = (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top) - 4;
    Conversation_items_per_page = 5;
    Conversation_thumb->min_val = 0;
    if(Conversation_total_items <= Conversation_items_per_page)
    {
        Conversation_thumb->max_val = 0; 
        Conversation_thumb->page_size = Conversation_items_per_page;
    } 
    else
    {
        Conversation_thumb->max_val = Conversation_total_items - Conversation_items_per_page + 1;
        Conversation_thumb->page_size = Conversation_items_per_page;
    }   
}
