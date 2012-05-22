/*
 * =====================================================================================
 *
 *       Filename:  SendRankData.h
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
#ifndef rankURL_SendRankData_h
#define rankURL_SendRankData_h

#include "Thread.h"
#include "Mutex.h"
#include "RankProtocol.h"

#include <list>

typedef void CURL;

class Responder
{
public:
	virtual ~Responder(){}
	Responder(){}
	virtual void receiveSuccessHandle(RankProtocol * rank)//must delete the content by youself
	{
        
	}
	virtual void receiveFailedHandle(RankProtocol *rank)//must delete the content by youself
	{//if content is NULL ,meaning the server result not a amf .
	}
};

struct RequestData
{
    Responder *responder;
    uint8_t * data;	
    unsigned int size;
};

class SendRankData :public Thread
{
public:
    static  SendRankData* get_instance(){return m_pInstance;}
    void    postMethodSend(uint8_t *buffer,Responder * responder,unsigned int size);
    
    void    run();
private:
    static SendRankData *m_pInstance;
    SendRankData(){start();}
    Mutex mMutex;
    std::list<RequestData*> requestCache;
};

#endif
