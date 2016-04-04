#include "mlib_socket.h"
#include "SocketBuffer.h"
#include <curl/curl.h>

#include "mlib_log.h"
#include "mlib_utils.h"
#include "mlib_thread.h"
#include <sys/socket.h>
#include <unistd.h>
#include "cocos2d.h"

MLIB_NS_BEGIN

static std::recursive_mutex g_mutex;

const std::string MSocketRequest::EVENT_FINISHED = "event_finished";
const std::string MSocketRequest::EVENT_CANCELLED = "event_cancelled";
const std::string MSocketRequest::EVENT_DELETE = "event_delete";

static MSharedQueue<MSocketRequest *> g_requests_low;
static MSharedQueue<MSocketRequest *> g_requests_normal;
static MSharedQueue<MSocketRequest *> g_requests_high;

static uint32_t g_num_of_threads_low = 0;
static uint32_t g_num_of_threads_normal = 0;
static uint32_t g_num_of_threads_high = 0;



void __request_thread_run(MSharedQueue<MSocketRequest *> & requests, bool isTemp = false)
{
    MSocketRequest * req = nullptr;
    
    while (1)
    {
        if (requests.empty() && isTemp)
        {
            break;
        }
        
        requests.pop(req);
        
        if (req->_isCancelled)
        {
            MSocketRequest::Delete(req);
            continue;
        }
        
        auto now = time(NULL);
        if (now - req->_startTime > req->_timeoutInSeconds)
        {
            req->_response = req->createResponse(MSocketResponse::ERROR, nullptr, 0);
            req->socketState(MSocketRequest::SOCKET_FAILED);
            req->_isSuccess = false;
        }
        else
        {
            req->_isSuccess = false;
        
            if(req->_hSocket != -1)
            {
                close(req->_hSocket);
            }
            
            req->_hSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (req->_hSocket == -1)
            {
                
            }
            
            sockaddr_in socketAddr;
            memset(&socketAddr, 0, sizeof(socketAddr));
            
            socketAddr.sin_family = AF_INET;
            socketAddr.sin_port = htons(req->_iport);
            socketAddr.sin_addr.s_addr = inet_addr(req->_host.c_str());
            
            memset(&(socketAddr.sin_zero), 0, sizeof(socketAddr.sin_zero));
            
            int iErrorCode = connect(req->_hSocket, (sockaddr*)&socketAddr, sizeof(socketAddr));
            if (iErrorCode == -1)
            {
                printf("socket connect error:%d\n",errno);
                return;
                
            }
            
            req->socketState(MSocketRequest::SOCKET_SUCCESS);
            
            cocos2d::CCLog("send send");
            cocos2d::CCLog("%x, %s, %zu", req, req->_paramStream.c_str() + 2, req->_paramLen);
            
            
            const char *pValue = req->_paramStream.c_str();
            unsigned short msgLen = req->_paramLen + 2;
            
            char* pTmpData = new char(msgLen);
            
            char *pValueLength = static_cast<char*>(static_cast<void*>(&req->_paramLen));
            memcpy(pTmpData, pValueLength + 1, sizeof(char));
            memcpy(pTmpData + 1, pValueLength, sizeof(char));
            memcpy(pTmpData + 2, pValue, req->_paramLen);
            
            cocos2d::CCLog("message2 %s", pTmpData + 2);
            ssize_t ret = send(req->_hSocket, pTmpData, msgLen, 0);
            if(ret == -1)
            {
                return;
            }
            
            delete pTmpData;
            

            
            SocketBuffer recvBuff;
            MBuffer msgBuffer;
            ret = recv(req->_hSocket, recvBuff.getBuffer() + recvBuff.getPosition(), recvBuff.remaining(), 0);
            if(ret == -1)
            {
                req->_isSuccess = false;
            }
            else if(ret == 0)
            {
                req->_isSuccess = false;
            }
            else
            {
                req->_isSuccess = true;
                
                short msgLen = 0;
                char* pMsgLen = static_cast<char*>(static_cast<void*>(&msgLen));
                
                signed char high = recvBuff.getByte();
                memcpy(pMsgLen + 1, &high, sizeof(char));
                
                signed char low = recvBuff.getByte();
                memcpy(pMsgLen, &low, sizeof(char));
                
                cocos2d::CCLog("receive message %s, %d", recvBuff.getBuffer() + recvBuff.getPosition(), msgLen);
                
                msgBuffer.appendData(recvBuff.getBuffer() + recvBuff.getPosition(), msgLen);
            }
            
            if (req->_isSuccess)
            {
                cocos2d::CCLog("msg : %s, size : %zu", msgBuffer.getData(), msgBuffer.size());
                req->_response = req->createResponse(MSocketResponse::OK, msgBuffer.getData(), msgBuffer.size());
                
                req->_isSuccess = req->_isSuccess && req->_response->isValid();
            }
            else
            {
                req->_response = req->createResponse(MSocketResponse::ERROR, nullptr, 0);
            }
        }
        
        runInMainThread([req] () {
            
            if (!req->_isCancelled)
            {
                cocos2d::CCLog("main thread success");
                req->addEventListener(MSocketRequest::EVENT_FINISHED, [req] (mlib::MEvent * evt) {
                    if (req->_isSuccess)
                    {
                        cocos2d::CCLog("_isSuccess");
                        if (req->_successHandler)
                        {
                            cocos2d::CCLog("_successHandler");
                            req->_successHandler(req);
                        }
                    }
                    else
                    {
                        cocos2d::CCLog("_error");
                        if (req->_errorHandler)
                        {
                            cocos2d::CCLog("_errorHandler");
                            req->_errorHandler(req);
                        }
                    }
                }, &g_mutex);
                
                req->dispatchEvent(MEvent(MSocketRequest::EVENT_FINISHED));
                req->removeEventListenerFor(&g_mutex);
            }
            else
            {
                cocos2d::CCLog("main thread failed");
            }
        });
        
        MSocketRequest::Delete(req);
    }
}

