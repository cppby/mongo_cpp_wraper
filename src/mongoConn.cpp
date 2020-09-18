#include "mongoConn.h"

#include "mongoConnPool.h"

namespace mongo {

stMongoConn::stMongoConn( mongoc_client_t *conn, const std::string &name ) : mClient(conn) {
    mDBName = name;
}

stMongoConn::~stMongoConn() {
    if ( NULL != mClient ) {
        printf("mongo conn is not free "); 
        stMongoConnPool::pushClient( mClient );
    }
}

mongoCollectionPtr stMongoConn::collection( const std::string& name ){
    return mongoCollectionPtr( new stMongoCollection( mongoc_client_get_collection( mClient, mDBName.c_str(), name.c_str() ) ) );
}

void stMongoConn::free(){
    stMongoConnPool::pushClient( mClient );
    mClient = NULL;
}

};