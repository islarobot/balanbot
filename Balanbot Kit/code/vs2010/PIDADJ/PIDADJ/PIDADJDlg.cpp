// PIDADJDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PIDADJ.h"
#include "PIDADJDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPIDADJDlg �Ի���




CPIDADJDlg::CPIDADJDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPIDADJDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_EnableAControl = 0;
	m_EnableWControl = 0;

	m_Sendbuffer[0] = 0;
	m_Sendbuffer[1] = 0;
	m_Sendbuffer[2] = 0;
	m_Sendbuffer[3] = 0;
	m_Sendbuffer[4] = 0;
	m_Sendbuffer[5] = 0;

	m_IsPacket = false;

	m_Control = false;

	m_CarSpeed = CAR_SPEED;

}

void CPIDADJDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Com);
}

BEGIN_MESSAGE_MAP(CPIDADJDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_VSCROLL()


	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CPIDADJDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPIDADJDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPIDADJDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPIDADJDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_READ_PID, &CPIDADJDlg::OnBnClickedReadPid)
	ON_BN_CLICKED(IDC_BUTTON6, &CPIDADJDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_WRITE_EEPROM, &CPIDADJDlg::OnBnClickedWriteEeprom)
	ON_BN_CLICKED(IDC_OPEN_CONTROL, &CPIDADJDlg::OnBnClickedOpenControl)
	ON_BN_CLICKED(ID_OK, &CPIDADJDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_WORK, &CPIDADJDlg::OnBnClickedWork)
	ON_BN_CLICKED(IDC_SLEEP_OFF, &CPIDADJDlg::OnBnClickedSleepOff)
	ON_BN_CLICKED(IDC_POWE_OFF, &CPIDADJDlg::OnBnClickedPoweOff)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPIDADJDlg ��Ϣ�������

BOOL CPIDADJDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	CScrollBar* pScrollBar;
	pScrollBar = (CScrollBar*)GetDlgItem(IDC_ANGLE_KP);
	pScrollBar->SetScrollRange(0,5000);
	pScrollBar->SetScrollPos(1000);


	pScrollBar = (CScrollBar*)GetDlgItem(IDC_ANGLE_KD);
	pScrollBar->SetScrollRange(0,5000);
	pScrollBar->SetScrollPos(1000);


	pScrollBar = (CScrollBar*)GetDlgItem(IDC_POS_KP);
	pScrollBar->SetScrollRange(0,5000);
	pScrollBar->SetScrollPos(1000);


	pScrollBar = (CScrollBar*)GetDlgItem(IDC_POS_KD);
	pScrollBar->SetScrollRange(0,5000);
	pScrollBar->SetScrollPos(1000);

	pScrollBar = (CScrollBar*)GetDlgItem(IDC_BASE);
	pScrollBar->SetScrollRange(0,5000);
	pScrollBar->SetScrollPos(1000);

	m_Com.InsertString(0,L"COM1");
	m_Com.InsertString(1,L"COM2");
	m_Com.InsertString(2,L"COM3");
	m_Com.InsertString(3,L"COM4");


	for(int i = 1;i < 5;i++)
	{

		if(	m_SerialPort.InitPort(this,i,9600))
		{
			m_Com.SetCurSel(i-1);
			m_Port = i;
			m_SerialPort.StartMonitoring();
			break;
		}
		else
		{
			CString str;
			str.Format(L"COM%d is occupied",i);
			//AfxMessageBox(str);
			MessageBox(str,L"tips");

		}
	}	

	m_brush1.CreateSolidBrush(RGB(0,255,0));
	m_brush2.CreateSolidBrush(RGB(255,0,0));
	//SetTimer(1,100,0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPIDADJDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPIDADJDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPIDADJDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPIDADJDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int nMin,nMax;
	pScrollBar->GetScrollRange(&nMin,&nMax);  //ȡ�ù�������Χ
	int TempPos = pScrollBar->GetScrollPos();
	 switch(nSBCode)
	 {
	 case SB_THUMBPOSITION://�϶�����
	  pScrollBar->SetScrollPos(nPos);
	  break;
	 case SB_LINELEFT://�����ߵļ�ͷ
	  if(TempPos > nMin)
	  {
	   TempPos--;
	  }
	  pScrollBar->SetScrollPos(TempPos);
	  break;
	 case SB_LINERIGHT  ://����ұߵļ�ͷ
	  if(TempPos<nMax)
	  {
	   TempPos++;
	  }
	  pScrollBar->SetScrollPos(TempPos);
	  break;
	 } 

	 if(TempPos > nMax) TempPos = nMax;
	 if(TempPos < nMin ) TempPos = nMin;
		TempPos = nMax - TempPos;
	 CString str;
	 	
	 float Tmp = (float)TempPos/100.0;
	 switch(pScrollBar->GetDlgCtrlID())
	 {
	 case IDC_ANGLE_KP:

		 str.Format(L"%0.2f",Tmp);
		 SetDlgItemText(IDC_SHOW_A_KP,str);
		 break;
	case IDC_ANGLE_KD:
		str.Format(L"%0.2f",Tmp);

		SetDlgItemText(IDC_SHOW_A_KD,str);
		break;
	case IDC_POS_KP:
		str.Format(L"%0.2f",Tmp);

		SetDlgItemText(IDC_SHOW_P_KP,str);
		break;
	case IDC_POS_KD:
		str.Format(L"%0.2f",Tmp);
		SetDlgItemText(IDC_SHOW_P_KI,str);
		break;
	case IDC_BASE:
		str.Format(L"%0.2f",Tmp);
		SetDlgItemText(IDC_SHOW_BASE,str);
	default:break;
	 }

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CPIDADJDlg::AssemblyData(int type, unsigned char * value)
{	
	m_Sendbuffer[0] = 0xAA;
	m_Sendbuffer[1] = type;
	m_Sendbuffer[2] = value[0];
	m_Sendbuffer[3] = value[1];
	m_Sendbuffer[4] = value[2];
	m_Sendbuffer[5] = m_Sendbuffer[2]^m_Sendbuffer[3]^m_Sendbuffer[4];
	return 1;
}


