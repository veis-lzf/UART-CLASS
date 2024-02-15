#include "stdafx.h"
#include "CSerialPort.h"

#define DEBUG_MODE 1

#if DEBUG_MODE
// ��������ڽ�����򿪷�����MessageBox����Ϊ��Ӧ����־�����������
void CSerialPort::ShowError(DWORD dwError) // ��ӡ��ʾ������Ϣ
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        0, // Default language
        (LPTSTR)&lpMsgBuf,
        0,
        NULL
    );
    // Display the string.
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONERROR);
    // Free the buffer.
    LocalFree(lpMsgBuf);
}
#else
void CSerialPort::ShowError(DWORD dwError)
{
}
#endif

CSerialPort::CSerialPort()
{
	m_hComm = NULL;
    memset(&m_dcb, 0, sizeof(m_dcb));
}

CSerialPort::~CSerialPort()
{
    CloseComm();
}

BOOL CSerialPort::isOpen()
{
    if (m_hComm == NULL)
        return FALSE;

    return TRUE;
}

// �򿪴���
BOOL CSerialPort::OpenComm(CString szComm)
{
	if (m_hComm == NULL) {
		m_hComm = ::CreateFile(szComm, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		if (INVALID_HANDLE_VALUE == m_hComm) {
            ShowError(GetLastError());
            m_hComm = NULL;
            return FALSE;
		}
        return TRUE;
	}
	return FALSE;
}

// ���ô��ڲ���
BOOL CSerialPort::SetCommState(DWORD dwBaudrate, BYTE byParity, BYTE byByteSize, BYTE byStopBits)
{
    if (NULL == m_hComm) return FALSE;
    
    // 1.��ȡ���ڲ���
    BOOL bRet = ::GetCommState(m_hComm, &m_dcb);
    if (!bRet) {
        if (m_hComm) {
            CloseHandle(m_hComm);
            m_hComm = NULL;
        }
        return FALSE;
    }

    // 2.�޸Ĵ��ڲ���
    m_dcb.BaudRate = dwBaudrate;
    m_dcb.ByteSize = byByteSize;
    m_dcb.StopBits = byStopBits;
    m_dcb.Parity = byParity;
    bRet = ::SetCommState(m_hComm, &m_dcb);
    if (!bRet) {
        if (m_hComm) {
            ShowError(GetLastError());
            CloseHandle(m_hComm);
            m_hComm = NULL;
        }
        return FALSE;
    }

    return TRUE;
}

// ���û�������С
BOOL CSerialPort::SetupComm(DWORD dwInQueue, DWORD dwOutQueue)
{
    if (NULL == m_hComm) return FALSE;

    return ::SetupComm(m_hComm, dwInQueue, dwOutQueue);
}

// ������ڱ�־λ
BOOL CSerialPort::PurgeComm(DWORD dwFlags)
{
    if (NULL == m_hComm) return FALSE;

    return ::PurgeComm(m_hComm, dwFlags);
}

// ���ô����¼�����
BOOL CSerialPort::SetCommMask(DWORD dwEvtMask)
{
    if (NULL == m_hComm) return FALSE;

    return ::SetCommMask(m_hComm, dwEvtMask);
}

// д�ļ�����
BOOL CSerialPort::WriteFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    if (NULL == m_hComm) return FALSE;

    return ::WriteFile(m_hComm, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

// ���ļ�����
BOOL CSerialPort::ReadFile(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    if (NULL == m_hComm) return FALSE;

    return ::ReadFile(m_hComm, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

// �����ص��������
BOOL CSerialPort::GetOverlappedResult(LPOVERLAPPED lpOverlaped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait)
{
    if (NULL == m_hComm) return FALSE;

    return ::GetOverlappedResult(m_hComm, lpOverlaped, lpNumberOfBytesTransferred, bWait);
}

// �������ͨ�Ŵ���
BOOL CSerialPort::ClearCommError(LPDWORD lpErrors, LPCOMSTAT lpStat)
{
    if (NULL == m_hComm) return FALSE;

    return ::ClearCommError(m_hComm, lpErrors, lpStat);
}

// ��ȡ���ڲ���
DCB CSerialPort::GetCommState()
{
    return m_dcb;
}

// ����Ӳ�����֣���������
// �������ڴ��ڴ�֮�󻹿�����
BOOL CSerialPort::SetFlowCtrl(BOOL val)
{
    BOOL bRet = FALSE;
    if (m_hComm == NULL) return FALSE;

    bRet = ::GetCommState(m_hComm, &m_dcb);
    if (!bRet) {
        if (m_hComm) {
            CloseHandle(m_hComm);
            m_hComm = NULL;
        }
        return FALSE;
    }

    if (val)
    {
        m_dcb.fOutxCtsFlow = TRUE;
        m_dcb.fOutxDsrFlow = TRUE;
        m_dcb.fRtsControl = RTS_CONTROL_ENABLE;
        m_dcb.fDtrControl = RTS_CONTROL_ENABLE;
    }
    else
    {
        m_dcb.fOutxCtsFlow = FALSE;
        m_dcb.fOutxDsrFlow = FALSE;
        m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
        m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
    }

    bRet = ::SetCommState(m_hComm, &m_dcb);
    if (!bRet) {
        if (m_hComm) {
            ShowError(GetLastError());
            CloseHandle(m_hComm);
            m_hComm = NULL;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL CSerialPort::EnableXonAndXoff(BOOL val)
{
    BOOL bRet = FALSE;
    if (m_hComm == NULL) return FALSE;

    bRet = ::GetCommState(m_hComm, &m_dcb);
    if (!bRet) {
        if (m_hComm) {
            CloseHandle(m_hComm);
            m_hComm = NULL;
        }
        return FALSE;
    }
    // �޸Ĳ���
    if (val)
    {
        m_dcb.fOutX = TRUE;
        m_dcb.fInX = TRUE;
    }
    else
    {
        m_dcb.fOutX = FALSE;
        m_dcb.fInX = FALSE;
    }
    bRet = ::SetCommState(m_hComm, &m_dcb);
    if (!bRet) {
        if (m_hComm) {
            ShowError(GetLastError());
            CloseHandle(m_hComm);
            m_hComm = NULL;
        }
        return FALSE;
    }
    return TRUE;
}

// ��ȡ��·״̬
int CSerialPort::GetLineStatus()
{
    DWORD dwEvtmask;
    int ret = 0;
    if (m_hComm == NULL) return FALSE;

    if (GetCommModemStatus(m_hComm, &dwEvtmask) == 0) {
        return FALSE;
    }
    ret = dwEvtmask;
    return ret;
}

// ����DTR��״̬
BOOL CSerialPort::SetDTR(BOOL enable)
{
    if (m_hComm == NULL) return FALSE;

    DWORD data = CLRDTR;
    if (enable) data = SETDTR;
    if (EscapeCommFunction(m_hComm, data)) return TRUE;
    return FALSE;
}

// ����RTS��״̬
BOOL CSerialPort::SetRTS(BOOL enable)
{
    if (m_hComm == NULL) return FALSE;

    DWORD data = CLRRTS;
    if (enable) data = SETRTS;
    if (EscapeCommFunction(m_hComm, data)) return TRUE;
    return FALSE;
}

// �رմ���
BOOL CSerialPort::CloseComm()
{
    // �ر��߳�
    m_ThreadComm.StopThread();

    // ������Դ
    if (m_hComm == NULL) return FALSE;
    BOOL bRet = CloseHandle(m_hComm);
    m_hComm = NULL;

    return bRet;
}

// ���������߳�
void CSerialPort::StartComm(void)
{
    m_ThreadComm.SetThreadData((DWORD)this); // ���봮�����thisָ��
    m_ThreadComm.StartThread(); // �����߳�
}
