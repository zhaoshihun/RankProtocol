/*
 * =====================================================================================
 *
 *       Filename:  SendRankData.cpp
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
#include "SendRankData.h"
#include "curl.h"

SendRankData *SendRankData::m_pInstance = new SendRankData();
void    SendRankData::postMethodSend(uint8_t *buffer,Responder * responder,unsigned int size)
{
    Lock loc(mMutex);
    RequestData *rd = new RequestData;
    rd->responder = responder;
    rd->data = buffer;
    rd->size =size;
    requestCache.push_back(rd);
    
}
size_t postWriteData(void *recvBuffer,size_t size,size_t nmemb,void *userParam)
{
    RankProtocol * rankP = (RankProtocol*) userParam ;
    
    rankP->SetBuffer((uint8_t*)recvBuffer);
    
    return size*nmemb; 
}
void SendRankData::run()
{
    while(1)
    {
        usleep(10000);
        
        if(requestCache.size()>0)
        {
            RequestData* content=NULL;
            content = requestCache.front();
            
            CURL* easy_handle = curl_easy_init();
            
            //char *encodedURL = curl_easy_escape(easy_handle,p->data, strlen(p->data));
            
            curl_easy_setopt(easy_handle,CURLOPT_URL,"http://10.18.1.195/HomServer/www/http/GateWay.php?cmd=DailyRewards&uid=xxx");
            
            RankProtocol *rankObj = new RankProtocol();
            
            curl_easy_setopt(easy_handle,CURLOPT_POSTFIELDS,content->data);
            
            curl_easy_setopt(easy_handle,CURLOPT_POSTFIELDSIZE,content->size);
            
            requestCache.pop_front();//delete first element
            
            curl_easy_setopt(easy_handle,CURLOPT_WRITEFUNCTION,postWriteData);//receive callback function
            curl_easy_setopt(easy_handle,CURLOPT_WRITEDATA,rankObj);
            
            curl_easy_setopt(easy_handle,CURLOPT_POST,1);
            curl_easy_setopt(easy_handle,CURLOPT_VERBOSE,1); /* open comment when debug mode.*/
            
            CURLcode code=curl_easy_perform(easy_handle);
            if(code == 0)
            {
                content->responder->receiveSuccessHandle(rankObj);
            }
            else 
            {
                content->responder->receiveFailedHandle(NULL);
            }
            //curl_free(encodedURL);
            curl_easy_cleanup(easy_handle);
        }
        else 
        {
            int aa;//debug
        }
    }
}