LONG CPIDADJDlg::OnCommunication(WPARAM ch, LPARAM port) 
{
	unsigned char buff,tmp;
	static int b = 0;
	static int cnt = 0;
	buff = (char)ch;
	if(port == m_Port)
	{
	    for(int i = 5; i > 0; i--)
        {
           m_Receivebuffer[i] = m_Receivebuffer[i-1];
        }
			m_Receivebuffer[0] = buff;
        
        if(m_Receivebuffer[5] == 0xAA)
        {
           tmp = m_Receivebuffer[1]^m_Receivebuffer[2]^m_Receivebuffer[3];
           if(tmp == m_Receivebuffer[0])
           {
             m_IsPacket = true;
           }
        }	
		
	    ProcessData();
	}
	
	return 1;
}
void CPIDADJDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static int cntt = 0;
	if(cntt <5)
	{
		cntt++;
		unsigned char data[3];
		data[0] = 0;
		data[1] =0;
		data[2] = 0;
	
		AssemblyData(TYPE_APPEND_PID,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);
	}
	else
	{
		cntt = 0;
		KillTimer(1);
	}


	CDialog::OnTimer(nIDEvent);
}


void CPIDADJDlg::ProcessData()
{
	CScrollBar* pScrollBar;
	int nMin,nMax;
	float data;
	CString str;

	if(m_IsPacket == true)
	{
		m_IsPacket = false;
		switch(m_Receivebuffer[4])
		{
		case TYPE_APPEND_PID:  //update pid

			  unsigned int Upper,Lower;
			  double NewPara;
			  int NewPos;
			  Upper = m_Receivebuffer[2];
			  Lower = m_Receivebuffer[1];
			  NewPos = (Upper<<8 | Lower);
              NewPara = (float)(Upper<<8 | Lower)/100.0;
			switch(m_Receivebuffer[3])
			{
			case 0x01:
							pScrollBar = (CScrollBar*)GetDlgItem(IDC_ANGLE_KP);
							pScrollBar->GetScrollRange(&nMin,&nMax); 
							pScrollBar->SetScrollPos(nMax - NewPos);
							str.Format(L"%0.2f",NewPara);
							SetDlgItemText(IDC_SHOW_A_KP,str); break;
			case 0x02:
						pScrollBar = (CScrollBar*)GetDlgItem(IDC_ANGLE_KD);
						pScrollBar->GetScrollRange(&nMin,&nMax); 
						pScrollBar->SetScrollPos(nMax - NewPos);
						str.Format(L"%0.2f",NewPara);
						SetDlgItemText(IDC_SHOW_A_KD,str); break;
			case 0x03:
						pScrollBar = (CScrollBar*)GetDlgItem(IDC_POS_KP);
						pScrollBar->GetScrollRange(&nMin,&nMax); 
						pScrollBar->SetScrollPos(nMax - NewPos);
						str.Format(L"%0.2f",NewPara);
						SetDlgItemText(IDC_SHOW_P_KP,str); break;
			case 0x04:
						pScrollBar = (CScrollBar*)GetDlgItem(IDC_POS_KI);
						pScrollBar->GetScrollRange(&nMin,&nMax); 
						pScrollBar->SetScrollPos(nMax - NewPos);
						str.Format(L"%0.2f",NewPara);
						SetDlgItemText(IDC_SHOW_P_KI,str); break;
			case 0x05:
						pScrollBar = (CScrollBar*)GetDlgItem(IDC_BASE);
						pScrollBar->GetScrollRange(&nMin,&nMax); 
						pScrollBar->SetScrollPos(nMax - NewPos);
						str.Format(L"%0.2f",NewPara);
						SetDlgItemText(IDC_SHOW_BASE,str); break;
			default: break;
			}   
			break;

		default:
			break;
		}
	}
}
void  CPIDADJDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	float f;
	unsigned int fi = 0;

	CScrollBar* pScrollBar;
	int nMin,nMax;

	GetDlgItemText(IDC_SHOW_A_KP,str);
	f = (float)atof( CW2A(str));
	fi = f*100;

	unsigned char data[3];
	data[0] = KIND_A_KP;
	data[1] = fi/256;
	data[2] = fi%256;
	
	AssemblyData(TYPE_UPDATE_PID,data);  

	m_SerialPort.WriteToPort(m_Sendbuffer,6);

	pScrollBar = (CScrollBar*)GetDlgItem(IDC_ANGLE_KP);
	pScrollBar->GetScrollRange(&nMin,&nMax); 
	pScrollBar->SetScrollPos(nMax - fi);	
}

void CPIDADJDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	float f;
	unsigned int fi = 0;

	CScrollBar* pScrollBar;
	int nMin,nMax;

	GetDlgItemText(IDC_SHOW_A_KD,str);
	f = (float)atof( CW2A(str));
	fi = f*100;

	unsigned char data[3];
	data[0] = KIND_A_KD;
	data[1] = fi/256;
	data[2] = fi%256;
	
	AssemblyData(TYPE_UPDATE_PID ,data); 

	m_SerialPort.WriteToPort(m_Sendbuffer,6);

	pScrollBar = (CScrollBar*)GetDlgItem(IDC_ANGLE_KD);
	pScrollBar->GetScrollRange(&nMin,&nMax); 
	pScrollBar->SetScrollPos(nMax - fi);	

}

void CPIDADJDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	float f;
	unsigned int fi = 0;

	CScrollBar* pScrollBar;
	int nMin,nMax;

	GetDlgItemText(IDC_SHOW_P_KP,str);
	f = (float)atof( CW2A(str));
	fi = f*100;

	unsigned char data[3];
	data[0] = KIND_P_KP;
	data[1] = fi/256;
	data[2] = fi%256;
	
	AssemblyData(TYPE_UPDATE_PID ,data); 
	m_SerialPort.WriteToPort(m_Sendbuffer,6);

	pScrollBar = (CScrollBar*)GetDlgItem(IDC_POS_KP);
	pScrollBar->GetScrollRange(&nMin,&nMax); 
	pScrollBar->SetScrollPos(nMax - fi);
}

void CPIDADJDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		CString str;
		float f;
		unsigned int fi = 0;

		CScrollBar* pScrollBar;
		int nMin,nMax;

		GetDlgItemText(IDC_SHOW_P_KI,str);
		f = (float)atof( CW2A(str));
		fi = f*100;

		unsigned char data[3];
		data[0] = KIND_P_KI;
		data[1] = fi/256;
		data[2] = fi%256;
	
		AssemblyData(TYPE_UPDATE_PID ,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);

		pScrollBar = (CScrollBar*)GetDlgItem(IDC_POS_KI);
		pScrollBar->GetScrollRange(&nMin,&nMax); 
		pScrollBar->SetScrollPos(nMax - fi);
}

