    #include <windows.h>
    #include <stdbool.h>
    #include <ctype.h>
    #define MAX_BUTTON_WIDTHP 180
    #define MAX_BUTTON_HEIGHTP 72
    #define MIN_BUTTON_WIDTHP 135
    #define MIN_BUTTON_HEIGHTP 42

    // UI for message 
    bool UiMessage=FALSE;
    // Ui for Schedule
    bool UiSchedule;
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
    int UiInboxTimes,UiGeneralTimes;
    // when the button general is clicked
    bool UiGeneral = FALSE;
    // changing Choice 
    bool ResetChoice;
    bool UiGeneralResetChoice;
    // for the inbox ui
    RECT ChatRect;
    HWND HandleSearch;
    // create the child window of scrollbar (UiInbox)
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
    //int choseentype;
    //
    ScrollbarInfo Conversation_thumb;
    ScrollbarInfo UiGeneralConversation_thumb;
    void CalculateThumbRect(HWND hwnd, RECT* thumb_rect,RECT WindowSize);
    void DrawScrollBar(HDC hdc, HWND hwnd,RECT WindowSize);
    BOOL PointInRect(POINT pt, RECT* rect);
    void UpdateScrollValue(HWND hwnd, float new_val);
    void UpdateScrollbarRange(HWND hwnd,RECT ScrollBarRect,ScrollbarInfo *g_scrollbar);
    void DrawContentWithScroll(HDC Mdc_Child_1, HWND hwnd,RECT ScrollBarRect,Clients Message[100],ScrollbarInfo *g_scrollbar);
    void UiInboxConversation(HWND HandleWnd,HDC Mdc,CntTrd ConnectingTools,RECT WindowSize,RECT PanelRect,float CurrentHEmoji,float CurrentVEmoji,float CurrentHAttach,float CurrentVAttach,float CurrentHSend,float CurrentVSend);
    int GetClickedRecipient(POINT click_point);
    void BuildVisibleItemsList(HDC hdc, HWND hwnd, RECT ScrollBarRect, Clients Message[], ScrollbarInfo *g_scrollbar);
    void RenderingConversationMessage(HWND hwnd,HWND HandleWnd,HDC Mdc,const char * Recipient,bool FontSize);
    bool HasOnlyWhitespace(HWND hwndEdit);
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
    int range = g_scrollbar.max_val - g_scrollbar.min_val;   
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
void DrawScrollBar(HDC hdc, HWND hwnd,RECT WindowSize)
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
    for(int i = 0; i < visible_item_count; i++)
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
void CalculateConversationThumbRect(HWND hwnd, RECT* thumb_rect,RECT WindowSize,float HeightIncrementationChecking)
{
    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    float scrollbar_height = (client_rect.bottom - client_rect.top);
    int range = Conversation_thumb.max_val - Conversation_thumb.min_val;
    if(range <= 0) range = 1;
    float thumb_height = max(20, (Conversation_thumb.page_size * scrollbar_height) / (range + Conversation_thumb.page_size)) ;
    float track_height = scrollbar_height;
    int thumb_pos = 0;
    if(range > 0)
    {
        if(Conversation_thumb.current_val >= Conversation_thumb.max_val)
        {
            thumb_pos = track_height;
        }
        else
        {
            thumb_pos = (Conversation_thumb.current_val * track_height) / range;
        }
    }
    if(thumb_pos + thumb_height > scrollbar_height)
    {
        thumb_pos = scrollbar_height - thumb_height;
    }
    if(HeightIncrementationChecking > (client_rect.bottom - client_rect.top))
    {
        thumb_rect->left = client_rect.right - SCROLLBAR_WIDTH - 2;
        thumb_rect->top = thumb_pos;
        thumb_rect->right = client_rect.right - 2;
        thumb_rect->bottom = thumb_rect->top + thumb_height;
    }
}
// for drawing the thumb 
void DrawConversationScrollBar(HDC Mdc, HWND hwnd,RECT WindowSize,float HeightIncrementationChecking)
{
    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    RECT track_rect = {
        client_rect.right - SCROLLBAR_WIDTH - 2,
        0,
        client_rect.right - 2,
        client_rect.bottom
    };
    HBRUSH track_brush;
    if(HeightIncrementationChecking > (client_rect.bottom - client_rect.top))
    {
        track_brush = CreateSolidBrush(TRACK_COLOR);
    }
    else
    {
        track_brush = CreateSolidBrush(RGB(250,245,230));
    }
    FillRect(Mdc, &track_rect, track_brush);
    DeleteObject(track_brush);  
    CalculateConversationThumbRect(hwnd,&Conversation_thumb.thumb_rect,WindowSize,HeightIncrementationChecking);    
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
    HBRUSH old_brush = SelectObject(Mdc, thumb_brush);
    HPEN old_pen = SelectObject(Mdc, CreatePen(PS_SOLID, 1, thumb_color));
    if(HeightIncrementationChecking > (client_rect.bottom - client_rect.top))
    {
        RoundRect(Mdc, 
        Conversation_thumb.thumb_rect.left + 2, 
        Conversation_thumb.thumb_rect.top,
        Conversation_thumb.thumb_rect.right - 2, 
        Conversation_thumb.thumb_rect.bottom,
        6, 6);
    }
    float ConversationHeight = ((WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.09) - (PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.074)) - (WindowSize.right-WindowSize.left)*0.007;
    MoveWindow(hwnd,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2877,
    PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.072,
    WindowSize.right - (Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2877),
    ConversationHeight,TRUE);
    DeleteObject(SelectObject(Mdc, old_pen));
    SelectObject(Mdc, old_brush);
    DeleteObject(thumb_brush);
}

