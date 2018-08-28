// FINDX.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "afx.h"
#include <stdio.h>
#include "base64.h"
#include <winsock.h>
#include "smtp.h"

//#pragma  comment(  linker,  "/subsystem:\"windows\"  /entry:\"wmainCRTStartup\""  )


#define DEEP_DG  (5000)


unsigned int file_find_cnt = 0,file_add_cnt = 0;
char * dds,*path_d;

CString current_path;

char TSD_Header[20];

int Tchar_to_char(_TCHAR * tchar,char * buffer);

char command_buffer[200];

unsigned char pos_244[2*1024*1024];
unsigned int pos_len;

unsigned char pos_read[10240];
unsigned int pos_read_len;

char hbuffer[2000][64];

char hbuffer_path[200][512];

unsigned int h_cnt = 0;

char cbuffer[2*1024*1024];

unsigned char h_va[169];

int match_file(char * src,char *match,unsigned int len);

int FindBmpFile(CString strFoldername)
{
    CFileFind tempFind;
    BOOL bFound; 
    bFound=tempFind.FindFile(strFoldername +   "\\*.*"); 
    CString strTmp,strpath;   //如果找到的是文件夹 存放文件夹路径
	/*-----------*/
    while(bFound)      //遍历所有文件
    {
        bFound=tempFind.FindNextFile(); 
                                                              
        if(tempFind.IsDots()) 
			continue; 

			if(tempFind.IsDirectory())   
            {
                strTmp = _T("");
                strTmp = tempFind.GetFilePath();
				current_path = strTmp;
                FindBmpFile(strTmp);
            }
            else
            {
				strTmp = tempFind.GetFileName();//tempFind.GetFilePath();
				strpath = tempFind.GetFilePath();
                 // 在此处添加对找到文件的处理
				/*file_find_cnt*/
				file_find_cnt ++;
				/*----------*/
				USES_CONVERSION;
				/*--------------------------*/
				dds = T2A(strTmp);
				path_d = T2A(strpath);
				/*--------------------------*/
				char type[200];
				int len = strlen(dds);
				int pos = 0;
				/* clear */
				memset(type,0,sizeof(type));
				/*-----search-----*/
				for( int i = 0 ; i < len ; i ++ )
				{
					if( dds[i] == '.' )
					{
						pos = i;//last '.'
					}
				}
				/*---------------*/
				if( pos != 0 )
				{
					memcpy(type,&dds[pos],len-pos);
				}
				/*-------------------------*/
#if 1
                if( strcmp(type,".c") == 0 )
				{
					//FILE * c_file = fopen(path_d,"rb");

					//fread(cbuffer,1,sizeof(cbuffer),c_file );

					//int i = 0;

					//for( i = 0 ; i < 169 ; i ++ )
					//{
					//	if( strstr(cbuffer,hbuffer[i]) != NULL )
					//	{
					//		//printf("%s,%d\r\n",hbuffer[i],file_add_cnt++);
					//		h_va[i] = 0xff;
					//		//break;
					//	}
					//}

					////if( i == 169 )
					////{
					////	printf("%s %d  , %d\r\n",dds,file_find_cnt,file_add_cnt++);
					////}

					if( strstr((const char *)pos_244,dds) == NULL )
					{
						DeleteFile(strpath);
					    printf("%s %d  , %d\r\n",path_d,file_find_cnt,file_add_cnt++);
					}

					//fclose(c_file);
					//memcpy(hbuffer[h_cnt++],dds,strlen(dds));
				}
#endif 
				//if( strcmp(type,".h") == 0 )
				//{
				//   memcpy(hbuffer_path[h_cnt++],path_d,strlen(path_d));
				//   printf("%s,%d\r\n",path_d,file_add_cnt++);
				//}

				//if( strcmp(type,".txt") == 0 ||  strcmp(type,".htm") == 0 || strcmp(type,".tesbak") == 0 ||  strcmp(type,".asm") == 0 )
				//{
				//	DeleteFile(strpath);
				//	printf("%s %d  , %d\r\n",dds,file_find_cnt,file_add_cnt++);
				//}

				//if( strcmp(type,".s") == 0 )
				//{
				//	printf("%s %d  , %d\r\n",path_d,file_find_cnt,file_add_cnt++);
				//}

				//printf("%s %d  , %d\r\n",path_d,file_find_cnt,file_add_cnt);
            }
    }

	/*------------------------------*/
    tempFind.Close();
	/*---------------*/
    return 0;
}
email_config config_niu;

int match_file(char * src,char *match,unsigned int len)
{
	int j;

	for( int i = 0 ; i < pos_len ; i ++ )
	{
		for( j = 0 ; j < len ; j ++ )
		{
			if( src[i+j] != match[j] )
			{
				break;
			}
		}

		if( j == len )
		{
			return 0;
		}
	}
	return (-1);
}

int _tmain(int argc, _TCHAR* argv[])
{
	config_niu.host = "smtp.exmail.qq.com";
	config_niu.account = "niuzl@feimarobotics.com";
	config_niu.password = "3361100niu";
	config_niu.from = "niuzl@feimarobotics.com";
	config_niu.to = "niuzl@feimarobotics.com";

	current_path = _T("F:/D200_POS_SVN/D200_POS");//d200\\find_test");F:\D200_POS_SVN\D200_POS\Driver

	int rest;//C://Users//YJ-User17//Desktop//log//pos//pos_244.pos
	//
	FILE * pos_p = fopen("F:/D200_POS_SVN/D200_POS/Project/MDK-ARM/Project.uvprojx","rb");

	if( pos_p == NULL )
	{
		printf("open fail\r\n");
		return (-1);
	}

	pos_len = fread(pos_244,1,sizeof(pos_244),pos_p );
	/*----------------------------------------------------------------------------------------------------------------*/
	//FILE * h_file = fopen("h_file.bin","rb");

	//fread(hbuffer,1,sizeof(hbuffer),h_file );

	//fclose(h_file);
	///*----------------------------------------------------------------------------------------------------------------*/
	//FILE * h1_file = fopen("h1_file.bin","rb");

	//fread(hbuffer_path,1,sizeof(hbuffer_path),h1_file);

	//fclose(h1_file);
	/*----------------------------------------------------------------------------------------------------------------*/
	/*while(1)
	{
	  */
		rest = FindBmpFile(current_path);
	/*------------------------------*/
	//FILE * h1_file = fopen("h1_file.bin","wb+");

	//fwrite(hbuffer_path,1,sizeof(hbuffer_path),h1_file);

	//fclose(h1_file);
	/*------------------------------*/
    //delete file
	/*	USES_CONVERSION;

	for( int i = 0 ; i < 169 ; i ++ )
	{
		if( h_va[i] != 0xff )
		{
			CString path = A2T(hbuffer_path[i]);

			DeleteFile(path);

			printf("%s %d\r\n",hbuffer_path[i],file_add_cnt++);
		}
	}*/
	/*****************************************************************************************************************/
	printf("%s total: %d\r\n",dds,file_find_cnt);



	/*	if( file_add_cnt < DEEP_DG )
		{
			break;
		}

		file_add_cnt = 0;

	}*/
	//int ret = send_email(&config_niu,"123","456");

	//if( ret == 0 )
	//{
	//	printf("send mail ok\r\n");
	//}else
	//{
	//	printf("send mail err\r\n");
	//}
	return 0;
}
