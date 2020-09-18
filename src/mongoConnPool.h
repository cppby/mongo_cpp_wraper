#ifndef MONGO_CONN_POOL_H
#define MONGO_CONN_POOL_H

#include <string>
#include <memory>
#include "mongoConn.h"
#include "bsonw.h"

namespace mongo {
    enum MONGO_WRITE_TYPE {
        UNACKNOWLEDGED  = 0,    //不确定写入
        ACKNOWLEDGED  = 1,      //1个节点确定写入
    };

    class stMongoConnPool {
        public:
            //mongo 地址， 数据库名
            static bool initPool( const std::string &dbUrl,  const std::string& dbName, uint32_t maxCount );

            static void closePool();

            //获取一个mongo client
            static mongoConnPtr popClient();

            //归还一个client
            static void pushClient( mongoc_client_t *pClient );

            static bool ping();

            static const std::string &getDBName() { return mDBName; }
        private:
        //mongo连接信息 
            static mongoc_uri_t* mUrl;
        //mongo池信息
		    static mongoc_client_pool_t * mPoolImpl;
        //库名
            static std::string mDBName;
    };
    typedef std::shared_ptr<stMongoConnPool> mongoConnPoolPtr;
};

#endif