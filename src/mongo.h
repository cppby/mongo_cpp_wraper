#ifndef MONGO_H
#define MONGO_H

#include <bson.h>
#include <mongoc.h>
#include <vector>
#include "bsonw.h"

//mongo collection， 游标封装
namespace mongo {

class stMongoCursor;
typedef std::shared_ptr<stMongoCursor> mongoCursorPtr;

//collection
class stMongoCollection {
    friend class stMongoConn;
protected:
    stMongoCollection( mongoc_collection_t* collection );
public:
    ~stMongoCollection();
public:    
//查
    mongoCursorPtr find( const stBson &query, const stBson &opt );

//查找一个
    BsonPtr findOne( const stBson &query, const stBson &opt ); 

//find and modify
	bool findAndModify( const stBson &query, const stBson &value, stBson &result );

//只更行
    bool update( const stBson &query, const stBson &value );

//更新/新增
    bool replace( const stBson &query, const stBson &value ); 

//插入
    bool insert( const stBson &value );

//删除    
    bool remove( const stBson &query );

//统计
    int count( const stBson &query, const stBson &opt );

//自增id
	int64_t incrID( const stBson &query, const std::string &key, uint32_t step = 1);

private:
    mongoc_collection_t* mCollection;   
};
typedef std::shared_ptr<stMongoCollection> mongoCollectionPtr;


//游标
class stMongoCursor {
friend class stMongoCollection;
protected:
    stMongoCursor( mongoc_cursor_t *cursor );
public:
    ~stMongoCursor();

    BsonPtr next();    

    BsonPtr current();

    bool featch( BsonPtr pBson );
private:
    mongoc_cursor_t* mCursor;
}; 

//BulkOperation

};


#endif
