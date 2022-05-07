#include "FileManager.h"
#include <fcntl.h>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <string>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#define O_BINARY (0)
#endif

#ifdef _WIN32
const int CREATE_PMODE = S_IREAD | S_IWRITE;
#else
const int CREATE_PMODE = S_IRWXU | S_IRWXG | S_IRWXO;
#endif

int g_openfiles = 0;
FileAutoClose::FileAutoClose(const std::string &path, int flags)
{
	m_FP = _open(path.c_str(), flags, CREATE_PMODE);
	if(m_FP < 0)
	{
		int err = errno;
		if(err != 2)
		{
			err = err;
		}
	}
	else
	{
		g_openfiles++;
		if(g_openfiles > 8)
		{
			int aaa = 0;
		}
	}
}
FileAutoClose::FileAutoClose(const char *path, int flags)
{
	m_FP = _open(path, flags, CREATE_PMODE);
	if(m_FP < 0)
	{
		int err = errno;
		if(err != 2)
		{
			err = err;
		}
	}
	else
	{
		g_openfiles++;
		if(g_openfiles > 8)
		{
			int aaa = 0;
		}
	}
}
FileAutoClose::~FileAutoClose()
{
	close();
}

bool FileAutoClose::sync()
{
	if(m_FP >= 0)
	{
		//if(DirVisitor::syncFile(m_FP)) return true;
	}
	
	return false;
}

void FileAutoClose::close()
{
	if(m_FP >= 0)
	{
		_close(m_FP);
		m_FP = -1;

		g_openfiles--;
	}
}

bool FileAutoClose::isNull()
{
	return m_FP<0;
}

bool FileAutoClose::read(void *buf, int nbytes)
{
	int n;
	if((n=_read(m_FP, buf, nbytes)) == nbytes) return true;
	else
	{
		int err = errno;
		if(err != 2)
		{
			err = err;
		}
		return false;
	}
}

bool FileAutoClose::write(const void *buf, int nbytes)
{
	int n;
	if((n=_write(m_FP, buf, nbytes)) == nbytes) return true;
	else
	{
		int err = errno;
		return false;
	}
}

int FileAutoClose::fileSize()
{
	int s = _lseek(m_FP, 0, SEEK_END);
	if(s < 0) return 0;
	else return s;
}

int FileAutoClose::tell()
{
	return ::_lseek(m_FP, 0, SEEK_CUR);
}

bool FileAutoClose::seek(int offset, int pos)
{
	if(_lseek(m_FP, offset, pos) >= 0) return true;
	else
	{
		int err = errno;
		return false;
	}
}

bool FileAutoClose::isFileExist(const char *path)
{
	return _access(path, 0) == 0;
}

void *ReadWholeFile(const char *path, int &datalen)
{
	std::string fullpath(path);

	FileAutoClose fp(fullpath, O_RDONLY|O_BINARY);
	if (fp.isNull())
		return NULL;
		

	datalen = fp.fileSize();
	if(datalen == 0) return NULL;

	void *buf = malloc(datalen);
	if(buf == NULL) return NULL;

	fp.seek(0, SEEK_SET);
	if(!fp.read(buf, datalen))
	{
		free(buf);
		return NULL;
	}
	return buf;
}