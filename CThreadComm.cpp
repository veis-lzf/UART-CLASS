#include "stdafx.h"
#include "CThreadComm.h"
#include "CSerialPort.h"
#include "VCOMDialog.h"
#include "ToolsBox.h"

CThreadComm::CThreadComm(void)
{
}

CThreadComm::~CThreadComm(void)
{
}

void CThreadComm::runTask(void)
{
	CVCOMDialog *pComDlg = (CVCOMDialog *)theApp.GetActiveWnd(); // ��ȡ���Ի���ָ��
	if (pComDlg == NULL)
		return;

	CSerialPort *pSerialPort = (CSerialPort *)GetThreadData(); // ��ȡ������ָ��
	if (pSerialPort == NULL)
		return;

	DWORD dwError = 0;
	COMSTAT comStat;
	memset(&comStat, 0, sizeof(COMSTAT));
	pSerialPort->ClearCommError(&dwError, &comStat);
	
	BOOL bRet = TRUE;
	// �Ӵ󻺳����������ļ����չ��̶���
	char recvBuf[4096*10] = { 0 }; // ���ջ�����
	char recvTemp[4096] = { 0 }; // ��ʱ������
	DWORD dwLen = 0;
	DWORD dwRead = 0;
	if (comStat.cbInQue > 0) {
		OVERLAPPED overlappedRead;
		memset(&overlappedRead, 0, sizeof(OVERLAPPED));
		overlappedRead.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

		if (comStat.cbInQue < 4096) {
			bRet = pSerialPort->ReadFile(recvTemp, comStat.cbInQue, &dwRead, &overlappedRead);
		}
		else {
			bRet = pSerialPort->ReadFile(recvTemp, 4096, &dwRead, &overlappedRead);
		}
		
		// �ж��Ƿ����һ�ζ�ȡ
		if (comStat.cbInQue >= dwRead) {
			memcpy(recvBuf + dwLen, recvTemp, dwRead);
			dwLen += dwRead;
			::SendMessage(pComDlg->GetSafeHwnd(), WM_RECEIVE_COMPLETE, (WPARAM)&recvBuf, (LPARAM)dwLen);// �޸�Ϊ�봫���жϣ�����ÿ�ζ������ݴ�ϵ����޷���ȷ������Ϣ
		}

		if (overlappedRead.hEvent)
			CloseHandle(overlappedRead.hEvent);

		// �ж��Ƿ��Ѿ���ȡ���ȫ������
		if (comStat.cbInQue == dwRead) {
			//::SendMessage(pComDlg->GetSafeHwnd(), WM_RECEIVE_COMPLETE, (WPARAM)&recvBuf, (LPARAM)dwLen);
		}

		// �ж��Ƿ񱻹���
		if (!bRet) {
			if (ERROR_IO_PENDING == GetLastError()) {
				while (!bRet) {
					bRet = pSerialPort->GetOverlappedResult(NULL, &dwRead, TRUE);
					if (GetLastError() != ERROR_IO_INCOMPLETE) {
						pSerialPort->ClearCommError(&dwError, &comStat);
						break;
					}
				}
			}
		}
	}
	Sleep(100);
}
