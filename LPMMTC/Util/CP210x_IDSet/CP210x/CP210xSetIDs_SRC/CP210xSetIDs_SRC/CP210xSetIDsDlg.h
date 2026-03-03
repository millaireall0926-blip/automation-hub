// CP210xSetIDsDlg.h : header file
//

#if !defined(AFX_CP210xSETIDSDLG_H__F10CDA21_3FAF_4B7D_9C29_8855C44684B9__INCLUDED_)
#define AFX_CP210xSETIDSDLG_H__F10CDA21_3FAF_4B7D_9C29_8855C44684B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

#include "dbt.h"
#include "initguid.h"

/////////////////////////////////////////////////////////////////////////////
// CCP210xSetIDsDlg dialog

class CCP210xSetIDsDlg : public CDialog
{
// Construction
public:
	CCP210xSetIDsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCP210xSetIDsDlg)
	enum { IDD = IDD_CP210xSETIDS_DIALOG };
	CString	m_PID;
	CString	m_Power;
	CString	m_SerNum;
	CString	m_VID;
	CString	m_ProdString;
	CString	m_VersionMajor;
	CString	m_VersionMinor;
	CString	m_PartNum;
	CString m_IfcString0;
	CString m_IfcString1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCP210xSetIDsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCP210xSetIDsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnUpdateDeviceList();
	afx_msg void OnButtonProgramIds();
	afx_msg void OnSelchangeDeviceSelect();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	afx_msg void OnButtonAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillDeviceList();
	void SetVid();
	void SetPid();
	void SetSelfPower();
	void SetMaxPower();
	void SetDeviceVersion();
	void SetDeviceMode();
	void SetInterfaceFlushBuffers();
	void SetSerialNumber();
	void SetProductString();
	void SetInterface0String();
	void SetInterface1String();

	void RegisterNotification();
    void UnregisterNotification();

    HDEVNOTIFY m_hNotifyDevNode;

	HANDLE	m_hUSBDevice;
	int		m_DeviceCurSel;
public:
	int m_ECIMode;
	int m_SCIMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CP210xSETIDSDLG_H__F10CDA21_3FAF_4B7D_9C29_8855C44684B9__INCLUDED_)
