
// CFCheaterDlg.h: 头文件
//

#pragma once


// CCFCheaterDlg 对话框
class CCFCheaterDlg : public CDialogEx
{
	// 构造
public:
	CCFCheaterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CFCHEATER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
private:
	friend DWORD monitorThreadFunc(LPVOID);
	afx_msg void OnBnClickedCourse();
	afx_msg void OnBnClickedTs();

	CButton m_bnTS;
	HANDLE m_gameThread;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