BOOL CPIDADJDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	//�Ƿ�Ϊ������Ϣ
	if( pMsg->message==WM_KEYDOWN)
	{
		if(m_Control == true)
		{
			switch(pMsg->wParam)
			{
			case 'W':BtnUp();break;
			case 'S':BtnDown();break;
			case 'A':BtnLeft();break;
			case 'D':BtnRight();break;
			default:break;
			}
		}
	}

	if( pMsg->message==WM_KEYUP)
	{
		if(m_Control == true)
		{
			switch(pMsg->wParam)
			{
			case 'W':BtnStop();break;
			case 'S':BtnStop();break;
			case 'A':BtnStop();break;
			case 'D':BtnStop();break;
			default:break;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPIDADJDlg::BtnUp()
{
		unsigned char data[3];
		data[0] = 0x01;
		data[1] =0;
		data[2] = 188;//m_CarSpeed;
	
		AssemblyData(TYPE_CONTROL_CAR,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);

}

void CPIDADJDlg::BtnDown()
{
		unsigned char data[3];
		data[0] = 0x02;
		data[1] =0;
		data[2] = 188;//m_CarSpeed;
	
		AssemblyData(TYPE_CONTROL_CAR,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);
}

void CPIDADJDlg::BtnLeft()
{
		unsigned char data[3];
		data[0] = 0x03;
		data[1] =0;
		data[2] = m_CarSpeed;
	
		AssemblyData(TYPE_CONTROL_CAR,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);
}

void CPIDADJDlg::BtnRight()
{
		unsigned char data[3];
		data[0] = 0x04;
		data[1] =0;
		data[2] = m_CarSpeed;
	
		AssemblyData(TYPE_CONTROL_CAR,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);
}

void CPIDADJDlg::BtnStop()
{
		unsigned char data[3];
		data[0] = 0x00;
		data[1] =0;
		data[2] = 0;
	
		AssemblyData(TYPE_CONTROL_CAR,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);
}

void CPIDADJDlg::OnBnClickedReadPid()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	SetTimer(1,100,0);

}


void CPIDADJDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		CString str;
		float f;
		unsigned int fi = 0;

		CScrollBar* pScrollBar;
		int nMin,nMax;

		GetDlgItemText(IDC_SHOW_BASE,str);
		f = (float)atof( CW2A(str));
		fi = f*100;

		unsigned char data[3];
		data[0] = KIND_K_BASE;
		data[1] = fi/256;
		data[2] = fi%256;
	
		AssemblyData(TYPE_UPDATE_PID ,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);

		pScrollBar = (CScrollBar*)GetDlgItem(IDC_BASE);
		pScrollBar->GetScrollRange(&nMin,&nMax); 
		pScrollBar->SetScrollPos(nMax - fi);
}


void CPIDADJDlg::OnBnClickedWriteEeprom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

		unsigned char data[3];
		data[0] = 0;
		data[1] =0;
		data[2] = 0;
	
		AssemblyData(TYPE_WRITE_EEPROM,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);
}


void CPIDADJDlg::OnBnClickedOpenControl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_OPEN_CONTROL))->GetCheck()) 
	{
		m_Control = true;
		GetDlgItem(IDC_TIP)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_Control = false;
		GetDlgItem(IDC_TIP)->ShowWindow(SW_HIDE);
	}

}


void CPIDADJDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CPIDADJDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_SerialPort.StopMonitoring();
	m_SerialPort.ClosePort();
	CDialog::OnClose();
}


void CPIDADJDlg::OnBnClickedWork()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		unsigned char data[3];
		data[0] = 0;
		data[1] =0;
		data[2] = 0x01;
	
		AssemblyData(TYPE_CAR_STATUS,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);
}


void CPIDADJDlg::OnBnClickedSleepOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		unsigned char data[3];
		data[0] = 0;
		data[1] =0;
		data[2] = 0x02;
	
		AssemblyData(TYPE_CAR_STATUS,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);
}


void CPIDADJDlg::OnBnClickedPoweOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		unsigned char data[3];
		data[0] = 0;
		data[1] =0;
		data[2] = 0x03;
	
		AssemblyData(TYPE_CAR_STATUS,data); 

		m_SerialPort.WriteToPort(m_Sendbuffer,6);
}


HBRUSH CPIDADJDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd-> GetDlgCtrlID() == IDC_READ_PID)
	{
		pDC->SetTextColor(RGB(0,255,0));

		return (HBRUSH) m_brush1.GetSafeHandle();
	}

	if (pWnd-> GetDlgCtrlID() == IDC_WRITE_EEPROM)
	{
		pDC->SetTextColor(RGB(255,0,0));

		return (HBRUSH) m_brush2.GetSafeHandle();
	}


	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