static void __create_thread(MSharedQueue<MSocketRequest *> & queue, uint32_t & counter, uint32_t limit = 0, size_t count = 1)
{
    for (size_t i = 0; i < count; ++i)
    {
        std::thread([&queue, &counter, limit]() {
            {
                std::lock_guard<decltype(g_mutex)> guard(g_mutex);
                if (limit > 0 && counter >= limit)
                {
                    return;
                }
                counter ++;
            }
            
            __request_thread_run(queue, limit != 0);
            
            {
                std::lock_guard<decltype(g_mutex)> guard(g_mutex);
                if (counter > 0)
                {
                    counter--;
                }
            }
        }).detach();
    }
}



#pragma mark MSocketRequest

MSocketRequest::MSocketRequest(const std::string& host, const int port) :
_host(host),
_iport(port),
_timeoutInSeconds(30),
_networkTimeout(30),
_response(nullptr),
_startTime(0),
_finished(false),
_isSuccess(false),
_isCancelled(false),
_priority(NORMAL)
{
    
}

MSocketRequest::~MSocketRequest()
{
    if (_response)
    {
        delete _response;
        _response = nullptr;
    }
}

void MSocketRequest::setParameter(Json::Value& value)
{
    Json::FastWriter writer;
    _paramStream = writer.write(value);
    _paramLen = strlen(_paramStream.c_str());
    
    cocos2d::CCLog("message %s", _paramStream.c_str());
}

void MSocketRequest::send()
{
    std::lock_guard<decltype(g_mutex)> guard(g_mutex);
    
    static bool threadInitialized = false;
    
    if (!threadInitialized)
    {
        threadInitialized = true;
        __create_thread(g_requests_low, g_num_of_threads_low, 0 ,1);
        __create_thread(g_requests_normal, g_num_of_threads_normal, 0, 2);
        __create_thread(g_requests_high, g_num_of_threads_high, 0, 3);
        
        // monitor thread
        std::thread([](){
            while (1) {
                __create_thread(g_requests_low, g_num_of_threads_low, 2, g_requests_low.size());
                __create_thread(g_requests_normal, g_num_of_threads_normal, 10, g_requests_normal.size());
                __create_thread(g_requests_high, g_num_of_threads_high, 20, g_requests_high.size());
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
            
        }).detach();
    }
    
    _startTime = time(NULL);
    
    switch (_priority) {
        case LOW:
            g_requests_low.push(this);
            break;
        case NORMAL:
            g_requests_normal.push(this);
            break;
        case HIGH:
            g_requests_high.push(this);
            break;
            
        default:
            break;
    }
}

void MSocketRequest::cancel()
{
    runInMainThread([this] () {
        if (!_isCancelled)
        {
            _isCancelled = true;
            dispatchEvent(MEvent(EVENT_CANCELLED));
        }
        else
        {
            M_WARNING("Request is already cancelled!");
        }
        
    });
}

void MSocketRequest::Delete(mlib::MSocketRequest *&req)
{
    if (!req->_finished)
    {
        req->dispatchEvent(MEvent(EVENT_DELETE));
        req->_finished = true;
        delete req;
        
        req = nullptr;
        
    }
}

void MSocketRequest::onSuccess(std::function<void (MSocketRequest *)> handler)
{
    _successHandler = handler;
}

void MSocketRequest::onError(std::function<void (MSocketRequest *)> handler)
{
    _errorHandler = handler;
}

MSocketResponse * MSocketRequest::createResponse(unsigned short statusCode, const char *data, size_t size)
{
    return new MSocketResponse(statusCode, data, size);
}


#pragma mark MSocketResponse

MSocketResponse::MSocketResponse(unsigned short statusCode, const char * data, size_t size) :
_statusCode(statusCode),
_responseData(data, size),
_isErrorHandled(false)
{
    
}


MLIB_NS_END
