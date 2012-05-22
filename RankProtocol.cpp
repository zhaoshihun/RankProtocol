/*
 * =====================================================================================
 *
 *       Filename:  RankProtocol.cpp
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
#include "RankProtocol.h"

RankProtocol::RankProtocol()
{
    buffer = new uint8_t[2048];
    total_length = 2048;
    position = 0;
}

RankProtocol::RankProtocol(int size)
{
    buffer = new uint8_t[size];
    total_length  = size;
    position = 0;
}

RankProtocol::~RankProtocol()
{
    delete []buffer;
}

void RankProtocol::WriteInt(int value)
{
    uint8_t data[4] = {0};
    data[0] = value >> 24;
    data[1] = value >> 16;
    data[2] = value >> 8;
    data[3] = value;
    
    *(int*)(buffer+position) = *(int*)data;
    
    position += sizeof(int);
}

void RankProtocol::RankProtocolHeader()
{
//extension
}

void RankProtocol::ReadRankProtocol()
{
//extension
}

void RankProtocol::WriteString(const std::string &value)
{
    short size = value.size();
    WriteShort(&size);
    
    memcpy(position+buffer,value.c_str(),size);
    
    position += size;
}
void RankProtocol::WriteShort(const short *value)
{
    uint8_t data[2] = {0};
    data[0] = *value >> 8;
    data[1] = *value;
    
    *(short*)(buffer+position) = *(short*)data;
    
    position += sizeof(short);
}
unsigned short RankProtocol::ReadShort()
{
    int v = 0;
    unsigned char *tmp = new uint8_t[sizeof(short)];
    memcpy(tmp,buffer+position,sizeof(short));
	
    std::vector<unsigned char> t(tmp, tmp + sizeof(short));
    delete [] tmp;
    
    for(int i = 0; i < t.size(); i++) {
        v = v << 8 | t[i];
    }
    t.clear();
    position +=sizeof(short);
    return v;
}
int RankProtocol::ReadInt()
{
    int v = 0;
    unsigned char *tmp = new uint8_t[sizeof(int)];
    memcpy(tmp,buffer+position,sizeof(int));
	
    std::vector<unsigned char> t(tmp, tmp + sizeof(int));
    delete [] tmp;
    
    for(int i = 0; i < t.size(); i++) {
        v = v << 8 | t[i];
    }
    t.clear();
    position +=sizeof(int);
    return v;
}
void RankProtocol::WriteByte(uint8_t value)
{
    *(uint8_t*)(buffer+position) = value;
    position += 1;
}
uint8_t RankProtocol::ReadByte()
{//TO confirm
    uint8_t value = 0x00;
    value = *(uint8_t*)(buffer+position) ;
    position+=1;
    return value;
}
std::string RankProtocol::ReadString()
{
    short size = ReadShort();
    char temp[1024]={0};
    memcpy(temp,buffer+position,size);
    position += size;
    return temp;
}
void RankProtocol::Serialize(std::vector<UserInfo*> *data)
{
    short size = data->size();
    
    WriteShort(&size);
    
    for(std::vector<UserInfo*>::iterator iter = data->begin();iter!=data->end();iter++)
    {
        WriteByte(InfoType::uid);
        WriteString((*iter)->uid);
        
        WriteByte(InfoType::username);
        WriteString((*iter)->username);
        
        WriteByte(InfoType::rank);
        WriteInt((*iter)->rank);
        
        WriteByte(InfoType::stage);
        WriteInt((*iter)->stage);
        
        WriteByte(InfoType::country);
        WriteString((*iter)->country);
    }
}
void RankProtocol::Deserialize(std::vector<UserInfo*> *&data)
{
    int packageSize = ReadInt();
    
    for(int i=0;i<packageSize;i++)
    {
        UserInfo *info = new UserInfo[sizeof(UserInfo)];
        for(int j=0;j<5;j++)
        {
            uint8_t type = ReadByte();
            
            switch (type){
                case InfoType::country:
                    info->country = ReadString();
                    break;
                case InfoType::uid:
                    info->uid = ReadString();
                    break;
                case InfoType::username:
                    info->username = ReadString();
                    break;
                case InfoType::rank:
                    //info->rank = ReadInt();
                    info->rank = i+1;
                    break;
                case InfoType::stage:
                    info->stage = ReadInt();
                    break;
                default:
                    break;
            }
        }
        data->push_back(info);
    }
}
void RankProtocol::Relloc(unsigned int size)
{
    if(size>=total_length-position)
    {
        uint8_t *temp = new uint8_t[total_length*2];
        memcpy(temp,buffer,position);
        delete [] buffer;
        buffer = temp;
    }
}