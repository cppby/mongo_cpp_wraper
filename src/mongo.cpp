#include "mongo.h"


namespace mongo {

stMongoCollection::stMongoCollection( mongoc_collection_t* collection ) : mCollection(collection) {

}

stMongoCollection::~stMongoCollection(){
    mongoc_collection_destroy( mCollection );
}

//查
mongoCursorPtr stMongoCollection::find( const stBson &query, const stBson& ops ){
    mongoc_cursor_t* pCursor = mongoc_collection_find_with_opts ( mCollection, query.raw(), ops.raw(), NULL);
    return pCursor != NULL ? mongoCursorPtr( new stMongoCursor( pCursor ) ) : NULL;    
}

//查找一个
BsonPtr stMongoCollection::findOne( const stBson &query, const stBson &ops ){
    mongoCursorPtr pCursor = find( query, ops );
    return pCursor != NULL ? pCursor->next() : NULL;    
}

//只更行
bool stMongoCollection::update( const stBson &query, const stBson &value ){
    bson_error_t err;
    if( !mongoc_collection_update( mCollection, MONGOC_UPDATE_NONE, query.raw(), value.raw(), NULL, &err ) ) {
        printf( "Error: %s\n", err.message );
        return false;
    }
    return true;    
}

//更新/新增
bool stMongoCollection::replace( const stBson &query, const stBson &value ){
    bson_error_t err;
    if( !mongoc_collection_update( mCollection, MONGOC_UPDATE_UPSERT, query.raw(), value.raw(), NULL, &err ) ) {
        printf( "Error: %s\n", err.message );
        return false;
    }
    return true;      
}

//插入
bool stMongoCollection::insert( const stBson &value ){
    bson_error_t err;
    if( !mongoc_collection_insert( mCollection, MONGOC_INSERT_NONE, value.raw(), NULL, &err ) ) {
        printf( "Error: %s\n", err.message );
        return false;
    }
    return true;
}

//删除    
bool stMongoCollection::remove( const stBson &query ){
    bson_error_t err;
    if( !mongoc_collection_remove( mCollection, MONGOC_REMOVE_NONE, query.raw(), NULL, &err ) ) {
        printf( "Error: %s\n", err.message );
        return false;
    }
    return true;        
}

//统计
int stMongoCollection::count( const stBson &query, const stBson &opt ){
    bson_error_t err;
    int64_t result = mongoc_collection_count_documents ( mCollection, query.raw(), opt.raw(), NULL, NULL, &err );
    return ( int )result;
}

bool stMongoCollection::findAndModify( const stBson &query, const stBson &value, stBson &result ) {
    bson_error_t err;
    bson_t reply;
		
	if ( !mongoc_collection_find_and_modify( mCollection, query.raw(), NULL
		, value.raw(), NULL
		, false, true, true
		, &reply, &err )
 	){
        printf( "Error: %s\n", err.message );
		return false;
	}
    result.copy( bson_copy(&reply) );
	return true;
}

int64_t stMongoCollection::incrID( const stBson &query, const std::string &key,  uint32_t step ){
	char sqlStr[128]={0};
	snprintf( sqlStr, sizeof(sqlStr), "{\"$inc\" : {\"%s\":%u}}", key.c_str(), step );

	stBson update( bson_new() );
	if ( !update.initFromJson( sqlStr ) ){
		printf( "init bson from Json[%s] faild", sqlStr );
		return -1;
	}
	
	stBson result(nullptr);
	if ( !findAndModify( query, update, result ) ){
		printf( "init bson from Json[%s] faild", sqlStr );
		return -1;
	}
	
	char valueKey[64]={0};

	snprintf( valueKey, sizeof(valueKey),"value.%s", key.c_str() );

	return result.asInt64( valueKey );
}


/////////////////////////////////////////////////////////////////////////////
//游标
stMongoCursor::stMongoCursor( mongoc_cursor_t *cursor ) : mCursor( cursor ){
}

stMongoCursor::~stMongoCursor(){
    mongoc_cursor_destroy( mCursor );
}

BsonPtr stMongoCursor::next(){
   	const bson_t *doc;

    if( !mongoc_cursor_next( mCursor, &doc ) ) {
        return NULL;
    }
    BsonPtr pBson = BsonPtr( new stBson( bson_copy(doc) ) );  
    return pBson;
}

BsonPtr stMongoCursor::current(){
    const bson_t* doc = mongoc_cursor_current( mCursor );
    if( NULL == doc ){
        return NULL;
    }
    BsonPtr pBson = BsonPtr( new stBson( bson_copy(doc) ) );  
    return pBson;
}

bool stMongoCursor::featch( BsonPtr pBson ){
    if ( NULL == pBson ){
        return false;
    }

   	const bson_t *doc;
    if( !mongoc_cursor_next( mCursor, &doc ) ) {
        return false;
    }
    pBson->copy( bson_copy(doc) );
    return true;
 }

}
