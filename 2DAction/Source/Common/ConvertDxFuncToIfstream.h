/* ====================================================================== */
/**
 * @brief	DXライブラリを使用してアーカイブ化したテキスト(json)ファイル
			にアクセスするためのラッパークラス
			
 *
 * @note
 */
/* ====================================================================== */

#ifndef __CONV_DX_FUNC__
#define __CONV_DX_FUNC__

#include <sstream>
#include "System/SystemDefine.h"

class dxfstreambuf:public std::streambuf{
protected:
	int FileHandle;
public:
	dxfstreambuf( const char* FilePath){
		FileHandle=FileRead_open(FilePath);
	}
	~dxfstreambuf(){FileRead_close(FileHandle);}
protected:
	std::streamsize xsgetn(char* c_p,std::streamsize size){
		return static_cast<std::streamsize>( FileRead_read( c_p, static_cast<int>( size ), FileHandle) );
	}
	int uflow(){return FileRead_getc(FileHandle);}
	int pbackfail(int nCh){
		FileRead_seek(FileHandle,-1,SEEK_CUR);
		return 0;
	}
	int underflow(){
		int ans=uflow();
		FileRead_seek(FileHandle,-1,SEEK_CUR);
		return ans;
	}
public:
	bool eof(){return FileRead_eof(FileHandle)!=0;}
	LONGLONG tellg(){return FileRead_tell(FileHandle);}
	void seekg(int n){seekg(n,SEEK_SET);}
	void seekg(int n,int origin){FileRead_seek(FileHandle,n,origin);}
};
class idxfstream:public std::istream{
private:
	dxfstreambuf* streambuf;
	std::_Uninitialized uninit;
public:
	idxfstream():std::istream(uninit){streambuf=0;}
	idxfstream(const char* FilePath):std::istream(uninit){
		streambuf=0;
		open(FilePath);
	}
	~idxfstream(){close();}
	void open(const char* FilePath){
		close();
		streambuf= new dxfstreambuf(FilePath);
		std::istream::init(streambuf);
	}
	void close(){
		if(streambuf==0)return;
		delete streambuf;
		streambuf=0;
	}
	bool eof(){return streambuf->eof();}
	bool is_open(){return streambuf!=0;}
	pos_type tellg(){return streambuf->tellg();}
	void seekg(pos_type n){return streambuf->seekg( static_cast<int>( n ) );}
	void seekg(off_type n,int rel){return streambuf->seekg( static_cast<int>( n ),rel);}
};

#endif
