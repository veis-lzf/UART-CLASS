# C++串口类

## 说明：

1. 需要包含CThread.h、CThreadComm.h和CSerialPort.h三个头文件
2. CThread：C++线程类；CThreadComm：串口线程类，继承与CThread；CSerialPort串口接口类
3. 在CThreadComm类继承于CThread类，其中需要自行实现串口任务处理函数runTask，以实现串口接收任务；

```c++
class CSerialPort
{
	HANDLE m_hComm;
	DCB m_dcb;
	CThreadComm m_ThreadComm; // 创建线程使用，重写runTask函数实现串口任务处理

	void ShowError(DWORD dwError);
protected:
	
public:
	CSerialPort();
	~CSerialPort(); // 析构函数

	BOOL isOpen();
	// 打开串口
	BOOL OpenComm(CString szComm); 
	// 设置串口参数
	BOOL SetCommState(DWORD dwBaudrate, BYTE byParity, BYTE byByteSize, BYTE byStopBits); 
	// 设置发送和接收缓冲区大小
	BOOL SetupComm(DWORD dwInQueue, DWORD dwOutQueue); 
	// 清除串口标志
	BOOL PurgeComm(DWORD dwFlags); 
	// 设置串口事件类型
	BOOL SetCommMask(DWORD dwEvtMask); 
	// 写文件操作
	BOOL WriteFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
	// 读文件操作
	BOOL ReadFile(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped); 
	// 获取重叠操作状态
	BOOL GetOverlappedResult(LPOVERLAPPED lpOverlaped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait);
	// 清除串口通信错误
	BOOL ClearCommError(LPDWORD lpErrors, LPCOMSTAT lpStat);
	// 获取串口参数
	DCB GetCommState(void);
	// 设置流控
	BOOL SetFlowCtrl(void);
	// 获取线路状态
	int GetLineStatus(void);
	// 设置DTR状态
	BOOL SetDTR(BOOL enable);
	// 设置RTS状态
	BOOL SetRTS(BOOL enable);
	// 关闭串口
	BOOL CloseComm(void);

public:
	// 启动串口接收线程
	void StartComm(void);
};
```

```c++
class CThread
{
public:
	CThread(void);
	virtual ~CThread(void);
	virtual void SetThreadData(DWORD dwParam = 0);
	virtual DWORD GetThreadData(void);
	void StartThread(void);
	void StopThread(void);
	// 线程任务纯虚函数，必须重载
	virtual void runTask(void) = 0;

protected:
	static DWORD ThreadProc(LPVOID lpParam); // 线程回调函数

protected:
	HANDLE m_hThread; // 线程句柄
	BOOL m_bExit; // 判断线程是否退出
	DWORD m_dwParam; // 用于外部传参使用
};
```

```c++
class CThreadComm :
    public CThread
{
public:
    CThreadComm(void);
    virtual ~CThreadComm(void);
    virtual void runTask(void);
};
```

