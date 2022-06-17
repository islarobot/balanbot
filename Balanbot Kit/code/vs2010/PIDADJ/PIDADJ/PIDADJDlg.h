// PIDADJDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "Serialport.h"

#define KIND_A_KP   0x01
#define KIND_A_KD   0x02
#define KIND_P_KP   0x03
#define KIND_P_KI   0x04

#define KIND_K_BASE   0x05

#define TYPE_UPDATE_PID  0x02
#define TYPE_CONTROL_CAR  0x03
#define TYPE_APPEND_PID   0x04
#define TYPE_WRITE_EEPROM 0x05
#define TYPE_CAR_STATUS  0x06


#define CAR_SPEED 100






// CPIDADJDlg �Ի���
class CPIDADJDlg : public CDialog
{
// ����
public:
	CPIDADJDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PIDADJ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��



	CSerialPortEx m_SerialPort;
	int  m_Port;
	int m_EnableAControl;
	int m_EnableWControl;


	unsigned char m_Sendbuffer[6];
	unsigned char m_Receivebuffer[6];

	afx_msg LONG OnCommunication(WPARAM ch, LPARAM port); 


	int m_KDAPos;
	CBrush m_brush1;
	CBrush m_brush2;
	
	int m_IsPacket;
	bool m_Control;
	int m_CarSpeed;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Com;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int AssemblyData(int type, unsigned char* value);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ProcessData();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void BtnUp();
	void BtnDown();
	void BtnLeft();
	void BtnRight();
	void BtnStop();
	afx_msg void OnBnClickedReadPid();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedWriteEeprom();
	afx_msg void OnBnClickedOpenControl();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnBnClickedWork();
	afx_msg void OnBnClickedSleepOff();
	afx_msg void OnBnClickedPoweOff();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
