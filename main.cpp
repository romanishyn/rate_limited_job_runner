#include "toket_buket.h"

#include <iostream>
#include <vector>
#include <memory>
        
auto createAsyncRequestJob( client, request, basCallback )
{
    return [=](jobRunner j){
        client->sendAsyncRequest( request,  [ basCallback, m_jobRunner ] (auto &&...args) {
            m_jobRunner.done();
            basCallback(...args);
        };
    };
}

class SpdlResolution
{
private:
    ClientFactoruy factory;

    JobRunner m_jobRunner;

public:
    void resolve( std::string spdl, callback( statusCode, vector< std::stirng> items) )
    {
        std::shared_ptr< Client > client = factory.createClient();

        auto basCallback = [ callback, m_jobRunner] (status,
            & respose
            ResponseContext) 
        {
            //m_jobRunner.tryRunNextJob();

            if status != 0 )
             callback( Error, {} );
            else
            {
                callback( Success, respose->items );
            }
        };

        Request request;
        request.spdl = spdl;

        m_jobRunner->run(
            createAsyncRequestJob(...)
        );

        m_jobRunner->run(
            [=](jobRunner j){j.done();}
        );

        m_jobRunner->run(
            [=](jobRunner j){
                client->sendAsyncRequest( request,  [ basCallback, m_jobRunner ] (auto &&...args) {
                    m_jobRunner.done();
                    basCallback(...args);
                };
            };
        );


    }
};

int main()
{
    std::unique_ptr<RateLimiter> rateLimiter = std::make_unique<TokenBucket>( 1, 1 );
    
    return 0;
}