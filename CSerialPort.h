#pragma once

#include "CThreadComm.h"

class CSerialPort
{
	HANDLE m_hComm;
	DCB m_dcb;
	CThreadComm m_ThreadComm; // �����߳�ʹ�ã���дrunTask����ʵ�ִ���������

	void ShowError(DWORD dwError);
protected:
	
public:
	CSerialPort();
	~CSerialPort(); // ��������

	BOOL isOpen();
	// �򿪴���
	BOOL OpenComm(CString szComm); 
	// ���ô��ڲ���
	BOOL SetCommState(DWORD dwBaudrate, BYTE byParity, BYTE byByteSize, BYTE byStopBits); 
	// ���÷��ͺͽ��ջ�������С
	BOOL SetupComm(DWORD dwInQueue, DWORD dwOutQueue); 
	// ������ڱ�־
	BOOL PurgeComm(DWORD dwFlags); 
	// ���ô����¼�����
	BOOL SetCommMask(DWORD dwEvtMask); 
	// д�ļ�����
	BOOL WriteFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
	// ���ļ�����
	BOOL ReadFile(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped); 
	// ��ȡ�ص�����״̬
	BOOL GetOverlappedResult(LPOVERLAPPED lpOverlaped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait);
	// �������ͨ�Ŵ���
	BOOL ClearCommError(LPDWORD lpErrors, LPCOMSTAT lpStat);
	// ��ȡ���ڲ���
	DCB GetCommState(void);
	// ��������
	BOOL SetFlowCtrl(BOOL val);
	// ����������
	BOOL EnableXonAndXoff(BOOL val);
	// ��ȡ��·״̬
	int GetLineStatus(void);
	// ����DTR״̬
	BOOL SetDTR(BOOL enable);
	// ����RTS״̬
	BOOL SetRTS(BOOL enable);
	// �رմ���
	BOOL CloseComm(void);

public:
	// �������ڽ����߳�
	void StartComm(void);
};

