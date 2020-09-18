#include "mongoConnPool.h"


namespace mongo {
    //mongo连接信息 
    mongoc_uri_t* stMongoConnPool::mUrl = NULL;
    //mongo池信息
    mongoc_client_pool_t* stMongoConnPool::mPoolImpl = NULL;
    //库名
    std::string stMongoConnPool::mDBName = "";

    //初始化池子
    bool stMongoConnPool::initPool( const std::string &dbUrl,  const std::string& dbName, uint32_t maxConn ){
        static bool isInit = false;        
        if ( true == isInit ){
            printf("mongo pool has init");
            return false;
        }

        //mongo 初始化
        mongoc_init();

        //建立urlInfo
        mUrl = mongoc_uri_new( dbUrl.c_str() );
		if ( !mUrl ) {
			printf("create mongo url err:%s", dbUrl.c_str());
			return false;
		}

        //设置最大连接数
        if ( !mongoc_uri_set_option_as_int32(mUrl, MONGOC_URI_MAXPOOLSIZE, maxConn) ) {
			printf("mongoc_uri_set_option_as_int32 err");
			return false;
		}

        //设置appName
	    if ( !mongoc_uri_set_appname(mUrl, dbName.c_str()) ) {
	        printf("mongoc_uri_set_appname err:%s", dbName.c_str());
		    return false;
		}

        //初始化Pool句柄
        //设置写入引擎， 目前不设置，默认写入1个节点即返回,不保证落地磁盘
		mPoolImpl = mongoc_client_pool_new(mUrl);
        if ( !mPoolImpl ){
            printf("mongoc_client_pool_new err");
            return false;
        }

        mDBName = dbName;

        return ping();
    }

    void stMongoConnPool::closePool(){
        if (mPoolImpl) {
			mongoc_client_pool_destroy(mPoolImpl);
		}
		if (mUrl) {
			mongoc_uri_destroy(mUrl);
		}
    }

     //获取一个mongo client
    mongoConnPtr stMongoConnPool::popClient(){
        auto c = mongoc_client_pool_pop(mPoolImpl);
		if (!c) {
			return NULL;
		}
		return mongoConnPtr( new stMongoConn(c, mDBName) );  
    }

    void stMongoConnPool::pushClient( mongoc_client_t *pClient ){
        mongoc_client_pool_push( mPoolImpl , pClient );  
    }

    bool stMongoConnPool::ping(){
        auto client = mongoc_client_pool_pop(mPoolImpl);
		if (!client) {
			printf("mongoc_client_pool_new2 err");
			return false;
        }

        bson_t ping = BSON_INITIALIZER;
		bson_error_t error;
		BSON_APPEND_INT32(&ping, "ping", 1);

		bool ret = mongoc_client_command_simple( client, "admin", &ping, NULL, NULL, &error);

        mongoc_client_pool_push( mPoolImpl, client );

        return ret;
    }
};