// updating the thumb is place in the scrollbar 
void UpdateConversationScrollValue(HWND hwnd, float new_val)
{
    new_val = max(Conversation_thumb.min_val, min(Conversation_thumb.max_val, new_val));
    if (new_val != Conversation_thumb.current_val)
    {
        Conversation_thumb.current_val = new_val;
        if(Conversation_thumb.current_val == Conversation_thumb.max_val)
        {
            Conversation_scrolloffset = 0;
        }
    }
}
int Conversation_total_messages;
float Conversation_window_height;
int Conversation_messages_per_page;
extern int i;
void UpdateConversationScrollbarRange(HWND hwnd,RECT ConversationScrollBarRect,ScrollbarInfo *Conversation_thumb,int Message_Count,bool *NewRecipientFlag)
{
    GetClientRect(hwnd,&ConversationScrollBarRect);
    Conversation_total_messages = ((Message_Count < 11) ? 11 : Message_Count);
    Conversation_window_height = (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top) - 4;
    Conversation_messages_per_page = 11;
    Conversation_thumb->min_val = 0;
    Conversation_thumb->page_size = Conversation_messages_per_page;
    if(Conversation_total_messages <= Conversation_messages_per_page)
    {
        Conversation_thumb->max_val = 0; 
    } 
    else
    {
        Conversation_thumb->max_val = Conversation_total_messages - Conversation_messages_per_page;
        Conversation_thumb->max_val = Conversation_thumb->max_val;
    }
    if(*NewRecipientFlag)   
    {
        *NewRecipientFlag = FALSE;
        Conversation_scrolloffset = 0;
        Conversation_thumb->current_val = Conversation_thumb->max_val;
    }
}
// filtering messages and render them on the screen in function with the scroll bar and the size of the window and the font 
float Frontier;
float HeightIncrementationChecking;
void RenderingConversationMessage(HWND hwnd,HWND HandleWnd,HDC Mdc_Conversation_child,const char * Recipient,bool FontSize)
{
    // Safety check for null or empty recipient
    if(!Recipient || strlen(Recipient) == 0)
        return;
    float FontWidth = 8.5;
    float FontHeight = 40;
    RECT WndRect;
    GetClientRect(HandleWnd,&WndRect);
    GetClientRect(hwnd,&ConversationScrollBarRect);
    float MessagePosition = (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top);
    int CharactersPerLine,NumberOfLines;
    float MessageHeight;
    float AccumulatedMessagesHeight = 0;
    CharactersPerLine = (int)(((ConversationScrollBarRect.right - ConversationScrollBarRect.left) - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.5)/FontWidth);
    if(CharactersPerLine <= 0) CharactersPerLine = 1; // Prevent division by zero
    int check_message_left;
    HFONT Font;
    HFONT FontTimeStamp;
    HFONT OldFontTimeStamp;
    float TimeStamp_Height,TimeStamp_Width;
    if(FontSize)
    {
        Font=CreateFont(23,10,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
        FontTimeStamp = CreateFont(19,8,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
    }
    else 
    {
        Font=CreateFont(20,8,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
        FontTimeStamp = CreateFont(16,6,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
    }
    HFONT OldFont=SelectObject(Mdc_Conversation_child,Font);
    TimeStamp_Height = (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.11;
    TimeStamp_Width = (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.17;
    for(int i=0;i<countclient && i<40;i++)
    {
        if(strcmp(MessagesConversations[i].OwnerName,Recipient) == 0)
        {
            for(int k=0;k<MessagesConversations[i].count && k<100;k++)
            {
                SIZE textSize;
                GetTextExtentPoint32A(Mdc_Conversation_child,MessagesConversations[i].Conversation[k].message,strlen(MessagesConversations[i].Conversation[k].message),&textSize);
                int message_length = strlen(MessagesConversations[i].Conversation[k].message);
                NumberOfLines = max(1, message_length/ CharactersPerLine);
                MessageHeight = NumberOfLines * textSize.cy + (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.035;
                AccumulatedMessagesHeight += MessageHeight;
                if(AccumulatedMessagesHeight > Conversation_scrolloffset &&
                AccumulatedMessagesHeight < (Conversation_scrolloffset + ConversationScrollBarRect.bottom + MessageHeight))
                {
                    // this is for whether you can request new message or not 
                    check_message_left = k;
                    if(check_message_left == (MessagesConversations[i].count - 1))
                    {
                        RecipientPass[i].messages_left = FALSE;
                    }
                    else
                    {
                        RecipientPass[i].messages_left = TRUE;
                    }
                    MessagePosition -= MessageHeight;
                    Frontier = AccumulatedMessagesHeight;
                    if(strcmp(MessagesConversations[i].Conversation[k].owner,Recipient) == 0)
                    {
                        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
                        HPEN hOldPen = (HPEN)SelectObject(Mdc_Conversation_child, hPen);
                        HBRUSH RecipientBrush = CreateSolidBrush(RGB(210, 210, 210));
                        HBRUSH old_brush = SelectObject(Mdc_Conversation_child,RecipientBrush);
                        RECT RecipientMessagePositionRect;
                        float widthMessage = textSize.cx + (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.06;
                        if(widthMessage > (ConversationScrollBarRect.right /1.85))
                        {
                            widthMessage = ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.18);
                        }
                        RecipientMessagePositionRect.left = ConversationScrollBarRect.left + (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.035;
                        RecipientMessagePositionRect.top = MessagePosition;
                        RecipientMessagePositionRect.right =  widthMessage;
                        RecipientMessagePositionRect.bottom = RecipientMessagePositionRect.top + MessageHeight - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.011;
                        RoundRect(Mdc_Conversation_child,RecipientMessagePositionRect.left,RecipientMessagePositionRect.top,RecipientMessagePositionRect.right,RecipientMessagePositionRect.bottom,
                        (WndRect.right-WndRect.left)*0.0132,(WndRect.right-WndRect.left)*0.0132);
                        RecipientMessagePositionRect.left += (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.01;
                        RecipientMessagePositionRect.right -= (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.01;
                        DrawText(Mdc_Conversation_child,MessagesConversations[i].Conversation[k].message,-1,&RecipientMessagePositionRect,DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);
                        MessagePosition -= TimeStamp_Height;
                        RoundRect(Mdc_Conversation_child,((ConversationScrollBarRect.right - ConversationScrollBarRect.left) / 2) - (TimeStamp_Width / 2),RecipientMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.083,
                        ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) / 2) + (TimeStamp_Width / 2),
                        RecipientMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.023,(WndRect.right-WndRect.left)*0.01,(WndRect.right-WndRect.left)*0.01);
                        RECT TimeStampRect;
                        TimeStampRect.left = ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) / 2) - (TimeStamp_Width / 2);
                        TimeStampRect.top = RecipientMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.083;
                        TimeStampRect.right = ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) / 2) + (TimeStamp_Width / 2);
                        TimeStampRect.bottom = RecipientMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.023;
                        OldFontTimeStamp=SelectObject(Mdc_Conversation_child,FontTimeStamp);
                        char TimeStampBuffer[50];
                        strftime(TimeStampBuffer, sizeof(TimeStampBuffer), "%m-%d-%Y, %H:%M",&MessagesConversations[i].Conversation[k].TimeStamp);
                        DrawText(Mdc_Conversation_child,TimeStampBuffer,-1,&TimeStampRect,DT_CENTER | DT_SINGLELINE);
                        SelectObject(Mdc_Conversation_child, old_brush);
                        DeleteObject(RecipientBrush);
                        SelectObject(Mdc_Conversation_child, hOldPen);
                        DeleteObject(hPen);
                        SelectObject(Mdc_Conversation_child,OldFontTimeStamp);
                    }
                    else if(strcmp(MessagesConversations[i].Conversation[k].owner,SendingTools.username) == 0)
                    {
                        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(112, 146, 190));
                        HPEN hOldPen = (HPEN)SelectObject(Mdc_Conversation_child, hPen);
                        HBRUSH MyBrush = CreateSolidBrush(RGB(112, 146, 190));
                        HBRUSH old_brush = SelectObject(Mdc_Conversation_child,MyBrush);
                        RECT MyMessagePositionRect;
                        float widthMessage = textSize.cx;
                        if(widthMessage < (ConversationScrollBarRect.right /2.3))
                        {
                            widthMessage = ConversationScrollBarRect.right/2;
                        }
                        MyMessagePositionRect.left = (ConversationScrollBarRect.right - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.056  - textSize.cx);
                        if(MyMessagePositionRect.left < (ConversationScrollBarRect.right - ConversationScrollBarRect.left)/2.3)
                        {
                            MyMessagePositionRect.left = ConversationScrollBarRect.right - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.7;
                        }
                        MyMessagePositionRect.top = MessagePosition;
                        MyMessagePositionRect.right = ConversationScrollBarRect.right - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.035;
                        MyMessagePositionRect.bottom = MyMessagePositionRect.top + MessageHeight - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.005;
                        // Draw the bubble
                        RoundRect(Mdc_Conversation_child,MyMessagePositionRect.left,MyMessagePositionRect.top,MyMessagePositionRect.right,MyMessagePositionRect.bottom,
                        (WndRect.right-WndRect.left)*0.0132,(WndRect.right-WndRect.left)*0.0132);
                        MyMessagePositionRect.left += (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.01;
                        MyMessagePositionRect.right -= (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.008;
                        DrawText(Mdc_Conversation_child,MessagesConversations[i].Conversation[k].message,-1,&MyMessagePositionRect,DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);
                        hPen = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
                        hOldPen = (HPEN)SelectObject(Mdc_Conversation_child, hPen);
                        MyBrush = CreateSolidBrush(RGB(210, 210, 210));
                        old_brush = SelectObject(Mdc_Conversation_child,MyBrush);
                        MessagePosition -= TimeStamp_Height;
                        RoundRect(Mdc_Conversation_child,(ConversationScrollBarRect.right / 2) - (TimeStamp_Width / 2),MyMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.083,
                        (ConversationScrollBarRect.right / 2) + (TimeStamp_Width / 2),
                        MyMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.023,(WndRect.right-WndRect.left)*0.01,(WndRect.right-WndRect.left)*0.01);
                        RECT TimeStampRect;
                        OldFontTimeStamp=SelectObject(Mdc_Conversation_child,FontTimeStamp);
                        TimeStampRect.left = (ConversationScrollBarRect.right / 2) - (TimeStamp_Width / 2);
                        TimeStampRect.top = MyMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.083;
                        TimeStampRect.right = (ConversationScrollBarRect.right / 2) + (TimeStamp_Width / 2);
                        TimeStampRect.bottom = MyMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.023;
                        char TimeStampBuffer[50];
                        strftime(TimeStampBuffer, sizeof(TimeStampBuffer), "%m-%d-%Y, %H:%M",&MessagesConversations[i].Conversation[k].TimeStamp);
                        DrawText(Mdc_Conversation_child,TimeStampBuffer,-1,&TimeStampRect,DT_CENTER | DT_SINGLELINE);
                        SelectObject(Mdc_Conversation_child, old_brush);
                        DeleteObject(MyBrush);
                        SelectObject(Mdc_Conversation_child, hOldPen);
                        DeleteObject(hPen);
                        SelectObject(Mdc_Conversation_child,OldFontTimeStamp);
                    }
                    /*else 
                    {
                        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
                        HPEN hOldPen = (HPEN)SelectObject(Mdc_Conversation_child, hPen);
                        HBRUSH RecipientBrush = CreateSolidBrush(RGB(210, 210, 210));
                        HBRUSH old_brush = SelectObject(Mdc_Conversation_child,RecipientBrush);
                        RECT RecipientMessagePositionRect;
                        float widthMessage = textSize.cx + (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.06;
                        if(widthMessage > (ConversationScrollBarRect.right /1.72))
                        {
                            widthMessage = ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.18);
                        }
                        RecipientMessagePositionRect.left = ConversationScrollBarRect.left + (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.035;
                        RecipientMessagePositionRect.top = MessagePosition;
                        RecipientMessagePositionRect.right =  widthMessage;
                        RecipientMessagePositionRect.bottom = RecipientMessagePositionRect.top + MessageHeight - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.011;
                        RoundRect(Mdc_Conversation_child,RecipientMessagePositionRect.left,RecipientMessagePositionRect.top,RecipientMessagePositionRect.right,RecipientMessagePositionRect.bottom,
                        (WndRect.right-WndRect.left)*0.0175,(WndRect.right-WndRect.left)*0.0175);
                        RecipientMessagePositionRect.left += (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.01;
                        RecipientMessagePositionRect.right -= (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.01;
                        DrawText(Mdc_Conversation_child,MessagesConversations[i].Conversation[k].message,-1,&RecipientMessagePositionRect,DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);
                        SelectObject(Mdc_Conversation_child, old_brush);
                        DeleteObject(RecipientBrush);
                        SelectObject(Mdc_Conversation_child, hOldPen);
                        DeleteObject(hPen);
                    }*/
                }
            }
            break;
        }
    }
    SelectObject(Mdc_Conversation_child,OldFont);
    DeleteObject(Font);  
    SelectObject(Mdc_Conversation_child,OldFontTimeStamp);
    DeleteObject(FontTimeStamp);
}
bool HasOnlyWhitespace(HWND hwndEdit)
{
    int textLength = GetWindowTextLength(hwndEdit);
    if (textLength == 0)
    {
        return TRUE; 
    }
    char* buffer = malloc(textLength + 1);
    if (!buffer) return FALSE;
    GetWindowText(hwndEdit, buffer, textLength + 1);
    for (int i = 0; i < textLength; i++)
    {
        if (!isspace((unsigned char)buffer[i]))
        {
            free(buffer);
            return FALSE; 
        }
    }
    free(buffer);
    return TRUE; 
}
// this function it draw the converstion all the workers in the company (adding the feature in it which is using file of the conversation )
HWND UiGeneralMessageBarHandle;
void UiGeneralConversation(HWND HandleWnd,HDC Mdc,CntTrd ConnectingTools,RECT WindowSize,RECT PanelRect,float UiGeneralCurrentHEmoji,float UiGeneralCurrentVEmoji,float UiGeneralCurrentHAttach,float UiGeneralCurrentVAttach,float UiGeneralCurrentHSend,float UiGeneralCurrentVSend)
{
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    GetClientRect(HandleWnd,&WindowSize);   
    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.19,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.009,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.68,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.06,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);
    // emoji circle Button
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.19 - UiGeneralCurrentHEmoji,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0872 - UiGeneralCurrentVEmoji,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.22 + UiGeneralCurrentHEmoji,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0285 + UiGeneralCurrentVEmoji);
    // Attach Button
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.228 - UiGeneralCurrentHAttach,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0872 - UiGeneralCurrentVAttach,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.259 + UiGeneralCurrentHAttach,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0285 + UiGeneralCurrentVAttach);
    // send message button 
    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.6454 - UiGeneralCurrentHSend,
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0875 - UiGeneralCurrentVSend,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.68 + UiGeneralCurrentHSend,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.075 + 
    (WindowSize.bottom - WindowSize.top)*0.05 + UiGeneralCurrentVSend,
    (WindowSize.right - WindowSize.left)*0.0175,(WindowSize.bottom - WindowSize.top)*0.026); 
    SelectObject(Mdc,OldPen);
    DeleteObject(Pen);
    SelectObject(Mdc,OldButtonColor);
    DeleteObject(OldButtonColor);
    // rendering the recipient is name
    RECT RecipientRect;
    RecipientRect.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3;
    RecipientRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.022;
    RecipientRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.554;
    RecipientRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.05;
    HBRUSH ButtonColorRecipient = CreateSolidBrush(RGB(0,0,0));
    HBRUSH OldButtonColorRecipient = SelectObject(Mdc,ButtonColorRecipient);
    HFONT Font=CreateFont(23,11,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,"Segoe UI");
    HFONT OldFont=(HFONT)SelectObject(Mdc,Font);
    HPEN PenRecipient=CreatePen(PS_DOT,2,RGB(0,0,0));
    HPEN OldPenRecipient=SelectObject(Mdc,PenRecipient);
    DrawText(Mdc,"MonTech's General Chat",-1,&RecipientRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldFont);
    DeleteObject(Font);
    // Emoji form
    // left eye
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.197 - UiGeneralCurrentHEmoji/4.2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0633 - UiGeneralCurrentVEmoji/4.2,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.201 + UiGeneralCurrentHEmoji/4.2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.058 + UiGeneralCurrentVEmoji/4.2);
    // right eye
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2095 - UiGeneralCurrentHEmoji/4.2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0633 - UiGeneralCurrentVEmoji/4.2,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.21353 + UiGeneralCurrentHEmoji/4.2,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.058 + UiGeneralCurrentVEmoji/4.2);
    // smile 
    Arc(Mdc,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.197 - UiGeneralCurrentHEmoji/4.7,
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.054 - UiGeneralCurrentVEmoji/4.2,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.213 + UiGeneralCurrentHEmoji/4.2,     
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.04 + UiGeneralCurrentVEmoji/3,       
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.197 - UiGeneralCurrentHEmoji/4.2,     
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.048 + UiGeneralCurrentVEmoji/4.2,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.213 + UiGeneralCurrentHEmoji/4.2,
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.048 + UiGeneralCurrentVEmoji/4.2);
    // attach
    // first line
    MoveToEx(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2345 - UiGeneralCurrentHAttach/5.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0615 - UiGeneralCurrentVAttach/5.8,NULL);
    LineTo(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2515 + UiGeneralCurrentHAttach/5.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0615 + UiGeneralCurrentVAttach/5.8);
    // second line
    MoveToEx(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2345 - UiGeneralCurrentHAttach/5.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.05 - UiGeneralCurrentVAttach/5.8,NULL);
    LineTo(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2515 + UiGeneralCurrentHAttach/5.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.05 + UiGeneralCurrentVAttach/5.8);
    // first line is ellipse
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.23775 - UiGeneralCurrentHAttach/6.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.065 - UiGeneralCurrentVAttach/6.5,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2414 + UiGeneralCurrentHAttach/6.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0562 + UiGeneralCurrentVAttach/6.25);
    // second line is ellipse
    Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24578 - UiGeneralCurrentHAttach/6.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0539 - UiGeneralCurrentVAttach/6.5,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.2494 + UiGeneralCurrentHAttach/6.5,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.045 + UiGeneralCurrentVAttach/6.5);
    SelectObject(Mdc,OldButtonColorRecipient);
    DeleteObject(OldButtonColorRecipient);
    SelectObject(Mdc,OldPenRecipient);
    DeleteObject(PenRecipient);
    // search button where you can search in the conversation about some words or infos
    HBRUSH Gray = CreateSolidBrush(RGB(210, 210, 210));
    OldButtonColor=SelectObject(Mdc,Gray);
    HPEN penSend = CreatePen(PS_SOLID,3,RGB(0, 0, 0));
    HPEN oldPenSend = SelectObject(Mdc,penSend);
    /*Ellipse(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.364 + (WindowSize.right - WindowSize.left)*0.43,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.024,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.368 + (WindowSize.right - WindowSize.left)*0.447,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.0452);
    MoveToEx(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.364 + (WindowSize.right - WindowSize.left)*0.43085,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.04222,NULL);
    LineTo(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.364 + (WindowSize.right - WindowSize.left)*0.42605,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.04705);*/
    SelectObject(Mdc,OldButtonColor);
    DeleteObject(Gray);
    // bracket on button send 
    // down line 
    MoveToEx(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.6454 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.68) / 2),
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.071 - UiGeneralCurrentVSend/5,NULL);
    LineTo(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.6454 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.68) / 2),
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.095 + (WindowSize.bottom - WindowSize.top)*0.0505 + UiGeneralCurrentVSend/5);
    // left line
    MoveToEx(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.6454 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.68) / 2),
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.071 - UiGeneralCurrentVSend/5,NULL);
    LineTo(Mdc,((((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.6454 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.68))/ 2) - (WindowSize.right-WindowSize.left)*0.008),
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.059 - UiGeneralCurrentVSend/5);
    // right line 
    MoveToEx(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.6454 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.68) / 2),
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.071 - UiGeneralCurrentVSend/5,NULL);
    LineTo(Mdc,((((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.6454 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.68))/ 2) + (WindowSize.right-WindowSize.left)*0.008),
    WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.059 - UiGeneralCurrentVSend/5);
    SelectObject(Mdc,oldPenSend);
    DeleteObject(penSend);
    SelectObject(Mdc,OldButtonColorRecipient);
    DeleteObject(OldButtonColorRecipient);
}
//             UiGeneral
// filtering messages and render them on the screen in function with the scroll bar and the size of the window and the font 
float UiGeneralFrontier;
float UiGeneralHeightIncrementationChecking;
void UiGeneralRenderingConversationMessage(HWND hwnd,HWND HandleWnd,HDC Mdc_UiGeneralConversation_child,bool FontSize)
{
    float FontWidth = 8.5;
    float FontHeight = 40;
    RECT WndRect;
    GetClientRect(HandleWnd,&WndRect);
    GetClientRect(hwnd,&ConversationScrollBarRect);
    float MessagePosition = (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top) + UiGeneralConversation_scrolloffset;
    int CharactersPerLine,NumberOfLines;
    float MessageHeight;
    float UiGeneralAccumulatedMessagesHeight = 0;
    CharactersPerLine = (int)(((ConversationScrollBarRect.right - ConversationScrollBarRect.left) - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.5)/FontWidth);
    if(CharactersPerLine <= 0) CharactersPerLine = 1; // Prevent division by zero
    int check_message_left;
    HFONT Font;
    HFONT FontTimeStamp;
    HFONT OldFontTimeStamp;
    float TimeStamp_Height,TimeStamp_Width;
    int WidthFirst,HeightFirst;
    if(FontSize)
    {
        HeightFirst = 23;
        WidthFirst = 10;
        Font=CreateFont(23,10,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
        FontTimeStamp = CreateFont(19,8,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
    }
    else 
    {
        HeightFirst = 20;
        WidthFirst = 8;
        Font=CreateFont(20,8,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
        FontTimeStamp = CreateFont(16,6,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
    }
    HFONT OldFont=SelectObject(Mdc_UiGeneralConversation_child,Font);
    TimeStamp_Height = (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.11;
    TimeStamp_Width = (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.17;
    for(int k=0;k<GeneralChatConversation.count && k<200;k++)
    {
        SIZE textSize;
        SIZE UsernameSize;
        GetTextExtentPoint32A(Mdc_UiGeneralConversation_child,GeneralChatConversation.Conversation[k].message,strlen(GeneralChatConversation.Conversation[k].message),&textSize);
        int message_length = strlen(GeneralChatConversation.Conversation[k].message);
        NumberOfLines = max(1, message_length/ CharactersPerLine);
        MessageHeight = NumberOfLines * textSize.cy + (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.035;
        if(MessagePosition >= 0 && MessagePosition <= (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top))
        {
            // this is for whether you can request new message or not 
            check_message_left = k;
            if(check_message_left == (GeneralChatConversation.count - 1))
            {
                UiGeneralRequestSettingPass.messages_left = FALSE;
            }
            else
            {
                UiGeneralRequestSettingPass.messages_left = TRUE;
            }
            UiGeneralFrontier = UiGeneralAccumulatedMessagesHeight;
            if(strcmp(GeneralChatConversation.Conversation[k].owner,SendingTools.username) == 0)
            {
                MessagePosition -= MessageHeight;
                UiGeneralAccumulatedMessagesHeight += MessageHeight;
                HPEN hPen = CreatePen(PS_SOLID, 1, RGB(112, 146, 190));
                HPEN hOldPen = (HPEN)SelectObject(Mdc_UiGeneralConversation_child, hPen);
                HBRUSH MyBrush = CreateSolidBrush(RGB(112, 146, 190));
                HBRUSH old_brush = SelectObject(Mdc_UiGeneralConversation_child,MyBrush);
                RECT MyMessagePositionRect;
                float widthMessage = textSize.cx;
                if(widthMessage < (ConversationScrollBarRect.right /2.3))
                {
                    widthMessage = ConversationScrollBarRect.right/2;
                }
                MyMessagePositionRect.left = (ConversationScrollBarRect.right - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.056  - textSize.cx);
                if(MyMessagePositionRect.left < (ConversationScrollBarRect.right - ConversationScrollBarRect.left)/2.3)
                {
                    MyMessagePositionRect.left = ConversationScrollBarRect.right - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.7;
                }
                MyMessagePositionRect.top = MessagePosition;
                MyMessagePositionRect.right = ConversationScrollBarRect.right - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.035;
                MyMessagePositionRect.bottom = MyMessagePositionRect.top + MessageHeight - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.005;
                // Draw the bubble
                RoundRect(Mdc_UiGeneralConversation_child,MyMessagePositionRect.left,MyMessagePositionRect.top,MyMessagePositionRect.right,MyMessagePositionRect.bottom,
                (WndRect.right-WndRect.left)*0.0132,(WndRect.right-WndRect.left)*0.0132);
                MyMessagePositionRect.left += (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.01;
                MyMessagePositionRect.right -= (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.008;
                DrawText(Mdc_UiGeneralConversation_child,GeneralChatConversation.Conversation[k].message,-1,&MyMessagePositionRect,DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);
                printf("those are the message of me : %s\n",GeneralChatConversation.Conversation[k].message);
                hPen = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
                hOldPen = (HPEN)SelectObject(Mdc_UiGeneralConversation_child, hPen);
                MyBrush = CreateSolidBrush(RGB(210, 210, 210));
                old_brush = SelectObject(Mdc_UiGeneralConversation_child,MyBrush);
                MessagePosition -= TimeStamp_Height;
                RoundRect(Mdc_UiGeneralConversation_child,(ConversationScrollBarRect.right / 2) - (TimeStamp_Width / 2),MyMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.083,
                (ConversationScrollBarRect.right / 2) + (TimeStamp_Width / 2),
                MyMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.023,(WndRect.right-WndRect.left)*0.01,(WndRect.right-WndRect.left)*0.01);
                RECT TimeStampRect;
                OldFontTimeStamp=SelectObject(Mdc_UiGeneralConversation_child,FontTimeStamp);
                TimeStampRect.left = (ConversationScrollBarRect.right / 2) - (TimeStamp_Width / 2);
                TimeStampRect.top = MyMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.083;
                TimeStampRect.right = (ConversationScrollBarRect.right / 2) + (TimeStamp_Width / 2);
                TimeStampRect.bottom = MyMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.023;
                char TimeStampBuffer[50];
                strftime(TimeStampBuffer, sizeof(TimeStampBuffer), "%m-%d-%Y, %H:%M",&GeneralChatConversation.Conversation[k].TimeStamp);
                DrawText(Mdc_UiGeneralConversation_child,TimeStampBuffer,-1,&TimeStampRect,DT_CENTER | DT_SINGLELINE);
                SelectObject(Mdc_UiGeneralConversation_child, old_brush);
                DeleteObject(MyBrush);
                SelectObject(Mdc_UiGeneralConversation_child, hOldPen);
                DeleteObject(hPen);
                SelectObject(Mdc_UiGeneralConversation_child,OldFontTimeStamp);
            }
            else
            {
                // Create username font to measure its actual size
                HFONT TempUsernameFont = CreateFont(
                    HeightFirst - 3, WidthFirst - 1, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial")
                );
                HFONT OldTempFont = (HFONT)SelectObject(Mdc_UiGeneralConversation_child, TempUsernameFont);
                GetTextExtentPoint32A(Mdc_UiGeneralConversation_child,GeneralChatConversation.Conversation[k].owner,strlen(GeneralChatConversation.Conversation[k].owner),&UsernameSize);
                SelectObject(Mdc_UiGeneralConversation_child, OldTempFont);
                DeleteObject(TempUsernameFont);
                MessagePosition -= (MessageHeight + (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.08);
                UiGeneralAccumulatedMessagesHeight += (MessageHeight + (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.08);
                HPEN hPen = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
                HPEN hOldPen = (HPEN)SelectObject(Mdc_UiGeneralConversation_child, hPen);
                HBRUSH RecipientBrush = CreateSolidBrush(RGB(210, 210, 210));
                HBRUSH old_brush = SelectObject(Mdc_UiGeneralConversation_child,RecipientBrush);
                RECT RecipientMessagePositionRect;
                // Calculate message width, but ensure it's at least as wide as the username
                float widthMessage = textSize.cx + (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.051;
                float usernameWidth = UsernameSize.cx + (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.051;
                if(widthMessage < usernameWidth)
                {
                    widthMessage = usernameWidth; // Expand bubble to fit username
                }
                float maxBubbleWidth = ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.18);
                if(widthMessage > maxBubbleWidth)
                {
                    widthMessage = maxBubbleWidth;
                }
                if(widthMessage < 10) widthMessage = 10; // safety minimum
                RecipientMessagePositionRect.left = ConversationScrollBarRect.left + (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.035;
                RecipientMessagePositionRect.top = MessagePosition;
                RecipientMessagePositionRect.right =  RecipientMessagePositionRect.left + widthMessage;
                RecipientMessagePositionRect.bottom = (RecipientMessagePositionRect.top + (MessageHeight + (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.08) - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.011);
                RoundRect(Mdc_UiGeneralConversation_child,RecipientMessagePositionRect.left,RecipientMessagePositionRect.top,RecipientMessagePositionRect.right,RecipientMessagePositionRect.bottom,
                (WndRect.right-WndRect.left)*0.0132,(WndRect.right-WndRect.left)*0.0132);
                RecipientMessagePositionRect.left += (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.01;
                RecipientMessagePositionRect.right -= (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.04;
                // Draw username with color
                RECT UsernameRect = RecipientMessagePositionRect;
                UsernameRect.bottom = UsernameRect.top + (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.06;
                COLORREF oldTextColor = GetTextColor(Mdc_UiGeneralConversation_child);
                HFONT UsernameFont = CreateFont(
                    HeightFirst - 3, // Smaller font size
                    WidthFirst - 1, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial")
                );
                HFONT OldUsernameFont = (HFONT)SelectObject(Mdc_UiGeneralConversation_child, UsernameFont);
                for(int i=0;i<countclient && i<40;i++)
                {
                    if(strcmp(MessagesConversations[i].OwnerName,GeneralChatConversation.Conversation[k].owner) == 0)
                    {
                        RGB ColorRgbValue = ColorRgb(MessagesConversations[i].color);
                        SetTextColor(Mdc_UiGeneralConversation_child, RGB(ColorRgbValue.r,ColorRgbValue.g,ColorRgbValue.b));
                        break;
                    }
                }
                DrawText(Mdc_UiGeneralConversation_child,GeneralChatConversation.Conversation[k].owner,-1,&UsernameRect,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);
                // Restore original font and text color
                SelectObject(Mdc_UiGeneralConversation_child, OldUsernameFont);
                DeleteObject(UsernameFont);
                SetTextColor(Mdc_UiGeneralConversation_child, oldTextColor);   
                // Draw message content
                RECT MessageRect;
                MessageRect.left = RecipientMessagePositionRect.left;
                MessageRect.top = RecipientMessagePositionRect.top + (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.05;
                MessageRect.right = RecipientMessagePositionRect.right;
                MessageRect.bottom = RecipientMessagePositionRect.bottom;
                hPen = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
                hOldPen = (HPEN)SelectObject(Mdc_UiGeneralConversation_child, hPen);
                DrawText(Mdc_UiGeneralConversation_child,GeneralChatConversation.Conversation[k].message,-1,&MessageRect,DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);
                printf("those are the message of my co-worker : %s\n",GeneralChatConversation.Conversation[k].message);
                MessagePosition -= TimeStamp_Height;
                RoundRect(Mdc_UiGeneralConversation_child,((ConversationScrollBarRect.right - ConversationScrollBarRect.left) / 2) - (TimeStamp_Width / 2),RecipientMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.083,
                ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) / 2) + (TimeStamp_Width / 2),
                RecipientMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.023,(WndRect.right-WndRect.left)*0.01,(WndRect.right-WndRect.left)*0.01);
                RECT TimeStampRect;
                TimeStampRect.left = ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) / 2) - (TimeStamp_Width / 2);
                TimeStampRect.top = RecipientMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.083;
                TimeStampRect.right = ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) / 2) + (TimeStamp_Width / 2);
                TimeStampRect.bottom = RecipientMessagePositionRect.top - (ConversationScrollBarRect.bottom - ConversationScrollBarRect.top)*0.023;
                OldFontTimeStamp=SelectObject(Mdc_UiGeneralConversation_child,FontTimeStamp);
                char TimeStampBuffer[50];
                strftime(TimeStampBuffer, sizeof(TimeStampBuffer), "%m-%d-%Y, %H:%M",&GeneralChatConversation.Conversation[k].TimeStamp);
                DrawText(Mdc_UiGeneralConversation_child,TimeStampBuffer,-1,&TimeStampRect,DT_CENTER | DT_SINGLELINE);
                SelectObject(Mdc_UiGeneralConversation_child, old_brush);
                DeleteObject(RecipientBrush);
                SelectObject(Mdc_UiGeneralConversation_child, hOldPen);
                DeleteObject(hPen);
                SelectObject(Mdc_UiGeneralConversation_child,OldFontTimeStamp);
            }
            /*else 
            {
                HPEN hPen = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
                HPEN hOldPen = (HPEN)SelectObject(Mdc_Conversation_child, hPen);
                HBRUSH RecipientBrush = CreateSolidBrush(RGB(210, 210, 210));
                HBRUSH old_brush = SelectObject(Mdc_Conversation_child,RecipientBrush);
                RECT RecipientMessagePositionRect;
                float widthMessage = textSize.cx + (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.06;
                if(widthMessage > (ConversationScrollBarRect.right /1.72))
                {
                    widthMessage = ((ConversationScrollBarRect.right - ConversationScrollBarRect.left) - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.18);
                }
                RecipientMessagePositionRect.left = ConversationScrollBarRect.left + (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.035;
                RecipientMessagePositionRect.top = MessagePosition;
                RecipientMessagePositionRect.right =  widthMessage;
                RecipientMessagePositionRect.bottom = RecipientMessagePositionRect.top + MessageHeight - (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.011;
                RoundRect(Mdc_Conversation_child,RecipientMessagePositionRect.left,RecipientMessagePositionRect.top,RecipientMessagePositionRect.right,RecipientMessagePositionRect.bottom,
                (WndRect.right-WndRect.left)*0.0175,(WndRect.right-WndRect.left)*0.0175);
                RecipientMessagePositionRect.left += (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.01;
                RecipientMessagePositionRect.right -= (ConversationScrollBarRect.right - ConversationScrollBarRect.left)*0.01;
                DrawText(Mdc_Conversation_child,GeneralChatConversation.Conversation[k].message,-1,&RecipientMessagePositionRect,DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);
                SelectObject(Mdc_Conversation_child, old_brush);
                DeleteObject(RecipientBrush);
                SelectObject(Mdc_Conversation_child, hOldPen);
                DeleteObject(hPen);
            }*/
        }
    }
    SelectObject(Mdc_UiGeneralConversation_child,OldFont);
    DeleteObject(Font);  
    SelectObject(Mdc_UiGeneralConversation_child,OldFontTimeStamp);
    DeleteObject(FontTimeStamp);
}
RECT UiGeneralConversationScrollBarRect;
// for calculating the thumb is rect 
void UiGeneralCalculateConversationThumbRect(HWND hwnd, RECT* thumb_rect,RECT WindowSize,float UiGeneralHeightIncrementationChecking)
{
    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    float scrollbar_height = (client_rect.bottom - client_rect.top );
    int range = UiGeneralConversation_thumb.max_val - UiGeneralConversation_thumb.min_val;
    if(range <= 0) range = 1;
    float thumb_height = max(20, (UiGeneralConversation_thumb.page_size * scrollbar_height) / (range + UiGeneralConversation_thumb.page_size)) ;
    float track_height = scrollbar_height - thumb_height;
    int thumb_pos = 0;
    if(range > 0)
    {
        if(UiGeneralConversation_thumb.current_val >= UiGeneralConversation_thumb.max_val)
        {
            thumb_pos = track_height;
        }
        else
        {
            thumb_pos = (UiGeneralConversation_thumb.current_val * track_height) / range;
        }
    }
    if(thumb_pos + thumb_height > scrollbar_height)
    {
        thumb_pos = scrollbar_height - thumb_height;
    }
    if(UiGeneralHeightIncrementationChecking > (client_rect.bottom - client_rect.top))
    {
      thumb_rect->left = client_rect.right - SCROLLBAR_WIDTH - 2;
      thumb_rect->top = thumb_pos;
      thumb_rect->right = client_rect.right - 2;
      thumb_rect->bottom = thumb_rect->top + thumb_height;
    }
}
// for drawing the thumb 
void UiGeneralDrawConversationScrollBar(HDC Mdc, HWND hwnd,RECT WindowSize,float UiGeneralHeightIncrementationChecking)
{
    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    RECT track_rect = {
        client_rect.right - SCROLLBAR_WIDTH - 2,
        0,
        client_rect.right - 2,
        client_rect.bottom
    };
    HBRUSH track_brush;
    if(UiGeneralHeightIncrementationChecking > (client_rect.bottom - client_rect.top))
    {
        track_brush = CreateSolidBrush(TRACK_COLOR);
    }
    else
    {
        track_brush = CreateSolidBrush(RGB(247, 240, 217));
    }
    FillRect(Mdc, &track_rect, track_brush);
    DeleteObject(track_brush);  
    UiGeneralCalculateConversationThumbRect(hwnd,&UiGeneralConversation_thumb.thumb_rect,WindowSize,UiGeneralHeightIncrementationChecking);    
    COLORREF thumb_color = THUMB_COLOR;
    if(UiGeneralConversation_thumb.thumb_pressed)
    {
        thumb_color = THUMB_PRESSED_COLOR;
    }
    else if(UiGeneralConversation_thumb.thumb_hover)
    {
        thumb_color = THUMB_HOVER_COLOR;
    }
    
    HBRUSH thumb_brush = CreateSolidBrush(thumb_color);
    HBRUSH old_brush = SelectObject(Mdc, thumb_brush);
    HPEN old_pen = SelectObject(Mdc, CreatePen(PS_SOLID, 1, thumb_color));
    if(UiGeneralHeightIncrementationChecking > (client_rect.bottom - client_rect.top))
    {
    RoundRect(Mdc, 
    UiGeneralConversation_thumb.thumb_rect.left + 2, 
    UiGeneralConversation_thumb.thumb_rect.top,
    UiGeneralConversation_thumb.thumb_rect.right - 2, 
    UiGeneralConversation_thumb.thumb_rect.bottom,
    6, 6);
    }
    float ConversationHeight = ((WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.09) - (PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.074)) - (WindowSize.right-WindowSize.left)*0.007;
    DeleteObject(SelectObject(Mdc, old_pen));
    SelectObject(Mdc, old_brush);
    DeleteObject(thumb_brush);
}
// updating the thumb is place in the scrollbar 
void UiGeneralUpdateConversationScrollValue(HWND hwnd, float new_val)
{
    new_val = max(UiGeneralConversation_thumb.min_val, min(UiGeneralConversation_thumb.max_val, new_val));
    if (new_val != UiGeneralConversation_thumb.current_val)
    {
        UiGeneralConversation_thumb.current_val = new_val;
        if(UiGeneralConversation_thumb.current_val == UiGeneralConversation_thumb.max_val)
        {
            UiGeneralConversation_scrolloffset = 0;
        }
    }
}
int UiGeneralConversation_total_messages;
float UiGeneralConversation_window_height;
int UiGeneralConversation_messages_per_page;
void UiGeneralUpdateConversationScrollbarRange(HWND hwnd,RECT UiGeneralConversationScrollBarRect,ScrollbarInfo *UiGeneralConversation_thumb,int Message_Count,bool *UiGeneralNewFlag)
{
    GetClientRect(hwnd,&UiGeneralConversationScrollBarRect);
    UiGeneralConversation_total_messages = ((Message_Count < 11) ? 11 : Message_Count);
    UiGeneralConversation_window_height = (UiGeneralConversationScrollBarRect.bottom - UiGeneralConversationScrollBarRect.top) - 4;
    UiGeneralConversation_messages_per_page = 11;
    UiGeneralConversation_thumb->min_val = 0;
    UiGeneralConversation_thumb->page_size = UiGeneralConversation_messages_per_page;
    if(UiGeneralConversation_total_messages <= UiGeneralConversation_messages_per_page)
    {
        UiGeneralConversation_thumb->max_val = 0; 
    } 
    else
    {
        UiGeneralConversation_thumb->max_val = UiGeneralConversation_total_messages - UiGeneralConversation_messages_per_page;
        UiGeneralConversation_thumb->max_val = UiGeneralConversation_thumb->max_val;
    }   
    if(*UiGeneralNewFlag)
    {
        *UiGeneralNewFlag = FALSE;
        UiGeneralConversation_scrolloffset = 0;
        UiGeneralConversation_thumb->current_val = UiGeneralConversation_thumb->max_val;
    }
}
// UiSchedule 
void ScheduleUi(HDC Mdc,HWND HandleWnd,RECT WindowSize,bool FontSize)
{
    GetClientRect(HandleWnd,&WindowSize);
    HPEN Pen=CreatePen(BS_SOLID,2,RGB(210,210,210));
    HPEN OldPen=SelectObject(Mdc,Pen);
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
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    HFONT OldFont=SelectObject(Mdc,Font);
    RECT RoundedRec;
    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.009,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.63,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.06,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);
    RoundedRec.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3;
    RoundedRec.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.02;
    RoundedRec.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.546;
    RoundedRec.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.046;
    DrawText(Mdc,"This Week",-1,&RoundedRec,DT_SINGLELINE | DT_CENTER);
    Pen=CreatePen(BS_SOLID,2,RGB(180, 180, 190));
    OldPen=SelectObject(Mdc,Pen);
    MoveToEx(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.09,NULL);
    LineTo(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.39);
    Pen=CreatePen(BS_SOLID,2,RGB(244,238,210));
    OldPen=SelectObject(Mdc,Pen);
    ButtonColor = CreateSolidBrush(RGB(244,238,210));
    OldButtonColor = SelectObject(Mdc,ButtonColor);
    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.04,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.095,
    ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.15,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);

    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.04,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.17,
    ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.225,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);

    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.04,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.245,
    ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.3,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);

    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.04,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.32,
    ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.375,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);


    RoundRect(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.132,
    WindowSize.right - (WindowSize.right-WindowSize.left)*0.02,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.187,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);

    RoundRect(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.207,
    WindowSize.right - (WindowSize.right-WindowSize.left)*0.02,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.262,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);
    
    RoundRect(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.282,
    WindowSize.right - (WindowSize.right-WindowSize.left)*0.02,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.337,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);
    SelectObject(Mdc,OldPen);
    DeleteObject(Pen);
    RECT ShiftDaysRect,ShiftTimeRect;
    ShiftDaysRect.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.05;
    ShiftDaysRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.15;
    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.11;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.132;
    ShiftTimeRect = ShiftDaysRect;
    DrawText(Mdc,"Monday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.left = ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.24);
    ShiftTimeRect.right = ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02);
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.1105;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);

    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.185;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.207;
    DrawText(Mdc,"Tuesday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.185;
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.1855;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.207;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);
    
    ShiftDaysRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.17;
    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.26;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.282;
    DrawText(Mdc,"Wednesday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftDaysRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.15;
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.2605;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.282;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);

    ShiftDaysRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.15;
    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.335;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.357;
    DrawText(Mdc,"Thursday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.3355;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.357;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);


    ShiftDaysRect.left = ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.04);
    ShiftDaysRect.right = ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.13);
    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.147;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.169;
    DrawText(Mdc,"Friday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.left = WindowSize.right - (WindowSize.right-WindowSize.left)*0.26;
    ShiftTimeRect.right = WindowSize.right - (WindowSize.right-WindowSize.left)*0.05;
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.1475;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.169;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);

    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.222;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.244;
    DrawText(Mdc,"Saturday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.2225;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.244;
    DrawText(Mdc,"Day off",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);

    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.297;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.319;
    DrawText(Mdc,"Sunday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.2975;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.319;
    DrawText(Mdc,"Day off",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);
}