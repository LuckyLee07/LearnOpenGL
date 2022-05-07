#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include <string>

//预处理新增_CRT_SECURE_NO_WARNINGS

class FileAutoClose
{
public:
	FileAutoClose(const std::string &path, int flags);
	FileAutoClose(const char *path, int flags);
	~FileAutoClose();

	bool sync();
	void close();
	bool isNull();
	bool read(void *buf, int nbytes);
	bool write(const void *buf, int nbytes);
	int fileSize();
	int tell();
	bool seek(int offset, int pos);

	static bool isFileExist(const char *path);

private:
	int m_FP;
};

extern void *ReadWholeFile(const char *path, int &datalen);

#endif