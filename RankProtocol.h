/*
 * =====================================================================================
 *
 *       Filename:  RankProtocol.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/16/2012 09:56:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zsh miechal (ALL), miechalzhao@gmail.com
 *        Company:  none
 *
 * =====================================================================================
 */
#ifndef rankURL_RankProtocol_h
#define rankURL_RankProtocol_h
#include<string>
#include<vector>
typedef unsigned char uint8_t;

typedef struct userInfo
{
    std::string uid;
    std::string username;
    int stage;
    int rank;
    std::string country;
    //any other column
}UserInfo;

class InfoType
{
public:
    static const char 
    uid =0x01,
    username =0x02,
    country = 0x03,
    stage = 0x04,
    rank =0x05;
};

//template<class T>
//inline void CoreSwap(T& a, T& b)
//{
//	T t = a;
//	a = b;
//	b = t;
//}
//inline void FlipU32(unsigned int  value)
//{
//	uint8_t *pa = (uint8_t *) &(value);
//	CoreSwap(pa[0], pa[3]);
//	CoreSwap(pa[1], pa[2]);
//}

class RankProtocol
{
public:
    RankProtocol();//default alloc 2048 bytes
    RankProtocol(int size);
    //RankProtocol(uint8_t *buffer);
    ~RankProtocol();
    void RankProtocolHeader();
    void WriteInt(int value);
    void WriteString(const std::string &value);
    void WriteByte(uint8_t value);
    void WriteShort(const short *value);
    
    int ReadInt();
    unsigned short ReadShort();
    uint8_t ReadByte();
    std::string ReadString();
    void ReadRankProtocol();
    
    void Serialize(std::vector<UserInfo*> *data);
    void Deserialize(std::vector<UserInfo*> *&data);
    
    uint8_t *GetBuffer(){return buffer;}
    void     SetBuffer(uint8_t *b){buffer = b;}
    unsigned int GetLength(){return position;}
    void SetPosition(int ps){ position = ps;}
    int GetPosition(){return position;}
private:
    uint8_t* buffer;
    unsigned int position;
    unsigned int total_length;
    void Relloc(unsigned int size);

};
#endif
