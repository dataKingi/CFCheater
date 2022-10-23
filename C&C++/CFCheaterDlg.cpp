
// CFCheaterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CFCheater.h"
#include "CFCheaterDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <stdio.h>
#include <atlstr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

LONG WindowHandle;//窗口句柄
LONG WindowBaseAddress;//窗口基址
LONG bdxBaseAddress;//不掉血基址
LONG tsBaseAddress;//透视基址crossfire.exe+43AD44 crossfire.exe+43AD44
LONG* WriteData;//写入数据
LONG positionBaseAddress;//位置基址
LONG gamePosition;//游戏位置
BOOL SoundSysDesc()//DLL函数
{
	return TRUE;
}
BOOL SoundSysMake()//DLL函数
{
	return TRUE;
}
HWND gameHandle;

static CCFCheaterDlg* g_Dlg;
static HANDLE g_processHandle;

void WriteMemory(void* value, DWORD valueSize, ...) {
	if (value == NULL || valueSize == 0 || g_processHandle == NULL) return;

	DWORD tempValue = 0;

	va_list addresses;
	va_start(addresses, valueSize);
	DWORD offset = 0;
	DWORD lastAddress = 0;
	while ((offset = va_arg(addresses, DWORD)) != -1) {
		lastAddress = tempValue + offset;
		::ReadProcessMemory(g_processHandle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
	}
	va_end(addresses);

	::WriteProcessMemory(g_processHandle, (LPVOID)lastAddress, value, valueSize, NULL);
}

void WriteMemory(void* value, DWORD valueSize, DWORD address) {
	WriteMemory(value, valueSize, address, -1);
}

DWORD monitorThreadFunc(LPVOID lpThreadParameter) {//监控游戏进程
	while (1) {
		gameHandle = FindWindow(CString("CrossFire"), CString("穿越火线"));//取游戏句柄
		DWORD pid;
		GetWindowThreadProcessId(gameHandle, &pid);
		g_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (gameHandle == NULL) {//如果取不到句柄
			g_Dlg->m_bnTS.EnableWindow(FALSE);//禁止点击
			g_Dlg->m_bnTS.SetCheck(FALSE);//同时如果关掉了游戏就将选中的选项给砍了
			g_processHandle = NULL;
		}
		else if (g_processHandle == NULL) {//取到了句柄
			g_Dlg->m_bnTS.EnableWindow(TRUE);//允许点击
		}
	}
	return NULL;
}

/*void TS() {
	tsBaseAddress = (LONG)GetModuleHandleA("crossfire.exe") + 0x43AD44;


}*/

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCFCheaterDlg 对话框



CCFCheaterDlg::CCFCheaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CFCHEATER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCFCheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TS, m_bnTS);
}

BEGIN_MESSAGE_MAP(CCFCheaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CCFCheaterDlg::OnBnClickedCourse)
	ON_BN_CLICKED(IDC_TS, &CCFCheaterDlg::OnBnClickedTs)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCFCheaterDlg 消息处理程序

BOOL CCFCheaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_gameThread = CreateThread(NULL, NULL, monitorThreadFunc, NULL, NULL, NULL);
	g_Dlg = this;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCFCheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void GetQQ() {

}

void CCFCheaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		/*CDialogEx::OnPaint();*/
		CPaintDC   dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC   dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP2);  //对话框的背景图片                       
		BITMAP   bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap* pbmpOld = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCFCheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCFCheaterDlg::OnBnClickedCourse() {
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, CString("open"), CString(""), NULL, NULL, SW_SHOWNORMAL);
}

void CCFCheaterDlg::OnBnClickedTs()
{
	// TODO: 在此添加控件通知处理程序代码
	/*BOOL check = IsDlgButtonChecked(IDC_TS);*/
	CButton* button = (CButton*)GetDlgItem(IDC_TS);
	if (button->GetCheck()) {//如果选择框被单击且是选中状态
		//修改汇编代码
		/*个人感觉这里应该还要再加一个判断单击后的选择框是否是被强制选中的（通过不对劲的手段让这个玩意被单击）状态，
			即EnableWindow(True)，不过加不加也无所谓了*/
			/*BYTE data[] = {};
			WriteMemory(, , );*/
		SetWindowTextA(gameHandle, "透视已开启");
	}
	else
	{//关掉此功能后还原
		/*BYTE data[] = {};
		WriteMemory(, , );*/
		SetWindowTextA(gameHandle, "穿越火线");
	}
}


HBRUSH CCFCheaterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)//处理控件透明
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)     //IDC_DEVICE_STAT为Static控件的ID
	{
		pDC->SetBkMode(TRANSPARENT);   //设置透明属性
		return (HBRUSH)GetStockObject(NULL_BRUSH);   //返回空画刷
	}

	UINT id = pWnd->GetDlgCtrlID();
	if (id == IDC_TS)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		pDC->BitBlt(0, 0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);  //<把父窗口背景图片先画到按钮上
		ReleaseDC(dc);

		hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
