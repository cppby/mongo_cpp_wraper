#ifndef MONGO_CONN_H
#define MONGO_CONN_H

#include <memory>
#include "mongo.h"
#include "bsonw.h"

namespace mongo {
    //mongon 连接的最小单位
    class stMongoConn {
        friend class stMongoConnPool;
        protected:
            stMongoConn( mongoc_client_t *conn, const std::string &name );
        public:
            ~stMongoConn();
        public:
            mongoCollectionPtr collection( const std::string& name );

            void free();

            mongoc_client_t* getClient(){ return mClient; }

            bool isOk(){ return mClient != NULL; }
        public:
            mongoc_client_t* mClient = NULL;

            std::string mDBName = "";
    };
    typedef std::shared_ptr<stMongoConn> mongoConnPtr;
};

#endif